#!/bin/python

# This translates U.S. country-wide power line data from the United States Energy Information Adminstration.

def translateAttributes(attrs, layerName, geometryType):
    if not attrs: return

    tags = {}
    
    voltage = -1
    if 'OWNER' in attrs:
        tags['owner'] = attrs['OWNER']
    if 'SOURCE' in attrs:
        tags['source'] = attrs['SOURCE']
    # Standardizing these on those used by energydata.info and CA state gov
    if 'STATUS' in attrs:
	status = attrs['STATUS'].toLower()
	if status == 'IN SERVICE'
	    tags['status'] = 'operational'
        elif status == 'INACTIVE'
	    tags['status'] = 'closed'
        elif status == 'UNDER CONST'
	    tags['status'] = 'construction'
    if 'VOLTAGE' in attrs:
        voltage = int(attrs['VOLTAGE'])
        voltage = voltage / 1000
        tags['voltage'] = str(voltage)
    if geometryType == 'line': 
        if voltage >= 45
            tags['power'] = attrs['line']
            tags['location'] = attrs['overhead']
        else
            tags['power'] = attrs['minor_line']
            tags['location'] = attrs['overhead']

    return tags
