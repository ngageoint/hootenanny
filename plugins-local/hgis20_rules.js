/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
    HGISv20 One2one rules
*/

hgis20.rules = {
    // Tags/Attributes for individual layers
    // layerIn = Rules for Import ONLY
    // layerCommon = Rules for Import and Export
    // layerOut = Rules for Export ONLY. NOTE: These get swapped around before use
    layerIn : [
//      ['XtableName','ADMIN0','XXX','XXX'],
//      ['XtableName','Admin0_Access_To_Services','XXX','XXX'],
//      ['XtableName','Admin0_Education','XXX','XXX'],
//      ['XtableName','Admin0_Employment','XXX','XXX'],
//      ['XtableName','Admin0_Household_Goods','XXX','XXX'],
//      ['XtableName','Admin0_Mortality','XXX','XXX'],
//      ['XtableName','Admin0_Population','XXX','XXX'],
//      ['XtableName','Admin0_Poverty','XXX','XXX'],
//      ['XtableName','ADMIN1','XXX','XXX'],
//      ['XtableName','Admin1_Access_To_Services','XXX','XXX'],
//      ['XtableName','Admin1_Education','XXX','XXX'],
//      ['XtableName','Admin1_Employment','XXX','XXX'],
//      ['XtableName','Admin1_Household_Goods','XXX','XXX'],
//      ['XtableName','Admin1_Mortality','XXX','XXX'],
//      ['XtableName','Admin1_Population','XXX','XXX'],
//      ['XtableName','Admin1_Poverty','XXX','XXX'],
//      ['XtableName','ADMIN2','XXX','XXX'],
//      ['XtableName','Admin2_Access_To_Services','XXX','XXX'],
//      ['XtableName','Admin2_Education','XXX','XXX'],
//      ['XtableName','Admin2_Employment','XXX','XXX'],
//      ['XtableName','Admin2_Household_Goods','XXX','XXX'],
//      ['XtableName','Admin2_Mortality','XXX','XXX'],
//      ['XtableName','Admin2_Population','XXX','XXX'],
//      ['XtableName','Admin2_Poverty','XXX','XXX'],
//      ['XtableName','ADMIN3','XXX','XXX'],
//      ['XtableName','ADMIN4','XXX','XXX'],
//      ['XtableName','ADMIN5','XXX','XXX'],
//      ['XtableName','Agricultural_POI','XXX','XXX'],
//      ['XtableName','Airfield_POI','XXX','XXX'],
//      ['XtableName','Airfields','aeroway','aerodrome'],
//      ['XtableName','Border_Crossings','barrier','border_control'],
//      ['XtableName','Bridges_Tunnels','XXX','XXX'],
//      ['XtableName','Commercial_POI','XXX','XXX'],
//      ['XtableName','Conflict_Points','hgis:conflict_point','yes'],
//      ['XtableName','Crime_Data','hgis:crime_point','yes'],
//      ['XtableName','Cultural_POI','XXX','XXX'],
//      ['XtableName','Dams','waterway','dam'],
     ['XtableName','Educational_Institutions','amenity','school'],
//      ['XtableName','Electrical_POI','XXX','XXX'],
//      ['XtableName','Embassies','XXX','XXX'],
//      ['XtableName','Ethnicity','ethnicity_data','yes'],
//      ['XtableName','Foreign_Investment_Projects','XXX','XXX'],
//      ['XtableName','Free_Trade_Zones','free_trade_zone','yes'],
     ['XtableName','Free_Trade_Zones_Polygons','place','free_trade_zone'],
//      ['XtableName','Geonames','XXX','XXX'],
//      ['XtableName','GNDB','XXX','XXX'],
//      ['XtableName','Government_POI','XXX','XXX'],
//      ['XtableName','HADR','hgis:hdar','yes'],
//      ['XtableName','Health_Events','hgis:health_point','yes'],
//      ['XtableName','Hydrology_POI','XXX','XXX'],
     ['XtableName','Hydrology_Polygons','waterway','lake'],
     ['XtableName','Hydrology_Polylines','waterway','river'],
//      ['XtableName','Internet_Cafes','amenity','internet_cafe'],
//      ['XtableName','Languages','language_data','yes'],
//      ['XtableName','Libraries','amenity','library'],
//      ['XtableName','Lodging_POI','XXX','XXX'],
//      ['XtableName','Marine_POI','XXX','XXX'],
//      ['XtableName','Media_Outlets','XXX','XXX'],
//      ['XtableName','Medical_Facilities','XXX','XXX'],
//      ['XtableName','Military_Installations','military','installation'],
//      ['XtableName','Natural_POI','XXX','XXX'],
//      ['XtableName','Natural_Resources','XXX','XXX'],
//      ['XtableName','Places_POI','XXX','XXX'],
//      ['XtableName','Ports','landuse','port'],
//      ['XtableName','Power_Plants','power','plant'],
//      ['XtableName','Prisons','amenity','prison'],
//      ['XtableName','Protected_Areas','protected_area','yes'],
//      ['XtableName','Public_POI','XXX','XXX'],
//      ['XtableName','Public_Security_POI','XXX','XXX'],
//      ['XtableName','Public_Transportation_POI','XXX','XXX'],
//      ['XtableName','Railways','railway','rail'],
//      ['XtableName','Railways_POI','XXX','XXX'],
//      ['XtableName','Recreation_POI','XXX','XXX'],
//      ['XtableName','Refugee_Camps','XXX','XXX'],
//      ['XtableName','Religion','XXX','XXX'],
//      ['XtableName','Religious_Institutions','amenity','place_of_worship'],
//      ['XtableName','Road_POI','XXX','XXX'],
//      ['XtableName','Roads','highway','road'],
//      ['XtableName','Smuggling_Routes','smuggling_route','yes'],
//      ['XtableName','Smuggling_Routes_Polyline','XXX','XXX'],
//      ['XtableName','Soils','geological','soil_surface_region'], // Link to TDSv61
//      ['XtableName','Tourist_Locations_POI','XXX','XXX'],
    ], // End layerIn

    layerCommon : [
//      ['XtableName','ADMIN0','XXX','XXX'],
//      ['XtableName','Admin0_Access_To_Services','XXX','XXX'],
//      ['XtableName','Admin0_Education','XXX','XXX'],
//      ['XtableName','Admin0_Employment','XXX','XXX'],
//      ['XtableName','Admin0_Household_Goods','XXX','XXX'],
//      ['XtableName','Admin0_Mortality','XXX','XXX'],
//      ['XtableName','Admin0_Population','XXX','XXX'],
//      ['XtableName','Admin0_Poverty','XXX','XXX'],
//      ['XtableName','ADMIN1','XXX','XXX'],
//      ['XtableName','Admin1_Access_To_Services','XXX','XXX'],
//      ['XtableName','Admin1_Education','XXX','XXX'],
//      ['XtableName','Admin1_Employment','XXX','XXX'],
//      ['XtableName','Admin1_Household_Goods','XXX','XXX'],
//      ['XtableName','Admin1_Mortality','XXX','XXX'],
//      ['XtableName','Admin1_Population','XXX','XXX'],
//      ['XtableName','Admin1_Poverty','XXX','XXX'],
//      ['XtableName','ADMIN2','XXX','XXX'],
//      ['XtableName','Admin2_Access_To_Services','XXX','XXX'],
//      ['XtableName','Admin2_Education','XXX','XXX'],
//      ['XtableName','Admin2_Employment','XXX','XXX'],
//      ['XtableName','Admin2_Household_Goods','XXX','XXX'],
//      ['XtableName','Admin2_Mortality','XXX','XXX'],
//      ['XtableName','Admin2_Population','XXX','XXX'],
//      ['XtableName','Admin2_Poverty','XXX','XXX'],
//      ['XtableName','ADMIN3','XXX','XXX'],
//      ['XtableName','ADMIN4','XXX','XXX'],
//      ['XtableName','ADMIN5','XXX','XXX'],
//      ['XtableName','Agricultural_POI','XXX','XXX'],
//      ['XtableName','Airfield_POI','XXX','XXX'],
     ['XtableName','Airfields','aeroway','aerodrome'],
     ['XtableName','Border_Crossings','barrier','border_control'],
//      ['XtableName','Bridges_Tunnels','XXX','XXX'],
//      ['XtableName','Commercial_POI','XXX','XXX'],
     ['XtableName','Conflict_Points','hgis:conflict_point','yes'],
     ['XtableName','Crime_Data','hgis:crime_point','yes'],
//      ['XtableName','Cultural_POI','XXX','XXX'],
     ['XtableName','Dams','waterway','dam'],
//      ['XtableName','Educational_Institutions','XXX','XXX'],
     ['XtableName','Electrical_POI','power','generator'],
//      ['XtableName','Embassies','XXX','XXX'],
//      ['XtableName','Ethnicity','ethnicity_data','yes'],
     ['XtableName','Foreign_Investment_Projects','foreign_investment','yes'],
     ['XtableName','Free_Trade_Zones','place','free_trade_zone'],
//      ['XtableName','Free_Trade_Zones_Polygons','free','XXX'],
//      ['XtableName','Geonames','XXX','XXX'],
//      ['XtableName','GNDB','XXX','XXX'],
//      ['XtableName','Government_POI','XXX','XXX'],
     ['XtableName','HADR','hgis:hdar','yes'],
     ['XtableName','Health_Events','hgis:health_point','yes'],
//      ['XtableName','Hydrology_POI','XXX','XXX'],
//      ['XtableName','Hydrology_Polygons','XXX','XXX'],
//      ['XtableName','Hydrology_Polylines','XXX','XXX'],
     ['XtableName','Internet_Cafes','amenity','internet_cafe'],
     ['XtableName','Languages','language_data','yes'],
     ['XtableName','Libraries','amenity','library'],
//      ['XtableName','Lodging_POI','XXX','XXX'],
//      ['XtableName','Marine_POI','XXX','XXX'],
//      ['XtableName','Media_Outlets','XXX','XXX'],
//      ['XtableName','Medical_Facilities','XXX','XXX'],
     ['XtableName','Military_Installations','military','installation'],
//      ['XtableName','Natural_POI','XXX','XXX'],
//      ['XtableName','Natural_Resources','XXX','XXX'],
//      ['XtableName','Places_POI','XXX','XXX'],
     ['XtableName','Ports','landuse','port'],
     ['XtableName','Power_Plants','power','plant'],
     ['XtableName','Prisons','amenity','prison'],
     ['XtableName','Protected_Areas','protected_area','yes'],
//      ['XtableName','Public_POI','XXX','XXX'],
//      ['XtableName','Public_Security_POI','XXX','XXX'],
//      ['XtableName','Public_Transportation_POI','XXX','XXX'],
     ['XtableName','Railways','railway','rail'],
//      ['XtableName','Railways_POI','XXX','XXX'],
//      ['XtableName','Recreation_POI','XXX','XXX'],
     ['XtableName','Refugee_Camps','refugee','yes'],
     ['XtableName','Religion','demographic','religion'],
     ['XtableName','Religious_Institutions','amenity','place_of_worship'],
//      ['XtableName','Road_POI','XXX','XXX'],
     ['XtableName','Roads','highway','road'],
     ['XtableName','Smuggling_Routes','smuggling_route','yes'],
//      ['XtableName','Smuggling_Routes_Polyline','XXX','XXX'],
     ['XtableName','Soils','geological','soil_surface_region'], // Link to TDSv61
//      ['XtableName','Tourist_Locations_POI','XXX','XXX'],
    ], // End layerCommon

    layerOut : [
//      ['XtableName','ADMIN0','XXX','XXX'],
//      ['XtableName','Admin0_Access_To_Services','XXX','XXX'],
//      ['XtableName','Admin0_Education','XXX','XXX'],
//      ['XtableName','Admin0_Employment','XXX','XXX'],
//      ['XtableName','Admin0_Household_Goods','XXX','XXX'],
//      ['XtableName','Admin0_Mortality','XXX','XXX'],
//      ['XtableName','Admin0_Population','XXX','XXX'],
//      ['XtableName','Admin0_Poverty','XXX','XXX'],
//      ['XtableName','ADMIN1','XXX','XXX'],
//      ['XtableName','Admin1_Access_To_Services','XXX','XXX'],
//      ['XtableName','Admin1_Education','XXX','XXX'],
//      ['XtableName','Admin1_Employment','XXX','XXX'],
//      ['XtableName','Admin1_Household_Goods','XXX','XXX'],
//      ['XtableName','Admin1_Mortality','XXX','XXX'],
//      ['XtableName','Admin1_Population','XXX','XXX'],
//      ['XtableName','Admin1_Poverty','XXX','XXX'],
//      ['XtableName','ADMIN2','XXX','XXX'],
//      ['XtableName','Admin2_Access_To_Services','XXX','XXX'],
//      ['XtableName','Admin2_Education','XXX','XXX'],
//      ['XtableName','Admin2_Employment','XXX','XXX'],
//      ['XtableName','Admin2_Household_Goods','XXX','XXX'],
//      ['XtableName','Admin2_Mortality','XXX','XXX'],
//      ['XtableName','Admin2_Population','XXX','XXX'],
//      ['XtableName','Admin2_Poverty','XXX','XXX'],
//      ['XtableName','ADMIN3','XXX','XXX'],
//      ['XtableName','ADMIN4','XXX','XXX'],
//      ['XtableName','ADMIN5','XXX','XXX'],
//      ['XtableName','Agricultural_POI','XXX','XXX'],
//      ['XtableName','Airfield_POI','XXX','XXX'],
//      ['XtableName','Airfields','aeroway','aerodrome'],
//      ['XtableName','Border_Crossings','barrier','border_control'],
     ['XtableName','Bridges_Tunnels','bridge','yes'],
     ['XtableName','Bridges_Tunnels','tunnel','yes'],
//      ['XtableName','Commercial_POI','XXX','XXX'],
//      ['XtableName','Conflict_Points','XXX','XXX'],
//      ['XtableName','Crime_Data','XXX','XXX'],
//      ['XtableName','Cultural_POI','XXX','XXX'],
//      ['XtableName','Dams','waterway','dam'],
     ['XtableName','Educational_Institutions','amenity','school'],
//      ['XtableName','Electrical_POI','XXX','XXX'],
//      ['XtableName','Embassies','XXX','XXX'],
//      ['XtableName','Ethnicity','XXX','XXX'],
//      ['XtableName','Foreign_Investment_Projects','XXX','XXX'],
//      ['XtableName','Free_Trade_Zones','XXX','XXX'],
//      ['XtableName','Free_Trade_Zones_Polygons','XXX','XXX'],
//      ['XtableName','Geonames','XXX','XXX'],
//      ['XtableName','GNDB','XXX','XXX'],
//      ['XtableName','Government_POI','XXX','XXX'],
//      ['XtableName','HADR','XXX','XXX'],
//      ['XtableName','Health_Events','XXX','XXX'],
//      ['XtableName','Hydrology_POI','XXX','XXX'],
//      ['XtableName','Hydrology_Polygons','XXX','XXX'],
//      ['XtableName','Hydrology_Polylines','XXX','XXX'],
//      ['XtableName','Internet_Cafes','amenity','internet_cafe'],
//      ['XtableName','Languages','XXX','XXX'],
//      ['XtableName','Libraries','amenity','library'],
//      ['XtableName','Lodging_POI','XXX','XXX'],
//      ['XtableName','Marine_POI','XXX','XXX'],
//      ['XtableName','Media_Outlets','XXX','XXX'],
//      ['XtableName','Medical_Facilities','XXX','XXX'],
//      ['XtableName','Military_Installations','military','installation'],
//      ['XtableName','Natural_POI','XXX','XXX'],
//      ['XtableName','Natural_Resources','XXX','XXX'],
//      ['XtableName','Places_POI','XXX','XXX'],
//      ['XtableName','Ports','landuse','port'],
//      ['XtableName','Power_Plants','power','plant'],
//      ['XtableName','Prisons','amenity','prison'],
//      ['XtableName','Protected_Areas','XXX','XXX'],
//      ['XtableName','Public_POI','XXX','XXX'],
//      ['XtableName','Public_Security_POI','XXX','XXX'],
//      ['XtableName','Public_Transportation_POI','XXX','XXX'],
//      ['XtableName','Railways','XXX','XXX'],
//      ['XtableName','Railways_POI','XXX','XXX'],
//      ['XtableName','Recreation_POI','XXX','XXX'],
//      ['XtableName','Refugee_Camps','XXX','XXX'],
//      ['XtableName','Religion','XXX','XXX'],
//      ['XtableName','Religious_Institutions','amenity','place_of_worship'],
//      ['XtableName','Road_POI','XXX','XXX'],
//      ['XtableName','Roads','XXX','XXX'],
//      ['XtableName','Smuggling_Routes','XXX','XXX'],
//      ['XtableName','Smuggling_Routes_Polyline','XXX','XXX'],
//      ['XtableName','Soils','XXX','XXX'],
//      ['XtableName','Tourist_Locations_POI','XXX','XXX'],
    ], // End layerOut

    // ##### Start of One2One Rules #####

    // One2one rules for Text Fields
    txtBiased : {
     'ACTOR':'crime:actor',
     'ACTOR1':'conflict:actor1',
     'ACTOR2':'conflict:actor2',
     'ADDRESS':'addr:full', // Address
     'ADM0_NAME':'is_in:country',
     'ADM1_NAME':'admin_level1:name', // Could be a state, county etc
     'ADM2_NAME':'admin_level2:name',
     'ADM3_NAME':'admin_level3:name',
     'ADM4_NAME':'admin_level4:name',
     'ADM5_NAME':'admin_level5:name',
     'admin1$TYPE1':'admin_level1:type1', // No idea what these are
     'admin2$TYPE1':'admin_level2:type1',
     'admin2$TYPE2':'admin_level2:type2',
     'admin3$TYPE1':'admin_level3:type1',
     'admin3$TYPE2':'admin_level3:type2',
     'admin3$TYPE3':'admin_level3:type3',
     'admin4$TYPE1':'admin_level4:type1',
     'admin4$TYPE2':'admin_level4:type2',
     'admin4$TYPE3':'admin_level4:type3',
     'admin4$TYPE4':'admin_level4:type4',
     'admin5$TYPE1':'admin_level5:type1',
     'admin5$TYPE2':'admin_level5:type2',
     'admin5$TYPE3':'admin_level5:type3',
     'admin5$TYPE4':'admin_level5:type4',
     'admin5$TYPE4':'admin_level5:type5',
     'ALL_COM':'natural_resources:all_com',
     'ALT_NAME':'alt_name', // Geographic Name Information : Full Name
     'ALT_NAMES':'ethnicity:alt_names',
     'ANCH_DEPTH':'port:anchor_depth',
     'APP_DATE':'foreign_investment:app_date',
     'AREA_AFF':'hadr:area_affected',
     'CAPACITY':'capacity',
     'CARBCHMOOR':'port:cargo:beach_mooring',
     'CARGMDMOOR':'port:cargo:med_mooring',
     'CARGO_ANCH':'port:cargo:anchorage',
     'CARGODEPTH':'port:cargo:depth',
     'CARGOWHARF':'port:cargo:wharf',
     'CARICEMOOR':'port:cargo:ice_mooring',
     'CHAN_DEPTH':'port:channel_depth',
     'CITY':'is_in:city', // City Name
     'COMM_AIR':'port:air_communication_available',
     'COMMENTS':'note', // Note : Memorandum
     'COMM_FAX':'port:fax_available',
     'COMM_PHONE':'port:phone_available',
     'COMM_RADIO':'port:radio_available',
     'COMM_RAIL':'port:rail_communication_available',
     'COMM_VHF':'port:vhf_available',
     'COUNTRY':'crossing_to_country',
     'CRANEFIXED':'port:fixed_crane',
     'CRANEFLOAT':'port:floating_crane',
     'CRANEMOBIL':'port:mobile_crane',
     'DATE':'date',
     'DECKSUPPLY':'port:deck_supply_available',
     'DEGAUSS':'port:degauss_available',
     'DESCRIP':'foreign_investment:description',
     'DESIG':'protected_area:designation',
     'DESIG_TYPE':'protected_area:designation_type',
     'DIESEL':'port:diesel_available',
     'DOMSOI':'soil:domsoi',
     'DRTYBALLST':'port:dirty_ballast_removal_available',
     'DRYDOCK':'port:drydock_size',
     'ELECREPAIR':'port:electrical_repair_available',
     'ELECTRICAL':'port:electrical_port_services_available',
     'END_DT':'date:end',
     'ENG_SUPPLY':'port:engine_supply_available',
     'ENGTYPE0':'raw:ENGTYPE0', // Need more info on this
     'ENGTYPE1':'raw:ENGTYPE1',
     'ENGTYPE2':'raw:ENGTYPE2',
     'ENGTYPE3':'raw:ENGTYPE3',
     'ENGTYPE4':'raw:ENGTYPE4',
     'ENGTYPE5':'raw:ENGTYPE5',
     'ENTRY_ICE':'port:entry:ice',
     'ENTRYOTHER':'port:entry:other',
     'ENTRYSWELL':'port:entry:swell',
     'ENTRY_TIDE':'port:entry:tide',
     'ETAMESSAGE':'port:eta_message_required',
     'FAOSOIL':'soil:faosoil',
     'FUEL_OIL':'port:fuel_oil_available',
     'FULL_NAME':'name:full',
     'GARBAGE':'port:garbage_disposal_available',
     'GOV_TYPE':'governance_type',
     'HARBORSIZE':'port:harbour_size',
     'HARBORTYPE':'port:harbour_type',
     'HEIGHT':'height',
     'HOLDGROUND':'port:holdground',
     'IATA':'iata',
     'ICAO':'icao',
     'IUCN_CAT':'ref:iucn',
     'LANG_FAM':'language:family', // Supposed to be a coded domain
     'LANG_SUBGR':'language:subgroup', // Supposed to be a coded domain
     'LANGUAGE':'language',
     'LEND_INSTR':'foreign_investment:lending_instrument',
     'LIFT_0_24':'port:0_24ton_crane_available',
     'LIFT_100_O':'port:100ton_and_over_crane_available',
     'LIFT_25_49':'port:25 to 49ton_crane_available',
     'LIFT50_100':'port:50_to_100ton_crane_available',
     'LOC_ASSIST':'port:local_assistance_available',
     'LOCATION':'conflict:location',
     'LONGSHORE':'port:longshore_services_available',
     'MAJ_NAME':'name:major_name',
     'MANAG_AUTH':'manageing_authority',
     'MAX_VESSEL':'port:maximum_vessel_length',
     'MED_FACIL':'port:medical_facilities',
     'MODIFY_DAT':'geonames:modified_date',
     'MODIFY_DT':'gndb:modified_date',
     'NAME':'name', // Geographic Name Information : Full Name
     'NAME_FAO':'name:fao',
     'NAME_ISO':'name:iso',
     'NAME_LOCAL':'name:local',
     'NAME_VARIA':'name:variation',
     'NAV_EQUIP':'port:navigation_equipment_services_available',
     'NT':'gndb:nt',
     'NUM_GEN':'generator:number', // Number of Generators
     'OIL_DEPTH':'port:oil_depth', // Description in the spec doesn't make much sense
     'ORG_SOURCE':'conflict:source_information',
     'OVERHD_LIM':'port:overhead_limit',
     'PEOPLEGP_1':'ethnicity:people_group1', // Supposed to be coded values
     'PEOPLEGP_2':'ethnicity:people_group2',
     'PEOPLEGP_3':'ethnicity:people_group3',
     'PEOPLEGP_4':'ethnicity:people_group4',
     'PEOPLEGP_5':'ethnicity:people_group5',
     'PILOTADVSD':'port:pilot_advised',
     'PILOTAVAIL':'port:pilot_available',
     'PILOT_REQD':'port:pilot_required',
     'PORT_NAME':'port:name',
     'PORTOFENTR':'port:port_of_entry',
     'PRATIQUE':'port:pratique', // Quarantine required before arrival?
     'PROVISIONS':'port:provisions_available',
     'QUAR_OTHER':'port:other_quarantine_requirements',
     'RAILWAY':'port:repair_railway_size',
     'REPAIRCODE':'port:repair_code',
     'RIVER':'river_name',
     'ROUTES':'smuggling:routes', // ??? difference between this and the other smuggling route fields
     'SECTOR':'foreign_investment:sector',
     'SERV_STEAM':'port:steam_services_available',
     'SHELTER':'port:shelter',
     'SORT_NAME':'geonames:sort:name',
     'SOURCE':'source', // Source Information : Source Description
     'SOURCE2':'source:2', // Source Information : Source Description
     'SOURCE_DT':'source:datetime', // Source Information : Source Date and Time
     'SOURCE2_DT':'source:datetime:2', // Source Information : Source Date and Time
     'SSCC_CERT':'port:sscc_certificate_required',
     'START_DT':'date:start',
     'SUB_NAME':'name:sub_name',
     'TELEPHONE':'phone',
     'TOT_CAP_MW':'generator:output:electricity', // Generator Output
     'TUG_ASSIST':'port:tug_assist',
     'TUGSALVAGE':'port:tug_salvage',
     'TURN_BASIN':'port:turn_basin',
     'UNIT':'military:unit_name',
     'US_REP':'port:US_representative_available',
     'VALIDFR':'admin_level0:validfr',
     'WEBSITE':'website',
     'YR_OPER':'year_operational',
    }, // End txtBiased

    // One2one ruled for Number fields
    numBiased : {
     'ACC_ELEC':'demographic:electricity',
     'ACC_H2O':'demographic:water',
     'ACC_INT':'demographic:internet_access',
     'ACC_PHOL':'demographic:landline_telephone_access',
     'ACC_PHOM':'demographic:mobile_telephone_access',
     'ACC_RAD':'demographic:radio_access',
     'ACC_SEW':'demographic:sewerage',
     'ACC_TV':'demographic:television_access',
     'AGE_15_65':'demographic:population:age_15_65',
     'AGE_65OVR':'demographic:population:age_over_65',
     'AGE_UNDR15':'demographic:population:age_under_15',
     'AGR_SEC':'demographic:agr_sec',
     'APP_YEAR':'foreign_investment:app_year',
     'ASSETS':'poverty:assets',
     'CBR':'demographic:cbr',
     'CDR':'demographic:cdr',
     'CONFLICT':'poverty:conflict',
     'CORRUPT':'poverty:corrupt',
     'DGMDPI':'poverty:dgmdpi',
     'ECO_ACT_F':'demographic:eco_act_f',
     'ECO_ACT_M':'demographic:eco_act_m',
     'ECO_ACT_T':'demographic:eco_act_t',
     'EDU_FAC':'poverty:edu_fac',
     'ELECTRIC':'poverty:electric',
     'EST_PRIS':'prison:num_prisoners', // Number of prisoners
     'FATALITY':'fatalities', // Number of fatalities
     'GINI':'poverty:gini',
     'HEALTH_FAC':'poverty:health_fac',
     'HH_AST':'demographic:hh_ast',
     'HIV':'demographic:hiv',
     'HOUSEHOLDS':'demographic:households',
     'HR':'poverty:hr',
     'HUMSEC_FAC':'poverty:humsec_fac',
     'IMR':'demographic:imr',
     'LF_EXP':'demographic:life_expectancy',
     'LFPR':'demographic:lfpr',
     'LIT_RATE':'demographic:literacy_rate',
     'LIT_RT_F':'demographic:literacy_rate:female',
     'LIT_RT_M':'demographic:literacy_rate:male',
     'LIT_RT_T':'demographic:literacy_rate:total',
     'MAN_SEC':'poverty:man_sec',
     'MAT_FAC':'poverty:mat_fac',
     'MAT_MR':'poverty:mat_mr',
     'NAT_FAC':'poverty:nat_fac',
     'NO_TK_AREA':'protected_area:no_tk_area',
     'N_RUNWAYS':'runway:count',
     'NUM_DTH':'conflict:number_of_deaths',
     'NUM_INJ':'conflict:number_injured',
     'PER_POP_F':'demographic:population:percent_female',
     'PER_POP_M':'demographic:population:percent_male',
     'PER_RUL':'demographic:population:percent_rural',
     'PER_URB':'demographic:population:percent_urban',
     'POP_DEN':'demographic:population:density',
     'POP_T':'demographic:population:total',
     'POPULATION':'refugee:population',
     'PREV_DIS':'poverty:prev_dis',
     'PRI_COMPL':'demographic:primary_school:completion',
     'PRI_ENROL':'demographic:primary_school:enrollment',
     'PRIM_ED':'poverty:prim_ed',
     'PUB_SEC':'demographic:pub_sec',
     'REP_AREA':'protected_area:rep_area',
     'REP_M':'protected_area:rep_m',
     'RES_SEC':'demographic:res_sec',
     'R_LENGTH':'length',
     'R_WIDTH':'width',
     'SEC_COMPL':'demographic:secondary_school:completion',
     'SEC_ENROL':'demographic:secondary_school:enrollment',
     'SEV_SEC':'demographic:sev_sec',
     'STATUS_YR':'protected_area:status_year',
     'STRAHLER':'strahler',
     'TER_COMPL':'demographic:tertiary_school:completion',
     'TER_ENROL':'demographic:tertiary_school:enrollment',
     'TERROR':'poverty:terror',
     'TERT_ED':'poverty:tert_ed',
     'TFR':'demographic:tfr',
     'TIDE_RANGE':'port:tide_range',
     'TL_AFFECT':'total_affected',
     'TOT_BOYS':'school:student:male',
     'TOT_CLASS':'school:classes',
     'TOT_F_TEAC':'school:teacher:female',
     'TOT_GIRLS':'school:student:female',
     'TOT_M_TEAC':'school:teacher:male',
     'U5_MR':'demographic:u5_mr',
     'UE':'poverty:ue',
     'UN5_MR':'demographic:un5_mr',
     'UNEDUCATED':'demographic:uneducated',
     'UNEMP_F':'demographic:unemployment:female',
     'UNEMP_M':'demographic:unemployment:male',
     'UNEMP_T':'demographic:unemployment:total',
     'VALUE':'foreign_investment:value',
     'WATER':'port:water_available',
     'YEAR':'year',
    }, // End numBiased
    
    

    fuzyone2one :[
      ['commercial$TYPE2','Bar',schemaTools.isSimilar('amenity=bar', 0.8, 0.1, 0.5)],
      ['religioni$TYPE','Mosque',schemaTools.isSimilar('building=mosque', 0.8, 0.1, 0.5)],
      ['commercial$TYPE2','Market',schemaTools.isSimilar('amenity=marketplace', 0.8, 0.1, 0.5)],
      ['commercial$TYPE2','Beauty Salon',schemaTools.isSimilar('shop=beauty', 0.8, 0.1, 0.5)],
      ['commercial$TYPE2','Delicatessen',schemaTools.isSimilar('shop=deli', 0.8, 0.1, 0.5)],
      ['commercial$TYPE2','Grocery',schemaTools.isSimilar('shop=grocery', 0.8, 0.1, 0.5)],
      ['commercial$TYPE2','Restaurant',schemaTools.isSimilar('amenity=restaurant', 0.8, 0.1, 0.5)],
      ['commercial$TYPE2','Store',schemaTools.isSimilar('shop=general', 0.8, 0.1, 0.5)],
      ['commercial$TYPE2','Tower',schemaTools.isSimilar('man_made=tower', 0.8, 0.1, 0.5)],
      ['cultural$TYPE','Memorial',schemaTools.isSimilar('historic=memorial', 0.8, 0.1, 0.5)],
      ['cultural$TYPE','Monument',schemaTools.isSimilar('historic=monument', 0.8, 0.1, 0.5)],
      ['cultural$TYPE','Theatre',schemaTools.isSimilar('amenity=theatre', 0.8, 0.1, 0.5)],
      ['cultural$TYPE','Burial Site',schemaTools.isSimilar('amenity=grave_yard', 0.8, 0.1, 0.5)],
      ['hydroa$TYPE','River',schemaTools.isSimilar('waterway=river', 0.8, 0.1, 0.5)],
      ['lodging$TYPE2','Guest House',schemaTools.isSimilar('tourism=guest_house', 0.8, 0.1, 0.5)],
      ['medical$TYPE1','Hospital',schemaTools.isSimilar('amenity=hospital', 0.8, 0.1, 0.5)],
      ['medical$TYPE1','Pharmacy',schemaTools.isSimilar('amenity=pharmacy', 0.8, 0.1, 0.5)],
      ['religioni$TYPE','Church',schemaTools.isSimilar('building=church', 0.8, 0.1, 0.5)],
      ['tourist$TYPE','Library',schemaTools.isSimilar('amenity=library', 0.8, 0.1, 0.5)],
      ['tourist$TYPE','Casino',schemaTools.isSimilar('amenity=casino', 0.8, 0.1, 0.5)],
      ['religion$TYPE1','0',schemaTools.isSimilar('religion=christian', 0.8, 0.1, 0.5)],
      ['religion$TYPE1','3',schemaTools.isSimilar('religion=buddhist', 0.8, 0.1, 0.5)]
    ],

    // Common one2one rules. Used for both import and export
    // taken from the HGISv20 spec.
    one2one : [
     // COM_TYPE - Commodity Type
     ['COM_TYPE','Both','commodity_type','Both'],
     ['COM_TYPE','Metallic','commodity_type','Metallic'],
     ['COM_TYPE','Non-Metallic','commodity_type','Non-Metallic'],

     // CONF_IMAGE - Confirmed with Imagery
     ['CONF_IMAGE','assessed','hgis:imagery_confirmed','assessed'],
     ['CONF_IMAGE','confirmed','hgis:imagery_confirmed','confirmed'],
     ['CONF_IMAGE','reported','hgis:imagery_confirmed','reported'],
     ['CONF_IMAGE','unconfirmed','hgis:imagery_confirmed','unconfirmed'],

     // CUSTOMS - At Airport
     ['airfield$CUSTOMS','Yes','hgis:customs','yes'], // Fix in custom rules
     ['airfield$CUSTOMS','No','hgis:customs','no'],

     // DEV_STAT - Natural Resources
     ['DEV_STAT','Occurrence','development_status','Occurrence'],
     ['DEV_STAT','Past Producer','development_status','Past Producer'],
     ['DEV_STAT','Plant','development_status','Plant'],
     ['DEV_STAT','Producer','development_status','Producer'],
     ['DEV_STAT','Prospect','development_status','Prospect'],

     // DISEASE -
     ['DISEASE','acute jaundice syndrome','disease','acute_jaundice_syndrome'],
     ['DISEASE','acute neurological syndrome','disease','acute_neurological_syndrome'],
     ['DISEASE','acute respiratory syndrome','disease','acute_respiratory_syndrome'],
     ['DISEASE','acute watery diarrhoeal syndrome','disease','acute_watery_diarrhoeal_syndrome'],
     ['DISEASE','bubonic plague','disease','bubonic_plague'],
     ['DISEASE','cerebro spinal','disease','cerebro_spinal'],
     ['DISEASE','chikungunya','disease','chikungunya'],
     ['DISEASE','cholera','disease','cholera'],
     ['DISEASE','cutaneous leishmaniasis','disease','cutaneous_leishmaniasis'],
     ['DISEASE','dengue','disease','dengue'],
     ['DISEASE','ebola','disease','ebola'],
     ['DISEASE','equine encephalitis','disease','equine_encephalitis'],
     ['DISEASE','flu','disease','flu'],
     ['DISEASE','hantavirus','disease','hantavirus'],
     ['DISEASE','lassa fever','disease','lassa_fever'],
     ['DISEASE','leptospirosis','disease','leptospirosis'],
     ['DISEASE','malaria','disease','malaria'],
     ['DISEASE','measles','disease','measles'],
     ['DISEASE','meningitis','disease','meningitis'],
     ['DISEASE','meningococcal disease','disease','meningococcal'],
     ['DISEASE','neonatal tetanus','disease','neonatal_tetanus'],
     ['DISEASE','parasitic infectious disease','disease','parasitic_infectious_disease'],
     ['DISEASE','pertussis','disease','pertussis'],
     ['DISEASE','polio','disease','polio'],
     ['DISEASE','poliomyelitis','disease','poliomyelitis'],
     ['DISEASE','rabies','disease','rabies'],
     ['DISEASE','rift valley fever','disease','rift_valley_fever'],
     ['DISEASE','total tetanus','disease','total tetanus'],
     ['DISEASE','tuberculosis','disease','tuberculosis'],
     ['DISEASE','yellow fever','disease','yellow_fever'],

     // DSG - From Geonames & Geonames translation
//      ['DSG','ADM1','poi','first-order_administrative_division'],
//      ['DSG','ADM2','poi','second-order_administrative_division'],
//      ['DSG','PPL','place','populated'], // Populated Place
//      ['DSG','PPLA','poi','seat_of_a_first-order_administrative_division'],
//      ['DSG','PPLA2','poi','seat_of_a_second-order_administrative_division'],
//      ['DSG','PPLC','capital','yes'], // Capital of a political entity
//      ['DSG','PPLL','place','hamlet'], // Populated Locality
//      ['DSG','PPLW','distroyed:place','populated'], // Destroyed populated place
//      ['DSG','PPLX','place','neighbourhood'], // Section of a populated place

     // EVENT_TYPE - Conflict Points
     ['EVENT_TYPE','armed assault','conflict:event','armed_assault'],
     ['EVENT_TYPE','arson','conflict:event','arson'],
     ['EVENT_TYPE','assassination','conflict:event','assassination'],
     ['EVENT_TYPE','battle - government regains territory','conflict:event','battle - government regains territory'],
     ['EVENT_TYPE','battle - no change of territory','conflict:event','battle - no change of territory'],
     ['EVENT_TYPE','battle - rebels overtake territory','conflict:event','battle - rebels overtake territory'],
     ['EVENT_TYPE','bombing/explosion','conflict:event','bombing/explosion'],
     ['EVENT_TYPE','communication','conflict:event','communication'],
     ['EVENT_TYPE','facility/infrastructure attack','conflict:event','facility/infrastructure attack'],
     ['EVENT_TYPE','financing','conflict:event','financing'],
     ['EVENT_TYPE','headquarters or base established','conflict:event','headquarters or base established'],
     ['EVENT_TYPE','hijacking','conflict:event','hijacking'],
     ['EVENT_TYPE','hostage taking (barricade incident)','conflict:event','hostage taking (barricade incident)'],
     ['EVENT_TYPE','hostage taking (kidnapping)','conflict:event','hostage taking (kidnapping)'],
     ['EVENT_TYPE','military operations','conflict:event','military operations'],
     ['EVENT_TYPE','non-violent activity by a conflict actor','conflict:event','non-violent activity by a conflict actor'],
     ['EVENT_TYPE','non-violent transfer of territory','conflict:event','non-violent transfer of territory'],
     ['EVENT_TYPE','police operations','conflict:event','police operations'],
     ['EVENT_TYPE','riots/protests','conflict:event','riots/protests'],
     ['EVENT_TYPE','robbery','conflict:event','robbery'],
     ['EVENT_TYPE','territorial incursion','conflict:event','territorial incursion'],
     ['EVENT_TYPE','torture','conflict:event','torture'],
     ['EVENT_TYPE','trafficking','conflict:event','trafficking'],
     ['EVENT_TYPE','unarmed assault','conflict:event','unarmed assault'],
     ['EVENT_TYPE','vandalism','conflict:event','vandalism'],
     ['EVENT_TYPE','violence against civilians','conflict:event','violence against civilians'],

     // FC - From Geonames
     ['FC','A','geonames:feature_class','a'], // Country, state, region
     ['FC','H','geonames:feature_class','h'], // stream, lake
     ['FC','L','geonames:feature_class','l'], // parks, area
     ['FC','P','geonames:feature_class','p'], // City, village
     ['FC','R','geonames:feature_class','r'], // Road, railway
     ['FC','S','geonames:feature_class','s'], // spot, building, farm
     ['FC','T','geonames:feature_class','t'], // mountain, hill, rock
     ['FC','U','geonames:feature_class','u'], // Undersea
     ['FC','V','geonames:feature_class','v'], // forest, heath

     // FI_STATUS - Foreign Investment status
     ['FI_STATUS','Active','foreign_investment:status','active'],
     ['FI_STATUS','Closed','foreign_investment:status','closed'],
     ['FI_STATUS','Dropped','foreign_investment:status','dropped'],
     ['FI_STATUS','Pipeline','foreign_investment:status','pipeline'],

     // HYDRO_DESC - Hydro Description Type
     ['HYDRO_DESC','Perennial/Permanent','intermittent','no'],
     ['HYDRO_DESC','Non-Perennial/Intermittent/Fluctuating','intermittent','yes'],

     // Taken from TDSv61: ISO3 - (Location Country) Designation : GENC Short URN-based Identifier
     // ['ISO3','other','is_in:country_code','other'], // Other
     ['ISO3','ABW','is_in:country_code','AW'], // Aruba
     ['ISO3','AFG','is_in:country_code','AF'], // Islamic Republic of Afghanistan
     ['ISO3','AGO','is_in:country_code','AO'], // RepublicFstate of Angola
     ['ISO3','AIA','is_in:country_code','AI'], // Anguilla
     ['ISO3','ALB','is_in:country_code','AL'], // Republic of Albania
     ['ISO3','AND','is_in:country_code','AD'], // Principality of Andorra
     ['ISO3','ARE','is_in:country_code','AE'], // United Arab Emirates
     ['ISO3','ARG','is_in:country_code','AR'], // Argentine Republic
     ['ISO3','ARM','is_in:country_code','AM'], // Republic of Armenia
     ['ISO3','ASM','is_in:country_code','AS'], // Territory of American Samoa
     ['ISO3','ATA','is_in:country_code','AQ'], // Antarctica
     ['ISO3','ATF','is_in:country_code','TF'], // French Southern and Antarctic Lands
     ['ISO3','ATG','is_in:country_code','AG'], // Antigua and Barbuda
     ['ISO3','AUS','is_in:country_code','AU'], // Commonwealth of Australia
     ['ISO3','AUT','is_in:country_code','AT'], // Republic of Austria
     ['ISO3','AX1','is_in:country_code','A1'], // Unknown
     ['ISO3','AX2','is_in:country_code','A2'], // Guantanamo Bay Naval Base
     ['ISO3','AZE','is_in:country_code','AZ'], // Republic of Azerbaijan
     ['ISO3','BDI','is_in:country_code','BI'], // Republic of Burundi
     ['ISO3','BEL','is_in:country_code','BE'], // Kingdom of Belgium
     ['ISO3','BEN','is_in:country_code','BJ'], // Republic of Benin
     ['ISO3','BES','is_in:country_code','BQ'], // Bonaire, Sint Eustatius, and Saba
     ['ISO3','BFA','is_in:country_code','BF'], // Burkina Faso
     ['ISO3','BGD','is_in:country_code','BD'], // Peoples Republic of Bangladesh
     ['ISO3','BGR','is_in:country_code','BG'], // Republic of Bulgaria
     ['ISO3','BHR','is_in:country_code','BH'], // Kingdom of Bahrain
     ['ISO3','BHS','is_in:country_code','BS'], // Commonwealth of The Bahamas
     ['ISO3','BIH','is_in:country_code','BA'], // Bosnia and Herzegovina
     ['ISO3','BLM','is_in:country_code','BL'], // Saint Barthelemy
     ['ISO3','BLR','is_in:country_code','BY'], // Republic of Belarus
     ['ISO3','BLZ','is_in:country_code','BZ'], // Belize
     ['ISO3','BMU','is_in:country_code','BM'], // Bermuda
     ['ISO3','BOL','is_in:country_code','BO'], // Plurinational State of Bolivia
     ['ISO3','BRA','is_in:country_code','BR'], // Federative Republic of Brazil
     ['ISO3','BRB','is_in:country_code','BB'], // Barbados
     ['ISO3','BRN','is_in:country_code','BN'], // Brunei Darussalam
     ['ISO3','BTN','is_in:country_code','BT'], // Kingdom of Bhutan
     ['ISO3','BVT','is_in:country_code','BV'], // Bouvet Island
     ['ISO3','BWA','is_in:country_code','BW'], // Republic of Botswana
     ['ISO3','CAF','is_in:country_code','CF'], // Central African Republic
     ['ISO3','CAN','is_in:country_code','CA'], // Canada
     ['ISO3','CCK','is_in:country_code','CC'], // Territory of Cocos (Keeling) Islands
     ['ISO3','CHE','is_in:country_code','CH'], // Swiss Confederation
     ['ISO3','CHL','is_in:country_code','CL'], // Republic of Chile
     ['ISO3','CHN','is_in:country_code','CN'], // Peoples Republic of China
     ['ISO3','CIV','is_in:country_code','CI'], // Republic of Côte dIvoire
     ['ISO3','CMR','is_in:country_code','CM'], // Republic of Cameroon
     ['ISO3','COD','is_in:country_code','CD'], // Democratic Republic of the Congo
     ['ISO3','COG','is_in:country_code','CG'], // Republic of the Congo
     ['ISO3','COK','is_in:country_code','CK'], // Cook Islands
     ['ISO3','COL','is_in:country_code','CO'], // Republic of Colombia
     ['ISO3','COM','is_in:country_code','KM'], // Union of the Comoros
     ['ISO3','CPT','is_in:country_code','CP'], // Clipperton Island
     ['ISO3','CPV','is_in:country_code','CV'], // Republic of Cape Verde
     ['ISO3','CRI','is_in:country_code','CR'], // Republic of Costa Rica
     ['ISO3','CUB','is_in:country_code','CU'], // Republic of Cuba
     ['ISO3','CUW','is_in:country_code','CW'], // Curaçao
     ['ISO3','CXR','is_in:country_code','CX'], // Territory of Christmas Island
     ['ISO3','CYM','is_in:country_code','KY'], // Cayman Islands
     ['ISO3','CYP','is_in:country_code','CY'], // Republic of Cyprus
     ['ISO3','CZE','is_in:country_code','CZ'], // Czech Republic
     ['ISO3','DEU','is_in:country_code','DE'], // Federal Republic of Germany
     ['ISO3','DGA','is_in:country_code','DG'], // Diego Garcia
     ['ISO3','DJI','is_in:country_code','DJ'], // Republic of Djibouti
     ['ISO3','DMA','is_in:country_code','DM'], // Commonwealth of Dominica
     ['ISO3','DNK','is_in:country_code','DK'], // Kingdom of Denmark
     ['ISO3','DOM','is_in:country_code','DO'], // Dominican Republic
     ['ISO3','DZA','is_in:country_code','DZ'], // Peoples Democratic Republic of Algeria
     ['ISO3','ECU','is_in:country_code','EC'], // Republic of Ecuador
     ['ISO3','EGY','is_in:country_code','EG'], // Arab Republic of Egypt
     ['ISO3','ERI','is_in:country_code','ER'], // State of Eritrea
     ['ISO3','ESH','is_in:country_code','EH'], // Western Sahara
     ['ISO3','ESP','is_in:country_code','ES'], // Kingdom of Spain
     ['ISO3','EST','is_in:country_code','EE'], // Republic of Estonia
     ['ISO3','ETH','is_in:country_code','ET'], // Federal Democratic Republic of Ethiopia
     ['ISO3','FIN','is_in:country_code','FI'], // Republic of Finland
     ['ISO3','FJI','is_in:country_code','FJ'], // Republic of Fiji
     ['ISO3','FLK','is_in:country_code','FK'], // Falkland Islands (Islas Malvinas)
     ['ISO3','FRA','is_in:country_code','FR'], // French Republic
     ['ISO3','FRO','is_in:country_code','FO'], // Faroe Islands
     ['ISO3','FSM','is_in:country_code','FM'], // Federated States of Micronesia
     ['ISO3','GAB','is_in:country_code','GA'], // Gabonese Republic
     ['ISO3','GBR','is_in:country_code','GB'], // United Kingdom of Great Britain and Northern Ireland
     ['ISO3','GEO','is_in:country_code','GE'], // Georgia
     ['ISO3','GGY','is_in:country_code','GG'], // Bailiwick of Guernsey
     ['ISO3','GHA','is_in:country_code','GH'], // Republic of Ghana
     ['ISO3','GIB','is_in:country_code','GI'], // Gibraltar
     ['ISO3','GIN','is_in:country_code','GN'], // Republic of Guinea
     ['ISO3','GLP','is_in:country_code','GP'], // Department of Guadeloupe
     ['ISO3','GMB','is_in:country_code','GM'], // Republic of The Gambia
     ['ISO3','GNB','is_in:country_code','GW'], // Republic of Guinea-Bissau
     ['ISO3','GNQ','is_in:country_code','GQ'], // Republic of Equatorial Guinea
     ['ISO3','GRC','is_in:country_code','GR'], // Hellenic Republic
     ['ISO3','GRD','is_in:country_code','GD'], // Grenada
     ['ISO3','GRL','is_in:country_code','GL'], // Greenland
     ['ISO3','GTM','is_in:country_code','GT'], // Republic of Guatemala
     ['ISO3','GUF','is_in:country_code','GF'], // Department of Guiana
     ['ISO3','GUM','is_in:country_code','GU'], // Territory of Guam
     ['ISO3','GUY','is_in:country_code','GY'], // Co-operative Republic of Guyana
     ['ISO3','HKG','is_in:country_code','HK'], // Hong Kong Special Administrative Region
     ['ISO3','HMD','is_in:country_code','HM'], // Territory of Heard Island and McDonald Islands
     ['ISO3','HND','is_in:country_code','HN'], // Republic of Honduras
     ['ISO3','HRV','is_in:country_code','HR'], // Republic of Croatia
     ['ISO3','HTI','is_in:country_code','HT'], // Republic of Haiti
     ['ISO3','HUN','is_in:country_code','HU'], // Hungary
     ['ISO3','IDN','is_in:country_code','ID'], // Republic of Indonesia
     ['ISO3','IMN','is_in:country_code','IM'], // Isle of Man
     ['ISO3','IND','is_in:country_code','IN'], // Republic of India
     ['ISO3','IOT','is_in:country_code','IO'], // British Indian Ocean Territory
     ['ISO3','IRL','is_in:country_code','IE'], // Ireland
     ['ISO3','IRN','is_in:country_code','IR'], // Islamic Republic of Iran
     ['ISO3','IRQ','is_in:country_code','IQ'], // Republic of Iraq
     ['ISO3','ISL','is_in:country_code','IS'], // Republic of Iceland
     ['ISO3','ISR','is_in:country_code','IL'], // State of Israel
     ['ISO3','ITA','is_in:country_code','IT'], // Italian Republic
     ['ISO3','JAM','is_in:country_code','JM'], // Jamaica
     ['ISO3','JEY','is_in:country_code','JE'], // Bailiwick of Jersey
     ['ISO3','JOR','is_in:country_code','JO'], // Hashemite Kingdom of Jordan
     ['ISO3','JPN','is_in:country_code','JP'], // Japan
     ['ISO3','KAZ','is_in:country_code','KZ'], // Republic of Kazakhstan
     ['ISO3','KEN','is_in:country_code','KE'], // Republic of Kenya
     ['ISO3','KGZ','is_in:country_code','KG'], // Kyrgyz Republic
     ['ISO3','KHM','is_in:country_code','KH'], // Kingdom of Cambodia
     ['ISO3','KIR','is_in:country_code','KI'], // Republic of Kiribati
     ['ISO3','KNA','is_in:country_code','KN'], // Federation of Saint Kitts and Nevis
     ['ISO3','KOR','is_in:country_code','KR'], // Republic of Korea
     ['ISO3','KWT','is_in:country_code','KW'], // State of Kuwait
     ['ISO3','LAO','is_in:country_code','LA'], // Lao Peoples Democratic Republic
     ['ISO3','LBN','is_in:country_code','LB'], // Lebanese Republic
     ['ISO3','LBR','is_in:country_code','LR'], // Republic of Liberia
     ['ISO3','LBY','is_in:country_code','LY'], // Libya
     ['ISO3','LCA','is_in:country_code','LC'], // Saint Lucia
     ['ISO3','LIE','is_in:country_code','LI'], // Principality of Liechtenstein
     ['ISO3','LKA','is_in:country_code','LK'], // Democratic Socialist Republic of Sri Lanka
     ['ISO3','LSO','is_in:country_code','LS'], // Kingdom of Lesotho
     ['ISO3','LTU','is_in:country_code','LT'], // Republic of Lithuania
     ['ISO3','LUX','is_in:country_code','LU'], // Grand Duchy of Luxembourg
     ['ISO3','LVA','is_in:country_code','LV'], // Republic of Latvia
     ['ISO3','MAC','is_in:country_code','MO'], // Macau Special Administrative Region
     ['ISO3','MAF','is_in:country_code','MF'], // Saint Martin
     ['ISO3','MAR','is_in:country_code','MA'], // Kingdom of Morocco
     ['ISO3','MCO','is_in:country_code','MC'], // Principality of Monaco
     ['ISO3','MDA','is_in:country_code','MD'], // Republic of Moldova
     ['ISO3','MDG','is_in:country_code','MG'], // Republic of Madagascar
     ['ISO3','MDV','is_in:country_code','MV'], // Republic of Maldives
     ['ISO3','MEX','is_in:country_code','MX'], // United Mexican States
     ['ISO3','MHL','is_in:country_code','MH'], // Republic of the Marshall Islands
     ['ISO3','MKD','is_in:country_code','MK'], // Republic of Macedonia
     ['ISO3','MLI','is_in:country_code','ML'], // Republic of Mali
     ['ISO3','MLT','is_in:country_code','MT'], // Republic of Malta
     ['ISO3','MMR','is_in:country_code','MM'], // Union of Burma
     ['ISO3','MNE','is_in:country_code','ME'], // Montenegro
     ['ISO3','MNG','is_in:country_code','MN'], // Mongolia
     ['ISO3','MNP','is_in:country_code','MP'], // Commonwealth of the Northern Mariana Islands
     ['ISO3','MOZ','is_in:country_code','MZ'], // Republic of Mozambique
     ['ISO3','MRT','is_in:country_code','MR'], // Islamic Republic of Mauritania
     ['ISO3','MSR','is_in:country_code','MS'], // Montserrat
     ['ISO3','MTQ','is_in:country_code','MQ'], // Department of Martinique
     ['ISO3','MUS','is_in:country_code','MU'], // Republic of Mauritius
     ['ISO3','MWI','is_in:country_code','MW'], // Republic of Malawi
     ['ISO3','MYS','is_in:country_code','MY'], // Malaysia
     ['ISO3','MYT','is_in:country_code','YT'], // Department of Mayotte
     ['ISO3','NAM','is_in:country_code','NA'], // Republic of Namibia
     ['ISO3','NCL','is_in:country_code','NC'], // New Caledonia
     ['ISO3','NER','is_in:country_code','NE'], // Republic of the Niger
     ['ISO3','NFK','is_in:country_code','NF'], // Territory of Norfolk Island
     ['ISO3','NGA','is_in:country_code','NG'], // Federal Republic of Nigeria
     ['ISO3','NIC','is_in:country_code','NI'], // Republic of Nicaragua
     ['ISO3','NIU','is_in:country_code','NU'], // Niue
     ['ISO3','NLD','is_in:country_code','NL'], // Kingdom of the Netherlands
     ['ISO3','NOR','is_in:country_code','NO'], // Kingdom of Norway
     ['ISO3','NPL','is_in:country_code','NP'], // Federal Democratic Republic of Nepal
     ['ISO3','NRU','is_in:country_code','NR'], // Republic of Nauru
     ['ISO3','NZL','is_in:country_code','NZ'], // New Zealand
     ['ISO3','OMN','is_in:country_code','OM'], // Sultanate of Oman
     ['ISO3','PAK','is_in:country_code','PK'], // Islamic Republic of Pakistan
     ['ISO3','PAN','is_in:country_code','PA'], // Republic of Panama
     ['ISO3','PCN','is_in:country_code','PN'], // Pitcairn, Henderson, Ducie, and Oeno Islands
     ['ISO3','PER','is_in:country_code','PE'], // Republic of Peru
     ['ISO3','PHL','is_in:country_code','PH'], // Republic of the Philippines
     ['ISO3','PLW','is_in:country_code','PW'], // Republic of Palau
     ['ISO3','PNG','is_in:country_code','PG'], // Independent State of Papua New Guinea
     ['ISO3','POL','is_in:country_code','PL'], // Republic of Poland
     ['ISO3','PRI','is_in:country_code','PR'], // Commonwealth of Puerto Rico
     ['ISO3','PRK','is_in:country_code','KP'], // Democratic Peoples Republic of Korea
     ['ISO3','PRT','is_in:country_code','PT'], // Portuguese Republic
     ['ISO3','PRY','is_in:country_code','PY'], // Republic of Paraguay
     ['ISO3','PSE','is_in:country_code','PS'], // Palestinian Territory
     ['ISO3','PYF','is_in:country_code','PF'], // French Polynesia
     ['ISO3','QAT','is_in:country_code','QA'], // State of Qatar
     ['ISO3','REU','is_in:country_code','RE'], // Department of Reunion
     ['ISO3','ROU','is_in:country_code','RO'], // Romania
     ['ISO3','RUS','is_in:country_code','RU'], // Russian Federation
     ['ISO3','RWA','is_in:country_code','RW'], // Republic of Rwanda
     ['ISO3','SAU','is_in:country_code','SA'], // Kingdom of Saudi Arabia
     ['ISO3','SDN','is_in:country_code','SD'], // Republic of the Sudan
     ['ISO3','SEN','is_in:country_code','SN'], // Republic of Senegal
     ['ISO3','SGP','is_in:country_code','SG'], // Republic of Singapore
     ['ISO3','SGS','is_in:country_code','GS'], // South Georgia and South Sandwich Islands
     ['ISO3','SHN','is_in:country_code','SH'], // Saint Helena, Ascension, and Tristan da Cunha
     ['ISO3','SLB','is_in:country_code','SB'], // Solomon Islands
     ['ISO3','SLE','is_in:country_code','SL'], // Republic of Sierra Leone
     ['ISO3','SLV','is_in:country_code','SV'], // Republic of El Salvador
     ['ISO3','SMR','is_in:country_code','SM'], // Republic of San Marino
     ['ISO3','SOM','is_in:country_code','SO'], // Somalia, Federal Republic of
     ['ISO3','SPM','is_in:country_code','PM'], // Territorial Collectivity of Saint Pierre and Miquelon
     ['ISO3','SRB','is_in:country_code','RS'], // Republic of Serbia
     ['ISO3','SSD','is_in:country_code','SS'], // Republic of South Sudan
     ['ISO3','STP','is_in:country_code','ST'], // Democratic Republic of Sao Tome and Principe
     ['ISO3','SUR','is_in:country_code','SR'], // Republic of Suriname
     ['ISO3','SVK','is_in:country_code','SK'], // Slovak Republic
     ['ISO3','SVN','is_in:country_code','SI'], // Republic of Slovenia
     ['ISO3','SWE','is_in:country_code','SE'], // Kingdom of Sweden
     ['ISO3','SWZ','is_in:country_code','SZ'], // Kingdom of Swaziland
     ['ISO3','SXM','is_in:country_code','SX'], // Sint Maarten
     ['ISO3','SYC','is_in:country_code','SC'], // Republic of Seychelles
     ['ISO3','SYR','is_in:country_code','SY'], // Syrian Arab Republic
     ['ISO3','TCA','is_in:country_code','TC'], // Turks and Caicos Islands
     ['ISO3','TCD','is_in:country_code','TD'], // Republic of Chad
     ['ISO3','TGO','is_in:country_code','TG'], // Togolese Republic
     ['ISO3','THA','is_in:country_code','TH'], // Kingdom of Thailand
     ['ISO3','TJK','is_in:country_code','TJ'], // Republic of Tajikistan
     ['ISO3','TKL','is_in:country_code','TK'], // Tokelau
     ['ISO3','TKM','is_in:country_code','TM'], // Turkmenistan
     ['ISO3','TLS','is_in:country_code','TL'], // Democratic Republic of Timor-Leste
     ['ISO3','TON','is_in:country_code','TO'], // Kingdom of Tonga
     ['ISO3','TTO','is_in:country_code','TT'], // Republic of Trinidad and Tobago
     ['ISO3','TUN','is_in:country_code','TN'], // Tunisian Republic
     ['ISO3','TUR','is_in:country_code','TR'], // Republic of Turkey
     ['ISO3','TUV','is_in:country_code','TV'], // Tuvalu
     ['ISO3','TWN','is_in:country_code','TW'], // Taiwan
     ['ISO3','TZA','is_in:country_code','TZ'], // United Republic of Tanzania
     ['ISO3','UGA','is_in:country_code','UG'], // Republic of Uganda
     ['ISO3','UKR','is_in:country_code','UA'], // Ukraine
     ['ISO3','URY','is_in:country_code','UY'], // Oriental Republic of Uruguay
     ['ISO3','USA','is_in:country_code','US'], // United States of America
     ['ISO3','UZB','is_in:country_code','UZ'], // Republic of Uzbekistan
     ['ISO3','VAT','is_in:country_code','VA'], // State of the Vatican City
     ['ISO3','VCT','is_in:country_code','VC'], // Saint Vincent and the Grenadines
     ['ISO3','VEN','is_in:country_code','VE'], // Bolivarian Republic of Venezuela
     ['ISO3','VGB','is_in:country_code','VG'], // Virgin Islands, British
     ['ISO3','VIR','is_in:country_code','VI'], // United States Virgin Islands
     ['ISO3','VNM','is_in:country_code','VN'], // Socialist Republic of Vietnam
     ['ISO3','VUT','is_in:country_code','VU'], // Republic of Vanuatu
     ['ISO3','WLF','is_in:country_code','WF'], // Wallis and Futuna
     ['ISO3','WSM','is_in:country_code','WS'], // Independent State of Samoa
     ['ISO3','XAC','is_in:country_code','XA'], // Territory of Ashmore and Cartier Islands
     ['ISO3','XAZ','is_in:country_code','XAZ'], // Entity 1
     ['ISO3','XBI','is_in:country_code','QS'], // Bassas da India
     ['ISO3','XBK','is_in:country_code','XB'], // Baker Island
     ['ISO3','XCR','is_in:country_code','XCR'], // Entity 2
     ['ISO3','XCS','is_in:country_code','XC'], // Coral Sea Islands Territory
     ['ISO3','XCY','is_in:country_code','XCY'], // Entity 3
     ['ISO3','XEU','is_in:country_code','XE'], // Europa Island
     ['ISO3','XGL','is_in:country_code','QX'], // Glorioso Islands
     ['ISO3','XGZ','is_in:country_code','XG'], // Gaza Strip
     ['ISO3','XHO','is_in:country_code','XH'], // Howland Island
     ['ISO3','XJA','is_in:country_code','XU'], // Johnston Atoll
     ['ISO3','XJM','is_in:country_code','XJ'], // Jan Mayen
     ['ISO3','XJN','is_in:country_code','QU'], // Juan de Nova Island
     ['ISO3','XJV','is_in:country_code','XQ'], // Jarvis Island
     ['ISO3','XKM','is_in:country_code','XKM'], // Entity 4
     ['ISO3','XKN','is_in:country_code','XKN'], // Entity 5
     ['ISO3','XKR','is_in:country_code','XM'], // Kingman Reef
     ['ISO3','XKS','is_in:country_code','XK'], // Republic of Kosovo
     ['ISO3','XMW','is_in:country_code','QM'], // Midway Islands
     ['ISO3','XNV','is_in:country_code','XV'], // Navassa Island
     ['ISO3','XPL','is_in:country_code','XL'], // Palmyra Atoll
     ['ISO3','XPR','is_in:country_code','XP'], // Paracel Islands
     ['ISO3','XQP','is_in:country_code','QP'], // Etorofu, Habomai, Kunashiri, and Shikotan Islands
     ['ISO3','XQZ','is_in:country_code','QZ'], // Akrotiri
     ['ISO3','XSP','is_in:country_code','XS'], // Spratly Islands
     ['ISO3','XSV','is_in:country_code','XR'], // Svalbard
     ['ISO3','XTR','is_in:country_code','XT'], // Tromelin Island
     ['ISO3','XWB','is_in:country_code','XW'], // West Bank
     ['ISO3','XWK','is_in:country_code','QW'], // Wake Island
     ['ISO3','XXD','is_in:country_code','XD'], // Dhekelia
     ['ISO3','XXX','is_in:country_code','[None]'], // No Mans Land
     ['ISO3','YEM','is_in:country_code','YE'], // Republic of Yemen
     ['ISO3','ZAF','is_in:country_code','ZA'], // Republic of South Africa
     ['ISO3','ZMB','is_in:country_code','ZM'], // Republic of Zambia
     ['ISO3','ZWE','is_in:country_code','ZW'], // Republic of Zimbabwe

     // New addition to the spec
     ['MIL_TYPE','Administrative Offices','military','administrative_offices'],
     ['MIL_TYPE','Air Force Academy','military','air_force_academy'],
     ['MIL_TYPE','Aircraft','military','aircraft'],
     ['MIL_TYPE','Aircraft Storage','military','aircraft_storage'],
     ['MIL_TYPE','Airfield','military','airfield'],
     ['MIL_TYPE','Analytical Laboratory','military','analytical_laboratory'],
     ['MIL_TYPE','Anti-Aircraft Site','military','anti-aircraft_site'],
     ['MIL_TYPE','Anti-Ship Site','military','anti-ship_site'],
     ['MIL_TYPE','Anti-Tank Site','military','anti-tank_site'],
     ['MIL_TYPE','Armistice Signing Hall','military','armistice_signing_hall'],
     ['MIL_TYPE','Armistice Talks Hall','military','armistice_talks_hall'],
     ['MIL_TYPE','Arsenal','military','arsenal'],
     ['MIL_TYPE','Artillery','military','artillery'],
     ['MIL_TYPE','Ballistic','military','ballistic'],
     ['MIL_TYPE','Barracks','military','barracks'],
     ['MIL_TYPE','Base','military','base'],
     ['MIL_TYPE','Border Control','military','border_control'],
     ['MIL_TYPE','Building','military','building'],
     ['MIL_TYPE','Bunker','military','bunker'],
     ['MIL_TYPE','Camp','military','camp'],
     ['MIL_TYPE','Checkpoint','military','checkpoint'],
     ['MIL_TYPE','Coastal Battery Positions','military','coastal_battery_positions'],
     ['MIL_TYPE','Command Facility','military','command_facility'],
     ['MIL_TYPE','Communications and Electronics Site','military','communications_and_electronics_site'],
     ['MIL_TYPE','Compound','military','compound'],
     ['MIL_TYPE','Defensive River Obstacles','military','defensive_river_obstacles'],
     ['MIL_TYPE','Detention Center','military','detention_center'],
     ['MIL_TYPE','Dock Facility','military','dock_facility'],
     ['MIL_TYPE','Entrance','military','entrance'],
     ['MIL_TYPE','Facility','military','facility'],
     ['MIL_TYPE','Factory','military','factory'],
     ['MIL_TYPE','Firebase','military','firebase'],
     ['MIL_TYPE','Firing Position','military','firing_position'],
     ['MIL_TYPE','Fortified Position','military','fortified_position'],
     ['MIL_TYPE','Fortress','military','fortress'],
     ['MIL_TYPE','Garrison','military','garrison'],
     ['MIL_TYPE','Gate','military','gate'],
     ['MIL_TYPE','Gatehouse','military','gatehouse'],
     ['MIL_TYPE','Gateway','military','gateway'],
     ['MIL_TYPE','Guard Post','military','guard_post'],
     ['MIL_TYPE','Hangar','military','hangar'],
     ['MIL_TYPE','Headquarters','military','headquarters'],
     ['MIL_TYPE','Helipad','military','helipad'],
     ['MIL_TYPE','Installation','military','installation'],
     ['MIL_TYPE','Joint Security Area','military','joint_security_area'],
     ['MIL_TYPE','Launch Site','military','launch_site'],
     ['MIL_TYPE','Main Building Guards','military','main_building_guards'],
     ['MIL_TYPE','Mortar Pits','military','mortar_pits'],
     ['MIL_TYPE','Motor Pool','military','motor_pool'],
     ['MIL_TYPE','Mound','military','mound'],
     ['MIL_TYPE','Nuclear Site','military','nuclear_site'],
     ['MIL_TYPE','Observation Point','military','observation_point'],
     ['MIL_TYPE','Obstructions','military','obstructions'],
     ['MIL_TYPE','Open Shelter','military','open_shelter'],
     ['MIL_TYPE','Outpost','military','outpost'],
     ['MIL_TYPE','PTG','military','ptg'],
     ['MIL_TYPE','Parade Reviewing Stand','military','parade_reviewing_stand'],
     ['MIL_TYPE','Peace House','military','peace_house'],
     ['MIL_TYPE','Post','military','post'],
     ['MIL_TYPE','Psychological Warfare Liaison Office','military','psychological_warfare_liaison_office'],
     ['MIL_TYPE','Rail Cargo Facility','military','rail_cargo_facility'],
     ['MIL_TYPE','Reconnaissance Bureau','military','reconnaissance_bureau'],
     ['MIL_TYPE','Recreation Facility','military','recreation_facility'],
     ['MIL_TYPE','Research And Housing','military','research_and_housing'],
     ['MIL_TYPE','Revetment','military','revetment'],
     ['MIL_TYPE','Revolutionary Site','military','revolutionary_site'],
     ['MIL_TYPE','Security','military','security'],
     ['MIL_TYPE','Shed','military','shed'],
     ['MIL_TYPE','Shelters','military','shelters'],
     ['MIL_TYPE','Site','military','site'],
     ['MIL_TYPE','Storage Facility','military','storage_facility'],
     ['MIL_TYPE','Submarine Pen','military','submarine_pen'],
     ['MIL_TYPE','Submerged Facility','military','submerged_facility'],
     ['MIL_TYPE','Support Facilities','military','support_facilities'],
     ['MIL_TYPE','Test Site','military','test_site'],
     ['MIL_TYPE','Training Site','military','training_site'],
     ['MIL_TYPE','Trench','military','trench'],
     ['MIL_TYPE','Tunnel','military','tunnel'],
     ['MIL_TYPE','Underground Facility','military','underground_facility'],
     ['MIL_TYPE','Unit','military','unit'],
     ['MIL_TYPE','Vehicle Positions','military','vehicle_positions'],
     ['MIL_TYPE','Vehicles','military','vehicles'],
     ['MIL_TYPE','Visitor Center and Gift Shop','military','visitor_center_and_gift_shop'],

     // MISCLU1 - Soils
     ['MISCLU1','Dunes/Shifting Sands','soil:misclu1','dune_shifting_sands'],
     ['MISCLU1','None','soil:misclu1','no_information'],
     ['MISCLU1','Rock Debris/Desert Detritus','soil:misclu1','rock_debris_or_desert_detritus'],
     ['MISCLU1','Salt Flats','soil:misclu1','salt_flats'],

     // OP_STATUS - Operational Status - Moved to Input only rule and Custom export rule
//      ['OP_STATUS','Operational','operational_status','operational'],
//      ['OP_STATUS','Under Construction','condition','construction'],

     // OP_CON - Operational Control?
     ['OP_CON','Commercial','controlling_authority','public'],
     ['OP_CON','Government','controlling_authority','national'],

     // OPER_TYPE - Natural Resources
     ['OPER_TYPE','Brine Operation','industrial','brine_operation'],
     ['OPER_TYPE','Geothermal','resource:type','geothermal'],
     ['OPER_TYPE','Leach','resource:type','leach'], // ?????
     ['OPER_TYPE','Offshore','resource:location','Offshore'],
     ['OPER_TYPE','Placer','mine:type','placer'],
     ['OPER_TYPE','Processing Plant','industrial','processing_plant'],
     ['OPER_TYPE','Surface','resource:location','surface'],
     ['OPER_TYPE','Surface - Underground','resource:location','surface - underground'],
     ['OPER_TYPE','Underground','resource:location','underground'],
     ['OPER_TYPE','Well','man_made','water_well'],
     ['OPER_TYPE','Quarry','landuse','quarry'],
     ['OPER_TYPE','Mine','landuse','mineral_mining'],
     ['OPER_TYPE','Plantation','landuse','plantation'],
     ['OPER_TYPE','Refinery','industrial','refinery'],
     ['OPER_TYPE','Smelter','man_made','smelter'],

     // OWNERSHIP - Free Trade Zones
     ['OWNERSHIP','Federal Government','free_trade_zone:ownership','federal_government'],
     ['OWNERSHIP','State Government','free_trade_zone:ownership','state_government'],
     ['OWNERSHIP','Under Parallel Authority','free_trade_zone:ownership','under_parallel_authority'],
     ['OWNERSHIP','Private','free_trade_zone:ownership','private'],
     ['OWNERSHIP','Public','free_trade_zone:ownership','public'],

     // OWNERTYPE - Media Outlets
     ['OWNERTYPE','Government','ownership','government'],
     ['OWNERTYPE','Private','ownership','private'],

     // PERMAFROST - Soils
     ['PERMAFROST','Discontinuous','permafrost','discontinuous'],
     ['PERMAFROST','None','permafrost','none'],
     ['PERMAFROST','Permafrost','permafrost','yes'],

     // PHASE1 - Soils
     ['PHASE1','Cerrado','soil:phase1','Cerrado'],
     ['PHASE1','Duripan','soil:phase1','Duripan'],
     ['PHASE1','Fragipan','soil:phase1','Fragipan'],
     ['PHASE1','Lithic','soil:phase1','Lithic'],
     ['PHASE1','None','soil:phase1','None'],
     ['PHASE1','Petric','soil:phase1','Petric'],
     ['PHASE1','Petrocalcic','soil:phase1','Petrocalcic'],
     ['PHASE1','Petroferric','soil:phase1','Petroferric'],
     ['PHASE1','Petrogypsic','soil:phase1','Petrogypsic'],
     ['PHASE1','Phreatic','soil:phase1','Phreatic'],
     ['PHASE1','Saline','soil:phase1','Saline'],
     ['PHASE1','Sodic','soil:phase1','Sodic'],
     ['PHASE1','Stony','soil:phase1','Stony'],

     // PHASE2 - Soils
     ['PHASE2','Cerrado','soil:phase2','Cerrado'],
     ['PHASE2','Duripan','soil:phase2','Duripan'],
     ['PHASE2','Fragipan','soil:phase2','Fragipan'],
     ['PHASE2','Lithic','soil:phase2','Lithic'],
     ['PHASE2','None','soil:phase2','None'],
     ['PHASE2','Petric','soil:phase2','Petric'],
     ['PHASE2','Petrocalcic','soil:phase2','Petrocalcic'],
     ['PHASE2','Petroferric','soil:phase2','Petroferric'],
     ['PHASE2','Petrogypsic','soil:phase2','Petrogypsic'],
     ['PHASE2','Phreatic','soil:phase2','Phreatic'],
     ['PHASE2','Saline','soil:phase2','Saline'],
     ['PHASE2','Sodic','soil:phase2','Sodic'],
     ['PHASE2','Stony','soil:phase2','Stony'],

     // PRI_COM - Primary Commodities
     ['PRI_COM','Aluminum','product','aluminum'],
     ['PRI_COM','Antimony','product','antimony'],
     ['PRI_COM','Arsenic','product','arsenic'],
     ['PRI_COM','Barite','product','barite'],
     ['PRI_COM','Barium-Barite','product','barium-barite'],
     ['PRI_COM','Bauxite','product','bauxite'],
     ['PRI_COM','Bentonite','product','bentonite'],
     ['PRI_COM','Boron','product','boron'],
     ['PRI_COM','Calcium','product','calcium'],
     ['PRI_COM','Chromium','product','chromium'],
     ['PRI_COM','Clay','product','clay'],
     ['PRI_COM','Coal','product','coal'],
     ['PRI_COM','Cobalt','product','cobalt'],
     ['PRI_COM','Copper','product','copper'],
     ['PRI_COM','Crude Oil','product','petrochemical'],
     ['PRI_COM','Diatomite','product','diatomite'],
     ['PRI_COM','Fluorspar','product','fluorspar'],
     ['PRI_COM','Gemstone','product','gemstone'],
     ['PRI_COM','Gold','product','gold'],
     ['PRI_COM','Granite','product','granite'],
     ['PRI_COM','Gypsum','product','gypsum'],
     ['PRI_COM','Gypsum-Anhydrite','product','gypsum-anhydrite'],
     ['PRI_COM','Indium','product','indium'],
     ['PRI_COM','Iron','product','iron'],
     ['PRI_COM','Lead','product','lead'],
     ['PRI_COM','Limestone','product','limestone'],
     ['PRI_COM','Magnesite','product','magnesite'],
     ['PRI_COM','Manganese','product','manganese'],
     ['PRI_COM','Marble','product','marble'],
     ['PRI_COM','Mercury','product','mercury'],
     ['PRI_COM','Molybdenum','product','molybdenum'],
     ['PRI_COM','Nickel','product','nickel'],
     ['PRI_COM','Onyx','product','onyx'],
     ['PRI_COM','Palm Oil','product','palm_oil'],
     ['PRI_COM','Perlite','product','perlite'],
     ['PRI_COM','Phosphorous','product','phosphorous'],
     ['PRI_COM','Platinum','product','platinum'],
     ['PRI_COM','Potash','product','potash'],
     ['PRI_COM','Potassium','product','potassium'],
     ['PRI_COM','Pozzolana','product','pozzolana'],
     ['PRI_COM','REE','product','ree'], // ????
     ['PRI_COM','Salt','product','salt'],
     ['PRI_COM','Silicate','product','silicate'],
     ['PRI_COM','Silicon','product','silicon'],
     ['PRI_COM','Silver','product','silver'],
     ['PRI_COM','Strontium','product','strontium'],
     ['PRI_COM','Sulfur','product','sulfur'],
     ['PRI_COM','Titanium','product','titanium'],
     ['PRI_COM','Tungsten','product','tungsten'],
     ['PRI_COM','Uranium','product','uranium'],
     ['PRI_COM','Water','product','water'],
     ['PRI_COM','Zinc','product','zinc'],

     // RUNWAY - Runway Type
     ['RUNWAY','Paved','aeroway:pavement','paved'],
     ['RUNWAY','Unpaved','aeroway:pavement','unpaved'],

     // R1_SURFACE - Runway 1 Surface
     ['R1_SURFACE','Asphalt','surface','asphalt'],
     ['R1_SURFACE','Dirt','surface','earth'],
     ['R1_SURFACE','Grass','surface','grass'],
     ['R1_SURFACE','Concrete','surface','concrete'],

     // R2_SURFACE - Runway 2 Surface
     ['R2_SURFACE','Asphalt','surface:2','asphalt'],
     ['R2_SURFACE','Dirt','surface:2','earth'],
     ['R2_SURFACE','Grass','surface:2','grass'],
     ['R2_SURFACE','Concrete','surface:2','concrete'],

     // SPA_ACC - Spatial Accuracy
     ['SPA_ACC','1','hgis:accuracy','high'],
     ['SPA_ACC','2','hgis:accuracy','medium'],
     ['SPA_ACC','3','hgis:accuracy','low'],
     ['SPA_ACC','4','hgis:accuracy','regional'],
     ['SPA_ACC','5','hgis:accuracy','country'],

     // Status - Protected area status
     ['STATUS','Proposed','site_status','proposed'],
     ['STATUS','Designated','site_status','designated'],
     ['STATUS','Not Reported','site_status','not_reported'],

     // STATUS - Free Trade Zones
     ['STATUS','Operational','operational_status','operational'],
     ['STATUS','Under Construction','condition','construction'],
     ['STATUS','Declaration','operational_status','planned'],

     // TYPE_AREA - Type of protected area
     ['TYPE_AREA','Terrestrial','protected_area','terrestrial'],
     ['TYPE_AREA','Marine','protected_area','marine'],
     ['TYPE_AREA','Terrestrial/Marine','protected_area','terrestrial_or_marine'],
     ['TYPE_AREA','Not Reported','protected_area','not_reported'],

     // TYPE, TYPE1 and TYPE2 are shared by a lot of features
     // This is ugly
     ['agriculture$TYPE1','Aquaculture','landuse','aquaculture'],
     ['agriculture$TYPE1','Farm','place','farm'],
     ['agriculture$TYPE1','Livestock','landuse','livestock'],
     ['agriculture$TYPE1','Nursery','landuse','plant_nursery'],
     ['agriculture$TYPE1','Orchard','landuse','orchard'],
     ['agriculture$TYPE1','Vineyard','landuse','vineyard'],

     ['agriculture$TYPE2','Breeding Ground','livestock','breeding ground'],
     ['agriculture$TYPE2','Cooperative','agriculture:type','cooperative'],
     ['agriculture$TYPE2','Dairy','crop','dairy'],
     ['agriculture$TYPE2','Duck','livestock','duck'],
     ['agriculture$TYPE2','Fisheries','livestock','fish'],
     ['agriculture$TYPE2','Goat','livestock','goat'],
     ['agriculture$TYPE2','Greenhouse','building','greenhouse'], // Not great, conflict with landuse=greenhouse_horticulture
     ['agriculture$TYPE2','Ostrich','livestock','ostrich'],
     ['agriculture$TYPE2','Other','livestock','other'],
     ['agriculture$TYPE2','Pig','livestock','pig'],
     ['agriculture$TYPE2','Pigeon','livestock','pigeon'],
     ['agriculture$TYPE2','Poultry','livestock','poultry'],
     ['agriculture$TYPE2','Rabbit','livestock','rabbit'],
     ['agriculture$TYPE2','Rice Field','crop','rice'],
     ['agriculture$TYPE2','Salt','crop','salt'], // Need To Fix

     ['airfieldp$TYPE','Hangars','building','hangar'],
     ['airfieldp$TYPE','Aprons','aeroway','apron'],
     ['airfieldp$TYPE','Jetway','aeroway','aerobridge'],
     ['airfieldp$TYPE','Radar','man_made','radar_station'],
     ['airfieldp$TYPE','Tower','control_tower','yes'],

     ['airfield$TYPE','Commercial','controlling_authority','civilian'],
     ['airfield$TYPE','Military','controlling_authority','military'],
     ['airfield$TYPE','Dual','controlling_authority','joint_military_civilian'],

     ['border$TYPE1','Official','border_type','official'],
     ['border$TYPE1','Unofficial','border_type','unofficial'],

     ['border$TYPE2','Border Structure','border:type','border_structure'],
     ['border$TYPE2','Established Path','border:type','established_path'],
     ['border$TYPE2','Multiple Tire Tracks','border:type','multiple_tire_tracks'],
     ['border$TYPE2','Trail','border:type','trail'],
     ['border$TYPE2','Village','border:type','village'],

     ['bridge$TYPE','Bridge','bridge','yes'],
     ['bridge$TYPE','Tunnel','tunnel','yes'],

     ['bridge$TYPE2','Pedestrian','transport:type','pedestrian'],
     ['bridge$TYPE2','Rail','transport:type','railway'],
     ['bridge$TYPE2','Road','transport:type','road'],
     ['bridge$TYPE2','Water','bridge:type','water'], /// What is this???

     ['commercial$TYPE1','Automotive','commercial:category','automotive'],
     ['commercial$TYPE1','Financial','commercial:category','financial'],
     ['commercial$TYPE1','Industrial','commercial:category','industrial'],
     ['commercial$TYPE1','Manufacturing','commercial:category','manufacturing'],
     ['commercial$TYPE1','Other','commercial:category','other'],
     ['commercial$TYPE1','Retail','commercial:category','retail'],
     ['commercial$TYPE1','Service','commercial:category','service'],
     ['commercial$TYPE1','Telecommunications','commercial:category','telecommunications'],

     ['commercial$TYPE2','Airline','shop','travel_agency'],
     ['commercial$TYPE2','ATM','amenity','atm'],
     ['commercial$TYPE2','Auto Repair','shop','car_repair'],
     ['commercial$TYPE2','Bakery','shop','bakery'],
     ['commercial$TYPE2','Bank','amenity','bank'],
     //['commercial$TYPE2','Bar','amenity','bar'],
     //['commercial$TYPE2','Beauty Salon','shop','beauty'],
     ['commercial$TYPE2','Brewery','building','brewery'],
     ['commercial$TYPE2','Butcher','shop','butcher'],
     ['commercial$TYPE2','Car Dealership','shop','car'],
     ['commercial$TYPE2','Car Rental','amenity','car_rental'],
     ['commercial$TYPE2','Car Wash','amenity','car_wash'],
     ['commercial$TYPE2','Computer','shop','computer'],
     ['commercial$TYPE2','Construction','amenity','construction'],
     ['commercial$TYPE2','Day Care','amenity','child_care'],
     //['commercial$TYPE2','Delicatessen','shop','deli'],
     ['commercial$TYPE2','Electronics','shop','electronics'],
     ['commercial$TYPE2','Entertainment','amenity','entertainment'],
     ['commercial$TYPE2','Factory','building','industrial'],
     ['commercial$TYPE2','Film','amenity','film'],
     ['commercial$TYPE2','Freight - Shipping','amenity','freight_shipping'],
     ['commercial$TYPE2','Funeral Home','shop','funeral_directors'],
     ['commercial$TYPE2','Gas','amenity','fuel'],
     //['commercial$TYPE2','Grocery','shop','grocery'],
     ['commercial$TYPE2','Heavy Machinery','amenity','heavy_machinery'], // ???
     ['commercial$TYPE2','Legal','office','lawyer'],
     //['commercial$TYPE2','Market','amenity','marketplace'],
     ['commercial$TYPE2','Medical','amenity','medical'], // Not good
     ['commercial$TYPE2','Mining','amenity','mining'], // ???
     ['commercial$TYPE2','Nursery','amenity','nursery'],
     ['commercial$TYPE2','Office','building','office'],
     ['commercial$TYPE2','Other','commercial:type','other'],
     ['commercial$TYPE2','Research','building','research'],
     //['commercial$TYPE2','Restaurant','amenity','restaurant'],
     ['commercial$TYPE2','Shipping','amenity','shipping'],
     //['commercial$TYPE2','Store','shop','general'],
     ['commercial$TYPE2','Telecommunications','shop','mobile_phone'],
     ['commercial$TYPE2','Television','shop','television'], // Not great
     ['commercial$TYPE2','Tourist','tourism','information'],
     //['commercial$TYPE2','Tower','man_made','tower'],
     ['commercial$TYPE2','Veterinarian','amenity','veterinary'],

     ['crime$TYPE','Armed Robbery','crime:event','armed_robbery'],
     ['crime$TYPE','Assault','crime:event','assault'],
     ['crime$TYPE','Corruption','crime:event','corruption'],
     ['crime$TYPE','Drug Smuggling','crime:event','drug_smuggling'],
     ['crime$TYPE','Extortion','crime:event','extortion'],
     ['crime$TYPE','Fuel Smuggling','crime:event','fuel_smuggling'],
     ['crime$TYPE','Hijack','crime:event','hijack'],
     ['crime$TYPE','IED','crime:event','ied'],
     ['crime$TYPE','Kidnapping','crime:event','kidnapping'],
     ['crime$TYPE','Murder','crime:event','murder'],
     ['crime$TYPE','Piracy','crime:event','piracy'],
     ['crime$TYPE','Robbery','crime:event','robbery'],
     ['crime$TYPE','Theft','crime:event','theft'],
     ['crime$TYPE','Violence','crime:event','violence'],
     ['crime$TYPE','Weapons Smuggling','crime:event','weapons_smuggling'],

     //['cultural$TYPE','Burial Site','amenity','grave_yard'],  // Not great
     ['cultural$TYPE','Cultural Center','amenity','cultural_center'],
     //['cultural$TYPE','Memorial','historic','memorial'],
     //['cultural$TYPE','Monument','historic','monument'],
     ['cultural$TYPE','Museum','tourism','museum'],
     ['cultural$TYPE','Other','cultural','other'],
     ['cultural$TYPE','Sculpture','artwork:type','sculpture'],
     ['cultural$TYPE','Statue','artwork:type','statue'],
     //['cultural$TYPE','Theatre','amenity','theatre'],

     ['dams$TYPE','Electricity','dam:use','electricity'],
     ['dams$TYPE','Electricity, Water Supply','dam:use','electricity_water_supply'],
     ['dams$TYPE','Irrigation','dam:use','irrigation'],
     ['dams$TYPE','Irrigation, Electricity, Water Supply','dam:use','irrigation_electricity_water_supply'],
     ['dams$TYPE','Irrigation, Water Supply','dam:use','irrigation_water_supply'],
     ['dams$TYPE','Water Supply','dam:use','water_supply'],

     ['education$TYPE1','Primary','isced:level','1'],
     ['education$TYPE1','Secondary','isced:level','2'],
     ['education$TYPE1','Tertiary','amenity','university'],
     ['education$TYPE1','Vocational','amenity','college'],

     ['education$TYPE2','Public','school:type','public'],
     ['education$TYPE2','Private','school:type','private'],
     ['education$TYPE2','Military','school:type','military'],

     ['electrical$TYPE','Generator','power','generator'],
     ['electrical$TYPE','Solar Panel','generator:method','photovoltaic'],
     ['electrical$TYPE','Substation','power','substation'],
     ['electrical$TYPE','Transmission Pole','power','pole'],
     ['electrical$TYPE','Transmission Tower','power','tower'],
     ['electrical$TYPE','Transformer','power','transformer'],

     ['embassy$TYPE','Consulate','diplomatic','consulate'],
     ['embassy$TYPE','Embassy','diplomatic','embassy'],
     ['embassy$TYPE','Foreign Mission','diplomatic','mission'],

     ['government$TYPE1','Civic','government:type','civic'],
     ['government$TYPE1','Foreign','government:type','foreign'],
     ['government$TYPE1','Other','government:type','other'],
     ['government$TYPE1','Political','government:type','political'],

     ['government$TYPE2','Local Government Facility','government:facility','local_government_facility'],
     ['government$TYPE2','National Facility','government:facility','national_facility'],
     ['government$TYPE2','Other','government:facility','other'],
     ['government$TYPE2','Political Party Facility','government:facility','political_party_facility'],
     ['government$TYPE2','Post Office','amenity','post_office'],
     ['government$TYPE2','Social','government:facility','social'],
     ['government$TYPE2','State Facility','government:facility','state_facility'],

     ['hadr$TYPE1','Drought','hgis:hdar','drought'],
     ['hadr$TYPE1','Earthquake','hgis:hdar','earthquake'],
     ['hadr$TYPE1','Epidemic','hgis:hdar','epidemic'],
     ['hadr$TYPE1','Flood','hgis:hdar','flood'],
     ['hadr$TYPE1','Industrial Accident','hgis:hdar','industrial_accident'],
     ['hadr$TYPE1','Insect Infestation','hgis:hdar','insect_infestation'],
     ['hadr$TYPE1','Misc. Accident','hgis:hdar','misc_accident'],
     ['hadr$TYPE1','Storm','hgis:hdar','storm'],
     ['hadr$TYPE1','Transport Accident','hgis:hdar','transport_accident'],
     ['hadr$TYPE1','Wildfire','hgis:hdar','wildfire'],

     ['hadr$TYPE2','Air','hadr:type','air'],
     ['hadr$TYPE2','Bacterial Infectious Disease','hadr:type','bacterial_infectious disease'],
     ['hadr$TYPE2','Collapse','hadr:type','collapse'],
     ['hadr$TYPE2','Cyclone','hadr:type','cyclone'],
     ['hadr$TYPE2','Drought','hadr:type','drought'],
     ['hadr$TYPE2','Earthquake','hadr:type','earthquake'],
     ['hadr$TYPE2','Explosion','hadr:type','explosion'],
     ['hadr$TYPE2','Fire','hadr:type','fire'],
     ['hadr$TYPE2','Flood','hadr:type','flood'],
     ['hadr$TYPE2','Flash Flood','hadr:type','flash_flood'],
     ['hadr$TYPE2','Forest Fire','hadr:type','forest_fire'],
     ['hadr$TYPE2','General Flood','hadr:type','general_flood'],
     ['hadr$TYPE2','Landslide','hadr:type','landslide'],
     ['hadr$TYPE2','Local Storm','hadr:type','local_storm'],
     ['hadr$TYPE2','Road','hadr:type','road'],
     ['hadr$TYPE2','Tropical Storm','hadr:type','tropical_storm'],
     ['hadr$TYPE2','Viral Infectious Disease','hadr:type','viral_infectious_disease'],
     ['hadr$TYPE2','Water','hadr:type','water'],

     ['health$TYPE','Bacterial Infectious Diseases','health_event','bacterial_infectious_diseases'],
     ['health$TYPE','Parasite Borne','health_event','parasite_borne'],
     ['health$TYPE','Viral Infectious Diseases','health_event','viral_infectious_diseases'],

     ['hydrop$TYPE','Fire Hydrant','emergency','fire_hydrant'],
     ['hydrop$TYPE','Water Supply','amenity','water_point'],
     ['hydrop$TYPE','Water Tank','man_made','water_tank'],
     ['hydrop$TYPE','Water Tower','man_made','water_tower'],
     ['hydrop$TYPE','Water Treatment Plant','man_made','water_works'],
//      ['hydrop$TYPE','Well','man_made','water_well'],

     ['hydroa$TYPE','Lake','water','lake'],
     //['hydroa$TYPE','River','waterway','river'],
     ['hydroa$TYPE','Inland Water','waterway','inland_water'], // ?????
     ['hydroa$TYPE','Land Subject to Inundation','water','intermittent'],
     ['hydroa$TYPE','Reservoir','water','reservoir'],
     ['hydroa$TYPE','Wetlands','natural','wetland'],
     ['hydroa$TYPE','Irrigated Area','irrigation','yes'],
     ['hydroa$TYPE','Lagoon','water','lagoon'],

     // Moved to input only. Custom rule for export
//      ['hydrol$TYPE','Lake','raw:hydrolTYPE','lake'],
//      ['hydrol$TYPE','River/Stream','raw:hydrolTYPE','river/stream'],
//      ['hydrol$TYPE','Inland Water','raw:hydrolTYPE','inland water'],
//      ['hydrol$TYPE','Land Subject to Inundation','raw:hydrolTYPE','land subject to inundation'],

     ['lodging$TYPE1','Permanent','transient','no'],
     ['lodging$TYPE1','Transient','transient','yes'],

     ['lodging$TYPE2','Apartment','tourism','apartment'],
     ['lodging$TYPE2','Bed & Breakfast','tourism','bed_and_breakfast'],
     ['lodging$TYPE2','Camp Site','tourism','camp_site'],
     ['lodging$TYPE2','Condominium','building','condominium'],
     ['lodging$TYPE2','Dormitory','building','dormitory'],
     ['lodging$TYPE2','Elite Area','landuse','elite_area'],
     //['lodging$TYPE2','Guest House','tourism','guest_house'],
     ['lodging$TYPE2','Hostel','tourism','hostel'],
     ['lodging$TYPE2','Hotel','tourism','hotel'],
     ['lodging$TYPE2','Housing','building','residential'],
     ['lodging$TYPE2','Inn','tourism','inn'],
     ['lodging$TYPE2','Lodge','tourism','lodge'],
     ['lodging$TYPE2','Motel','tourism','motel'],
     ['lodging$TYPE2','Resort','tourism','resort'],
     ['lodging$TYPE2','Villas','building','villas'],

     ['marine$TYPE','Beach','natural','beach'],
     ['marine$TYPE','Docking','waterway','dock'],
     ['marine$TYPE','Harbor','landuse','harbor'],
     ['marine$TYPE','Jetty','man_made','jetty'],
     ['marine$TYPE','Levee','man_made','levee'],
     ['marine$TYPE','Lighthouses & Beacons','man_made','lighthouse'],
     ['marine$TYPE','Lock','lock','yes'],
     ['marine$TYPE','Marina','leisure','marina'],
     ['marine$TYPE','Pier','man_made','pier'],
     ['marine$TYPE','Ship Yard','landuse','ship_yard'],

     ['media$TYPE','Magazine','communication:type','magazine'],
     ['media$TYPE','Newspaper','communication:type','newspaper'],
     ['media$TYPE','Radio','communication:type','radio'],
     ['media$TYPE','Television','communication:type','television'],
     ['media$TYPE','Yearbook','communication:type','yearbook'],

     ['medical$TYPE1','Clinic','amenity','clinic'],
     ['medical$TYPE1','Dispensary','amenity','dispensary'],
     //['medical$TYPE1','Hospital','amenity','hospital'],
     ['medical$TYPE1','Medical Facility','amenity','medical_facility'],
     //['medical$TYPE1','Pharmacy','amenity','pharmacy'],
     ['medical$TYPE1','Rehabilitation Center','amenity','rehabilitation_center'],
     ['medical$TYPE1','Sanatorium','amenity','sanatorium'],

     ['medical$TYPE2','Bath House','amenity','public_bath'],
     ['medical$TYPE2','Children','medical:type','children'],
     ['medical$TYPE2','Dental','amenity','dentist'],
     ['medical$TYPE2','Emergency','emergency','yes'],
     ['medical$TYPE2','Headquarters','headquarters','yes'],
     ['medical$TYPE2','Health Center','amenity','health_centre'],
     ['medical$TYPE2','Health Complex','amenity','health_complex'],
     ['medical$TYPE2','Hot Spring','natural','hot_spring'],
     ['medical$TYPE2','Other','medical:type','other'],
     ['medical$TYPE2','Pharmaceutical','amenity','pharmaceutical'], // Fix this
     ['medical$TYPE2','Rehabilitation','amenity','rehabilitation'],
     ['medical$TYPE2','Spa','leisure','spa'],

     ['military$TYPE','Air Force','military:service','air_force'],
     ['military$TYPE','Army','military:service','army'],
     ['military$TYPE','Coast Guard','military:service','coast_guard'],
     ['military$TYPE','Marines','military:service','marines'],
     ['military$TYPE','Navy','military:service','navy'],
     ['military$TYPE','Security Force','military:service','security_force'],
     ['military$TYPE','Special Operations Force','military:service','special_operations_force'],
     ['military$TYPE','United Nations','military:service','united_nations'],

     ['natural$TYPE','Arch','natural','arch'],
     ['natural$TYPE','Bay','natural','bay'],
//      ['natural$TYPE','Beach','natural','beach'],
     ['natural$TYPE','Cape','natural','cape'],
     ['natural$TYPE','Cave','natural','cave_entrance'],
     ['natural$TYPE','Cliff','natural','cliff'],
     ['natural$TYPE','Cove','natural','cove'],
     ['natural$TYPE','Depression','natural','depression'],
     ['natural$TYPE','Desert','natural','desert'],
     ['natural$TYPE','Dike','natural','dyke'],
     ['natural$TYPE','Dune','natural','dune'],
     ['natural$TYPE','Escarpment','geological','escarpment'],
     ['natural$TYPE','Ford','nford','yes'], // Fixed in custom rules
     ['natural$TYPE','Forest','landuse','forest'],
     ['natural$TYPE','Gorge','natural','gorge'],
     ['natural$TYPE','Hammock','natural','hammock'],
     ['natural$TYPE','Hill','natural','hill'],
     ['natural$TYPE','Island','natural','island'],
//      ['natural$TYPE','Lagoon','nlagoon','yes'], // ARRRGGH!
     ['natural$TYPE','Landmark','landmark','yes'],
     ['natural$TYPE','Mesa','geological','mesa'],
     ['natural$TYPE','Mountain','natural','mountain'],
     ['natural$TYPE','Mud Flats','wetland','tidal_flat'],
     ['natural$TYPE','Oasis','natural','oasis'],
     ['natural$TYPE','Other','waterway','other'],
     ['natural$TYPE','Pass','mountain_pass','yes'],
     ['natural$TYPE','Peak','natural','peak'],
     ['natural$TYPE','Plain','natural','plain'],
     ['natural$TYPE','Plateau','natural','plateau'],
     ['natural$TYPE','Point','natural','point'],
     ['natural$TYPE','Ponds','water','pond'],
     ['natural$TYPE','Preserve/Refuge','landuse','preserve'],
     ['natural$TYPE','Reef','natural','reef'],
     ['natural$TYPE','Ridge','natural','ridge'],
     ['natural$TYPE','Rock','natural','rock'],
     ['natural$TYPE','Salt Area','natural','salt_area'],
     ['natural$TYPE','Sinkhole','natural','sinkhole'],
     ['natural$TYPE','Springs','natural','spring'],
     ['natural$TYPE','Spur','natural','spur'],
     ['natural$TYPE','Valley','natural','valley'],
     ['natural$TYPE','Volcano','natural','volcano'],
     ['natural$TYPE','Wadi','waterway','wadi'],
     ['natural$TYPE','Waterfall','waterway','waterfall'],
//      ['natural$TYPE','Wetland','nwetland','yes'], // ARRRGH

     ['places$TYPE','Neighborhood','place','neighbourhood'],

     ['publicp$TYPE','Toilets','amenity','toilets'],
     ['publicp$TYPE','Shower','amenity','shower'],
     ['publicp$TYPE','Phone','amenity','telephone'],
     ['publicp$TYPE','Drinking Fountain','amenity','drinking_water'],
     ['publicp$TYPE','Bench','amenity','bench'],
     ['publicp$TYPE','Bathroom','amenity','bathroom'],

     ['pplant$TYPE','Coal','generator:source','coal'],
     ['pplant$TYPE','Combustion','generator:source','combustion'], // ????
     ['pplant$TYPE','Diesel','generator:source','diesel'],
     ['pplant$TYPE','Electric','plant:output:electricity','yes'], // ????
     ['pplant$TYPE','Gas','generator:source','gas'],
     ['pplant$TYPE','Geothermal','generator:source','geothermal'],
     ['pplant$TYPE','Hydroelectricity','generator:source','hydro'],
     ['pplant$TYPE','Nuclear','generator:source','nuclear'],
     ['pplant$TYPE','Oil','generator:source','oil'],
     ['pplant$TYPE','Wind','generator:source','wind'],

     ['public$TYPE','Administrative Facility','ofice','public_administration'],
     ['public$TYPE','Customs','amenity','customs'],
     ['public$TYPE','Check Point','amenity','check_point'],
     ['public$TYPE','Fire Station','amenity','fire_station'],
     ['public$TYPE','Police Station','amenity','police'],
     ['public$TYPE','Security Station','use','security_services'], // TDSv61

     ['prison$TYPE','General','prison:type','general'],
     ['prison$TYPE','Juvenile','amenity','juvenile_corrections'],
     ['prison$TYPE','National','prison:type','national'],
     ['prison$TYPE','Regional','prison:type','regional'],
     ['prison$TYPE','Solitary','prison:type','solitary'],

     ['railwayl$TYPE1','National','railway:level','national'],
     ['railwayl$TYPE1','Regional','railway:level','regional'],

     // Moved to input only. Output via custom rules
//      ['railwayl$TYPE2','Freight & Passenger','railway:type','freight_and_passenger'],
//      ['railwayl$TYPE2','Freight','railway:type','freight'],
//      ['railwayl$TYPE2','Passenger','railway:type','passenger'],
     ['railwayl$TYPE2','Subway','railway','subway'],
     ['railwayl$TYPE2','Tram','railway','tram'],

     ['railwayp$TYPE1','Elite','railway:type','elite'],
//      ['railwayp$TYPE1','Freight & Passenger','railway:type','freight_and_passenger'],
//      ['railwayp$TYPE1','Freight','railway:type','freight'],
//      ['railwayp$TYPE1','Passenger','railway:type','passenger'],
     ['railwayp$TYPE1','Tram Subway','railway','tram_subway'],

     ['railwayp$TYPE2','Depot','man_made','depot'],
     ['railwayp$TYPE2','Station','railway','station'],
     ['railwayp$TYPE2','Stop','railway','stop'],
     ['railwayp$TYPE2','Yard','service','yard'],

     ['recreation$TYPE','Other','leisure','other'],
     ['recreation$TYPE','Park','leisure','park'],
     ['recreation$TYPE','Pool','leisure','swimming_pool'],
     ['recreation$TYPE','Sports Facility','sport','multi'],

     ['refugee$TYPE','IDP Camp','idp','yes'],
     ['refugee$TYPE','Refugee Camp','refugee','yes'],
     ['refugee$TYPE','Refugee Location','refugee','location'],
     ['refugee$TYPE','Refugee Return','refugee','return'],
     ['refugee$TYPE','Refugee Settlement','amenity','refugee_housing'],
     ['refugee$TYPE','Urban Refugee Area','refugee','urban_refugee_area'],
     ['refugee$TYPE','Village of Interest','refugee','village_of_interest'],

     // NOTE: This is taken from the SUBTYPE value, not the domain
     //['religion$TYPE1','0','religion','christian'],
     ['religion$TYPE1','1','religion','muslim'],
     ['religion$TYPE1','2','religion','hindu'],
     //['religion$TYPE1','3','religion','buddhist'],
     ['religion$TYPE1','5','religion','jewish'],
     ['religion$TYPE1','6','religion','ethnic'],
     ['religion$TYPE1','7','religion','juche'],
     ['religion$TYPE1','8','religion','sikh'],
     ['religion$TYPE1','9','religion','unaffiliated'],
     ['religion$TYPE1','10','religion','999999'],
     ['religion$TYPE1','11','religion','agnostic'],
     ['religion$TYPE1','12','religion','mixed'],
     ['religion$TYPE1','13','religion','gnostic'],

     ['religion$TYPE2','Sunni','denomination','sunni'],
     ['religion$TYPE2','Shia','denomination','shia'],
     ['religion$TYPE2','Wahhabi','denomination','wahhabi'],
     ['religion$TYPE2','Mixed','denomination','mixed'],

     ['religion$TYPE2','Druze','denomination','druze'],

     ['religion$TYPE2','Mahayana','denomination','Mahayana'],
     ['religion$TYPE2','Theravada','denomination','theravada'],
     ['religion$TYPE2','Vajrayana','denomination','vajrayana'],

     ['religion$TYPE2','Protestant','denomination','protestant'],
     ['religion$TYPE2','Orthodox','denomination','orthodox'],
     ['religion$TYPE2','Catholic','denomination','catholic'],

     ['religion$TYPE2','Shamanism','denomination','shamanic'],
     ['religion$TYPE2','Animism','denomination','animist'],
     ['religion$TYPE2','Chinese Folk Religionists','denomination','chinese'],

     ['religion$TYPE2','Vaishnavism','denomination','viashnavism'],
     ['religion$TYPE2','Shaivism','denomination','shaivism'],
     ['religion$TYPE2','Shaktism','denomination','shaktism'],
     ['religion$TYPE2','Smartism','denomination','smartism'],

     ['religion$TYPE2','Conservative','denomination','conservative'],
     ['religion$TYPE2','Reform','denomination','reform'],

     ['religion$TYPE2','Udasi','denomination','udasi'],
     ['religion$TYPE2','Sahajdharis','denomination','sahajdharis'],
     ['religion$TYPE2','Keshadharis','denomination','keshadharis'],

     //['religioni$TYPE','Church','building','church'],
     //['religioni$TYPE','Mosque','building','mosque'],
     ['religioni$TYPE','Mission','building','mission'],
     ['religioni$TYPE','Youth Center','building','youth_center'],
     ['religioni$TYPE','Temple','building','temple'],
     ['religioni$TYPE','Shrine','building','shrine'],
     ['religioni$TYPE','Synagogue','building','synagogue'],
     ['religioni$TYPE','Monastery','amenity','monastery'],

     ['roadp$TYPE','Tolls','toll','yes'],
     ['roadp$TYPE','Highway Interchange','interchange','yes'], // Deconflict with AP020
     ['roadp$TYPE','exit','highway','exit'],
     ['roadp$TYPE','Toll Gate','barrier','toll_booth'],
     ['roadp$TYPE','Roundabout','junction','roundabout'],
     ['roadp$TYPE','Crosswalk','highway','crossing'],
     ['roadp$TYPE','Junction','junction','yes'],
     ['roadp$TYPE','Traffic Light','highway','traffic_signals'],

     ['road$TYPE','Bridleway','highway','bridleway'],
     ['road$TYPE','Bus Guideway','highway','bus_guideway'],
     ['road$TYPE','Construction','highway','construction'],
     ['road$TYPE','Cycleway','highway','cycleway'],
     ['road$TYPE','Footway','highway','footway'],
     ['road$TYPE','Ford','ford','yes'],
     ['road$TYPE','Living Street','highway','living_street'],
     ['road$TYPE','Motorway Link','highway','motorway_link'],
     ['road$TYPE','Motorway','highway','motorway'],
     ['road$TYPE','Path','highway','path'],
     ['road$TYPE','Pedestrian','highway','pedestrian'],
     ['road$TYPE','Primary Link','highway','primary_link'],
     ['road$TYPE','Primary','highway','primary'],
     ['road$TYPE','Raceway','highway','raceway'],
     ['road$TYPE','Residential','highway','residential'],
     ['road$TYPE','Rest Area','highway','rest_area'],
     ['road$TYPE','Secondary Link','highway','secondary_link'],
     ['road$TYPE','Secondary','highway','secondary'],
     ['road$TYPE','Service','highway','service'],
     ['road$TYPE','Steps','highway','steps'],
     ['road$TYPE','Tertiary Link','highway','tertiary_link'],
     ['road$TYPE','Tertiary','highway','tertiary'],
     ['road$TYPE','Track','highway','track'],
     ['road$TYPE','Trail','highway','trail'],
     ['road$TYPE','Trunk Link','highway','trunk_link'],
     ['road$TYPE','Trunk','highway','trunk'],
     ['road$TYPE','Unclassified','highway','unclassified'],

     ['smugglingl$TYPE','Rail','smuggling:route:type','rail'],
     ['smugglingl$TYPE','Road','smuggling:route:type','road'],
     ['smugglingl$TYPE','Shipping Lane','smuggling:route:type','shipping lane'],
     ['smugglingl$TYPE','Trail','smuggling:route:type','trail'],

     ['smugglingp$TYPE','Goods','smuggling:type','goods'],
     ['smugglingp$TYPE','Human','smuggling:type','human'],
     ['smugglingp$TYPE','Narcotics/Drugs','smuggling:type','narcotics/drugs'],
     ['smugglingp$TYPE','Oil/Fuel','smuggling:type','oil/fuel'],
     ['smugglingp$TYPE','Poaching','smuggling:type','poaching'],
     ['smugglingp$TYPE','Precious Minerals','smuggling:type','precious minerals'],
     ['smugglingp$TYPE','Smuggling Hub','smuggling:type','smuggling hub'],
     ['smugglingp$TYPE','Weapons','smuggling:type','weapons'],

     ['tourist$TYPE','Botanic Garden','garden:type','botanical'],
     //['tourist$TYPE','Casino','amenity','casino'],
     ['tourist$TYPE','Cultural','amenity','cultural'],
     ['tourist$TYPE','Gallery','tourism','gallery'],
     ['tourist$TYPE','Game Reserve','landuse','game_reserve'],
     ['tourist$TYPE','Historical','historic','yes'],
     //['tourist$TYPE','Library','amenity','library'],
//      ['tourist$TYPE','Memorial','raw:touristTYPE','memorial'],
//      ['tourist$TYPE','Monument','raw:touristTYPE','monument'],
//      ['tourist$TYPE','Museum','raw:touristTYPE','museum'],
     ['tourist$TYPE','National Park','leisure','national park'],
     ['tourist$TYPE','Other','tourism','other'],
     ['tourist$TYPE','Outdoor','tourism','outdoor'],
     ['tourist$TYPE','Outdoor/Adventure','tourism','outdoor_adventure'],
//      ['tourist$TYPE','Spa','leisure','spa'],
//      ['tourist$TYPE','Theater','raw:touristTYPE','theater'],
     ['tourist$TYPE','Theme Park','tourism','theme_park'],
     ['tourist$TYPE','Viewpoint','tourism','viewpoint'],
     ['tourist$TYPE','Water Park','leisure','water_park'],
     ['tourist$TYPE','Zoo','tourism','zoo'],

     // Public Transport is handled with custom rules
//      ['transport$TYPE1','Bus','transport:type','bus'],
//      ['transport$TYPE1','Car','transport:type','automotive'],
//      ['transport$TYPE1','Ferry','transport:type','ferry'],
//      ['transport$TYPE1','Rail','transport:type','rail'],
//      ['transport$TYPE1','Subway','transport:type','subway'],
//      ['transport$TYPE1','Taxi','transport:type','taxi'],
//      ['transport$TYPE1','Tram','transport:type','tram'],
//      ['transport$TYPE1','Trolley','transport:type','trolley'],
     ['transport$TYPE1','Bus','bus','yes'],
     ['transport$TYPE1','Car','car','yes'],
     ['transport$TYPE1','Ferry','ferry','yes'],
     ['transport$TYPE1','Rail','train','yes'],
     ['transport$TYPE1','Subway','subway','yes'],
     ['transport$TYPE1','Taxi','taxi','yes'],
     ['transport$TYPE1','Tram','tram','yes'],
     ['transport$TYPE1','Trolley','trolley','yes'],

     ['transport$TYPE2','Parking Lot','amenity','parking'],
     ['transport$TYPE2','Station','public_transport','station'],
     ['transport$TYPE2','Stop','public_transport','stop_position'],
     ['transport$TYPE2','Terminal','public_transport','terminal'],
     ['transport$TYPE2','Tunnel','transport:type2','tunnel'],
     ['transport$TYPE2','Yard','public_transport','yard'],

     // UNITS - Natural Resources
     ['UNITS','Kilograms per year','capacity:units','kilograms_per_year'],
     ['UNITS','Metric tons per year','capacity:units','metric_tons_per_year'],
     ['UNITS','Thousand metric tons per year','capacity:units','thousand_metric_tons_per_year'],

     // USE - Airport Type
     ['USE','International','aerodrome','international'],
     ['USE','Local','aerodrome','local'],
     ['USE','Regional','aerodrome','regional'],

     ], // End one2one

     // Input Translation Rules:
     // These are only used for IMPORT and get added to the standard one2one rules
     one2oneIn : [
     ['OP_STATUS','Operational','operational_status','operational'],
     ['OP_STATUS','Under Construction','condition','construction'],

     ['hydrop$TYPE','Well','man_made','water_well'],

     ['hydrol$TYPE','Inland Water','waterway','inland_water'],
     ['hydrol$TYPE','Lake','water','lake'],
     ['hydrol$TYPE','Land Subject to Inundation','water','intermittent'],
     ['hydrol$TYPE','River/Stream','waterway','river'],

     ['natural$TYPE','Beach','natural','beach'],
     ['natural$TYPE','Lagoon','water','lagoon'],
     ['natural$TYPE','Wetland','natural','wetland'],

     ['tourist$TYPE','Memorial','historic','memorial'],
     ['tourist$TYPE','Monument','historic','monument'],
     ['tourist$TYPE','Museum','tourism','museum'],
     ['tourist$TYPE','Spa','leisure','spa'],
     ['tourist$TYPE','Theater','amenity','theatre'],

     // Spelling and variations
     ['marine$TYPE','Lighthouse & Beacons','man_made','lighthouse'],
     ['lodging$TYPE2','Condominiums','building','condominium'],

     // This Is Ugly
     ['railwayp$TYPE1','Freight & Passenger','railway:type','freight_and_passenger'],
     ['railwayp$TYPE1','Freight','railway:type','freight'],
     ['railwayp$TYPE1','Passenger','railway:type','passenger'],
     ['railwayl$TYPE2','Freight & Passenger','railway:type','freight_and_passenger'],
     ['railwayl$TYPE2','Freight','railway:type','freight'],
     ['railwayl$TYPE2','Passenger','railway:type','passenger'],

    ], // End one2oneIn

    // One2one translation table for converting "Other" OSM attributes
    // This is for Export only. The values are swapped before use
    one2oneOut : [
     [undefined,undefined,'natural','water'], // to get rid of errors

     ['railwayp$TYPEX','Freight & Passenger','railway:type','freight_and_passenger'],
     ['railwayp$TYPEX','Freight','railway:type','freight'],
     ['railwayp$TYPEX','Passenger','railway:type','passenger'],

     // dual features
     ['natural$TYPE','Wetland','nwetland','yes'], // ARRRGH
     ['natural$TYPE','Lagoon','nlagoon','yes'], // ARRRGGH!

     // From OSM

    ], // End one2oneOut

    // ##### End of One2One Rules #####

    // Month list: Used to make the months pretty
    monthList : ['January','February','March','April','May','June','July','August','September','October','November','December'],

    // List of unique attributes for each layer
    uniqList : {
      'NAME_ISO':'ADMIN0','NAME_FAO':'ADMIN0','NAME_LOCAL':'ADMIN0','NAME_VARIA':'ADMIN0','VALIDFR':'ADMIN0',
      'ICAO':'Airfields','IATA':'Airfields','N_RUNWAYS':'Airfields','R_LENGTH':'Airfields','USE':'Airfields','R_WIDTH':'Airfields',
      'COUNTRY':'Border_Crossings',
      'ACTOR1':'Conflict_Points','ACTOR2':'Conflict_Points','NUM_DTH':'Conflict_Points','NUM_INJ':'Conflict_Points',
      'ACTOR1':'Conflict_Points','ORG_SOURCE':'Conflict_Points',
      'ACTOR':'Crime_Data',
      'RIVER':'Dams','YR_OPER':'Dams','HEIGHT':'Dams',
      'TOT_BOYS':'Educational_Institutions','TOT_CLASS':'Educational_Institutions','TOT_F_TEAC':'Educational_Institutions',
      'TOT_GIRLS':'Educational_Institutions','TOT_M_TEAC':'Educational_Institutions',
      // 'WEBSITE':'Embassies',
      'PEOPLEGP_1':'Ethnicity','PEOPLEGP_2':'Ethnicity','PEOPLEGP_3':'Ethnicity','PEOPLEGP_4':'Ethnicity','PEOPLEGP_5':'Ethnicity',
      'SECTOR':'Foreign_Investment_Projects','VALUE':'Foreign_Investment_Projects','LEND_INSTR':'Foreign_Investment_Projects',
      'DESCRIP':'Foreign_Investment_Projects','APP_DATE':'Foreign_Investment_Projects','APP_YEAR':'Foreign_Investment_Projects',
      'MAJ_NAME':'Hydrology_Polylines','SUB_NAME':'Hydrology_Polylines','STRAHLER':'Hydrology_Polylines',
      'LANG_FAM':'Languages','LANG_SUBGR':'Languages',
      'LANGUAGE':'Media_Outlets',
      'MIL_TYPE':'Military_Installations',
      'ALL_COM':'Natural_Resources',
      // Need to do Ports
      'NUM_GEN':'Power_Plants','TOT_CAP_MW':'Power_Plants',
      'DESIG':'Protected_Areas','DESIG_TYPE':'Protected_Areas','IUCN_CAT':'Protected_Areas','REP_M':'Protected_Areas',
      'REP_AREA':'Protected_Areas','STATUS_YR':'Protected_Areas','GOV_TYPE':'Protected_Areas','MANAG_AUTH':'Protected_Areas',
      'NO_TK_AREA':'Protected_Areas',
      'EST_PRIS':'Prisons',
      'POPULATION':'Refugee_Camps',
      'FAOSOIL':'Soils','DOMSOI':'Soils'
      },

   // Lookup table for mapping TYPE2 to TYPE1
   // The format is: TYPE2 = TYPE1
   mapType : {
       Agricultural_POI : {
           'Breeding Ground':'Farm',
           'Cooperative':'Farm',
           'Dairy':'Farm',
           'Duck':'Livestock',
           'Fisheries':'Aquaculture',
           'Goat':'Livestock',
           'Greenhouse':'Nursery',
           'Ostrich':'Livestock',
           'Other':'999999',
           'Pig':'Livestock',
           'Pigeon':'Livestock',
           'Poultry':'Farm',
           'Rabbit':'Livestock',
           'Rice Field':'Farm',
           'Salt':'999999'
       },
       Commercial_POI : {
           'Airline':'Service',
           'ATM':'Financial',
           'Auto Repair':'Automotive',
           'Bakery':'Retail',
           'Bank':'Financial',
           'Bar':'Retail',
           'Beauty Salon':'Retail',
           'Brewery':'Manufacturing',
           'Butcher':'Retail',
           'Car Dealership':'Automotive',
           'Car Rental':'Automotive',
           'Car Wash':'Automotive',
           'Computer':'Retail',
           'Construction':'Industrial',
           'Day Care':'Service',
           'Delicatessen':'Retail',
           'Electronics':'Retail',
           'Entertainment':'Service',
           'Factory':'Industrial',
           'Film':'Service',
           'Freight - Shipping':'Service',
           'Funeral Home':'Service',
           'Fuel':'Retail',
           'Gas':'Retail',
           'Grocery':'Retail',
           'Heavy Machinery':'Industrial',
           'Legal':'Service',
           'Market':'Retail',
           'Medical':'Service',
           'Mining':'Industrial',
           'Nursery':'Retail',
           'Office':'Service',
           'Other':'Other',
           'Research':'Service',
           'Restaurant':'Service',
           'Shipping':'Service',
           'Store':'Retail',
           'Telecommunications':'Telecommunications',
           'Television':'Telecommunications',
           'Tourist':'Service',
           'Tower':'Telecommunications',
           'Veterinarian':'Service'
       },
       Government_POI : {
           'National Facility':'Civic',
           'Other':'Other',
           'Political Party Facility':'Political',
           'Post Office':'Civic',
           'Social':'Civic',
           'State Facility':'Civic',
           'Local Government Facility':'Civic'
       },
       Lodging_POI : {
           'Apartment':'Permanent',
           'Bed & Breakfast':'Transient',
           'Camp Site':'Transient',
           'Condominium':'Permanent',
           'Dormitory':'Transient',
           'Elite Area':'Permanent',
           'Guest House':'Transient',
           'Hostel':'Transient',
           'Hotel':'Transient',
           'Housing':'Permanent',
           'Inn':'Transient',
           'Lodge':'Transient',
           'Motel':'Transient',
           'Resort':'Transient',
           'Villas':'Permanent'
       },
       Medical_Facilities : {
           'Bath House':'Medical Facility',
           // 'Children':'medical:type',
           'Dental':'Clinic',
           'Emergency':'Medical Facility',
           'Headquarters':'999999',
           'Health Center':'Medical Facility',
           'Health Complex':'Medical Facility',
           'Hot Spring':'Medical Facility',
           'Other':'Medical Facility',
           'Pharmaceutical':'Pharmacy',
           'Rehabilitation':'Clinic',
           'Spa':'Medical Facility'
       }
   }, // End mapType


   // ##### Start of intList #####
   // This list is for validateing the integer attributes prior to export
   intList : [ 'FATALITY', 'NO_TK_AREA', 'POPULATION', 'REP_AREA', 'REP_M', 'STATUS_YR', 'TL_AFFECT' ],
   // ##### End of intList#####

   // Lookup table for layernames.
   // We use this for assigning a short name and validation
    layerNames : {
     'ADMIN0':'admin0',
     'Admin0_Access_To_Services':'d0services',
     'Admin0_Education':'d0education',
     'Admin0_Employment':'d0employment',
     'Admin0_Household_Goods':'d0household',
     'Admin0_Mortality':'d0mortality',
     'Admin0_Population':'d0population',
     'Admin0_Poverty':'d0poverty',
     'ADMIN1':'admin1',
     'Admin1_Access_To_Services':'d1services',
     'Admin1_Education':'d1education',
     'Admin1_Employment':'d1employment',
     'Admin1_Household_Goods':'d1household',
     'Admin1_Mortality':'d1mortality',
     'Admin1_Population':'d1population',
     'Admin1_Poverty':'d1poverty',
     'ADMIN2':'admin2',
     'Admin2_Access_To_Services':'d2services',
     'Admin2_Education':'d2education',
     'Admin2_Employment':'d2employment',
     'Admin2_Household_Goods':'d2household',
     'Admin2_Mortality':'d2mortality',
     'Admin2_Population':'d2population',
     'Admin2_Poverty':'d2poverty',
     'ADMIN3':'admin3',
     'ADMIN4':'admin4',
     'ADMIN5':'admin5',
     'Agricultural_POI':'agriculture',
     'Airfield_POI':'airfieldp',
     'Airfields':'airfield',
     'Border_Crossings':'border',
     'Bridges_Tunnels':'bridge',
     'Commercial_POI':'commercial',
     'Conflict_Points':'conflict',
     'Crime_Data':'crime',
     'Cultural_POI':'cultural',
     'Dams':'dams',
     'Educational_Institutions':'education',
     'Electrical_POI':'electrical',
     'Embassies':'embassy',
     'Ethnicity':'ethnicity',
     'Foreign_Investment_Projects':'foreign',
     'Free_Trade_Zones':'freep',
     'Free_Trade_Zones_Polygons':'freepoly',
     'GNDB':'gndb',
     'Geonames':'geonames',
     'Government_POI':'government',
     'HADR':'hadr',
     'Health_Events':'health',
     'Hydrology_POI':'hydrop',
     'Hydrology_Polygons':'hydroa',
     'Hydrology_Polylines':'hydrol',
     'Internet_Cafes':'internet',
     'Languages':'languages',
     'Libraries':'libraries',
     'Lodging_POI':'lodging',
     'Marine_POI':'marine',
     'Media_Outlets':'media',
     'Medical_Facilities':'medical',
     'Military_Installations':'military',
     'Natural_POI':'natural',
     'Natural_Resources':'resources',
     'Places_POI':'places',
     'Ports':'ports',
     'Power_Plants':'pplant',
     'Prisons':'prison',
     'Protected_Areas':'protected',
     'Public_POI':'publicp',
     'Public_Security_POI':'public',
     'Public_Transportation_POI':'transport',
     'Railways':'railwayl',
     'Railways_POI':'railwayp',
     'Recreation_POI':'recreation',
     'Refugee_Camps':'refugee',
     'Religion':'religion',
     'Religious_Institutions':'religioni',
     'Road_POI':'roadp',
     'Roads':'road',
     'Smuggling_Routes':'Smuggle',
     'Smuggling_Routes_Polyline':'Smugglel',
     'Soils':'soils',
     'Tourist_Locations_POI':'tourist',
     } // End layerNames

} // End of hgis20.rules
