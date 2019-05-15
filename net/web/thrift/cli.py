#!/usr/bin/python
import sys
sys.path.append('./gen-py')

from helloworld import HelloWorld
from helloworld.ttypes import *
from helloworld.constants import *

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

try:
  # Make socket
  transport = TSocket.TSocket('127.0.0.1', 30303)

  # Buffering is critical. Raw sockets are very slow
  transport = TTransport.TBufferedTransport(transport)

  # Wrap in a protocol
  protocol = TBinaryProtocol.TBinaryProtocol(transport)

  # Create a client to use the protocol encoder
  client = HelloWorld.Client(protocol)

  # Connect!
  transport.open()

  client.ping()
  print "ping()"

  msg = client.sayHello()
  print "msg:%s" % msg
  msg = client.sayMsg(HELLO_YK)
  print "msg:%s" % msg

  transport.close()

except Thrift.TException, tx:
  print "%s" % (tx.message)
