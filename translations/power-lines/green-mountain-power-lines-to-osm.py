#!/bin/python

# This translates power line data distributed by Green Mountain Power in Vermont.

def translateAttributes(attrs, layerName, geometryType):

    #print(geometryType)
    if not attrs or geometryType != 'Line': return

    # Haven't seen much useful in this schema, unfortunately.

    attrs['power'] = 'line'
    # Should we go ahead and set location=overhead here?
    #tags['location'] = 'overhead'

    return attrs
