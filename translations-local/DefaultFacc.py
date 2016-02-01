#!/bin/python

import facc

def translateAttributes(attrs, layerName, geometryType):
    if not attrs: return

    # Guess the FCODE based on the input filename
    fcodeMap = {
        # Building
        "AL015": ['al015', 'building'],
        # Cart Track
        'AP010': ['ap010', 'trackl', 'track_l', 'cart_track', 'carttrack'],
        # Interchange
        'AP020': ['ap020', 'interchange_l', 'interchangel'],
        # Road
        'AP030': ['ap030', 'roadnet', 'road_l', 'roadl'],
        # Trail
        'AP050': ['ap050', 'traill', 'trail_l'],
        # Bridge 
        'AQ040': ['aq040', 'bridgel', 'bridge_l', 'bridge_tunnel_l', 'bridge_overpass_l'],
        # Tunnel
        'AQ130': ['aq130', 'tunnell', 'tunnel_l']
    }

    tags = {}

    # if the FCODE is specified on a per feature basis then it overrides the value inherited from the filename.
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

    # Convert from FACC to OSM using the generic conversion approach.
    tags = facc.translateFacc(attrs, fcode)

    ###
    # Provide customizations on a per layer basis here.
    ###

    # Sometimes the bridges & tunnels are redundantly stored in multiple files.
    if fcode == 'AP030':
        if 'tunnel' in tags or 'bridge' in tags:
            return None

    if 'classification' in tags and tags['classification'] == 'UNCLASSIFIED':
        tags['classification'] = 'UNCLASSIFIED//FOUO'

    return tags
