#!/bin/python

# This translates power line data distributed by Green Mountain Power in Vermont.

def translateAttributes(attrs, layerName, geometryType):

    print(geometryType)
    if not attrs or geometryType != 'line': return

    tags = {}

    tags['power'] = attrs['line']
    #tags['location'] = attrs['overhead']

    return tags
