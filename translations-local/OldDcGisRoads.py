#!/bin/python
 
qLookup = {}
qLookup['NW'] = 'Northwest'
qLookup['NE'] = 'Northeast'
qLookup['SW'] = 'Southwest'
qLookup['SE'] = 'Southeast'
 
def translateAttributes(attrs, layerName, geometryType):
    if not attrs: return
 
    tags = {}
 
    tags['accuracy'] = '50'
 
    name = []
    if 'REGISTERED' in attrs and attrs['REGISTERED'] != '':
        name.append(attrs['REGISTERED'].title())
    if 'DESCRIPTIO' in attrs and attrs['DESCRIPTIO'] != '':
        name.append(attrs['DESCRIPTIO'].title())
    if 'QUADRANT' in attrs and attrs['QUADRANT'] != '' and len(name) >= 1:
        q = attrs['QUADRANT']
        if q in qLookup:
            name.append(qLookup[q])
        else:
            name.append(q)
    if len(name) >= 1:
        tags['name'] = ' '.join(name)
 
    if 'FUNCTIONAL' in attrs:
        f = attrs['FUNCTIONAL'].lower()
        if f == 'principal arterial':
            tags['highway'] = 'primary'
        elif f == 'minor arterial':
            tags['highway'] = 'secondary'
        elif f == 'collector':
            tags['highway'] = 'tertiary'
        elif f == 'local':
            tags['highway'] = 'unclassified'
        else:
            tags['highway'] = 'road'
    else:
        tags['highway'] = 'road'

    if 'PER_ID' in attrs:
        tags['REF2'] = attrs['PER_ID']
    if 'REF_ID' in attrs:
        tags['REF1'] = attrs['REF_ID']

 
    # There is also a one way attribute in the data, but I can't tell which 
    # way is which so I'll leave it out.
 
    # For debugging
    #dum = {}
    #dum['REGISTERED'] = attrs['REGISTERED']
    #dum['DESCRIPTIO'] = attrs['DESCRIPTIO']
    #dum['QUADRANT'] = attrs['QUADRANT']
    #dum['FUNCTIONAL'] = attrs['FUNCTIONAL']
    #print "Input: " + str(dum)
    #print "Output: " + str(tags)
 
    return tags
