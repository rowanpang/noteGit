#include <stdio.h>
#include <string.h>

/*
 *echo probe_libmsg > set_event
 *echo 1 > tracing_on
 */

int strcmp(const char* s1,const char* s2)
{
    int i = 9;
    /*
     *$ perf probe /root/noteGit/gnu/perf/libmsg.so strcmp i
     *	    probe 发生在func 入口,此时i还没有正确赋值. trace 中的显示不正确.
     *$ cat uprobe_events
     *    p:probe_libmsg/strcmp /root/noteGit/gnu/perf/libmsg.so:0x000000000000061a i=-4(%bp):s32
     */

    /*
     *
     *$ perf probe /root/noteGit/gnu/perf/libmsg.so strcmp%return
     *$ cat uprobe_events
     *    r:probe_libmsg/strcmp /root/noteGit/gnu/perf/libmsg.so:0x000000000000060a
     */


/*
 *        $ perf probe /root/noteGit/gnu/perf/libmsg.so strcmp+0x17 i
 *        $ cat uprobe_events
 *            p:probe_libmsg/strcmp /root/noteGit/gnu/perf/libmsg.so:0x0000000000000621 i=-4(%bp):s32
 *
 *        $ cat trace
 *            # tracer: nop
 *            #
 *            # entries-in-buffer/entries-written: 1/1   #P:4
 *            #
 *            #                              _-----=> irqs-off
 *            #                             / _----=> need-resched
 *            #                            | / _---=> hardirq/softirq
 *            #                            || / _--=> preempt-depth
 *            #                            ||| /     delay
 *            #           TASK-PID   CPU#  ||||    TIMESTAMP  FUNCTION
 *            #              | |       |   ||||       |         |
 *                        main-31998 [001] d... 4352086.404811: strcmp: (0x7ff730de9621) i=9
 */

	/*
	 *$ perf probe /root/noteGit/gnu/perf/libmsg.so strcmp+0x17 s1:string i:s32
	 *$ cat uprobe_events
	 *    p:probe_libmsg/strcmp /root/noteGit/gnu/perf/libmsg.so:0x0000000000000621 s1_string=+0(-24(%bp)):string i_s32=-4(%bp):s32
	 *$ echo probe_libmsg > set_event
	 *$ cat trace
	 *    # tracer: nop
	 *    #
	 *    # entries-in-buffer/entries-written: 4/4   #P:4
	 *    #
	 *    #                              _-----=> irqs-off
	 *    #                             / _----=> need-resched
	 *    #                            | / _---=> hardirq/softirq
	 *    #                            || / _--=> preempt-depth
	 *    #                            ||| /     delay
	 *    #           TASK-PID   CPU#  ||||    TIMESTAMP  FUNCTION
	 *    #              | |       |   ||||       |         |
	 *                main-2877  [002] d... 4357331.193189: strcmp: (0x7f255a127621) s1_string="askdf" i_s32=9
	 *                main-3006  [002] d... 4357348.321079: strcmp: (0x7f2efd7d0621) s1_string="askdf" i_s32=9
	 */


    printf("preload invoke always return 0\n");
    return 0;
}
