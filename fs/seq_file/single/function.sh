#!/bin/bash
debugfs=/sys/kernel/debug

echo nop > /tracing/current_tracer

echo 0 > /tracing/tracing_on

echo 21300 > /tracing/set_ftrace_pid

echo function_graph > /tracing/current_tracer

#replace test_proc_show by your function name

echo test_proc_show > /tracing/set_graph_function

echo 1 > /tracing/tracing_on

exec ""

