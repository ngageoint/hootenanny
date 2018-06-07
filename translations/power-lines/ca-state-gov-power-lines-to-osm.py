#!/bin/python

# This translates power line data from the California State Goverment obtained from http://www.data.gov.

def translateAttributes(attrs, layerName, geometryType):

    #print(geometryType)
    if not attrs or geometryType != 'Line': return
    
    if 'Circuit' in attrs:
        attrs['circuits'] = attrs['Circuit']
        del attrs['Circuit']
    if 'Comments' in attrs:
        attrs['comments'] = attrs['Comments']
        del attrs['Comments']
    if 'Name' in attrs:
        attrs['name'] = attrs['Name']
        del attrs['Name']
    if 'Owner' in attrs:
        attrs['owner'] = attrs['Owner']
        del attrs['Owner']
    if 'Status' in attrs:
        attrs['status'] = attrs['Status'].lower()
        del attrs['Status']

    voltage = -1
    if 'kV' in attrs:
        voltageStr = attrs['kV'];
        voltageStr.replace('DC', '')
        try:
            voltage = int(voltageStr)
        except ValueError as err:
            pass
        if voltage > 0:
            voltage = voltage * 1000
            attrs['voltage'] = str(voltage)
        del attrs['kV']

    # If we don't have voltage, assume major line?
    attrs['location'] = 'overhead'
    if voltage == -1:
        attrs['power'] = 'line'
    elif voltage >= 45:
        attrs['power'] = 'line'
    else:
        attrs['power'] = 'minor_line'

    if 'Type' in attrs:
        lineType = attrs['Type'].lower()
        # The inforation in Type will override what was previously assumed to be an overhead line if a different type of line is 
        # encountered.
        if lineType == 'ug':
            attrs['power'] = 'cable'
            attrs['location'] = 'underground'
        elif lineType == 'uw':
            attrs['power'] = 'cable'
            attrs['location'] = 'underwater'

    return attrs
