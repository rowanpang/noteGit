#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <strings.h>
#include <unistd.h>
#include <assert.h>


typedef struct Ss_slist_node_t {
	struct Ss_slist_node_t * volatile next;
} Ss_slist_node;

typedef Ss_slist_node Plat_fifo_node;

typedef struct Plat_fifo_t {
	Plat_fifo_node *head;
	Plat_fifo_node dummy;
	Plat_fifo_node *tail;
} Plat_fifo;

static void plat_fifo_init(Plat_fifo *fifo_i)
{
	fifo_i->dummy.next = NULL;
	fifo_i->head = &fifo_i->dummy;
	fifo_i->tail = &fifo_i->dummy;
}

typedef int bool;

#define false 0
#define true 1

#define NOTE_INTERVAL 1000
#define USEC 10000

#define SMP_LOCK 	"lock\n\t"
#define plat_define_cmpxchgp(NAME, TYPE) 	\
static __inline__ bool 						\
NAME(TYPE * volatile * pointer_i, 			\
	const TYPE * old_i, 					\
	const TYPE * new_i) 					\
{ 											\
	TYPE *result; 							\
	__asm__ __volatile__ ( 					\
		SMP_LOCK 							\
		"cmpxchgq %1, %2" 					\
		: "=a"(result) 						\
		: "q"(new_i), "m"(*pointer_i), "0"(old_i) \
		: "memory"); 						\
 											\
	return result == old_i; 				\
}

plat_define_cmpxchgp(plat_fifo_node_cmpxchg, Plat_fifo_node)

static void plat_fifo_add_tail(Plat_fifo *fifo_i, Plat_fifo_node *node_i)
{
	Plat_fifo_node *oldtail;

	node_i->next = NULL;
	do {
		oldtail = fifo_i->tail;
	} while ( !plat_fifo_node_cmpxchg(&fifo_i->tail, oldtail, node_i));
	
	oldtail->next = node_i;
}

static bool plat_fifo_empty(Plat_fifo *fifo_i)
{
	return (fifo_i->head == &fifo_i->dummy && fifo_i->head->next == NULL);
	//return (fifo_i->head->next == NULL);
}

static Plat_fifo_node *plat_fifo_remove_head(Plat_fifo *fifo_i)
{
	Plat_fifo_node *node;
	Plat_fifo_node *prev_node;
	bool done = false;
	
	do {
		prev_node = node;
		node = NULL;
	
		if (plat_fifo_empty(fifo_i)) {
			done = true;
		} else {
			node = fifo_i->head;
			//assert(node->next);
			//while(!node->next);
			if (!node->next) {
				printf("next NULL, fifo:%p, head:%p, tail:%p, dummy:%p, node:%p, prev_node:%p\n",fifo_i, fifo_i->head, fifo_i->tail, &fifo_i->dummy, node, prev_node);
				while(!node->next); 			//fifo_i->head 的load, before 了上一次 add_tail oldtail->next = node_i;  怎么fix????
			}
			fifo_i->head = node->next;
			
			if (node == &fifo_i->dummy) {
				plat_fifo_add_tail(fifo_i, node);
			} else {
				done = true;
			}
		}
	} while(!done);

	return node;
}

void *do_add(void *arg)
{
	Plat_fifo *fifo = (Plat_fifo *)arg;
	Plat_fifo_node *node;
	unsigned long i = 0;

	do {
			node = malloc(sizeof(Plat_fifo_node));
			bzero(node,sizeof(Plat_fifo_node));
			plat_fifo_add_tail(fifo, node);
			i++;
			if (i % NOTE_INTERVAL== 0) {
				printf("add tail: %ld\n",i);
				usleep(USEC);
			}
	} while(1);
}

void *do_remove(void *arg)
{
	Plat_fifo *fifo = (Plat_fifo *)arg;
	Plat_fifo_node *node = NULL;
	unsigned long i = 0;

	do {
			node = plat_fifo_remove_head(fifo);
			if (node) {
				free(node);
				i++;
				if (i % NOTE_INTERVAL== 0) {
					printf("remove head: %ld\n",i);
					usleep(USEC);
				}
			}
	} while(1);
}

int main(void)
{
	pthread_t adder;
	pthread_t remover;
	
	Plat_fifo mainfifo;	
	plat_fifo_init(&mainfifo);

	pthread_create(&adder, NULL, do_add, &mainfifo);
	pthread_create(&remover, NULL, do_remove, &mainfifo);

	pthread_join(adder,NULL);
	pthread_join(remover,NULL);
}
