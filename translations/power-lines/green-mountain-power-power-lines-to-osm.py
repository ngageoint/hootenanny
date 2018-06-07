#!/bin/python

# This translates power line data distributed by Green Mountain Power in Vermont.

def translateAttributes(attrs, layerName, geometryType):
    if not attrs: return

    tags = {}

    if geometryType == 'line': 
        tags['power'] = attrs['line']
        #tags['location'] = attrs['overhead']

    return tags
