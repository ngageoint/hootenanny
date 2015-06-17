#!/bin/python

def translateAttributes(attrs, layerName):
    if not attrs: return
    
    tags = {}

    for k, v in attrs.iteritems():
        tags[k.lower()] = v


    return tags

