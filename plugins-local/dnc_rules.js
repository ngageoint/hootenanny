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
    DNC One2one rules
*/

dnc.rules = {
    // ##### Start of One2One Rules #####

    // FCODE rules for Import
    // This is where anything "unique" to DNC goes.
    fcodeOne2oneIn : [
        ['F_CODE','AC000','facility','yes'], // Processing Plant - Superseded by AL010
        ['F_CODE','AH010','bastion','yes'], // Bastion/Rampart - not in NAS
        ['F_CODE','AH050','site:type','fortification'], // Fortification - not in NAS
        ['F_CODE','AL005','boundary','protected_area'], // Animal Sanctuary - not in NAS
        ['F_CODE','AL015','building','yes'], // Building - NFDD AL013
        ['F_CODE','AL240','man_made','tower'], // Tower (Non-Communication) - NFDD AL241
        ['F_CODE','AQ010','cableway','yes'], // Aerial Cableway / Ski Lift Lines - Superseded by AT041
        ['F_CODE','AT020','man_made','radar_station'], // Early Warning Radar Site - not in NAS
        ['F_CODE','AT030','power','line'], // Power Line - Superseded by AT005
        ['F_CODE','AT060','cable:type','communication'], // Communication Line - NFDD AT005
        ['F_CODE','AT080','tower:type','communication'], // Communications Tower - NFDD AL241
        ['F_CODE','BA020','foreshore','yes'], // Foreshore Region - NFDD BA023
        ['F_CODE','BD000','underwater_hazard','yes'], // Underwater Danger / Underwater Hazard - Not in NAS
        ['F_CODE','BD010','seamark:water_turbulence:category','breakers'], // Breakers - not in NAS
        ['F_CODE','BH080','water','lake'], // Lake - NFDD BH082
        ['F_CODE','BH130','water','reservoir'], // Reservoir - NFDD BH082
        ['F_CODE','BH210','shoreline','yes'], // Inland Shoreline - NFDD BA010 Land/water boundary
        ['F_CODE','EC030','landuse','forest'], // Wood - NFDD EC015

    ], // End fcodeOne2oneIn

    // One2one rules for Text Fields
    txtBiased : {
        'BRR':'raw:BRR', // Bearing and Reciprocal Category
        'COL':'seamark:light:character', // Character of Light
        'DAN':'raw:DAN', // Description of Aids to Navigation
        'DAT':'date', // Date
        'DRP':'raw:DRP', // Description of Reference Point
        'LSA':'raw:LSA', // Light Sector Angle
        'MLR':'raw:MLR', // Multiple Light Ranges
        'NAM':'name', // Geographic Name Information : Full Name
        'TXT':'source:text' // Text
     }, // End txtBiased

    // One2one ruled for Number fields
    numBiased : {
        'BR2':'raw:BR2', // Broadcast Frequency 2
        'BRF':'raw:BRF', // Broadcast Frequency
        'BRG':'raw:BRG', // Bearing of Object
        'BRS':'raw:BRS', // Bearing from Seaward
        'CRN':'raw:CRN', // Current Rate Minimum
        'CRV':'ele:max', // Depth Curve or Contour Value
        'CRX':'raw:CRX', // Current Rate Maximum
        'CVH':'raw:CVH', // Depth Curve or Contour Value - High
        'CVL':'raw:CVL', // Depth Curve or Contour Value - Low
        'DEP':'depth', // Depth
        'DOF':'flow_direction', // Direction of FLow
        'EOL':'seamark:light:height', // Elevation of Light
        'HDH':'hydrographic_drying_height', // Hydrographic Drying Height
        'HDP':'depth:hydrographic', // Hydrographic Depth
        'HSB':'height:above_sea_bottom', // Height above sea bottom
        'HGT':'height', // Height Above Surface Level
        'LVN':'raw:LVN', // Light Range Nominal
        'MVC':'bridge:under_clearance', // Maximum Vertical Clearance
        'OHC':'max_clearance', // Overhead Clearance
        'OR2':'raw:OR2', // Operating Range Category
        'ORC':'raw:ORC', // Operating Range Category 2
        'PER':'raw:PER', // Period of Light
        'SHC':'safe_clearance:horizontal', // Safe Horizontal Clearance
        'VAL':'value', // Value - a generic value
        'VAV':'raw:VAV', // Variation Anomaly Value
        'WID':'width', // Width
        'ZV2':'ele' // Highest Z-Value
     }, // End numBiased
    

    // One2one rules. Used for import. Taken from the DNC spec
    one2one : [
        // ACC - Horizontal Accuracy Category
        ['ACC','1','source:accuracy:horizontal:category','accurate'],
        ['ACC','2','source:accuracy:horizontal:category','approximate'],
        ['ACC','3','source:accuracy:horizontal:category','doubtful'],
        ['ACC','5','source:accuracy:horizontal:category','disputed'],
        ['ACC','6','source:accuracy:horizontal:category','undisputed'],
        ['ACC','7','source:accuracy:horizontal:category','precise'],
        ['ACC','8','source:accuracy:horizontal:category','abrogated'],


        // AHC - Associated Hydrographic Category
        ['AHC','1','intermittent','no'], // Perennial
        ['AHC','2','intermittent','yes'], // Intermittent
        ['AHC','3','intermittent','ephemeral'], // Ephemeral - not great...


        // APT - Airfield Type
        ['APT','1','aeroway:airfield:type','major'], // Major
        ['APT','2','aeroway:airfield:type','minor'], // Minor
        ['APT','3','aeroway:airfield:type','seaplane'], // Seaplane
        ['APT','9','aeroway','heliport'], // Seaplane


        // ATN - Aids to Navigation
        ['ATN','0','navigation_aid','unknown'], // Unknown
        ['ATN','1','navigation_aid','marked'], // Marked
        ['ATN','2','navigation_aid','unmarked'], // Unmarked
        ['ATN','3','navigation_aid','lit'], // Lit
        ['ATN','9','navigation_aid','unlit'], // Unlit
        ['ATN','999','navigation_aid','heliport'], // Other


        // BFC - Building Function Category
        ['BFC','0','building','yes'],
        ['BFC','1','use','manufacturing'], // Fabrication Structures
        ['BFC','2','office','government'],
        ['BFC','3','building','capitol'], // Capitol Building
        ['BFC','4','building','castle'], // Castle
        ['BFC','5','amenity','government_administration_building'],
        ['BFC','6','building','hospital'],
        ['BFC','7','amenity','place_of_worship'],
        ['BFC','8','military','administration'],
        ['BFC','9','tourism','museum'],
        ['BFC','10','building','observatory'], // Observatory
        ['BFC','11','building','palace'],
        ['BFC','12','amenity','police'],
        ['BFC','13','amenity','prison'],
        ['BFC','14','amenity','ranger_station'], // Ranger Station
        ['BFC','15','building','school'],
        ['BFC','16','building','residential'],
        ['BFC','17','building:multi_unit','yes'], // Multi Unit Dwelling
        ['BFC','18','building','cemetery_building'],
        ['BFC','19','building','farm'],
        ['BFC','20','building','greenhouse'], // Greenhouse
        ['BFC','21','building','garage'],
        ['BFC','22','building','watermill'], // Watermill/Gristmill
        ['BFC','23','building','wind_tunnel'], // Wind Tunnel
        ['BFC','24','building','warehouse'],
        ['BFC','25','railway','roundhouse'], // Roundhouse
        ['BFC','26','repair','railway_vehicle'],
        ['BFC','27','man_made','depot'], // Depot Terminal
        ['BFC','28','office','administrative'],
        ['BFC','29','repair','aircraft'], // Aircraft Maintenance Shop
        ['BFC','30','building','hangar'],
        ['BFC','31','building','customs_house'], // Customs House
        ['BFC','33','building','healthcare'],
        ['BFC','34','sport','shooting_range'], // Firing Range
        ['BFC','35','amenity','post_office'],
        ['BFC','36','military','barracks'],
        ['BFC','37','amenity','fire_station'],
        ['BFC','38','amenity','prison'], // Jail
        ['BFC','40','use','telephone_switching_station'], // Telephone Switching Station
        ['BFC','51','amenity','marketplace'],
        ['BFC','52','amenity','townhall'], // Town Hall
        ['BFC','53','amenity','bank'],
        ['BFC','54','amenity','fuel'],
        ['BFC','55','leisure','yacht_club'], // Yacht Club/Sailing Club
        ['BFC','56','amenity','pub'], // Public Inn
        ['BFC','57','amenity','restaraunt'],
        ['BFC','58','use','observation'], // Observation
        ['BFC','59','use','research'],
        ['BFC','61','amenity','courthouse'],
        ['BFC','Courthouse','amenity','courthouse'],
        ['BFC','62','building','legation'], // Legation
        ['BFC','63','building','mission'], // Mission
        ['BFC','64','building','chancery'], // Chancery
        ['BFC','65','amenity','ambassadorial_residence' ],
        ['BFC','66','amenity','embassy' ],
        ['BFC','67','amenity','consulate' ],
        ['BFC','68','building','guardhouse'],
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
        ['BFC','81','building','maritime_station'], // Maritime Station
        ['BFC','82','man_made','lighthouse'], // Lighthouse
        ['BFC','83','plant','power'],
        ['BFC','84','industrial','filtration_plant'], // Filtration Plant
        ['BFC','85','amenity','newspaper_plant'],
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
        ['BFC','96','amenity','diplomatic_building'],
        ['BFC','97','building','trading_post'], // Trading Post
        ['BFC','99','military','battery'], // Battery
        ['BFC','100','building','healthcare'], // Medical Centre. Dont know if hospital, clinic etc
        ['BFC','101','building','municipal_hall'], // Municipal Hall
        ['BFC','102','industrial','oil_gas_facility'], // Not great
        ['BFC','103','building','outbuilding'], // Outbuilding
        ['BFC','104','industrial','paper_mill'],
        ['BFC','105','amenity','reformatory'], // Reformatory
        ['BFC','106','amenity','sanitarium'], // Sanitarium
        ['BFC','107','building','satellite_tracking_station'], // Satellite Tracking Station
        ['BFC','109','social_facility','senior'], // Senior Citizen's Home
        ['BFC','110','amenity','shipyard'], // Shipyard
        ['BFC','111','leisure','sports_centre'],
        ['BFC','112','industrial','steel_mill'], // Steel Mill
        ['BFC','113','amenity','weigh_scale_(highway)'], // Weigh Scale (Highway)
        ['BFC','115','tourism','hostel'], // Hostel
        ['BFC','116','industrial','manufacturing'],
        ['BFC','117','tourism','motel'],
        ['BFC','118','amenity','community_centre'],
        ['BFC','119','amenity','townhall'],
        ['BFC','120','industrial','motor_vehicle'],
        ['BFC','121','building','armory'], // Armory
        ['BFC','122','shop','mall'],
        ['BFC','123','amenity','prison'], // Correctional Institute
        ['BFC','124','repair','yes'], // Repair Facility
        ['BFC','125','building','barn'],
        ['BFC','126','amenity','astronomical_station'], // Astronomical Station
        ['BFC','127','amenity','theatre'], // Theater
        ['BFC','128','amenity','library'],
        ['BFC','129','aeroway','terminal'], // Airport Terminal
        ['BFC','130','amenity','bus_station'],
        ['BFC','131','use','pilot_office'], // Pilot Office
        ['BFC','132','use','pilot_look-out'], // Pilot Look-out
        ['BFC','133','building','commercial'],
        ['BFC','140','building','yes'], // Significant building ???
        ['BFC','150','military','barracks'],
        ['BFC','151','building','dormitory'], // Dormitory
        ['BFC','723','amenity','fire_and_police'], // Combined Fire and Police Station
        ['BFC','999','building','other'],
        ['BFC','Other','building','other'],


        // BOT - Bridge Opening Type
        ['BOT','0','bridge','yes'], // Unknown
        ['BOT','4','bridge','bascule'], // Draw/Bascule
        ['BOT','10','bridge','swing'],
        ['BOT','11','bridge','lift'],
        ['BOT','12','bridge','retractable'],
        ['BOT','13',undefined,undefined], // Not Applicable
        ['BOT','17','bridge','fixed'],


        // BSC - Bridge Structure Type
        ['BSC','0',undefined,undefined], // Unknown
        ['BSC','1','bridge:structure','open_spandrel_arch'], // Open Spandrel Arch
        ['BSC','2','bridge:structure','cantilever'], // Cantilever
        ['BSC','5','bridge:structure','floating'], // Floating
        ['BSC','6','bridge:structure','girder'], // Girder
        ['BSC','7','bridge:structure','tower_suspension'], // Tower Suspension
        ['BSC','8','bridge:structure','truss'], // Truss
        ['BSC','9','bridge:structure','suspension'], // Suspension
        ['BSC','12','bridge:structure','transporter'], // Transporter
        ['BSC','15','bridge:structure','slab'], // Slab
        ['BSC','16','bridge:structure','stringer_beam'], // Stringer Beam
        ['BSC','19','bridge:structure','bowstring'], // Bowstring-bridge
        ['BSC','22','bridge:structure','covered'], // Covered
        ['BSC','26','bridge:structure','closed_spandrel_arch'], // Closed Spandrel Arch
        ['BSC','27','bridge:structure','cable_stayed'], // Cable Stayed
        ['BSC','30','bridge:structure','submersible'], // Submersible
        ['BSC','31','bridge:structure','arch'], // Arch
        ['BSC','32','bridge:structure','trestle'], // Trestle
        ['BSC','999','bridge:structure','other'], // Trestle        


        // BSM - Bridge Span Mobility Type
        ['BSM','0',undefined,undefined], // Unknown
        ['BSM','1','bridge:span_mobility','moveable'], // Moveable Span
        ['BSM','2','bridge:span_mobility','fixed'], // Fixed Span


        // CHA - Light Characteristoc Category
        ['CHA','21','raw:CHA','lighted'], // Lighted
        ['CHA','23','raw:CHA','unlighted'], // Unlighted


        // COD - Certainty of Delineation
        ['COD','0','deliniation','information_unknown'], // Unknown
        ['COD','1','deliniation','known'], // Limits and Information Known
        ['COD','2','deliniation','unknown'], // Limits and Information Unknown
    

        // ELA - Elevation Accuracy Category
        ['ELA','1','source:accuracy:vertical:category','accurate'],
        ['ELA','2','source:accuracy:vertical:category','approximate'],


        // EXS - Existence Category 
        ['EXS','0','existence','unknown'], // Unknown
        ['EXS','1','existence','definite'],
        ['EXS','2','existence','doubtful'],
        ['EXS','3','existence','reported'],
        ['EXS','5','condition','construction'],
        ['EXS','6','condition','abandoned'],
        ['EXS','7','condition','destroyed'],
        ['EXS','8','condition','dismantled'], // Dismantled 
        ['EXS','10','condition','proposed'], // Proposed 
        ['EXS','11','operational_status','temporary'], // Temporary 
        ['EXS','22','oneway','yes'], // One Way
        ['EXS','23','oneway','no'], // Two Way
        ['EXS','27','operational_status','closed'], // Closed/Locked
        ['EXS','28','condition','functional'], // Fully Functional - default
        ['EXS','30','isolated','no'], // Not Isolated ???
        ['EXS','31','isolated','yes'], // Isolated 
        ['EXS','32','navigation','yes'], // Navigable 
        ['EXS','36','operational_status','commissioned_and_operational'], // Commissioned and Operational 
        ['EXS','37','operational_status','commissioned_and_on_test'], // Commissioned and on Test! 
        ['EXS','38','operational_status','commissioned_and_out_of_service'], // Commissioned and out of service 
        ['EXS','39','operational_status','not_commissioned_and_operational'], // Not commissioned and operational 
        ['EXS','40','operational_status','not_commissioned_and_on_test'], // Not commissioned and on test 
        ['EXS','41','operational_status','not_commissioned_and_out_of_service'], // Not commissioned and out of service 
        ['EXS','42','operational_status','continuous_operation'], // Continuous Operation 
        ['EXS','43','operational_status','intermittent_operation'], // Intermittent operation 
        ['EXS','45','artificial','no'], // Natural 
        ['EXS','46','artificial','yes'], // Man-Made 
        ['EXS','48','operational_status','controlled'], // Controlled 
        ['EXS','50','tidal','no'], // Non Tidal
        ['EXS','51','tidal','yes'], // Tidal / Tidal Fluctuation
        ['EXS','59','operational_status','not_usable'], // Not Usable 
        ['EXS','62','operational_status','partially_destroyed'], // Partially Destroyed 
        ['EXS','66','condition','damaged'],
        ['EXS','997',undefined,undefined], // Unpopulated
        ['EXS','999','operational_status','other'],


        // HDI - Hydrographic Depth/Height Information 
        ['HDI','9','hydrographic_depth_information','depth_known_by_other_than_wire_drag'],
        ['HDI','10','hydrographic_depth_information','depth_known_by_wire_drag'],
        ['HDI','11','hydrographic_depth_information','depth_unknown_but_safe_to_depth_shown'],
        ['HDI','12','hydrographic_depth_information','depth_unknown'],
        ['HDI','13','hydrographic_depth_information','uncovering_height_known'],
        ['HDI','14','hydrographic_depth_information','uncovering_height_unknown'],
        ['HDI','15','hydrographic_depth_information','depth_unknown'],
        ['HDI','13',undefined,undefined], // Not Applicable


        // HWT - House of Worship Type
        ['HWT','0',undefined,undefined],
        ['HWT','2','building','cathedral'],
        ['HWT','3','building','chapel'],
        ['HWT','4','building','church'],
        ['HWT','5','building','marabout'],
        ['HWT','6','building','minaret'],
        ['HWT','7','building','religious_community'],
        ['HWT','9','building','mosque'],
        ['HWT','11','building','pagoda'],
        ['HWT','14','building','shrine'],
        ['HWT','15','building','tabernacle'],
        ['HWT','16','building','temple'],
        ['HWT','20','building','synagogue'],
        ['HWT','21','building','stupa'],
        ['HWT','22',undefined,undefined],


        // HYC - Hydrologic Category
        ['HYC','3','intermittent','dry'], // Dry
        ['HYC','6','intermittent','yes'], // Non-Perennial
        ['HYC','8','intermittent','no'], // Perennial


        // IAS - IMO Approval Status
        ['IAS','1','raw:IAS','approved'], // Approved
        ['IAS','2','raw:IAS','not_approved'], // Not Approved


        // LOC - Vertical Relative Location. FACC codes added
        ['LOC','0','location','unknown'], // Unknown
        ['LOC','1','location','above_surface/does_not_cover_(height_known)'], // Above Surface/Does not Cover (Height Known) 
        ['LOC','2','location','awash_at_chart_datum'], // Awash at Chart Datum 
        ['LOC','4','location','underwater'], // Below Water Surface
        ['LOC','8','location','surface'], // On Ground Surface
        ['LOC','13','location','hull_showing'], // Hull Showing 
        ['LOC','14','location','masts_showing'], // Masts Showing 
        ['LOC','15','location','on_water_surface/floating'], // On Water Surface/Floating 
        ['LOC','17','location','on_waterbody_bottom'], // No OSM
        ['LOC','20','location','funnel_showing'], // Funnel Showing 
        ['LOC','21','location','superstructuure_showing'], // Superstructure Showing 
        ['LOC','22','location','offshore'], // Off Shore 
        ['LOC','23','location','below_waterbody_bottom'], // No OSM
        ['LOC','24','location','suspended_or_elevated_above_sea_bottom'], // Suspended or Elevated above sea bottom 
        ['LOC','25','location','overground'], // From DIGEST FACC - Elevated above ground or water surface
        ['LOC','28','location','masts_and_funnel_showing'], // Masts and Funnel Showing 
        ['LOC','30','location','non-floating'], // Non-Floating 
        ['LOC','40','location','underground'],
        ['LOC','44','location','surface'], // The default is on the surface
        ['LOC','45','location','overground'], // Above ground
        ['LOC','46','location','underwater'], // Above waterbody bottom
        ['LOC','997',undefined,undefined], // Unpopulated 
        ['LOC','998',undefined,undefined], // Not Applicable 
        ['LOC','999',undefined,undefined], // Other 


        // MCC - Material Composition Type
        ['MCC','0',undefined,undefined], // No Information
        ['MCC','2','material','aluminum'], // Aluminum
        ['MCC','4','material','ash'], // Ash
        ['MCC','5','material','asphalt'], // Asphalt
        ['MCC','7','material','bedrock'], // Bedrock
        ['MCC','8','material','boulders'], // Boulders
        ['MCC','9','material','brick'], // Brick
        ['MCC','12','material','chalk'], // Chalk
        ['MCC','14','material','cinders'], // Cinders
        ['MCC','15','material','cirripedia'], // Cirripedia
        ['MCC','16','material','clay'], // Clay
        ['MCC','18','material','cobble'], // Cobble
        ['MCC','21','material','concrete'], // Concrete
        ['MCC','24','material','coral'], // Coral
        ['MCC','25','material','coral_head'], // Coral Head
        ['MCC','28','material','diatoms'], // Diatoms
        ['MCC','36','material','foraminifers'], // Foraminifers
        ['MCC','37','material','fucus'], // Fucus
        ['MCC','41','material','globigerina'], // Globingerina
        ['MCC','45','material','grass'], // Grass / Thatch
        ['MCC','46','material','gravel'], // Gravel
        ['MCC','48','material','ground'], // Ground
        ['MCC','51','material','iron'], // Iron
        ['MCC','52','material','lava'], // Lava
        ['MCC','55','material','loess'], // Loess
        ['MCC','58','material','madrepores'], // Madrepores
        ['MCC','59','material','manganese'], // Manganese
        ['MCC','61','material','marl'], // Marl
        ['MCC','62','material','masonry'], // Masonry
        ['MCC','63','material','mattes'], // mattes
        ['MCC','64','material','metal'], // Metal
        ['MCC','65','material','mud'], // Mud
        ['MCC','66','material','mussels'], // Mussels
        ['MCC','69','material','ooze'], // ooze
        ['MCC','70','material','oysters'], // Oysters
        ['MCC','73','material','pebbles'], // Pebbles
        ['MCC','74','material','plastic'], // Plastic
        ['MCC','75','material','polyzoa'], // Polyzoa
        ['MCC','77','material','prestressed_concrete'], // Prestressed Concrete
        ['MCC','78','material','pteropods'], // Pteropods
        ['MCC','79','material','pumice'], // Pumice
        ['MCC','80','material','quartz'], // Quartz
        ['MCC','83','material','reinforced_concrete'], // Reinforced Concrete
        ['MCC','84','material','rock'], // Rock
        ['MCC','86','material','rubble'], // Rubble
        ['MCC','88','material','sand'], // Sand
        ['MCC','90','material','schist'], // Schist
        ['MCC','92','material','scoria'], // Scoria
        ['MCC','93','material','sea_tangle'], // Sea Tangle
        ['MCC','94','material','seaweed'], // Seaweed
        ['MCC','96','material','shells'], // Shells
        ['MCC','98','material','shingle'], // Shingle
        ['MCC','99','material','shingle'], // Shingle
        ['MCC','103','material','frozen_water'], // Frozen Water
        ['MCC','104','material','soil'], // Soil
        ['MCC','105','material','spicules'], // Spicules
        ['MCC','106','material','sponge'], // Sponge
        ['MCC','107','material','steel'], // Steel
        ['MCC','108','material','stone'], // Stone
        ['MCC','111','material','tufa'], // Tufa
        ['MCC','117','material','wood'], // Wood
        ['MCC','119','material','evaporite'], // Evaporite
        ['MCC','150','material','treated_timber'], // Treated Timber
        ['MCC','152','material','fibreglass'], // Fibreglass
        ['MCC','999','material','other'], // Other


        // MCS - Material Composition Secondary
        ['MCS','0',undefined,undefined], // No Information
        ['MCC','46','raw:MCS','gravel'], // Gravel
        ['MCC','65','raw:MCS','mud'], // Mud
        ['MCC','88','raw:MCS','sand'], // Sand
        ['MCC','98','raw:MCS','shingle'], // Shingle
        ['MCC','108','raw:MCS','stone'], // Stone

        
        // OPS - Operational Status
        ['OPS','1','operational_status','operational'], // Operational
        ['OPS','2','operational_status','non_operational'], // Non Operational


        // OWO - Over Water Obstruction
        ['OWO','1','waterway:overhead_obstruction','yes'], // Feature crosses navigable water 
        ['OWO','2','waterway:overhead_obstruction','no'], // Feature does not cross navigable water 


        // PLT - Pipeline Type
        ['PLT','0','pipeline:usage','unknown'], // Unknown
        ['PLT','1','pipeline:usage','transport'], // Transport
        ['PLT','2','pipeline:usage','outfall'], // Outfall
        ['PLT','3','pipeline:usage','intake'], // Intake


        // PRO - Product
        ['PRO','0','product','unknown'], // Unknown
        ['PRO','3','product','ammunition'], // Ammunition
        ['PRO','13','product','chemical'], // Chemical
        ['PRO','17','product','coal'], // Coal
        ['PRO','19','product','coke'], // Coke
        ['PRO','31','product','electric_power'], // Electric Power
        ['PRO','33','product','explosive'], // Explosive
        ['PRO','38','product','gas'], // Gas
        ['PRO','39','product','petrol'], // Petrol (Gasoline)
        ['PRO','46','product','gravel'], // Gravel -- From PPO
        ['PRO','67','product','oil'], // Oil
        ['PRO','82','product','radioactive_material'], // Radioactive Material
        ['PRO','87','product','salt'], // Salt
        ['PRO','88','product','sand'], // Sand
        ['PRO','95','product','sewage'], // Sewage
        ['PRO','108','product','stone'], // Stone
        ['PRO','116','product','water'], // Water
        ['PRO','130',undefined,undefined], // None
        ['PRO','999','product','other'], // Other


        // RPA - Required Port Access
        ['RPA','0','port:access','unknown'], // Unknown
        ['RPA','1','port:access','required'], // Required
        ['RPA','2','port:access','not_required'], // Not Required


        // REF - Radar Reflector Attribute
        ['REF','1','raw:REF','radar_reflector_present'], // Radar Reflector Present
        ['REF','2','raw:REF','radar_reflector_absent'], // Radar Reflector Absent


        // RRC - Railway Use
        ['RRC','0','railway','rail'], // Unknown
        ['RRC','2','railway','carline'], // Carline
        ['RRC','3','railway','monorail'], // Carline
        ['RRC','8','railway','logging'], // Logging
        ['RRC','13','railway','marine_railway'], // Marine Railway
        ['RRC','14','railway','tram'], // Tramway
        ['RRC','15','railway','funicular'], // Funicular
        ['RRC','16','usage','main'], // Main Line
        ['RRC','17','usage','branch'], // Branch-line
        ['RRC','21','railway:in_road','yes'],


        // RTT - Route Intended Use
        ['RTT','0','raw:RTT','unknown'], // Unknown
        ['RTT','2','raw:RTT','recommended_for_other_than_deep_draft_vessels'], // Recommended for other than deep draft vessels
        ['RTT','3','raw:RTT','recommended_for_deep_draft_vessels'], // Recommended for deep draft vessels
        ['RTT','4','raw:RTT','deep_water_route'], // Deep Water Route
        ['RTT','5','raw:RTT','transit_route'], // Transit Route
        ['RTT','11','raw:RTT','two_way_route'], // Two Way Route
        ['RTT','13','raw:RTT','recommended_direction_of_traffic_flow'], // Recommended direction of traffic flow


        // SFC - Sea Floor Category
        ['SFC','1','sea_floor_category','unknown_obstruction'], // Unknown Obstruction - Not in FACC
        ['SFC','2','sea_floor_category','other'], // Other
        ['SFC','3','sea_floor_category','fish_haven'], // Fish Haven
        ['SFC','5','sea_floor_category','submerged_production_platform'], // Submerged Production Platform
        ['SFC','6','sea_floor_category','wreckage'], // Wreckage
        ['SFC','7','sea_floor_category','shoaling'], // Shoaling
        ['SFC','8','sea_floor_category','less_water_reported'], // Less Water Reported
        ['SFC','9','sea_floor_category','unexploded_ordnance'], // Unexploded Ordnance
        ['SFC','10','sea_floor_category','unexpected_non_submarine_contact'], // Unspecified Non-submarine COntact
        ['SFC','11','sea_floor_category','pinnacle'], // Pinnacle


        // SIC - Snow / Ice Category
        // ['SIC','-999999',undefined,undefined], // No Information
        ['SIC','0','landcover','unknown'], // Unknown
        ['SIC','1','landcover','snowfield'], // Snowfield
        ['SIC','2','landcover','ice-field'], // Ice-field


        // SLT - Shoreline Type
        ['SLT','0',undefined,undefined], // Unknown
        ['SLT','6','shoreline:type','mangrove'], // Mangrove
        ['SLT','8','shoreline:type','marshy'], // Marshy
        ['SLT','10','shoreline:type','stony'], // Stony
        ['SLT','11','shoreline:type','building_rubble'], // Building Rubble
        ['SLT','13','shoreline:type','sandy'], // Sandy
        ['SLT','14','shoreline:type','shingly'], // Shingly
        ['SLT','15','shoreline:type','other'], // Other
        

        // SOH - Severity of Hazard
        ['SOH','1','hazard:severity','dangerous'], // Dangerous
        ['SOH','2','hazard:severity','non_dangerous'], // non Dangerous
        ['SOH','3','hazard:severity','obstruction'], // Obstruction
        ['SOH','99','hazard:severity','non_dangerous_to_surface_navigation_but_avoid_anchoring/trawling'], // From FACC


        // SSC - Structure Shape
        ['SSC','0','shape','unknown'], // Unknown
        ['SSC','6','shape','conical'], // Conical
        ['SSC','7','shape','cylindrical'], // Cylindrical
        ['SSC','10','shape','pillar'], // Pillar
        ['SSC','12','shape','pyramidal'], // Pyramidal
        ['SSC','16','shape','spar'], // Spar
        ['SSC','17','shape','spherical'], // Spherical
        ['SSC','21','attraction','artificial_mountain'], // Artificial Mountain
        ['SSC','23','attraction','ferris_wheel'], // Ferris Wheel
        ['SSC','25','attraction','roller-coaster'], // Roller Coaster
        ['SSC','60','shape','mast'], // Mast
        ['SSC','73','shape','superbuoy'], // Superbuoy
        ['SSC','77','shape','arched'], // Arched
        ['SSC','85','shape','diamond_shaped_buoy'], // Diamond shaped buoy
        ['SSC','87','shape','domed'], // Domed
        ['SSC','107','shape','tower'], // Tower
        ['SSC','108','shape','scanner'], // Scanner
        ['SSC','109','shape','obelisk'], // Obelisk
        ['SSC','999','shape','other'], // Other


        // SSR - Structure Shape of Roof
        ['SSR','0',undefined,undefined],
        ['SSR','40','building:roof_shape','domed'],
        ['SSR','41','building:roof_shape','flat'],
        ['SSR','51','building:roof_shape','with_steeple'],
        ['SSR','77','building:roof_shape','with_cupola'],
        ['SSR','79','building:roof_shape','with_tower'],
        ['SSR','80','building:roof_shape','with_minaret'],
        ['SSR','999','building:roof_shape','other'],


        // SST - Sound Signal Type
        ['SST','0','sound_signal','unknown'], // Unknown
        ['SST','1','sound_signal','bell'], // Bell
        ['SST','2','sound_signal','diaphone'], // Diaphone
        ['SST','3','sound_signal','explosive'], // Explosive Fog Signal
        ['SST','4','sound_signal','gong'], // Gong
        ['SST','6','sound_signal','horn'], // Horn
        ['SST','9','sound_signal','siren'], // Siren
        ['SST','14','sound_signal','whistle'], // Whistle
        ['SST','15','sound_signal','reed'], // Reed
        ['SST','16',undefined,undefined], // None


        // TRE - Tree Category
        ['TRE','0','wood','unknown'], // Unknown
        ['TRE','1','wood','deciduous'], // Deciduous
        ['TRE','2','wood','evergreen'], // Evergreen
        ['TRE','3','wood','mixed'], // Mixed


        // TUC - Transportation Use Category
        // ['TUC','0','transport:type','unknown'], // Unknown
        ['TUC','0',undefined,undefined], // Unknown
        ['TUC','1','transport:type','road_and_railway'], // Both Road and Railway
        ['TUC','3','transport:type','railway'], // Railroad
        ['TUC','4','transport:type','road'], // Road
        ['TUC','6','transport:type','road'], // Street
        ['TUC','7','highway:through_route','yes'],  // Through Route
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


        // USE - Usage
        ['USE','0',undefined,undefined], // Unknown
        ['USE','133','raw:USE','single_point_mooring'], // Single Point Mooring
        ['USE','23','raw:USE','international'], // International
        ['USE','32','raw:USE','insular'], // insular
        ['USE','51','raw:USE','telegraph'], // Telegraph
        ['USE','52','raw:USE','telephone'], // Telephone
        ['USE','53','raw:USE','power'], // Power
        ['USE','57','raw:USE','marine'], // Marine
        ['USE','69','raw:USE','levee/dike'], // Levee / Dike
        ['USE','133','raw:USE','single_point_mooring'], // Single Point Mooring
        ['USE','119','raw:USE','berthing_vessels'], // Single Point Mooring
        ['USE','120','raw:USE','recreational'], // Recreational
        ['USE','127','raw:USE','causeway'], // As a Causeway
        ['USE','131','raw:USE','flood_barrage'], // flood Barrage
        ['USE','132','raw:USE','container'], // Container
        ['USE','133','raw:USE','single_point_mooring'], // Single Point Mooring
        ['USE','999','raw:USE','other'], // Single Point Mooring


        // VEG - Vegetation Characteristics
        ['VEG','0','vegetation:type','unknown'], // Unknown
        ['VEG','11','vegetation:type','casuarina'], // Casuarina
        ['VEG','12','vegetation:type','conifer'], // Conifer
        ['VEG','16','vegetation:type','nipa'], // Nipa
        ['VEG','17','vegetation:type','palm'], // Palm
        ['VEG','18','vegetation:type','filao'], // Filao
        ['VEG','19','vegetation:type','mangrove'], // Mangrove
        ['VEG','29','vegetation:type','eucalyptus'], // Eucalyptus
        ['VEG','38','vegetation:type','cypress'], // Cypress
        ['VEG','999','vegetation:type','other'], // Other

        
        // VRR - Vertical Reference Category 
        ['VRR','0',undefined,undefined], // Unknown 
        ['VRR','1','hydrographic_vertical_position','always_dry'], // Above Surface/Does Not Cover (at High Water)
        ['VRR','2','hydrographic_vertical_position','awash_at_chart_datum'], // Awash at Sounding Datum
        ['VRR','4','hydrographic_vertical_position','always_submerged'], // Below Surface/Submerged
        ['VRR','8','hydrographic_vertical_position','covers_and_uncovers'], // Covers and Uncovers
        ['VRR','9','hydrographic_vertical_position','not_applicable'], // Not Applicable

    ] // ##### End of One2One Rules #####

} // End of dnc.rules
