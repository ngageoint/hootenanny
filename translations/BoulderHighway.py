#!/bin/python

def translateAttributes(attrs, layerName, geometryType):
    if not attrs: return

    tags = {'highway':'road', 'accuracy':'25'}

    if 'LABEL_NAME' in attrs and attrs['LABEL_NAME'] != '':
        tags['name'] = attrs['LABEL_NAME']

    return tags

