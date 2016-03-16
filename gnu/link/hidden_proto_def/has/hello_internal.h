
extern void print();

# define hidden __attribute__ ((visibility ("hidden")))
# define hidden_proto(fct) __hidden_proto (fct, fct##_internal)
# define __hidden_proto(fct, internal)	\
     extern __typeof__ (fct) internal;	\
     extern __typeof__ (fct) fct __asm (#internal) hidden;		/*means:fct is hidden and in asm use fct##_internal instead,gcc -S hello.c -o hello.s*/ 

# define hidden_def(fct)\
     asm (".global " #fct "\n.set " #fct ", " #fct "_internal");	/*define the fct symbol as global,so can referenced by other module*/

hidden_proto(print);
