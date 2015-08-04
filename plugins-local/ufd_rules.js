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
    UFD One2one rules
    For importing UFD 1,2 and 3
*/


ufd.rules = {
    // ##### Start of One2One Rules #####

    // FCODE rules for Import
    fcodeOne2oneIn : [
        ['F_CODE','AA012','landuse','quarry'], // Quarry - NFDD AA010
        ['F_CODE','AA050','man_made','water_well'], // Well - NFDD BH230
        ['F_CODE','AC000','facility','yes'], // Processing/Treatment Plant - NFDD AL010
        ['F_CODE','AE010','facility','yes'], // Assembly Plant - NFDD AL010
        ['F_CODE','AH050','site:type','fortification'], // Fortification - FCODE Retired
        ['F_CODE','AK190','man_made','recreational_pier'], // Fishing Pier/Promenade Pier - NFDD BB081
        ['F_CODE','AL015',undefined,undefined], // Building - NFDD AL013 - To Stop Warnings
        ['F_CODE','AL045','facility','yes'], // Complex Outline - No FCODE, moving to AL010 Facility
        ['F_CODE','AL100','building','hut'], // Hut - NFDD AL099
        ['F_CODE','AL210','route:protection','yes'], // Snow Shed/Rock Shed - NFDD AL211
        ['F_CODE','AM031','landuse','depot'], // Timber Yard - NFDD AM010
        ['F_CODE','AL240','man_made','tower'], // Tower (Non-Communication) - NFDD AL241
        ['F_CODE','AQ010','cableway','yes'], // Aerial Cableway Lines/Ski Lift Lines - NFDD AT041
        ['F_CODE','AQ020','raw:fcode','AQ020'], // Aerial Cableway Pylon/Ski Pylon - NFDD AT042
        ['F_CODE','AQ064','man_made','causeway'], // Causeway - NFDD AQ063
        ['F_CODE','AT020','man_made','radar_station'], // Early Warning Radar Site - NFDD AT045
        ['F_CODE','AT030','power','line'], // Power Transmission Line - NFDD AT005
        ['F_CODE','AT040','pylon','yes'], // Power Transmission Pylon - NFDD AT042
        ['F_CODE','AT050','landuse','facility'], // Communication Building - No Code
        ['F_CODE','AT060','cable:type','communication'], // // Telephone Line/Telegraph Line - NFDD AT005
        ['F_CODE','AT070','raw:fcode','AT070'], // Telephone-Telegraph Pylon/Pole - NFDD AT042
        ['F_CODE','AT080','tower:type','communication'], // Communication Tower - NFDD AL241
        ['F_CODE','BB041','man_made','breakwater'], // Breakwater - NFDD BB081
        ['F_CODE','BB043','man_made','groyne'], // Groin - NFDD BB081
        ['F_CODE','BB140','man_made','pier'], // Jetty - NFDD BB081
        ['F_CODE','BB190','waterway','dock'], // Pier/Wharf/Quay - NFDD BB081
        ['F_CODE','BB220','leisure','slipway'], // Ramp (Maritime) - NFDD BB082
        ['F_CODE','BB230','wall','seawall'], // Seawall -NFDD BB081
        ['F_CODE','BB240','leisure','slipway'], // Slipway/Patent Slip -NFDD BB082
        ['F_CODE','BH050','place','maritime_farm'], // Fish Hatchery/Fish Farm/Marine Farm - NFDD BH051
        ['F_CODE','BH060','water:race','flume'], // Flume - NFDD BH065
        ['F_CODE','BH080','water','lake'], // Lake/Pond - NFDD BH082
        ['F_CODE','BH130','water','reservoir'], // Reservoir - NFDD BH082
        ['F_CODE','BH171','emergency','fire_hydrant'], // Fire Hydrant - NFDD AL017
        ['F_CODE','DB030','natural','cave_entrance'], // Cave - NFDD DB029
        ['F_CODE','DB060','natural','fissure'], // Crevice/Crevasse - NFDD DB061
        ['F_CODE','EC030','landuse','forest'], // Trees - NFDD EC015
        ['F_CODE','FA001','raw:fcode','FA001'], // Administrative Area - No code
        ['F_CODE','FA020','raw:fcode','FA020'], // Armistice Line - No code
        ['F_CODE','GB010','raw:fcode','GB010'], // Airport Lighting - FCODE Retired
        ['F_CODE','SU002','railway','subway'], // Subway - NFDD AN010
        ['F_CODE','ZB020','raw:fcode','ZB020'], // Benchmark - NFDD ZB050
        ['F_CODE','ZD019','raw:fcode','ZD019'], // General Miscellaneous Feature - No code
        ], // End fcodeOne2oneIn

    // One2one rules for Text attributes
    txtBiased : {
        'BE_NUMBER':'be_number', // Basic Encyclopedia Number
        'COMMENTS':'source:comments',
        'COMM_CPYRT_NOTICE':'source:copyright', // Commercial Copyright Notice
        'COMM_LIC_TIER_NOTICE':'source:commercial_distribution_restriction', // Distribution of Commercial Data
        'CREATION_DATE':'source:creation_date',
        'IKO':'icao', // ICAO Designator
        'NAM':'name', // Name Category
        'NFI':'gndb_id:2', // Named Feature Identifier
        'NFN':'gndb_id', // Name Identifier
        'NM3':'raw:NM3', // Name 3 (name of the political entity on one side of a boundary).
        'NM4':'raw:NM4', // Name 4 (name of the political entity on the other side of the boundary).
        'OPERATOR':'source:operator_id', // Operator ID
        'ORIG_SOURCE_DATE':'source:datetime',
        'ORIG_SOURCE_INFO':'source:description', // Originating Source Information
        'ORIGINATOR':'source:originator', // Agency that Supplied the Data
        'RSTRN_DCLS_XMPT_CD':'security:declassification:exception', // Restriction Declassification Exemptions
        'RSTRN_DECLASS':'security:declassification:date', // Restriction Declassification
        'RSTRN_DSEM_CTRL_IC':'security:dissemination_control:ic', // Restrictions on Dissemination Control IC
        'RSTRN_FOREIGN_GOV':'security:foreign_government_access', // Restrctions on Foreign Government Access
        'RSTRN_RELEASIBILITY':'security:releasability', // Restrictions on Releasibility
        'RTN':'ref', // Route Number
        'SEC_CD_CNTRL':'security:codeword_control', // Security Codeword Control
        'SEC_CLASS_SYS_ID':'security:classification:system_identifier', // Security Classification System Identifier
        'SEC_CLS_AUTH_DESC':'security:classification:non_standard_authority', // Non-standard Classification Authority
        'SEC_CLS_AUTH_MULT':'security:classification:multiple_authority', // Multiple Classification Authority Sources
        'TXT':'source:text', // Text
        'UPDATE_SOURCE_INFO':'source:update:description', // Update/Review Source Information
        'UPDATE_SOURCE_DATE':'source:update:datetime',
        'VDR':'source:datum:sounding:name', // Vertical Datum Record - ???
        'VOI':'aeroway:obstruction', // Vertical Obstruction Identifier
        }, // End txtBiased


    // One2one rules for Number attributes
    numBiased : {
        'ACE':'source:accuracy:horizontal', // Absolute Circular Error
        'ALE':'source:accuracy:linear', // Absolute Linear Error
        'AOO':'angle', // Angle of Orientation
        'ARA':'feature_area', // Area: this has been moved from 'area' due to hoot conflicts
        'DEP':'depth:max', // Depth Below Surface Level
        'DMB':'raw:DMB', // Density Measure (Brush/Undergrowth)
        'DMT':'raw:DMT', // Density Measure (% of Tree/Canopy Cover)
        'DOF':'flow_direction', // Direction of Flow
        'FCL':'ferry:crossing_distance', // Ferry Crossing Length
        'GAW':'gauge', // Gauge Width
        'HGT':'height', // Height Above Surface Level
        'KVA':'voltage', // Kilovolt Capacity Attribute
        'LC1':'bridge:load_class1', // Load Class Type 1
        'LC2':'bridge:load_class2', // Load Class Type 2
        'LC3':'bridge:load_class3', // Load Class Type 3
        'LC4':'bridge:load_class4', // Load Class Type 4
        'LEG':'bridge:length', // Length/Diameter in Meters With Greater Precision
        'LEN':'length', // Length
        'LEN_':'length2', // Length/Diameter2 ?????
        'LNU':'length:interior_useable', // Useable Length
        'LTN':'lanes', // Track or Lane Count
        'MVC':'max_clearance', // Maximum Vertical Clearance
        'MWG':'divider:width', // Median Width with greater than 1 meter resolution
        'NPL':'cables', // Number of Parallel Lines
        'OHB':'height', // Overall Height of Bridge
        'OHC':'max_clearance', // Overhead Clearance Category
        'PFG':'height:predominant', // Predominant Feature Height
        'PHT':'height', // Predominant Height
        'SCALE':'source:scale', // Feature Scale (e.g., 50000, 100000)
        'SDO':'raw:SDO', // Sand Dune Orientation
        'SGC':'raw:SGC', // Gradient/Slope
        'WD1':'width:minimum_traveled_way', // Minimum Traveled Way Width (decimeters)
        'WDU':'width:interior_useable', // Usable Width
        'WGP':'raw:WGP', // Width with greater than 1 meter resolution
        'WID':'width', // Width
        'WOC':'width:crest', // Width of Crest
        'WT2':'width:second_way', // Width of Second Traveled Way
        'ZV2':'ele', // Highest Z-Value
        'ZV3':'ele', // Airfield/Aerodrome Elevation
        }, // End numBiased

    // Common one2one rules. Used for both import and export
    one2one : [
        // ACC - Accuracy Category 
        // ['ACC','0','source:horiz_accuracy_cat','unknown'], // Unknown 
        ['ACC','0',undefined,undefined], // Unknown 
        ['ACC','1','source:accuracy:horizontal:category','accurate'], // Accurate
        ['ACC','2','source:accuracy:horizontal:category','approximate'], // Approximate
        ['ACC','3','source:accuracy:horizontal:category','doubtful'], // Doubtful
        ['ACC','997',undefined,undefined], // Unpopulated 

        // ACE_EVAL_METHOD_CD - Absolute Circular Error Evaluation Method 
        ['ACE_EVAL_METHOD_CD','ACA','source:accuracy:horizontal:evaluation','geodetic_survey_control_-_adequate_sample'], // Geodetic Survey Control - adequate sample
        ['ACE_EVAL_METHOD_CD','ACS','source:accuracy:horizontal:evaluation','geodetic_survey_control_-_small_sample'], // Geodetic Survey Control - small sample
        ['ACE_EVAL_METHOD_CD','AHA','source:accuracy:horizontal:evaluation','photogrammetric_(old_stats_unable_to_convert_to_ana_due_to_significant_bias)'], // Photogrammetric (old stats unable to convert to ANA due to significant bias)
        ['ACE_EVAL_METHOD_CD','ANA','source:accuracy:horizontal:evaluation','photogrammetric_control_(gdasii)_-_adequate_sample'], // Photogrammetric Control (GDASII) - adequate sample
        ['ACE_EVAL_METHOD_CD','ANS','source:accuracy:horizontal:evaluation','photogrammetric_control_(gdasii)_-_small_sample'], // Photogrammetric Control (GDASII) - small sample
        ['ACE_EVAL_METHOD_CD','APA','source:accuracy:horizontal:evaluation','photogrammetric_control_(iec_fpe_npf)_-_adequate_sample'], // Photogrammetric Control (IEC FPE NPF) - adequate sample
        ['ACE_EVAL_METHOD_CD','APS','source:accuracy:horizontal:evaluation','photogrammetric_control_(iec_fpe_npf)_-_small_sample'], // Photogrammetric Control (IEC FPE NPF) - small sample
        ['ACE_EVAL_METHOD_CD','AXB','source:accuracy:horizontal:evaluation','extrapolation'], // Extrapolation
        ['ACE_EVAL_METHOD_CD','AZD','source:accuracy:horizontal:evaluation','unable_to_perform_metric_evaluation_other_than_ezm_ezg_azf_and_aze_(assumed_adequate)'], // Unable to perform metric evaluation other than EZM EZG AZF and AZE (assumed adequate)
        ['ACE_EVAL_METHOD_CD','AZE','source:accuracy:horizontal:evaluation','insufficient_control_presently_available_to_perform_metric_evaluation_(assumed_adequate)'], // Insufficient control presently available to perform metric evaluation (assumed adequate)
        ['ACE_EVAL_METHOD_CD','AZF','source:accuracy:horizontal:evaluation','insufficient_identifiable_map_features_for_metric_evaluation_(assumed_adequate)'], // Insufficient identifiable map features for metric evaluation (assumed adequate)
        ['ACE_EVAL_METHOD_CD','BMD','source:accuracy:horizontal:evaluation','error_budget_evaluation_(production_data)'], // Error Budget Evaluation (Production Data)
        ['ACE_EVAL_METHOD_CD','CLD','source:accuracy:horizontal:evaluation','accuracy_evaluation_printed_on_the_sheet_or_evaluation_derived_from_source_accuracy'], // Accuracy evaluation printed on the sheet or evaluation derived from source accuracy
        ['ACE_EVAL_METHOD_CD','CXB','source:accuracy:horizontal:evaluation','results_from_a_large_sampling_of_post-production_evaluations_that_are_similar_in_compilation_or_specification'], // Results from a large sampling of post-production evaluations that are similar in compilation or specification
        ['ACE_EVAL_METHOD_CD','EMC','source:accuracy:horizontal:evaluation','product_specification_accuracy_value_(assumed_adequate)'], // Product specification accuracy value (assumed adequate)
        ['ACE_EVAL_METHOD_CD','EYC','source:accuracy:horizontal:evaluation','very_old_evaluation_of_unknown_origin'], // Very old evaluation of unknown origin
        ['ACE_EVAL_METHOD_CD','EZD','source:accuracy:horizontal:evaluation','estimate_based_on_analyst_familiarity'], // Estimate based on analyst familiarity
        ['ACE_EVAL_METHOD_CD','EZG','source:accuracy:horizontal:evaluation','insufficient_graticule_for_metric_evaluation_(assumed_limited)'], // Insufficient graticule for metric evaluation (assumed limited)
        ['ACE_EVAL_METHOD_CD','EZM','source:accuracy:horizontal:evaluation','scale_too_small_(<1:300_000)_to_perform_metric_evaluation_on_gdas_systems_(assumed_adequate)'], // Scale too small (<1:300 000) to perform metric evaluation on GDAS systems (assumed adequate)
        ['ACE_EVAL_METHOD_CD','EZZ','source:accuracy:horizontal:evaluation','digital_product_no_evaluation_method_available'], // Digital Product no evaluation method available
        ['ACE_EVAL_METHOD_CD','FZD','source:accuracy:horizontal:evaluation','evaluation_deferred_(no_measurement)'], // Evaluation deferred (no measurement)

        // ALE_EVAL_METHOD_CD - Absolute Linear Error Evaluation Method 
        ['ALE_EVAL_METHOD_CD','ACA','source:accuracy:linear:evaluation','geodetic_survey_control_-_adequate_sample'], // Geodetic Survey Control - adequate sample
        ['ALE_EVAL_METHOD_CD','ACS','source:accuracy:linear:evaluation','geodetic_survey_control_-_small_sample'], // Geodetic Survey Control - small sample
        ['ALE_EVAL_METHOD_CD','AHA','source:accuracy:linear:evaluation','photogrammetric_(old_stats_unable_to_convert_to_ana_due_to_significant_bias)'], // Photogrammetric (old stats unable to convert to ANA due to significant bias)
        ['ALE_EVAL_METHOD_CD','ANA','source:accuracy:linear:evaluation','photogrammetric_control_(gdasii)_-_adequate_sample'], // Photogrammetric Control (GDASII) - adequate sample
        ['ALE_EVAL_METHOD_CD','ANS','source:accuracy:linear:evaluation','photogrammetric_control_(gdasii)_-_small_sample'], // Photogrammetric Control (GDASII) - small sample
        ['ALE_EVAL_METHOD_CD','APA','source:accuracy:linear:evaluation','photogrammetric_control_(iec_fpe_npf)_-_adequate_sample'], // Photogrammetric Control (IEC FPE NPF) - adequate sample
        ['ALE_EVAL_METHOD_CD','APS','source:accuracy:linear:evaluation','photogrammetric_control_(iec_fpe_npf)_-_small_sample'], // Photogrammetric Control (IEC FPE NPF) - small sample
        ['ALE_EVAL_METHOD_CD','AXB','source:accuracy:linear:evaluation','extrapolation'], // Extrapolation
        ['ALE_EVAL_METHOD_CD','AZD','source:accuracy:linear:evaluation','unable_to_perform_metric_evaluation_other_than_ezm_ezg_azf_and_aze_(assumed_adequate)'], // Unable to perform metric evaluation other than EZM EZG AZF and AZE (assumed adequate)
        ['ALE_EVAL_METHOD_CD','AZE','source:accuracy:linear:evaluation','insufficient_control_presently_available_to_perform_metric_evaluation_(assumed_adequate)'], // Insufficient control presently available to perform metric evaluation (assumed adequate)
        ['ALE_EVAL_METHOD_CD','AZF','source:accuracy:linear:evaluation','insufficient_identifiable_map_features_for_metric_evaluation_(assumed_adequate)'], // Insufficient identifiable map features for metric evaluation (assumed adequate)
        ['ALE_EVAL_METHOD_CD','BMD','source:accuracy:linear:evaluation','error_budget_evaluation_(production_data)'], // Error Budget Evaluation (Production Data)
        ['ALE_EVAL_METHOD_CD','CLD','source:accuracy:linear:evaluation','accuracy_evaluation_printed_on_the_sheet_or_evaluation_derived_from_source_accuracy'], // Accuracy evaluation printed on the sheet or evaluation derived from source accuracy
        ['ALE_EVAL_METHOD_CD','CXB','source:accuracy:linear:evaluation','results_from_a_large_sampling_of_post-production_evaluations_that_are_similar_in_compilation_or_specification'], // Results from a large sampling of post-production evaluations that are similar in compilation or specification
        ['ALE_EVAL_METHOD_CD','EMC','source:accuracy:linear:evaluation','product_specification_accuracy_value_(assumed_adequate)'], // Product specification accuracy value (assumed adequate)
        ['ALE_EVAL_METHOD_CD','EYC','source:accuracy:linear:evaluation','very_old_evaluation_of_unknown_origin'], // Very old evaluation of unknown origin
        ['ALE_EVAL_METHOD_CD','EZD','source:accuracy:linear:evaluation','estimate_based_on_analyst_familiarity'], // Estimate based on analyst familiarity
        ['ALE_EVAL_METHOD_CD','EZG','source:accuracy:linear:evaluation','insufficient_graticule_for_metric_evaluation_(assumed_limited)'], // Insufficient graticule for metric evaluation (assumed limited)
        ['ALE_EVAL_METHOD_CD','EZM','source:accuracy:linear:evaluation','scale_too_small_(<1:300_000)_to_perform_metric_evaluation_on_gdas_systems_(assumed_adequate)'], // Scale too small (<1:300 000) to perform metric evaluation on GDAS systems (assumed adequate)
        ['ALE_EVAL_METHOD_CD','EZZ','source:accuracy:linear:evaluation','digital_product_no_evaluation_method_available'], // Digital Product no evaluation method available
        ['ALE_EVAL_METHOD_CD','FZD','source:accuracy:linear:evaluation','evaluation_deferred_(no_measurement)'], // Evaluation deferred (no measurement)

        // APT - Airfield Type - Needs Work
        // ['APT','0','raw:APT','unknown'], // Unknown 
        ['APT','0',undefined,undefined], // Unknown 
        ['APT','1','aeroway:airfield:type','major'], // Major Airfield 
        ['APT','11','aeroway:airfield:type','heliport_at_hospitals'], // Heliport at Hospitals 
        ['APT','14','aeroway:airfield:type','airport/airfield'], // Airport/Airfield 
        ['APT','15','aeroway:airfield:type','undefined_landing_area'], // Undefined Landing Area 
        ['APT','2','aeroway:airfield:type','minor'], // Minor Airfield 
        ['APT','9','aeroway:airfield:type','heliport'], // Heliport 
        ['APT','997','aeroway:airfield:type','unpopulated'], // Unpopulated 
        ['APT','999','aeroway:airfield:type','other'], // Other 

        // ATC - Aqueduct Type Category 
        // ['ATC','0','aqueduct:type','unknown'], // Unknown 
        ['ATC','0',undefined,undefined], // Unknown 
        ['ATC','1','aqueduct:type','qanat'], // Qanat/Kanat/Karez Shaft 
        ['ATC','3','aqueduct:type','underground'], // Underground Aqueduct 
        ['ATC','999','aqueduct:type','other'], // Other 

        // ATN - Aids to Navigation 
        // ['ATN','0','navigation_aid','unknown'], // Unknown
        ['ATN','0',undefined,undefined], // Unknown 
        ['ATN','3','navigation_aid','lit'], // Lit
        ['ATN','4','navigation_aid','unlit'], // Unlit

        // BAC - Built-Up Area Classification 
        // ['BAC','0','building:density','unknown'], // Unknown 
        ['BAC','0',undefined,undefined], // Unknown 
        ['BAC','1','building:density','sparse_to_moderate'], // Sparse to Moderate 
        ['BAC','2','building:density','dense'], // Dense 

        // BCC - Bypass Condition Category 
        // ['BCC','0','bypass','unknown'], // Unknown 
        ['BCC','0',undefined,undefined], // Unknown 
        ['BCC','1','bypass','easy'], // Easy 
        ['BCC','2','bypass','difficult'], // Difficult 
        ['BCC','3','bypass','impossible'], // Impossible 

        // BFC - Building Function Category
        ['BFC','0','building','yes'],
        ['BFC','Unknown','building','yes'],
        ['BFC','1','use','manufacturing'], // Fabrication Structures 
        ['BFC','2','office','government'],
        ['BFC','Government Building','office','government'],
        ['BFC','3','building','capitol'], // Capitol Building 
        ['BFC','4','building','castle'], // Castle 
        ['BFC','5','amenity','government_administration_building'],
        ['BFC','Government Administration Building','amenity','government_administration_building'],
        ['BFC','6','building','hospital'],
        ['BFC','Hospital','building','hospital'],
        ['BFC','7','amenity','place_of_worship'],
        ['BFC','House of Worship','amenity','place_of_worship'],
        ['BFC','8','military','administration'],
        ['BFC','Military Administration/Operations Building','military','administration'],
        ['BFC','9','tourism','museum'],
        ['BFC','10','building','observatory'], // Observatory 
        ['BFC','11','building','palace'],
        ['BFC','Palace','building','palace'],
        ['BFC','12','amenity','police'],
        ['BFC','Police Station','amenity','police'],
        ['BFC','13','amenity','prison'],
        ['BFC','Prison','amenity','prison'],
        ['BFC','14','amenity','ranger_station'], // Ranger Station 
        ['BFC','15','building','school'],
        ['BFC','School','building','school'],
        ['BFC','16','building','residential'],
        ['BFC','House','building','residential'],
        ['BFC','17','building:multi_unit','yes'], // Multi Unit Dwelling 
        ['BFC','18','building','cemetery_building'],
        ['BFC','19','building','farm'],
        ['BFC','Farm Building','building','farm'],
        ['BFC','20','building','greenhouse'], // Greenhouse 
        ['BFC','21','building','garage'],
        ['BFC','22','building','watermill'], // Watermill/Gristmill 
        ['BFC','23','building','wind_tunnel'], // Wind Tunnel 
        ['BFC','24','building','warehouse'],
        ['BFC','25','railway','roundhouse'], // Roundhouse 
        ['BFC','26','repair','railway_vehicle'],
        ['BFC','27','man_made','depot'], // Depot Terminal 
        ['BFC','28','office','administrative'],
        ['BFC','Administration Building','office','administrative'],
        ['BFC','29','repair','aircraft'], // Aircraft Maintenance Shop 
        ['BFC','30','building','hangar'],
        ['BFC','Hangar','building','hangar'],
        ['BFC','31','building','customs_house'], // Customs House 
        ['BFC','33','building','healthcare'],
        ['BFC','34','sport','shooting_range'], // Firing Range 
        ['BFC','35','amenity','post_office'],
        ['BFC','Post Office','amenity','post_office'],
        ['BFC','36','military','barracks'],
        ['BFC','37','amenity','fire_station'],
        ['BFC','Barracks/Dormitory','military','barracks'],
        ['BFC','38','amenity','prison'], // Jail 
        ['BFC','40','use','telephone_switching_station'], // Telephone Switching Station 
        ['BFC','51','amenity','marketplace'],
        ['BFC','Market','amenity','marketplace'],
        ['BFC','52','amenity','townhall'], // Town Hall 
        ['BFC','53','amenity','bank'],
        ['BFC','54','amenity','fuel'],
        ['BFC','Service/Refueling Station','amenity','fuel'],
        ['BFC','55','leisure','yacht_club'], // Yacht Club/Sailing Club 
        ['BFC','56','amenity','pub'], // Public Inn 
        ['BFC','57','amenity','restaraunt'],
        ['BFC','58','use','observation'], // Observation 
        ['BFC','59','use','research'],
        ['BFC','Research and Development Lab/Research Facility','use','research'],
        ['BFC','61','amenity','courthouse'],
        ['BFC','Courthouse','amenity','courthouse'],
        ['BFC','62','building','legation'], // Legation 
        ['BFC','63','building','mission'], // Mission 
        ['BFC','64','building','chancery'], // Chancery 
        ['BFC','Mission','building','mission' ],
        ['BFC','65','amenity','ambassadorial_residence' ],
        ['BFC','66','amenity','embassy' ],
        ['BFC','67','amenity','consulate' ],
        ['BFC','Embassy','amenity','embassy' ],
        ['BFC','68','building','guardhouse'],
        ['BFC','Guard House','building','guardhouse'],
        ['BFC','69','building','guardroom'],
        ['BFC','70','amenity','animal_shelter'], // Kennel 
        ['BFC','71','industrial','oil-mill'], // Oil Mill (Vegetable) 
        ['BFC','72','industrial','aerator'], // Aerator 
        ['BFC','73','industrial','carpentry'], // Carpentry 
        ['BFC','74','craft','sawmilling'], // Sawmill 
        ['BFC','75','man_made','kiln'], // Kiln/Oven 
        ['BFC','76','use','signal_box'], // Signal Box/Railway Signalman's House 
        ['BFC','77','use','harbor_masters_office'], // Harbor Master's Office 
        ['BFC','78','building','marine_police'], // Marine Police 
        ['BFC','79','amenity','rescue'], // Rescue 
        ['BFC','82','man_made','lighthouse'], // Lighthouse 
        ['BFC','83','plant','power'],
        ['BFC','84','industrial','filtration_plant'], // Filtration Plant 
        ['BFC','85','amenity','newspaper_plant'],
        ['BFC','Processing/Treatment','industrial','manufacturing'],
        ['BFC','86','use','main_telephone_exchange'], // Telephone Exchange (Main) 
        ['BFC','87','amenity','auditorium'], // Auditorium 
        ['BFC','88','tourism','opera_house'], // Opera House 
        ['BFC','89','industrial','manufacturing'],
        ['BFC','90','building','pumping'], // Pumphouse 
        ['BFC','91','building','static_caravan'], // Mobile Home 
        ['BFC','92','man_made','weather_station'], // Weather Station 
        ['BFC','93','building','dependents_housing'], // Dependents Housing/Bivouac Area 
        ['BFC','94','railway','station'],
        ['BFC','95','tourism','hotel'],
        ['BFC','Hotel','tourism','hotel'],
        ['BFC','96','amenity','diplomatic_building'],
        ['BFC','97','building','trading_post'], // Trading Post 
        ['BFC','99','military','battery'], // Battery 
        ['BFC','100','building','healthcare'], // Medical Centre. Dont know if hospital, clinic etc
        ['BFC','101','building','municipal_hall'], // Municipal Hall 
        ['BFC','102','industrial','oil_gas_facility'], // Not great
        ['BFC','103','building','outbuilding'], // Outbuilding 
        ['BFC','104','industrial','paper_mill'],
        ['BFC','Oil/Gas Facilities Building','industrial','oil_gas_facility'], // Not great
        ['BFC','105','amenity','reformatory'], // Reformatory 
        ['BFC','106','amenity','sanitarium'], // Sanitarium 
        ['BFC','107','building','satellite_tracking_station'], // Satellite Tracking Station 
        ['BFC','109','social_facility','senior'], // Senior Citizen's Home 
        ['BFC','110','amenity','shipyard'], // Shipyard 
        ['BFC','111','leisure','sports_centre'],
        ['BFC','Sportsplex','leisure','sports_centre'],
        ['BFC','112','industrial','steel_mill'], // Steel Mill 
        ['BFC','113','amenity','weigh_scale_(highway)'], // Weigh Scale (Highway) 
        ['BFC','115','tourism','hostel'], // Hostel 
        ['BFC','116','industrial','manufacturing'],
        ['BFC','117','tourism','motel'],
        ['BFC','118','amenity','community_centre'],
        ['BFC','Community Center','amenity','community_centre'],
        ['BFC','119','amenity','townhall'],
        ['BFC','City Hall','amenity','townhall'],
        ['BFC','120','industrial','motor_vehicle'],
        ['BFC','121','building','armory'], // Armory 
        ['BFC','122','shop','mall'],
        ['BFC','123','amenity','prison'], // Correctional Institute 
        ['BFC','124','repair','yes'], // Repair Facility
        ['BFC','125','building','barn'],
        ['BFC','126','amenity','astronomical_station'], // Astronomical Station 
        ['BFC','127','amenity','theatre'], // Theater 
        ['BFC','Theater','amenity','theatre'],
        ['BFC','128','amenity','library'],
        ['BFC','129','aeroway','terminal'], // Airport Terminal 
        ['BFC','Airport Terminal','aeroway','terminal'],
        ['BFC','130','amenity','bus_station'],
        ['BFC','131','use','pilot_office'], // Pilot Office 
        ['BFC','132','use','pilot_look-out'], // Pilot Look-out 
        ['BFC','133','building','commercial'],
        ['BFC','140','building','yes'], // Significant building ???
        ['BFC','Non-Governmental building of Significance (other than government).','building','yes'], // Significant building ???
        ['BFC','150','military','barracks'],
        ['BFC','151','building','dormitory'], // Dormitory 
        ['BFC','723','amenity','fire_and_police'], // Combined Fire and Police Station 
        ['BFC','999','building','other'],
        ['BFC','Other','building','other'],
        // BFC - Building Function Category 

        // BOT - Bridge Opening Type
        ['BOT','0','bridge','yes'], // Unk
        ['BOT','4','bridge','bascule'],
        ['BOT','10','bridge','swing'],
        ['BOT','11','bridge','lift'],
        ['BOT','12','bridge','retractable'],
        ['BOT','13',undefined,undefined], // NA 
        ['BOT','14','bridge','submersible'],
        ['BOT','999','bridge','other'],

        // BSC - Bridge Structure
        // ['BSC','0','bridge:structure','unknown'],
        ['BSC','0',undefined,undefined],
        ['BSC','1','bridge:structure','open_spandrel_arch'],
        ['BSC','2','bridge:structure','cantilever'],
        ['BSC','3','bridge:structure','deck'],
        ['BSC','5','bridge:structure','floating'],
        ['BSC','6','bridge:structure','girder'],
        ['BSC','7','bridge:structure','tower_suspension'],
        ['BSC','8','bridge:structure','truss'],
        ['BSC','9','bridge:structure','suspension'],
        ['BSC','12','bridge:structure','transporter'],
        ['BSC','15','bridge:structure','slab'],
        ['BSC','16','bridge:structure','stringer_beam'],
        ['BSC','17','bridge:structure','arch_suspension'],
        ['BSC','26','bridge:structure','closed_spandrel_arch'],
        ['BSC','27','bridge:structure','cable_stayed'],
        ['BSC','28','bridge:structure','viaduct'], // ???
        ['BSC','997',undefined,undefined], // Unpopulated 
        ['BSC','999','bridge:structure','other'],

        // BSM - Bridge Span Mobility 
        // ['BSM','0','bridge:mobile_span','unknown'], // Unknown 
        ['BSM','0',undefined,undefined], // Unknown 
        ['BSM','1','bridge:mobile_span','yes'], // Moveable Span 
        ['BSM','2','bridge:mobile_span','no'], // Fixed Span 
        ['BSM','997','bridge:mobile_span','unpopulated'], // Unpopulated 

        // BST - Boundary Status Type 
        // ['BST','0','raw:BST','unknown'], // Unknown 
        ['BST','0',undefined,undefined], // Unknown 
        ['BST','1','raw:BST','definite'], // Definite 
        ['BST','2','raw:BST','indefinite'], // Indefinite 
        ['BST','3','raw:BST','in_dispute'], // In Dispute 
        ['BST','4','raw:BST','no_defined_boundary'], // No Defined Boundary 

        // BUD - Brush/Undergrowth Density Code 
        // ['BUD','0','undergrowth:density','unknown'], // Unknown 
        ['BUD','0',undefined,undefined], // Unknown 
        ['BUD','1','undergrowth:density','open'], // Open (<=5%) 
        ['BUD','2','undergrowth:density','sparse'], // Sparse (>5%<=15%) 
        ['BUD','3','undergrowth:density','medium'], // Medium (>15%<=50%) 
        ['BUD','4','undergrowth:density','dense'], // Dense (>50%) 
        ['BUD','997','undergrowth:density','unpopulated'], // Unpopulated 
        ['BUD','998','undergrowth:density','not_applicable'], // Not Applicable 

        // CAB - Cable Classification 
        // ['CAB','0','cable:type','unknown'], // Unknown 
        ['CAB','0',undefined,undefined], // Unknown 
        ['CAB','3','cable:type','telephone'], // Telephone 
        ['CAB','4','cable:type','telegraph'], // Telegraph 
        ['CAB','7','cable:type','digital_communication'], // Digital Wideband Communication Lines 

        // CET - Cut/Embankment Type Category 
        ['CET','1','raw:CET','one_side'], // One Side 
        ['CET','2','raw:CET','both_sides'], // Both Sides 

        // COC - Conspicuous Category 
        // ['COC','0','raw:COC','unknown'], // Unknown 
        ['COC','0',undefined,undefined], // Unknown 
        ['COC','1','raw:COC','conspicuous_from_sea'], // Conspicuous from sea 
        ['COC','6','raw:COC','inconspicuous'], // Inconspicuous 

        // COD - Certainty of Delineation 
        ['COD','1','deliniation','known'], // Limits and Information Known 
        ['COD','2','deliniation','unknown'], // Limits and Information Unknown 
        ['COD','997','deliniation','unpopulated'], // Unpopulated 

        //COE - Certainty of Existence - Really???
        ['COE','0',undefined,undefined], // Unknown - WTF?
        ['COE','1',undefined,undefined], // Definite - Default?
        ['COE','Definite',undefined,undefined], // Definite - Default?
        ['COE','2','existence','doubtful'], // Doubtful
        ['COE','3','existence','reported'], // Reported
        ['COE','4','existence','approximate'], // Approximate/About
        ['COE','997',undefined,undefined], // Unpopulated

        // COF - Complex Outline Function 
        // ['COF','0','raw:COF','unknown'], // Unknown 
        ['COF','0',undefined,undefined], // Unknown 
        ['COF','1','raw:COF','agricultural'], // Agricultural 
        ['COF','10','raw:COF','mixed_urban'], // Mixed Urban 
        ['COF','11','raw:COF','recreational'], // Recreational 
        ['COF','12','raw:COF','religious'], // Religious 
        ['COF','13','raw:COF','reserve'], // Reserve 
        ['COF','14','raw:COF','residential'], // Residential 
        ['COF','15','raw:COF','transportation'], // Transportation 
        ['COF','16','raw:COF','utilities'], // Utilities 
        ['COF','17','raw:COF','maritime'], // Maritime 
        ['COF','2','raw:COF','commercial'], // Commercial 
        ['COF','3','raw:COF','communications'], // Communications 
        ['COF','4','raw:COF','diplomatic'], // Diplomatic 
        ['COF','5','raw:COF','educational'], // Educational 
        ['COF','6','raw:COF','governmental'], // Governmental 
        ['COF','7','raw:COF','industrial'], // Industrial 
        ['COF','8','raw:COF','medical'], // Medical 
        ['COF','9','raw:COF','military'], // Military 
        ['COF','999','raw:COF','other'], // Other 

        // COMPLETENESS_CODE - Completeness Code 
        // The best guess is that this is "COP" which is "Creation Process Complete"
        // The definition is: 
        // "An indication that the process by which the digital representation of the 
        // feature(s) or data set(s) within an area is created, is complete in accordance 
        // with the specification that was used as the basis for defining the digital 
        // representation of the feature(s) or data set(s)."

        ['COMPLETENESS_CODE','0','is_complete','no'], // Not Complete 
        ['COMPLETENESS_CODE','1','is_complete','yes'], // Complete 

        // CRA - Crane Type Category 
        // ['CRA','0','crane:type','unknown'], // Unknown 
        ['CRA','0',undefined,undefined], // Unknown 
        ['CRA','2','crane:type','bridge'], // Bridge/Gantry 
        ['CRA','3','crane:type','rotating'], // Rotating 
        ['CRA','7','crane:type','tower'], // Tower Crane 
        ['CRA','999','crane:type','other'], // Other 

        // CRM - Crane Mobility Type 
        // ['CRM','0','crane:mobile','unknown'], // Unknown 
        ['CRM','0',undefined,undefined], // Unknown 
        ['CRM','1','crane:mobile','fixed'], // Fixed 
        ['CRM','2','crane:mobile','travelling'], // Traveling 
        ['CRM','3','crane:mobile','floating'], // Floating 
        ['CRM','999','crane:mobile','other'], // Other 

        // DFT - Dam Face Type 
        // ['DFT','0','raw:DFT','unknown'], // Unknown 
        ['DFT','0',undefined,undefined], // Unknown 
        ['DFT','1','dam:face:type','vertical'], // Vertical 
        ['DFT','2','dam:face:type','slope'], // Slope 

        // DMT - Density Measurement 
        // ['DMT','0','tree_density','unknown'], // Unknown 
        ['DMT','0',undefined,undefined], // Unknown 
        ['DMT','25','tree:density','scattered_trees'], // Scattered Trees 
        ['DMT','51','tree:density','trees/mangrove'], // Trees/Mangrove 

        // EBT - Educational Building Type 
        ['EBT','0',undefined,undefined], // Unknown 
        ['EBT','1','building','academy'], // Academy 
        ['EBT','2','building','college'], // College 
        ['EBT','3','building','educational_centre'], // Educational Center 
        ['EBT','4','building','lyceum'], // Lyceum 
        ['EBT','5','building','university'], // University 
        ['EBT','6','building','seminary'], // Seminary 
        ['EBT','8',undefined,undefined], // Not Applicable 
        ['EBT','999','building','other'], // Other 

        // ELA - Elevation Accuracy 
        // ['ELA','0','raw:ELA','unknown'], // Unknown 
        ['ELA','0',undefined,undefined], // Unknown 
        ['ELA','1','source:accuracy:vertical:category','accurate'], // Accurate
        ['ELA','2','source:accuracy:vertical:category','approximate'], // Approximate

        // EXS - Existence Category 
        // ['EXS','0','operational_status','unknown'], 
        ['EXS','0',undefined,undefined], 
        ['EXS','5','condition','construction'],
        ['EXS','Under Construction','condition','construction'],
        ['EXS','6','condition','abandoned'],
        ['EXS','Abandoned/Disused','condition','abandoned'],
        ['EXS','7','condition','destroyed'],
        ['EXS','Destroyed','condition','destroyed'],
        ['EXS','8','condition','dismantled'], // Dismantled 
        ['EXS','10','condition','proposed'], // Proposed 
        ['EXS','11','operational_status','temporary'], // Temporary 
        ['EXS','27','operational_status','closed'], // Closed/Locked 
        ['EXS','28','condition','functional'], // Fully Functional - default
        ['EXS','Operational','condition','functional'], // Fully Functional - default
        ['EXS','30','isolated','no'], // Not Isolated ???
        ['EXS','31','isolated','yes'], // Isolated 
        ['EXS','32','navigation','yes'], // Navigable 
        ['EXS','36','operational_status','commissioned_and_operational'], // Commissioned and Operational 
        ['EXS','37','operational_status','commissioned_and_on_test'], // Commissioned and on Test! 
        ['EXS','38','operational_status','commissioned_and_out_of_service'], // Commissioned and out of service 
        ['EXS','39','operational_status','not_commissioned_and_operational'], // Not commissioned and operational 
        ['EXS','40','operational_status','not_commissioned_and_on_test'], // Not commissioned and on test 
        ['EXS','41','operational_status','not_commissioned_and_out_of_service'], // Not commissioned and out of service 
        ['EXS','42','operating_cycle','continuous'], // Continuous Operation
        ['EXS','43','operating_cycle','intermittent'], // Intermittent operation
        ['EXS','45','artificial','no'], // Natural 
        ['EXS','46','artificial','yes'], // Man-Made 
        ['EXS','48','operational_status','controlled'], // Controlled 
        ['EXS','59','operational_status','not_usable'], // Not Usable 
        ['EXS','62','operational_status','partially_destroyed'], // Partially Destroyed 
        ['EXS','66','condition','damaged'],
        ['EXS','997',undefined,undefined], // Unpopulated
        ['EXS','999','operational_status','other'],

        // FCM - Feature Construction Material - Really??
        // ['FCM','0','raw:FCM','unknown'], // Unknown 
        ['FCM','0',undefined,undefined], // Unknown 
        ['FCM','1','material','concrete'], // C - Concrete 
        ['FCM','2','material','concrete_and_steel'], // L - Concrete and Steel 
        ['FCM','3','material','concrete_and_wood'], // D - Concrete and Wood 
        ['FCM','4','material','masonry'], // M - Masonry 
        ['FCM','5','material','masonry_and_steel'], // N - Masonry and Steel 
        ['FCM','6','material','masonry_and_wood'], // R - Masonry and Wood 
        ['FCM','7','material','steel'], // S - Steel 
        ['FCM','8','material','steel_and_wood'], // T - Steel and Wood 
        ['FCM','9','material','wood'], // W - Wood

        // FCO - Feature Configuration 
        // ['FCO','0','arrangement','unknown'], // Unknown 
        ['FCO','0',undefined,undefined], // Unknown 
        ['FCO','11','arrangement','double'], // Double 
        ['FCO','12','arrangement','juxtaposition'], // Juxtaposition 
        ['FCO','2','arrangement','multiple'], // Multiple 
        ['FCO','3','arrangement','single'], // Single 

        // FER - Ferry Type 
        // ['FER','0','ferry:type','unknown'], // Unknown 
        ['FER','0',undefined,undefined], // Unknown 
        ['FER','1','ferry:type','cable'], // With cables/chains 
        ['FER','2','ferry:type','free-moving'], // Without cables/chains 
        ['FER','997','ferry:type','unpopulated'], // Unpopulated 

        // FPT - Facility Priority Type 
        ['FPT','1','aeroway:airfield:type','major'], // Major 
        ['FPT','2','aeroway:airfield:type','minor_and_hard'], // Minor (Hard) 
        ['FPT','3','aeroway:airfield:type','minor_and_soft'], // Minor (Soft) 

        // FRT - Firing Range Type 
        // ['FRT','0','range:type','unknown'], // Unknown 
        ['FRT','0',undefined,undefined], // Unknown 
        ['FRT','1','range:type','small_arms'], // Rifle/Small Arms 
        ['FRT','2','range:type','tank'], // Tank 
        ['FRT','3','range:type','artillery'], // Artillery 
        ['FRT','4','range:type','grenade'], // Grenade 
        ['FRT','5','range:type','demolition'], // Demolition Area 
        ['FRT','6','range:type','impact_area'], // Impact Area 
        ['FRT','999','range:type','other'], // Other 

        // FTC - Farming Type Category 
        // ['FTC','0','farm:method','unknown'], // Unknown 
        ['FTC','0',undefined,undefined], // Unknown 
        ['FTC','1','farm:method','slash_and_burn'], // Slash & Burn - Shifting Cultivation 
        ['FTC','3','farm:pattern','terraced'], // Terraced 
        ['FTC','4','farm:method','permanent_irrigation'], // Ditch Irrigation 
        ['FTC','8','farm:method','crop_rotation'], // Crop Rotation 
        ['FTC','999','farm:method','other'], // Other 

        // FTI - Fence Type Indicator 
        // ['FTI','0','fence:type','unknown'], // Unknown 
        ['FTI','0',undefined,undefined], // Unknown 
        ['FTI','1','fence:type','metal'], // Metal 
        ['FTI','2','fence:type','wood'], // Wood 
        ['FTI','3','fence:type','stone'], // Stone 
        ['FTI','5','fence:type','barbed_wire'], // Barbed Wire 
        ['FTI','6','fence:type','chain_link'], // Chain Link 
        ['FTI','999','fence:type','other'], // Other

        // FTP - Fabrication Type 
        // ['FTP','0','raw:FTP','unknown'], // Unknown 
        ['FTP','0',undefined,undefined], // Unknown 
        ['FTP','1','use','light_fabrication'], // Light Fabrication 
        ['FTP','2','use','heavy_fabrication'], // Heavy Fabrication

        // GTC - Gate Type Category 
        // ['GTC','0','barrier','unknown'], // Unknown 
        ['GTC','0',undefined,undefined], // Unknown 
        ['GTC','1','barrier','toll_booth'], // Tollgate 
        ['GTC','2','barrier','crossing'], // Crossing 
        ['GTC','999','barrier','other'], // Other 

        // GUG - Guyed or Unguyed Category 
        // ['GUG','0','guyed','unknown'], // Unknown 
        ['GUG','0',undefined,undefined], // Unknown 
        ['GUG','1','guyed','yes'], // Guyed 
        ['GUG','2','guyed','no'], // Unguyed

        // HFC - Hydrologic Form Category
        ['HFC','0','waterway','yes'], 
        ['HFC','1','waterway','stream'],
        ['HFC','2','waterway','disappearing'], // Disappearing 
        ['HFC','8','waterway','river'],
        ['HFC','14','waterway','braided_stream'],
        ['HFC','16','waterway','dissipating'],
        ['HFC','19','waterway','gorge'],
        ['HFC','21','waterway','wadi'],
        ['HFC','999','waterway','other'],

        // HWT - House of Worship Type
        ['HWT','0',undefined,undefined],
        ['HWT','2','building','cathedral'],
        ['HWT','Cathedral','building','cathedral'],
        ['HWT','3','building','chapel'],
        ['HWT','4','building','church'],
        ['HWT','Church','building','church'],
        ['HWT','5','building','marabout'],
        ['HWT','6','building','minaret'],
        ['HWT','7','building','religious_community'], // Monastery Convent 
        ['HWT','9','building','mosque'],
        ['HWT','Mosque','building','mosque'],
        ['HWT','11','building','pagoda'],
        ['HWT','14','building','shrine'],
        ['HWT','15','building','tabernacle'],
        ['HWT','16','building','temple'],
        ['HWT','20','building','synagogue'],
        ['HWT','21','building','stupa'],
        ['HWT','997',undefined,undefined], // Unpopulated 
        ['HWT','998',undefined,undefined],
        ['HWT','Not Applicable',undefined,undefined],
        ['HWT','999','building:religious','other'], // To avoid conflicts

        // HYC - Hydrologic Persistance
        // ['HYC','0','intermittent','unknown'], 
        ['HYC','0',undefined,undefined], 
        ['HYC','2',undefined,undefined], // Not Applicable 
        ['HYC','3','intermittent','dry'],
        ['HYC','6','intermittent','yes'], // Non-Perennial/Intermittent/Fluctuating 
        ['HYC','8','intermittent','no'], // Perennial

        // IWO - Inland Water Obstruction 
        // ['IWO','0','raw:IWO','unknown'], // Unknown 
        ['IWO','0',undefined,undefined], // Unknown 
        ['IWO','1','raw:IWO','obstruction'], // Obstruction 
        ['IWO','2','raw:IWO','not_an_obstruction'], // Not an obstruction 

        // LFA - Light Function Aeronautical 
        // ['LFA','0','aeroway:LFA','unknown'], // Unknown 
        ['LFA','0',undefined,undefined], // Unknown 
        ['LFA','1','aeroway:light','airport_terminal_lights'], // Airport Terminal Lights 
        ['LFA','2','aeroway:light','apron_flood'], // Apron Flood 
        ['LFA','3','aeroway:light','boundary'], // Boundary Lights 
        ['LFA','4','aeroway:light','runway_centreline'], // Runway Centerline Lighting 
        ['LFA','5','aeroway:light','runway_end_identification_lighting'], // Runway End Identification Lighting (REIL) 
        ['LFA','6','aeroway:light','runway_edge'], // Runway Lights/Runway Edge Lights 
        ['LFA','7','aeroway:light','sequenced_strobe'], // Sequenced Strobe 
        ['LFA','8','aeroway:light','taxiway_lighting'], // Taxiway Lighting 
        ['LFA','9','aeroway:light','visual_approach_slope_indicator'], // Visual Approach Slope Indicator (VASI) 
        ['LFA','10','aeroway:light','rotating_beacon'], // Rotating Beacon 
        ['LFA','11','aeroway:light','obstruction_lighting'], // Obstruction Lighting 
        ['LFA','12','aeroway:light','threshold'], // Threshold Light(s) 
        ['LFA','13','aeroway:light','touchdown_zone'], // Touchdown Zone Lighting 
        ['LFA','14','aeroway:light','other_airport_lighting'], // Other Airport Lighting 
        ['LFA','15','aeroway:light','alsf-i'], // ALSF-1 (Approach Lighting System with seq. flashing) 
        ['LFA','16','aeroway:light','alsf-ii'], // ALSF-II 
        ['LFA','17','aeroway:light','ssalf'], // (SSALF) 
        ['LFA','18','aeroway:light','ssalr'], // (SSALR) 
        ['LFA','19','aeroway:light','malsf'], // (MALSF) 
        ['LFA','20','aeroway:light','malsr'], // (MALSR) 
        ['LFA','21','aeroway:light','landing_direction_indicator'], // Landing Direction Indicator (LDIN) 
        ['LFA','22','aeroway:light','runway_alignment_indicator_lights'], // RAIL (Runway Alignment Indicator Lights)! 
        ['LFA','23','aeroway:light','omni_directional_approach_landing_system'], // ODALS (Omni Directional Approach Landing System) 
        ['LFA','24','aeroway:light','other_approach_lighting'], // Other Approach Lighting 
        ['LFA','25','aeroway:light','precision_approach_path_indicator'], // Precision Approach Path Indicator (PAPI)! 
        ['LFA','26','aeroway:light','strobe'], // Strobe 
        ['LFA','27','aeroway:light','runway_flood'], // Runway Flood 
        ['LFA','28','aeroway:light','variable_intensity_runway_lights'], // Variable Intensity Runway Lights 
        ['LFA','29','aeroway:light','portable_runway_lights'], // Portable Runway Lights 
        ['LFA','30','aeroway:light','flares'], // Flares 
        ['LFA','31','aeroway:light','wind_indicator_lights'], // Wind Indicator Lights 
        ['LFA','32','aeroway:light','visual_approach_slope_indicator_3_bar'], // Visual Approach Slope Indicator (3 bar) 
        ['LFA','33','aeroway:light','optical_landing_system'], // Optical Landing System 
        ['LFA','53','aeroway:light','beacon'], // Beacon 
        ['LFA','72','aeroway:light','identification_beacon'], // Identification Beacon 
        ['LFA','73','aeroway:light','none_available'], // None available 
        ['LFA','100','aeroway:light','overrun_centreline'], // Overrun centerline 
        ['LFA','101','aeroway:light','centreline_and_bar'], // Centerline and bar 
        ['LFA','102','aeroway:light','u.s._configuration_(b)'], // U.S. Configuration (B) 
        ['LFA','103','aeroway:light','hong_kong_curve'], // Hong Kong Curve 
        ['LFA','104','aeroway:light','left_single_row'], // Left single row 
        ['LFA','105','aeroway:light','centre_row'], // Center row 
        ['LFA','106','aeroway:light','former_nato_standard'], // Former NATO standard 
        ['LFA','107','aeroway:light','nato_standard'], // NATO standard 
        ['LFA','108','aeroway:light','centre_and_double_row'], // Center and double row 
        ['LFA','109','aeroway:light','portable_approach'], // Portable approach 
        ['LFA','110','aeroway:light','centre_row_category_2'], // Center row (Cat 2 high visibility) 
        ['LFA','111','aeroway:light','centre_row_category_1'], // Center row (Cat 1 high intensity) 
        ['LFA','112','aeroway:light','navy_parallel_row_and_crossbar'], // Navy parallel row and crossbar 
        ['LFA','113','aeroway:light','two_parallel_row'], // Two parallel row 
        ['LFA','114','aeroway:light','left_row_high_intensity'], // Left row (High intensity) 
        ['LFA','115','aeroway:light','air_force_overrun'], // Air Force overrun 
        ['LFA','116','aeroway:light','calvert'], // Calvert (British) 
        ['LFA','117','aeroway:light','single_row_centreline'], // Single row centerline 
        ['LFA','118','aeroway:light','narrow_multi_cross'], // Narrow multi cross 
        ['LFA','119','aeroway:light','centreline_high_intensity_approach_lights'], // Centerline (High intensity approach lights) 
        ['LFA','120','aeroway:light','alternate_centreline_and_bar_approach_lights'], // Alternate centerline and bar approach lights 
        ['LFA','121','aeroway:light','cross'], // Cross 
        ['LFA','122','aeroway:light','neon_ladder'], // Neon ladder 
        ['LFA','123','aeroway:light','singapore_centreline_approach_lights'], // Singapore centerline approach lights 
        ['LFA','124','aeroway:light','centreline_2_crossbars_approach_lights'], // Centerline 2 crossbars approach lights 
        ['LFA','125','aeroway:light','t-vasi'], // T-VASI T-bar 
        ['LFA','126','aeroway:light','pvasi'], // PVASI pulsating 
        ['LFA','127','aeroway:light','apap_alignment'], // APAP alignment of elements systems 
        ['LFA','128','aeroway:light','vasi_with_threshold_indicator'], // VASI (with indicator for threshold crossing height) 
        ['LFA','129','aeroway:light','lcvasi'], // LCVASI low cost 
        ['LFA','130','aeroway:light','high_intensity_runway_lights'], // High intensity runway lights 
        ['LFA','131','aeroway:light','medium_intensity_runway_lights'], // Medium intensity runway lights 
        ['LFA','132','aeroway:light','low_intensity_runway_lights'], // Low intensity runway lights 
        ['LFA','999','aeroway:light','other'], // Other 

        // LMC - Landmark Category 
        // ['LMC','0','navigation_landmark','unknown'], // Unknown 
        ['LMC','0',undefined,undefined], // Unknown 
        ['LMC','1','navigation:landmark','yes'], // Landmark 
        ['LMC','2','navigation:landmark','no'], // Not a landmark 
        ['LMC','997',undefined,undefined], // Unpopulated 

        // LOC - Vertical Relative Location
        // ['LOC','0','location','unknown'], 
        ['LOC','0',undefined,undefined], 
        ['LOC','1','location','above_surface/does_not_cover_(height_known)'], // Above Surface/Does not Cover (Height Known) 
        ['LOC','2','location','awash_at_chart_datum'], // Awash at Chart Datum 
        ['LOC','4','location','underwater'], // From DIGEST FACC - Below Water Surface
        ['LOC','8','location','surface'], // From DIGEST FACC
        ['LOC','On Ground Surface','location','surface'], // From DIGEST FACC
        ['LOC','15','location','on_water_surface/floating'], // On Water Surface/Floating 
        ['LOC','17','location','on_waterbody_bottom'], // No OSM
        ['LOC','22','location','offshore'], // Off Shore 
        ['LOC','23','location','below_waterbody_bottom'], // No OSM
        ['LOC','24','location','suspended_or_elevated_above_sea_bottom'], // Suspended or Elevated above sea bottom 
        ['LOC','25','location','overground'], // From DIGEST FACC - Elevated above ground or water surface
        ['LOC','Suspended or Elevated Above Ground or Water Surface','location','overground'], // From DIGEST FACC - Elevated above ground or water surface
        ['LOC','30','location','non-floating'], // Non-Floating 
        ['LOC','40','location','underground'],
        ['LOC','44','location','surface'], // The default is on the surface
        ['LOC','45','location','overground'], // Above ground
        ['LOC','46','location','underwater'], // Above waterbody bottom
        ['LOC','997',undefined,undefined], // Unpopulated 
        ['LOC','998',undefined,undefined], // Not Applicable 
        ['LOC','999',undefined,undefined], // Other 

        // MAC - Maritime Area Category 
        // ['MAC','0','raw:MAC','unknown'], // Unknown 
        ['MAC','0',undefined,undefined], // Unknown 
        ['MAC','11','raw:MAC','anchorage_(general)'], // Anchorage (general) 
        ['MAC','12','raw:MAC','anchoring_berths'], // Anchoring Berths 
        ['MAC','13','raw:MAC','explosives_anchorage'], // Explosives anchorage 
        ['MAC','14','raw:MAC','large_vessel/deep_water/deep_draft_anchorage'], // Large Vessel/Deep Water/Deep Draft anchorage 
        ['MAC','16','raw:MAC','quarantine_anchorage'], // Quarantine Anchorage 
        ['MAC','17','raw:MAC','reserved_anchorage'], // Reserved Anchorage 
        ['MAC','19','raw:MAC','tanker_anchorage'], // Tanker Anchorage 
        ['MAC','53','raw:MAC','seaplane'], // Seaplane 
        ['MAC','54','raw:MAC','time_limited'], // Time Limited 

        // MCC - Structural Material Type
        ['MCC','-32767',undefined,undefined], // Null
        // ['MCC','0','material','unknown'],
        ['MCC','0',undefined,undefined],
        ['MCC','2','material','aluminum'],
        ['MCC','5','material','asphalt'],
        ['MCC','7','material','bedrock'], // Bedrock 
        ['MCC','8','material','boulders'], // Boulders 
        ['MCC','9','material','brick'],
        ['MCC','14','material','cinders'], // Cinders 
        ['MCC','16','material','clay'],
        ['MCC','20','material','composition'],
        ['MCC','21','material','concrete'],
        ['MCC','22','material','conglomerate'],
        ['MCC','24','material','coral'], // Coral 
        ['MCC','30','material','earthen'], // Earthen 
        ['MCC','46','material','gravel'],
        ['MCC','52','material','lava'], // Lava 
        ['MCC','55','material','loess'], // Loess 
        ['MCC','57','material','macadam'],
        ['MCC','62','material','masonry'],
        ['MCC','64','material','metal'],
        ['MCC','65','material','mud'], // Mud 
        ['MCC','72','material','part_metal'],
        ['MCC','73','material','pebbles'],
        ['MCC','77','material','prestressed_concrete'],
        ['MCC','83','material','reinforced_concrete'],
        ['MCC','84','material','rock'],
        ['MCC','86','material','rubble'],
        ['MCC','88','material','sand'],
        ['MCC','96','material','shells'], // Shells 
        ['MCC','98','material','shingle'], // Shingle 
        ['MCC','99','material','silt'], // Silt 
        ['MCC','104','material','soil'],
        ['MCC','107','material','steel'],
        ['MCC','108','material','stone'],
        ['MCC','115','material','volcanic_ash'], // Volcanic Ash 
        ['MCC','117','material','wood'],
        ['MCC','998',undefined,undefined],
        ['MCC','999','material','other'],

        // MCS - Material Composition Secondary 
        // ['MCS','0','raw:MCS','unknown'], // Unknown 
        ['MCS','0',undefined,undefined], // Unknown 
        ['MCS','104','raw:MCS','soil'], // Soil 
        ['MCS','108','raw:MCS','stone'], // Stone 
        ['MCS','115','raw:MCS','volcanic_ash'], // Volcanic Ash 
        ['MCS','16','raw:MCS','clay'], // Clay 
        ['MCS','24','raw:MCS','coral'], // Coral 
        ['MCS','46','raw:MCS','gravel'], // Gravel 
        ['MCS','52','raw:MCS','lava'], // Lava 
        ['MCS','55','raw:MCS','loess'], // Loess 
        ['MCS','65','raw:MCS','mud'], // Mud 
        ['MCS','8','raw:MCS','boulders'], // Boulders 
        ['MCS','84','raw:MCS','rock/rocky'], // Rock/Rocky 
        ['MCS','88','raw:MCS','sand'], // Sand 
        ['MCS','98','raw:MCS','shingle'], // Shingle 
        ['MCS','99','raw:MCS','silt'], // Silt 
        ['MCS','999','raw:MCS','other'], // Other 

        // MED - Median Present
        // ['MED','0','median','unknown'], 
        ['MED','0',undefined,undefined], 
        ['MED','1','median','yes'],
        ['MED','2','median','no'], 
        ['MED','Without Median','median','no'], 

        // MIN - Mining Category 
        // ['MIN','0','mine:type','unknown'], // Unknown 
        ['MIN','0',undefined,undefined], // Unknown 
        ['MIN','2','mine:shaft:orientation','horizontal'], // Horizontal Shaft 
        ['MIN','3','mine:type','opencast'], // Open Pit 
        ['MIN','4','mine:type','placer'], // Placer 
        ['MIN','5','mine:type','prospect'], // Prospect 
        ['MIN','6','mine:type','strip-mine'], // Strip 
        ['MIN','7','mine:shaft:orientation','vertical'], // Vertical Shaft 
        ['MIN','8','mine:type','peatery'], // Peat Cuttings 
        ['MIN','9','mine:type','below_surface'], // Below Surface Mine 
        ['MIN','997','mine:type','unpopulated'], // Unpopulated 

        // MINE - Mining Category 
        // ['MIN','0','mine:type','unknown'], // Unknown 
        ['MINE','0',undefined,undefined], // Unknown 
        ['MINE','2','mine:shaft:orientation','horizontal'], // Horizontal Shaft 
        ['MINE','3','mine:type','opencast'], // Open Pit 
        ['MINE','4','mine:type','placer'], // Placer 
        ['MINE','5','mine:type','prospect'], // Prospect 
        ['MINE','6','mine:type','strip-mine'], // Strip 
        ['MINE','7','mine:shaft:orientation','vertical'], // Vertical Shaft 
        ['MINE','8','mine:type','peatery'], // Peat Cuttings 
        ['MINE','9','mine:type','below_surface'], // Below Surface Mine 
        ['MINE','997','mine:type','unpopulated'], // Unpopulated 

        // MST - Missile Site Type 
        // ['MST','0','missile','unknown'], // Unknown 
        ['MST','0',undefined,undefined], // Unknown 
        ['MST','1','missile','anti-ballistic_missile'], // ABM 
        ['MST','2','missile','intercontinental_ballistic_missile'], // ICBM 
        ['MST','3','missile','intermediate-range_ballistic_missile'], // IRBM 
        ['MST','4','missile','sa-1_guild'], // SA1 
        ['MST','5','missile','sa-2_guideline'], // SA2 
        ['MST','6','missile','sa-3_goa'], // SA3 
        ['MST','7','missile','sa-4_ganef'], // SA4 
        ['MST','8','missile','sa-5_gammon'], // SA5 
        ['MST','9','missile','sa-6_gainful'], // SA6 
        ['MST','10','missile','sa-7_grail'], // SA7 
        ['MST','11','missile','sa-8_gecko'], // SA8 
        ['MST','12','missile','sa-9_gaskin'], // SA9 
        ['MST','13','missile','medium-range_ballistic_missile'], // MRBM 
        ['MST','14','missile','surface-to-surface_missile'], // SSM 
        ['MST','15','missile','surface-to-air_missile'], // SAM 
        ['MST','17','missile','sa-10_grumble'], // Sa10 
        ['MST','999','missile','other'], // Other 

        // NST - Navigation System Types 
        // ['NST','0','navigationaid','unknown'], // Unknown
        ['NST','0',undefined,undefined], // Unknown 
        ['NST','2','navigationaid','consol'], // CONSOL
        ['NST','3','navigationaid','decca'], // DECCA
        ['NST','7','navigationaid','loran'], // Long Range Air Navigation System (LORAN)
        ['NST','8','navigationaid','omega'], // OMEGA
        ['NST','11','navigationaid','radar'], // Radar
        ['NST','12','navigationaid','radio'], // Radio
        ['NST','13','navigationaid','radio_telephone'], // Radio Telephone
        ['NST','15','navigationaid','television'], // TV
        ['NST','16','navigationaid','microwave'], // Microwave
        ['NST','33','navigationaid','radio_telegraph'], // Radio Telegraph
        ['NST','997','navigationaid','unpopulated'], // Unpopulated
        ['NST','999','navigationaid','other'], // Other

        // OCC - Overhead Clearance Category Code 
        // ['OCC','0','raw:OCC','unknown'], // Unknown 
        ['OCC','0',undefined,undefined], // Unknown 
        ['OCC','1','overhead_clearance','restricted'], // Restricted 
        ['OCC','2','overhead_clearance','unlimited'], // Unlimited 

        // OHD - Derived Obstacle Height/Depth Category 
        // ['OHD','0','height_depth_category','unknown'], // Unknown 
        ['OHD','0',undefined,undefined], // Unknown 
        ['OHD','1','height_depth_category','>1.5_and_<=5.0'], // >1.5 and <=5.0 
        ['OHD','2','height_depth_category','>5.0_and_<=10.0'], // >5.0 and <=10.0 
        ['OHD','3','height_depth_category','>10.0_and_<=20.0'], // >10.0 and <=20.0 
        ['OHD','4','height_depth_category','>20.0_and_<=40.0'], // >20.0 and <=40.0 
        ['OHD','5','height_depth_category','>40.0'], // >40.0 

        // ORD - Ordinal Category 
        // ['ORD','0','place:importance','unknown'], // Unknown 
        ['ORD','0',undefined,undefined], // Unknown 
        ['ORD','1','place:importance','first'], // Primary/First Order 
        ['ORD','2','place:importance','second'], // Secondary/Second Order 
        ['ORD','3','place:importance','third'], // Tertiary/Third Order 
        ['ORD','4','place:importance','fourth'], // Quartenary/Fourth Order 
        ['ORD','5','place:importance','fifth'], // Quinary/Fifth Order 
        ['ORD','999','place:importance','other'], // Other 

        // ORIGINATING_SOURCE - Originating Source Name 
        ['ORIGINATING_SOURCE','Arc Digitized Raster Graphic (ADRG)','source:name','arc_digitized_raster_graphic_(adrg)'], // Arc Digitized Raster Graphic (ADRG) 
        ['ORIGINATING_SOURCE','Automated Aeronautical Facilities Information File (AAFIF)','source:name','automated_aeronautical_facilities_information_file_(aafif)'], //  Automated Aeronautical Facilities Information File (AAFIF) 
        ['ORIGINATING_SOURCE','Boundaries (International Boundaries Database)','source:name','boundaries_(international_boundaries_database)'], // Boundaries (International Boundaries Database) 
        ['ORIGINATING_SOURCE','CHUM','source:name','chum'], // CHUM 
        ['ORIGINATING_SOURCE','City Graphic','source:name','city_graphic'], // City Graphic 
        ['ORIGINATING_SOURCE','Combat Chart','source:name','combat_chart'], // Combat Chart 
        ['ORIGINATING_SOURCE','Compressed Arc Digitized Raster Graphic (CADRG)','source:name','compressed_arc_digitized_raster_graphic_(cadrg)'], // Compressed Arc Digitized Raster Graphic (CADRG) 
        ['ORIGINATING_SOURCE','Controlled Imagery Base 1 (CIB1)','source:name','controlled_imagery_base_1_(cib1)'], // Controlled Imagery Base 1 (CIB1) 
        ['ORIGINATING_SOURCE','Controlled Imagery Base 5 (CIB5)','source:name','controlled_imagery_base_5_(cib5)'], // Controlled Imagery Base 5 (CIB5) 
        ['ORIGINATING_SOURCE','Digital Globe','source:name','digital_globe'], // Digital Globe 
        ['ORIGINATING_SOURCE','Digital Nautical Chart (DNC)','source:name','digital_nautical_chart_(dnc)'], // Digital Nautical Chart (DNC) 
        ['ORIGINATING_SOURCE','Digital Print File (DPF)','source:name','digital_print_file_(dpf)'], // Digital Print File (DPF) 
        ['ORIGINATING_SOURCE','Digital Terrain Elevation Data 1 (DTED1)','source:name','digital_terrain_elevation_data_1_(dted1)'], // Digital Terrain Elevation Data 1 (DTED1) 
        ['ORIGINATING_SOURCE','Digital Terrain Elevation Data 2 (DTED2)','source:name','digital_terrain_elevation_data_2_(dted2)'], // Digital Terrain Elevation Data 2 (DTED2) 
        ['ORIGINATING_SOURCE','Digital Terrain Elevation Data 3 (DTED3)','source:name','digital_terrain_elevation_data_3_(dted3)'], // Digital Terrain Elevation Data 3 (DTED3) 
        ['ORIGINATING_SOURCE','Digital Terrain Elevation Data 4 (DTED4)','source:name','digital_terrain_elevation_data_4_(dted4)'], // Digital Terrain Elevation Data 4 (DTED4) 
        ['ORIGINATING_SOURCE','Digital Topographic Data 1 (DTOP1)','source:name','digital_topographic_data_1_(dtop1)'], // Digital Topographic Data 1 (DTOP1) 
        ['ORIGINATING_SOURCE','Digital Topographic Data 2 (DTOP2)','source:name','digital_topographic_data_2_(dtop2)'], // Digital Topographic Data 2 (DTOP2) 
        ['ORIGINATING_SOURCE','Digital Topographic Data 3 (DTOP3)','source:name','digital_topographic_data_3_(dtop3)'], // Digital Topographic Data 3 (DTOP3) 
        ['ORIGINATING_SOURCE','Digital Topographic Data 4 (DTOP4)','source:name','digital_topographic_data_4_(dtop4)'], // Digital Topographic Data 4 (DTOP4) 
        ['ORIGINATING_SOURCE','Digital Topographic Data 5 (DTOP5)','source:name','digital_topographic_data_5_(dtop5)'], // Digital Topographic Data 5 (DTOP5) 
        ['ORIGINATING_SOURCE','Digital Vertical Obstruction File (DVOF)','source:name','digital_vertical_obstruction_file_(dvof)'], // Digital Vertical Obstruction File (DVOF) 
        ['ORIGINATING_SOURCE','Foundation Feature Data (FFD)','source:name','foundation_feature_data_(ffd)'], // Foundation Feature Data (FFD) 
        ['ORIGINATING_SOURCE','Foundation Feature Data/Relocatable Target Data (FFD/RTAD)','source:name','foundation_feature_data/relocatable_target_data_(ffd/rtad)'], //  Foundation Feature Data/Relocatable Target Data (FFD/RTAD) 
        ['ORIGINATING_SOURCE','GPS Field Collected Open Source','source:name','gps_field_collected_open_source'], // GPS Field Collected Open Source 
        ['ORIGINATING_SOURCE','GeoCover/LandCover','source:name','geocover/landcover'], // GeoCover/LandCover 
        ['ORIGINATING_SOURCE','GeoNames','source:name','geonames'], // GeoNames 
        ['ORIGINATING_SOURCE','Image City Map (ICM)','source:name','image_city_map_(icm)'], // Image City Map (ICM) 
        ['ORIGINATING_SOURCE','Imagery (IKONOS)','source:name','imagery_(ikonos)'], // Imagery (IKONOS) 
        ['ORIGINATING_SOURCE','Imagery (NTM)','source:name','imagery_(ntm)'], // Imagery (NTM) 
        ['ORIGINATING_SOURCE','Imagery (Other)','source:name','imagery_(other)'], // Imagery (Other) 
        ['ORIGINATING_SOURCE','Interim Terrain Data (ITD)','source:name','interim_terrain_data_(itd)'], // Interim Terrain Data (ITD) 
        ['ORIGINATING_SOURCE','Interim Vector Data (IVD)','source:name','interim_vector_data_(ivd)'], // Interim Vector Data (IVD) 
        ['ORIGINATING_SOURCE','Joint Operational Graphic (JOG)','source:name','joint_operational_graphic_(jog)'], // Joint Operational Graphic (JOG) 
        ['ORIGINATING_SOURCE','Landsat','source:name','landsat'], // Landsat 
        ['ORIGINATING_SOURCE','Littoral Warfare Data (LWD)','source:name','littoral_warfare_data_(lwd)'], // Littoral Warfare Data (LWD) 
        ['ORIGINATING_SOURCE','Mapping Charting and Geodetic (MC&G)','source:name','mapping_charting_and_geodetic_(mc&g)'], // Mapping Charting and Geodetic (MC&G) 
        ['ORIGINATING_SOURCE','Mission Specific Data 1 (MSD1)','source:name','mission_specific_data_1_(msd1)'], // Mission Specific Data 1 (MSD1) 
        ['ORIGINATING_SOURCE','Mission Specific Data 2 (MSD2)','source:name','mission_specific_data_2_(msd2)'], // Mission Specific Data 2 (MSD2) 
        ['ORIGINATING_SOURCE','Mission Specific Data 3 (MSD3)','source:name','mission_specific_data_3_(msd3)'], // Mission Specific Data 3 (MSD3) 
        ['ORIGINATING_SOURCE','Mission Specific Data 4 (MSD4)','source:name','mission_specific_data_4_(msd4)'], // Mission Specific Data 4 (MSD4) 
        ['ORIGINATING_SOURCE','Mission Specific Data 5 (MSD5)','source:name','mission_specific_data_5_(msd5)'], // Mission Specific Data 5 (MSD5) 
        ['ORIGINATING_SOURCE','Modernized Integrated Data Base (MIDB)','source:name','modernized_integrated_data_base_(midb)'], // Modernized Integrated Data Base (MIDB) 
        ['ORIGINATING_SOURCE','Multiple','source:name','multiple'], // Multiple 
        ['ORIGINATING_SOURCE','NIMA GIS Medical Facilities Database','source:name','nima_gis_medical_facilities_database'], // NIMA GIS Medical Facilities Database 
        ['ORIGINATING_SOURCE','Native Data Source','source:name','native_data_source'], // Native Data Source 
        ['ORIGINATING_SOURCE','Native Map Source','source:name','native_map_source'], // Native Map Source 
        ['ORIGINATING_SOURCE','Nominally Attributed Topographic Evaluation Map (NATE Map)','source:name','nominally_attributed_topographic_evaluation_map_(nate_map)'], //  Nominally Attributed Topographic Evaluation Map (NATE Map) 
        ['ORIGINATING_SOURCE','Other','source:name','other'], // Other 
        ['ORIGINATING_SOURCE','Planning Graphic','source:name','planning_graphic'], // Planning Graphic 
        ['ORIGINATING_SOURCE','Reference Map','source:name','reference_map'], // Reference Map 
        ['ORIGINATING_SOURCE','Shuttle Radar Topographic Mission (SRTM)','source:name','shuttle_radar_topographic_mission_(srtm)'], // Shuttle Radar Topographic Mission (SRTM) 
        ['ORIGINATING_SOURCE','Site Map/Nuclear Facility','source:name','site_map/nuclear_facility'], // Site Map/Nuclear Facility 
        ['ORIGINATING_SOURCE','Tactical Oceanographic Data Lv 0 (TOD0)','source:name','tactical_oceanographic_data_lv_0_(tod0)'], // Tactical Oceanographic Data Lv 0 (TOD0) 
        ['ORIGINATING_SOURCE','Tactical Oceanographic Data Lv 1 (TOD1)','source:name','tactical_oceanographic_data_lv_1_(tod1)'], // Tactical Oceanographic Data Lv 1 (TOD1) 
        ['ORIGINATING_SOURCE','Tactical Oceanographic Data Lv 2 (TOD2)','source:name','tactical_oceanographic_data_lv_2_(tod2)'], // Tactical Oceanographic Data Lv 2 (TOD2) 
        ['ORIGINATING_SOURCE','Tactical Oceanographic Data Lv 4 (TOD4)','source:name','tactical_oceanographic_data_lv_4_(tod4)'], // Tactical Oceanographic Data Lv 4 (TOD4) 
        ['ORIGINATING_SOURCE','Tactical Oceanographic Data Lv 5 (TOD5)','source:name','tactical_oceanographic_data_lv_5_(tod5)'], // Tactical Oceanographic Data Lv 5 (TOD5) 
        ['ORIGINATING_SOURCE','Topographic Line Map (TLM)','source:name','topographic_line_map_(tlm)'], // Topographic Line Map (TLM) 
        ['ORIGINATING_SOURCE','United Nations Data','source:name','united_nations_data'], // United Nations Data 
        ['ORIGINATING_SOURCE','Unknown','source:name','unknown'], // Unknown 
        ['ORIGINATING_SOURCE','Urban Vector Map (UVMap)','source:name','urban_vector_map_(uvmap)'], // Urban Vector Map (UVMap) 
        ['ORIGINATING_SOURCE','Vector Interim Terrain Data (VITD)','source:name','vector_interim_terrain_data_(vitd)'], // Vector Interim Terrain Data (VITD) 
        ['ORIGINATING_SOURCE','Vector Map Lv0 (VMap0)','source:name','vector_map_lv0_(vmap0)'], // Vector Map Lv0 (VMap0) 
        ['ORIGINATING_SOURCE','Vector Map Lv1 (VMap1)','source:name','vector_map_lv1_(vmap1)'], // Vector Map Lv1 (VMap1) 
        ['ORIGINATING_SOURCE','Vector Map Lv2 (VMap2)','source:name','vector_map_lv2_(vmap2)'], // Vector Map Lv2 (VMap2) 
        ['ORIGINATING_SOURCE','World Vector Shoreline Plus (WVSPlus)','source:name','world_vector_shoreline_plus_(wvsplus)'], // World Vector Shoreline Plus (WVSPlus) 

        // OWO - Over Water Obstruction 
        ['OWO','1','waterway:overhead_obstruction','yes'], // Feature crosses navigable water 
        ['OWO','2','waterway:overhead_obstruction','no'], // Feature does not cross navigable water 
        ['OWO','997','waterway:overhead_obstruction','unpopulated'], // Unpopulated 

        // PPC - Power Plant Category 
        // ['PPC','0','generator:source','unknown'], // Unknown 
        ['PPC','0',undefined,undefined], // Unknown 
        ['PPC','1','generator:source','hydro'], // Hydro-electric 
        ['PPC','2','generator:source','nuclear'], // Nuclear 
        ['PPC','3','generator:source','solar'], // Solar 
        ['PPC','4','generator:source','thermal'], // Thermal 
        ['PPC','5','generator:source','wind'], // Wind 
        ['PPC','6','generator:source','tidal'], // Tidal 
        ['PPC','7','generator:source','internal_combustion'], // Internal Combustion 
        ['PPC','9','generator:source','substation'], // Substation 
        ['PPC','10','generator:source','transformer_yard'], // Transformer Yard 
        ['PPC','997','generator:source','unpopulated'], // Unpopulated 
        ['PPC','998','generator:source','not_applicable'], // Not Applicable 
        ['PPC','999','generator:source','other'], // Other 

        // PPT - Populated Place Type 
        // ['PPT','0','raw:PPT','unknown'], // Unknown 
        ['PPT','0',undefined,undefined], // Unknown 
        ['PPT','3','raw:PPT','tent_dwellings'], // Tent Dwellings 
        ['PPT','999','raw:PPT','other'], // Other 

        // PRM - Permanency 
        // ['PRM','0','permanent','unknown'], // Unknown 
        ['PRM','0',undefined,undefined], // Unknown 
        ['PRM','1','permanent','yes'], // Permanent 
        ['PRM','2','permanent','no'], // Temporary 
        ['PRM','999','permanent','other'], // Other 

        // PRO - Product Category 
        // ['PRO','0','product','unknown'], // Unknown 
        ['PRO','0',undefined,undefined], // Unknown 
        ['PRO','1','product','aircraft'], // Aircraft 
        ['PRO','2','product','aluminum'],
        ['PRO','3','product','ammunition'],
        ['PRO','5','product','asphalt'], // Asphalt 
        ['PRO','9','product','brick'], // Brick 
        ['PRO','11','product','cement'],
        ['PRO','12','product','chalk'], // Chalk 
        ['PRO','13','product','chemical'], // Chemical 
        ['PRO','16','product','clay'], // Clay 
        ['PRO','17','product','coal'], // Coal 
        ['PRO','19','product','coke'], // Coke 
        ['PRO','20','product','composition'], // Composition 
        ['PRO','21','product','concrete'], // Concrete 
        ['PRO','23','product','copper'], // Copper 
        ['PRO','26','product','desalinated_water'], // Desalinated Water 
        ['PRO','27','product','diamonds'], // Diamonds 
        ['PRO','29','product','dolomite'], // Dolomite 
        ['PRO','31','product','electric_power'],
        ['PRO','33','product','explosive'], // Explosives 
        ['PRO','35','product','food'],
        ['PRO','38','product','gas'], // Gas 
        ['PRO','39','product','petrol'], // Gasoline 
        ['PRO','40','product','glass'], // Glass 
        ['PRO','42','product','gold'], // Gold 
        ['PRO','43','product','granite'], // Granite 
        ['PRO','46','product','gravel'], // Gravel 
        ['PRO','50','product','heat'], // Heat 
        ['PRO','51','product','iron'], // Iron 
        ['PRO','54','product','lead'], // Lead 
        ['PRO','56','product','lumber'],
        ['PRO','57','product','macadam'], // Macadam 
        ['PRO','59','product','manganese'], // Manganese 
        ['PRO','60','product','marble'], // Marble 
        ['PRO','64','product','metal'],
        ['PRO','67','product','oil'],
        ['PRO','71','product','paper'], // Paper 
        ['PRO','72','product','part_metal'], // Part Metal 
        ['PRO','74','product','plastic'],
        ['PRO','80','product','quartz'], // Quartz 
        ['PRO','82','product','radioactive_material'], // Radioactive Material 
        ['PRO','84','product','rock/rocky'], // Rock/Rocky 
        ['PRO','85','product','rubber'], // Rubber 
        ['PRO','87','product','salt'], // Salt 
        ['PRO','88','product','sand'], // Sand 
        ['PRO','89','product','sandstone'], // Sandstone 
        ['PRO','95','product','sewage'],
        ['PRO','100','product','silver'], // Silver 
        ['PRO','101','product','slag'], // Slag 
        ['PRO','103','product','snow/ice'], // Snow/Ice 
        ['PRO','107','product','steel'], // Steel 
        ['PRO','108','product','stone'], // Stone 
        ['PRO','109','product','sugar'], // Sugar 
        ['PRO','110','product','travertine'], // Travertine 
        ['PRO','112','product','uranium'], // Uranium 
        ['PRO','113','product','vegetation_products'], // Vegetation Products 
        ['PRO','116','product','water'],
        ['PRO','117','product','wood'], // Wood 
        ['PRO','118','product','zinc'], // Zinc 
        ['PRO','119','product','bauxite'], // Bauxite 
        ['PRO','120','product','bananas'], // Bananas 
        ['PRO','121','product','cotton'],
        ['PRO','122','product','bamboo'], // Bamboo 
        ['PRO','123','product','coffee'],
        ['PRO','124','product','common_fruit_and/or_nuts'], // Common fruit and/or nuts 
        ['PRO','125','product','palms'], // Palms 
        ['PRO','126','product','palmetto'], // Palmetto 
        ['PRO','127','product','tailings'], // Tailings 
        ['PRO','128','product','refuse'], // Refuse 
        ['PRO','129','product','tobacco'],
        ['PRO','130','product','none'], // None 
        ['PRO','131','product','personnel'], // Personnel 
        ['PRO','133','product','telecommunications'], // Telecommunications 
        ['PRO','134','product','fish'], // Fish 
        ['PRO','135','product','textile'], // Textile 
        ['PRO','137','product','automobiles_and_trucks'], // Automobiles and Trucks 
        ['PRO','996','product','unpopulated'], // Unpopulated 
        ['PRO','997',undefined,undefined],
        ['PRO','Not Applicable',undefined,undefined],
        ['PRO','998','product','multiple'], // Multiple 
        ['PRO','999','product','other'], // Other 
        ['PRO','Other','product','other'],

        // REL - Religious Designation
        ['REL','0','religion','unknown'],
        // ['REL','0',undefined,undefined],
        ['REL','1','religion','buddhist'],
        ['REL','2','religion','muslim'],
        ['REL','Moslem','religion','muslim'],
        ['REL','3','religion','roman_catholic'],
        ['REL','4','religion','christian'],
        ['REL','Christian (undefined)','religion','christian'],
        ['REL','5','religion','jewish'],
        ['REL','6','religion','orthodox'],
        ['REL','7','religion','protestant'],
        ['REL','8','religion','shinto'],
        ['REL','998',undefined,undefined],
        ['REL','Not Applicable',undefined,undefined],
        ['REL','999','religion','other'],

        // RGC - Railroad Gauge Category 
        // ['RGC','0','gauge','unknown'], 
        ['RGC','0',undefined,undefined], 
        ['RGC','1','gauge:type','broad'],
        ['RGC','2','gauge:type','narrow'],
        ['RGC','3','gauge:type','standard'],
        ['RGC','6','railway','monorail'], // Gauge = 0.5? 
        ['RGC','997',undefined,undefined], // In data, not in spec

        // RIT - Road Interchange Type 
        ['RIT','0','junction','yes'], 
        ['RIT','1','junction','cloverleaf'],
        ['RIT','2','junction','diamond'],
        ['RIT','3','junction','fork'],
        ['RIT','4','junction','rotary'],
        ['RIT','5','junction','staggered_ramps'],
        ['RIT','6','junction','standard_ramps'],
        ['RIT','7','junction','symmetrical_ramps'],
        ['RIT','8','junction','trumpet'],
        ['RIT','9','junction','turban'],
        ['RIT','10','junction','wye'],
        ['RIT','999','junction','other'],

        // RRA - Railway Power Method
        ['RRA','-999999',undefined,undefined], // In data, not in spec
        // ['RRA','0','electrified','unknown'], 
        ['RRA','0',undefined,undefined], 
        ['RRA','1','electrified','rail'],
        ['RRA','3','electrified','contact_line'],
        ['RRA','4','electrified','no'],
        ['RRA','999',undefined,undefined], // Other

        // RRC - Railway Use
        ['RRC','-32768',undefined,undefined], // Null
        ['RRC','-999999',undefined,undefined], 
        ['RRC','0','railway','yes'],
        ['RRC','2','railway','carline'],
        ['RRC','3','railway','monorail'],
        ['RRC','6','railway','subway'],
        ['RRC','8','railway','logging'],
        ['RRC','11','railway','rapid_transit'],
        ['RRC','14','railway','tram'],
        ['RRC','15','railway','funicular'],
        ['RRC','16','railway','yes'],
        ['RRC','17','usage','branch'], // Branch Line 
        ['RRC','18','usage','main'], // Main Line/Branch Line 
        ['RRC','21','railway:in_road','yes'], // Railroad in Road 
        ['RRC','31','railway','magnetic_levitation'], // Magnetic Levitation 
        ['RRC','32','railway','automated_transit_system'],
        ['RRC','33','railway','longhaul'],
        ['RRC','999','railway','Other'],

        // RSA - Railroad Siding Attribute 
        // ['RSA','0','service','unknown'], // Unknown 
        ['RSA','0',undefined,undefined], // Unknown 
        ['RSA','1','service','spur'], // Spur 
        ['RSA','2','service','siding'], // Siding 
        ['RSA','3','service','passing'], // Passing 

        // RST - Load Bearing Surface Type
        // ['RST','0','surface','unknown'], 
        ['RST','0',undefined,undefined], 
        ['RST','1','surface','paved'], 
        ['RST','Hard/Paved','surface','paved'], 
        ['RST','2','surface','unpaved'], 
        ['RST','Loose/Unpaved','surface','unpaved'], 
        ['RST','5','surface','grass'], // Grass/Sod (Soft) 
        ['RST','6','surface','ground'], // Natural 
        ['RST','997',undefined,undefined],  // Unpopulated
        ['RST','999',undefined,undefined], // Other 

        // RSTRN_DSEM_CTRL_NIC - Restrictions on Dissemination Control Non-IC
        // Kind of ugly and will probably cause problems with dirty-word checkers
        ['RSTRN_DSEM_CTRL_NIC','SC','security:dissemination_control:non_ic','SC'],
        ['RSTRN_DSEM_CTRL_NIC','SINFO','security:dissemination_control:non_ic','SINFO'],
        ['RSTRN_DSEM_CTRL_NIC','DS','security:dissemination_control:non_ic','DS'],
        ['RSTRN_DSEM_CTRL_NIC','LIMDIS','security:dissemination_control:non_ic','DS'],
        ['RSTRN_DSEM_CTRL_NIC','XD','security:dissemination_control:non_ic','XD'],
        ['RSTRN_DSEM_CTRL_NIC','ND','security:dissemination_control:non_ic','ND'],
        ['RSTRN_DSEM_CTRL_NIC','SBU','security:dissemination_control:non_ic','SBU'],
        ['RSTRN_DSEM_CTRL_NIC','SBU-NF','security:dissemination_control:non_ic','SBU-NF'],
        ['RSTRN_DSEM_CTRL_NIC','LES','security:dissemination_control:non_ic','LES'],
        ['RSTRN_DSEM_CTRL_NIC','LES-NF','security:dissemination_control:non_ic','LES-NF'],

        // RTP - Reservoir Type 
        // ['RTP','0','reservoir:type','unknown'], // Unknown 
        ['RTP','0',undefined,undefined], // Unknown 
        ['RTP','1','reservoir:type','constructed_basin'], // Constructed Basin 
        ['RTP','2','reservoir:type','back-up_water/impounded_by_a_dam'], // Back-up Water/Impounded by a Dam 

        // RTT - Route Intended Use
        ['RTT','0','highway','road'], 
        ['RTT','14','highway','primary'], 
        ['RTT','Primary Route','highway','primary'], 
        ['RTT','15','highway','secondary'], 
        ['RTT','Secondary Route','highway','secondary'], 
        ['RTT','16','highway','motorway'], // Limited Access
        ['RTT','997','highway','road'], // Unpopulated
        ['RTT','999','highway','road'], // Unknown

        // SCB - Surface Condition Attribute 
        // ['SCB','0','surface:condition','unknown'], // Unknown 
        ['SCB','0',undefined,undefined], // Unknown 
        ['SCB','1','surface:condition','good'], // Good - No cracks or potholes. 
        ['SCB','2','surface:condition','fair'], // Fair - Some cracks or potholes. 
        ['SCB','3','surface:condition','poor'], // Poor - Extensive cracks or potholes. 

        // SCC - Spring/Well Characteristic Category 
        // ['SCC','0','water:type','unknown'], // Unknown 
        ['SCC','0',undefined,undefined], // Unknown 
        ['SCC','1','water:type','alkaline'], // Alkaline 
        ['SCC','10','water:type','salt'], // Salt 
        ['SCC','11','water:type','fresh'], // Fresh 
        ['SCC','2','water:type','not_applicable'], // Not Applicable 
        ['SCC','4','water:type','mineral'], // Mineral 
        ['SCC','9','water:potable','yes'], // Freshwater/Potable 
        ['SCC','999','water:type','other'], // Other 

        // SEC_CLASS - Security Classification 
        // I'm pretty sure we are not going to see 99% of these
        // Moved to Post Processing
//         ['SEC_CLASS','01','security:classification','cosmic_top_secret-bohemia'], // COSMIC Top Secret-Bohemia
//         ['SEC_CLASS','02','security:classification','cosmic_top_secret-balk'], // COSMIC Top Secret-Balk
//         ['SEC_CLASS','03','security:classification','nato_secret'], // NATO Secret
//         ['SEC_CLASS','04','security:classification','nato_secret-savate'], // NATO Secret-Savate
//         ['SEC_CLASS','05','security:classification','nato_secret-avicula'], // NATO Secret-Avicula
//         ['SEC_CLASS','06','security:classification','nato_confidential'], // NATO Confidential
//         ['SEC_CLASS','07','security:classification','nato_restricted'], // NATO Restricted
//         ['SEC_CLASS','08','security:classification','nato_unclassified'], // NATO Unclassified
//         ['SEC_CLASS','09','security:classification','cosmic_top_secret_atom'], // COSMIC Top Secret Atom
//         ['SEC_CLASS','10','security:classification','secret_atomal'], // Secret Atomal
//         ['SEC_CLASS','11','security:classification','confidential_atomal'], // Confidential Atomal
//         ['SEC_CLASS','C','security:classification','C'], // Confidential
//         ['SEC_CLASS','S','security:classification','S'], // Secret
//         ['SEC_CLASS','TS','security:classification','TS'], // Top Secret
//         ['SEC_CLASS','U','security:classification','U'], // Unclassified

        // SEC_CLS_AUTHORITY - Classification Authority 
        ['SEC_CLS_AUTHORITY','D_IMGPOL','security:classification:authority','D_IMGPOL'], // Classification Derived from Imagery Policy Series Classification Guide
        ['SEC_CLS_AUTHORITY','D_MIDB','security:classification:authority','D_MIDB'], // Classification Derived from DoD MIDB Security Classification Guide
        ['SEC_CLS_AUTHORITY','D_NGA','security:classification:authority','D_NGA'], // Classification Derived from NGA Security Classification Guide
        ['SEC_CLS_AUTHORITY','D_OPNAV','security:classification:authority','D_OPNAV'], // Classification Derived from Op Nav Inst S5513.5B-24
        ['SEC_CLS_AUTHORITY','I','security:classification:authority','I'], // Classified by Director Central Intelligence
        ['SEC_CLS_AUTHORITY','M','security:classification:authority','M'], // Classification Derived from Multiple Sources
        ['SEC_CLS_AUTHORITY','O','security:classification:authority','O'], // Classified by some other means

        // SHP - Shape of Beacon 
        ['SHP','0','raw:SHP','undefined'], // Undefined 
        ['SHP','1','raw:SHP','stake/pole'], // Stake/Pole 
        ['SHP','10','raw:SHP','daymark_board_-_rectangle'], // Daymark Board - Rectangle 
        ['SHP','2','raw:SHP','withy'], // Withy 
        ['SHP','3','raw:SHP','beacon_tower'], // Beacon Tower 
        ['SHP','4','raw:SHP','lattice_beacon'], // Lattice Beacon 
        ['SHP','5','raw:SHP','pile_beacon'], // Pile Beacon 
        ['SHP','7','raw:SHP','buoyant_beacon'], // Buoyant Beacon 
        ['SHP','8','raw:SHP','daymark_board_-_square'], // Daymark Board - Square 
        ['SHP','9','raw:SHP','daymark_board_-_triangle'], // Daymark Board - Triangle!

        // SLT - Shoreline Type Category 
        // ['SLT','0','shoreline:type','unknown'], // Unknown 
        ['SLT','0',undefined,undefined], // Unknown 
        ['SLT','6','shoreline:type','mangrove'], // Mangrove/Nipa 
        ['SLT','8','shoreline:type','marshy'], // Marsh Swamp 
        ['SLT','10','shoreline:type','stony'], // Rocky 
        ['SLT','11','shoreline:type','building_rubble'], // Rubble 
        ['SLT','13','shoreline:type','sandy'], // Sandy 
        ['SLT','14','shoreline:type','shingly'], // Stony Shingly 
        ['SLT','999','shoreline:type','other'], // Other 

        // SMC - Surface Material Category 
        // ['SMC','0','surface','unknown'], // Unknown 
        ['SMC','0',undefined,undefined], // Unknown 
        ['SMC','2','surface','aluminum'], // Aluminum 
        ['SMC','5','surface','asphalt'], // Asphalt 
        ['SMC','7','surface','bedrock'], // Bedrock 
        ['SMC','8','surface','boulders'], // Boulders 
        ['SMC','9','surface','brick'], // Brick 
        ['SMC','16','surface','clay'], // Clay 
        ['SMC','20','surface','composition'], // Composition 
        ['SMC','21','surface','concrete'], // Concrete 
        ['SMC','22','surface','conglomerate'], // Conglomerate 
        ['SMC','24','surface','coral'], // Coral 
        ['SMC','30','surface','earth'], // Earthen 
        ['SMC','45','surface','grass'], // Grass/Thatch 
        ['SMC','46','surface','gravel'], // Gravel 
        ['SMC','52','surface','lava'], // Lava 
        ['SMC','55','surface','loess'], // Loess 
        ['SMC','62','surface','masonry_(brick/stone)'], // Masonry (Brick/Stone) 
        ['SMC','64','surface','metal'], // Metal 
        ['SMC','65','surface','mud'], // Mud 
        ['SMC','68','surface','oil_blister'], // Oil Blister 
        ['SMC','72','surface','part_metal'], // Part Metal 
        ['SMC','83','surface','reinforced_concrete'], // Reinforced Concrete 
        ['SMC','84','surface','rock'], // Rock/Rocky 
        ['SMC','88','surface','sand'], // Sand 
        ['SMC','103','surface','snow'], // Snow/Ice 
        ['SMC','104','surface','ground'], // Soil 
        ['SMC','107','surface','steel'], // Steel 
        ['SMC','108','surface','stone'], // Stone 
        ['SMC','116','surface','water'], // Water 
        ['SMC','117','surface','wood'], // Wood 
        ['SMC','119','surface','distorted_surface'], // Distorted Surface 
        ['SMC','120','surface','sand_and_gravel'], // Sand and Gravel 
        ['SMC','122','surface','evaporites'], // Evaporites 
        ['SMC','124','surface','sand_and_boulders'], // Sand and Boulders 
        ['SMC','126','surface','sand_and_mud'], // Sand and Mud 
        ['SMC','127','surface','karst'], // Karst 
        ['SMC','200','surface','herbaceous/scrub_vegetation_(excluding_trees)'], // Herbaceous/Scrub Vegetation (Excluding trees) 
        ['SMC','201','surface','trees'], // Trees 
        ['SMC','250','surface','composite_-_50_%_or_more_of_the_runway_length_is_permanent'], // Composite - 50 % or more of the runway length is  permanent
        ['SMC','251','surface','pem_-_part_concrete_part_asphalt_or_part_bitumen_bound_macadam'], // PEM - part concrete part asphalt or part bitumen  bound macadam
        ['SMC','252','surface','permanent_-_hard_surface_type_unknown'], // Permanent - hard surface type unknown 
        ['SMC','253','surface','bituminous'], // Bituminous - tar or asphalt mixed in place oiled 
        ['SMC','254','surface','composite_soft_-_less_than_50%_of_the_runway_length_is_permanent'], // Composite Soft - less than 50% of the runway length is  permanent
        ['SMC','255','surface','graded_or_rolled_earth_grass_on_graded_earth'], // Graded or rolled earth grass on graded earth 
        ['SMC','256','surface','grass_or_earth_not_graded_or_rolled'], // Grass or earth not graded or rolled 
        ['SMC','257','surface','ice'], // Ice 
        ['SMC','258','surface','snow'], // Snow 
        ['SMC','259','surface','macadam'], // Macadam - crushed rock water bound
        ['SMC','260','surface','membrane_-_plastic_or_other_coated_fiber_material'], // Membrane - plastic or other coated fiber material 
        ['SMC','261','surface','mix_-_mix_in_place_using_non-bituminous_binder_such_as_portland_cement'], // Mix - mix in place using non-bituminous binder such as  portland cement
        ['SMC','262','surface','laterite'], // Laterite 
        ['SMC','263','surface','sand_-_sand_graded_rolled_or_oiled'], // Sand - sand graded rolled or oiled 
        ['SMC','264','surface','combination'], // Combination 
        ['SMC','265','surface','asphalt_over_concrete'], // Asphalt Over Concrete 
        ['SMC','999','surface','other_resolution'], // Other  resolution

        // SPEC_ID - Extraction Specification ID 
        ['SPEC_ID','1AA-TPC','source:extraction_specification','1aa-tpc'], // 1AA-TPC 
        ['SPEC_ID','1AB-ONC','source:extraction_specification','1ab-onc'], // 1AB-ONC 
        ['SPEC_ID','1AE-JOG-A/G','source:extraction_specification','1ae-jog-a/g'], // 1AE-JOG-A/G 
        ['SPEC_ID','1CD-DTED-1','source:extraction_specification','1cd-dted-1'], // 1CD-DTED-1 
        ['SPEC_ID','1CE-DFAD-1','source:extraction_specification','1ce-dfad-1'], // 1CE-DFAD-1 
        ['SPEC_ID','1CF-DTED-2','source:extraction_specification','1cf-dted-2'], // 1CF-DTED-2 
        ['SPEC_ID','1CG-DFAD-2','source:extraction_specification','1cg-dfad-2'], // 1CG-DFAD-2 
        ['SPEC_ID','2AA/001-HAC-1','source:extraction_specification','2aa/001-hac-1'], // 2AA/001-HAC-1 
        ['SPEC_ID','2AA/002-HAC-2','source:extraction_specification','2aa/002-hac-2'], // 2AA/002-HAC-2 
        ['SPEC_ID','2AA/003-HAC-3','source:extraction_specification','2aa/003-hac-3'], // 2AA/003-HAC-3 
        ['SPEC_ID','2AA/004-HAC-4','source:extraction_specification','2aa/004-hac-4'], // 2AA/004-HAC-4 
        ['SPEC_ID','2AA/005-HAC-5','source:extraction_specification','2aa/005-hac-5'], // 2AA/005-HAC-5 
        ['SPEC_ID','2AA/006-HAC-6','source:extraction_specification','2aa/006-hac-6'], // 2AA/006-HAC-6 
        ['SPEC_ID','2AA/007-HAC-7','source:extraction_specification','2aa/007-hac-7'], // 2AA/007-HAC-7 
        ['SPEC_ID','2AA/008-HAC-8','source:extraction_specification','2aa/008-hac-8'], // 2AA/008-HAC-8 
        ['SPEC_ID','2AA/009-HAC-9','source:extraction_specification','2aa/009-hac-9'], // 2AA/009-HAC-9 
        ['SPEC_ID','2AD-Combat','source:extraction_specification','2ad-combat'], // 2AD-Combat 
        ['SPEC_ID','3AA-TLM50','source:extraction_specification','3aa-tlm50'], // 3AA-TLM50 
        ['SPEC_ID','3AG-TLM100','source:extraction_specification','3ag-tlm100'], // 3AG-TLM100 
        ['SPEC_ID','3KA-VITD','source:extraction_specification','3ka-vitd'], // 3KA-VITD 
        ['SPEC_ID','3KC/001-DTOP','source:extraction_specification','3kc/001-dtop'], // 3KC/001-DTOP 
        ['SPEC_ID','3KH-VMap-2','source:extraction_specification','3kh-vmap-2'], // 3KH-VMap-2 
        ['SPEC_ID','3KL-VMap-0','source:extraction_specification','3kl-vmap-0'], // 3KL-VMap-0 
        ['SPEC_ID','3KM-VMap-1','source:extraction_specification','3km-vmap-1'], // 3KM-VMap-1 
        ['SPEC_ID','3KU-UVMap','source:extraction_specification','3ku-uvmap'], // 3KU-UVMap 
        ['SPEC_ID','4AA-ATC','source:extraction_specification','4aa-atc'], // 4AA-ATC 
        ['SPEC_ID','4AC-JOG-R','source:extraction_specification','4ac-jog-r'], // 4AC-JOG-R 
        ['SPEC_ID','4GE-TERCOM-L','source:extraction_specification','4ge-tercom-l'], // 4GE-TERCOM-L 
        ['SPEC_ID','4GF-TERCOM-E','source:extraction_specification','4gf-tercom-e'], // 4GF-TERCOM-E 
        ['SPEC_ID','4GG-TERCOM-T','source:extraction_specification','4gg-tercom-t'], // 4GG-TERCOM-T 
        ['SPEC_ID','5EE-FFD','source:extraction_specification','5ee-ffd'], // 5EE-FFD 
        ['SPEC_ID','7BC/302-BATHY','source:extraction_specification','7bc/302-bathy'], // 7BC/302-BATHY 
        ['SPEC_ID','7BC/502-AERO','source:extraction_specification','7bc/502-aero'], // 7BC/502-AERO 
        ['SPEC_ID','7SC/001-MPO-1','source:extraction_specification','7sc/001-mpo-1'], // 7SC/001-MPO-1 
        ['SPEC_ID','7SC/002-MPO-2','source:extraction_specification','7sc/002-mpo-2'], // 7SC/002-MPO-2 
        ['SPEC_ID','7SC/005-MPO-5','source:extraction_specification','7sc/005-mpo-5'], // 7SC/005-MPO-5 
        ['SPEC_ID','7SC/006-MPO-6','source:extraction_specification','7sc/006-mpo-6'], // 7SC/006-MPO-6 
        ['SPEC_ID','7SC/008-MPO-8','source:extraction_specification','7sc/008-mpo-8'], // 7SC/008-MPO-8 
        ['SPEC_ID','7SC/009-MPO-9','source:extraction_specification','7sc/009-mpo-9'], // 7SC/009-MPO-9 
        ['SPEC_ID','7SC/010-MPO-10','source:extraction_specification','7sc/010-mpo-10'], // 7SC/010-MPO-10 
        ['SPEC_ID','7SC/011-MPO-11','source:extraction_specification','7sc/011-mpo-11'], // 7SC/011-MPO-11 
        ['SPEC_ID','DNC','source:extraction_specification','dnc'], // DNC 
        ['SPEC_ID','Mission Specific Data 1 (MSD1)','source:extraction_specification','mission_specific_data_1_(msd1)'], // Mission Specific Data 1 (MSD1) 
        ['SPEC_ID','Mission Specific Data 2 (MSD2)','source:extraction_specification','mission_specific_data_2_(msd2)'], // Mission Specific Data 2 (MSD2) 
        ['SPEC_ID','Mission Specific Data 3 (MSD3)','source:extraction_specification','mission_specific_data_3_(msd3)'], // Mission Specific Data 3 (MSD3) 
        ['SPEC_ID','Mission Specific Data 4 (MSD4)','source:extraction_specification','mission_specific_data_4_(msd4)'], // Mission Specific Data 4 (MSD4) 
        ['SPEC_ID','Mission Specific Data 5 (MSD5)','source:extraction_specification','mission_specific_data_5_(msd5)'], // Mission Specific Data 5 (MSD5) 
        ['SPEC_ID','Other','source:extraction_specification','other'], // Other 
        ['SPEC_ID','UUU-DBCS','source:extraction_specification','uuu-dbcs'], // UUU-DBCS 

        // SSC - Structure Shape Category 
        // ['SSC','0','shape','unknown'], // Unknown 
        ['SSC','0',undefined,undefined], // Unknown 
        ['SSC','2','shape','blimp'], // Blimp 
        ['SSC','4','shape','bullet'], // Bullet 
        ['SSC','7','shape','cylindrical_(upright)/can'], // Cylindrical (Upright)/CAN 
        ['SSC','12','shape','pyramidal'], // Pyramid 
        ['SSC','15','shape','solid/filled'], // Solid/filled 
        ['SSC','17','shape','spherical'], // Spherical (Hemispherical) 
        ['SSC','18','shape','truss'], // Truss 
        ['SSC','21','shape','artificial_mountain'], // Artificial Mountain 
        ['SSC','22','shape','crescent'], // Crescent 
        ['SSC','23','shape','ferris_wheel'], // Ferris Wheel 
        ['SSC','24','shape','enclosed'], // Enclosed 
        ['SSC','25','shape','roller_coaster'], // Roller Coaster 
        ['SSC','26','shape','lateral'], // Lateral 
        ['SSC','27','shape','mounds'], // Mounds 
        ['SSC','28','shape','ripple'], // Ripple 
        ['SSC','29','shape','star'], // Star 
        ['SSC','30','shape','transverse'], // Transverse 
        ['SSC','46','shape','open'], // Open 
        ['SSC','52','shape','a_frame'], // A Frame 
        ['SSC','53','shape','h_frame'], // H Frame 
        ['SSC','54','shape','i_frame'], // I Frame 
        ['SSC','56','shape','y_frame'], // Y Frame 
        ['SSC','59','shape','cylindrical_with_framework'], // Telescoping Gasholder (Gasometer) 
        ['SSC','60','shape','mast'], // Mast 
        ['SSC','65','shape','cylindrical_with_flat_top'], // Cylindrical with flat top 
        ['SSC','66','shape','cylindrical_with_domed_top'], // Cylindrical with domed top 
        ['SSC','71','shape','cylindrical_with_conical_top'], // Cylindrical/Peaked 
        ['SSC','74','shape','t_frame'], // T Frame 
        ['SSC','77','shape','arched'], // Arch 
        ['SSC','79','shape','round'], // Round 
        ['SSC','80','shape','rectangular_prism'], // Rectangular 
        ['SSC','83','shape','square_prism'], // Square 
        ['SSC','84','shape','irregular'], // Irregular 
        ['SSC','87','shape','domed'], // Dome 
        ['SSC','88','shape','spherical_on_column'], // Spherical with Column Support 
        ['SSC','107','shape','tower'], // Tower 
        ['SSC','108','shape','scanner'], // Scanner 
        ['SSC','109','shape','obelisk'], // Obelisk 
        ['SSC','110','shape','radome_tower_mounted'], // Radome Tower Mounted 
        ['SSC','999','shape','other'], // Other

        // SSR - Structure Shape of Roof
        ['SSR','0',undefined,undefined],
        ['SSR','Unknown',undefined,undefined],
        ['SSR','6','building:roof_shape','conical'],
        ['SSR','Conical/Peaked/NUN','building:roof_shape','conical'],
        ['SSR','38','building:roof_shape','semi-cylindrical'],
        ['SSR','Curved/Round (Quonset)','building:roof_shape','semi-cylindrical'],
        ['SSR','40','building:roof_shape','domed'],
        ['SSR','Dome','building:roof_shape','domed'],
        ['SSR','41','building:roof_shape','flat'],
        ['SSR','Flat','building:roof_shape','flat'],
        ['SSR','42','building:roof_shape','pitched'],
        ['SSR','Gable (Pitched)','building:roof_shape','pitched'],
        ['SSR','47','building:roof_shape','sawtoothed'],
        ['SSR','Sawtooth','building:roof_shape','sawtoothed'],
        ['SSR','50','building:roof_shape','with_clerestory'],
        ['SSR','With Monitor','building:roof_shape','with_clerestory'],
        ['SSR','51','building:roof_shape','with_steeple'],
        ['SSR','With Steeple','building:roof_shape','with_steeple'],
        ['SSR','55','building:roof_shape','flat_with_clerestory'],
        ['SSR','Flat with Monitor','building:roof_shape','flat_with_clerestory'],
        ['SSR','64','building:roof_shape','pitched_with_clerestory'],
        ['SSR','Gable with Monitor','building:roof_shape','pitched_with_clerestory'],
        ['SSR','77','building:roof_shape','with_cupola'],
        ['SSR','With Cupola','building:roof_shape','with_cupola'],
        ['SSR','78','building:roof_shape','with_turret'],
        ['SSR','79','building:roof_shape','with_tower'],
        ['SSR','80','building:roof_shape','with_minaret'],
        ['SSR','With Minaret','building:roof_shape','with_minaret'],
        ['SSR','999','building:roof_shape','other'],
        ['SSR','Other','building:roof_shape','other'],

        // SWC - Soil Wetness Condition 
        // ['SWC','0','raw:SWC','unknown'], // Unknown 
        ['SWC','0',undefined,undefined], // Unknown 
        ['SWC','1','soil:wetness','normally_dry'], // Dry
        ['SWC','2','soil:wetness','normally_moist'], // Moist
        ['SWC','3','soil:wetness','normally_wet'], // Wet
        ['SWC','4','soil:wetness','normally_frozen'], // Frozen/Permafrost
        ['SWC','999','soil:wetness','other'], // Other

        // SWT - Well/Spring Feature Type 
        // ['SWT','0','raw:SWT','unknown'], // Unknown 
        ['SWT','0',undefined,undefined], // Unknown 
        ['SWT','1','natural','geyser'], // Geyser 
        ['SWT','2','natural','hot_spring'], // Hot Spring 
        ['SWT','3','natural','fumarole'], // Fumarole 
        ['SWT','4','natural','artesian'], // Artesian 
        ['SWT','5','natural','water-hole'], // Water-hole 
        ['SWT','6','natural','walled-in_spring'], // Walled-In Spring 

        // TID - Tide Influenced
        // ['TID','0','tidal','unknown'],
        ['TID','0',undefined,undefined],
        ['TID','1','tidal','no'], // Non-Tidal 
        ['TID','2','tidal','yes'], // Tidal/Tidal Fluctuating 

        // TRA - Traversability 
        // ['TRA','0','foot','unknown'], // Unknown
        ['TRA','0',undefined,undefined], // Unknown 
        ['TRA','1','foot','yes'], // Traversable
        ['TRA','2','foot','no'], // Non-Traversable

        // TRE - Tree Type Category 
        // ['TRE','0','wood','unknown'], // Unknown 
        ['TRE','0',undefined,undefined], // Unknown 
        ['TRE','1','wood','deciduous'], // Deciduous 
        ['TRE','2','wood','evergreen'], // Evergreen 
        ['TRE','3','wood','mixed'], // Mixed 
        ['TRE','997','wood','unpopulated'], // Unpopulated 

        // TRF - Traffic Flow
        ['TRF','0',undefined,undefined],
        ['TRF','1','oneway','yes'], 
        ['TRF','One-way','oneway','yes'], 
        ['TRF','2',undefined,undefined], // Two way
        ['TRF','Two-way',undefined,undefined], // Two way
        ['TRF','3','oneway','yes'], // In DFDD & data but not in spec ??
        ['TRF','4',undefined,undefined], // Two way - In DFDD & data but not in spec??

        // TST - Transmission Suspension Type 
        // ['TST','0','cable:shape','unknown'], // Unknown 
        ['TST','0',undefined,undefined], // Unknown 
        ['TST','1','cable:shape','symmetric_catenary'], // Normal Suspension 
        ['TST','2','cable:shape','mountain_catenary'], // Catenary (Over Mountains) 
        ['TST','3','cable:shape','overwater_catenary'], // Catenary (Over Water) 
        ['TST','998','cable:shape','not_applicable'], // Not Applicable 
        ['TST','999','cable:shape','other'], // Other 

        // TTC - Tower Type Category 
        // ['TTC','0','tower:type','unknown'], // Unknown 
        ['TTC','0',undefined,undefined], // Unknown 
        ['TTC','12','tower:type','fire'], // Fire 
        ['TTC','2','tower:type','observation'], // Observation/Lookout 
        ['TTC','4','tower:type','undefined'], // Undefined 
        ['TTC','999','tower:type','other'], // Other 

        // TUC - Transportation Use Category.Hardcoded to data, needs fixing
        // ['TUC','0','transport:type','unknown'], // Unknown
        ['TUC','0',undefined,undefined], // Unknown
        // ['TUC','Unknown','transport:type','unknown'], // Unknown
        ['TUC','Unknown',undefined,undefined], // Unknown
        ['TUC','1','transport:type','road_and_railway'], // Both Road and Railway
        ['TUC','3','transport:type','railway'], // Railroad
        ['TUC','4','transport:type','road'], // Road
        ['TUC','Road','transport:type','road'], // Road
        ['TUC','6','transport:type','road'], // Street
        ['TUC','Street','transport:type','road'], // Street
        ['TUC','7','highway:through_route','yes'],  // Through Route
        ['TUC','Through Route','highway:through_route','yes'], // Through Route
        ['TUC','12','transport:type','marine'], // Marine
        ['TUC','13','transport:type','air'], // Air 
        ['TUC','14','transport:type','bus'], // Bus 
        ['TUC','17','transport:type','pedestrian'], // Pedestrian
        ['TUC','18','transport:type','pipeline'], // Pipeline 
        ['TUC','20','transport:type','aircraft'], // Aircraft 
        ['TUC','21','transport:type','ship'], // Ship 
        ['TUC','22','transport:type','automotive'], // Automotive 
        ['TUC','31','transport:type','motor_cycle'], // Motor Cycle 
        ['TUC','34','transport:type','waterway'], // Waterway 
        ['TUC','35','transport:type','no_transport_use'], // No Transport Use 
        ['TUC','36','transport:type','slip_road/access_road'], // Slip Road/Access Road 
        ['TUC','37','transport:type','portage'], // Portage 
        ['TUC','38','transport:type','canal'], // Canal 
        ['TUC','39','transport:type','caravan_route'], // Caravan Route 
        ['TUC','40','transport:type','subway'], // Subway 
        ['TUC','41','transport:type','aqueduct'], // Aqueduct 
        ['TUC','42','transport:type','both_road_and_runway'], // Both Road and Runway 
        ['TUC','43','transport:type','recreational_vehicles'], // Recreational Vehicles 
        ['TUC','997',undefined,undefined], // Unpopulated 
        ['TUC','999','transport:type','other'], // Other 

        // UPDATE_SOURCE - Update/Review Source Name 
        ['UPDATE_SOURCE','Arc Digitized Raster Graphic (ADRG)','source:review_source:type','arc_digitized_raster_graphic_(adrg)'], // Arc Digitized Raster Graphic (ADRG) 
        ['UPDATE_SOURCE','Automated Aeronautical Facilities Information File (AAFIF)','source:review_source:type','automated_aeronautical_facilities_information_file_(aafif)'], //  Automated Aeronautical Facilities Information File (AAFIF) 
        ['UPDATE_SOURCE','Boundaries (International Boundaries Database)','source:review_source:type','boundaries_(international_boundaries_database)'], // Boundaries (International Boundaries Database) 
        ['UPDATE_SOURCE','CHUM','source:review_source:type','chum'], // CHUM 
        ['UPDATE_SOURCE','City Graphic','source:review_source:type','city_graphic'], // City Graphic 
        ['UPDATE_SOURCE','Combat Chart','source:review_source:type','combat_chart'], // Combat Chart 
        ['UPDATE_SOURCE','Compressed Arc Digitized Raster Graphic (CADRG)','source:review_source:type','compressed_arc_digitized_raster_graphic_(cadrg)'], // Compressed Arc Digitized Raster Graphic (CADRG) 
        ['UPDATE_SOURCE','Controlled Imagery Base 1 (CIB1)','source:review_source:type','controlled_imagery_base_1_(cib1)'], // Controlled Imagery Base 1 (CIB1) 
        ['UPDATE_SOURCE','Controlled Imagery Base 5 (CIB5)','source:review_source:type','controlled_imagery_base_5_(cib5)'], // Controlled Imagery Base 5 (CIB5) 
        ['UPDATE_SOURCE','Digital Globe','source:review_source:type','digital_globe'], // Digital Globe 
        ['UPDATE_SOURCE','Digital Nautical Chart (DNC)','source:review_source:type','digital_nautical_chart_(dnc)'], // Digital Nautical Chart (DNC) 
        ['UPDATE_SOURCE','Digital Print File (DPF)','source:review_source:type','digital_print_file_(dpf)'], // Digital Print File (DPF) 
        ['UPDATE_SOURCE','Digital Terrain Elevation Data 1 (DTED1)','source:review_source:type','digital_terrain_elevation_data_1_(dted1)'], // Digital Terrain Elevation Data 1 (DTED1) 
        ['UPDATE_SOURCE','Digital Terrain Elevation Data 2 (DTED2)','source:review_source:type','digital_terrain_elevation_data_2_(dted2)'], // Digital Terrain Elevation Data 2 (DTED2) 
        ['UPDATE_SOURCE','Digital Terrain Elevation Data 3 (DTED3)','source:review_source:type','digital_terrain_elevation_data_3_(dted3)'], // Digital Terrain Elevation Data 3 (DTED3) 
        ['UPDATE_SOURCE','Digital Terrain Elevation Data 4 (DTED4)','source:review_source:type','digital_terrain_elevation_data_4_(dted4)'], // Digital Terrain Elevation Data 4 (DTED4) 
        ['UPDATE_SOURCE','Digital Topographic Data 1 (DTOP1)','source:review_source:type','digital_topographic_data_1_(dtop1)'], // Digital Topographic Data 1 (DTOP1) 
        ['UPDATE_SOURCE','Digital Topographic Data 2 (DTOP2)','source:review_source:type','digital_topographic_data_2_(dtop2)'], // Digital Topographic Data 2 (DTOP2) 
        ['UPDATE_SOURCE','Digital Topographic Data 3 (DTOP3)','source:review_source:type','digital_topographic_data_3_(dtop3)'], // Digital Topographic Data 3 (DTOP3) 
        ['UPDATE_SOURCE','Digital Topographic Data 4 (DTOP4)','source:review_source:type','digital_topographic_data_4_(dtop4)'], // Digital Topographic Data 4 (DTOP4) 
        ['UPDATE_SOURCE','Digital Topographic Data 5 (DTOP5)','source:review_source:type','digital_topographic_data_5_(dtop5)'], // Digital Topographic Data 5 (DTOP5) 
        ['UPDATE_SOURCE','Digital Vertical Obstruction File (DVOF)','source:review_source:type','digital_vertical_obstruction_file_(dvof)'], // Digital Vertical Obstruction File (DVOF) 
        ['UPDATE_SOURCE','Foundation Feature Data (FFD)','source:review_source:type','foundation_feature_data_(ffd)'], // Foundation Feature Data (FFD) 
        ['UPDATE_SOURCE','Foundation Feature Data/Relocatable Target Data (FFD/RTAD)','source:review_source:type','foundation_feature_data/relocatable_target_data_(ffd/rtad)'], //  Foundation Feature Data/Relocatable Target Data (FFD/RTAD) 
        ['UPDATE_SOURCE','GPS Field Collected Open Source','source:review_source:type','gps_field_collected_open_source'], // GPS Field Collected Open Source 
        ['UPDATE_SOURCE','GeoCover/LandCover','source:review_source:type','geocover/landcover'], // GeoCover/LandCover 
        ['UPDATE_SOURCE','GeoNames','source:review_source:type','geonames'], // GeoNames 
        ['UPDATE_SOURCE','Image City Map (ICM)','source:review_source:type','image_city_map_(icm)'], // Image City Map (ICM) 
        ['UPDATE_SOURCE','Imagery (IKONOS)','source:review_source:type','imagery_(ikonos)'], // Imagery (IKONOS) 
        ['UPDATE_SOURCE','Imagery (NTM)','source:review_source:type','imagery_(ntm)'], // Imagery (NTM) 
        ['UPDATE_SOURCE','Imagery (Other)','source:review_source:type','imagery_(other)'], // Imagery (Other) 
        ['UPDATE_SOURCE','Interim Terrain Data (ITD)','source:review_source:type','interim_terrain_data_(itd)'], // Interim Terrain Data (ITD) 
        ['UPDATE_SOURCE','Interim Vector Data (IVD)','source:review_source:type','interim_vector_data_(ivd)'], // Interim Vector Data (IVD) 
        ['UPDATE_SOURCE','Joint Operational Graphic (JOG)','source:review_source:type','joint_operational_graphic_(jog)'], // Joint Operational Graphic (JOG) 
        ['UPDATE_SOURCE','Landsat','source:review_source:type','landsat'], // Landsat 
        ['UPDATE_SOURCE','Littoral Warfare Data (LWD)','source:review_source:type','littoral_warfare_data_(lwd)'], // Littoral Warfare Data (LWD) 
        ['UPDATE_SOURCE','Mapping Charting and Geodetic (MC&G)','source:review_source:type','mapping_charting_and_geodetic_(mc&g)'], // Mapping Charting and Geodetic (MC&G) 
        ['UPDATE_SOURCE','Mission Specific Data 1 (MSD1)','source:review_source:type','mission_specific_data_1_(msd1)'], // Mission Specific Data 1 (MSD1) 
        ['UPDATE_SOURCE','Mission Specific Data 2 (MSD2)','source:review_source:type','mission_specific_data_2_(msd2)'], // Mission Specific Data 2 (MSD2) 
        ['UPDATE_SOURCE','Mission Specific Data 3 (MSD3)','source:review_source:type','mission_specific_data_3_(msd3)'], // Mission Specific Data 3 (MSD3) 
        ['UPDATE_SOURCE','Mission Specific Data 4 (MSD4)','source:review_source:type','mission_specific_data_4_(msd4)'], // Mission Specific Data 4 (MSD4) 
        ['UPDATE_SOURCE','Mission Specific Data 5 (MSD5)','source:review_source:type','mission_specific_data_5_(msd5)'], // Mission Specific Data 5 (MSD5) 
        ['UPDATE_SOURCE','Modernized Integrated Data Base (MIDB)','source:review_source:type','modernized_integrated_data_base_(midb)'], // Modernized Integrated Data Base (MIDB) 
        ['UPDATE_SOURCE','Multiple','source:review_source:type','multiple'], // Multiple 
        ['UPDATE_SOURCE','NIMA GIS Medical Facilities Database','source:review_source:type','nima_gis_medical_facilities_database'], // NIMA GIS Medical Facilities Database 
        ['UPDATE_SOURCE','Native Data Source','source:review_source:type','native_data_source'], // Native Data Source 
        ['UPDATE_SOURCE','Native Map Source','source:review_source:type','native_map_source'], // Native Map Source 
        ['UPDATE_SOURCE','Nominally Attributed Topographic Evaluation Map (NATE Map)','source:review_source:type','nominally_attributed_topographic_evaluation_map_(nate_map)'], //  Nominally Attributed Topographic Evaluation Map (NATE Map) 
        ['UPDATE_SOURCE','Other','source:review_source:type','other'], // Other 
        ['UPDATE_SOURCE','Planning Graphic','source:review_source:type','planning_graphic'], // Planning Graphic 
        ['UPDATE_SOURCE','Reference Map','source:review_source:type','reference_map'], // Reference Map 
        ['UPDATE_SOURCE','Shuttle Radar Topographic Mission (SRTM)','source:review_source:type','shuttle_radar_topographic_mission_(srtm)'], // Shuttle Radar Topographic Mission (SRTM) 
        ['UPDATE_SOURCE','Site Map/Nuclear Facility','source:review_source:type','site_map/nuclear_facility'], // Site Map/Nuclear Facility 
        ['UPDATE_SOURCE','Tactical Oceanographic Data Lv 0 (TOD0)','source:review_source:type','tactical_oceanographic_data_lv_0_(tod0)'], // Tactical Oceanographic Data Lv 0 (TOD0) 
        ['UPDATE_SOURCE','Tactical Oceanographic Data Lv 1 (TOD1)','source:review_source:type','tactical_oceanographic_data_lv_1_(tod1)'], // Tactical Oceanographic Data Lv 1 (TOD1) 
        ['UPDATE_SOURCE','Tactical Oceanographic Data Lv 2 (TOD2)','source:review_source:type','tactical_oceanographic_data_lv_2_(tod2)'], // Tactical Oceanographic Data Lv 2 (TOD2) 
        ['UPDATE_SOURCE','Tactical Oceanographic Data Lv 4 (TOD4)','source:review_source:type','tactical_oceanographic_data_lv_4_(tod4)'], // Tactical Oceanographic Data Lv 4 (TOD4) 
        ['UPDATE_SOURCE','Tactical Oceanographic Data Lv 5 (TOD5)','source:review_source:type','tactical_oceanographic_data_lv_5_(tod5)'], // Tactical Oceanographic Data Lv 5 (TOD5) 
        ['UPDATE_SOURCE','Topographic Line Map (TLM)','source:review_source:type','topographic_line_map_(tlm)'], // Topographic Line Map (TLM) 
        ['UPDATE_SOURCE','United Nations Data','source:review_source:type','united_nations_data'], // United Nations Data 
        ['UPDATE_SOURCE','Unknown','source:review_source:type','unknown'], // Unknown 
        ['UPDATE_SOURCE','Urban Vector Map (UVMap)','source:review_source:type','urban_vector_map_(uvmap)'], // Urban Vector Map (UVMap) 
        ['UPDATE_SOURCE','Vector Interim Terrain Data (VITD)','source:review_source:type','vector_interim_terrain_data_(vitd)'], // Vector Interim Terrain Data (VITD) 
        ['UPDATE_SOURCE','Vector Map Lv0 (VMap0)','source:review_source:type','vector_map_lv0_(vmap0)'], // Vector Map Lv0 (VMap0) 
        ['UPDATE_SOURCE','Vector Map Lv1 (VMap1)','source:review_source:type','vector_map_lv1_(vmap1)'], // Vector Map Lv1 (VMap1) 
        ['UPDATE_SOURCE','Vector Map Lv2 (VMap2)','source:review_source:type','vector_map_lv2_(vmap2)'], // Vector Map Lv2 (VMap2) 
        ['UPDATE_SOURCE','World Vector Shoreline Plus (WVSPlus)','source:review_source:type','world_vector_shoreline_plus_(wvsplus)'], // World Vector Shoreline Plus (WVSPlus)

        // USE_ - Usage2 - WTF???
        // ['USE_','0','raw:USE_','unknown'], // Unknown 
        ['USE_','0',undefined,undefined], // Unknown 
        ['USE_','155','raw:USE_','prohibited_area'], // Prohibited Area 
        ['USE_','158','raw:USE_','reserved'], // Reserved 
        ['USE_','20','raw:USE_','closed'], // Closed 
        ['USE_','21','raw:USE_','restricted'], // Restricted 
        ['USE_','22','raw:USE_','joint_military/civilian'], // Joint Military/Civilian 
        ['USE_','23','raw:USE_','international'], // International 
        ['USE_','4','raw:USE_','national'], // National 
        ['USE_','5','raw:USE_','state'], // State 
        ['USE_','50','raw:USE_','limited'], // Limited 
        ['USE_','6','raw:USE_','private'], // Private 
        ['USE_','8','raw:USE_','military'], // Military 
        ['USE_','999','raw:USE_','other'], // Other 

        // USG - Usage: This is just Ugly. 
        // It is a combination of primary user, function, or controlling authority
        // ['USG','0','controlling_authority','unknown'],
        ['USG','0',undefined,undefined],
        // ['USG','Unknown','controlling_authority','unknown'],
        ['USG','Unknown',undefined,undefined],
        ['USG','4','controlling_authority','national'],
        ['USG','National','controlling_authority','national'],
        ['USG','5','controlling_authority','state'],
        ['USG','State','controlling_authority','state'],
        ['USG','6','controlling_authority','private'],
        ['USG','7','controlling_authority','tribal'],
        ['USG','8','use','military'],
        ['USG','Military','use','military'],
        ['USG','11','use','motel/hotel'],
        ['USG','Motel/Hotel','use','motel/hotel'],
        ['USG','12','use','apartment'], // Apartment 
        ['USG','13','use','open'], // Open 
        ['USG','16','controlling_authority','city'],             
        ['USG','17','controlling_authority','public'],
        ['USG','20','use','closed'], // Closed 
        ['USG','21','use','restricted'], // Restricted 
        ['USG','22','use','joint_military/civilian'], // Joint Military/Civilian 
        ['USG','23','use','international'],             
        ['USG','International','use','international'],             
        ['USG','25','controlling_authority','federal'], // Federal 
        ['USG','26','use','primary/1st_order'], // Primary/1st Order 
        ['USG','30','use','secondary/2nd_order'], // Secondary/2nd Order 
        ['USG','31','use','tertiary/3rd_order'], // Tertiary/3rd Order 
        ['USG','32','use','insular'], // Insular 
        ['USG','33','use','provincial'], // Provincial 
        ['USG','37','use','interstate'], // Interstate 
        ['USG','41','use','industrial'],             
        ['USG','Industrial','use','industrial'],             
        ['USG','42','use','commercial'], // Commercial 
        ['USG','Commercial','use','commercial'],             
        ['USG','43','use','institute'], // Institutional 
        ['USG','44','use','residential'], // Residential 
        ['USG','Residential','use','residential'],             
        ['USG','45','use','agriculture'],             
        ['USG','Agricultural','use','agriculture'],             
        ['USG','48','use','decoy'], // Decoy 
        ['USG','49','use','civilian/public'], // Civilian/Public 
        ['USG','Civilian/Public','use','civilian/public'],             
        ['USG','50','use','limited'], // Limited 
        ['USG','69','use','levee/dike'], // Levee/Dike 
        ['USG','70','use','reserve/reservation'], // Reserve/Reservation 
        ['USG','95','use','alert_apron/hardstand'], // Alert Apron/Hardstand 
        ['USG','96','use','operational_apron/hardstand'], // Operational Apron/Hardstand 
        ['USG','97','use','hangar/apron'], // Hangar/Apron 
        ['USG','98','use','base_flight_apron'], // Base Flight Apron 
        ['USG','99','use','engine_test_pad/apron'], // Engine Test Pad/Apron 
        ['USG','100','use','transient_apron'], // Transient Apron 
        ['USG','101','use','depot_apron'], // Depot Apron 
        ['USG','102','use','stub_apron'], // Stub Apron 
        ['USG','103','use','dispersal_hardstand'], // Dispersal Hardstand 
        ['USG','104','use','pad_hardstand'], // Pad Hardstand 
        ['USG','105','use','refueling_hardstand'], // Refueling Hardstand 
        ['USG','106','use','parking_hardstand'], // Parking Hardstand 
        ['USG','107','use','engine_run-up_hardstand'], // Engine Run-up Hardstand 
        ['USG','108','use','firing-in_hardstand'], // Firing-In Hardstand 
        ['USG','109','use','compass_rose_hardstand'], // Compass Rose Hardstand 
        ['USG','110','use','maintenance_hardstand'], // Maintenance Hardstand 
        ['USG','113','controlling_authority','regional'], // Regional 
        ['USG','115','man_made','snow_fence'], // Snow Shed 
        ['USG','116','man_made','rock_shed'], // Rock Shed 
        ['USG','120','use','recreation'], // Recreational 
        ['USG','Recreational','use','recreation'],             
        ['USG','128','use','mixed_urban_or_built-up_land'], // Mixed Urban or built-up Land 
        ['USG','130','use','transportation'],             
        ['USG','Transportation','use','transportation'],             
        ['USG','134','use','utilities_and_communication'], // Utilities and Communication 
        ['USG','136','use','as_a_fill'], // As a Fill 
        ['USG','140','use','medical'],             
        ['USG','Medical','use','medical'],             
        ['USG','141','use','forest_preserve'], // Forest Preserve 
        ['USG','155','use','prohibited_area'], // Prohibited Area 
        ['USG','158','use','reserved'], // Reserved 
        ['USG','161','use','roll-on_roll-off_berth_(roro)'], // Roll-On Roll-Off Berth (RORO) 
        ['USG','998',undefined,undefined],
        ['USG','990',undefined,undefined],
        ['USG','999',undefined,undefined],
        ['USG','Other',undefined,undefined], // Other 

        // VCA - Void Collection Attribute 
        // ['VCA','0','raw:VCA','unknown'], // Unknown 
        ['VCA','0',undefined,undefined], // Unknown 
        ['VCA','1','void_collection:reason','not_requested'], // Data Not Requested By User
        ['VCA','2','void_collection:reason','inaccessible'], // Area Too Rough to Collect
        ['VCA','3','void_collection:reason','no_available_imagery'], // No Available Imagery
        ['VCA','4','void_collection:reason','different_height_threshold_within_data_block'], // Different Height Threshold Within Data Block
        ['VCA','5','void_collection:reason','low_data_collection_criteria'], // Low Data Collection Criteria
        ['VCA','6','void_collection:reason','no_available_map_source'], // No Available Map Source
        ['VCA','7','void_collection:reason','no_suitable_imagery'], // No Suitable Imagery
        ['VCA','8','void_collection:reason','not_required'], // Data Not Required

        // VCT - Void Collection Type 
        // ['VCT','0','void_collection:type','unknown'], // Unknown 
        ['VCT','0',undefined,undefined], // Unknown 
        ['VCT','1','void_collection:type','hypsography'], // Relief 
        ['VCT','999','void_collection:type','other'], // Other 

        // VDC - Vertical Datum Category 
        // ['VDC','0','vertical_datum','unknown'], // Unknown
        ['VDC','0',undefined,undefined], // Unknown 
        ['VDC','7','source:datum:sounding','mean_high_water'], // Mean High Water
        ['VDC','9','source:datum:sounding','mean_high_water_springs'], // Mean High Water Springs
        ['VDC','10','source:datum:sounding','mean_higher_high_water'], // Mean Higher High Water
        ['VDC','11','source:datum:sounding','mean_low_water'], // Mean Low Water
        ['VDC','13','source:datum:sounding','mean_low_water_springs'], // Mean Low Water Springs
        ['VDC','14','source:datum:sounding','mean_lower_low_water'], // Mean Lower Low Water
        ['VDC','15','source:datum:sounding','mean_sea_level'], // Mean Sea Level
        ['VDC','19','source:datum:sounding','mean_lower_low_water_springs'], // Mean Lower Low Water Springs
        ['VDC','24','source:datum:sounding','mean_higher_high_water_springs'], // Mean Higher High Water Springs
        ['VDC','26','source:datum:sounding','highest_normal_high_water'], // Highest Normal High Water
        ['VDC','27','source:datum:sounding','lowest_normal_low_water'], // Lowest Normal Low Water
        ['VDC','28','source:datum:sounding','highest_high_water'], // Highest High Water
        ['VDC','30','source:datum:sounding','indian_spring_high_water!'], // Indian Spring High Water!
        ['VDC','90','source:datum:sounding','lowest_low_water'], // Lowest Low Water
        ['VDC','998','source:datum:sounding','not_applicable'], // Not Applicable
        ['VDC','999','source:datum:sounding','other'], // Other

        // VEG - Vegetation Characteristics 
        // ['VEG','0','vegetation:type','unknown'], // Unknown 
        ['VEG','0',undefined,undefined], // Unknown 
        ['VEG','1','crop','dry_crop'], // Dry Crops 
        ['VEG','4','crop','rice'], // Rice Paddies 
        ['VEG','5','farm:pattern','intermingled_woods'], // Agriculture with scattered forests or rows of trees 
        ['VEG','6','bog:type','cranberry'], // Cranberry 
        ['VEG','7','bog:type','peat'], // Peat 
        ['VEG','8','grassland:type','pasture_meadow_steppe'], // Pasture meadow steppe 
        ['VEG','9','grassland:type','grassland_with_trees'], // Grassland with scattered trees 
        ['VEG','11','vegetation:type','casuarina'], // Casuarina 
        ['VEG','12','vegetation:type','coniferous'], // Coniferous 
        ['VEG','16','vegetation:type','nipa'], // Nipa Palm 
        ['VEG','17','vegetation:type','palm'], // Palm 
        ['VEG','18','vegetation:type','filao'], // Filao 
        ['VEG','19','vegetation:type','mangrove'], // Mangrove 
        ['VEG','24','wood','deciduous'], // Deciduous  - Really??? Same as TRE
        ['VEG','25','wood','evergreen'], // Evergreen 
        ['VEG','29','vegetation:type','eucalyptus'], // Eucalyptus 
        ['VEG','38','vegetation:type','cypress'], // Cypress 
        ['VEG','50','wood','mixed'], // Mixed Trees 
        ['VEG','52','vegetation:type','forest_clearing'], // Forest Clearing 
        ['VEG','56','grassland:type','without_trees'], // Without Trees 
        ['VEG','57','farm:pattern','intermingled_trees'], // Agriculture with scattered trees or rows of trees 
        ['VEG','997',undefined,undefined], // Unpopulated 
        ['VEG','999','vegetation:type','other'], // Other 

        // VGT - Volcanic Geologic Type 
        ['VGT','1','volcano:type','volcano'], // Volcano 
        ['VGT','2','volcano:type','cinder_cone'], // Cinder Cone 
        ['VGT','3','volcano:type','shield'], // Shield 
        ['VGT','4','volcano:type','caldera'], // Caldera 
        ['VGT','5','volcano:type','composite'], // Composite 

        // VH3 - Predominant Vegetation Height 
        // ['VH3','0','vegetation:height','unknown'], // Unknown 
        ['VH3','0',undefined,undefined], // Unknown 
        ['VH3','1','vegetation:height','>0_and_<=5'], // >0 and <=5 
        ['VH3','2','vegetation:height','>5_and_<=10'], // >5 and <=10 
        ['VH3','3','vegetation:height','>10_and_<=20'], // >10 and <=20 
        ['VH3','4','vegetation:height','>20_and_<=40'], // >20 and <=40 
        ['VH3','5','vegetation:height','>40'], // >40 
        ['VH3','997','vegetation:height','unpopulated'], // Unpopulated 
        ['VH3','998','vegetation:height','not_applicable'], // Not Applicable 

        // VOL - Volume 
        ['VOL','1','raw:VOL','empty'], // Empty 
        ['VOL','2','raw:VOL','light'], // Light 
        ['VOL','3','raw:VOL','moderate'], // Moderate 
        ['VOL','4','raw:VOL','heavy'], // Heavy 

        // VRR - Vertical Reference Category 
        // ['VRR','0','raw:VRR','unknown'], // Unknown 
        ['VRR','0',undefined,undefined], // Unknown 
        ['VRR','1','hydrographic_vertical_position','always_dry'], // Above Surface/Does Not Cover (at High Water)
        ['VRR','2','hydrographic_vertical_position','awash_at_chart_datum'], // Awash at Sounding Datum
        ['VRR','4','hydrographic_vertical_position','always_submerged'], // Below Surface/Submerged
        ['VRR','8','hydrographic_vertical_position','covers_and_uncovers'], // Covers and Uncovers
        ['VRR','9','hydrographic_vertical_position','not_applicable'], // Not Applicable

        // WFT - Well Feature Type 
        // ['WFT','0','raw:WFT','unknown'], // Unknown 
        ['WFT','0',undefined,undefined], // Unknown 
        ['WFT','2','well:type','walled-in'], // Walled-in
        ['WFT','3','well:type','artesian_well'], // Artesian Well
        ['WFT','5','well:type','dug_or_drilled_well'], // Dug or Drilled Well
        ['WFT','6','well:type','dug'], // Dug
        ['WFT','7','well:type','drilled'], // Drilled
        ['WFT','999','well:type','other'], // Other

        // WST - Water Sink Type 
        // ['WST','0','water:sink:type','unknown'], // Unknown 
        ['WST','0',undefined,undefined], // Unknown 
        ['WST','1','water:sink:type','dissipating'], // Dissipating 
        ['WST','2','water:sink:type','disappearing'], // Disappearing 

        // WTC - Road Weather Restriction
        // ['WTC','0','seasonal','unknown'], 
        ['WTC','0',undefined,undefined], 
        ['WTC','1','seasonal','no'], 
        ['WTC','All Weather','seasonal','no'], 
        ['WTC','2','seasonal','fair'], // possibly seasonal=
        ['WTC','Fair/Dry Weather','seasonal','fair'], // possibly seasonal=
        ['WTC','3','seasonal','winter'],
        ['WTC','4','seasonal','no'],

        // YSU - Service Branch 
        ['YSU','0','military:service','unknown'], // Unknown 
        ['YSU','1','military:service','air_force'], // Air Force 
        ['YSU','2','military:service','army'], // Army 
        ['YSU','3','military:service','coastguard'], // Coast Guard 
        ['YSU','4','military:service','marines'], // Marines 
        ['YSU','5','military:service','navy'], // Navy 
        ['YSU','7','military:service','joint'], // Joint 
        ['YSU','998','military:service','not_applicable'], // Not Applicable 
        ['YSU','999','military:service','other'], // Other

        // Z_VALUE_TYPE - Z-Coordinate Type 
        ['Z_VALUE_TYPE','2D data','source:vertical_source:type','2d_data'], // 2D data 
        ['Z_VALUE_TYPE','DTED1 derived','source:vertical_source:type','dted1'], // DTED1 derived 
        ['Z_VALUE_TYPE','DTED2 derived','source:vertical_source:type','dted2'], // DTED2 derived 
        ['Z_VALUE_TYPE','Other','source:vertical_source:type','other'], // Other 
        ['Z_VALUE_TYPE','Reflective surface derived','source:vertical_source:type','reflective_surface'], // Reflective surface derived 
        ['Z_VALUE_TYPE','Stereo derived','source:vertical_source:type','stereoscopic_imagery'], // Stereo derived 
        ['Z_VALUE_TYPE','TIN derived','source:vertical_source:type','tin'], // TIN derived 
        ['Z_VALUE_TYPE','Unknown','source:vertical_source:type','unknown_associated_attribute_information'], // Unknown  Associated Attribute Information
        ], // End one2one
    // ##### End of One2One Rules #####

} // End of ufd.rules
