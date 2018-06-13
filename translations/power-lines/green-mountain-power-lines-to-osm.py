#!/bin/python

# This translates power line data distributed by Green Mountain Power in Vermont.

def translateAttributes(attrs, layerName, geometryType):

    #print(geometryType)
    if not attrs or geometryType != 'Line': return

    # Haven't seen much useful in this schema, unfortunately.

    attrs['power'] = 'line'
    # The only Green Mountain Power dataset this translation has been used against was labeled "overhead only", so setting all lines
    # as overhead.  Check the dataset being translated before adding this tag to be sure its appropriate.
    attrs['location'] = 'overhead'

    return attrs
