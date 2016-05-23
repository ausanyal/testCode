#!/usr/bin/python

import sys
import collections
from decimal import Decimal

f = open('input', 'r')

lines = f.readlines()

nv = 0
ne = 0
d = {}
for line in lines:
    if ne == 0:
        nv,ne = line.split(' ')
    else:
        u,v,e = map(int, line.split(' '))
        if u not in d.keys():
            d[u] = {}
            d[u]['ud'] = {}
        if e not in d[u]['ud'].keys():
            d[u]['ud'][e] = []
        d[u]['ud'][e].append(v) 
        d[u]['od'] = collections.OrderedDict(sorted(d[u]['ud'].items()))

def find_smallest_cut(pd):
    le_key = Decimal('Infinity')

    ct = 0
    # we need to explore a new edge per iter
    # v cannot point to an existing vertex already explored
    for u in pd.keys():
        if u == 500:
            continue
        if u not in d.keys():
            #print "u: ", 6, "not in keys" 
            continue
        for e in d[u]['od'].keys():
            for v in d[u]['od'][e]:
                if v in pd.keys():
                    #print "v", v, "for u", u, "in pd.keys", pd.keys()
                    del d[u]['od'][e]
                    ct = ct + 1 

    le_u = None
    for u in pd.keys():
        if u == 500:
            continue
        if u not in d.keys():
            #print "u: ", 6, "not in keys" 
            continue
        if len(d[u]['od'].keys()) > 0:
            #print "1. u: ", u, "pd.keys: ", pd.keys(), "d[u]['od'].keys()[0] : ", d[u]['od'].keys()[0] 
            if d[u]['od'].keys()[0] < le_key:
                le_u = u
                le_key = d[le_u]['od'].keys()[0]

    if le_u is not None:
        v = d[le_u]['od'][le_key][0]
        #print "3. ", le_key, "u: ", le_u, "v: ", v
        return le_u, v, le_key
    else:
        print "(((((((((((((((((((((((((( ERROR ))))))))))))))))))))))))))", ct
        return 0, 0, 0

i = 1
count = 0
w = 0
pd = {}
# add i to pd
#print "************* Adding ", i, "to pd ", pd.keys()
pd[i] = [] 
while (count < nv): 
    u, v, le_key    = find_smallest_cut(pd)
    pd[u]           = [ v, le_key ]
    if v not in pd.keys() and (v != 0):
        # add v to pd
        pd[v] = [] 
        w = w + le_key
        print "************* Adding ", u, "-", v, "to pd ", pd.keys(), "e: ", le_key, "w: ", w
    #del d[u]['od'][le_key]
    count = count + 1
    #print "7: ", u, v, le_key, " pd.keys: ", pd.keys(), "count: ", count, "w: ", w, "len: ", len(pd.keys())
    if len(pd.keys()) == int(nv):
        print "Done"
        break

'''
def find_smallest_cut(pd):
    le_key = Decimal('Infinity')
    for u in pd.keys():
        print "1. u: ", u, "pd.keys: ", pd.keys()
        #print "2. ", d[u]
        if d[u]['od'].keys()[0] < le_key:
            le_u = u
            le_key = d[le_u]['od'].keys()[0]

            # for this le get first v from the list of (u, v1) or (u, v2) ...
            v = d[le_u]['od'][le_key][0]

            # we need to explore a new edge per iter
            # v cannot point to an existing vertex already explored
            if v in pd.keys():
                print "2: v: ", v, "is in pd.keys"
                del d[le_u]['od'][le_key]
                print "3: remaining in le_u: ", le_u, "keys: ", d[le_u]['od']
                le_key = Decimal('Infinity')
                if d[u]['od'].keys()[0] is not None:
                    le_key = d[le_u]['od'].keys()[0]
                continue
            #print "3. ", le_key
    #print "4. ", le_key, le_u
    #print "5. ", d[le_u]
    v       = d[le_u]['od'][le_key][0]
    #print "6. ", le_u, v, le_key
    return le_u, v, le_key
'''
