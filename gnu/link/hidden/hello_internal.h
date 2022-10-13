
# define hidden __attribute__ ((visibility ("hidden")))
# define hidden_proto(fct) __hidden_proto (fct, fct##_internal)
# define __hidden_proto(fct, internal)	\
     extern __typeof__ (fct) internal;	\
     extern __typeof__ (fct) fct __asm (#internal);             /*hidden;		/*__asm: 汇编链接时使用的真正名字为fct##_internal instead,gcc -S hello.c -o hello.s*/

# define hidden_def(fct)\
     asm (".global " #fct "\n.set " #fct ", " #fct "_internal");	/*define the fct symbol as global,so can referenced by other module*/


extern void print();
hidden_proto(print);
