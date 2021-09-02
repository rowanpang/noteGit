#!/bin/bash
function doTest()
{
    echo "test for: $cmd"
    mtraceLog="${PWD}/mtrace.log.${cmd#./}"
    export MALLOC_TRACE=$mtraceLog
    $cmd
    mtrace $cmd $mtraceLog
    unset MALLOC_TRACE
}


cmd="./bad"
doTest

echo;echo


cmd="./good"
doTest
