#!/bin/python

# This translates power line data from the California State Goverment obtained from http://www.data.gov.

def translateAttributes(attrs, layerName, geometryType):
    if not attrs: return

    tags = {}
    
    voltage = -1
    if 'Circuit' in attrs:
        tags['circuits'] = attrs['Circuit']
    if 'Comments' in attrs:
        tags['comments'] = attrs['Comments']
    if 'Name' in attrs:
        tags['name'] = attrs['Name']
    if 'Owner' in attrs:
        tags['owner'] = attrs['Owner']
    if 'Status' in attrs:
        tags['status'] = attrs['Status'].toLower()
    if 'kV' in attrs:
        voltage = int(attrs['kV'])
        voltage = voltage / 1000
        tags['voltage'] = str(voltage)
    if geometryType == 'line': 
        if 'Type' in attrs:
            lineType = attrs['Type'].toLower()
            if lineType == 'ug'
                tags['power'] = attrs['cable']
                tags['location'] = attrs['underground']
            elif lineType == 'uw'
                tags['power'] = attrs['cable']
                tags['location'] = attrs['underwater']
        elif voltage >= 45
            tags['power'] = attrs['line']
            tags['location'] = attrs['overhead']
        else
            tags['power'] = attrs['minor_line']
            tags['location'] = attrs['overhead']

    return tags
