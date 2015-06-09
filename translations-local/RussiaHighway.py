#!/bin/python

import facc

def translateAttributes(attrs, layerName):
    if not attrs: return

    fcodeMap = {
        # Interchange
        'AP020': ['ap020', 'interchange_l', 'interchangel'],
        # Road
        'AP030': ['ap030', 'roadnet', 'road_l', 'roadl'],
        # Bridge 
        'AQ040': ['aq040', 'bridge_l', 'bridge_tunnel_l'],
        # Tunnel
        'AQ130': ['aq130', 'tunnel_l']
    }

    tags = {}

    fcode = ''
    if 'f_code' in attrs:
        fcode = attrs['f_code']
    elif 'FCODE' in attrs:
        fcode = attrs['FCODE']
    else:
        for (key, value) in fcodeMap.iteritems():
            for v in value:
                if v.lower() in layerName.lower():
                    fcode = key

    tags = facc.translateFacc(attrs, fcode)

    # Sometimes the bridges & tunnels are redundantly stored in multiple files.
    if fcode == 'AP030':
        if 'tunnel' in tags or 'bridge' in tags:
            return None

    specId = attrs['SPEC_ID']
#    if (specId == '3KM-VMap-1'):
#        tags['accuracy'] = '250'
#    elif (specId == '3KM-VMap-2'):
#        tags['accuracy'] = '30'


    if 'classification' in tags and tags['classification'] == 'UNCLASSIFIED':
        tags['classification'] = 'UNCLASSIFIED//FOUO'

    try:
        if 'accuracy' in tags:
            num = int(tags['accuracy'])
            if num > 75:
                tags = None
                print "removed one: " + tags['accuracy']
    except ValueError:
        pass

    return tags
