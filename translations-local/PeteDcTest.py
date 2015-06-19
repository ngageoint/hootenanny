#!/bin/python

def translateAttributes(attrs, layerName):
    if not attrs: return

    tags = {}

    tags['accuracy'] = '50'
    
    tags['name'] = attrs['NAME']
    tags['highway'] = attrs['HIGHWAY']
    if 'PER_ID' in attrs:
        tags['REF2'] = attrs['PER_ID']
    if 'REF_ID' in attrs:
        tags['REF1'] = attrs['REF_ID']

    return tags
