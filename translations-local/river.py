#!/bin/python

import facc

def translateAttributes(attrs, layerName, geometryType):
    if not attrs: return

    tags = {}

    fcode = ''
    if 'f_code' in attrs:
        fcode = attrs['f_code']
    elif 'FCODE' in attrs:
        fcode = attrs['FCODE']

    tags = facc.translateFacc(attrs, fcode)

    # I really don't know if it is a river or a stream. 
    tags['waterway'] = 'river'

    if 'classification' in tags and tags['classification'] == 'UNCLASSIFIED':
        tags['classification'] = 'UNCLASSIFIED//FOUO'

    return tags
