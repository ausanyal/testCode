#!/usr/bin/python

import collections, os, re, time, random
from datetime import datetime

f1 = open('testinput.txt', 'w+')
f2 = open('testinput2.txt', 'w+')

dict_python_v4 = {}
dict_python_v6 = {}

def get_hw_weight(af):
    if af == 'v4':
        return 1
    else:
        return 4

def update_dict(af, d, key, val):
    if (key > 0):
        if key in d.keys():
            d[key] = d[key] + (val * get_hw_weight(af))
        else:
            d[key] = val * get_hw_weight(af)

#list = [12, 9, 8, 16, 15, 16]
v4list = []
for num in range(1, 1500+1):
#for v4p in list:
    v4p = random.randrange(1, 32)
    str1 = "v4 " + "add " + str(v4p) + " 1"
    str2 = "v4 " + "del " + str(v4p) + " 1"
    f1.write(str1)
    f1.write('\n')
    f2.write(str1)
    f2.write('\n')
    update_dict('v4', dict_python_v4, v4p,   1)
    update_dict('v4', dict_python_v4, v4p-1, 2)
    update_dict('v4', dict_python_v4, v4p-2, 4)
    update_dict('v4', dict_python_v4, v4p-3, 8)

print dict_python_v4

for num in range(1, 1500+1):
#for v4p in list:
    v6p = random.randrange(1, 128)
    str1 = "v6 " + "add " + str(v6p) + " 1"
    str2 = "v6 " + "del " + str(v6p) + " 1"
    f1.write(str1)
    f1.write('\n')
    f2.write(str1)
    f2.write('\n')
    update_dict('v6', dict_python_v6, v6p,   1)
    update_dict('v6', dict_python_v6, v6p-1, 2)
    update_dict('v6', dict_python_v6, v6p-2, 4)
    update_dict('v6', dict_python_v6, v6p-3, 8)

print dict_python_v6

f1.close()

a = datetime.now()
os.system("./run_trie_tile.sh")
b = datetime.now()
c = b-a

v4d_ccplus = {}
f2 = open("ccplus.out", "r")
lines = f2.readlines()
for line in lines:
    p = re.compile('.*v4.*prefix_len: ([0-9]+).*weight: ([0-9]+)')
    n = p.match(line)
    if n is not None:
        v4d_ccplus[int(n.group(1))] = int(n.group(2))
        #print "setting v4d_ccplus d[%d] = %d" % (int(n.group(1)), int(n.group(2)))

v6d_ccplus = {}
f2 = open("ccplus.out", "r")
lines = f2.readlines()
for line in lines:
    p = re.compile('.*v6.*prefix_len: ([0-9]+).*weight: ([0-9]+)')
    n = p.match(line)
    if n is not None:
        v6d_ccplus[int(n.group(1))] = int(n.group(2))

ov4d_ccplus = collections.OrderedDict(sorted(v4d_ccplus.items())) 
ov4d_python = collections.OrderedDict(sorted(dict_python_v4.items())) 
ov6d_ccplus = collections.OrderedDict(sorted(v6d_ccplus.items())) 
ov6d_python = collections.OrderedDict(sorted(dict_python_v6.items())) 
print "****************  V4  *******************"
print ov4d_python
print ov4d_ccplus
print "****************************************"
print "****************  V6  *******************"
print ov6d_python
print ov6d_ccplus
print "****************************************"

shared_items = set(ov4d_python.items()) & set(ov4d_ccplus.items())
print "V4: py_len1: %d / ccplus_len2: %d / shared_len3: %d " % (len(ov4d_python), len(v4d_ccplus), len(shared_items))
if len(ov4d_python) != len(shared_items) or len(ov4d_ccplus) != len(shared_items):
    print "\n\nERROR\n"
else:
    print "\n\nSUCCESS\nccplus code took: ",
    print c
    print "\n\n"

shared_items = set(ov6d_python.items()) & set(v6d_ccplus.items())
print "v6: py_len1: %d / ccplus_len2: %d / shared_len3: %d " % (len(ov6d_python), len(v6d_ccplus), len(shared_items))
if len(ov6d_python) != len(shared_items) or len(v6d_ccplus) != len(shared_items):
    print "\n\nERROR\n"
else:
    print "\n\nSUCCESS\nccplus code took: ",
    print c
    print "\n\n"

f2.close()
