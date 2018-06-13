#!/bin/python

# This translates power line data from https://energydata.info/.  This translation only targets African datasets from that site, so datasets
# from non-African countries likely would need their own translation.

def translateAttributes(attrs, layerName, geometryType):
    
    #print(geometryType)
    if not attrs or geometryType != 'Line': return

    if 'status' in attrs:
        attrs['status'] = attrs['status'].lower()

    voltage = -1
    if 'voltage_kV' in attrs:
        try:
            voltage = int(attrs['voltage_kV'])
        except ValueError as err:
            pass
        if voltage > 0:
            voltage = voltage * 1000
            attrs['voltage'] = str(voltage)
        del attrs['voltage_kV']

    # If we don't have voltage, assume major line?
    # Should we go ahead and set location=overhead here?
    #attrs['location'] = 'overhead'
    if voltage == -1:
        attrs['power'] = 'line'
    elif voltage >= 45:
        attrs['power'] = 'line'  
    else:
        attrs['power'] = 'minor_line'

    return attrs
