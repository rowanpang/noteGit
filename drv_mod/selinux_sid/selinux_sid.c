#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sem.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/pid_namespace.h>
#include <linux/err.h>
#include <linux/selinux.h>

struct task_security_struct {
	u32 osid;		/* SID prior to last execve */
	u32 sid;		/* current SID */
	u32 exec_sid;		/* exec SID */
	u32 create_sid;		/* fscreate SID */
	u32 keycreate_sid;	/* keycreate SID */
	u32 sockcreate_sid;	/* fscreate SID */
};

static pid_t spid = 1;
static pid_t epid = 1;
static char *cpid = NULL;
static unsigned int apid[10]={0};

#define RANGE_FLAG 0x80000000
#define RANGE_START_BITS 16
#define RANGE_START_MASK 0x7fff
#define RANGE_END_MASK 0xffff
#define RANGE_START(digit) ((((digit) & RANGE_START_MASK) << RANGE_START_BITS) | RANGE_FLAG)
#define RANGE_END(digit,range) (((digit) & RANGE_END_MASK) | range)

module_param(spid,int,0700);
module_param(epid,int,0700);

module_param(cpid,charp,0700);

static int get_num(char **pid)
{
	int tmp = 0;
	while(isdigit(**pid)){
		tmp = (**pid - '0') + tmp*10;
		(*pid)++;
	}
	return tmp;
}

static int parse_cpid(char *cpid)
{
	if(cpid == NULL)
		return 0;
	char *pid = kzalloc(strlen(cpid)+1,GFP_KERNEL);
	strcpy(pid,cpid);
    printk(KERN_DEBUG "--%s\n",pid);
	int cur = 0;
	while(*pid != '\0'){
		if(isdigit(*pid)){
			apid[cur]=get_num(&pid);
			cur ++;
		}else if(*pid == '['){
			while(!isdigit(*pid))
				pid++;
			apid[cur] = RANGE_START(get_num(&pid));

			while(!isdigit(*pid))
				pid++;
			apid[cur] = RANGE_END(get_num(&pid),apid[cur]);

			while(*pid == ' ' || *pid == ',')
				pid++;
		}else if(*pid == ']'){
			pid++;
			cur++;
		}else{
			pid++;
		}
	}	
	int tmp = 0;
	for(tmp = 0;tmp < 10;tmp++){
		printk(KERN_DEBUG "cur:%d,pid:%#x\n",tmp,apid[tmp]);
	}

	kfree(pid);
	return 0;
}


typedef struct task_struct* (*pfind_task_by_pid_ns)(pid_t nr,struct pid_namespace *ns);
static int __init  selinux_sid_init(void)
{
	struct task_struct *tsk;
	bool se_enable = 0;
	pid_t tpid = 0;
    printk(KERN_DEBUG "---------show selinux sid for process\n");

	se_enable = selinux_is_enabled();
	if(!se_enable){
		printk(KERN_DEBUG "selinux disabled,do nothing\n");
		goto EXIT;
	}

	parse_cpid(cpid);

	pfind_task_by_pid_ns ftbpn=(pfind_task_by_pid_ns)0xffffffff81091fa0;
	
	for(tpid=spid;tpid<epid+1;tpid++){
		tsk = ftbpn(tpid,&init_pid_ns);
		if(IS_ERR_OR_NULL(tsk)){
			printk(KERN_DEBUG "pid:%d tsk error:%#x\n",tpid,PTR_ERR(tsk));
			continue;
		}
	
		struct task_security_struct *tsec =(struct task_security_struct*)tsk->real_cred->security;
		if(IS_ERR_OR_NULL(tsec)){
			printk(KERN_DEBUG "pid:%d tsec error :%#x\n",tpid,PTR_ERR(tsec));
			continue;
		}

		printk(KERN_DEBUG "pid:%d,sid:%d,exec_sid:%d\n",tpid,tsec->sid,tsec->exec_sid);

	}

    printk(KERN_DEBUG "--------end selinux sid for process\n");
EXIT:
    return 0;
}

static void __exit selinux_sid_exit(void)
{
      printk(KERN_DEBUG "selinux_sid exit\n");
}

MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("selinux task_security_struct list"); 
MODULE_AUTHOR("Rowan Pang"); 

module_init(selinux_sid_init); 
module_exit(selinux_sid_exit);
