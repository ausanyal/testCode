#!/usr/bin/python

import collections, os, re, time, random
from datetime import datetime

f1 = open('testinput.txt', 'w+')
f3 = open('testinput2.txt', 'w+')
f2 = open('gen_result.txt', 'w+')

v4d_python = {}
v4d_python = {}

def get_hw_weight(af):
    if af == 'v4':
        return 1
    else:
        return 4

def update_dict(af, d, key, val):
    if (key > 0):
        if key in d.keys():
            d[key] = d[key] + val
        else:
            d[key] = val

#list = [12, 9, 8, 16, 15, 16]
v4list = []
for num in range(1, 10+1):
#for v4p in list:
    v4p = random.randrange(1, 32)
    str1 = "v4 " + "add " + str(v4p) + " 1"
    str2 = "v4 " + "del " + str(v4p) + " 1"
    f1.write(str1)
    f1.write('\n')
    f3.write(str1)
    f3.write('\n')
    update_dict('v4', v4d_python, v4p,   1)
    update_dict('v4', v4d_python, v4p-1, 2)
    update_dict('v4', v4d_python, v4p-2, 4)
    update_dict('v4', v4d_python, v4p-3, 8)

for num in range(1, 10+1):
#for v4p in list:
    v6p = random.randrange(1, 128)
    str1 = "v6 " + "add " + str(v6p) + " 1"
    str2 = "v6 " + "del " + str(v6p) + " 1"
    f1.write(str1)
    f1.write('\n')
    f3.write(str1)
    f3.write('\n')
    update_dict('v6', v6d_python, v6p,   1)
    update_dict('v6', v6d_python, v6p-1, 2)
    update_dict('v6', v6d_python, v6p-2, 4)
    update_dict('v6', v6d_python, v6p-3, 8)

f1.close()

ov4d_python = collections.OrderedDict(sorted(v4d_python.items())) 
ov6d_python = collections.OrderedDict(sorted(v6d_python.items())) 
#os.system("cat testinput.txt")
a = datetime.now()
os.system("./run_trie_tile.sh")
b = datetime.now()
c = b-a

v4d_ccplus = {}
f3 = open("ccplus.out", "r")
lines = f3.readlines()
for line in lines:
    p = re.compile('.*prefix_len: ([0-9]+).*weight: ([0-9]+)')
    n = p.match(line)
    v4d_ccplus[int(n.group(1))] = int(n.group(2))
    #print "setting v4d_ccplus d[%d] = %d" % (int(n.group(1)), int(n.group(2)))

ov4d_ccplus = collections.OrderedDict(sorted(v4d_ccplus.items())) 
print "*************************"
print v4d_python
print v4d_ccplus
print "*************************"
shared_items = set(v4d_python.items()) & set(v4d_ccplus.items())
print "len1: %d / len2: %d / len3: %d " % (len(v4d_python), len(v4d_ccplus), len(shared_items))
if len(v4d_python) != len(shared_items) or len(v4d_ccplus) != len(shared_items):
    print "\n\nERROR\n"
else:
    print "\n\nSUCCESS\nccplus code took: ",
    print c
    print "\n\n"

f2.close()
f3.close()
