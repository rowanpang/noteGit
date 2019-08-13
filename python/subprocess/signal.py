#!/usr/bin/python

import signal
TIMEOUT = 2 # number of seconds your want for timeout

# class InputTimeOutError(Exception):
    # pass

# def interrupted(signum, frame):
    # raise InputTimeOutError

# signal.signal(signal.SIGALRM, interrupted)
signal.signal(signal.SIGALRM, lambda x,y: raise_(InputTimeOutError(Exception)))

def input():
    try:
            print 'You have %d seconds to type in your stuff...'%(TIMEOUT)
            foo = raw_input()
            return foo
    except Exception as e:
            # timeout
            print e
            return 'timeout---'

# set alarm
signal.alarm(TIMEOUT)
s = input()
# disable the alarm after success
signal.alarm(0)
print 'You typed', s
