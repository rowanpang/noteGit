#!/bin/sh

tracefs="/sys/kernel/debug/tracing"

echo "current pid:$$"

echo 0 > $tracefs/tracing_on
echo '' > $tracefs/trace

echo nop > $tracefs/current_tracer              #without function trcee,just the event log,


echo 'signal:*' > $tracefs/set_event
#echo 'sig==20' > $tracefs/events/signal/signal_deliver/filter
#echo 'sig==17' > $tracefs/events/signal/signal_generate/filter

echo '0' > $tracefs/events/signal/signal_deliver/filter
echo '0' > $tracefs/events/signal/signal_generate/filter

echo $$ > $tracefs/set_event_pid
echo $$ > $tracefs/set_ftrace_pid


echo 1 > $tracefs/tracing_on

sleep 1
exec ./sigtstp


#after stop,
#cat $tracefs/trace
