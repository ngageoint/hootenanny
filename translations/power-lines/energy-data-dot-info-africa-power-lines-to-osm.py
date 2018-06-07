#!/bin/python

# This translates power line data from https://energydata.info/.  This translation only targets African datasets from that site, so datasets
# from non-African countries likely would need their own translation.

def translateAttributes(attrs, layerName, geometryType):
    
    print(geometryType)
    if not attrs or geometryType != 'line': return

    tags = {}

    voltage == -1
    if 'operator' in attrs:
        tags['operator'] = attrs['operator']
    if 'source' in attrs:
        tags['source'] = attrs['source']
    if 'status' in attrs:
        tags['status'] = attrs['status'].toLower()
    if 'voltage_kV' in attrs:
        voltage = int(attrs['voltage_kV'])
        voltage = voltage / 1000
        tags['voltage'] = str(voltage)
    if voltage >= 45:
        tags['power'] = attrs['line']
        #tags['location'] = attrs['overhead']
    else
        tags['power'] = attrs['minor_line']
        #tags['location'] = attrs['overhead']

    return tags
