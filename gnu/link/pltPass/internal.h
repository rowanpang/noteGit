#include "hiddenMacro.h"

extern void show(char* msg);
hidden_proto(show)

extern void show2(char* msg);
hidden_proto(show2)
