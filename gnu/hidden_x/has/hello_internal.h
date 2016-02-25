
extern void print();

# define hidden __attribute__ ((visibility ("hidden")))
# define hidden_proto(fct) __hidden_proto (fct, fct##_internal)
# define __hidden_proto(fct, internal)	\
     extern __typeof__ (fct) internal;	\
     extern __typeof__ (fct) fct __asm (#internal) hidden;
# define hidden_def(fct)\
     asm (".global " #fct "\n.set " #fct ", " #fct "_internal");

hidden_proto(print);
