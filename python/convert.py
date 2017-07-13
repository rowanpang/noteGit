#!/usr/bin/python
import binascii

# [0x53, 0x21, 0x6A] -> '\x53\x21\x6a'
x = [0x53, 0x21, 0x6A]
y = str(bytearray(x))
print '''[0x53, 0x21, 0x6A] -> '\\x53\\x21\\x6a'  %s''' %y

# '\x53\x21\6a' -> [0x53, 0x21, 0x6A]
x = y
y = list(x)                     #list from str
print y
y = map(ord,list(x))            #help(map)  return a list
print y

# '53216A' -> '\x53\x21\x6A'
x = '53216A'
y =  binascii.unhexlify(x)
print y
y = bytearray.fromhex(x)
print str(y)



# a = ['word1, 23, 12','word2, 10, 19','word3, 11, 15']
#                       convert to 
# b = [['word1',23,12],['word2', 10, 19],['word3', 11, 15]]

input = ['word1, 23, 12','word2, 10, 19','word3, 11, 15']
output = []
for item in input:
    items = item.split(',')
    output.append([items[0], int(items[1]), int(items[2])])

print output
