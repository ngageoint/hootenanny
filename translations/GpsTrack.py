#!/bin/python

def translateAttributes(attrs, layerName, geometryType):
    if not attrs: return

    tags = attrs
    tags['accuracy'] = '10'
    tags['highway'] = 'road'
    if 'track_fid' in attrs:
        tags['hoot:track:id'] = attrs['track_fid']
        tags['hoot:track:seq'] = "%09d" % int(attrs['track_seg_point_id'])

    return tags

