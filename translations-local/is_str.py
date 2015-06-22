#!/bin/python

import facc

def translateAttributes(attrs, layerName):
    if not attrs: return

    tags = {}

    if 'STREET' in attrs:
        tags['name'] = attrs['STREET']
    tags['highway'] = "road"

    tags['classification'] = 'UNCLASSIFIED//FOUO'

    return tags
