#!/bin/python

def helperFunction(tags, attrs):
    # Is the STNAME attribute properly populated?
   # if 'NAME_ENGLI' in attrs and attrs['NAME_ENGLI'] != '':
   #     tags['name'] = attrs['NAME_ENGLI']
    return tags 

def translateAttributes(attrs, layerName):
    # Intialize our results object
    tags = {}

    # general
    #tags['iso3'] = attrs['ISO3']
    for x in attrs:
      tags[x.lower()]=attrs[x]

    # special country stuff
    if 'African_Continent' in layerName:
      tags['boundary'] = 'african_countries'
      tags['feature_dataset'] = 'admin_boundaries'
      tags['name'] = attrs['NAME_ENGLI']

    elif 'South_America_Admin_0' in layerName:
      tags['boundary'] = 'south_american_countries'
      tags['feature_dataset'] = 'admin_boundaries'
      tags['name'] = attrs['NAME_ENGLI']

    # special country stuff
    elif 'African_Countries' in layerName:
      if 'Admin0' in layerName: 
        tags['admin_level'] = '2'
      elif 'Admin1' in layerName: 
        tags['admin_level'] = '4'
      elif 'Admin2' in layerName: 
        tags['admin_level'] = '6'
      tags['boundary'] = 'african_countries'
      tags['feature_dataset'] = 'admin_boundaries'

    # special country stuff
    elif 'SouthAmerican_Countries' in layerName:
      if 'Admin0' in layerName: 
        tags['admin_level'] = '2'
      elif 'Admin1' in layerName: 
        tags['admin_level'] = '4'
      elif 'Admin2' in layerName: 
        tags['admin_level'] = '6'
      tags['boundary'] = 'south_american_countries'
      tags['feature_dataset'] = 'admin_boundaries'

    # Abyei dispute 
    elif 'AbyeiDispuite' in layerName:
      tags['boundary'] = 'abyei_dispuite'
      tags['feature_dataset'] = 'admin_boundaries'

    # ACLED 
    elif 'ACLED' in layerName:
      tags['place'] = 'acled_location'
      tags['feature_dataset'] = 'miscellaneous'

    #admin
    elif 'Admin_0' in layerName:
      name = "admin_boundaries"
      tags['feature_dataset'] = 'admin_boundaries'
      tags['admin_level'] = '2'
      tags['gadm_level'] = '0'
    elif 'Admin_1' in layerName:
      name = "admin_boundaries"
      tags['feature_dataset'] = 'admin_boundaries'
      tags['admin_level'] = '4'
      tags['gadm_level'] = '1'
    elif 'Admin_2' in layerName:
      name = "admin_boundaries"
      tags['feature_dataset'] = 'admin_boundaries'
      tags['admin_level'] = '6'
      tags['gadm_level'] = '2'

    elif 'Admin' in layerName:
      layerNameFields = layerName.split("_")
      numFields = len(layerNameFields)
      name = ""
      if layerName.startswith("VEN"):
        for i in range(1, numFields-3):
          name +=  layerNameFields[i] + "_"
        name += layerNameFields[numFields-3]
        tags['feature_dataset'] = 'demographics'
      elif layerName.startswith("Admin"):
        for i in range(1, numFields-1):
          name +=  layerNameFields[i] + "_"
        name += layerNameFields[numFields-1]
        tags['feature_dataset'] = 'demographics'
      else:
        if numFields == 4:
          name = "admin_boundaries"
          tags['feature_dataset'] = 'admin_boundaries'
        else:
          for i in range(2, numFields-3):
            name +=  layerNameFields[i] + "_"
          name += layerNameFields[numFields-3]
          tags['feature_dataset'] = 'demographics'
      tags['name'] = name
      tags['border_type'] = name
      tags['boundary'] = 'administrative'
      if 'Admin0' in layerName:
        tags['admin_level'] = '2'
        tags['gadm_level'] = '0'
      elif 'Admin1' in layerName:
        tags['gadm_level'] = '1'
        if 'SSD' in layerName or 'SOM' in layerName:
          tags['admin_level'] = '3'
        else:
          tags['admin_level'] = '4'
      elif 'Admin2' in layerName:
        tags['gadm_level'] = '2'
        if 'SSD' in layerName or 'SOM' in layerName:
          tags['admin_level'] = '4' 
        elif 'BWA' in layerName:
          tags['admin_level'] = '8'
        else:
          tags['admin_level'] = '6'
      elif 'Admin3' in layerName:
        tags['gadm_level'] = '3'
        if 'SSD' in layerName:
          tags['admin_level'] = '6'
        else:
          tags['admin_level'] = '7'
      elif 'Admin_Zones' in layerName or 'AdminRegion' in layerName:
        tags['admin_level'] = '7'
        tags['gadm_level'] = 'admin_zone'

    #admin (encoded as adm)
    elif '_Adm' in layerName:
      layerNameFields = layerName.split("_")
      numFields = len(layerNameFields)
      name = ""
      if numFields == 4:
        name = "admin_boundaries"
        tags['feature_dataset'] = 'admin_boundaries'
      else:
        for i in range(2, numFields-3):
          name +=  layerNameFields[i] + "_"
        name += layerNameFields[numFields-3]
        tags['feature_dataset'] = 'demographics'
      tags['name'] = name
      tags['border_type'] = name
      tags['boundary'] = 'administrative'
      if 'Adm0' in layerName:
        tags['admin_level'] = '2'
        tags['gadm_level'] = '0'
      elif 'Adm1' in layerName:
        tags['gadm_level'] = '1'
        if 'SSD' in layerName or 'SOM' in layerName:
          tags['admin_level'] = '3'
        else:
          tags['admin_level'] = '4'
      elif 'Adm2' in layerName:
        tags['gadm_level'] = '2'
        if 'SSD' in layerName or 'SOM' in layerName:
          tags['admin_level'] = '4' 
        elif 'BWA' in layerName:
          tags['admin_level'] = '8'
        else:
          tags['admin_level'] = '6'
      elif 'Adm3' in layerName:
        tags['gadm_level'] = '3'
        if 'SSD' in layerName:
          tags['admin_level'] = '6'
        else:
          tags['admin_level'] = '7'
      elif 'AdmR' in layerName:
        tags['admin_level'] = '7'
        tags['gadm_level'] = 'admin_zone'

    # ADMIN in caps (think these are the modern updates)
    elif 'ADMIN' in layerName:
      if '0' in layerName:
        tags['name'] = attrs['ADM0_NAME'] 
      elif '1' in layerName:
        tags['name'] = attrs['ADM1_NAME'] 
      elif '2' in layerName:
        tags['name'] = attrs['ADM2_NAME'] 
      tags['feature_dataset'] = 'admin_boundaries'
 
    # agricultural industry 
    elif 'AgriculturalIndustry' in layerName or 'Agricultural_Zone' in layerName or 'Agriculture' in layerName:
      tags['boundary'] = 'agricultural_industry'
      tags['feature_dataset'] = 'economy'

    # agricultural industry points 
    elif 'AgriculturalIndustry_Point' in layerName or 'Agricultural_POI' in layerName:
      tags['place'] = 'agricultural_industry_poi'
      tags['feature_dataset'] = 'economy'

    # air bases
    elif 'AirBase' in layerName or 'Air_Base' in layerName:
      tags['landuse'] = 'military' 
      tags['feature_dataset'] = 'miscellaneous'

    # aircraft accidents 
    elif 'AircraftAccident' in layerName:
      tags['place'] = 'aircraft_accidents' 
      tags['feature_dataset'] = 'miscellaneous'

    # airports
    elif 'Airport' in layerName or 'Airfield' in layerName:
      tags['aeroway'] = 'aerodrome'
      tags['feature_dataset'] = 'transportation'

    # autonomous regions
    elif 'AutonomousRegion' in layerName:
      tags['boundary'] = 'autonomous_region'
      tags['feature_dataset'] = 'admin_boundaries'

    # automotive
    elif 'Automotive' in layerName:
      tags['place'] = 'automotive'
      tags['feature_dataset'] = 'miscellaneous'

    # avian flu outbreak
    elif 'AvianFlu' in layerName:
      tags['boundary'] = 'avian_flu_outbreak'
      tags['feature_dataset'] = 'significant_events'

    # bangkok metro stations
    elif 'BangkokMetro' in layerName:
      tags['amenity'] = 'metro_station' 
      tags['feature_dataset'] = 'transportation'
      tags['grouping'] = 'bangkok'

    # bangkok metro stations
    elif 'BangkokMetroStation' in layerName:
      tags['amenity'] = 'metro_station' 
      tags['feature_dataset'] = 'transportation'
      tags['grouping'] = 'bangkok'

    # bangui arrondissements 
    elif 'BanguiArrondissements' in layerName:
      tags['boundary'] = 'bangui_arrondissements'
      tags['feature_dataset'] = 'demographics'

    # bangui arrondissements population 
    elif 'BanguiArrondissements_Population' in layerName:
      tags['boundary'] = 'bangui_arrondissements_population'
      tags['feature_dataset'] = 'demographics'

    # beaches
    elif 'Beaches' in layerName:
      tags['natural'] = 'beach'
      tags['feature_dataset'] = 'water_resources'

    # border crossings
    elif 'BorderCrossing' in layerName or 'Border_Crossing' in layerName:
      tags['crossing'] = 'border_crossing'
      tags['feature_dataset'] = 'miscellaneous'

    # births 
    elif 'Birth' in layerName: 
      tags['boundary'] = 'births'
      tags['feature_dataset'] = 'demographics'

    # border stations 
    elif 'Border_Station' in layerName or 'BorderPoint' in layerName: 
      tags['building'] = 'border_station'
      tags['feature_dataset'] = 'miscellaneous'

    # bridge and tunnels 
    elif 'Bridges_Tunnels' in layerName:
      if 'Bridge' in attrs['TYPE']:
        tags['bridge'] = 'yes'
      elif 'Tunnel' in attrs['TYPE']:
        tags['tunnel'] = 'yes'
      tags['feature_dataset'] = 'transportation'

    # bus companies 
    elif 'BusCompanies' in layerName:
      tags['office'] = 'bus_company'
      tags['feature_dataset'] = 'transportation'

    # bus routes 
    elif 'BusRoute' in layerName:
      tags['route'] = 'bus'
      tags['feature_dataset'] = 'transportation'

    # bus stations 
    elif 'Bus_Station' in layerName:
      tags['public_transport'] = 'station'
      tags['feature_dataset'] = 'transportation'

    # bus stops
    elif 'BusStops' in layerName:
      tags['highway'] = 'bus_stop'
      tags['feature_dataset'] = 'transportation'

    # hybrid bus rail stop combo
    elif 'Bus_RailStops' in layerName:
      tags['highway'] = 'bus_railstop'
      tags['feature_dataset'] = 'transportation'

    # candidate votes 
    elif 'CandidateVotes' in layerName:
      tags['boundary'] = 'candidate_votes'
      tags['feature_dataset'] = 'groups'

    # caracas metro routes 
    elif 'Caracas_Metro_Line' in layerName:
      tags['route'] = 'metro_route' 
      tags['feature_dataset'] = 'transportation'
      tags['grouping'] = 'caracas'

    # caracas metro stations
    elif 'Caracas_Metro_Station' in layerName:
      tags['amenity'] = 'metro_station' 
      tags['feature_dataset'] = 'transportation'
      tags['grouping'] = 'caracas'

    # cattle camps 
    elif 'CattleCamps' in layerName:
      tags['boundary'] = 'cattle_camps'
      tags['feature_dataset'] = 'transportation'

    # cell towers 
    elif 'CellTower' in layerName or 'Cell_Tower' in layerName:
      tags['man_made'] = 'cell_towers'
      tags['feature_dataset'] = 'communications'

    # cemetery 
    elif 'Cemeteries' in layerName or 'Cemetery' in layerName or 'Cemetaries' in layerName:
      tags['landuse'] = 'cemetery'
      tags['feature_dataset'] = 'religion'

    # child mortality 
    elif 'ChildMortality' in layerName:
      tags['boundary'] = 'child_mortality'
      tags['feature_dataset'] = 'demographics'

    # cities towns pops 
    elif 'CitiesTownsPop' in layerName:
      tags['place'] = 'cities_towns_pop'
      tags['feature_dataset'] = 'demographics'

    # city road networks 
    elif 'City_Road_Network' in layerName:
      if 'ROAD_TYPE' in attrs:
        tags['highway'] = attrs['ROAD_TYPE'] 
      else:
        tags['highway'] = attrs['TYPE'] 
      tags['feature_dataset'] = 'transportation'

    # child malnutrition 
    elif 'ChildMalnutrition' in layerName:
      tags['boundary'] = 'child_malnutrition'
      tags['feature_dataset'] = 'medical_health'

    # climate zones 
    elif 'Climate_Zone' in layerName or 'Climate' in layerName:
      tags['boundary'] = 'climate_zones'
      tags['feature_dataset'] = 'miscellaneous'

    # coal areas 
    elif 'Coal_Areas' in layerName:
      tags['boundary'] = 'coal_areas'
      tags['feature_dataset'] = 'land'

    # colleges       
    elif 'College' in layerName:
      tags['amenity'] = 'college' 
      tags['feature_dataset'] = 'education'

    # commercial poi 
    elif 'Commercial_POI' in layerName:
      tags['place'] = 'commercial_poi'
      tags['feature_dataset'] = 'miscellaneous'

    # communication towers 
    elif 'Communication_Tower' in layerName:
      tags['man_made'] = 'communication_towers'
      tags['feature_dataset'] = 'communications'

    # communities
    elif 'Communities' in layerName:
      tags['boundary'] = 'communities'
      tags['feature_dataset'] = 'miscellaneous'

    # conflict points 
    elif 'ConflictPoint' in layerName or 'Conflict_Point' in layerName:
      tags['place'] = 'conflict_poi'
      tags['feature_dataset'] = 'miscellaneous'

    # conservation area 
    elif 'ConservationArea' in layerName:
      tags['boundary'] = 'protected_area'
      tags['feature_dataset'] = 'land'

    # crime data 
    elif 'Crime_Data' in layerName:
      tags['place'] = 'crime_data'
      tags['feature_dataset'] = 'significant_events'

    # crisis border crossing 
    elif 'Crisis_Border_Crossing' in layerName:
      tags['crossing'] = 'crisis_border_crossing'
      tags['feature_dataset'] = 'significant_events'

    # crisis conflict area 
    elif 'Crisis_Conflict_Area' in layerName:
      tags['boundary'] = 'crisis_conflict_areas'
      tags['feature_dataset'] = 'significant_events'

    # crisis external camps
    elif 'Crisis_External_Camp' in layerName:
      tags['place'] = 'crisis_external_camps'
      tags['feature_dataset'] = 'significant_events'

    # crisis external refugee registration 
    elif 'Crisis_External_Refugee_Registration' in layerName:
      tags['boundary'] = 'crisis_external_refugee_registration'
      tags['feature_dataset'] = 'significant_events'

    # crop land 
    elif 'Cropland' in layerName or 'Crop_Area' in layerName:
      tags['landuse'] = 'farm'
      tags['feature_dataset'] = 'land'

    # cultural centers 
    elif 'CulturalCenter' in layerName or 'Cultural_POI' in layerName or 'Culture_Center' in layerName:
      tags['amenity'] = 'community_centre'
      tags['feature_dataset'] = 'education'

    # dams
    elif 'Dams' in layerName:
      tags['waterway'] = 'dam'
      tags['feature_dataset'] = 'water_resources'

    # demography
    elif 'Demography' in layerName:
      layerNameFields = layerName.split("_")
      numFields = len(layerNameFields)
      name = ""
      for i in range(1, numFields-3):
        name +=  layerNameFields[i] + "_"
      name += layerNameFields[numFields-3]
      tags['name'] = name
      tags['boundary'] = name
      tags['feature_dataset'] = 'demographics'
      print("name="+name)

    # dengue 
    elif 'Dengue' in layerName:
      tags['place'] = 'dengue'
      tags['feature_dataset'] = 'medical_health'

    # development projects 
    elif 'DevelopmentProject' in layerName:
      tags['place'] = 'developmental_project'
      tags['feature_dataset'] = 'economy'

    # disaster distribution map
    elif 'DisasterDistributionMap' in layerName:
      tags['boundary'] = 'disaster_distribution_map'
      tags['feature_dataset'] = 'significant_events'

    # disaster response elements 
    elif 'DisasterResponseElement' in layerName:
      tags['boundary'] = 'disaster_response_elements'
      tags['feature_dataset'] = 'significant_events'

    # disputed boundaries
    elif 'DisputedBoundaries' in layerName:
      tags['boundary'] = 'disputed_boundary'
      tags['feature_dataset'] = 'admin_boundaries'

    # drought
    elif 'Drought' in layerName:
      tags['boundary'] = 'drought_boundary'
      tags['feature_dataset'] = 'significant_events'

    # earthquakes hazard zones 
    elif 'SeismicHazard' in layerName:
      tags['boundary'] = 'seismic_hazard_boundaries'
      tags['feature_dataset'] = 'significant_events'

    # earthquakes 
    elif 'Earthquake' in layerName or 'Seismic' in layerName:
      tags['place'] = 'earthquake_location'
      tags['feature_dataset'] = 'significant_events'

    # ecowas 
    elif 'ECOWAS' in layerName:
      tags['boundary'] = 'ecowas'
      tags['feature_dataset'] = 'economy'

    # schools
    elif 'Education' in layerName or 'School' in layerName:
      if 'TYPE_1' in attrs:
        tags['amenity'] = attrs['TYPE_1'] 
      elif 'TYPE' in attrs:
        tags['amenity'] = attrs['TYPE']
      else:
        tags['amenity'] = 'school' 
      tags['feature_dataset'] = 'education'

    # election results 
    elif 'ElectionResults' in layerName or 'ElectionsData' in layerName:
      tags['boundary'] = 'election_results'
      tags['feature_dataset'] = 'groups'

    # electricity towers 
    elif 'ElectricityTower' in layerName or 'Electrical_POI' in layerName:
      tags['power'] = 'tower'
      tags['feature_dataset'] = 'miscellaneous'

    # electric lines 
    elif 'ElectricalLine' in layerName or 'ElectricityLine' in layerName:
      tags['power'] = 'line'
      tags['feature_dataset'] = 'miscellaneous'

    # electoral results 
    elif 'ElectoralDist' in layerName:
      tags['boundary'] = 'electoral_districts'
      tags['feature_dataset'] = 'admin_boundaries'

    # electoral results political 
    elif 'ElectoralDistrict_Political' in layerName:
      tags['boundary'] = 'electoral_districts_political'
      tags['feature_dataset'] = 'demographics'

    # embassies 
    elif 'Embassies' in layerName or 'Embassy' in layerName:
      tags['amenity'] = 'embassy' 
      tags['feature_dataset'] = 'miscellaneous'
      if 'Consulate' in layerName:
        tags['embassy_type'] = 'consulate'

    # ethnic conflict 
    elif 'EthnicConflict' in layerName:
      tags['boundary'] = 'ethnic_conflict' 
      tags['feature_dataset'] = 'ethnicity'

    # ethnicity 
    elif 'Ethnicity' in layerName or 'Ethnolinguistic' in layerName or 'EthnoLinguistic' in layerName or 'EthnicMap' in layerName or 'EthnicGroup' in layerName:
      tags['boundary'] = 'ethnicity' 
      tags['feature_dataset'] = 'ethnicity'

    # ferry ports 
    elif 'FerryPort' in layerName or 'FerryStop' in layerName:
      tags['man_made'] = 'ferry_port' 
      tags['feature_dataset'] = 'transportation'

    # ferry transportation 
    elif 'FerryTransportation' in layerName or 'FerryService' in layerName or 'FerryRoute' in layerName:
      tags['route'] = 'ferry' 
      tags['feature_dataset'] = 'transportation'

    # fisheries 
    elif 'Fisheries' in layerName:
      tags['boundary'] = 'fisheries' 
      tags['feature_dataset'] = 'economy'

    # food shortage 
    elif 'FoodShortage' in layerName:
      tags['boundary'] = 'food_shortage' 
      tags['feature_dataset'] = 'significant_events'

    # flood 
    elif 'Flood' in layerName:
      tags['geological'] = 'flood_area' 
      tags['feature_dataset'] = 'significant_events'

    # flora 
    elif 'Flora' in layerName:
      tags['leisure'] = 'nature_reserve' 
      tags['feature_dataset'] = 'land'
      tags['grouping'] = 'flora'

    # foreign aid 
    elif 'ForeignAid' in layerName:
      tags['place'] = 'foreign_aid' 
      tags['feature_dataset'] = 'economy'

    # foreign investment project 
    elif 'ForeignInvestment' in layerName or 'ForeignDevelopment' in layerName or 'Foreign_Investment' in layerName or 'ForeignINV' in layerName or 'ForeignInvest' in layerName:
      tags['building'] = 'construction' 
      tags['feature_dataset'] = 'economy'

    # forest 
    elif 'Forest' in layerName:
      tags['landuse'] = 'forest' 
      tags['feature_dataset'] = 'land'

    # forest reserve 
    elif 'Forest_Reserve' in layerName:
      tags['leisure'] = 'nature_reserve' 
      tags['feature_dataset'] = 'land'
      tags['grouping'] = 'forest'

    # free trade zone 
    elif 'FreeTradeZone' in layerName or 'Free_Trade_Zone' in layerName or 'FTZ' in layerName or 'FreeTradeArea' in layerName:
      tags['landuse'] = 'free_torestrade_zone' 
      tags['feature_dataset'] = 'economy'

    # geologic contacts 
    elif 'Geologic_Contact' in layerName:
      tags['boundary'] = 'geologic_contacts' 
      tags['feature_dataset'] = 'land'

    # geologic folds 
    elif 'Geologic_Fold' in layerName:
      tags['boundary'] = 'geologic_folds' 
      tags['feature_dataset'] = 'land'

    # geologic provinces 
    elif 'Geologic_Province' in layerName:
      tags['boundary'] = 'geologic_provinces' 
      tags['feature_dataset'] = 'land'

    # geologic provinces 
    elif 'Geologic_Unit' in layerName:
      tags['boundary'] = 'geologic_units' 
      tags['feature_dataset'] = 'land'

    # geonames 
    elif 'Geoname' in layerName or 'GeoName' in layerName:
      tags['place'] = 'geonames' 
      tags['feature_dataset'] = 'admin_boundaries'

    # generalized geology 
    elif 'Generalized_Geology' in layerName:
      tags['boundary'] = 'generalized_geology' 
      tags['feature_dataset'] = 'land'

    # glaciers 
    elif 'Glacier' in layerName:
      tags['natural'] = 'glacier' 
      tags['feature_dataset'] = 'water_resources'

    # GNDB 
    elif 'GNDB' in layerName:
      tags['gndb'] = 'gndb_country_code' 
      tags['feature_dataset'] = 'admin_boundaries'

    # government 
    elif 'Government' in layerName:
      tags['office'] = 'government' 
      tags['feature_dataset'] = 'miscellaneous'

    # gov and indigenous groups 
    elif 'GOV_Indig' in layerName:
      tags['office'] = 'government_and_indigenous_groups' 
      tags['feature_dataset'] = 'ethnicity'

    # grasslands
    elif 'Grassland' in layerName:
      tags['boundary'] = 'grasslands' 
      tags['feature_dataset'] = 'land'

    # gsm towers
    elif 'GSM_Tower' in layerName:
      tags['man_made'] = 'gsm_tower' 
      tags['feature_dataset'] = 'communications'

    # HADR 
    elif 'HADR' in layerName:
      tags['boundary'] = 'hadr' 
      tags['feature_dataset'] = 'significant_events'

    # HADR 
    elif 'HADR_Pts' in layerName:
      tags['place'] = 'hadr_poi' 
      tags['feature_dataset'] = 'significant_events'

    # Health epidemic points     
    elif 'HealthEpidemicPoints' in layerName:
      tags['place'] = 'health_epidemic_poi' 
      tags['feature_dataset'] = 'medical_health'

    # health epidemic polygons
    elif 'HealthEpidemicPolygon' in layerName or 'HealthDistrict' in layerName or 'HealthEvents_Epidemic' in layerName or 'Epidemic' in layerName:
      tags['boundary'] = 'health_epidemic_area' 
      tags['feature_dataset'] = 'medical_health'

    # health events 
    elif 'HealthEvent' in layerName or 'Health_Event' in layerName:
      tags['boundary'] = 'health_event' 
      tags['feature_dataset'] = 'medical_health'

    # health generic 
    elif 'Health_' in layerName:
      layerNameFields = layerName.split("_")
      numFields = len(layerNameFields)
      name = ""
      for i in range(2, numFields-3):
        name +=  layerNameFields[i] + "_"
      name += layerNameFields[numFields-3]
      tags['name'] = name
      tags['boundary'] = name 
      tags['feature_dataset'] = 'medical_health'

    # HIV rates     
    elif 'HIVRate' in layerName or 'HealthEvents_HIV' in layerName or 'HIV_Stat' in layerName:
      tags['boundary'] = 'HIV_rate' 
      tags['feature_dataset'] = 'medical_health'

    # hospital     
    elif 'Hospital' in layerName:
      tags['amenity'] = 'hospital' 
      tags['feature_dataset'] = 'medical_health'

    # hotels     
    elif 'Hotel' in layerName or 'Lodging' in layerName:
      tags['tourism'] = 'hotel' 
      tags['feature_dataset'] = 'economy'

    # humanitarian office     
    elif 'HumanitarianOffice' in layerName:
      tags['office'] = 'humanitarian_office' 
      tags['feature_dataset'] = 'groups'

    # hydrobasin     
    elif 'Hydrobasin' in layerName:
      tags['landuse'] = 'hydrobasin' 
      tags['feature_dataset'] = 'water_resources'

    # hydrologic drainage     
    elif 'Hydrologic_Drainage' in layerName:
      tags['landuse'] = 'hydrologic_drainage' 
      tags['feature_dataset'] = 'water_resources'

    # hydrocarbon blocks     
    elif 'HydrocarbonBlock' in layerName or 'Hydrocarbon_Exploration' in layerName:
      tags['boundary'] = 'hydrocarbon_block' 
      tags['feature_dataset'] = 'land'

    # hydro geology     
    elif 'Hydrogeology' in layerName:
      tags['boundary'] = 'hydrogeology' 
      tags['feature_dataset'] = 'water_resources'

    # ID (imminent danger) points       
    elif 'IDP' in layerName:
      tags['place'] = 'imminent_danger_poi' 
      tags['feature_dataset'] = 'groups'

    # Ilemi Dispute       
    elif 'IlemiDispute' in layerName:
      tags['boundary'] = 'ilemi_disputed_region' 
      tags['feature_dataset'] = 'admin_boundaries'

    # indigenous ethno linguistic groups       
    elif 'Indigenous_Ethno_Linguistic' in layerName:
      tags['boundary'] = 'indigenous_ethno_linguistic_group' 
      tags['feature_dataset'] = 'languages'

    # indigenous groups       
    elif 'Indigenous_Groups_' in layerName:
      tags['boundary'] = 'indigenous_group' 
      tags['feature_dataset'] = 'ethnicity'

    # indigenous groups by state       
    elif 'Indigenous_GroupsbyState' in layerName:
      tags['boundary'] = 'indigenous_groups_by_state' 
      tags['feature_dataset'] = 'ethnicity'

    # indigenous reserve       
    elif 'IndigenousReserve' in layerName:
      tags['boundary'] = 'indigenous_reserve' 
      tags['feature_dataset'] = 'land'

    # Infant mortality       
    elif 'InfantMort' in layerName:
      tags['boundary'] = 'infant_mortality' 
      tags['feature_dataset'] = 'medical_health'

    # Industrial coverage       
    elif 'Industrial_Coverage' in layerName:
      tags['boundary'] = 'industrial_coverage' 
      tags['feature_dataset'] = 'economy'

    # internet cafe 
    elif 'InternetCafe' in layerName or 'Internet_Cafe' in layerName:
      tags['amenity'] = 'cafe' 
      tags['internet_access'] = 'internet_cafe' 
      tags['feature_dataset'] = 'communications'

    # irrigation areas       
    elif 'Irrigated_Areas' in layerName or 'IrrigationZone' in layerName:
      tags['waterway'] = 'irrigation_area' 
      tags['feature_dataset'] = 'water_resources'

    # islamic association       
    elif 'IslamicAssn' in layerName or 'IslamicOrg' in layerName:
      tags['boundary'] = 'islamic_assn' 
      tags['feature_dataset'] = 'religion'

    # jakarta train stations       
    elif 'Jakarta_Train_Station' in layerName:
      tags['public_transport'] = 'jakarta_train_station' 
      tags['feature_dataset'] = 'transportation'

    # landscape       
    elif 'Landscape' in layerName:
      tags['boundary'] = 'landscape' 
      tags['feature_dataset'] = 'land'

    # landslide       
    elif 'Landslide' in layerName:
      tags['place'] = 'landslide_location' 
      tags['feature_dataset'] = 'significant_events'

    # landcover       
    elif 'Landcover' in layerName:
      tags['landuse'] = 'landcover' 
      tags['feature_dataset'] = 'land'

    # landuse       
    elif 'Landuse' in layerName:
      tags['landuse'] = 'landuse' 
      tags['feature_dataset'] = 'land'

    # lakes and wetlands       
    elif 'Lake' in layerName or 'Wetland' in layerName:
      tags['natural'] = 'water' 
      tags['feature_dataset'] = 'water_resources'

    # languages 
    elif 'Language' in layerName:
      tags['boundary'] = 'languages' 
      tags['feature_dataset'] = 'languages'

    # library 
    elif 'Libraries' in layerName:
      tags['amenity'] = 'library' 
      tags['feature_dataset'] = 'miscellaneous'

    # languages 
    elif 'LivelihoodZones' in layerName:
      tags['boundary'] = 'livelihood_zone' 
      tags['feature_dataset'] = 'demographics'

    # livestock migration routes
    elif 'LivestockMigration' in layerName:
      tags['route'] = 'livestock_migration_route' 
      tags['feature_dataset'] = 'transportation'

    # locations       
    elif 'Location' in layerName:
      tags['place'] = 'location' 
      tags['feature_dataset'] = 'admin_boundaries'

    # major cities
    elif 'MajorCities_Unemployment' in layerName:
      tags['boundary'] = 'major_cities_unemployment' 
      tags['feature_dataset'] = 'demographics'

    # malaria cases
    elif 'MalariaCase' in layerName or 'MalariaOutbreak' in layerName or '_Malaria' in layerName:
      tags['boundary'] = 'malaria_cases' 
      tags['feature_dataset'] = 'medical_health'

    # major roads       
    elif 'Major_Road' in layerName:
      if 'Primary' in attrs['USE_DESC']:
        tags['highway'] = 'primary' 
      elif 'Secondary' in attrs['USE_DESC']:
        tags['highway'] = 'secondary' 
      tags['feature_dataset'] = 'transportation'

    # marine poi       
    elif 'Marine_POI' in layerName:
      tags['place'] = 'marine_poi' 
      tags['feature_dataset'] = 'critical_infrastructure'

    # marriage
    elif 'Marriage' in layerName:
      tags['boundary'] = 'marriage' 
      tags['feature_dataset'] = 'demographics'

    # marz capital       
    elif 'Marz_Capital' in layerName:
      tags['place'] = 'marz_capital' 
      tags['feature_dataset'] = 'miscellaneous'

    # media outlets
    elif 'MediaOutlets' in layerName or 'Media_Outlet' in layerName:
      tags['office'] = 'media_outlet' 
      tags['feature_dataset'] = 'communications'

    # medical facilities
    elif 'Medical' in layerName or 'HealthCare' in layerName or 'MedicFac' in layerName:
      tags['amenity'] = 'clinic' 
      tags['feature_dataset'] = 'medical_health'

    # metro stations
    elif 'MetroStation' in layerName:
      tags['amenity'] = 'metro_station' 
      tags['feature_dataset'] = 'transportation'

    # manila metro routes 
    elif 'ManilaMetroRoute' in layerName:
      tags['route'] = 'metro_route' 
      tags['feature_dataset'] = 'transportation'
      tags['grouping'] = 'manila'

    # manila metro stations
    elif 'ManilaMetroStation' in layerName:
      tags['amenity'] = 'metro_station' 
      tags['feature_dataset'] = 'transportation'
      tags['grouping'] = 'manila'

    # migratory patterns nomads
    elif 'MigratoryPatternsNomads' in layerName:
      tags['route'] = 'migratory_patterns_nomads' 
      tags['feature_dataset'] = 'ethnicity'

    # military
    elif 'Military' in layerName:
      tags['landuse'] = 'military' 
      tags['feature_dataset'] = 'miscellaneous'

    # mineral deposits 
    elif 'Mineral' in layerName:
      tags['place'] = 'mineral_deposit' 
      tags['feature_dataset'] = 'land'

    # mine companies 
    elif 'Mine_Companies' in layerName:
      tags['place'] = 'mine_company' 
      tags['feature_dataset'] = 'land'

    # mines 
    elif 'Mines' in layerName:
      tags['place'] = 'mine' 
      tags['feature_dataset'] = 'land'

    # mining companies 
    elif 'MiningCompanies' in layerName or 'Mining' in layerName:
      tags['place'] = 'mining_companies' 
      tags['feature_dataset'] = 'land'

    # miscellaneous poi 
    elif 'Misc_Places_of_Interest' in layerName:
      tags['place'] = 'poi' 
      tags['feature_dataset'] = 'miscellaneous'

    # monasteries 
    elif 'Monasteries' in layerName:
      tags['amenity'] = 'place_of_worship' 
      tags['feature_dataset'] = 'religion'

    # national parks pts       
    elif 'National_Park_Pts' in layerName:
      tags['place'] = 'national_park_poi' 
      tags['feature_dataset'] = 'land'

    # national parks       
    elif 'NationalPark' in layerName or 'National_Park' in layerName:
      tags['boundary'] = 'national_park' 
      tags['feature_dataset'] = 'economy'

    # natural      
    elif 'Natural_POI' in layerName:
      tags['natural'] = 'natural_poi' 
      tags['feature_dataset'] = 'land'

    # natural disaster     
    elif 'NaturalDisaster' in layerName:
      tags['boundary'] = 'natural_disasters' 
      tags['feature_dataset'] = 'significant_events'

    # natural features     
    elif 'Natural_Feature' in layerName:
      tags['natural'] = attrs['TYPE'] 
      tags['feature_dataset'] = 'land'

    # natural resources       
    elif 'NaturalResources' in layerName or 'Natural_Resources' in layerName:
      tags['place'] = 'natural_resources' 
      tags['feature_dataset'] = 'land'

    # natural resources limestone       
    elif 'NaturalResourcesLimestone' in layerName:
      tags['place'] = 'natural_resources_limestone' 
      tags['feature_dataset'] = 'land'

    # natural resources mineral exploration
    elif 'NaturalResourcesMineralExploration' in layerName:
      tags['place'] = 'natural_resources_mineral_exploration' 
      tags['feature_dataset'] = 'land'

    # newspapers
    elif 'Newspaper' in layerName:
      tags['place'] = 'newspaper' 
      tags['feature_dataset'] = 'miscellaneous'

    # niamey districts
    elif 'NiameyDistrict' in layerName:
      tags['boundary'] = 'niamey_districts'
      tags['feature_dataset'] = 'admin_boundaries'

    # niamey gas stations 
    elif 'Niamey_Gas_Stations' in layerName:
      tags['place'] = 'niamey_gas_stations'
      tags['feature_dataset'] = 'miscellaneous'

    # niamey government buildings 
    elif 'Niamey_Government_Building' in layerName:
      tags['place'] = 'niamey_government_buildings'
      tags['feature_dataset'] = 'miscellaneous'

    # niamey government buildings 
    elif 'Niamey_Religious_Institution' in layerName:
      tags['place'] = 'niamey_religious_institutions'
      tags['feature_dataset'] = 'religion'

    # NGO 
    elif 'NGO' in layerName:
      tags['office'] = 'ngo' 
      tags['feature_dataset'] = 'groups'

    # npc approved surveys 
    elif 'NPCApprovedSurvey' in layerName:
      tags['place'] = 'npc_approved_survey' 
      tags['feature_dataset'] = 'miscellaneous'

    # Oil and Gas 
    elif 'OilandGas_Points' in layerName or 'PetroleumFacilities' in layerName or 'Oil_Gas' in layerName:
      tags['man_made'] = 'petroleum_facility' 
      tags['feature_dataset'] = 'miscellaneous'

    # Oil operations 
    elif 'OilOperation' in layerName:
      tags['place'] = 'oil_operation' 
      tags['feature_dataset'] = 'miscellaneous'

    # Oil terminals marine 
    elif 'OilTerminalsMarine' in layerName:
      tags['place'] = 'oil_terminals_marine' 
      tags['feature_dataset'] = 'land'

    # Oil refinery 
    elif 'OilRefinery' in layerName or 'OilRefineries' in layerName:
      tags['man_made'] = 'petroleum_facility' 
      tags['feature_dataset'] = 'land'

    # Oil exploitation zones 
    elif 'OilExploitationZone' in layerName:
      tags['boundary'] = 'petroleum_exploitation_zone' 
      tags['feature_dataset'] = 'land'

    # Oil and Gas fields 
    elif 'OilandGas_Polygons' in layerName or 'PetrolFields' in layerName or 'Oilfield' in layerName or 'Gasfield' in layerName or 'OilField' in layerName or 'OilandGasField' in layerName or 'OilBasin' in layerName or 'OilGasField' in layerName:
      tags['landuse'] = 'basin' 
      tags['feature_dataset'] = 'land'

    # open water 
    elif 'Open_Water' in layerName:
      tags['boundary'] = 'open_water' 
      tags['feature_dataset'] = 'water_resources'

    # palestinian refugee camps 
    elif 'Palestinian_Refugee_Camp' in layerName:
      tags['boundary'] = 'palestinian_refugee_camp' 
      tags['feature_dataset'] = 'groups'

    # pan american highway 
    elif 'PanAmericanHWY' in layerName:
      tags['highway'] = 'primary' 
      tags['feature_dataset'] = 'transportation'
      tags['grouping'] = 'pan_american_highway'

    # parliamentary voting 
    elif 'ParliamentaryVoting' in layerName:
      tags['boundary'] = 'parliamentary_voting' 
      tags['feature_dataset'] = 'groups'

    # peaks 
    elif 'Peaks' in layerName:
      tags['natural'] = 'peak' 
      tags['feature_dataset'] = 'land'

    # petroleum lines
    elif 'PetroleumLine' in layerName or 'Oilline' in layerName or 'OilGasPipeline' in layerName or 'Gasline' in layerName:
      tags['man_made'] = 'pipeline' 
      tags['feature_dataset'] = 'miscellaneous'

    # poi 
    elif 'POI' in layerName:
      tags['place'] = 'poi' 
      tags['feature_dataset'] = 'miscellaneous'

    # police 
    elif 'PoliceStation' in layerName or 'Police' in layerName:
      tags['amenity'] = 'police' 
      tags['feature_dataset'] = 'miscellaneous'

    # political party 
    elif 'PoliticalParty' in layerName or 'PoliticalAffiliation' in layerName or 'PoliticalParties' in layerName:
      tags['boundary'] = 'political' 
      tags['feature_dataset'] = 'miscellaneous'

    # political seats 
    elif 'Political_RegionalSeat' in layerName:
      tags['boundary'] = 'political_regional_seat' 
      tags['feature_dataset'] = 'groups'

    # political seats percent
    elif 'Political_RegionalSeatsPercent' in layerName:
      tags['boundary'] = 'political_regional_seats_percent' 
      tags['feature_dataset'] = 'groups'

    # Ports 
    elif 'Ports' in layerName:
      tags['man_made'] = 'port' 
      tags['feature_dataset'] = 'transportation'

    # post office
    elif 'PostOffice' in layerName:
      tags['amenity'] = 'post_office' 
      tags['feature_dataset'] = 'miscellaneous'

    # potential flood areas
    elif 'Potential_Flood' in layerName:
      tags['place'] = 'potential_flood_location'
      tags['feature_dataset'] = 'significant_events'

    # Power line 
    elif 'Powerlines' in layerName:
      tags['power'] = 'line' 
      tags['feature_dataset'] = 'miscellaneous'

    # Power Plants 
    elif 'PowerPlants' in layerName or 'Powerplants' in layerName or 'Power_Plant' in layerName or 'PowerStation' in layerName or 'PowerTowersStation' in layerName:
      tags['power'] = 'plant' 
      tags['feature_dataset'] = 'miscellaneous'

    # precipitation 
    elif 'Precipitation' in layerName:
      tags['boundary'] = 'precipitation' 
      tags['feature_dataset'] = 'water_resources'

    # presidential voting 
    elif 'PresidentialVoting' in layerName:
      tags['boundary'] = 'presidential_voting' 
      tags['feature_dataset'] = 'groups'

    # prison 
    elif 'Prison' in layerName:
      tags['amenity'] = 'prison' 
      tags['feature_dataset'] = 'miscellaneous'

    # protected habit 
    elif 'Protected_Habitat' in layerName or 'Protected_Area' in layerName or 'ProtectedArea' in layerName:
      tags['boundary'] = 'protected_habitat' 
      tags['feature_dataset'] = 'land'

    # public security 
    elif 'Public_Security' in layerName:
      tags['place'] = 'public_security_poi' 
      tags['feature_dataset'] = 'miscellaneous'

    # public transport 
    elif 'PublicTransport' in layerName or 'PublicTransRoutes' in layerName or 'PublicTransStation' in layerName or 'Public_Transportation' in layerName or 'TransportationStop' in layerName or 'Transportation_Route' in layerName:
      if 'TYPE' in attrs:
        tags['public_transport'] = attrs['TYPE'] 
      elif 'TYPE_2' in attrs: 
        tags['public_transport'] = attrs['TYPE_2'] 
      tags['feature_dataset'] = 'transportation'

    # radio stations 
    elif 'Radio_Station' in layerName: 
      tags['man_made'] = 'radio_station' 
      tags['feature_dataset'] = 'communications'

    # railways 
    elif 'Railways_POI' in layerName: 
      tags['place'] = 'railway_poi' 
      tags['feature_dataset'] = 'transportation'

    # railroad stations 
    elif 'RailroadStation' in layerName or 'RailwayStation' in layerName: 
      tags['railway'] = 'station' 
      tags['feature_dataset'] = 'transportation'

    # railways 
    elif 'Rail' in layerName or 'Railroad' in layerName: 
      tags['route'] = 'railway' 
      tags['feature_dataset'] = 'transportation'

    # rare earth mines deposits       
    elif 'RareEarth_Mines_Deposit' in layerName:
      tags['place'] = 'rare_earth_mines_deposits' 
      tags['feature_dataset'] = 'land'

    # recreation poi       
    elif 'Recreation_POI' in layerName:
      tags['place'] = 'recreation_poi' 
      tags['feature_dataset'] = 'miscellaneous'

    # refugee       
    elif 'Refugee' in layerName:
      tags['place'] = 'refugee_location' 
      tags['feature_dataset'] = 'groups'

    # regions poverty 
    elif 'Regions_Poverty' in layerName:
      tags['boundary'] = 'regions_poverty' 
      tags['feature_dataset'] = 'demographics'

    # Religious affiliation 
    elif 'Religion' in layerName or 'ReligiousAffiliation' in layerName or 'Religious_Breakdown' in layerName:
      tags['boundary'] = 'religious_affiliation' 
      tags['feature_dataset'] = 'religion'

    # Religious institution 
    elif 'ReligiousInstitutions' in layerName or 'Religious_Institutions' in layerName:
      tags['amenity'] = 'place_of_worship' 
      tags['feature_dataset'] = 'religion'

    # Reservoir 
    elif 'Reservoir' in layerName:
      tags['landuse'] = 'reservoir' 
      tags['feature_dataset'] = 'land'

    # Rivers 
    elif 'River' in layerName:
      tags['waterway'] = 'river' 
      tags['feature_dataset'] = 'water_resources'

    # River basins 
    elif 'River_Basin' in layerName:
      tags['landuse'] = 'river_basin' 
      tags['feature_dataset'] = 'water_resources'

    # Roads 
    elif 'Roads' in layerName:
      if 'ROAD_TYPE' in attrs:
        tags['highway'] = attrs['ROAD_TYPE'] 
      elif 'F_CODE_DES' in attrs:
        tags['highway'] = attrs['F_CODE_DES'] 
      else:
        tags['highway'] = attrs['TYPE'] 
      tags['feature_dataset'] = 'transportation'

    # Roads all 
    elif 'Roads_All' in layerName:
      tags['highway'] = attrs['TYPE'] 
      tags['feature_dataset'] = 'transportation'
      tags['grouping'] = 'roads_all'

    # Roads main 
    elif 'Roads_Main' in layerName:
      tags['highway'] = attrs['TYPE'] 
      tags['feature_dataset'] = 'transportation'
      tags['grouping'] = 'roads_main'

    # sahel
    elif 'Sahel' in layerName:
      tags['boundary'] = 'sahel' 
      tags['feature_dataset'] = 'land'

    # sahel migration
    elif 'SahelMigration' in layerName:
      tags['boundary'] = 'sahel_migration' 
      tags['feature_dataset'] = 'significant_events'

    # salt pan
    elif 'SaltPan' in layerName:
      tags['landuse'] = 'salt_pond' 
      tags['feature_dataset'] = 'land'

    # soils
    elif 'Soils' in layerName:
      tags['boundary'] = 'soils' 
      tags['feature_dataset'] = 'natural_features'

    # smuggling cities 
    elif 'SmugglingCities' in layerName:
      tags['place'] = 'smuggling_cities' 
      tags['feature_dataset'] = 'transportation'

    # smuggling Points 
    elif 'SmugglingPoints' in layerName or 'SmugglingHubs' in layerName or 'Smuggling_' in layerName:
      tags['place'] = 'smuggling_poi' 
      tags['feature_dataset'] = 'transportation'

    # smuggling polygon 
    elif 'Smuggling_Polygon' in layerName:
      tags['boundary'] = 'smuggling_area' 
      tags['feature_dataset'] = 'transportation'

    # smuggling routes 
    elif 'SmugglingRoute' in layerName or 'Smuggling_Route' in layerName or 'Smuggling_Polyline' in layerName:
      tags['boundary'] = 'smuggling_route' 
      tags['feature_dataset'] = 'transportation'

    # Social capital
    elif 'SocialCapital' in layerName:
      tags['boundary'] = 'social_capital' 
      tags['feature_dataset'] = 'groups'

    # sports 
    elif '_Sports' in layerName:
      tags['sport'] = attrs['TYPE'] 
      tags['feature_dataset'] = 'miscellaneous'

    # spring 
    elif 'Spring' in layerName:
      tags['natural'] = 'spring' 
      tags['feature_dataset'] = 'water_resources'

    # state religion 
    elif 'State_Religion' in layerName:
      tags['boundary'] = 'state_religion' 
      tags['feature_dataset'] = 'religion'

    # state reserve 
    elif 'State_Reserve' in layerName:
      tags['boundary'] = 'state_reserve' 
      tags['feature_dataset'] = 'land'

    # state sanctuary 
    elif 'State_Sanctuaries' in layerName:
      tags['boundary'] = 'state_sanctuary' 
      tags['feature_dataset'] = 'land'

    # substations (power)
    elif 'Substation' in layerName:
      tags['power'] = 'substation' 
      tags['feature_dataset'] = 'miscellaneous'

    # surficial geology
    elif 'Surficial_Geology' in layerName:
      tags['geological'] = 'surficial_geology' 
      tags['feature_dataset'] = 'land'

    # surge hazard 
    elif 'SurgeHazard' in layerName:
      tags['boundary'] = 'surge_hazard'
      tags['feature_dataset'] = 'significant_events'

    # surrounding countries
    elif 'Surrounding_Countries' in layerName:
      tags['boundary'] = 'surrounding_countries' 
      tags['feature_dataset'] = 'miscellaneous'

    # Survey companies 
    elif 'Survey_Companies' in layerName or 'SurveyCompanies' in layerName or 'SURVEYCOMP' in layerName:
      tags['office'] = 'survey_company' 
      tags['feature_dataset'] = 'land'
 
    # TB mortality rates 
    elif 'TB_MortalityRate' in layerName: 
      tags['boundary'] = 'tb_mortality_rates' 
      tags['feature_dataset'] = 'medical_health'

    # television stations 
    elif 'Television_Station' in layerName: 
      tags['man_made'] = 'television_station' 
      tags['feature_dataset'] = 'communications'

    # tv radio stations 
    elif 'TVRadioStation' in layerName: 
      tags['man_made'] = 'television_and_radio_station' 
      tags['feature_dataset'] = 'communications'

    # temperatures 
    elif 'Temperature' in layerName:
      tags['boundary'] = 'temperature' 
      tags['feature_dataset'] = 'miscellaneous'

    # terrorist activities
    elif 'Terrorism' in layerName or 'TerroristAttacks' in layerName or 'TerroristActivity' in layerName or 'TerroristEvents' in layerName:
      if 'TYPE' in attrs:
        tags['activity'] = attrs['TYPE'] 
      elif 'ATTACK_TYP' in attrs:
        tags['activity'] = attrs['ATTACK_TYP']
      elif 'ATK_TYPE' in attrs:
        tags['activity'] = attrs['ATK_TYPE']
      tags['feature_dataset'] = 'miscellaneous'
 
    # tourist attractions 
    elif 'Tourism' in layerName or 'TouristSite' in layerName or 'TouristLoc' in layerName or 'TouristAttraction' in layerName or 'TouristArea' in layerName or 'TouristLocation' in layerName or 'Tourist_Location' in layerName:
      if 'TYPE' in attrs:
        tags['tourism'] = attrs['TYPE'] 
      elif 'DESCRIPT' in attrs:
        tags['tourism'] = attrs['DESCRIPT']
      tags['feature_dataset'] = 'economy'
 
    # train stations       
    elif 'TrainStation' in layerName:
      tags['public_transport'] = 'train_station' 
      tags['feature_dataset'] = 'transportation'

    # transmission lines       
    elif 'Transmission_Line' in layerName:
      tags['power'] = 'line' 
      tags['feature_dataset'] = 'miscellaneous'

    # tree       
    elif 'Tree' in layerName:
      tags['boundary'] = 'tree_coverage' 
      tags['feature_dataset'] = 'land'

    # tribal points       
    elif 'TribalPoints' in layerName:
      tags['place'] = 'tribal_poi' 
      tags['feature_dataset'] = 'ethnicity'

    # tribe 
    elif 'Tribe' in layerName:
      tags['boundary'] = 'tribe'
      tags['feature_dataset'] = 'ethnicity'

    # tsunami hazard 
    elif 'TsunamiHazard' in layerName:
      tags['boundary'] = 'tsunami_hazard'
      tags['feature_dataset'] = 'significant_events'

    # tundra      
    elif 'Tundra' in layerName:
      tags['boundary'] = 'tundra' 
      tags['feature_dataset'] = 'land'

    # typhoons
    elif 'Typhoon' in layerName:
      tags['boundary'] = 'typhoon'
      tags['feature_dataset'] = 'significant_events'

    # UNAMID UN Agency Presence       
    elif 'UNAMID' in layerName or 'UNAgency' in layerName:
      tags['place'] = 'un_presence_location' 
      tags['feature_dataset'] = 'miscellaneous'

    # UN Mission locations       
    elif 'UNMission' in layerName or 'UNPresence' in layerName:
      tags['place'] = 'un_mission_location' 
      tags['feature_dataset'] = 'miscellaneous'

    # unhcr poi's       
    elif 'UNHCR_POIs' in layerName:
      tags['place'] = 'unhcr_poi' 
      tags['reference_point'] = 'yes' 
      tags['feature_dataset'] = 'groups'

    # universities       
    elif 'Universities' in layerName or 'University' in layerName:
      tags['amenity'] = 'university' 
      tags['feature_dataset'] = 'education'

    # uranium mines 
    elif 'UraniumMines' in layerName:
      tags['place'] = 'mine' 
      tags['feature_dataset'] = 'land'
      tags['grouping'] = 'uranium'

    # USAID Projects       
    elif 'USAID' in layerName:
      tags['place'] = 'usaid_poi' 
      tags['feature_dataset'] = 'economy'

    # vegetation       
    elif 'Vegetation' in layerName:
      tags['landuse'] = 'vegetation' 
      tags['feature_dataset'] = 'land'

    # volcanos       
    elif 'Volcanoes' in layerName:
      tags['natural'] = 'volcano' 
      tags['feature_dataset'] = 'land'

    # voter turnout 
    elif 'VoterTurnout' in layerName or 'VoterData' in layerName:
      tags['boundary'] = 'voter_turnout'
      tags['feature_dataset'] = 'groups'

    # water areas       
    elif 'Water_Areas' in layerName or 'WaterAreas' in layerName or 'WaterAreas_NonPerrenial' in layerName or 'WaterPoly' in layerName or 'Hydrology_Polygon' in layerName:
      if 'TYPE' in attrs:
        tags['waterway'] = attrs['TYPE'] 
      elif 'F_CODE_DES' in attrs:
        tags['waterway'] = attrs['F_CODE_DES']
      tags['feature_dataset'] = 'water_resources'

    # water falls 
    elif 'Waterfalls_Islands_Dams' in layerName:
      tags['place'] = 'waterfalls_islands_dams'
      tags['feature_dataset'] = 'water_resources'

    # water lines       
    elif 'Water_Lines' in layerName or 'WaterLines' in layerName or 'Hydrology_Polyline' in layerName:
      if 'F_CODE_DES' in attrs:
        tags['waterway'] = attrs['F_CODE_DES'] 
      else:
        tags['waterway'] = 'water_line' 
      tags['feature_dataset'] = 'water_resources'

    # weather annual days of rain 
    elif 'Weather_Annual_Days_of_Rain' in layerName:
      tags['boundary'] = 'weather_annual_days_of_rain'
      tags['feature_dataset'] = 'miscellaneous'

    # weather annual hl temp 
    elif 'Weather_Annual_HL_Temp' in layerName:
      tags['boundary'] = 'weather_annual_hl_temp'
      tags['feature_dataset'] = 'miscellaneous'

    # weather annual rainfall 
    elif 'Weather_Annual_Rainfall' in layerName:
      tags['boundary'] = 'weather_annual_rainfall'
      tags['feature_dataset'] = 'miscellaneous'

    # weather humidity 
    elif 'Weather_Humidity' in layerName:
      tags['boundary'] = 'weather_humidity'
      tags['feature_dataset'] = 'miscellaneous'

    # wells 
    elif 'Wells' in layerName or 'WellLocation' in layerName:
      tags['man_made'] = 'water_well'
      tags['feature_dataset'] = 'water_resources'

    # yellow fever 
    elif 'YellowFeverRisk' in layerName:
      tags['boundary'] = 'yellow_fever_risk'
      tags['feature_dataset'] = 'medical_health'

    # Useful when debugging. You can see print statements on stdout when Hootenanny is running
    #print "Input: " + str(attrs)
    #print "Output: " + str(tags)

    # Return our translated tags
    return tags
