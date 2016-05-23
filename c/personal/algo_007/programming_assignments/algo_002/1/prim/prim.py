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

        if v not in d.keys():
            d[v] = {}
            d[v]['ud'] = {}
        if e not in d[v]['ud'].keys():
            d[v]['ud'][e] = []
        d[v]['ud'][e].append(u) 
        d[v]['od'] = collections.OrderedDict(sorted(d[v]['ud'].items()))

def find_smallest_cut(pd):
    le_key = Decimal('Infinity')

    ct = 0
    # we need to explore a new edge per iter
    # v cannot point to an existing vertex already explored
    for u in pd.keys():
        if u == 500:
            continue
        if u not in d.keys():
            continue
        for e in d[u]['od'].keys():
            for v in d[u]['od'][e]:
                if v in pd.keys():
                    if e in d[u]['od'].keys():
                        del d[u]['od'][e]
                        ct = ct + 1 

                    if v in d.keys():
                        if e in d[v]['od'].keys():
                            del d[v]['od'][e]

    le_u = None
    for u in pd.keys():
        if u == 500:
            continue
        if u not in d.keys():
            continue
        if len(d[u]['od'].keys()) > 0:
            if d[u]['od'].keys()[0] < le_key:
                le_u = u
                le_key = d[le_u]['od'].keys()[0]

    if le_u is not None:
        v = d[le_u]['od'][le_key][0]
        return le_u, v, le_key
    else:
        print "(((((((((((((((((((((((((( ERROR ))))))))))))))))))))))))))", ct
        return 0, 0, 0

i = 1
count = 0
w = 0
pd = {}
# add i to pd
pd[i] = [] 
while (count < nv): 
    u, v, le_key    = find_smallest_cut(pd)
    pd[u]           = [ v, le_key ]
    if v not in pd.keys() and (v != 0):
        # add v to pd
        pd[v] = [] 
        w = w + le_key
        print "************* Adding ", u, "-", v, "to pd ", pd.keys(), "e: ", le_key, "len: ", len(pd.keys()), "w: ", w

    if u in d.keys():
        del d[u]['od'][le_key]
    if v in d.keys():
        del d[v]['od'][le_key]

    count = count + 1
    if len(pd.keys()) == int(nv):
        print "Done"
        break
