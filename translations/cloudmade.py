#!/bin/python


def foo():
    tags2 = {'a':1, 'b':2}
    print type(tags2)
    for k in tags2.keys():
        print k
    print "hello world"
    return 1

def translateAttributes(attrs, layerName):
    if not attrs: return

    tags = {}
    for (k, v) in attrs.iteritems():
        k = k.lower()
        if (k == 'type'):
            k = 'highway'
        tags[k] = v

    return tags
