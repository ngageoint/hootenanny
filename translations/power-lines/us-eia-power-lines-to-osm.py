#!/bin/python

# This translates U.S. country-wide power line data published by the United States Energy Information Adminstration.

def translateAttributes(attrs, layerName, geometryType):

    #print(geometryType)
    if not attrs or geometryType != 'Line': return
    
    if 'OWNER' in attrs:
        attrs['owner'] = attrs['OWNER']
        del attrs['OWNER']
    if 'SOURCE' in attrs:
        attrs['source'] = attrs['SOURCE']
        del attrs['SOURCE']
    # Standardizing these on those used by energydata.info and CA state gov
    if 'STATUS' in attrs:
	status = attrs['STATUS'].lower()
	if status == 'IN SERVICE':
	    attrs['status'] = 'operational'
        elif status == 'INACTIVE':
	    attrs['status'] = 'closed'
        elif status == 'UNDER CONST':
	    attrs['status'] = 'construction'
        del attrs['STATUS']

    voltage = -1
    if 'VOLTAGE' in attrs:
        try:
            voltage = int(attrs['VOLTAGE'])
        except ValueError as err:
            pass
        if voltage > 0:
            voltage = voltage * 1000
            attrs['voltage'] = str(voltage) 
        del attrs['VOLTAGE']

    # If we don't have voltage, assume major line?
    # Should we go ahead and set location=overhead here?
    #attrs['location'] = 'overhead'
    if voltage == -1:
        attrs['power'] = 'line'
    elif voltage >= 45000:
        attrs['power'] = 'line'
    else:
        attrs['power'] = 'minor_line'

    return attrs
