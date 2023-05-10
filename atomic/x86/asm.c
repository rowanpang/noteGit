#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define gettid() syscall(__NR_gettid)
#define THREADS 50

#define MAX_ERRNO    4095
#define IS_ERR_VALUE(x) ( (unsigned long)(void *)(x) >= (unsigned long)-MAX_ERRNO )

#if defined(__powerpc64__)
#define HMT_very_low()   asm volatile("or 31,31,31   # very low priority")
#define HMT_low()        asm volatile("or 1,1,1      # low priority")
#define HMT_medium_low() asm volatile("or 6,6,6      # medium low priority")
#define HMT_medium()     asm volatile("or 2,2,2      # medium priority")
#define HMT_medium_high() asm volatile("or 5,5,5      # medium high priority")
#define HMT_high()       asm volatile("or 3,3,3      # high priority")

#define LOCK_TOKEN 1
typedef struct {
        volatile unsigned int slock;
} arch_spinlock_t;

arch_spinlock_t gspinlock = { .slock = 0 };
static inline unsigned long __arch_spin_trylock(arch_spinlock_t *lock)
{
        unsigned long tmp, token;

        token = LOCK_TOKEN;
        __asm__ __volatile__(
"1:     lwarx  			%0,0,%2,1\n\
        cmpwi           %0,0\n\
        bne-            2f\n\
        stwcx.          %1,0,%2\n\
        bne-            1b\n\
       	isync \n\
2:"
        : "=&r" (tmp)
        : "r" (token), "r" (&lock->slock)
        : "cr0", "memory");

        return tmp;
}

static inline void arch_spin_lock(arch_spinlock_t *lock)
{
        while (1) {
                if ((__arch_spin_trylock(lock) == 0))
                        break;
                do {
                        HMT_low();
                } while ((lock->slock != 0));
                HMT_medium();
        }
}

static inline void arch_spin_unlock(arch_spinlock_t *lock)
{
	__asm__ __volatile__("# arch_spin_unlock\n\t" "lwsync" : : :"memory");
	lock->slock = 0;
}

struct uint_128 {
	unsigned long u64[2];
};

typedef struct uint_128 u128;
typedef unsigned long uint64;

static inline u128 ppc64_compare_and_swap_u128(volatile u128* var_ptr, u128 oldval, u128 newval)
{
	u128 ret;
    register volatile uint64 prev0 __asm__("r10");
    register volatile uint64 prev1 __asm__("r11");
#if 0
    register volatile uint64 newv0 __asm__("r8") = (newval.u128 >> 64);
    register volatile uint64 newv1 __asm__("r9") = newval.u128;
    uint64 oldv0 = (oldval.u128 >> 64);
    uint64 oldv1 = oldval.u128;
#else
    register volatile uint64 newv0 __asm__("r8") = newval.u64[1];
    register volatile uint64 newv1 __asm__("r9") = newval.u64[0];
    uint64 oldv0 = oldval.u64[1];
    uint64 oldv1 = oldval.u64[0];
#endif

/*
lqarx:
	lqarx RTp,RA,RB,EH
	In Little-Endian mode,
		the even-numbered GPR is loaded with the byte-reversed doubleword from storage addressed by EA+8 and
		the odd-numbered GPR is loaded with the byte-reversed doubleword addressed by EA.
	RTp should be even.
*/
    __asm__ __volatile__("\n"
                         "\tlwsync\n"
                         "1:\n"
                         "\tlqarx %[pv0], 0, %[ptr],1\n"
                         "\tcmpd %[pv0], %[ov0]\n"
                         "\tbne- 2f\n"
                         "\tcmpd %[pv1], %[ov1]\n"
                         "\tbne- 2f\n"
                         "\tstqcx. %[nv0], 0, %[ptr]\n"
                         "\tbne- 1b\n"
                         "\tisync\n"
                         "2:\n"
                         : [pv0] "+&r"(prev0), [pv1] "+&r"(prev1)
                         : [ptr] "r"(var_ptr), [ov0] "r"(oldv0), [ov1] "r"(oldv1),
                           [nv0] "r"(newv0), [nv1] "r"(newv1)
                         : "memory", "cc");

	ret.u64[1] = prev0;
	ret.u64[0] = prev1;
    return ret;
}

#define ULOCK_0 0x11223344,
#define ULOCK_1 0x55667788,

#define LOCK_0 0x88112233,
#define LOCK_1 0x44556677,
//u128 global = { 	will bus error, need 16 aligned Ref stqcx.
u128 global __attribute__((aligned(16))) = {
			.u64[0]=0x11223344,
			.u64[1]=0x55667788,
		};
u128 *pglobal = &global;
#endif

int gbuiltinlock = 0;
void builtinlock()
{
	int expect;
	int desired;
	int ret = 0;
	do {
                expect = 0;
                desired = 1;
		ret = __atomic_compare_exchange_n(&gbuiltinlock,&expect,desired,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
		if(ret) {
			printf("buildin gotlock, tid:%d, builtinlock: %#p, %#lx\n",gettid(),&gbuiltinlock,gbuiltinlock);
			break;
		}
                //printf("trying again, tid:%d, glock:%#lx\n",gettid(),gbuiltinlock);
	} while(1);
}

void builtinunlock()
{
	int expect;
	int desired;
	int ret = 0;

	do {
                expect = 1;
                desired = 0;
		ret = __atomic_compare_exchange_n(&gbuiltinlock,&expect,desired,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
		if(ret) {
			//printf("buildin unlock, tid:%d, builtinlock:%#p, %#lx\n",gettid(),&gbuiltinlock,gbuiltinlock);
			break;
		}
	} while(1);
}


void mylock()
{
	builtinlock();
	return;
#if defined(__powerpc64__)
	//arch_spin_lock(&gspinlock);
	//return;
	u128 old={ 									//expect ulock;
				.u64[0]=0x11223344,
				.u64[1]=0x55667788
			};

	u128 new={ 									//swap to lock
				.u64[0]=0x88112233,
				.u64[1]=0x44556677
			};

	u128 prev; 									//prev value

	do {
		prev = ppc64_compare_and_swap_u128(pglobal, old, new);
		if (prev.u64[0] == old.u64[0] && prev.u64[1] == old.u64[1]) {
			break;  	//got lock
		}
		//usleep(1);
    	//printf("trying lock.. tid:%d\n",gettid());
	} while(1);

	//printf("lock prev:%#lx,%#lx, now:%#lx,%#lx\n",prev.u64[0],prev.u64[1],global.u64[0],global.u64[1]);
#endif
}

void myunlock()
{
	builtinunlock();
	return;
#if defined(__powerpc64__)
	//arch_spin_unlock(&gspinlock);
	//return;
	u128 old={ 									//expect lock;
				.u64[0]=0x88112233,
				.u64[1]=0x44556677
			};

	u128 new={ 									//swap to unlock
				.u64[0]=0x11223344,
				.u64[1]=0x55667788
			};

	u128 prev; 									//prev value

	do {
		prev = ppc64_compare_and_swap_u128(pglobal, old, new);
		if (prev.u64[0] == old.u64[0] && prev.u64[1] == old.u64[1]) {
			break;  	//unlock ok
		}
		//usleep(1000);
    	printf("trying unlock.. tid:%d\n",gettid());
	} while(1);

	//printf("unlock prev:%#lx,%#lx, now:%#lx,%#lx\n",prev.u64[0],prev.u64[1],global.u64[0],global.u64[1]);
#endif
}

#if defined(__powerpc64__)
void simpletest()
{
	u128 old={
				.u64[0]=0x11223344,
				.u64[1]=0x55667788
			};

	u128 new={
				.u64[0]=0x88112233,
				.u64[1]=0x44556677
			};

	u128 prev;

	prev = ppc64_compare_and_swap_u128(pglobal, old, new);

	printf("prev:%#lx,%#lx, now:%#lx,%#lx\n",prev.u64[0],prev.u64[1],global.u64[0],global.u64[1]);
}
#endif

static inline void * ERR_PTR(long error)
{
    return (void *) error;
}

static inline long PTR_ERR(const void *ptr)
{
    return (long) ptr;
}

static inline int IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)ptr);
}

void* threadfn(void *arg)
{
    unsigned long idx;
    int urand;
    unsigned long tmp = 0;
    unsigned long i = 0;

    while(1) {
		tmp=0;
        mylock();
        idx = (unsigned long) arg;
        urand = 1000*(rand()%900 + 100); //random sleep [100,1000)ms
        printf("gotlock: idx:%03ld,tid:%d,parent:%d,us:%d \n",idx,gettid(),getppid(),urand);
		do {
				i=idx*urand;
				i*=idx;
				i*=idx;
				i*=idx;
				i*=idx;
				tmp++;
		} while(tmp < 0xffffff);

        printf("prepare unlock: idx:%03ld,tid:%d,parent:%d,us:%d \n",idx,gettid(),getppid(),urand);
        printf("\n\n\n");
        myunlock();
    }

    return ERR_PTR(0);
}

int main(int argc, char** argv)
{
	int ret;
	unsigned long i, *status[THREADS];
	pthread_t threads[THREADS];
	printf("in main:%d,parent:%d \n",getpid(),getppid());

	/* Spawn all testing threads */
    for (i = 0; i < THREADS; i++) {
        if (pthread_create(&threads[i], NULL, threadfn, (void *)i)) {
            perror("pthread_create() failed");
            ret = -1;
            goto fail;
        }
    }

    //sleep(100);
	while(1);

    for (i = 0; i < THREADS; i++) {
        pthread_cancel(threads[i]);
        printf("in main join thidx:%03ld\n",i);

        if (pthread_join(threads[i], (void **)&(status[i]))) {
            perror("pthread_join() failed");
            ret = -1;
            goto fail;
        }

        if (status[i] == PTHREAD_CANCELED){
            printf("thread %03ld caceled\n",i);
            continue;
        } else if (IS_ERR(status[i])) {
            printf("%ldth thread failed to join with %ld status\n", i, PTR_ERR(status[i]));
            ret = -1;
            goto fail;
        }
    }

    ret = 0;

fail:
    return ret;
}
