#!/bin/python

def translateAttributes(attrs, layerName):
    if not attrs: return

    tags = {}

    tags['accuracy'] = '5'
    
    name = []
    if 'REGISTERED' in attrs:
        name.append(attrs['REGISTERED'])
    if 'STREETTYPE' in attrs:
        name.append(attrs['STREETTYPE'])
    if 'QUADRANT' in attrs:
        name.append(attrs['QUADRANT'])
    if len(name) >= 1:
        tags['name'] = ' '.join(name)

    if 'SEGMENTTYP' in attrs:
        t = attrs['SEGMENTTYP']
        if t == '1' or t == '3':
            tags['highway'] = 'motorway'
        else:
            tags['highway'] = 'road'

    # There is also a one way attribute in the data, but I can't tell which 
    # way is which so I'll leave it out.

    return tags
