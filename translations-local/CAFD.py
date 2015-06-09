#!/bin/python

def helperFunction(tags, attrs):
    # Is the STNAME attribute properly populated?
    if 'GFID' in attrs and attrs['GFID'] != '':
        tags['name'] = attrs['GFID']
    if 'ORIG_SOURC' in attrs and attrs['ORIG_SOURC'] != '':
        tags['timestamp'] = attrs['ORIG_SOURC']
    if 'SHAPE_Leng' in attrs and attrs['SHAPE_Leng'] != '':
        tags['shape_length'] = attrs['SHAPE_Leng']
    return tags 

def translateAttributes(attrs, layerName):
    # Intialize our results object
    tags = {}

    # specific to layer type
    if 'AirportAirfield' in layerName:
      tags['aeroway'] = 'aerodrome'
      helperFunction(tags, attrs)
    elif 'Aqueduct' in layerName:
      tags['bridge'] = 'aqueduct'
      helperFunction(tags, attrs)
    elif 'BridgeOverpass' in layerName:
      tags['bridge'] = 'overpass'
      helperFunction(tags, attrs)
    elif 'Building' in layerName:
      tags['building'] = 'building'
      helperFunction(tags, attrs)
      if 'SHAPE_Area' in attrs and attrs['SHAPE_Area'] != '':
          tags['shape_area'] = attrs['SHAPE_Area']
    elif 'BuiltUpArea' in layerName:
      tags['amenity'] = 'grave_yard'
      tags['landuse'] = 'cemetery'
      helperFunction(tags, attrs)
    elif 'Cemetery' in layerName:
      tags['builtup_area'] = 'builtup_area'
      helperFunction(tags, attrs)
    elif 'CommunicationTower' in layerName:
      tags['man_made'] = 'tower'
      helperFunction(tags, attrs)
    elif 'Cropland' in layerName:
      tags['landuse'] = 'farm'
      helperFunction(tags, attrs)
    elif 'Dam' in layerName:
      tags['waterway'] = 'dam'
      helperFunction(tags, attrs)
    elif 'Fortification' in layerName:
      tags['barrier'] = 'city_wall'
      helperFunction(tags, attrs)
    elif 'Heli' in layerName:
      tags['aeroway'] = 'helipad'
      helperFunction(tags, attrs)
    elif 'Lake' in layerName:
      tags['natural'] = 'water'
      helperFunction(tags, attrs)
    elif 'Military' in layerName:
      tags['landuse'] = 'military'
      helperFunction(tags, attrs)
    elif 'Park' in layerName:
      tags['leisure'] = 'park'
      helperFunction(tags, attrs)
    elif 'Pipeline' in layerName:
      tags['man_made'] = 'pipeline'
      helperFunction(tags, attrs)
    elif 'PowerPlant' in layerName:
      tags['power'] = 'plant'
      helperFunction(tags, attrs)
    elif 'PowerTransLine' in layerName:
      tags['power'] = 'line'
      helperFunction(tags, attrs)
    elif 'Reservoir' in layerName:
      tags['landuse'] = 'reservoir'
      helperFunction(tags, attrs)
    elif 'RiverStream' in layerName:
      tags['waterway'] = 'river'
      helperFunction(tags, attrs)
    elif 'Road' in layerName:
      tags['highway'] = 'primary'
      helperFunction(tags, attrs)
    elif 'TowerNonComm' in layerName:
      tags['man_made'] = 'tower'
      helperFunction(tags, attrs)
    elif 'Trail' in layerName:
      tags['route'] = 'pedestrian'
      helperFunction(tags, attrs)
    elif 'Tree' in layerName:
      tags['natural'] = 'tree'
      helperFunction(tags, attrs)
    elif 'Tunnel' in layerName:
      tags['tunnel'] = 'tunnel'
      helperFunction(tags, attrs)
    elif 'WaterTower' in layerName:
      tags['man_made'] = 'water_tower'
      helperFunction(tags, attrs)

    # Useful when debugging. You can see print statements on stdout when Hootenanny is running
   # print "Input: " + str(attrs)
   # print "Output: " + str(tags)

    # Return our translated tags
    return tags
