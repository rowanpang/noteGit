#undef TRACE_SYSTEM
#define TRACE_SYSTEM xhr_test

/*
 *$ ll /sys/kernel/debug/tracing/events/xhr_test/
 *-rw-r--r-- 1 root root 0 Jul  3 23:19 enable
 *-rw-r--r-- 1 root root 0 Jul  3 23:19 filter
 *drwxr-xr-x 2 root root 0 Jul  3 23:19 te_test
 */

#if !defined(_TRACE_TE_TEST_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_TE_TEST_H

#include <linux/tracepoint.h>

TRACE_EVENT(te_test,
    TP_PROTO(int num),
    TP_ARGS(num),
    TP_STRUCT__entry(
	__field(int, output)
	__field(int, count)
    ),
    TP_fast_assign(
	__entry->count++;
	__entry->output = num;
    ),
    TP_printk("count=%d output=%d", __entry->count, __entry->output)
);

#endif /* _TRACE_TE_TEST_H */

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE module_traceevent

#include <trace/define_trace.h>
/*
 *————————————————
 *版权声明：本文为CSDN博主「xhr_embedded」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
 *原文链接：https://blog.csdn.net/u012849539/java/article/details/106771143
 */
