#!/bin/python

def translateAttributes(attrs, layerName, geometryType):
    if not attrs: return

    tags = {}

    tags['accuracy'] = '5'
    
    if 'FULLNAME' in attrs:
        name = attrs['FULLNAME']
        if name != 'NULL' and name != '':
            tags['name'] = name

    if 'MTFCC' in attrs:
        mtfcc = attrs['MTFCC']
        if mtfcc == 'S1100':
            tags['highway'] = 'primary'
        if mtfcc == 'S1200':
            tags['highway'] = 'secondary'
        if mtfcc == 'S1400':
            tags['highway'] = 'unclassified'
        if mtfcc == 'S1500':
            tags['highway'] = 'track'
            tags['surface'] = 'unpaved'
        if mtfcc == 'S1630':
            tags['highway'] = 'road'
        if mtfcc == 'S1640':
            tags['highway'] = 'service'
        if mtfcc == 'S1710':
            tags['highway'] = 'path'
            tags['foot'] = 'designated'
        if mtfcc == 'S1720':
            tags['highway'] = 'steps'
        if mtfcc == 'S1730':
            tags['highway'] = 'service'
        if mtfcc == 'S1750':
            tags['highway'] = 'road'
        if mtfcc == 'S1780':
            tags['highway'] = 'service'
            tags['service'] = 'parking_aisle'
        if mtfcc == 'S1820':
            tags['highway'] = 'path'
            tags['bicycle'] = 'designated'
        if mtfcc == 'S1830':
            tags['highway'] = 'path'
            tags['horse'] = 'designated'


    return tags
