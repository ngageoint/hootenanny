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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
    GIFD D4 One2one rules
*/

gifd.rules = {
    // FCODE rules for Import
    // This is where anything "unique" to GIFD goes.
    fcodeOne2oneIn : [
        ['F_CODE','AA012','raw:F_CODE','AA012'], // Quarry
        ['F_CODE','AA050','raw:F_CODE','AA050'], // Well
        ['F_CODE','AC000','raw:F_CODE','AC000'], // Processing/Treatment Plant
        ['F_CODE','AF010','raw:F_CODE','AF010'], // Chimney/Smokestack
        ['F_CODE','AH010','raw:F_CODE','AH010'], // Bastion/Rampart/Fortification
        ['F_CODE','AH020','raw:F_CODE','AH020'], // Trench
        ['F_CODE','AH050','raw:F_CODE','AH050'], // Fortification
        ['F_CODE','AK050','sport','tennis'], // Tennis Court - Valid NFDD but not in TDS Spec
        ['F_CODE','AK123','raw:F_CODE','AK123'], // Park Bench
        ['F_CODE','AK124','raw:F_CODE','AK124'], // Picnic Table
        ['F_CODE','AK140','raw:F_CODE','AK140'], // Planter
        ['F_CODE','AK141','raw:F_CODE','AK141'], // Statue Pedestal
        ['F_CODE','AK190','raw:F_CODE','AK190'], // Fishing Pier/Promenade Pier
        ['F_CODE','AL005','raw:F_CODE','AL005'], // Animal Sanctuary
        ['F_CODE','AL015','raw:F_CODE','AL015'], // Building
        ['F_CODE','AL016','raw:F_CODE','AL016'], // Overhang (Storefront)
        ['F_CODE','AL045','raw:F_CODE','AL045'], // Complex Outline
        ['F_CODE','AL050','raw:F_CODE','AL050'], // Display Sign
        ['F_CODE','AL075','raw:F_CODE','AL075'], // Gallery
        ['F_CODE','AL095','raw:F_CODE','AL095'], // Homogeneous Radar Significant Area
        ['F_CODE','AL100','building','hut'], // Hut
        ['F_CODE','AL135','raw:F_CODE','AL135'], // Native Settlement
        ['F_CODE','AL209','raw:F_CODE','AL209'], // Tent Dwellings
        ['F_CODE','AL210','raw:F_CODE','AL210'], // Snow Shed/Rock Shed
        ['F_CODE','AL215','raw:F_CODE','AL215'], // Special Area
        ['F_CODE','AL240','raw:F_CODE','AL240'], // Tower (Non-Communication)
        ['F_CODE','AM031','raw:F_CODE','AM031'], // Timber Yard
        ['F_CODE','AQ010','raw:F_CODE','AQ010'], // Aerial Cableway Lines/Ski Lift Lines
        ['F_CODE','AQ020','raw:F_CODE','AQ020'], // Aerial Cableway Pylon/Ski Pylon
        ['F_CODE','AQ036','raw:F_CODE','AQ036'], // Curb
        ['F_CODE','AQ058','raw:F_CODE','AQ058'], // Constriction/Expansion
        ['F_CODE','AQ064','raw:F_CODE','AQ064'], // Causeway
        ['F_CODE','AQ119','raw:F_CODE','AQ119'], // Route Marker
        ['F_CODE','AQ152','raw:F_CODE','AQ152'], // Overhead Walkway
        ['F_CODE','AT020','raw:F_CODE','AT020'], // Early Warning Radar Site
        ['F_CODE','AT030','raw:F_CODE','AT030'], // Power Transmission Line
        ['F_CODE','AT040','raw:F_CODE','AT040'], // Power Transmission Pylon
        ['F_CODE','AT050','raw:F_CODE','AT050'], // Communication Building
        ['F_CODE','AT060','raw:F_CODE','AT060'], // Telephone Line/Telegraph Line
        ['F_CODE','AT070','raw:F_CODE','AT070'], // Telephone-Telegraph Pylon/Pole
        ['F_CODE','AT080','raw:F_CODE','AT080'], // Communication Tower
        ['F_CODE','BA011','raw:F_CODE','BA011'], // Coastline
        ['F_CODE','BA021','raw:F_CODE','BA021'], // Nearshore (Precise IHO)
        ['F_CODE','BA022','raw:F_CODE','BA022'], // Backshore (Precise IHO)
        ['F_CODE','BA024','raw:F_CODE','BA024'], // Shoreline
        ['F_CODE','BB019','raw:F_CODE','BB019'], // Anchor
        ['F_CODE','BB020','raw:F_CODE','BB020'], // Berth
        ['F_CODE','BB030','raw:F_CODE','BB030'], // Bollard
        ['F_CODE','BB041','raw:F_CODE','BB041'], // Breakwater
        ['F_CODE','BB042','raw:F_CODE','BB042'], // Mole
        ['F_CODE','BB043','raw:F_CODE','BB043'], // Groin
        ['F_CODE','BB050','raw:F_CODE','BB050'], // Calling-In Point
        ['F_CODE','BB080','raw:F_CODE','BB080'], // Dolphin
        ['F_CODE','BB105','raw:F_CODE','BB105'], // Fishing Harbor
        ['F_CODE','BB115','raw:F_CODE','BB115'], // Gridiron
        ['F_CODE','BB140','raw:F_CODE','BB140'], // Jetty
        ['F_CODE','BB150','raw:F_CODE','BB150'], // Landing Place
        ['F_CODE','BB151','raw:F_CODE','BB151'], // Landing Stairs
        ['F_CODE','BB160','raw:F_CODE','BB160'], // Mooring Ring
        ['F_CODE','BB170','raw:F_CODE','BB170'], // Offshore Loading Facility
        ['F_CODE','BB180','raw:F_CODE','BB180'], // Oyster Bed/Mussel Bed
        ['F_CODE','BB190','raw:F_CODE','BB190'], // Pier/Wharf/Quay
        ['F_CODE','BB220','raw:F_CODE','BB220'], // Ramp (Maritime)
        ['F_CODE','BB225','raw:F_CODE','BB225'], // Rip Rap
        ['F_CODE','BB230','raw:F_CODE','BB230'], // Seawall
        ['F_CODE','BB240','raw:F_CODE','BB240'], // Slipway/Patent Slip
        ['F_CODE','BC020','raw:F_CODE','BC020'], // Buoy
        ['F_CODE','BC040','raw:F_CODE','BC040'], // Light
        ['F_CODE','BC055','raw:F_CODE','BC055'], // Marker
        ['F_CODE','BC060','raw:F_CODE','BC060'], // Light Sector
        ['F_CODE','BC080','raw:F_CODE','BC080'], // Perches/Stakes
        ['F_CODE','BC100','raw:F_CODE','BC100'], // Leading Line
        ['F_CODE','BC101','raw:F_CODE','BC101'], // Fog Signal
        ['F_CODE','BC102','raw:F_CODE','BC102'], // Direction of Lateral Buoyage
        ['F_CODE','BD000','raw:F_CODE','BD000'], // Underwater Danger/Hazard
        ['F_CODE','BD001','raw:F_CODE','BD001'], // Mine-Naval
        ['F_CODE','BD005','raw:F_CODE','BD005'], // Miscellaneous Underwater Feature
        ['F_CODE','BD010','raw:F_CODE','BD010'], // Breakers
        ['F_CODE','BD040','raw:F_CODE','BD040'], // Eddies
        ['F_CODE','BD060','raw:F_CODE','BD060'], // Kelp/Seaweed
        ['F_CODE','BD071','raw:F_CODE','BD071'], // Log Boom/Booming Ground
        ['F_CODE','BD074','raw:F_CODE','BD074'], // Chain/Wire
        ['F_CODE','BD080','raw:F_CODE','BD080'], // Overfalls/Tide Rips
        ['F_CODE','BD110','raw:F_CODE','BD110'], // Platform
        ['F_CODE','BE010','raw:F_CODE','BE010'], // Depth Curve
        ['F_CODE','BE015','raw:F_CODE','BE015'], // Depth Contour
        ['F_CODE','BE019','raw:F_CODE','BE019'], // Depth Area
        ['F_CODE','BE020','raw:F_CODE','BE020'], // Sounding
        ['F_CODE','BE030','raw:F_CODE','BE030'], // Track Swath
        ['F_CODE','BE040','raw:F_CODE','BE040'], // Track Line
        ['F_CODE','BE050','raw:F_CODE','BE050'], // Beach Profile
        ['F_CODE','BF010','raw:F_CODE','BF010'], // Bottom Characteristics
        ['F_CODE','BF012','raw:F_CODE','BF012'], // Hull Integrity Test Site
        ['F_CODE','BG010','raw:F_CODE','BG010'], // Current Flow
        ['F_CODE','BG012','raw:F_CODE','BG012'], // Water Turbulence
        ['F_CODE','BG020','raw:F_CODE','BG020'], // Tide Gauge
        ['F_CODE','BG030','raw:F_CODE','BG030'], // Tide Data Point
        ['F_CODE','BG040','raw:F_CODE','BG040'], // Current Diagram
        ['F_CODE','BH000','raw:F_CODE','BH000'], // Inland Water
        ['F_CODE','BH050','raw:F_CODE','BH050'], // Fish Hatchery/Fish Farm/Marine Farm
        ['F_CODE','BH060','raw:F_CODE','BH060'], // Flume
        ['F_CODE','BH080','raw:F_CODE','BH080'], // Lake/Pond
        ['F_CODE','BH130','raw:F_CODE','BH130'], // Reservoir
        ['F_CODE','BH171','raw:F_CODE','BH171'], // Fire Hydrant
        ['F_CODE','BH190','raw:F_CODE','BH190'], // Lagoon/Reef Pool
        ['F_CODE','BH200','raw:F_CODE','BH200'], // Miscellaneous Surface Drainage Feature
        ['F_CODE','BH210','raw:F_CODE','BH210'], // Inland Shoreline
        ['F_CODE','BI041','raw:F_CODE','BI041'], // Gate (Nautical)
        ['F_CODE','BI043','raw:F_CODE','BI043'], // Flood Barrage
        ['F_CODE','BJ070','raw:F_CODE','BJ070'], // Pack Ice
        ['F_CODE','BK010','raw:F_CODE','BK010'], // Acoustic Station
        ['F_CODE','BK020','raw:F_CODE','BK020'], // Magnetic Station
        ['F_CODE','CA020','raw:F_CODE','CA020'], // Ridge Line
        ['F_CODE','CA035','raw:F_CODE','CA035'], // Inland Water Elevation
        ['F_CODE','DB030','raw:F_CODE','DB030'], // Cave
        ['F_CODE','DB060','raw:F_CODE','DB060'], // Crevice/Crevasse
        ['F_CODE','DB145','raw:F_CODE','DB145'], // Miscellaneous Obstacle
        ['F_CODE','DB200','raw:F_CODE','DB200'], // Gully/Gorge
        ['F_CODE','EC030','landuse','forest'], // Trees
        ['F_CODE','EE000','raw:F_CODE','EE000'], // Miscellaneous Vegetation
        ['F_CODE','EE020','raw:F_CODE','EE020'], // Land devoid of vegetation
        ['F_CODE','FA001','boundary','administrative'], // Administrative Area
        ['F_CODE','FA005','raw:F_CODE','FA005'], // Access Zone
        ['F_CODE','FA041','raw:F_CODE','FA041'], // Contact Zone
        ['F_CODE','FA090','raw:F_CODE','FA090'], // Geophysical Prospecting Grid
        ['F_CODE','FC021','raw:F_CODE','FC021'], // Maritime Limit Boundary
        ['F_CODE','FC031','raw:F_CODE','FC031'], // Maritime Area
        ['F_CODE','FC036','raw:F_CODE','FC036'], // Restricted Area
        ['F_CODE','FC041','raw:F_CODE','FC041'], // Traffic Separation Scheme (TSS)
        ['F_CODE','FC100','raw:F_CODE','FC100'], // Measured Distance Line
        ['F_CODE','FC101','raw:F_CODE','FC101'], // Theodolite Line
        ['F_CODE','FC102','raw:F_CODE','FC102'], // Range Centerline
        ['F_CODE','FC130','raw:F_CODE','FC130'], // Radar Reference Line
        ['F_CODE','FC165','raw:F_CODE','FC165'], // Route (Maritime)
        ['F_CODE','FC170','raw:F_CODE','FC170'], // Safety Fairway
        ['F_CODE','FC177','raw:F_CODE','FC177'], // Swept Area
        ['F_CODE','FC178','raw:F_CODE','FC178'], // Hull Integrity Test Area
        ['F_CODE','FC179','raw:F_CODE','FC179'], // Side Scan Sonar Coverage
        ['F_CODE','FC180','raw:F_CODE','FC180'], // Hull Integrity Test Site Bottom Feature
        ['F_CODE','GA005','raw:F_CODE','GA005'], // Airspace
        ['F_CODE','GA010','raw:F_CODE','GA010'], // ATS Route Segment/Leg
        ['F_CODE','GA015','raw:F_CODE','GA015'], // Special Use Airspace
        ['F_CODE','GA020','raw:F_CODE','GA020'], // Airspace Boundary Segment
        ['F_CODE','GA030','raw:F_CODE','GA030'], // Off Route Radial/Bearing
        ['F_CODE','GA031','raw:F_CODE','GA031'], // Lead Radial
        ['F_CODE','GA035','raw:F_CODE','GA035'], // NAVAIDS (Aeronautical)
        ['F_CODE','GA045','raw:F_CODE','GA045'], // Route (Air)
        ['F_CODE','GA047','raw:F_CODE','GA047'], // Complex Terminal Route
        ['F_CODE','GA055','raw:F_CODE','GA055'], // Waypoint/Reporting-Calling In Point
        ['F_CODE','GA065','raw:F_CODE','GA065'], // Air Warning Light
        ['F_CODE','GA400','raw:F_CODE','GA400'], // Airport Associated Strip
        ['F_CODE','GB010','raw:F_CODE','GB010'], // Airport Lighting
        ['F_CODE','GB025','raw:F_CODE','GB025'], // Blast Barrier
        ['F_CODE','GB047','raw:F_CODE','GB047'], // Aircraft Facility Reference Point
        ['F_CODE','GB057','raw:F_CODE','GB057'], // Shoulder
        ['F_CODE','GB060','raw:F_CODE','GB060'], // Runway Radar Reflector
        ['F_CODE','GB080','raw:F_CODE','GB080'], // Wind Indicator
        ['F_CODE','GB090','raw:F_CODE','GB090'], // Displaced Threshold
        ['F_CODE','GB100','raw:F_CODE','GB100'], // Air Facility Point To Point Communications
        ['F_CODE','GB101','raw:F_CODE','GB101'], // Air Facility Special Purpose Vehicless
        ['F_CODE','GB102','raw:F_CODE','GB102'], // Air Facility Aircraft Starting Units
        ['F_CODE','GB103','raw:F_CODE','GB103'], // Air Facility Garbage Disposal
        ['F_CODE','GB104','raw:F_CODE','GB104'], // Air Facility Sewage Disposal
        ['F_CODE','GB105','raw:F_CODE','GB105'], // Non- Site 30 Data Element
        ['F_CODE','GB106','raw:F_CODE','GB106'], // Air Facility Water Supply
        ['F_CODE','GB107','raw:F_CODE','GB107'], // Air Facility Terrain And Drainage
        ['F_CODE','GB108','raw:F_CODE','GB108'], // Air Facility Fuel Refueling/Dispensing Unit
        ['F_CODE','GB109','raw:F_CODE','GB109'], // Air Facility Location And Remarks
        ['F_CODE','GB111','raw:F_CODE','GB111'], // Air Facility On-Base Fuel Storage
        ['F_CODE','GB112','raw:F_CODE','GB112'], // Air Facility On-Base Ordnance Storage
        ['F_CODE','GB113','raw:F_CODE','GB113'], // Air Facility Security
        ['F_CODE','GB114','raw:F_CODE','GB114'], // Air Facility Vehicles
        ['F_CODE','GB115','raw:F_CODE','GB115'], // Air Facility Available Water Transportation
        ['F_CODE','GB116','raw:F_CODE','GB116'], // Aircraft Bunker
        ['F_CODE','GB160','raw:F_CODE','GB160'], // Decontamination Pad
        ['F_CODE','GB170','raw:F_CODE','GB170'], // INS Alignment Pad
        ['F_CODE','GB220','raw:F_CODE','GB220'], // Air Obstruction
        ['F_CODE','GB221','raw:F_CODE','GB221'], // Miscellaneous Air Obstruction
        ['F_CODE','SA050','raw:F_CODE','SA050'], // Slope Polygon
        ['F_CODE','SA060','raw:F_CODE','SA060'], // Covered Drainage
        ['F_CODE','SU002','raw:F_CODE','SU002'], // Subway
        ['F_CODE','ZB020','raw:F_CODE','ZB020'], // Benchmark
        ['F_CODE','ZB035','raw:F_CODE','ZB035'], // Control Point/Control Station
        ['F_CODE','ZB036','raw:F_CODE','ZB036'], // Distance Mark
        ['F_CODE','ZB040','raw:F_CODE','ZB040'], // Diagnostic Point
        ['F_CODE','ZB060','raw:F_CODE','ZB060'], // Geodetic Point
        ['F_CODE','ZB070','raw:F_CODE','ZB070'], // Precise Radar Significant Location
        ['F_CODE','ZC040','raw:F_CODE','ZC040'], // Magnetic Disturbance Area
        ['F_CODE','ZC050','raw:F_CODE','ZC050'], // Isogonic Lines
        ['F_CODE','ZD003','raw:F_CODE','ZD003'], // Artifact Location
        ['F_CODE','ZD012','raw:F_CODE','ZD012'], // Geographic Information Point
        ['F_CODE','ZD015','raw:F_CODE','ZD015'], // Point of Change
        ['F_CODE','ZD019','raw:F_CODE','ZD019'], // General Miscellaneous Feature
        ['F_CODE','ZD550','raw:F_CODE','ZD550'], // Geomorphic Location
        ['F_CODE','ZD551','raw:F_CODE','ZD551'] // Geomorphic Line
       ], // End fcodeOne2oneIn


    // One2one rules for Text Fields
    txtBiased : {
       'ACP':'raw:ACP', // Annotation Category Primary
       'AIA':'raw:AIA', // Airspace Identification Attribute
       'AKY':'aeroway:key', // Airport Key ## Check
       'BEN':'be_number', // Basic Encyclopedia Number
       'BER':'berth_identifier', // Berth Identifier
       'BRN':'bridge:reference', // Bridge Reference Number
       'BRR':'raw:BRR', // Bearing and Reciprocal Category
       'CAP':'raw:CAP', // Capacity
       'CCR':'raw:CCR', // Color Code Remarks
       'CDV':'calendar:date_value', // Calendar Date Value
       'CHL':'raw:CHL', // Channel Number
       'COL':'seamark:light:character', // Character of Light
       'COMMENTS':'source:comments', // Comments
       'COMM_CPYRT_NOTICE':'source:copyright', // Commercial Copyright Notice
       'COMM_LIC_TIER_NOTICE':'source:commercial_distribution_restriction', // Distribution of Commercial Data
       'CREATION_DATE':'source:creation_date', // Feature Creation Date
       'DAN':'navigationaid:description', // Description of Aids to Navigation
       'DRP':'reference_point:description', // Description of Reference Point
       'EHI':'raw:EHI', // Extracted Runway High Identifier
       'ELI':'raw:ELI', // Extracted Runway Low Identifier
       // 'F_CODE':'raw:F_CODE', // Feature Code
       'GFID':'uuid:gfid', // Global Feature ID
       'HTB':'raw:HTB', // Hull Integrity Test Site - Changes in bottom topography
       'HTE':'raw:HTE', // Date/Dates of Hull Integrity Test Site
       'HTS':'raw:HTS', // Hull Integrity Test Site - Special features
       'IKO':'icao', // ICAO Designator
       'LAB':'raw:LAB', // Label of Feature
       'MAA':'raw:MAA', // Maximum Authorized Altitude
       'MBI':'military:bridge_information', // Military Bridge Information
       'MCA':'raw:MCA', // Morse Code Attribute
       'MDU':'raw:MDU', // Maximum Demonstrated Usage
       'MEA':'raw:MEA', // Minimum Enroute Altitude
       'MLR':'seamark:light:ranges', // Multiple Light Ranges
       'M_ACP_9D066':'raw:M_ACP_9D066', // Hierarchy 9D066 Annotation Category Primary
       'M_AKY_1P030':'raw:M_AKY_1P030', // Hierarchy 1P030 AKY
       'M_AKY_1U030':'raw:M_AKY_1U030', // Hierarchy 1P030 AKY
       'M_BEN_9D066':'be_number:mcg', // Hierarchy 9D066 BEN
       'M_BRR_6C165':'raw:M_BRR_6C165', // Hierarchy 6C165 BRR
       'M_BRS_6C165':'raw:M_BRS_6C165', // Hierarchy 6C165 BRS
       'M_CDV_1U030':'raw:M_CDV_1U030', // (unnamed attribute)
       'M_DAN_6C165':'raw:M_DAN_6C165', // Hierarchy 6C165 DAN
       'M_DCF':'raw:M_DCF', // Data Comparator Flag
       'M_DCF_1P020':'raw:M_DCF_1P020', // Hierarchy 1P020 DCF
       'M_DCF_1P030':'raw:M_DCF_1P030', // Hierarchy 1P030 DCF
       'M_DCF_1P031':'raw:M_DCF_1P031', // Hierarchy 1P131 DCF
       'M_DCF_1Q100':'raw:M_DCF_1Q100', // Hierarchy 1Q100 DCF
       'M_DCF_1Q131':'raw:M_DCF_1Q131', // Hierarchy 1Q131 DCF
       'M_DCF_1U030':'raw:M_DCF_1U030', // Hierarchy 1U130 DCF
       'M_DCF_2H020':'raw:M_DCF_2H020', // Hierarchy 2H020 DCF
       'M_DCF_2H030':'raw:M_DCF_2H030', // Hierarchy 2H030 DCF
       'M_DCF_2H140':'raw:M_DCF_2H140', // Hierarchy 2H040 DCF
       'M_DCF_2H141':'raw:M_DCF_2H141', // Hierarchy 2H041 DCF
       'M_DCF_6C160':'raw:M_DCF_6C160', // Hierarchy 6C160 DCF
       'M_DCF_6C165':'raw:M_DCF_6C165', // Hierarchy 6C165 DCF
       'M_DCF_6C180':'raw:M_DCF_6C180', // Hierarchy 6C180 DCF
       'M_DCF_9D066':'raw:M_DCF_9D066', // Hierarchy 9D066 DCF
       'M_DRI_9D066':'raw:M_DRI_9D066', // Hierarchy 9D066 Data Reference Identifier
       'M_DRP_6C165':'raw:M_DRP_6C165', // Hierarchy 6C165 DRP
       'M_FRK_1P030':'raw:M_FRK_1P030', // Hierarchy 1P030 FRK
       'M_NAM_1P030':'raw:M_NAM_1P030', // Hierarchy 1P030 NAM
       'M_NAM_1U030':'raw:M_NAM_1U030', // Hierarchy 1U030 NAM
       'M_NAM_2H020':'raw:M_NAM_2H020', // Hierarchy 2H020 NAM
       'M_NAM_2H140':'raw:M_NAM_2H140', // Hierarchy 2H140 NAM
       'M_NAM_6C165':'raw:M_NAM_6C165', // Hierarchy 6C165 NAM
       'M_NM8_1U030':'raw:M_NM8_1U030', // (unnamed attribute)
       'M_PFL_9D066':'raw:M_PFL_9D066', // Hierarchy 9D066 Proper Name/Function/Location
       'M_RLN_9D066':'raw:M_RLN_9D066', // Hierarchy 9D066 RLN
       'M_RLT_9D066':'raw:M_RLT_9D066', // Hierarchy 9D066 RLT
       'M_SPL_9D066':'raw:M_SPL_9D066', // Hierarchy 9D066 SPL
       'M_UFI':'uuid:mcg', // Unique Feature ID (MC&G)
       'M_UFI_1P020':'raw:M_UFI_1P020', // Hierarchy 1U020 UFI
       'M_UFI_1P030':'uuid:mcg2', // Hierarchy 1U030 UFI
       'M_UFI_1P031':'uuid:mcg3', // Hierarchy 1U031 UFI
       'M_UFI_1Q100':'raw:M_UFI_1Q100', // Hierarchy 1Q100 UFI
       'M_UFI_1Q131':'raw:M_UFI_1Q131', // Hierarchy 1Q131 UFI
       'M_UFI_1U030':'raw:M_UFI_1U030', // Hierarchy 1U030 UFI
       'M_UFI_2H020':'raw:M_UFI_2H020', // Hierarchy 2H020 UFI
       'M_UFI_2H030':'raw:M_UFI_2H030', // Hierarchy 2H030 UFI
       'M_UFI_2H140':'raw:M_UFI_2H140', // Hierarchy 2H140 UFI
       'M_UFI_2H141':'raw:M_UFI_2H141', // Hierarchy 2H141 UFI
       'M_UFI_6C160':'raw:M_UFI_6C160', // Hierarchy 6C160 UFI
       'M_UFI_6C165':'raw:M_UFI_6C165', // Hierarchy 6C165 UFI
       'M_UFI_6C180':'raw:M_UFI_6C180', // Hierarchy 6C180 UFI
       'M_UFI_9D066':'raw:M_UFI_9D066', // Hierarchy 9D066 UFI
       'M_VDR_2H020':'raw:M_VDR_2H020', // Hierarchy 2H020 VDR
       'M_VDR_6C165':'raw:M_VDR_6C165', // Hierarchy 6C165 VDR
       'NA2':'alt_name', // Second Name
       'NA4':'is_in:country_code', // Country Code (FIPS Pub 10-4)
       'NA8':'official_name', // Primary Official Name Category ## Check
       'NAM':'name', // Name
       'NFI':'gndb_id:2', // Named Feature Identifier
       'NFN':'gndb_id', // Name Identifier
       'NM3':'name:three', // Name 3
       'NM4':'name:four', // Name 4
       'OPERATOR':'source:operator_id', // Operator ID
       'ORIGINATOR':'source:originator', // Agency that Supplied the Data
       'ORIG_SOURCE_DATE':'source:datetime', // Originating Source Date
       'ORIG_SOURCE_INFO':'source:description', // Originating Source Information
       'OTM':'raw:OTM', // Airspace Operating Times
       'QLE':'security:releasability', // Releasability ## Check
       'RID':'raw:RID', // Runway Identifier
       'RSTRN_DCLS_XMPT_CD':'security:declassification:exception', // Restriction Declassification Exemptions
       'RSTRN_DECLASS':'security:declassification:date', // Restriction Declassification
       'RSTRN_DSEM_CTRL_IC':'security:dissemination_control:ic', // Restrictions on Dissemination Control IC
       'RSTRN_DSEM_CTRL_NIC':'security:dissemination_control:non_ic', // Restrictions on Dissemination Control Non-IC
       'RSTRN_FOREIGN_GOV':'security:foreign_government_access', // Restrctions on Foreign Government Access
       'RSTRN_RELEASIBILITY':'security:releasability', // Restrictions on Releasibility
       'RTN':'ref', // Route Number
       'SEC_CD_CNTRL':'security:codeword_control', // Security Codeword Control
       'SEC_CLASS_SYS_ID':'security:classification:system_identifier', // Security Classification System Identifier
       'SEC_CLS_AUTH_DESC':'security:classification:non_standard_authority', // Non-standard Classification Authority
       'SEC_CLS_AUTH_MULT':'security:classification:multiple_authority', // Multiple Classification Authority Sources
       'SUA':'raw:SUA', // Special Use Airspace Altitude Limits
       'SUE':'raw:SUE', // Survey Date - End
       'SVR':'raw:SVR', // Special Purpose Vehicle Remarks
       'TMS':'raw:TMS', // Extraction Timestamp
       'TSR':'raw:TSR', // Tailored Surface Roughness Description
       'TXR':'raw:TXR', // Taxiway Remarks
       'TXT':'note', // Text Attribute
       'UPDATE_SOURCE_DATE':'source:update:datetime', // Update/Review Source Date
       'UPDATE_SOURCE_INFO':'source:update:description', // Update/Review Source Information
       'VA2':'raw:VA2', // First magnetic variation value
       'VA3':'raw:VA3', // Second magnetic variation value
       'VA4':'raw:VA4', // Variation Anomaly String
       'VCR':'safe_clearance:vertical:remarks', // Vertical Clearance Remarks
       'VDR':'source:datum:sounding:name', // Vertical Datum Record ## Check
       'VOI':'aeroway:obstruction', // Vertical Obstruction Identifier
       'WPI':'world_port_index', // Port Index
       }, // End txtBiased

    // One2one ruled for Number fields
    numBiased : {
       'ACE':'source:accuracy:horizontal', // Absolute Circular Error ## ReDo
       'AGL':'raw:AGL', // Arresting Gear Location
       'AHA':'source:accuracy:horizontal', // Absolute Horizontal Accuracy in Meters
       'AHO':'source:accuracy:obstruction_height', // Accuracy of Obstruction Height Above Ground Level
       'ALE':'source:accuracy:linear', // Absolute Linear Error
       'AOO':'angle', // Angle of Orientation
       'ARE_':'feature_area', // Area With Greater Than 1 Meter Squared Resolution
       'ARR':'raw:ARR', // Angle of Radar Reflector
       'AV1':'raw:AV1', // Lowest Airspace Height
       'AV2':'raw:AV2', // Highest Airspace Height
       'AVA':'source:accuracy:vertical', // Absolute Vertical Accuracy in Meters
       'AZ1':'raw:AZ1', // Lowest Airspace Z-value
       'AZ2':'raw:AZ2', // Highest Airspace Z-value
       'AZ3':'raw:AZ3', // Minimum Safe Altitude Sector
       'BGL':'raw:BGL', // Bank Gradient Left
       'BGR':'raw:BGR', // Bank Gradient Right
       'BHL':'waterway:bank1:height', // Bank Height Left
       'BHR':'waterway:bank2:height', // Bank Height Right
       'BNF':'building:levels', // Building Number of Floors
       'BR2':'seamark:radio_station:frequency:2', // Broadcast Frequency (2)
       'BRF':'seamark:radio_station:frequency', // Broadcast Frequency
       'BRG':'bearing:to_object', // Bearing of Object
       'BRS':'bearing:from_seaward', // Bearing From Seaward
       'C01':'raw:C01', // Fuel Containers Capacity Each
       'C60':'raw:C60', // Rate of Current (IHO)
       'C61':'raw:C61', // Rate of Current (1) (IHO)
       'C62':'raw:C62', // Rate of Current (2) (IHO)
       'C63':'raw:C63', // Rate of Current (3) (IHO)
       'C64':'raw:C64', // Rate of Current (4) (IHO)
       'C65':'raw:C65', // Rate of Current (5) (IHO)
       'C66':'raw:C66', // Rate of Current (6) (IHO)
       'C67':'raw:C67', // Rate of Current (7) (IHO)
       'C68':'raw:C68', // Rate of Current (8) (IHO)
       'C69':'raw:C69', // Rate of Current (9) (IHO)
       'C70':'raw:C70', // Rate of Current (10) (IHO)
       'C71':'raw:C71', // Rate of Current (11) (IHO)
       'CDL':'covered_drain:length', // Covered Drain Length
       'CFD':'raw:CFD', // Cultural Feature Density
       'CRN':'water:current:speed:min', // Current Rate Minimum
       'CRV':'depth:value', // Depth Curve or Contour Value
       'CRX':'water:current:speed:max', // Current Rate Maximum
       'CSD':'raw:CSD', // Crane Support Distance
       'CTL':'railway:track:length', // Cumulative Track Length
       'CVH':'depth:value:max', // Depth Curve or Contour Value - High
       'CVL':'depth:value:min', // Depth Curve or Contour Value - Low
       'D60':'raw:D60', // Direction of Current (IHO)
       'D61':'raw:D61', // Direction of Current (1) (IHO)
       'D62':'raw:D62', // Direction of Current (2) (IHO)
       'D63':'raw:D63', // Direction of Current (3) (IHO)
       'D64':'raw:D64', // Direction of Current (4) (IHO)
       'D65':'raw:D65', // Direction of Current (5) (IHO)
       'D66':'raw:D66', // Direction of Current (6) (IHO)
       'D67':'raw:D67', // Direction of Current (7) (IHO)
       'D68':'raw:D68', // Direction of Current (8) (IHO)
       'D69':'raw:D69', // Direction of Current (9) (IHO)
       'D70':'raw:D70', // Direction of Current (10) (IHO)
       'D71':'raw:D71', // Direction of Current (11) (IHO)
       'DDM':'raw:DDM', // Ship's Draft
       'DEP':'depth:max', // Depth Below Surface Level
       'DF1':'seamark:calling-in_point:traffic_flow:direction:1', // Direction of Traffic - 1
       'DF2':'seamark:calling-in_point:traffic_flow:direction:2', // Direction of Traffic - 2
       'DF3':'seamark:calling-in_point:traffic_flow:direction:3', // Direction of Traffic - 3
       'DF4':'seamark:calling-in_point:traffic_flow:direction:4', // Direction of Traffic - 4
       'DMB':'undergrowth:density', // Density Measure (Brush/Undergrowth)
       'DMF':'feature_count', // Density Measure (Feature Count)
       'DMK':'raw:DMK', // Density Measure (% of Kelp Cover)
       'DMR':'raw:DMR', // Density Measure (% of Roof Cover)
       'DMS':'raw:DMS', // Density Measure (Structure Count)
       'DMT':'canopy_cover', // Density Measure (% of Tree/Canopy Cover)
       'DOF':'flow_direction', // Direction of Flow
       'DRN':'raw:DRN', // Depth Recorder Name
       'E01':'raw:E01', // Number Of Fuel Fill Stands
       'EDP':'raw:EDP', // Electronic Depth
       'ENT':'raw:ENT', // Apron/Hardstand Entry Clearance
       'EOL':'seamark:light:height', // Elevation of Light
       'FCL':'ferry:crossing_distance', // Ferry Crossing Length
       'FEO':'raw:FEO', // Feature Element Orientation
       'FHH':'raw:FHH', // True Feature Heading (High End)
       'FHL':'raw:FHL', // True Feature Heading (Low End)
       'FL1':'raw:FL1', // Flight Level 1
       'FL2':'raw:FL2', // Flight Level 2
       'GAW':'gauge', // Gauge Width
       'GEH':'raw:GEH', // Geomorphic Height
       'GHE':'raw:GHE', // Greatest Horizontal Extent
       'GPD':'raw:GPD', // Geomorphic Depth
       'HAS':'raw:HAS', // Height Above Surrounding Features
       'HCA':'horizontal:clearance', // Horizontal Clearance Attribute
       'HDH':'hydrographic_drying_height', // Hydrographic Drying Height
       'HDP':'depth:hydrographic', // Hydrographic Depth
       'HDW':'raw:HDW', // Hangar Door Width
       'HGS':'raw:HGS', // Height Of Spillway
       'HGT':'height', // Height Above Surface Level
       'HNG':'raw:HNG', // Hangar Door Height
       'HSB':'height:above_sea_bottom', // Height Above Sea Bottom
       'HTD':'raw:HTD', // Hull Integrity Test Site Direction of slope of test site
       'HTG':'raw:HTG', // Hull Integrity Test Site Slope Gradient
       'HTH':'raw:HTH', // Maximum height of obstacles in a Hull Integrity Test Site
       'IDN':'identification_number', // Identification Number
       'KVA':'voltage', // Kilovolt Capacity Attribute
       'LC1':'mlc:wheeled_oneway', // Load Class Type 1
       'LC2':'mlc:wheeled', // Load Class Type 2
       'LC3':'mlc:tracked_oneway', // Load Class Type 3
       'LC4':'mlc:tracked', // Load Class Type 4
       'LEG':'length', // Length/Diameter in Meters With Greater Precision
       'LEN_':'length', // Length/Diameter
       'LNU':'length:interior_useable', // Useable Length
       'LOG':'gradient:length', // Length of Gradient
       'LRP':'raw:LRP', // Length of Range With greater than 1 NM resolution
       'LSA':'seamark:light:sector:angle', // Light Sector Angle
       'LSI':'raw:LSI', // Light Sector Angle Initial
       'LST':'raw:LST', // Light Sector Angle Terminal
       'LTN':'lanes', // Track/Lane Number
       'LVN':'seamark:light:radius', // Light Range, Nominal
       'MAG':'magnetic_variation', // Magnetic Variation
       'MTN':'raw:MTN', // Mine Track Number
       'MVC':'bridge:under_clearance', // Maximum Vertical Clearance ## Check
       'MWG':'divider:width', // Median Width with greater than 1 meter resolution
       'M_ACE_1P020':'raw:M_ACE_1P020', // Absolute Circular Error of Feature Position
       'M_ACE_1P030':'source:accuracy:horizontal', // Absolute Circular Error of Feature Position
       'M_ACE_1P031':'source:accuracy:horizontal2', // Absolute Circular Error of Feature Position
       'M_ACE_1Q100':'raw:M_ACE_1Q100', // Absolute Circular Error of Feature Position
       'M_ACE_1Q131':'raw:M_ACE_1Q131', // Absolute Circular Error of Feature Position
       'M_ACE_1U030':'raw:M_ACE_1U030', // Absolute Circular Error of Feature Position
       'M_ACE_2H020':'raw:M_ACE_2H020', // Absolute Circular Error of Feature Position
       'M_ACE_2H030':'raw:M_ACE_2H030', // Absolute Circular Error of Feature Position
       'M_ACE_2H140':'raw:M_ACE_2H140', // Absolute Circular Error of Feature Position
       'M_ACE_2H141':'raw:M_ACE_2H141', // Absolute Circular Error of Feature Position
       'M_ACE_6C160':'raw:M_ACE_6C160', // Absolute Circular Error of Feature Position
       'M_ACE_6C165':'raw:M_ACE_6C165', // Absolute Circular Error of Feature Position
       'M_ACE_6C180':'raw:M_ACE_6C180', // Absolute Circular Error of Feature Position
       'M_ACE_9D066':'source:accuracy:horizontal', // Absolute Circular Error of Feature Position
       'M_ALE_1P020':'raw:M_ALE_1P020', // Absolute Linear Error of Feature Position
       'M_ALE_1P030':'source:accuracy:linear', // Absolute Linear Error of Feature Position
       'M_ALE_1P031':'source:accuracy:linear2', // Absolute Linear Error of Feature Position
       'M_ALE_1Q100':'raw:M_ALE_1Q100', // Absolute Linear Error of Feature Position
       'M_ALE_1Q131':'raw:M_ALE_1Q131', // Absolute Linear Error of Feature Position
       'M_ALE_1U030':'raw:M_ALE_1U030', // Absolute Linear Error of Feature Position
       'M_ALE_2H020':'raw:M_ALE_2H020', // Absolute Linear Error of Feature Position
       'M_ALE_2H030':'raw:M_ALE_2H030', // Absolute Linear Error of Feature Position
       'M_ALE_2H140':'raw:M_ALE_2H140', // Absolute Linear Error of Feature Position
       'M_ALE_2H141':'raw:M_ALE_2H141', // Absolute Linear Error of Feature Position
       'M_ALE_6C160':'raw:M_ALE_6C160', // Absolute Linear Error of Feature Position
       'M_ALE_6C165':'raw:M_ALE_6C165', // Absolute Linear Error of Feature Position
       'M_ALE_6C180':'raw:M_ALE_6C180', // Absolute Linear Error of Feature Position
       'M_ALE_9D066':'source:accuracy:linear', // Absolute Linear Error of Feature Position
       'M_DFS_1P020':'raw:M_DFS_1P020', // Hierarchy 1P020 DFS
       'M_DOF_6C165':'raw:M_DOF_6C165', // Hierarchy 6C165 DOF
       'M_DOF_6C180':'raw:M_DOF_6C180', // Hierarchy 6C180 DOF
       'M_DVA_1Q100':'raw:M_DVA_1Q100', // Hierarchy 1Q100 Distance Value Attribute
       'M_HDP_2H020':'raw:M_HDP_2H020', // Hierarchy 2H020 HDP
       'M_HDP_6C165':'raw:M_HDP_6C165', // Hierarchy 6C165 HDP
       'M_SGC_1P020':'raw:M_SGC_1P020', // Hierarchy 1P020 SGC
       'M_TMS_6C160':'raw:M_TMS_6C160', // Hierarchy 6C160 TMS
       'M_TMS_6C165':'raw:M_TMS_6C165', // Hierarchy 6C165 TMS
       'M_TMS_6C180':'raw:M_TMS_6C180', // Hierarchy 6C180TMS
       'M_UQI_6C160':'raw:M_UQI_6C160', // Hierarchy 6C160 UQI
       'M_UQI_6C165':'raw:M_UQI_6C165', // Hierarchy 6C165 UQI
       'M_UQI_6C180':'raw:M_UQI_6C180', // Hierarchy 6C180 UQI
       'M_WID_1P020':'raw:M_WID_1P020', // Hierarchy 1P020 WID
       'M_WID_6C180':'raw:M_WID_6C180', // Hierarchy 6C180 WID
       'M_ZV2_2H141':'raw:M_ZV2_2H141', // Hierarchy 2H141 ZV2
       'M_ZV2_9D066':'raw:M_ZV2_9D066', // Hierarchy 9D066 ZV2
       'NOS':'bridge:span_count', // Number of Spans
       'NPL':'cables', // Number of Parallel Lines
       'NSN':'raw:NSN', // Navigation System Name
       'OHB':'height', // Overall Height of Bridge
       'OHC':'max_clearance', // Overhead Clearance Category
       'OR2':'seamark:radio_station:range:2', // Operating Range Category (2)
       'ORC':'seamark:radio_station:range', // Operating Range Category
       'PER':'seamark:light:period', // Period of Light
       'PFE':'depth:predominant', // Predominant Feature Depth With greater than 1 meter resolution
       'PFG':'height:predominant', // Predominant Feature Height With greater than 1 meter resolution
       'PHT':'height', // Predominant Height
       'PPL':'raw:PPL', // Populated Place Category
       'RAD':'curve_radius', // Radius of Sharp Curve
       'RTH':'raw:RTH', // Runway True Heading- High
       'RTL':'raw:RTL', // Runway True heading - Low
       'SCALE':'source:scale', // Feature Scale (e.g., 50000, 100000)
       'SDO':'dune:orientation', // Sand Dune Orientation
       'SDR':'tree:diameter', // Stem Diameter Size With greater than 1 meter resolution
       'SGC':'incline', // Gradient/Slope
       'SHC':'safe_clearance:horizontal', // Safe Horizontal Clearance
       'SIZE_':'raw:SIZE_', // Font Size
       'SPD':'maxspeed', // Speed Limit (MPH)
       'SSI':'raw:SSI', // Sounding Source ID
       'TIM':'activity_duration', // Time Attribute
       'TNG':'raw:TNG', // Tonnage
       'TSD':'raw:TSD', // Tree Spacing With greater than 1 meter resolution
       'UBC':'bridge:under_clearance', // Underbridge Clearance Category
       'UBD':'bridge:under_clearance:precise', // Underbridge Clearance With Greater Precision
       'UID_':'uuid', // Feature Identification Number
       'USI':'raw:USI', // BAD Sounding Identifier
       'VAL':'value', // Value
       'VAV':'magnetic_variation', // Variation Anomaly Value
       'VC3':'safe_clearance:vertical', // Vertical Clearance, Safe With greater than 1 meter resolution
       'WD1':'width:minimum_traveled_way', // Minimum Traveled Way Width
       'WD2':'width:total_usable', // Total Usable Width
       'WD3':'width:gap', // Military Gap Width
       'WD5':'width:top', // Width Top
       'WDA':'depth:average', // Water Depth Average
       'WDU':'width:interior_useable', // Useable Width
       'WGP':'width:precise', // Width with greater than 1 meter resolution
       'WID':'width', // Width
       'WMS':'raw:WMS', // Movement Surface Width
       'WOC':'width:crest', // Width of Crest
       'WT2':'width:second_way', // Width of Second Traveled Way
       'ZV2':'ele', // Highest Z-Value
       'ZV3':'ele', // Airfield/Aerodrome elevation
       }, // End numBiased
    

    // Common one2one rules. Used for both import and export
    one2one : [
         // AAI - Horizontal Accuracy Category
       ['AAI','12','raw:AAI','<=0.001_second'], // <=0.001 second
       ['AAI','13','raw:AAI','0.001_-_0.1_second'], // 0.001 - 0.1 second
       ['AAI','14','raw:AAI','0.1_-_1.0_second'], // 0.1 - 1.0 second
       ['AAI','15','raw:AAI','1.0_-_10.0_seconds'], // 1.0 - 10.0 seconds
       ['AAI','16','raw:AAI','10.0_-_60.0_seconds'], // 10.0 - 60.0 seconds

       // AAW - Vertical Accuracy Category
       ['AAW','1','raw:AAW','<=10_feet'], // <=10 feet
       ['AAW','2','raw:AAW','11_-_30_feet'], // 11 - 30 feet
       ['AAW','3','raw:AAW','31_-_100_feet'], // 31 - 100 feet
       ['AAW','4','raw:AAW','>100_feet'], // >100 feet

       // ACC - Accuracy Category
       // ['ACC','0','source:accuracy:horizontal:category','unknown'], // Unknown
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

       // ACL - Accuracy Left Bank
       ['ACL','0','raw:ACL','unknown'], // Unknown
       ['ACL','1','raw:ACL','accurate'], // Accurate
       ['ACL','2','raw:ACL','approximate'], // Approximate

       // ACR - Accuracy Right Bank
       ['ACR','0',undefined,undefined], // Unknown
       ['ACR','1','raw:ACR','accurate'], // Accurate
       ['ACR','2','raw:ACR','approximate'], // Approximate

       // AFA - Available Facilities
       // ['AFA','0','available_service','unknown'], // Unknown
       ['AFA','19','available_service','helipad'], // Helipad
       ['AFA','995','available_service','none'], // None
       ['AFA','999','available_service','other'], // Other

       // AGC - Arresting Gear Category
       ['AGC','0','raw:AGC','unknown'], // Unknown
       ['AGC','1','raw:AGC','net'], // Net
       ['AGC','2','raw:AGC','cable'], // Cable
       ['AGC','6','raw:AGC','jet_barrier'], // Jet Barrier
       ['AGC','997',undefined,undefined], // Unpopulated
       ['AGC','999','raw:AGC','other'], // Other

       // AGD - (unnamed attribute)
       ['AGD','0','raw:AGD','unknown'], // Unknown
       ['AGD','1','raw:AGD','uni-directional_(in_direction_of_high_end)'], // Uni-directional (in direction of high end)
       ['AGD','2','raw:AGD','uni-directional_(in_direction_of_low_end)'], // Uni-directional (in direction of low end)
       ['AGD','3','raw:AGD','bi-directional'], // Bi-directional
       ['AGD','997',undefined,undefined], // Unpopulated

       // AHC - Associated Hydrographic Category 
       // NOTE: Deconflict with HYC 
       ['AHC','0','hydrographic_category:2','unknown'], // Unknown
       ['AHC','1','hydrographic_category:2','perennial'], // Perennial
       ['AHC','2','hydrographic_category:2','intermittent'], // Intermittent
       ['AHC','3','hydrographic_category:2','ephemeral'], // Ephemeral
       ['AHC','997',undefined,undefined], // Unpopulated

       // ALE_EVAL_METHOD_CD - Absolute Linear Error Evaluation Method
       ['ALE_EVAL_METHOD_CD','ACA','source:accuracy:linear:evaluation','geodetic_survey_control_-_adequate_sample'], // Geodetic Survey Control - adequate sample
       ['ALE_EVAL_METHOD_CD','ACS','source:accuracy:linear:evaluation','geodetic_survey_control_-_small_sample'], // Geodetic Survey Control - small sample
       ['ALE_EVAL_METHOD_CD','AHA','source:accuracy:linear:evaluation','photogrammetric_(old_stats,_unable_to_convert_to_ana_due_to_significant_bias)'], // Photogrammetric (old stats, unable to convert to ANA due to significant bias)
       ['ALE_EVAL_METHOD_CD','ANA','source:accuracy:linear:evaluation','photogrammetric_control_(gdasii)_-_adequate_sample'], // Photogrammetric Control (GDASII) - adequate sample
       ['ALE_EVAL_METHOD_CD','ANS','source:accuracy:linear:evaluation','photogrammetric_control_(gdasii)_-_small_sample'], // Photogrammetric Control (GDASII) - small sample
       ['ALE_EVAL_METHOD_CD','APA','source:accuracy:linear:evaluation','photogrammetric_control_(iec,_fpe,_npf)_-_adequate_sample'], // Photogrammetric Control (IEC, FPE, NPF) - adequate sample
       ['ALE_EVAL_METHOD_CD','APS','source:accuracy:linear:evaluation','photogrammetric_control_(iec,_fpe,_npf)_-_small_sample'], // Photogrammetric Control (IEC, FPE, NPF) - small sample
       ['ALE_EVAL_METHOD_CD','AXB','source:accuracy:linear:evaluation','extrapolation'], // Extrapolation
       ['ALE_EVAL_METHOD_CD','AZD','source:accuracy:linear:evaluation','unable_to_perform_metric_evaluation,_other_than_ezm,_ezg,_azf,_and_aze_(assumed_adequate)'], // Unable to perform metric evaluation, other than EZM, EZG, AZF, and AZE (assumed adequate)
       ['ALE_EVAL_METHOD_CD','AZE','source:accuracy:linear:evaluation','insufficient_control_presently_available_to_perform_metric_evaluation_(assumed_adequate)'], // Insufficient control presently available to perform metric evaluation (assumed adequate)
       ['ALE_EVAL_METHOD_CD','AZF','source:accuracy:linear:evaluation','insufficient_identifiable_map_features_for_metric_evaluation_(assumed_adequate)'], // Insufficient identifiable map features for metric evaluation (assumed adequate)
       ['ALE_EVAL_METHOD_CD','BMD','source:accuracy:linear:evaluation','error_budget_evaluation_(production_data)'], // Error Budget Evaluation (Production Data)
       ['ALE_EVAL_METHOD_CD','CLD','source:accuracy:linear:evaluation','accuracy_evaluation_printed_on_the_sheet_or_evaluation_derived_from_source_accuracy'], // Accuracy evaluation printed on the sheet or evaluation derived from source accuracy
       ['ALE_EVAL_METHOD_CD','CXB','source:accuracy:linear:evaluation','results_from_a_large_sampling_of_post-production_evaluations_that_are_similar_in_compilation_or_specification'], // Results from a large sampling of post-production evaluations that are similar in compilation or specification
       ['ALE_EVAL_METHOD_CD','EMC','source:accuracy:linear:evaluation','product_specification_accuracy_value_(assumed_adequate)'], // Product specification accuracy value (assumed adequate)
       ['ALE_EVAL_METHOD_CD','EYC','source:accuracy:linear:evaluation','very_old_evaluation_of_unknown_origin'], // Very old evaluation of unknown origin
       ['ALE_EVAL_METHOD_CD','EZD','source:accuracy:linear:evaluation','estimate_based_on_analyst_familiarity'], // Estimate based on analyst familiarity
       ['ALE_EVAL_METHOD_CD','EZG','source:accuracy:linear:evaluation','insufficient_graticule_for_metric_evaluation_(assumed_limited)'], // Insufficient graticule for metric evaluation (assumed limited)
       ['ALE_EVAL_METHOD_CD','EZM','source:accuracy:linear:evaluation','scale_too_small_(<1:300,000)_to_perform_metric_evaluation_on_gdas_systems_(assumed_adequate)'], // Scale too small (<1:300,000) to perform metric evaluation on GDAS systems (assumed adequate)
       ['ALE_EVAL_METHOD_CD','EZZ','source:accuracy:linear:evaluation','digital_product,_no_evaluation_method_available'], // Digital Product, no evaluation method available
       ['ALE_EVAL_METHOD_CD','FZD','source:accuracy:linear:evaluation','evaluation_deferred_(no_measurement)'], // Evaluation deferred (no measurement)

       // APT - Airfield Type
       ['APT','0',undefined,undefined], // Unknown
       ['APT','1','aeroway:use','major_airfield'], // Major Airfield
       ['APT','2','aeroway:use','minor_airfield'], // Minor Airfield
       ['APT','9','aeroway','heliport'], // Heliport
       ['APT','11','aeroway:use','heliport_at_hospitals'], // Heliport at Hospitals
       ['APT','14','aeroway:use','airport/airfield'], // Airport/Airfield
       ['APT','15','aeroway:use','undefined_landing_area'], // Undefined Landing Area
       ['APT','997',undefined,undefined], // Unpopulated
       ['APT','999','aeroway:use','other'], // Other

       // ARC - Artifact Code
       ['ARC','1','raw:ARC','source_material_adverse_area'], // Source Material Adverse Area
       ['ARC','2','raw:ARC','extraction_spec._delineation_set_difference'], // Extraction Spec. Delineation Set Difference
       ['ARC','3','raw:ARC','source_material_boundary'], // Source Material Boundary
       ['ARC','4','raw:ARC','missing_data'], // Missing Data
       ['ARC','999','raw:ARC','other'], // Other

       // ARD - Artifact Description
       ['ARD','1','raw:ARD','positional'], // Positional
       ['ARD','2','raw:ARD','attributional'], // Attributional
       ['ARD','3','raw:ARD','both_positional_and_attributional'], // Both Positional And Attributional

       // AS1 - Airport Strip Type
       ['AS1','1','raw:AS1','none'], // None
       ['AS1','2','raw:AS1','highway_strip'], // Highway Strip
       ['AS1','3','raw:AS1','ice_strip'], // Ice Strip
       ['AS1','4','raw:AS1','snow_strip'], // Snow Strip
       ['AS1','5','raw:AS1','decoy'], // Decoy
       ['AS1','997',undefined,undefined], // Unpopulated
       ['AS1','999','raw:AS1','other'], // Other

       // ATC - Aqueduct Type Category
       ['ATC','0','raw:ATC','unknown'], // Unknown
       ['ATC','1','raw:ATC','qanat/kanat/karez_shaft'], // Qanat/Kanat/Karez Shaft
       ['ATC','3','raw:ATC','underground_aqueduct'], // Underground Aqueduct
       ['ATC','999','raw:ATC','other'], // Other

       // ATL - ATS Route Level
       ['ATL','0','raw:ATL','unknown'], // Unknown
       ['ATL','1','raw:ATL','both'], // Both
       ['ATL','2','raw:ATL','high_level_(fl195)'], // High Level (FL195)
       ['ATL','3','raw:ATL','low_level_(fl195)'], // Low Level (FL195)

       // ATN - Aids to Navigation
       ['ATN','0','navigation_aid','unknown'], // Unknown
       ['ATN','1','navigation_aid','marked'], // Marked
       ['ATN','2','navigation_aid','unmarked'], // Unmarked
       ['ATN','3','navigation_aid','lit'], // Lit
       ['ATN','4','navigation_aid','unlit'], // Unlit
       ['ATN','997',undefined,undefined], // Unpopulated

       // AUA - ATS Use Attribute
       ['AUA','0','airspace:type','unknown'], // Unknown
       ['AUA','1','airspace:type','advisory_area_(ada)'], // Advisory Area (ADA)
       ['AUA','2','airspace:type','air_defense_identification_zone_(adiz)'], // Air Defense Identification Zone (ADIZ)
       ['AUA','3','airspace:type','air_route_traffic_control_center_(artcc)'], // Air Route Traffic Control Center (ARTCC)
       ['AUA','4','airspace:type','alert_area'], // Alert Area
       ['AUA','5','airspace:type','area_control_center_(acc)'], // Area Control Center (ACC)
       ['AUA','6','airspace:type','buffer_zone_(bz)'], // Buffer Zone (BZ)
       ['AUA','7','airspace:type','canadian_air_defense_identification_zone_(cadiz)'], // Canadian Air Defense Identification Zone (CADIZ)
       ['AUA','8','airspace:type','control_area_(cta)'], // Control Area (CTA)
       ['AUA','9','airspace:type','control_zone_(ctlz)'], // Control Zone (CTLZ)
       ['AUA','10','airspace:type','danger_area'], // Danger Area
       ['AUA','11','airspace:type','dew_east_military_identification_zone_(demiz)'], // Dew East Military Identification Zone (DEMIZ)
       ['AUA','12','airspace:type','distant_early_warning_identification_zone_(dewiz)'], // Distant Early Warning Identification Zone (DEWIZ)
       ['AUA','13','airspace:type','flight_information_region_(fir)'], // Flight Information Region (FIR)
       ['AUA','14','airspace:type','french_peripheral_identification_zone_(lip)'], // French Peripheral Identification Zone (LIP)
       ['AUA','15','airspace:type','military_aerodrome_traffic_zone_(maiz)'], // Military Aerodrome Traffic Zone (MAIZ)
       ['AUA','16','airspace:type','military_common_area_control_(mcac)'], // Military Common Area Control (MCAC)
       ['AUA','17','airspace:type','military_climb_corridor_(mcc)'], // Military Climb Corridor (MCC)
       ['AUA','18','airspace:type','military_flying_area_(canada,_mfa)'], // Military Flying Area (Canada, MFA)
       ['AUA','19','airspace:type','mid-canada_identification_zone_(midiz)'], // Mid-Canada Identification Zone (MIDIZ)
       ['AUA','20','airspace:type','military_operations_area_(moa)'], // Military Operations Area (MOA)
       ['AUA','21','airspace:type','military_terminal_control_area_(mtca)'], // Military Terminal Control Area (MTCA)
       ['AUA','22','airspace:type','military_upper_control_area_(muca)'], // Military Upper Control Area (MUCA)
       ['AUA','23','airspace:type','oceanic_control_area_(non-faa)_(oca)'], // Oceanic Control Area (non-FAA) (OCA)
       ['AUA','24','airspace:type','operating_area_(oparea)'], // Operating Area (OPAREA)
       ['AUA','25','airspace:type','prohibited_area'], // Prohibited Area
       ['AUA','26','airspace:type','positive_control_area_(pca)'], // Positive Control Area (PCA)
       ['AUA','27','airspace:type','positive_control_zone_(pcz)'], // Positive Control Zone (PCZ)
       ['AUA','28','airspace:type','radar_area'], // Radar Area
       ['AUA','29','airspace:type','restricted_area'], // Restricted Area
       ['AUA','30','airspace:type','security_identification_zone_(siz)'], // Security Identification Zone (SIZ)
       ['AUA','31','airspace:type','special_air_traffic_rules_area'], // Special Air Traffic Rules Area
       ['AUA','32','airspace:type','special_rules_zone'], // Special Rules Zone
       ['AUA','33','airspace:type','transition_area_(for_chart_use_only_-_ta)'], // Transition Area (For Chart Use Only - TA)
       ['AUA','34','airspace:type','terminal_control_area_(tca)'], // Terminal Control Area (TCA)
       ['AUA','35','airspace:type','continental_control_area_(cca)'], // Continental Control Area (CCA)
       ['AUA','36','airspace:type','special_operations_area_(air)'], // Special Operations Area (Air)
       ['AUA','37','airspace:type','terminal_radar_service_area_(trsa)'], // Terminal Radar Service Area (TRSA)
       ['AUA','38','airspace:type','upper_advisory_area_(uda)'], // Upper Advisory Area (UDA)
       ['AUA','39','airspace:type','upper_control_area_(uta)'], // Upper Control Area (UTA)
       ['AUA','40','airspace:type','upper_flight_information_region_(uir)'], // Upper Flight Information Region (UIR)
       ['AUA','41','airspace:type','warning_area'], // Warning Area
       ['AUA','42','airspace:type','zone_of_interior_(zi)'], // Zone of Interior (ZI)
       ['AUA','44','airspace:type','korea_limited_identification_zone_(kliz)'], // Korea Limited Identification Zone (KLIZ)
       ['AUA','45','airspace:type','uncontrolled_airspace'], // Uncontrolled Airspace
       ['AUA','46','airspace:type','controlled_airspace'], // Controlled Airspace
       ['AUA','47','airspace:type','airport_traffic_area_(ata)'], // Airport Traffic Area (ATA)
       ['AUA','48','airspace:type','airport_radar_service_area_(arsa)'], // Airport Radar Service Area (ARSA)
       ['AUA','49','airspace:type','controlled_firing_area'], // Controlled Firing Area
       ['AUA','50','airspace:type','parachute_jump_area'], // Parachute Jump Area
       ['AUA','51','airspace:type','airport_advisory_area'], // Airport Advisory Area
       ['AUA','52','airspace:type','designated_mountainous_area'], // Designated Mountainous Area
       ['AUA','54','airspace:type','non-free_flying_area'], // Non-Free Flying Area
       ['AUA','55','airspace:type','control_zone_-_no_fixed_wing_special_vfr_permitted'], // Control Zone - No Fixed Wing Special VFR Permitted
       ['AUA','56','airspace:type','altimeter_change_boundary'], // Altimeter Change Boundary
       ['AUA','57','airspace:type','defense_area'], // Defense Area
       ['AUA','58','airspace:type','aerodrome_control_zone'], // Aerodrome Control Zone
       ['AUA','59','airspace:type','class_c_control_zone'], // Class C Control Zone
       ['AUA','60','airspace:type','sparsely_settled_area'], // Sparsely Settled Area
       ['AUA','61','airspace:type','northern_domestic_airspace'], // Northern Domestic Airspace
       ['AUA','62','airspace:type','icao'], // ICAO
       ['AUA','63','airspace:type','upper_airspace_centers_operational_air_traffic'], // Upper Airspace Centers Operational Air Traffic
       ['AUA','64','airspace:type','controlled_visual_flight_rules_(cvfr)'], // Controlled Visual Flight Rules (CVFR)
       ['AUA','65','airspace:type','bird_hazard_areas'], // Bird Hazard Areas
       ['AUA','66','airspace:type','temporary_reserved_airspace_(tra)'], // Temporary Reserved Airspace (TRA)
       ['AUA','67','airspace:type','air_route_traffic_control_center_sector_or_discrete_sector'], // Air Route Traffic Control Center Sector or Discrete Sector
       ['AUA','68','airspace:type','sub-flight_information_region_(sub_fir)'], // Sub-Flight Information Region (SUB FIR)
       ['AUA','69','airspace:type','radar_area_sector_boundary'], // Radar Area Sector Boundary
       ['AUA','70','airspace:type','oceanic_control_area_(faa)_(oca)'], // Oceanic Control Area (FAA) (OCA)
       ['AUA','71','airspace:type','touchdown_zone'], // Touchdown Zone
       ['AUA','74','airspace:type','refueling/track_area'], // Refueling/Track Area
       ['AUA','75','airspace:type','berlin_control_zone'], // Berlin Control Zone
       ['AUA','79','airspace:type','special_use_airspace_exclusions'], // Special Use Airspace Exclusions
       ['AUA','80','airspace:type','radar_stage_1'], // Radar Stage 1
       ['AUA','81','airspace:type','radar_stage_2'], // Radar Stage 2
       ['AUA','82','airspace:type','radar_stage_3'], // Radar Stage 3
       ['AUA','83','airspace:type','berlin_control_zone_associated_corridors'], // Berlin Control Zone Associated Corridors
       ['AUA','997',undefined,undefined], // Unpopulated
       ['AUA','998','airspace:type','not_applicable'], // Not Applicable
       ['AUA','999','airspace:type','other'], // Other

       // AUB - Airspace Use Boundary
       ['AUB','0','raw:AUB','unknown'], // Unknown
       ['AUB','1','airspace_boundary:type','flight_information_region_(fir)'], // Flight Information Region (FIR)
       ['AUB','2','airspace_boundary:type','sub-fir'], // Sub-FIR
       ['AUB','3','airspace_boundary:type','control_zone_(ctz/ctr)'], // Control Zone (CTZ/CTR)
       ['AUB','4','airspace_boundary:type','military_ctz/ctr'], // Military CTZ/CTR
       ['AUB','6','airspace_boundary:type','special_rules_zone_(srz)'], // Special Rules Zone (SRZ)
       ['AUB','7','airspace_boundary:type','advisory_area_(ada)'], // Advisory Area (ADA)
       ['AUB','8','airspace_boundary:type','terminal_control_area_(tca)/military_tca_(mtma)'], // Terminal Control Area (TCA)/Military TCA (MTMA)
       ['AUB','13','airspace_boundary:type','area_control_center_(acc)'], // Area Control Center (ACC)
       ['AUB','14','airspace_boundary:type','radar_area'], // Radar Area
       ['AUB','19','airspace_boundary:type','upper_information_region_(uir)'], // Upper Information Region (UIR)
       ['AUB','20','airspace_boundary:type','upper_control_area'], // Upper Control Area
       ['AUB','21','airspace_boundary:type','military_upper'], // Military Upper
       ['AUB','22','airspace_boundary:type','upper_advisory_area_(uda)'], // Upper Advisory Area (UDA)
       ['AUB','23','airspace_boundary:type','control_area_(cta)'], // Control Area (CTA)
       ['AUB','24','airspace_boundary:type','special_rules_area'], // Special Rules Area
       ['AUB','27','airspace_boundary:type','air-to-air_refueling_area/track/route'], // Air-to-Air Refueling Area/Track/Route
       ['AUB','29','airspace_boundary:type','oceanic_control_area_(non-faa)_(oca)'], // Oceanic Control Area (non-FAA) (OCA)
       ['AUB','30','airspace_boundary:type','oceanic_control_area_(faa)_(oca)'], // Oceanic Control Area (FAA) (OCA)
       ['AUB','32','airspace_boundary:type','air_defense_identification_zone_(adiz)'], // Air Defense Identification Zone (ADIZ)
       ['AUB','33','airspace_boundary:type','buffer_zone'], // Buffer Zone
       ['AUB','58','airspace_boundary:type','air_route_traffic_control_center_(artcc)'], // Air Route Traffic Control Center (ARTCC)
       ['AUB','999','airspace_boundary:type','other'], // Other

       // AUL - Airspace Use Limitations
       ['AUL','0','raw:AUL','unknown'], // Unknown
       ['AUL','1','raw:AUL','danger_area'], // Danger Area
       ['AUL','2','raw:AUL','prohibited_area'], // Prohibited Area
       ['AUL','3','raw:AUL','restricted_area'], // Restricted Area
       ['AUL','5','raw:AUL','alert_area'], // Alert Area
       ['AUL','6','raw:AUL','warning_area'], // Warning Area
       ['AUL','9','raw:AUL','temporary_reserved_airspace_(tra)'], // Temporary Reserved Airspace (TRA)
       ['AUL','28','raw:AUL','military_operating_area_(moa)'], // Military Operating Area (MOA)
       ['AUL','999','raw:AUL','other'], // Other

       // AUR - Airspace Use Routes
       ['AUR','0','raw:AUR','unknown'], // Unknown
       ['AUR','5','raw:AUR','corridor'], // Corridor
       ['AUR','8','raw:AUR','advisory'], // Advisory
       ['AUR','9','raw:AUR','direct'], // Direct
       ['AUR','10','raw:AUR','military'], // Military
       ['AUR','11','raw:AUR','oceanic'], // Oceanic
       ['AUR','12','raw:AUR','area_navigation_(rnav)'], // Area Navigation (RNAV)
       ['AUR','14','raw:AUR','tacan'], // TACAN
       // ['AUR','998','raw:AUR',undefined,undefined], // Not Applicable
       ['AUR','998','raw:AUR','raw:AUR','not_applicable'], // Not Applicable
       ['AUR','999','raw:AUR','other'], // Other

       // AVB - Highway Access Road Category
       // ['AVB','0','raw:AVB','unknown'], // Unknown
       ['AVB','5','raw:AVB','access_road_to_gravel_highway'], // Access Road To Gravel Highway
       ['AVB','6','raw:AVB','access_road_to_secondary_highway'], // Access Road To Secondary Highway
       ['AVB','7','raw:AVB','access_road_to_primary_highway'], // Access Road To Primary Highway
       ['AVB','8','raw:AVB','access_road_to_all_weather_highway'], // Access Road To All Weather Highway
       ['AVB','997',undefined,undefined], // Unpopulated

       // AVR - Railroad Access Category
       ['AVR','0','raw:AVR','unknown'], // Unknown
       ['AVR','24','raw:AVR','branch_to_mainline'], // Branch To Mainline
       ['AVR','25','raw:AVR','railroad_just_east_of_airport'], // Railroad Just East Of Airport
       ['AVR','26','raw:AVR','railroad_just_west_of_airport'], // Railroad Just West Of Airport
       ['AVR','27','raw:AVR','railroad_just_north_of_airport'], // Railroad Just North Of Airport
       ['AVR','28','raw:AVR','railroad_just_south_of_airport'], // Railroad Just South Of Airport
       ['AVR','29','raw:AVR','railroad_at_town'], // Railroad At Town
       ['AVR','32','raw:AVR','spur_access_to_main_line'], // Spur Access To Main Line
       ['AVR','33','raw:AVR','spur_access_to_branch_line'], // Spur Access To Branch Line
       ['AVR','997',undefined,undefined], // Unpopulated

       // B01 - Fuel Storage Method
       ['B01','0','raw:B01','unknown'], // Unknown
       ['B01','1','raw:B01','k_-_buried_tank.'], // K - Buried Tank.
       ['B01','2','raw:B01','c_-_aboveground_tank.'], // C - Aboveground Tank.
       ['B01','3','raw:B01','h_-_aboveground_and_buried_tanks.'], // H - Aboveground and Buried Tanks.
       ['B01','4','raw:B01','b_-_semi-buried_tank.'], // B - Semi-buried Tank.
       ['B01','5','raw:B01','g_-_aboveground_and_semi-buried_tanks.'], // G - Aboveground and Semi-buried Tanks.
       ['B01','6','raw:B01','f_-_buried_and_semi-buried_tanks.'], // F - Buried and Semi-buried Tanks.
       ['B01','7','raw:B01','d_-_railroad_tank_car.'], // D - Railroad Tank Car.
       ['B01','8','raw:B01','p_-_truck.'], // P - Truck.
       ['B01','9','raw:B01','m_-_barrels,_drums,_or_cans.'], // M - Barrels, Drums, or Cans.
       ['B01','10','raw:B01','j_-_buried,_semi-buried,_and_aboveground_tanks.'], // J - Buried, Semi-buried, and Aboveground Tanks.

       // BAC - Built-Up Area Classification
       ['BAC','0','building:density','unknown'], // Unknown
       ['BAC','1','building:density','sparse'], // Sparse to Moderate
       ['BAC','2','building:density','dense'], // Dense
       ['BAC','997',undefined,undefined], // Unpopulated

       // BCC - Bypass Condition Category
       ['BCC','0','bypass','unknown'], // Unknown
       ['BCC','1','bypass','easy'], // Easy (Obstacle can be crossed within 2 KM of feature, no work)
       ['BCC','2','bypass','difficult'], // Difficult (Obstacle can be crossed within 2 KM of feature, work required).
       ['BCC','3','bypass','impossible'], // Impossible (Obstacle cannot be bypassed within 2 KM of feature)
       ['BCC','997',undefined,undefined], // Unpopulated

       // BET - Beacon Type Category
       ['BET','0','seamark:beacon:category','unknown'], // Unknown
       ['BET','35','seamark:beacon:category','articulated_lights'], // Articulated Lights
       ['BET','93','beacon_lateral:category','lateral_starboard'], // Lateral starboard-hand mark
       ['BET','94','beacon_lateral:category','port'], // Lateral port-hand mark
       ['BET','999','seamark:beacon:category','other'], // Other

       // BFC - Building Function Category
       ['BFC','0','raw:BFC','unknown'], // Unknown
       ['BFC','1','raw:BFC','fabrication_structures'], // Fabrication Structures
       ['BFC','2','raw:BFC','government_building'], // Government Building
       ['BFC','3','raw:BFC','capitol_building'], // Capitol Building
       ['BFC','4','raw:BFC','castle'], // Castle
       ['BFC','5','raw:BFC','government_administration_building'], // Government Administration Building
       ['BFC','6','raw:BFC','hospital'], // Hospital
       ['BFC','7','raw:BFC','house_of_worship'], // House of Worship
       ['BFC','8','raw:BFC','military_administration/operations_building'], // Military Administration/Operations Building
       ['BFC','9','raw:BFC','museum'], // Museum
       ['BFC','10','raw:BFC','observatory'], // Observatory
       ['BFC','11','raw:BFC','palace'], // Palace
       ['BFC','12','raw:BFC','police_station'], // Police Station
       ['BFC','13','raw:BFC','prison'], // Prison
       ['BFC','14','raw:BFC','ranger_station'], // Ranger Station
       ['BFC','15','raw:BFC','school'], // School
       ['BFC','16','raw:BFC','house'], // House
       ['BFC','17','raw:BFC','multi_unit_dwelling'], // Multi Unit Dwelling
       ['BFC','18','raw:BFC','cemetery_building'], // Cemetery Building
       ['BFC','19','raw:BFC','farm_building'], // Farm Building
       ['BFC','20','raw:BFC','greenhouse'], // Greenhouse
       ['BFC','21','raw:BFC','garage'], // Garage
       ['BFC','22','raw:BFC','watermill/gristmill'], // Watermill/Gristmill
       ['BFC','23','raw:BFC','wind_tunnel'], // Wind Tunnel
       ['BFC','24','raw:BFC','warehouse'], // Warehouse
       ['BFC','25','raw:BFC','roundhouse'], // Roundhouse
       ['BFC','26','raw:BFC','railroad_storage/repair_facility'], // Railroad Storage/Repair Facility
       ['BFC','27','raw:BFC','depot_terminal'], // Depot Terminal
       ['BFC','28','raw:BFC','administration_building'], // Administration Building
       ['BFC','29','raw:BFC','aircraft_maintenance_shop'], // Aircraft Maintenance Shop
       ['BFC','30','raw:BFC','hangar'], // Hangar
       ['BFC','31','raw:BFC','customs_house'], // Customs House
       ['BFC','33','raw:BFC','health_office'], // Health Office
       ['BFC','34','raw:BFC','firing_range'], // Firing Range
       ['BFC','35','raw:BFC','post_office'], // Post Office
       ['BFC','36','raw:BFC','barracks/dormitory'], // Barracks/Dormitory
       ['BFC','37','raw:BFC','fire_station'], // Fire Station
       ['BFC','38','raw:BFC','jail'], // Jail
       ['BFC','40','raw:BFC','telephone_switching_station'], // Telephone Switching Station
       ['BFC','51','raw:BFC','market'], // Market
       ['BFC','52','raw:BFC','town_hall'], // Town Hall
       ['BFC','53','raw:BFC','bank'], // Bank
       ['BFC','54','raw:BFC','service/refueling_station'], // Service/Refueling Station
       ['BFC','55','raw:BFC','yacht_club/sailing_club'], // Yacht Club/Sailing Club
       ['BFC','56','raw:BFC','public_inn'], // Public Inn
       ['BFC','57','raw:BFC','restaurant'], // Restaurant
       ['BFC','58','raw:BFC','observation'], // Observation
       ['BFC','59','raw:BFC','research_and_development_lab/research_facility'], // Research and Development Lab/Research Facility
       ['BFC','61','raw:BFC','courthouse'], // Courthouse
       ['BFC','62','raw:BFC','legation'], // Legation
       ['BFC','63','raw:BFC','mission'], // Mission
       ['BFC','64','raw:BFC','chancery'], // Chancery
       ['BFC','65','raw:BFC','ambassadorial_residence'], // Ambassadorial Residence
       ['BFC','66','raw:BFC','embassy'], // Embassy
       ['BFC','67','raw:BFC','consulate'], // Consulate
       ['BFC','68','raw:BFC','guard_house'], // Guard House
       ['BFC','69','raw:BFC','guard_shack/guard_room'], // Guard Shack/Guard Room
       ['BFC','70','raw:BFC','kennel'], // Kennel
       ['BFC','71','raw:BFC','oil_mill_(vegetable)'], // Oil Mill (Vegetable)
       ['BFC','72','raw:BFC','aerator'], // Aerator
       ['BFC','73','raw:BFC','carpentry'], // Carpentry
       ['BFC','74','raw:BFC','sawmill'], // Sawmill
       ['BFC','75','raw:BFC','kiln/oven'], // Kiln/Oven
       ['BFC','76','raw:BFC','signal_box/railway_signalmans_house'], // Signal Box/Railway Signalmans House
       ['BFC','77','raw:BFC','harbor_masters_office'], // Harbor Masters Office
       ['BFC','78','raw:BFC','marine_police'], // Marine Police
       ['BFC','79','raw:BFC','rescue'], // Rescue
       ['BFC','82','raw:BFC','lighthouse'], // Lighthouse
       ['BFC','83','raw:BFC','power_generation'], // Power Generation
       ['BFC','84','raw:BFC','filtration_plant'], // Filtration Plant
       ['BFC','85','raw:BFC','newspaper_plant'], // Newspaper Plant
       ['BFC','86','raw:BFC','telephone_exchange_(main)'], // Telephone Exchange (Main)
       ['BFC','87','raw:BFC','auditorium'], // Auditorium
       ['BFC','88','raw:BFC','opera_house'], // Opera House
       ['BFC','89','raw:BFC','processing/treatment'], // Processing/Treatment
       ['BFC','90','raw:BFC','pumphouse'], // Pumphouse
       ['BFC','91','raw:BFC','mobile_home'], // Mobile Home
       ['BFC','92','raw:BFC','weather_station'], // Weather Station
       ['BFC','93','raw:BFC','dependents_housing/bivouac_area'], // Dependents Housing/Bivouac Area
       ['BFC','94','raw:BFC','railroad_station'], // Railroad Station
       ['BFC','95','raw:BFC','hotel'], // Hotel
       ['BFC','96','raw:BFC','diplomatic_building'], // Diplomatic Building
       ['BFC','97','raw:BFC','trading_post'], // Trading Post
       ['BFC','99','raw:BFC','battery'], // Battery
       ['BFC','100','raw:BFC','medical_center'], // Medical Center
       ['BFC','101','raw:BFC','municipal_hall'], // Municipal Hall
       ['BFC','102','raw:BFC','oil/gas_facilities_building'], // Oil/Gas Facilities Building
       ['BFC','103','raw:BFC','outbuilding'], // Outbuilding
       ['BFC','104','raw:BFC','paper/pulp_mill'], // Paper/Pulp Mill
       ['BFC','105','raw:BFC','reformatory'], // Reformatory
       ['BFC','106','raw:BFC','sanitarium'], // Sanitarium
       ['BFC','107','raw:BFC','satellite_tracking_station'], // Satellite Tracking Station
       ['BFC','109','raw:BFC','senior_citizens_home'], // Senior Citizens Home
       ['BFC','110','raw:BFC','shipyard'], // Shipyard
       ['BFC','111','raw:BFC','sportsplex'], // Sportsplex
       ['BFC','112','raw:BFC','steel_mill'], // Steel Mill
       ['BFC','113','raw:BFC','weigh_scale_(highway)'], // Weigh Scale (Highway)
       ['BFC','115','raw:BFC','hostel'], // Hostel
       ['BFC','116','raw:BFC','factory'], // Factory
       ['BFC','117','raw:BFC','motel'], // Motel
       ['BFC','118','raw:BFC','community_center'], // Community Center
       ['BFC','119','raw:BFC','city_hall'], // City Hall
       ['BFC','120','raw:BFC','automobile_plant'], // Automobile Plant
       ['BFC','121','raw:BFC','armory'], // Armory
       ['BFC','122','raw:BFC','shopping_center'], // Shopping Center
       ['BFC','123','raw:BFC','correctional_institute'], // Correctional Institute
       ['BFC','124','raw:BFC','repair_facility'], // Repair Facility
       ['BFC','125','raw:BFC','barn/machinery_shed'], // Barn/Machinery Shed
       ['BFC','126','raw:BFC','astronomical_station'], // Astronomical Station
       ['BFC','127','raw:BFC','theater'], // Theater
       ['BFC','128','raw:BFC','library'], // Library
       ['BFC','129','raw:BFC','airport_terminal'], // Airport Terminal
       ['BFC','130','raw:BFC','bus_station'], // Bus Station
       ['BFC','131','raw:BFC','pilot_office'], // Pilot Office
       ['BFC','132','raw:BFC','pilot_look-out'], // Pilot Look-out
       ['BFC','133','raw:BFC','commercial_building'], // Commercial building
       ['BFC','140','raw:BFC','non-governmental_building_of_significance_(other_than_government).'], // Non-Governmental building of Significance (other than government).
       ['BFC','150','raw:BFC','barracks'], // Barracks
       ['BFC','151','raw:BFC','dormitory'], // Dormitory
       ['BFC','723','raw:BFC','combined_fire_and_police_station'], // Combined Fire and Police Station
       // ['BFC','998',undefined,undefined], // Not Applicable
       ['BFC','998','raw:BFC','not_applicable'], // Not Applicable
       ['BFC','999','raw:BFC','other'], // Other

       // BIT - Beach Indicator Type
       ['BIT','0','raw:BIT','unknown'], // Unknown
       ['BIT','1','raw:BIT','nearshore'], // Nearshore
       ['BIT','2','raw:BIT','foreshore'], // Foreshore
       ['BIT','3','raw:BIT','backshore'], // Backshore

       // BMC - Bottom Materials Composition
       ['BMC','0','raw:BMC','unknown'], // Unknown
       ['BMC','1','raw:BMC','clay_and_silt'], // Clay and Silt
       ['BMC','2','raw:BMC','silty_sands'], // Silty Sands
       ['BMC','3','raw:BMC','sand_and_gravel'], // Sand and Gravel
       ['BMC','4','raw:BMC','gravel_and_cobble'], // Gravel and Cobble
       ['BMC','5','raw:BMC','rocks_and_boulders'], // Rocks and Boulders
       ['BMC','6','raw:BMC','bedrock'], // Bedrock
       ['BMC','7','raw:BMC','paved'], // Paved
       ['BMC','8','raw:BMC','peat'], // Peat
       ['BMC','997',undefined,undefined], // Unpopulated

       // BOT - Bridge Opening Type
       ['BOT','0','bridge:movable','yes'], // Unknown
       ['BOT','4','bridge:movable','bascule'], // Draw/Bascule
       ['BOT','10','bridge:movable','swing'], // Swing
       ['BOT','11','bridge:movable','lift'], // Lift
       ['BOT','12','bridge:movable','retractable'], // Retractable
       // ['BOT','13',undefined,undefined], // Not Applicable
       ['BOT','13','bridge:movable','not_applicable'], // Not Applicable
       ['BOT','14','bridge:movable','submersible'], // Submersible
       ['BOT','999','bridge:movable','other'], // Other

       // BSC - Bridge/Bridge Superstructure Category
       ['BSC','0','bridge:structure','unknown'], // Unknown
       ['BSC','1','bridge:structure','open_spandrel_arch'], // Arch (assume open spandrel)
       ['BSC','2','bridge:structure','cantilever'], // Cantilever
       ['BSC','3','bridge:structure','deck'], // Deck
       ['BSC','5','bridge:structure','floating'], // Floating Bridge/Pontoon
       ['BSC','6','bridge:structure','girder'], // Girder
       ['BSC','7','bridge:structure','tower_suspension'], // Tower Suspension
       ['BSC','8','bridge:structure','truss'], // Truss
       ['BSC','9','bridge:structure','suspension'], // Suspension
       ['BSC','12','bridge:structure','transporter'], // Transporter
       ['BSC','15','bridge:structure','slab'], // Slab
       ['BSC','16','bridge:structure','stringer_beam'], // Stringer (beam)
       ['BSC','17','bridge:structure','arch_suspension'], // Arch Suspension
       ['BSC','26','bridge:structure','closed_spandrel_arch'], // Arch (closed spandrel)
       ['BSC','27','bridge:structure','cable_stayed'], // Cable Stayed
       ['BSC','28','bridge:structure','viaduct'], // Viaduct ## Check
       ['BSC','997',undefined,undefined], // Unpopulated
       ['BSC','999','bridge:structure','other'], // Other

       // BSM - Bridge Span Mobility
       ['BSM','0','bridge:mobile_span','unknown'], // Unknown
       ['BSM','1','bridge:mobile_span','yes'], // Moveable Span
       ['BSM','2','bridge:mobile_span','no'], // Fixed Span
       ['BSM','997',undefined,undefined], // Unpopulated

       // BSP - Bridge Span Category
       ['BSP','0','raw:BSP','unknown'], // Unknown
       ['BSP','1','raw:BSP','truss'], // Truss
       ['BSP','2','raw:BSP','truss,_moveable_or_swing'], // Truss, moveable or swing
       ['BSP','3','raw:BSP','plate_girder'], // Plate girder
       ['BSP','4','raw:BSP','plate_girder_moveable_as_vertical_lift'], // Plate girder moveable as vertical lift
       ['BSP','5','raw:BSP','plate_girder_moveable_as_draw_bridge'], // Plate girder moveable as draw bridge
       ['BSP','6','raw:BSP','plate_girder_moveable_as_bascule'], // Plate girder moveable as bascule
       ['BSP','7','raw:BSP','stringer,_beam'], // Stringer, beam
       ['BSP','8','raw:BSP','stringer,_moveable_as_vertical_lift'], // Stringer, moveable as vertical lift
       ['BSP','9','raw:BSP','stringer,_moveable_as_draw_bridge'], // Stringer, moveable as draw bridge
       ['BSP','10','raw:BSP','slab'], // Slab
       ['BSP','11','raw:BSP','arc,_closed_span'], // Arc, closed span
       ['BSP','12','raw:BSP','arc,_open_span'], // Arc, open span
       ['BSP','13','raw:BSP','floating_bridge,_pontoon_bridge'], // Floating bridge, pontoon bridge
       ['BSP','15','raw:BSP','frame_structure'], // Frame structure
       ['BSP','16','raw:BSP','vault_structure'], // Vault structure
       ['BSP','17','raw:BSP','unspecified_fixed'], // Unspecified fixed
       ['BSP','999','raw:BSP','other'], // Other

       // BST - Boundary Status Type
       ['BST','0','boundary:status','unknown'], // Unknown
       ['BST','1','boundary:status','definite'], // Definite
       ['BST','2','boundary:status','indefinite'], // Indefinite
       ['BST','3','boundary:status','in_dispute'], // In Dispute
       ['BST','4','boundary:status','no_defined_boundary'], // No Defined Boundary

       // BUD - Brush/Undergrowth Density Code
       ['BUD','0','raw:BUD','unknown'], // Unknown
       ['BUD','1','raw:BUD','open_(<=5%)'], // Open (<=5%)
       ['BUD','2','raw:BUD','sparse_(>5%<=15%)'], // Sparse (>5%<=15%)
       ['BUD','3','raw:BUD','medium_(>15%<=50%)'], // Medium (>15%<=50%)
       ['BUD','4','raw:BUD','dense_(>50%)'], // Dense (>50%)
       ['BUD','997',undefined,undefined], // Unpopulated
       // ['BUD','998',undefined,undefined], // Not Applicable
       ['BUD','998','raw:BUD','not_applicable'], // Not Applicable

       // BUT - Buoy Type Category
       ['BTC','0','seamark:buoy:type','unknown'], // Unknown
       ['BTC','1','seamark:type','buoy_cardinal'], // Cardinal
       ['BTC','2','seamark:buoy:type','float'], // Float
       ['BTC','3','seamark:buoy_special_purpose:category','buoy_isolated_danger'], // Isolated Danger
       ['BTC','4','seamark:buoy_special_purpose:category','lanby'], // Large Navigational Buoy (LANBY)
       ['BTC','6','seamark:type','light_float'], // Light Float
       ['BTC','7','seamark:mooring:category','buoy'], // Mooring
       ['BTC','5','seamark:type','buoy_lateral'], // Lateral
       ['BTC','10','seamark:buoy_special_purpose:category','odas'], // Ocean Data Acquisition System (ODAS)
       ['BTC','17','seamark:buoy:type','tanker'], // Tanker
       ['BTC','35','seamark:type','articulated_lights'], // Articulated Lights
       ['BTC','39','seamark:buoy:type','trot'], // Trot
       ['BTC','84','seamark:buoy:type','apex_(arc)'], // APEX (ARC) Buoy
       ['BTC','95','seamark:buoy_cardinal:category','west'], // Cardinal West Mark
       ['BTC','96','seamark:buoy_cardinal:category','south'], // Cardinal South Mark
       ['BTC','97','seamark:buoy_cardinal:category','east'], // Cardinal East Mark
       ['BTC','98','seamark:buoy_cardinal:category','north'], // Cardinal North Mark
       ['BTC','99','seamark:buoy:type','installation'], // Installation
       ['BTC','100','seamark:buoy:type','waverider'], // Waverider
       ['BTC','101','seamark:buoy:type','wave_meter'], // Wave Meter
       // ['BTC','998',undefined,undefined], // Not Applicable
       ['BTC','998','seamark:buoy:type','not_applicable'], // Not Applicable
       ['BTC','999','seamark:buoy:type','other'], // Other


       // BVL - Bank Vegetation Left
       ['BVL','0','raw:BVL','unknown'], // Unknown
       ['BVL','1','raw:BVL','open_(<=5%)'], // Open (<=5%)
       ['BVL','2','raw:BVL','sparse_(>5%<=15%)'], // Sparse (>5%<=15%)
       ['BVL','3','raw:BVL','medium_(>15%<=50%)'], // Medium (>15%<=50%)
       ['BVL','4','raw:BVL','dense_(>50%)'], // Dense (>50%)
       ['BVL','997',undefined,undefined], // Unpopulated

       // BVR - Bank Vegetation Right
       ['BVR','0','raw:BVR','unknown'], // Unknown
       ['BVR','1','raw:BVR','open_(<=5%)'], // Open (<=5%)
       ['BVR','2','raw:BVR','sparse_(>5%<=15%)'], // Sparse (>5%<=15%)
       ['BVR','3','raw:BVR','medium_(>15%<=50%)'], // Medium (>15%<=50%)
       ['BVR','4','raw:BVR','dense_(>50%)'], // Dense (>50%)
       ['BVR','997',undefined,undefined], // Unpopulated

       // CAB - Cable Classification
       ['CAB','0','raw:CAB','unknown'], // Unknown
       ['CAB','2','raw:CAB','power_line'], // Power Line
       ['CAB','3','raw:CAB','telephone'], // Telephone
       ['CAB','4','raw:CAB','telegraph'], // Telegraph
       ['CAB','6','raw:CAB','transmission_line_(iho_s-57)'], // Transmission Line (IHO S-57)
       ['CAB','7','raw:CAB','digital_wideband_communication_lines'], // Digital Wideband Communication Lines
       ['CAB','997',undefined,undefined], // Unpopulated
       ['CAB','999','raw:CAB','other'], // Other

       // CCA - Constriction/Expansion Category
       ['CCA','0','raw:CCA','unknown'], // Unknown
       ['CCA','1','raw:CCA','gateway'], // Gateway
       ['CCA','2','raw:CCA','a_narrow_pass_between_rocks'], // A narrow pass between rocks
       ['CCA','3','raw:CCA','road_siding_on_narrow_roads'], // Road siding on narrow roads
       ['CCA','4','raw:CCA','a_passage_through_a_building'], // A passage through a building
       ['CCA','999','raw:CCA','other'], // Other

       // CCC - Color Code Category
       ['CCC','0','seamark:buoy:colour','unknown'], // Unknown
       ['CCC','1','seamark:buoy:colour','black'], // Black
       ['CCC','2','seamark:buoy:colour','blue'], // Blue
       ['CCC','3','seamark:buoy:colour','brown'], // Brown
       ['CCC','4','seamark:buoy:colour','gray'], // Gray
       ['CCC','5','seamark:buoy:colour','green'], // Green
       ['CCC','7','seamark:buoy:colour','chocolate'], // Chocolate
       ['CCC','9','seamark:buoy:colour','orange'], // Orange
       ['CCC','12','seamark:buoy:colour','red'], // Red
       ['CCC','14','seamark:buoy:colour','violet'], // Violet
       ['CCC','15','seamark:buoy:colour','white'], // White
       ['CCC','19','seamark:buoy:colour','yellow'], // Yellow
       ['CCC','20','seamark:buoy:colour','red_and_white_(rw)'], // Red & White (RW)
       ['CCC','21','seamark:buoy:colour','red_and_green_(rg)'], // Red & Green (RG)
       ['CCC','22','seamark:buoy:colour','red_and_black_(rb)'], // Red & Black (RB)
       ['CCC','23','seamark:buoy:colour','red-green-red_(rgr)'], // Red-Green-Red (RGR)
       ['CCC','24','seamark:buoy:colour','green_and_white_(gw)'], // Green & White (GW)
       ['CCC','25','seamark:buoy:colour','green_and_red_(gr)'], // Green & Red (GR)
       ['CCC','26','seamark:buoy:colour','green_and_black_(gb)'], // Green & Black (GB)
       ['CCC','27','seamark:buoy:colour','green-red-green_(grg)'], // Green-Red-Green (GRG)
       ['CCC','28','seamark:buoy:colour','green-yellow-black_(gyb)'], // Green-Yellow-Black (GYB)
       ['CCC','29','seamark:buoy:colour','yellow_and_black_(yb)'], // Yellow & Black (YB)
       ['CCC','30','seamark:buoy:colour','yellow-black-yellow_(yby)'], // Yellow-Black-Yellow (YBY)
       ['CCC','31','seamark:buoy:colour','yellow_and_red_(yr)'], // Yellow & Red (YR)
       ['CCC','32','seamark:buoy:colour','yellow_and_green_(yg)'], // Yellow & Green (YG)
       ['CCC','33','seamark:buoy:colour','yellow-red-white_(yrw)'], // Yellow-Red-White (YRW)
       ['CCC','34','seamark:buoy:colour','black_and_yellow_(by)'], // Black & Yellow (BY)
       ['CCC','35','seamark:buoy:colour','black-yellow-black_(byb)'], // Black-Yellow-Black (BYB)
       ['CCC','36','seamark:buoy:colour','black-red-black_(brb)'], // Black-Red-Black (BRB)
       ['CCC','37','seamark:buoy:colour','black_and_white_(bw)'], // Black & White (BW)
       ['CCC','38','seamark:buoy:colour','black_and_red_(br)'], // Black & Red (BR)
       ['CCC','39','seamark:buoy:colour','black_and_green_(bg)'], // Black & Green (BG)
       ['CCC','40','seamark:buoy:colour','white_and_red_(wr)'], // White & Red (WR)
       ['CCC','41','seamark:buoy:colour','white_and_orange_(wor)'], // White & Orange (WOr)
       ['CCC','42','seamark:buoy:colour','white_and_green_(wg)'], // White & Green (WG)
       ['CCC','43','seamark:buoy:colour','white_and_black_(wb)'], // White & Black (WB)
       ['CCC','44','seamark:buoy:colour','white_and_yellow_(wy)'], // White & Yellow (WY)
       ['CCC','45','seamark:buoy:colour','white-red-green_(wrg)'], // White-Red-Green (WRG)
       ['CCC','46','seamark:buoy:colour','white-green-white_(wgw)'], // White-Green-White (WGW)
       ['CCC','47','seamark:buoy:colour','magenta'], // Magenta
       ['CCC','48','seamark:buoy:colour','amber'], // Amber
       ['CCC','49','seamark:buoy:colour','buff'], // Buff
       ['CCC','50','seamark:buoy:colour','nautical_purple'], // Nautical Purple
       ['CCC','51','seamark:buoy:colour','pink'], // Pink
       ['CCC','997',undefined,undefined], // Unpopulated
       ['CCC','999','seamark:buoy:colour','other'], // Other

       // CDA - Covered Drain Attribute
       ['CDA','0','raw:CDA','unknown'], // Unknown
       ['CDA','1','raw:CDA','uncovered'], // Uncovered
       ['CDA','2','raw:CDA','covered'], // Covered
       ['CDA','997',undefined,undefined], // Unpopulated
       // ['CDA','998',undefined,undefined], // Not Applicable
       ['CDA','998','raw:CDA','not_applicable'], // Not Applicable

       // CDP - Calendar Date Type
       ['CDP','0','calendar:date_type','unknown'], // Unknown
       ['CDP','18','calendar:date_type','receipt'], // Receipt
       ['CDP','19','calendar:date_type','source'], // Source
       ['CDP','24','calendar:date_type','up-to-dateness/revision'], // Up-to-dateness/revision

       // CET - Cut/Embankment Type Category
       ['CET','1','raw:CET','one_side'], // One Side
       ['CET','2','raw:CET','both_sides'], // Both Sides

       // CFT - Boundary Configuration Type Code
       ['CFT','0','raw:CFT','unknown'], // Unknown
       ['CFT','1','raw:CFT','ethnic/social/cultural_divisions'], // Ethnic/Social/Cultural Divisions
       ['CFT','2','raw:CFT','around_populated_places'], // Around Populated Places
       ['CFT','3','raw:CFT','cadastral_limits'], // Cadastral Limits
       ['CFT','4','raw:CFT','military_engagement_line'], // Military Engagement Line
       ['CFT','5','raw:CFT','historical_boundary'], // Historical Boundary
       ['CFT','6','raw:CFT','economic_boundary'], // Economic Boundary
       ['CFT','7','raw:CFT','follows_land_contour'], // Follows Land Contour
       ['CFT','8','raw:CFT','surface_watershed_boundary_also_referred_to_as:_surface_waterparting_boundary,_surface_drainage_divide,_divortium_aquarum'], // Surface Watershed Boundary Also referred to as: Surface Waterparting Boundary, Surface Drainage Divide, Divortium Aquarum
       ['CFT','9','raw:CFT','stream_thalweg'], // Stream Thalweg
       ['CFT','10','raw:CFT','peak-to-peak_along_ridges'], // Peak-to-Peak Along Ridges
       ['CFT','11','raw:CFT','peak-to-peak_along_drainage'], // Peak-to-Peak Along Drainage
       ['CFT','12','raw:CFT','shoreline_-_left_bank_of_watercourse'], // Shoreline - Left Bank of Watercourse
       ['CFT','13','raw:CFT','shoreline_-_right_bank_of_watercourse'], // Shoreline - Right Bank of Watercourse
       ['CFT','14','raw:CFT','shoreline'], // Shoreline
       ['CFT','15','raw:CFT','lake_transect'], // Lake Transect
       ['CFT','16','raw:CFT','meridian_(longitude_line)'], // Meridian (Longitude Line)
       ['CFT','17','raw:CFT','equidistant_line'], // Equidistant line
       ['CFT','18','raw:CFT','parallel_(latitude_line)'], // Parallel (Latitude Line)
       ['CFT','19','raw:CFT','polygon'], // Polygon
       ['CFT','20','raw:CFT','straight_line_segment'], // Straight Line Segment
       ['CFT','21','raw:CFT','straight_line_(geodesic_line)'], // Straight Line (Geodesic Line)
       ['CFT','22','raw:CFT','straight_line_(great_circle_text)'], // Straight Line (Great Circle Text)
       ['CFT','23','raw:CFT','envelope_of_arc'], // Envelope Of Arc
       ['CFT','29','raw:CFT','main_channel'], // Main Channel
       ['CFT','31','raw:CFT','configuration_taken_from_historical_document.'], // Configuration taken from historical document.
       ['CFT','32','raw:CFT','thalweg_to_peak'], // Thalweg to Peak
       ['CFT','999','raw:CFT','other'], // Other

       // CHA - Light Characteristic Category
       ['CHA','0','beacon:light','unknown'], // Unknown
       ['CHA','21','beacon:light','lighted'], // Lighted
       ['CHA','23','beacon:light','unlighted'], // Unlighted

       // CLI - Communication Lines Isolation
       ['CLI','0','raw:CLI','unknown'], // Unknown
       ['CLI','1','raw:CLI','isolated'], // Isolated
       ['CLI','2','raw:CLI','not_isolated'], // Not isolated
       ['CLI','997',undefined,undefined], // Unpopulated

       // CLR - Class of Rapids
       ['CLR','0','raw:CLR','unknown'], // Unknown
       ['CLR','1','raw:CLR','class_1_easy'], // Class 1 Easy
       ['CLR','2','raw:CLR','class_2_novice_or_medium'], // Class 2 Novice or Medium
       ['CLR','3','raw:CLR','class_3_intermediate_or_difficult'], // Class 3 Intermediate or Difficult
       ['CLR','4','raw:CLR','class_4_advanced_or_very_difficult'], // Class 4 Advanced or Very Difficult
       ['CLR','5','raw:CLR','class_5_expert_or_extremely_difficult'], // Class 5 Expert or Extremely Difficult
       ['CLR','6','raw:CLR','class_6_extreme_and_exploratory_or_unrunnable'], // Class 6 Extreme and Exploratory or Unrunnable

       // CLS - Sounding Classification
       ['CLS','1','sounding:classification','UNCLASSIFIED'], // Unclassified
       ['CLS','3','sounding:classification','CONFIDENTIAL'], // Confidential
       ['CLS','4','sounding:classification','SECRET'], // Secret
       ['CLS','5','sounding:classification','TOP_SECRET'], // Top Secret
       ['CLS','6','sounding:classification','FOUO'], // For Official Use Only
       ['CLS','7','sounding:classification','RESTRICTED'], // Restricted
       ['CLS','8','sounding:classification','cnf'], // Cnf
       ['CLS','9','sounding:classification','cwn'], // Cwn
       ['CLS','10','sounding:classification','seb'], // Seb
       ['CLS','11','sounding:classification','seg'], // Seg
       ['CLS','12','sounding:classification','smv'], // Smv
       ['CLS','13','sounding:classification','snf'], // Snf
       ['CLS','14','sounding:classification','swn'], // Swn
       ['CLS','15','sounding:classification','uds'], // Uds

       // CLT - Color of Text
       ['CLT','1','raw:CLT','black'], // Black
       ['CLT','2','raw:CLT','blue'], // Blue
       ['CLT','3','raw:CLT','red-brown'], // Red-Brown
       ['CLT','4','raw:CLT','magenta'], // Magenta

       // COC - Conspicuous Category
       ['COC','0','conspicuous','unknown'], // Unknown
       ['COC','1','conspicuous:from_sea','visual'], // Conspicuous from sea
       ['COC','4','conspicuous:from_ground','visual'], // Conspicuous from land
       ['COC','5','conspicuous:from_air','visual'], // Conspicuous from air
       ['COC','6','conspicuous','no'], // Inconspicuous

       // COD - Certainty of Delineation
       ['COD','1','delineation','known'], // Limits and Information Known
       ['COD','2','delineation','unknown'], // Limits and Information Unknown
       ['COD','997',undefined,undefined], // Unpopulated

       // COE - Certainty of Existence
       ['COE','0',undefined,undefined], // Unknown
       ['COE','1',undefined,undefined], // Definite ## Default??
       ['COE','2','existance','doubtful'], // Doubtful
       ['COE','3','existance','reported'], // Reported
       ['COE','4','existance','approximate'], // Approximate/About
       ['COE','997',undefined,undefined], // Unpopulated
       ['COE','999','existance','other'], // Other

       // COF - Complex Outline Function
       ['COF','0','facility:type','unknown'], // Unknown
       ['COF','1','facility:type','agricultural'], // Agricultural
       ['COF','2','facility:type','commercial'], // Commercial
       ['COF','3','facility:type','communications'], // Communications
       ['COF','4','facility:type','diplomatic'], // Diplomatic
       ['COF','5','facility:type','educational'], // Educational
       ['COF','6','facility:type','governmental'], // Governmental
       ['COF','7','facility:type','industrial'], // Industrial
       ['COF','8','facility:type','medical'], // Medical
       ['COF','9','facility:type','military'], // Military
       ['COF','10','facility:type','mixed_urban'], // Mixed Urban
       ['COF','11','facility:type','recreational'], // Recreational
       ['COF','12','facility:type','religious'], // Religious
       ['COF','13','facility:type','reserve'], // Reserve
       ['COF','14','facility:type','residential'], // Residential
       ['COF','15','facility:type','transportation'], // Transportation
       ['COF','16','facility:type','utilities'], // Utilities
       ['COF','17','facility:type','maritime'], // Maritime
       ['COF','999','facility:type','other'], // Other

       // COMPLETENESS_CODE - Completeness Code
       ['COMPLETENESS_CODE','0','gifd:completeness_code','not_complete'], // Not Complete
       ['COMPLETENESS_CODE','1','gifd:completeness_code','complete'], // Complete

       // CPA - Control Point Attribute
       ['CPA','0','survey_point:control','unknown'], // Unknown
       ['CPA','2','survey_point:control','horizontal'], // Horizontal
       ['CPA','3','survey_point:control','horizontal_and_benchmark'], // Horizontal With Bench Mark
       ['CPA','4','survey_point:control','astronomic'], // Astronomic position
       ['CPA','5','survey_point:control','vertical'], // Vertical
       ['CPA','997',undefined,undefined], // Unpopulated

       // CRA - Crane Type Category
       ['CRA','0','raw:CRA','unknown'], // Unknown
       ['CRA','2','raw:CRA','bridge/gantry'], // Bridge/Gantry
       ['CRA','3','raw:CRA','rotating'], // Rotating
       ['CRA','7','raw:CRA','tower_crane'], // Tower Crane
       ['CRA','997',undefined,undefined], // Unpopulated
       // ['CRA','998',undefined,undefined], // Not Applicable
       ['CRA','998','raw:CRA','not_applicable'], // Not Applicable
       ['CRA','999','raw:CRA','other'], // Other

       // CRM - Crane Mobility Type
       ['CRM','0','raw:CRM','unknown'], // Unknown
       ['CRM','1','raw:CRM','fixed'], // Fixed
       ['CRM','2','raw:CRM','traveling'], // Traveling
       ['CRM','3','raw:CRM','floating'], // Floating
       ['CRM','999','raw:CRM','other'], // Other

       // CSC - Crossing Control Category
       ['CSC','2','raw:CSC','signal_devices'], // Signal Devices
       ['CSC','3','raw:CSC','stop_sign(s)'], // Stop Sign(s)
       ['CSC','4','raw:CSC','no_control_or_warning_signs.'], // No control or warning signs.

       // CSM - Secondary Material Characteristics
       ['CSM','0','material:characteristic','unknown'], // Unknown
       ['CSM','1','material:characteristic','broken'], // Broken
       ['CSM','2','material:characteristic','coarse'], // Coarse
       ['CSM','3','material:characteristic','decayed'], // Decayed
       ['CSM','4','material:characteristic','fine,_minute_particles'], // Fine, Minute Particles
       ['CSM','5','material:characteristic','gritty'], // Gritty
       ['CSM','6','material:characteristic','hard'], // Hard
       ['CSM','7','material:characteristic','rotten'], // Rotten
       ['CSM','8','material:characteristic','soft'], // Soft
       ['CSM','9','material:characteristic','sticky'], // Sticky
       ['CSM','10','material:characteristic','stiff'], // Stiff
       ['CSM','11','material:characteristic','streaky'], // Streaky
       ['CSM','12','material:characteristic','tenacious'], // Tenacious
       ['CSM','13','material:characteristic','uneven'], // Uneven
       ['CSM','17','material:characteristic','calcareous'], // Calcareous
       ['CSM','18','material:characteristic','flinty'], // Flinty
       ['CSM','19','material:characteristic','glacial'], // Glacial
       ['CSM','20','material:characteristic','ground'], // Ground
       ['CSM','21','material:characteristic','large'], // Large
       ['CSM','22','material:characteristic','rocky'], // Rocky
       ['CSM','23','material:characteristic','small'], // Small
       ['CSM','24','material:characteristic','speckled'], // Speckled
       ['CSM','25','material:characteristic','varied'], // Varied
       ['CSM','26','material:characteristic','volcanic'], // Volcanic
       ['CSM','27','material:characteristic','medium'], // Medium
       ['CSM','997',undefined,undefined], // Unpopulated

       // CTC - Culvert Type Category
       ['CTC','1','culvert:type','regular_with_soil'], // Regular, Earth Back-Filled
       ['CTC','2','culvert:type','box_with_soil'], // Box, Earth Back-Filled
       ['CTC','3','culvert:type','load_bearing_box'], // Box, Load Bearing

       // CUR - Current Type Category
       ['CUR','0','current','unknown'], // Unknown
       ['CUR','1','current','ebb_stream'], // Ebb
       ['CUR','2','current','flood_stream'], // Flood
       ['CUR','3','current','general_flow'], // General Flow
       ['CUR','4','current','river_flow'], // River Flow
       ['CUR','5','current','ocean_current'], // Ocean Flow
       ['CUR','6','current','rip'], // Rip
       ['CUR','7','current','longshore'], // Longshore
       ['CUR','997',undefined,undefined], // Unpopulated

       // DCR - Draft Correction Indicator
       ['DCR','1','raw:DCR','yes'], // Yes
       ['DCR','2','raw:DCR','no'], // No

       // DFT - Dam Face Type
       ['DFT','0','raw:DFT','unknown'], // Unknown
       ['DFT','1','raw:DFT','vertical'], // Vertical
       ['DFT','2','raw:DFT','slope'], // Slope

       // DGC - Drop Gate Category
       ['DGC','0','raw:DGC','unknown'], // Unknown
       ['DGC','1','raw:DGC','overhead_drop'], // Overhead Drop
       ['DGC','2','raw:DGC','side_drop'], // Side Drop

       // DIR - Directivity
       ['DIR','0','directivity','unknown'], // Unknown
       ['DIR','1','directivity','uni'], // Uni
       ['DIR','2','directivity','bi'], // Bi
       ['DIR','3','directivity','omni'], // Omni
       ['DIR','4','directivity','right_uni'], // Right Uni
       ['DIR','5','directivity','left_uni'], // Left Uni
       // ['DIR','998',undefined,undefined], // Not Applicable
       ['DIR','998','directivity','not_applicable'], // Not Applicable

       // DMC - Boundary Demarcation Code
       ['DMC','0','boundary:demarcated','unknown'], // Unknown
       ['DMC','1','boundary:demarcated','yes'], // Demarcated
       ['DMC','2','boundary:demarcated','no'], // Not Demarcated
       ['DMC','999','boundary:demarcated','other'], // Other

       // DSP - Boundary Dispute Status
       ['DSP','0','boundary:dispute_type','unknown'], // Unknown
       ['DSP','1','boundary:dispute_type','disputed'], // Disputed
       ['DSP','3','boundary:dispute_type','defacto'], // DeFacto
       ['DSP','4','boundary:dispute_type','dejure'], // DeJure
       ['DSP','999','boundary:dispute_type','other'], // Other

       // DW1 - Depth of Water (1)
       ['DW1','0','raw:DW1','unknown'], // Unknown
       ['DW1','1','raw:DW1','<=_0.8'], // <= 0.8
       ['DW1','2','raw:DW1','>_0.8_and_<=_1.6'], // > 0.8 and <= 1.6
       ['DW1','3','raw:DW1','>_1.6_and_<=_2.4'], // > 1.6 and <= 2.4
       ['DW1','4','raw:DW1','>_2.4'], // > 2.4
       ['DW1','997',undefined,undefined], // Unpopulated

       // EBT - Educational Building Type
       ['EBT','0','raw:EBT','unknown'], // Unknown
       ['EBT','1','raw:EBT','academy'], // Academy
       ['EBT','2','raw:EBT','college'], // College
       ['EBT','3','raw:EBT','educational_center'], // Educational Center
       ['EBT','4','raw:EBT','lyceum'], // Lyceum
       ['EBT','5','raw:EBT','university'], // University
       ['EBT','6','raw:EBT','seminary'], // Seminary
       // ['EBT','8',undefined,undefined], // Not Applicable
       ['EBT','8','raw:EBT','not_applicable'], // Not Applicable
       ['EBT','999','raw:EBT','other'], // Other

       // ELA - Elevation Accuracy
       ['ELA','0','raw:ELA','unknown'], // Unknown
       ['ELA','1','raw:ELA','accurate'], // Accurate
       ['ELA','2','raw:ELA','approximate'], // Approximate
       ['ELA','997',undefined,undefined], // Unpopulated

       // EXS - Existence Category
       ['EXS','0',undefined,undefined], // Unknown
       ['EXS','1','existance','definite'], // Definite
       ['EXS','5','condition','construction'], // Under Construction
       ['EXS','6','condition','abandoned'], // Abandoned/Disused
       ['EXS','7','condition','destroyed'], // Destroyed
       ['EXS','8','condition','dismantled'], // Dismantled
       ['EXS','10','condition','proposed'], // Proposed
       ['EXS','11','operational_status','temporary'], // Temporary
       ['EXS','22','oneway','yes'], // One-Way
       ['EXS','23','oneway','no'], // Two-Way
       ['EXS','27','operational_status','closed'], // Closed/Locked
       ['EXS','28','condition','functional'], // Operational
       ['EXS','30','isolated','no'], // Not Isolated
       ['EXS','31','isolated','yes'], // Isolated
       ['EXS','32','navigation','yes'], // Navigable
       ['EXS','36','commissioned','commissioned_and_operational'], // Commissioned and Operational
       ['EXS','37','commissioned','commissioned_and_on_test'], // Commissioned and on Test
       ['EXS','38','commissioned','commissioned_and_out_of_service'], // Commissioned and out of service
       ['EXS','39','commissioned','not_commissioned_and_operational'], // Not commissioned and operational
       ['EXS','40','commissioned','not_commissioned_and_on_test'], // Not commissioned and on test
       ['EXS','41','commissioned','not_commissioned_and_out_of_service'], // Not commissioned and out of service
       ['EXS','42','operational_status','continuous_operation'], // Continuous Operation
       ['EXS','43','operational_status','intermittent_operation'], // Intermittent operation
       ['EXS','45','artificial','no'], // Natural
       ['EXS','46','artificial','yes'], // Man-Made
       ['EXS','48','operational_status','controlled'], // Controlled
       ['EXS','57','existance','sonar_confirmed'], // Sonar Confirmed
       ['EXS','58','existance','sonar_not_confirmed'], // Sonar Not Confirmed
       ['EXS','59','operational_status','not_usable'], // Not Usable
       ['EXS','60','shoreline:deliniated','no'], // Indefinite (Shoreline)
       ['EXS','62','operational_status','partially_destroyed'], // Partially Destroyed
       ['EXS','66','condition','damaged'], // Damaged
       ['EXS','997',undefined,undefined], // Unpopulated
       ['EXS','999','operational_status','other'], // Other

       // FAC - Pier-Wharf-Quay Face Type
       ['FAC','0','raw:FAC','unknown'], // Unknown
       ['FAC','1','raw:FAC','open'], // Open
       ['FAC','2','raw:FAC','solid_face'], // Solid Face

       // FBL - Boundary Lights
       ['FBL','0','raw:FBL','unknown'], // Unknown
       ['FBL','1','raw:FBL','available'], // Available
       ['FBL','2','raw:FBL','not_available'], // Not Available
       ['FBL','3','raw:FBL','n/a'], // N/A

       // FCM - Feature Construction Material Code
       ['FCM','0','raw:FCM','unknown'], // Unknown
       ['FCM','1','raw:FCM','c_-_concrete'], // C - Concrete
       ['FCM','2','raw:FCM','l_-_concrete_and_steel'], // L - Concrete and Steel
       ['FCM','3','raw:FCM','d_-_concrete_and_wood'], // D - Concrete and Wood
       ['FCM','4','raw:FCM','m_-_masonry'], // M - Masonry
       ['FCM','5','raw:FCM','n_-_masonry_and_steel'], // N - Masonry and Steel
       ['FCM','6','raw:FCM','r_-_masonry_and_wood'], // R - Masonry and Wood
       ['FCM','7','raw:FCM','s_-_steel'], // S - Steel
       ['FCM','8','raw:FCM','t_-_steel_and_wood'], // T - Steel and Wood
       ['FCM','9','raw:FCM','w_-_wood'], // W - Wood

       // FCO - Feature Configuration
       ['FCO','0',undefined,undefined], // Unknown
       ['FCO','2','arrangement','multiple'], // Multiple
       ['FCO','3','arrangement','single'], // Single
       ['FCO','5','feature_configuration','same'], // Divided same widths
       ['FCO','6','feature_configuration','different'], // Divided different widths
       ['FCO','7','feature_configuration','non-divided'], // Non-divided
       ['FCO','8','arrangement','poorly_defined'], // Poorly defined
       ['FCO','9','arrangement','well-defined'], // Well-defined
       ['FCO','11','arrangement','double'], // Double
       ['FCO','12','arrangement','juxtaposition'], // Juxtaposition

       // FDM - Fuel Dispensing Method
       ['FDM','0','raw:FDM','unknown'], // Unknown
       ['FDM','1','raw:FDM','available'], // Available
       ['FDM','3','raw:FDM','tank_trucks'], // Tank Trucks
       ['FDM','4','raw:FDM','hydrants'], // Hydrants
       ['FDM','5','raw:FDM','manual'], // Manual
       ['FDM','997',undefined,undefined], // Unpopulated

       // FDT - Fog Detector
       ['FDT','0','raw:FDT','unknown'], // Unknown
       ['FDT','1','raw:FDT','fog_detector_light_present'], // Fog Detector Light Present
       ['FDT','2','raw:FDT','fog_detector_light_absent'], // Fog Detector Light Absent

       // FEC - Final Evaluation Code
       ['FEC','0','raw:FEC','0_-_unknown'], // 0 - Unknown
       ['FEC','1','raw:FEC','1_-_best'], // 1 - Best
       ['FEC','2','raw:FEC','2_-_fair'], // 2 - Fair
       ['FEC','3','raw:FEC','3_-_poor'], // 3 - Poor
       ['FEC','4','raw:FEC','4_-_worst'], // 4 - Worst
       ['FEC','997',undefined,undefined], // Unpopulated

       // FER - Ferry Type
       ['FER','0','raw:FER','unknown'], // Unknown
       ['FER','1','raw:FER','with_cables/chains'], // With cables/chains
       ['FER','2','raw:FER','without_cables/chains'], // Without cables/chains
       ['FER','997',undefined,undefined], // Unpopulated

       // FHC - Harbor Facility Classification
       ['FHC','0','harbour:use','undefined'], // Undefined
       ['FHC','1','harbour:use','ro-ro_terminal_(roll_on,_roll_off)'], // Ro-Ro terminal (Roll on, Roll off)
       ['FHC','2','harbour:use','timber_yard'], // Timber yard
       ['FHC','3','harbour:use','ferry_terminal'], // Ferry Terminal
       ['FHC','4','harbour:use','fishing_harbor'], // Fishing Harbor
       ['FHC','5','harbour:use','yacht_harbor/marina'], // Yacht harbor/marina
       ['FHC','6','harbour:use','naval_base'], // Naval base
       ['FHC','7','harbour:use','tanker_terminal'], // Tanker terminal
       ['FHC','8','harbour:use','passenger_terminal'], // Passenger terminal
       ['FHC','9','harbour:use','shipyard'], // Shipyard
       ['FHC','10','harbour:use','container_terminal'], // Container terminal

       // FLT - Floodlit Illumination
       ['FLT','0','raw:FLT','unknown'], // Unknown
       ['FLT','1','raw:FLT','floodlit'], // Floodlit
       ['FLT','2','raw:FLT','not_floodlit'], // Not Floodlit

       // FON - Type of Font
       ['FON','1','raw:FON','machine_default'], // Machine Default

       // FPT - Facility Priority Type
       ['FPT','1','raw:FPT','major'], // Major
       ['FPT','2','raw:FPT','minor_(hard)'], // Minor (Hard)
       ['FPT','3','raw:FPT','minor_(soft)'], // Minor (Soft)
       ['FPT','4','raw:FPT','minor_(undifferentiated)'], // Minor (Undifferentiated)
       ['FPT','997',undefined,undefined], // Unpopulated

       // FRT - Firing Range Type
       ['FRT','0','raw:FRT','unknown'], // Unknown
       ['FRT','1','raw:FRT','rifle/small_arms'], // Rifle/Small Arms
       ['FRT','2','raw:FRT','tank'], // Tank
       ['FRT','3','raw:FRT','artillery'], // Artillery
       ['FRT','4','raw:FRT','grenade'], // Grenade
       ['FRT','5','raw:FRT','demolition_area'], // Demolition Area
       ['FRT','6','raw:FRT','impact_area'], // Impact Area
       ['FRT','999','raw:FRT','other'], // Other

       // FTC - Farming Type Category
       ['FTC','0','raw:FTC','unknown'], // Unknown
       ['FTC','1','raw:FTC','slash_and_burn-shifting_cultivation'], // Slash & Burn-Shifting cultivation
       ['FTC','3','raw:FTC','terraced'], // Terraced
       ['FTC','4','raw:FTC','ditch_irrigation'], // Ditch Irrigation
       ['FTC','8','raw:FTC','crop_rotation'], // Crop Rotation
       ['FTC','999','raw:FTC','other'], // Other

       // FTI - Fence Type Indicator
       ['FTI','0','raw:FTI','unknown'], // Unknown
       ['FTI','1','raw:FTI','metal'], // Metal
       ['FTI','2','raw:FTI','wood'], // Wood
       ['FTI','3','raw:FTI','stone'], // Stone
       ['FTI','5','raw:FTI','barbed_wire'], // Barbed Wire
       ['FTI','6','raw:FTI','chain_link'], // Chain Link
       ['FTI','999','raw:FTI','other'], // Other

       // FTP - Fabrication Type
       ['FTP','0','raw:FTP','unknown'], // Unknown
       ['FTP','1','raw:FTP','light_fabrication_(light_fabrication_industries_are_characterized_by_light_steel_or_woodframe_buildings_and_lack_heavy_equipment.)'], // Light Fabrication (Light fabrication industries are characterized by light steel or woodframe buildings and lack heavy equipment.)
       ['FTP','2','raw:FTP','heavy_fabrication_(heavy_fabrication_industries_are_characterized_by_large_heavy_steel_frame_buildings_and_may_utilize_large_cranes_for_heavy_lifting.)'], // Heavy Fabrication (Heavy fabrication industries are characterized by large heavy steel frame buildings and may utilize large cranes for heavy lifting.)
       ['FTP','997',undefined,undefined], // Unpopulated

       // GD4 - Defense SAM Site
       ['GD4','0','raw:GD4','unknown'], // Unknown
       ['GD4','1','raw:GD4','sam'], // SAM
       ['GD4','2','raw:GD4','aaa'], // AAA
       ['GD4','997',undefined,undefined], // Unpopulated

       // GEC - Geopolitical Entity Code
       ['GEC','0','geopolitical_entity','unknown'], // Unknown
       ['GEC','1','geopolitical_entity','country'], // Country
       ['GEC','2','geopolitical_entity','country_dependency'], // Country Dependency
       ['GEC','3','geopolitical_entity','organizational_dependency'], // Organizational Dependency
       ['GEC','4','geopolitical_entity','area_of_special_sovereignty'], // Area Of Special Sovereignty
       ['GEC','5','geopolitical_entity','area_of_no_sovereignty'], // Area Of No Sovereignty
       ['GEC','6','geopolitical_entity','administrative_subdivision'], // Administrative Subdivision
       ['GEC','7','geopolitical_entity','demilitarized_zone'], // Demilitarized Zone
       ['GEC','8','geopolitical_entity','zone_of_occupation'], // Zone Of Occupation
       ['GEC','999','geopolitical_entity','other'], // Other

       // GNC - Gate (Nautical) Classification
       ['GNC','0','raw:GNC','undefined'], // Undefined
       ['GNC','2','raw:GNC','tidal_gate_(flood_barrage)'], // Tidal Gate (Flood Barrage)
       ['GNC','3','raw:GNC','caisson'], // Caisson
       ['GNC','4','raw:GNC','lock_gate'], // Lock Gate

       // GSC - Ground Slope Category
       ['GSC','0','raw:GSC','unknown'], // Unknown
       ['GSC','1','raw:GSC','(0_to_>_45)_culturally_or_naturally_dissected_land'], // (0 to > 45) Culturally or Naturally Dissected Land
       ['GSC','2','raw:GSC','<=_3'], // <= 3
       ['GSC','3','raw:GSC','>_3_and_<=_10'], // > 3 and <= 10
       ['GSC','4','raw:GSC','>_10_and_<=_20'], // > 10 and <= 20
       ['GSC','5','raw:GSC','>_20_and_<=_30'], // > 20 and <= 30
       ['GSC','6','raw:GSC','>_30_and_<=_45'], // > 30 and <= 45
       ['GSC','7','raw:GSC','>_45'], // > 45

       // GTC - Gate Type Category
       ['GTC','0','raw:GTC','unknown'], // Unknown
       ['GTC','1','raw:GTC','tollgate'], // Tollgate
       ['GTC','2','raw:GTC','crossing'], // Crossing
       ['GTC','999','raw:GTC','other'], // Other

       // GTP - Geomorphic Type
       ['GTP','0','raw:GTP','unknown'], // Unknown
       ['GTP','1','raw:GTP','hill_top_or_peak'], // Hill Top or Peak
       ['GTP','2','raw:GTP','depression_bottom'], // Depression Bottom
       ['GTP','3','raw:GTP','escarpment/cliff_top'], // Escarpment/Cliff Top
       ['GTP','4','raw:GTP','escarpment/cliff_bottom'], // Escarpment/Cliff Bottom
       ['GTP','5','raw:GTP','fault/fault-line_scarp_top'], // Fault/Fault-Line Scarp Top
       ['GTP','6','raw:GTP','fault/fault-line_scarp_bottom'], // Fault/Fault-Line Scarp Bottom
       ['GTP','7','raw:GTP','centerline_drain'], // Centerline Drain
       ['GTP','8','raw:GTP','ridge_line'], // Ridge Line
       ['GTP','9','raw:GTP','valley_line'], // Valley Line
       ['GTP','10','raw:GTP','sharp_or_abrupt_slope_change'], // Sharp or Abrupt Slope Change
       ['GTP','11','raw:GTP','na'], // NA

       // GUG - Guyed or Unguyed Category
       ['GUG','0','guyed','unknown'], // Unknown
       ['GUG','1','guyed','yes'], // Guyed
       ['GUG','2','guyed','no'], // Unguyed
       ['GUG','997',undefined,undefined], // Unpopulated
       ['GUG','999','guyed','other'], // Other

       // GW1 - Gap Width Range (1)
       ['GW1','0','raw:GW1','unknown'], // Unknown
       ['GW1','1','raw:GW1','<=_3'], // <= 3
       ['GW1','2','raw:GW1','>_3_and_<=_18'], // > 3 and <= 18
       ['GW1','3','raw:GW1','>_18_and_<=_25'], // > 18 and <= 25
       ['GW1','4','raw:GW1','>_25_and_<=_50'], // > 25 and <= 50
       ['GW1','5','raw:GW1','>_50_and_<=_75'], // > 50 and <= 75
       ['GW1','6','raw:GW1','>_75_and_<=_100'], // > 75 and <= 100
       ['GW1','7','raw:GW1','>_100_and_<=_142'], // > 100 and <= 142
       ['GW1','8','raw:GW1','>_142'], // > 142
       // ['GW1','9',undefined,undefined], // Not Applicable
       ['GW1','9','raw:GW1','not_applicable'], // Not Applicable
       ['GW1','997',undefined,undefined], // Unpopulated

       // GW3 - Gap Width Range (3)
       ['GW3','0','raw:GW3','unknown'], // Unknown
       ['GW3','1','raw:GW3','<=1.5'], // <=1.5
       ['GW3','2','raw:GW3','>1.5_and_<=3.0'], // >1.5 and <=3.0
       ['GW3','3','raw:GW3','>3.0_and_<=18.0'], // >3.0 and <=18.0
       ['GW3','4','raw:GW3','>18.0_and_<=25.0'], // >18.0 and <=25.0
       ['GW3','5','raw:GW3','>25.0_and_<=30.0'], // >25.0 and <=30.0
       ['GW3','6','raw:GW3','>30.0_and_<=35.0'], // >30.0 and <=35.0
       ['GW3','7','raw:GW3','>35.0_and_<=40.0'], // >35.0 and <=40.0
       ['GW3','8','raw:GW3','>40.0_and_<=45.0'], // >40.0 and <=45.0
       ['GW3','9','raw:GW3','>45.0_and_<=50.0'], // >45.0 and <=50.0
       ['GW3','10','raw:GW3','>50.0_and_<=75.0'], // >50.0 and <=75.0
       ['GW3','11','raw:GW3','>75.0_and_<=100.0'], // >75.0 and <=100.0
       ['GW3','12','raw:GW3','>100.0_and_<=142.0'], // >100.0 and <=142.0
       ['GW3','13','raw:GW3','>142.00'], // >142.00

       // HAS_INFO - Has Additional Information Flag
       ['HAS_INFO','N',undefined,undefined], // No
       ['HAS_INFO','Y','gifd:additional_info','yes'], // Yes

       // HCC - Horizontal Clearance Code
       ['HCC','0','horizontal:clearance:code','unknown'], // Unknown
       ['HCC','1','horizontal:clearance:code','restricted'], // Restricted
       ['HCC','2','horizontal:clearance:code','unlimited'], // Unlimited

       // HDI - Hydrographic Depth/Height Information
       ['HDI','9','depth:hydrographic:information','depth_known_by_other_than_wire_drag'], // Depth Known by Other Than Wire Drag
       ['HDI','10','depth:hydrographic:information','depth_known_by_wire_drag'], // Depth Known by Wire Drag
       ['HDI','11','depth:hydrographic:information','depth_unknown_but_safe_to_depth_shown'], // Depth Unknown But Safe to Depth Shown
       ['HDI','12','depth:hydrographic:information','depth_unknown'], // Depth Unknown
       ['HDI','13','depth:hydrographic:information','uncovering_height_known'], // Uncovering Height Known
       ['HDI','14','depth:hydrographic:information','uncovering_height_unknown'], // Uncovering Height Unknown
       // ['HDI','15',undefined,undefined], // Not Applicable
       ['HDI','15','depth:hydrographic:information','not_applicable'], // Not Applicable
       ['HDI','997',undefined,undefined], // Unpopulated

       // HFC - Hydrological Form Category
       ['HFC','0','raw:HFC','unknown'], // Unknown
       ['HFC','1','raw:HFC','channelized_stream'], // Channelized Stream
       ['HFC','2','raw:HFC','disappearing'], // Disappearing
       ['HFC','8','raw:HFC','normal_channel'], // Normal Channel
       ['HFC','14','raw:HFC','braided'], // Braided
       ['HFC','16','raw:HFC','dissipating'], // Dissipating
       ['HFC','19','raw:HFC','gorge'], // Gorge
       ['HFC','21','raw:HFC','wadi/wash'], // Wadi/Wash
       ['HFC','997',undefined,undefined], // Unpopulated
       ['HFC','999','raw:HFC','other'], // Other

       // HL1 - Bank Height Left (1)
       ['HL1','0','raw:HL1','unknown'], // Unknown
       ['HL1','1','raw:HL1','<=_.5'], // <= .5
       ['HL1','2','raw:HL1','>_.5_and_<=_1.0'], // > .5 and <= 1.0
       ['HL1','3','raw:HL1','>_1.0_and_<=_5.0'], // > 1.0 and <= 5.0
       ['HL1','4','raw:HL1','>_5.0'], // > 5.0
       ['HL1','997',undefined,undefined], // Unpopulated

       // HL3 - Bank Height Left (3)
       ['HL3','0','raw:HL3','unknown'], // Unknown
       ['HL3','1','raw:HL3','<=0.2'], // <=0.2
       ['HL3','2','raw:HL3','>0.2_and_<=0.5'], // >0.2 and <=0.5
       ['HL3','3','raw:HL3','>0.5_and_<=1.0'], // >0.5 and <=1.0
       ['HL3','4','raw:HL3','>1.0_and_<=1.5'], // >1.0 and <=1.5
       ['HL3','5','raw:HL3','>1.5_and_<=2.0'], // >1.5 and <=2.0
       ['HL3','6','raw:HL3','>2.0_and_<=5.0'], // >2.0 and <=5.0
       ['HL3','7','raw:HL3','>5.0'], // >5.0

       // HLT - Hydrographic Light Type
       ['HLT','0','seamark:light:category','unknown'], // Unknown
       ['HLT','1','seamark:light:category','sectored_light'], // Sectored Light
       ['HLT','3','seamark:light:category','moire'], // Moir Effect Light
       ['HLT','4','seamark:light:category','strip_light'], // Strip Light
       ['HLT','6','seamark:light:category','lighted_beacon'], // Lighted Beacon
       ['HLT','7','seamark:light:category','directional'], // Directional Light
       ['HLT','997',undefined,undefined], // Unpopulated
       ['HLT','999','seamark:light:category','other'], // Other

       // HOC - Hydrographic Origin Category
       ['HOC','0','raw:HOC','unknown'], // Unknown
       ['HOC','1','raw:HOC','controlled'], // Controlled
       ['HOC','4','raw:HOC','man-made'], // Man-Made
       ['HOC','5','raw:HOC','natural'], // Natural
       ['HOC','997',undefined,undefined], // Unpopulated

       // HQC - Hypsography Portrayal Category
       ['HQC','0','contour:type','unknown'], // Unknown
       ['HQC','1','contour:type','index_contour'], // Index Contour
       ['HQC','2','contour:type','intermediate_contour'], // Intermediate Contour
       ['HQC','3','contour:type','half_auxiliary_contour'], // Half Auxiliary Contour
       ['HQC','4','contour:type','form_line'], // Form Line
       ['HQC','5','contour:type','depression_index_contour'], // Depression Index Contour
       ['HQC','6','contour:type','depression_intermediate_contour'], // Depression Intermediate Contour
       ['HQC','8','contour:type','mound_index_contour'], // Mound Index Contour
       ['HQC','9','contour:type','mound_intermediate_contour'], // Mound Intermediate Contour
       ['HQC','14','contour:type','quarter_auxiliary_contour'], // Quarter Auxiliary Contour
       ['HQC','19','contour:type','intermediate_carrying_contour'], // Intermediate Carrying Contour
       ['HQC','20','contour:type','auxiliary_carrying_contour'], // Auxiliary Carrying Contour
       ['HQC','21','contour:type','index_carrying_contour'], // Index Carrying Contour
       ['HQC','22','contour:type','depression_auxiliary_contour'], // Depression Auxiliary Contour
       ['HQC','997',undefined,undefined], // Unpopulated

       // HR1 - Bank Height Right (1)
       ['HR1','0','raw:HR1','unknown'], // Unknown
       ['HR1','1','raw:HR1','<=_.5'], // <= .5
       ['HR1','2','raw:HR1','>_.5_and_<=_1.0'], // > .5 and <= 1.0
       ['HR1','3','raw:HR1','>_1.0_and_<=_5.0'], // > 1.0 and <= 5.0
       ['HR1','4','raw:HR1','>_5.0'], // > 5.0
       ['HR1','997',undefined,undefined], // Unpopulated

       // HR3 - Bank Height Right (3)
       ['HR3','0','raw:HR3','unknown'], // Unknown
       ['HR3','1','raw:HR3','<=0.2'], // <=0.2
       ['HR3','2','raw:HR3','>0.2_and_<=0.5'], // >0.2 and <=0.5
       ['HR3','3','raw:HR3','>0.5_and_<=1.0'], // >0.5 and <=1.0
       ['HR3','4','raw:HR3','>1.0_and_<=1.5'], // >1.0 and <=1.5
       ['HR3','5','raw:HR3','>1.5_and_<=2.0'], // >1.5 and <=2.0
       ['HR3','6','raw:HR3','>2.0_and_<=5.0'], // >2.0 and <=5.0
       ['HR3','7','raw:HR3','>5.0'], // >5.0

       // HS1 - Current Information (1)
       ['HS1','0',undefined,undefined], // Unknown/Not Applicable
       ['HS1','1','water:current:appearance','jan'], // Jan
       ['HS1','2','water:current:appearance','feb'], // Feb
       ['HS1','3','water:current:appearance','mar'], // Mar
       ['HS1','4','water:current:appearance','apr'], // Apr
       ['HS1','5','water:current:appearance','may'], // May
       ['HS1','6','water:current:appearance','jun'], // Jun
       ['HS1','7','water:current:appearance','jul'], // Jul
       ['HS1','8','water:current:appearance','aug'], // Aug
       ['HS1','9','water:current:appearance','sep'], // Sep
       ['HS1','10','water:current:appearance','oct'], // Oct
       ['HS1','11','water:current:appearance','nov'], // Nov
       ['HS1','12','water:current:appearance','dec'], // Dec

       // HS2 - Current Information (2)
       ['HS2','0',undefined,undefined], // Unknown/Not Applicable
       ['HS2','1','water:current:disappearance','jan'], // Jan
       ['HS2','2','water:current:disappearance','feb'], // Feb
       ['HS2','3','water:current:disappearance','mar'], // Mar
       ['HS2','4','water:current:disappearance','apr'], // Apr
       ['HS2','5','water:current:disappearance','may'], // May
       ['HS2','6','water:current:disappearance','jun'], // Jun
       ['HS2','7','water:current:disappearance','jul'], // Jul
       ['HS2','8','water:current:disappearance','aug'], // Aug
       ['HS2','9','water:current:disappearance','sep'], // Sep
       ['HS2','10','water:current:disappearance','oct'], // Oct
       ['HS2','11','water:current:disappearance','nov'], // Nov
       ['HS2','12','water:current:disappearance','dec'], // Dec

       // HTC - Hull Integrity Test Site Characteristics
       ['HTC','0','raw:HTC','unknown'], // Unknown
       ['HTC','501','raw:HTC','smooth,_hard_bottom'], // Smooth, hard bottom
       ['HTC','502','raw:HTC','rough,_hard_bottom'], // Rough, hard bottom
       ['HTC','503','raw:HTC','rock_and_ledges_greater_than_2_m_in_height'], // Rock and ledges greater than 2 m in height
       ['HTC','504','raw:HTC','rock_and_ledges_less_than_or_equal_to_2_m_in_height'], // Rock and ledges less than or equal to 2 m in height
       ['HTC','505','raw:HTC','feature_0.6_m_or_greater_in_height'], // Feature 0.6 m or greater in height
       ['HTC','506','raw:HTC','scour_channels_-_slope_may_exceed_10_degrees'], // Scour channels - slope may exceed 10 degrees
       ['HTC','507','raw:HTC','slope_exceeding_10_degrees'], // Slope exceeding 10 degrees
       ['HTC','508','raw:HTC','slumping_where_local_slope_may_exceed_10_degrees'], // Slumping where local slope may exceed 10 degrees
       ['HTC','509','raw:HTC','cable'], // Cable
       ['HTC','510','raw:HTC','depths_within_test_zone_which_are_shallower_than_zone_limits'], // Depths within test zone which are shallower than zone limits
       ['HTC','511','raw:HTC','depths_within_test_zone_which_are_deeper_than_zone_limits'], // Depths within test zone which are deeper than zone limits
       // ['HTC','998',undefined,undefined], // Not Applicable
       ['HTC','998','raw:HTC','not_applicable'], // Not Applicable
       ['HTC','999','raw:HTC','other'], // Other

       // HTP - Hangar Type Category
       ['HTP','0','raw:HTP','unknown'], // Unknown
       ['HTP','1','raw:HTP','m_-_multi-bay'], // M - Multi-bay
       ['HTP','2','raw:HTP','o_-_open_end'], // O - Open end
       ['HTP','3','raw:HTP','n_-_nose_in'], // N - Nose in
       ['HTP','4','raw:HTP','g_-_underground'], // G - Underground
       ['HTP','5','raw:HTP','s_-_single_bay'], // S - Single bay
       ['HTP','6','raw:HTP','d_-_double_bay'], // D - Double bay
       ['HTP','7','raw:HTP','t_-_t-shaped'], // T - T-Shaped
       ['HTP','997',undefined,undefined], // Unpopulated

       // HTR - Height Range
       ['HTR','0','raw:HTR','unknown'], // Unknown
       ['HTR','1','raw:HTR','<=0.5'], // <=0.5
       ['HTR','2','raw:HTR','>0.5_and_<=1.0'], // >0.5 and <=1.0
       ['HTR','3','raw:HTR','>1.0_and_<=1.5'], // >1.0 and <=1.5
       ['HTR','4','raw:HTR','>1.5_and_<=2.0'], // >1.5 and <=2.0
       ['HTR','5','raw:HTR','>2.0_and_<=5.0'], // >2.0 and <=5.0
       ['HTR','6','raw:HTR','>5.0_and_<=10.0'], // >5.0 and <=10.0
       ['HTR','7','raw:HTR','>10.0_and_<=20.0'], // >10.0 and <=20.0
       ['HTR','8','raw:HTR','>20.0_and_<=35.0'], // >20.0 and <=35.0
       ['HTR','9','raw:HTR','>35.0'], // >35.0

       // HTZ - Hull Integrity Test Site Test Zone
       ['HTZ','501','raw:HTZ','zone_a'], // Zone A
       ['HTZ','502','raw:HTZ','zone_b'], // Zone B
       ['HTZ','503','raw:HTZ','zone_c'], // Zone C
       ['HTZ','504','raw:HTZ','zone_d'], // Zone D
       ['HTZ','505','raw:HTZ','zone_e'], // Zone E
       // ['HTZ','998',undefined,undefined], // Not Applicable
       ['HTZ','998','raw:HTZ','not_applicable'], // Not Applicable

       // HWT - House of Worship Type
       ['HWT','0',undefined,undefined], // Unknown
       ['HWT','2','building','cathedral'], // Cathedral
       ['HWT','3','building','chapel'], // Chapel
       ['HWT','4','building','church'], // Church
       ['HWT','5','building','marabout'], // Marabout
       ['HWT','6','tower:type','minaret'], // Minaret
       ['HWT','7','building','religious_community'], // Monastery, Convent
       ['HWT','9','building','mosque'], // Mosque
       ['HWT','11','building','pagoda'], // Pagoda
       ['HWT','14','building','shrine'], // Shrine
       ['HWT','15','building','tabernacle'], // Tabernacle
       ['HWT','16','building','temple'], // Temple
       ['HWT','20','building','synagogue'], // Synagogue
       ['HWT','21','building','stupa'], // Stupa
       ['HWT','997',undefined,undefined], // Unpopulated
       // ['HWT','998',undefined,undefined], // Not Applicable
       ['HWT','998','building:religious','not_applicable'], // Not Applicable

       // HYC - Hydrological Category
       ['HYC','0','intermittent','unknown'], // Unknown
       // ['HYC','2',undefined,undefined], // Not Applicable
       ['HYC','2','intermittent','not_applicable'], // Not Applicable
       ['HYC','3','intermittent','dry'], // Dry
       ['HYC','6','intermittent','yes'], // Non-Perennial/Intermittent/Fluctuating
       ['HYC','8','intermittent','no'], // Perennial/Permanent
       ['HYC','997',undefined,undefined], // Unpopulated
       ['HYC','999','intermittent','other'], // Other

       // HZD - Horizontal Datum
       ['HZD','0','source:datum:geodetic','unknown'], // Unknown
       ['HZD','300','source:datum:geodetic','world_geodetic_system_1984'], // World Geodetic System 1984
       ['HZD','999','source:datum:geodetic','other'], // Other

       // IAC - IALA Aid Category
       ['IAC','0','iala_navigation_aid','unknown'], // Unknown
       ['IAC','1','iala_navigation_aid','no'], // Non-IALA Aid
       ['IAC','3','iala_navigation_aid','region_a'], // IALA Aid - Region A
       ['IAC','4','iala_navigation_aid','region_b'], // IALA Aid - Region B
       ['IAC','997',undefined,undefined], // Unpopulated

       // IAS - IMO Adoption Status
       ['IAS','1','imo_adopted','yes'], // Adopted
       ['IAS','2','imo_adopted','no'], // Not Adopted

       // IBC - Installation Buoy Classification
       ['IBC','0','raw:IBC','undefined'], // Undefined
       ['IBC','1','raw:IBC','catenary_anchor_leg_mooring_(calm)'], // Catenary Anchor Leg Mooring (CALM)
       ['IBC','2','raw:IBC','single_buoy_mooring_(sbm)'], // Single Buoy Mooring (SBM)
       // ['IBC','998',undefined,undefined], // Not Applicable
       ['IBC','998','raw:IBC','not_applicable'], // Not Applicable

       // ICL - ICAO Airspace Classification
       ['ICL','0','raw:ICL','unknown'], // Unknown
       ['ICL','1','raw:ICL','class_a'], // Class A
       ['ICL','2','raw:ICL','class_b'], // Class B
       ['ICL','3','raw:ICL','class_c'], // Class C
       ['ICL','4','raw:ICL','class_d'], // Class D
       ['ICL','5','raw:ICL','class_e'], // Class E
       ['ICL','6','raw:ICL','class_f'], // Class F
       ['ICL','7','raw:ICL','class_g'], // Class G
       // ['ICL','998',undefined,undefined], // Not Applicable
       ['ICL','998','raw:ICL','not_applicable'], // Not Applicable

       // IWO - Inland Water Obstruction
       ['IWO','0','waterway:obstruction','unknown'], // Unknown
       ['IWO','1','waterway:obstruction','yes'], // Obstruction
       ['IWO','2','waterway:obstruction','no'], // Not an obstruction

       // LAF - Line Associated Features
       ['LAF','0','navigation_line_feature','unknown'], // Unknown
       ['LAF','1','navigation_line_feature','one_other_than_directional_light'], // One Object (Other than a Directional Light)
       ['LAF','2','navigation_line_feature','directional_light'], // Directional Light
       ['LAF','3','navigation_line_feature','two_lights_or_more'], // Two or more lights
       ['LAF','4','navigation_line_feature','two_beacons_or_more'], // Two or more beacons
       ['LAF','5','navigation_line_feature','two_other_than_two_lights_or_beacons'], // Two or More Objects (Other Than Two Lights or Beacons)
       ['LAF','6','navigation_line_feature','measured_distance_markers'], // Measured Distance Markers
       ['LAF','7','navigation_line_feature','directional_radiobeacon'], // Directional Radiobeacon
       ['LAF','8','navigation_line_feature','moir_effect_light'], // Moir Effect Light
       ['LAF','9','navigation_line_feature','leading_radio_transponder_beacon'], // Leading Radio Transponder Beacon
       ['LAF','997',undefined,undefined], // Unpopulated
       // ['LAF','998',undefined,undefined], // Not Applicable
       ['LAF','998','navigation_line_feature','not_applicable'], // Not Applicable

       // LFA - Light Function Aeronautical
       ['LFA','0','raw:LFA','unknown'], // Unknown
       ['LFA','1','raw:LFA','airport_terminal_lights'], // Airport Terminal Lights
       ['LFA','2','raw:LFA','apron_flood'], // Apron Flood
       ['LFA','3','raw:LFA','boundary_lights'], // Boundary Lights
       ['LFA','4','raw:LFA','runway_centerline_lighting'], // Runway Centerline Lighting
       ['LFA','5','raw:LFA','runway_end_identification_lighting_(reil)'], // Runway End Identification Lighting (REIL)
       ['LFA','6','raw:LFA','runway_lights/runway_edge_lights'], // Runway Lights/Runway Edge Lights
       ['LFA','7','raw:LFA','sequenced_strobe'], // Sequenced Strobe
       ['LFA','8','raw:LFA','taxiway_lighting'], // Taxiway Lighting
       ['LFA','9','raw:LFA','visual_approach_slope_indicator_(vasi)'], // Visual Approach Slope Indicator (VASI)
       ['LFA','10','raw:LFA','rotating_beacon'], // Rotating Beacon
       ['LFA','11','raw:LFA','obstruction_lighting'], // Obstruction Lighting
       ['LFA','12','raw:LFA','threshold_light(s)'], // Threshold Light(s)
       ['LFA','13','raw:LFA','touchdown_zone_lighting'], // Touchdown Zone Lighting
       ['LFA','14','raw:LFA','other_airport_lighting'], // Other Airport Lighting
       ['LFA','15','raw:LFA','alsf-1_(approach_lighting_system_with_seq._flashing)'], // ALSF-1 (Approach Lighting System with seq. flashing)
       ['LFA','16','raw:LFA','alsf-ii'], // ALSF-II
       ['LFA','17','raw:LFA','(ssalf)'], // (SSALF)
       ['LFA','18','raw:LFA','(ssalr)'], // (SSALR)
       ['LFA','19','raw:LFA','(malsf)'], // (MALSF)
       ['LFA','20','raw:LFA','(malsr)'], // (MALSR)
       ['LFA','21','raw:LFA','landing_direction_indicator_(ldin)'], // Landing Direction Indicator (LDIN)
       ['LFA','22','raw:LFA','rail_(runway_alignment_indicator_lights)'], // RAIL (Runway Alignment Indicator Lights)
       ['LFA','23','raw:LFA','odals_(omni_directional_approach_landing_system)'], // ODALS (Omni Directional Approach Landing System)
       ['LFA','24','raw:LFA','other_approach_lighting'], // Other Approach Lighting
       ['LFA','25','raw:LFA','precision_approach_path_indicator_(papi)'], // Precision Approach Path Indicator (PAPI)
       ['LFA','26','raw:LFA','strobe'], // Strobe
       ['LFA','27','raw:LFA','runway_flood'], // Runway Flood
       ['LFA','28','raw:LFA','variable_intensity_runway_lights'], // Variable Intensity Runway Lights
       ['LFA','29','raw:LFA','portable_runway_lights'], // Portable Runway Lights
       ['LFA','30','raw:LFA','flares'], // Flares
       ['LFA','31','raw:LFA','wind_indicator_lights'], // Wind Indicator Lights
       ['LFA','32','raw:LFA','visual_approach_slope_indicator_(3_bar)'], // Visual Approach Slope Indicator (3 bar)
       ['LFA','33','raw:LFA','optical_landing_system'], // Optical Landing System
       ['LFA','53','raw:LFA','beacon'], // Beacon
       ['LFA','72','raw:LFA','identification_beacon'], // Identification Beacon
       ['LFA','73','raw:LFA','none_available'], // None available
       ['LFA','100','raw:LFA','overrun_centerline'], // Overrun centerline
       ['LFA','101','raw:LFA','centerline_and_bar'], // Centerline and bar
       ['LFA','102','raw:LFA','u.s._configuration_(b)'], // U.S. Configuration (B)
       ['LFA','103','raw:LFA','hong_kong_curve'], // Hong Kong Curve
       ['LFA','104','raw:LFA','left_single_row'], // Left single row
       ['LFA','105','raw:LFA','center_row'], // Center row
       ['LFA','106','raw:LFA','former_nato_standard'], // Former NATO standard
       ['LFA','107','raw:LFA','nato_standard'], // NATO standard
       ['LFA','108','raw:LFA','center_and_double_row'], // Center and double row
       ['LFA','109','raw:LFA','portable_approach'], // Portable approach
       ['LFA','110','raw:LFA','center_row_(cat_2_high_visibility)'], // Center row (Cat 2 high visibility)
       ['LFA','111','raw:LFA','center_row_(cat_1_high_intensity)'], // Center row (Cat 1 high intensity)
       ['LFA','112','raw:LFA','navy_parallel_row_and_crossbar'], // Navy parallel row and crossbar
       ['LFA','113','raw:LFA','two_parallel_row'], // Two parallel row
       ['LFA','114','raw:LFA','left_row_(high_intensity)'], // Left row (High intensity)
       ['LFA','115','raw:LFA','air_force_overrun'], // Air Force overrun
       ['LFA','116','raw:LFA','calvert_(british)'], // Calvert (British)
       ['LFA','117','raw:LFA','single_row_centerline'], // Single row centerline
       ['LFA','118','raw:LFA','narrow_multi_cross'], // Narrow multi cross
       ['LFA','119','raw:LFA','centerline_(high_intensity_approach_lights)'], // Centerline (High intensity approach lights)
       ['LFA','120','raw:LFA','alternate_centerline_and_bar_approach_lights'], // Alternate centerline and bar approach lights
       ['LFA','121','raw:LFA','cross'], // Cross
       ['LFA','122','raw:LFA','neon_ladder'], // Neon ladder
       ['LFA','123','raw:LFA','singapore_centerline_approach_lights'], // Singapore centerline approach lights
       ['LFA','124','raw:LFA','centerline_2_crossbars_approach_lights'], // Centerline 2 crossbars approach lights
       ['LFA','125','raw:LFA','t-vasi_t-bar'], // T-VASI T-bar
       ['LFA','126','raw:LFA','pvasi_pulsating'], // PVASI pulsating
       ['LFA','127','raw:LFA','apap_alignment_of_elements_systems'], // APAP alignment of elements systems
       ['LFA','128','raw:LFA','vasi_(with_indicator_for_threshold_crossing_height)'], // VASI (with indicator for threshold crossing height)
       ['LFA','129','raw:LFA','lcvasi_low_cost'], // LCVASI low cost
       ['LFA','130','raw:LFA','high_intensity_runway_lights'], // High intensity runway lights
       ['LFA','131','raw:LFA','medium_intensity_runway_lights'], // Medium intensity runway lights
       ['LFA','132','raw:LFA','low_intensity_runway_lights'], // Low intensity runway lights
       ['LFA','997',undefined,undefined], // Unpopulated
       ['LFA','999','raw:LFA','other'], // Other

       // LMC - Landmark Category
       ['LMC','0',undefined,undefined], // Unknown
       ['LMC','1','navigation:landmark','yes'], // Landmark
       ['LMC','2','navigation:landmark','no'], // Not a landmark
       ['LMC','997',undefined,undefined], // Unpopulated

       // LOC - Location Category
       ['LOC','0','location','unknown'], // Unknown
       ['LOC','1','location','above_surface/does_not_cover_(height_known)'], // Above Surface/Does not Cover (Height Known)
       ['LOC','2','location','awash_at_chart_datum'], // Awash at Chart Datum
       ['LOC','3','location','dries/covers_(height_unknown)'], // Dries/Covers (Height Unknown)
       ['LOC','4','location','underwater'], // Below Water Surface
       ['LOC','8','location','on_ground_surface'], // On Ground Surface
       ['LOC','13','location','hull_showing'], // Hull Showing
       ['LOC','14','location','masts_showing'], // Masts Showing
       ['LOC','15','location','on_water_surface/floating'], // On Water Surface/Floating
       ['LOC','17','location','on_waterbody_bottom'], // Sunken/On sea bottom
       ['LOC','20','location','funnel_showing'], // Funnel Showing
       ['LOC','21','location','superstructure_showing'], // Superstructure Showing
       ['LOC','22','location','off_shore'], // Off Shore
       ['LOC','23','location','below_waterbody_bottom'], // Below Sea Bottom
       ['LOC','24','location','suspended_or_elevated_above_sea_bottom'], // Suspended or Elevated above sea bottom
       ['LOC','25','location','overground'], // Suspended or Elevated Above Ground or Water Surface
       ['LOC','28','location','masts_and_funnel_showing'], // Masts and Funnel Showing
       ['LOC','30','location','non-floating'], // Non-Floating
       ['LOC','39','location','depth_unknown'], // Depth Unknown
       ['LOC','40','location','underground'], // Underground
       ['LOC','997',undefined,undefined], // Unpopulated
       // ['LOC','998',undefined,undefined], // Not Applicable
       ['LOC','998','location_category','not_applicable'], // Not Applicable
       ['LOC','999',undefined,undefined], // Other

       // LSP - Boundary Lines Of Separation Code
       ['LSP','0','geopolitical_line','unknown'], // Unknown
       ['LSP','1','geopolitical_line','administrative'], // Administrative
       ['LSP','2','geopolitical_line','provisional_administrative'], // Provisional Administrative
       ['LSP','3','geopolitical_line','armistice_line'], // Armistice Line
       ['LSP','4','geopolitical_line','line_of_control'], // Line Of Control
       ['LSP','5','geopolitical_line','demarcation_line'], // Demarcation Line
       ['LSP','6','geopolitical_line','line_of_convenience'], // Line Of Convenience
       ['LSP','7','geopolitical_line','cease_fire_line'], // Cease Fire Line
       ['LSP','8','geopolitical_line','mandate_or_convention_line'], // Mandate or Convention Line
       ['LSP','9','geopolitical_line','claim_line'], // Claim Line
       ['LSP','10','geopolitical_line','inter-colonial_line'], // Inter-Colonial Line
       ['LSP','11','geopolitical_line','inter-entity_line'], // Inter-Entity Line
       ['LSP','12','geopolitical_line','line_of_adjacency'], // Line Of Adjacency
       ['LSP','13','geopolitical_line','line_of_withdrawal'], // Line Of Withdrawal
       ['LSP','14','geopolitical_line','military_agreement_line'], // Military Agreement Line
       ['LSP','15','geopolitical_line','treaty_line'], // Treaty Line
       ['LSP','999','geopolitical_line','other'], // Other

       // LWI - Lighted Wind Indicator Availability
       ['LWI','0','raw:LWI','unknown'], // Unknown
       ['LWI','1','raw:LWI','available'], // Available
       ['LWI','2','raw:LWI','not_available'], // Not Available
       ['LWI','997',undefined,undefined], // Unpopulated

       // MAC - Maritime Area Category
       ['MAC','0','maritime_area:type','unknown'], // Unknown
       ['MAC','2','maritime_area:type','dredged_channel/dredged_area'], // Dredged Channel/Dredged Area
       ['MAC','4','maritime_area:type','mine_danger_area'], // Mine Danger Area
       ['MAC','5','maritime_area:type','prohibited_shipping_area/entry_prohibited'], // Prohibited Shipping Area/Entry Prohibited
       ['MAC','6','maritime_area:type','reclamation_area'], // Reclamation Area
       ['MAC','9','maritime_area:type','works_in_progress_area'], // Works in Progress Area
       ['MAC','11','maritime_area:type','anchorage_(general)'], // Anchorage (general)
       ['MAC','12','maritime_area:type','anchoring_berths'], // Anchoring Berths
       ['MAC','13','maritime_area:type','explosives_anchorage'], // Explosives anchorage
       ['MAC','14','maritime_area:type','large_vessel/deep_water/deep_draft_anchorage'], // Large Vessel/Deep Water/Deep Draft anchorage
       ['MAC','15','maritime_area:type','anchoring_prohibited'], // Anchoring Prohibited
       ['MAC','16','maritime_area:type','quarantine_anchorage'], // Quarantine Anchorage
       ['MAC','17','maritime_area:type','reserved_anchorage'], // Reserved Anchorage
       ['MAC','19','maritime_area:type','tanker_anchorage'], // Tanker Anchorage
       ['MAC','20','maritime_area:type','submarine_cable_area'], // Submarine Cable Area
       ['MAC','21','maritime_area:type','pipeline_area'], // Pipeline Area
       ['MAC','22','maritime_area:type','fishing_prohibited'], // Fishing Prohibited
       ['MAC','23','maritime_area:type','cable_and_pipeline_area'], // Cable and Pipeline Area
       ['MAC','24','maritime_area:type','turning_area/swinging_circle'], // Turning Area/Swinging Circle
       ['MAC','25','maritime_area:type','spoil_area/spoil_ground_(see_ops_for_status)'], // Spoil Area/Spoil Ground (See OPS for status)
       ['MAC','26','maritime_area:type','unsurveyed_area'], // Unsurveyed Area
       ['MAC','27','maritime_area:type','submarine_exercise_area'], // Submarine Exercise Area
       ['MAC','28','maritime_area:type','mine_laying_practice_area'], // Mine Laying Practice Area
       ['MAC','29','maritime_area:type','firing_danger_area'], // Firing Danger Area
       ['MAC','30','maritime_area:type','dumping_ground_for_hazardous_materials'], // Dumping Ground for Hazardous Materials
       ['MAC','31','maritime_area:type','incineration_area'], // Incineration Area
       ['MAC','32','maritime_area:type','oil_field'], // Oil Field
       ['MAC','33','maritime_area:type','gas_field'], // Gas Field
       ['MAC','35','maritime_area:type','explosive_dumping_ground'], // Explosive Dumping Ground
       ['MAC','37','maritime_area:type','safety_zone'], // Safety Zone
       ['MAC','40','maritime_area:type','roundabout_zone_(tss)'], // Roundabout Zone (TSS)
       ['MAC','41','maritime_area:type','inshore_traffic_zone_(tss)'], // Inshore Traffic Zone (TSS)
       ['MAC','42','maritime_area:type','precautionary_area'], // Precautionary Area
       ['MAC','43','maritime_area:type','area_to_be_avoided'], // Area to be avoided
       ['MAC','44','maritime_area:type','degaussing_range'], // Degaussing Range
       ['MAC','45','maritime_area:type','outfall_area'], // Outfall Area
       ['MAC','46','maritime_area:type','intake_area'], // Intake Area
       ['MAC','48','maritime_area:type','pilot_boarding_area'], // Pilot Boarding Area
       ['MAC','49','maritime_area:type','cargo_transshipment_area'], // Cargo Transshipment Area
       ['MAC','53','maritime_area:type','seaplane'], // Seaplane
       ['MAC','54','maritime_area:type','time_limited'], // Time Limited
       ['MAC','55','maritime_area:type','fairway'], // Fairway
       ['MAC','56','maritime_area:type','fish_trap_area'], // Fish Trap Area
       ['MAC','57','maritime_area:type','marine_farm'], // Marine Farm
       ['MAC','58','maritime_area:type','dredging_area'], // Dredging Area
       ['MAC','61','maritime_area:type','sewer_area'], // Sewer Area
       ['MAC','101','maritime_area:type','marine_sanctuary'], // Marine Sanctuary
       ['MAC','102','maritime_area:type','waiting_area'], // Waiting Area
       ['MAC','103','maritime_area:type','mineswept_channel'], // Mineswept Channel
       ['MAC','104','maritime_area:type','major_navy_operating_area'], // Major Navy Operating Area
       ['MAC','105','maritime_area:type','minor_navy_operating_area'], // Minor Navy Operating Area
       ['MAC','106','maritime_area:type','asw_operating_area'], // ASW Operating Area
       ['MAC','107','maritime_area:type','submarine_operating_area'], // Submarine Operating Area
       ['MAC','108','maritime_area:type','submarine_transit_lane_(submerged)'], // Submarine Transit Lane (Submerged)
       ['MAC','109','maritime_area:type','submarine_transit_lane_(surface)'], // Submarine Transit Lane (Surface)
       ['MAC','110','maritime_area:type','surface_free_lane'], // Surface Free Lane
       ['MAC','111','maritime_area:type','surface_operating_area_(major)'], // Surface Operating Area (Major)
       ['MAC','113','maritime_area:type','anchoring_and_fishing_prohibited'], // Anchoring and fishing prohibited
       ['MAC','114','maritime_area:type','sea_test_range'], // Sea Test Range
       ['MAC','115','maritime_area:type','submarine_and_gunnery_exercise_area'], // Submarine and Gunnery Exercise Area
       ['MAC','116','maritime_area:type','named_operating_area'], // Named Operating Area
       ['MAC','123','maritime_area:type','caution_area'], // Caution Area
       ['MAC','140','maritime_area:type','exercise_area_limit'], // Exercise Area Limit
       ['MAC','141','maritime_area:type','unexploded_ordinance'], // Unexploded Ordinance
       ['MAC','142','maritime_area:type','submarine_warning_area'], // Submarine Warning Area
       ['MAC','143','maritime_area:type','naval_operations_area'], // Naval Operations Area
       ['MAC','144','maritime_area:type','inwater_tracking_range'], // Inwater Tracking Range
       ['MAC','145','maritime_area:type','foracs_v_limits'], // FORACS V Limits
       ['MAC','146','maritime_area:type','missile_test_area'], // Missile Test Area
       ['MAC','147','maritime_area:type','bombing_and_strafing_targets_area'], // Bombing and Strafing Targets Area
       ['MAC','148','maritime_area:type','drill_minefield'], // Drill Minefield
       ['MAC','149','maritime_area:type','abandoned_drill_minefield'], // Abandoned Drill Minefield
       ['MAC','150','maritime_area:type','acronym_area_-_purple'], // Acronym Area - Purple
       ['MAC','151','maritime_area:type','acronym_area_-_brown'], // Acronym Area - Brown
       ['MAC','152','maritime_area:type','acronym_area_-_blue'], // Acronym Area - Blue
       ['MAC','153','maritime_area:type','landing_craft_air_cushion_(lcac)'], // Landing Craft Air Cushion (LCAC)
       ['MAC','154','maritime_area:type','area_foxtrot'], // Area FOXTROT
       ['MAC','155','maritime_area:type','submarine_danger_area'], // Submarine Danger Area
       ['MAC','156','maritime_area:type','surface_ship_safety_lane'], // Surface Ship Safety Lane
       ['MAC','157','maritime_area:type','atlantic_fleet_weapons_range'], // Atlantic Fleet Weapons Range
       ['MAC','158','maritime_area:type','naval_defense_sea_area'], // Naval Defense Sea Area
       ['MAC','159','maritime_area:type','uqc/wqc_test_area'], // UQC/WQC Test Area
       ['MAC','997',undefined,undefined], // Unpopulated
       ['MAC','999','maritime_area:type','other'], // Other

       // MAS - Maintenance Status
       ['MAS','0','maintained','unknown'], // Unknown
       ['MAS','1','maintained','yes'], // Maintained
       ['MAS','2','maintained','no'], // Not Maintained
       ['MAS','997',undefined,undefined], // Unpopulated
       // ['MAS','998',undefined,undefined], // Not Applicable
       ['MAS','998','maintenance_status','not_applicable'], // Not Applicable

       // MBL - Maritime Boundary Limit
       ['MBL','0','raw:MBL','unknown'], // Unknown
       ['MBL','1','raw:MBL','colregs_demarcation_line'], // COLREGS Demarcation Line
       ['MBL','2','raw:MBL','customs_boundary'], // Customs Boundary
       ['MBL','3','raw:MBL','fishing_zone_boundary'], // Fishing Zone Boundary
       ['MBL','4','raw:MBL','harbor_limit'], // Harbor Limit
       ['MBL','6','raw:MBL','territorial_waters-limit_of_sovereignty'], // Territorial Waters-Limit of Sovereignty
       ['MBL','7','raw:MBL','territorial_waters_baseline'], // Territorial Waters Baseline
       ['MBL','9','raw:MBL','international_boundary_(at_sea)'], // International Boundary (at sea)
       ['MBL','10','raw:MBL','continental_shelf_boundary'], // Continental Shelf Boundary
       ['MBL','11','raw:MBL','limit_of_exclusive_economic_zone'], // Limit of Exclusive Economic Zone
       ['MBL','13','raw:MBL','clearing_line'], // Clearing Line
       ['MBL','16','raw:MBL','gulf_stream_limits'], // Gulf Stream Limits
       ['MBL','17','raw:MBL','three_nautical_mile_line'], // Three Nautical Mile Line
       ['MBL','18','raw:MBL','approximate_bathymetry'], // Approximate Bathymetry
       ['MBL','999','raw:MBL','other'], // Other

       // MCC - Material Composition Category
       ['MCC','0','material','unknown'], // Unknown
       ['MCC','2','material','aluminium'], // Aluminum
       ['MCC','4','material','ash'], // Ash
       ['MCC','5','material','asphalt'], // Asphalt
       ['MCC','7','material','bedrock'], // Bedrock
       ['MCC','8','material','boulders'], // Boulders
       ['MCC','9','material','brick'], // Brick
       ['MCC','11','material','cement'], // Cement
       ['MCC','12','material','chalk'], // Chalk
       ['MCC','14','material','cinders'], // Cinders
       ['MCC','15','material','cirripedia'], // Cirripedia
       ['MCC','16','material','clay'], // Clay
       ['MCC','18','material','cobble'], // Cobble
       ['MCC','20','material','composition'], // Composition
       ['MCC','21','material','concrete'], // Concrete
       ['MCC','22','material','conglomerate'], // Conglomerate
       ['MCC','23','material','copper'], // Copper
       ['MCC','24','material','coral'], // Coral
       ['MCC','25','material','coral_head'], // Coral Head
       ['MCC','28','material','diatoms'], // Diatoms
       ['MCC','30','material','earthen'], // Earthen
       ['MCC','36','material','foraminifera'], // Foraminifera
       ['MCC','37','material','fucus'], // Fucus
       ['MCC','40','material','glass'], // Glass
       ['MCC','41','material','globigerina'], // Globigerina
       ['MCC','43','material','granite'], // Granite
       ['MCC','45','material','grass/thatch'], // Grass/Thatch
       ['MCC','46','material','gravel'], // Gravel
       ['MCC','48','material','ground'], // Ground
       ['MCC','51','material','iron'], // Iron
       ['MCC','52','material','lava'], // Lava
       ['MCC','54','material','lead'], // Lead
       ['MCC','55','material','loess'], // Loess
       ['MCC','56','material','lumber'], // Lumber
       ['MCC','57','material','macadam'], // Macadam
       ['MCC','58','material','madrepores'], // Madrepores
       ['MCC','59','material','manganese'], // Manganese
       ['MCC','61','material','marl'], // Marl
       ['MCC','62','material','masonry_(brick/stone)'], // Masonry (Brick/Stone)
       ['MCC','63','material','mattes'], // Mattes
       ['MCC','64','material','metal'], // Metal
       ['MCC','65','material','mud'], // Mud
       ['MCC','66','material','mussels'], // Mussels
       ['MCC','69','material','ooze'], // Ooze
       ['MCC','70','material','oysters'], // Oysters
       ['MCC','72','material','part_metal'], // Part Metal
       ['MCC','73','material','pebbles'], // Pebbles
       ['MCC','74','material','plastic'], // Plastic
       ['MCC','75','material','polyzoa'], // Polyzoa
       ['MCC','77','material','prestressed_concrete'], // Prestressed Concrete
       ['MCC','78','material','pteropods'], // Pteropods
       ['MCC','79','material','pumice'], // Pumice
       ['MCC','80','material','quartz'], // Quartz
       ['MCC','81','material','radiolaria'], // Radiolaria
       ['MCC','83','material','reinforced_concrete'], // Reinforced Concrete
       ['MCC','84','material','rock'], // Rock/Rocky
       ['MCC','88','material','sand'], // Sand
       ['MCC','90','material','schist'], // Schist
       ['MCC','92','material','scoria'], // Scoria
       ['MCC','93','material','sea_tangle'], // Sea Tangle
       ['MCC','94','material','seaweed'], // Seaweed
       ['MCC','96','material','shells'], // Shells
       ['MCC','98','material','shingle'], // Shingle
       ['MCC','99','material','silt'], // Silt
       ['MCC','103','material','snow/ice'], // Snow/Ice
       ['MCC','104','material','soil'], // Soil
       ['MCC','105','material','spicules'], // Spicules
       ['MCC','106','material','sponge'], // Sponge
       ['MCC','107','material','steel'], // Steel
       ['MCC','108','material','stone'], // Stone
       ['MCC','110','material','travertine'], // Travertine
       ['MCC','111','material','tufa'], // Tufa
       ['MCC','113','material','vegetation_products'], // Vegetation Products
       ['MCC','115','material','volcanic_ash'], // Volcanic Ash
       ['MCC','117','material','wood'], // Wood
       ['MCC','120','material','glass_reinforced_plastic_(grp)'], // Glass Reinforced Plastic (GRP)
       ['MCC','997',undefined,undefined], // Unpopulated
       ['MCC','999','material','other'], // Other

       // MCS - Material Composition Secondary
       ['MCS','0','material:2','unknown'], // Unknown
       ['MCS','4','material:2','ash'], // Ash
       ['MCS','8','material:2','boulders'], // Boulders
       ['MCS','12','material:2','chalk'], // Chalk
       ['MCS','14','material:2','cinders'], // Cinders
       ['MCS','15','material:2','cirripedia'], // Cirripedia
       ['MCS','16','material:2','clay'], // Clay
       ['MCS','18','material:2','cobble'], // Cobble
       ['MCS','24','material:2','coral'], // Coral
       ['MCS','25','material:2','coral_head'], // Coral Head
       ['MCS','28','material:2','diatoms'], // Diatoms
       ['MCS','36','material:2','foraminifera'], // Foraminifera
       ['MCS','37','material:2','fucus'], // Fucus
       ['MCS','41','material:2','globigerina'], // Globigerina
       ['MCS','45','material:2','grass_/thatch'], // Grass /Thatch
       ['MCS','46','material:2','gravel'], // Gravel
       ['MCS','48','material:2','ground'], // Ground
       ['MCS','52','material:2','lava'], // Lava
       ['MCS','55','material:2','loess'], // Loess
       ['MCS','58','material:2','madrepores'], // Madrepores
       ['MCS','59','material:2','manganese'], // Manganese
       ['MCS','61','material:2','marl'], // Marl
       ['MCS','63','material:2','mattes'], // Mattes
       ['MCS','65','material:2','mud'], // Mud
       ['MCS','66','material:2','mussels'], // Mussels
       ['MCS','69','material:2','ooze'], // Ooze
       ['MCS','70','material:2','oysters'], // Oysters
       ['MCS','73','material:2','pebbles'], // Pebbles
       ['MCS','75','material:2','polyzoa'], // Polyzoa
       ['MCS','78','material:2','pteropods'], // Pteropods
       ['MCS','79','material:2','pumice'], // Pumice
       ['MCS','80','material:2','quartz'], // Quartz
       ['MCS','81','material:2','radiolaria'], // Radiolaria
       ['MCS','84','material:2','rock'], // Rock/Rocky
       ['MCS','88','material:2','sand'], // Sand
       ['MCS','90','material:2','schist'], // Schist
       ['MCS','92','material:2','scoria'], // Scoria
       ['MCS','93','material:2','sea_tangle'], // Sea Tangle
       ['MCS','94','material:2','seaweed'], // Seaweed
       ['MCS','96','material:2','shells'], // Shells
       ['MCS','98','material:2','shingle'], // Shingle
       ['MCS','99','material:2','silt'], // Silt
       ['MCS','104','material:2','soil'], // Soil
       ['MCS','105','material:2','spicules'], // Spicules
       ['MCS','106','material:2','sponge'], // Sponge
       ['MCS','108','material:2','stone'], // Stone
       ['MCS','111','material:2','tufa'], // Tufa
       ['MCS','115','material:2','volcanic_ash'], // Volcanic Ash
       ['MCS','997',undefined,undefined], // Unpopulated
       ['MCS','999','material:2','other'], // Other

       // MCU - Material Composition Underlying
       ['MCU','0','material:underlying','unknown'], // Unknown
       ['MCU','4','material:underlying','ash'], // Ash
       ['MCU','8','material:underlying','boulders'], // Boulders
       ['MCU','12','material:underlying','chalk'], // Chalk
       ['MCU','14','material:underlying','cinders'], // Cinders
       ['MCU','15','material:underlying','cirripedia'], // Cirripedia
       ['MCU','16','material:underlying','clay'], // Clay
       ['MCU','18','material:underlying','cobble'], // Cobble
       ['MCU','24','material:underlying','coral'], // Coral
       ['MCU','25','material:underlying','coral_head'], // Coral Head
       ['MCU','28','material:underlying','diatoms'], // Diatoms
       ['MCU','36','material:underlying','foraminifera'], // Foraminifera
       ['MCU','37','material:underlying','fucus'], // Fucus
       ['MCU','41','material:underlying','globigerina'], // Globigerina
       ['MCU','45','material:underlying','grass/thatch'], // Grass/Thatch
       ['MCU','46','material:underlying','gravel'], // Gravel
       ['MCU','48','material:underlying','ground'], // Ground
       ['MCU','52','material:underlying','lava'], // Lava
       ['MCU','58','material:underlying','madrepores'], // Madrepores
       ['MCU','59','material:underlying','manganese'], // Manganese
       ['MCU','61','material:underlying','marl'], // Marl
       ['MCU','63','material:underlying','mattes'], // Mattes
       ['MCU','65','material:underlying','mud'], // Mud
       ['MCU','66','material:underlying','mussels'], // Mussels
       ['MCU','69','material:underlying','ooze'], // Ooze
       ['MCU','70','material:underlying','oysters'], // Oysters
       ['MCU','73','material:underlying','pebbles'], // Pebbles
       ['MCU','75','material:underlying','polyzoa'], // Polyzoa
       ['MCU','78','material:underlying','pteropods'], // Pteropods
       ['MCU','79','material:underlying','pumice'], // Pumice
       ['MCU','80','material:underlying','quartz'], // Quartz
       ['MCU','81','material:underlying','radiolaria'], // Radiolaria
       ['MCU','84','material:underlying','rock'], // Rock/Rocky
       ['MCU','88','material:underlying','sand'], // Sand
       ['MCU','90','material:underlying','schist'], // Schist
       ['MCU','92','material:underlying','scoria'], // Scoria
       ['MCU','93','material:underlying','sea_tangle'], // Sea Tangle
       ['MCU','94','material:underlying','seaweed'], // Seaweed
       ['MCU','96','material:underlying','shells'], // Shells
       ['MCU','98','material:underlying','shingle'], // Shingle
       ['MCU','99','material:underlying','silt'], // Silt
       ['MCU','105','material:underlying','spicules'], // Spicules
       ['MCU','106','material:underlying','sponge'], // Sponge
       ['MCU','108','material:underlying','stone'], // Stone
       ['MCU','111','material:underlying','tufa'], // Tufa
       ['MCU','997',undefined,undefined], // Unpopulated
       ['MCU','999','material:underlying','other'], // Other

       // MED - Median Category
       ['MED','0',undefined,undefined], // Unknown
       ['MED','1','median','yes'], // With Median
       ['MED','2',undefined,undefined], // Without Median

       // MIA - Mine Actuation Independent Influence Acoustic Classification
       ['MIA','1','raw:MIA','low_freq.'], // Low freq.
       ['MIA','2','raw:MIA','audio_freq.'], // Audio freq.
       ['MIA','3','raw:MIA','high_freq.'], // High freq.
       ['MIA','4','raw:MIA','multiple_freq.'], // Multiple freq.

       // MIC - Mine Actuation Independent Contact Classification
       ['MIC','1','raw:MIC','plain'], // Plain
       ['MIC','2','raw:MIC','snagline'], // Snagline
       ['MIC','3','raw:MIC','antenna'], // Antenna

       // MID - Mine Identity Classification
       ['MID','0','raw:MID','unknown'], // Unknown
       ['MID','2','raw:MID','friend'], // Friend
       ['MID','3','raw:MID','hostile'], // Hostile
       ['MID','4','raw:MID','neutral'], // Neutral

       // MII - Mine Actuation Independent Influence Classification
       ['MII','1','raw:MII','pressure'], // Pressure
       ['MII','2','raw:MII','combined'], // Combined
       ['MII','3','raw:MII','classif._of_mine_actuation_independent_influence_magnetic_(mim)'], // Classif. of mine actuation independent influence magnetic (MIM)
       ['MII','4','raw:MII','classif._of_mine_actuation_independent_influence_acoustic_(mia)'], // Classif. of mine actuation independent influence acoustic (MIA)

       // MIM - Mine Actuation Independent Influence Magnetic Classification
       ['MIM','1','raw:MIM','sensitive'], // Sensitive
       ['MIM','2','raw:MIM','mid-sensitive'], // Mid-sensitive
       ['MIM','3','raw:MIM','course'], // Course

       // MIN_ - Mining Category
       ['MIN_','0','raw:MIN_','unknown'], // Unknown
       ['MIN_','2','raw:MIN_','horizontal_shaft'], // Horizontal Shaft
       ['MIN_','3','raw:MIN_','open_pit'], // Open Pit
       ['MIN_','4','raw:MIN_','placer'], // Placer
       ['MIN_','5','raw:MIN_','prospect'], // Prospect
       ['MIN_','6','raw:MIN_','strip'], // Strip
       ['MIN_','7','raw:MIN_','vertical_shaft'], // Vertical Shaft
       ['MIN_','8','raw:MIN_','peat_cuttings'], // Peat Cuttings
       ['MIN_','9','raw:MIN_','below_surface_mine'], // Below Surface Mine
       ['MIN_','997',undefined,undefined], // Unpopulated

       // MIO - Mine Actuation Independent Other Classification
       ['MIO','1','raw:MIO','electric_fields'], // Electric Fields
       ['MIO','2','raw:MIO','laser_sensors'], // Laser Sensors
       ['MIO','3','raw:MIO','seismic'], // Seismic
       ['MIO','4','raw:MIO','cosmic_ray'], // Cosmic ray
       ['MIO','5','raw:MIO','infra_red'], // Infra red
       ['MIO','6','raw:MIO','redistribution'], // Redistribution
       ['MIO','7','raw:MIO','velocity_field'], // Velocity field
       ['MIO','999','raw:MIO','other'], // Other

       // MMT - Mine Special Information Special Mine Types Classification
       ['MMT','1','raw:MMT','anti-sweeper'], // Anti-sweeper
       ['MMT','2','raw:MMT','anti-hunter'], // Anti-hunter
       ['MMT','3','raw:MMT','anti-hovercraft'], // Anti-hovercraft
       ['MMT','4','raw:MMT','drill'], // Drill
       ['MMT','5','raw:MMT','explosive_filled'], // Explosive filled
       ['MMT','6','raw:MMT','exercise_filled'], // Exercise filled
       ['MMT','7','raw:MMT','exercise'], // Exercise
       ['MMT','8','raw:MMT','practice'], // Practice
       ['MMT','9','raw:MMT','disposal_charge'], // Disposal Charge

       // MNA - Mine Actuation Classification
       ['MNA','1','raw:MNA','classif._of_mine_actuation_controlled_(mnc)'], // Classif. of mine actuation controlled (MNC)
       ['MNA','2','raw:MNA','classif._of_mine_actuation_independent_(mni)'], // Classif. of mine actuation independent (MNI)
       ['MNA','3','raw:MNA','mine_actuation_no_information'], // Mine actuation no information

       // MNC - Mine Actuation Controlled Classification
       ['MNC','1','raw:MNC','mine_actuation_controlled_cable'], // Mine actuation controlled cable
       ['MNC','2','raw:MNC','classif._of_mine_actuation_controlled_cableless_(mnl)'], // Classif. of mine actuation controlled cableless (MNL)

       // MNI - Mine Actuation Independent Classification
       ['MNI','1','raw:MNI','classif._of_mine_actuation_independent_contact_(mic)'], // Classif. of mine actuation independent contact (MIC)
       ['MNI','2','raw:MNI','classif._of_mine_actuation_independent_influence_(mii)'], // Classif. of mine actuation independent influence (MII)
       ['MNI','3','raw:MNI','classif._of_mine_actuation_independent_other_(mio)'], // Classif. of mine actuation independent other (MIO)

       // MNL - Mine Actuation Controlled Cableless Classification
       ['MNL','1','raw:MNL','frequency_communications_link'], // Frequency Communications Link
       ['MNL','2','raw:MNL','explicit_communications_link'], // Explicit Communications link
       ['MNL','3','raw:MNL','alternating_current_communications_link'], // Alternating Current Communications Link

       // MPC - Mine Position Classification
       ['MPC','1','raw:MPC','classif._of_mine_position_ground_(mpg)'], // Classif. of mine position ground (MPG)
       ['MPC','2','raw:MPC','classif._of_mine_position_moored_(mpm)'], // Classif. of mine position moored (MPM)
       ['MPC','3','raw:MPC','classif._of_mine_position_other_(mpo)'], // Classif. of mine position other (MPO)
       ['MPC','4','raw:MPC','mine_position_no_information'], // Mine position no information

       // MPG - Mine Position Ground Classification
       ['MPG','1','raw:MPG','<=_500_kg/charge'], // <= 500 kg/charge
       ['MPG','2','raw:MPG','>_500_kg'], // > 500 kg

       // MPM - Mine Position Moored Classification
       ['MPM','1','raw:MPM','deep_moored'], // Deep Moored
       ['MPM','2','raw:MPM','short_tethered'], // Short tethered

       // MPO - Mine Position Other Classification
       ['MPO','1','raw:MPO','drifting'], // Drifting
       ['MPO','2','raw:MPO','oscillating'], // Oscillating
       ['MPO','3','raw:MPO','creeping'], // Creeping
       ['MPO','4','raw:MPO','mobile'], // Mobile
       ['MPO','5','raw:MPO','homing'], // Homing
       ['MPO','6','raw:MPO','rising'], // Rising
       ['MPO','7','raw:MPO','bouquet'], // Bouquet
       ['MPO','8','raw:MPO','active'], // Active

       // MSC - Mine Status Classification
       ['MSC','1','raw:MSC','afloat'], // Afloat
       ['MSC','2','raw:MSC','sunk'], // Sunk
       ['MSC','3','raw:MSC','disposed'], // Disposed
       ['MSC','4','raw:MSC','fouled'], // Fouled
       ['MSC','5','raw:MSC','exploded'], // Exploded
       ['MSC','6','raw:MSC','countermined'], // Countermined
       ['MSC','7','raw:MSC','neutralized'], // Neutralized
       ['MSC','8','raw:MSC','rendered_safe'], // Rendered safe
       ['MSC','9','raw:MSC','recovered'], // Recovered
       ['MSC','10','raw:MSC','removed'], // Removed

       // MSD - Mine Special Information Special Devices Classification
       ['MSD','1','raw:MSD','arming_delay'], // Arming delay
       ['MSD','2','raw:MSD','ship_count'], // Ship count
       ['MSD','3','raw:MSD','intermittent_arming'], // Intermittent arming
       ['MSD','4','raw:MSD','delayed_rising'], // Delayed rising
       ['MSD','5','raw:MSD','obstructers'], // Obstructers
       ['MSD','6','raw:MSD','sterilizers'], // Sterilizers
       ['MSD','7','raw:MSD','flooders'], // Flooders
       ['MSD','8','raw:MSD','anti-watching'], // Anti-watching
       ['MSD','9','raw:MSD','classif._-mine_special_info_special_devices_anti-sweep_wire_(msw)'], // Classif. -mine special info special devices anti-sweep wire (MSW)
       ['MSD','10','raw:MSD','classif._-mine_special_info_special_devices_anti-recovery_(msr)'], // Classif. -mine special info special devices anti-recovery (MSR)
       ['MSD','11','raw:MSD','classif._-mine_special_info_special_devices_anti-hunting_(msh)'], // Classif. -mine special info special devices anti-hunting (MSH)

       // MSH - Mine Special Information Special Devices Anti-Hunting Classification
       ['MSH','1','raw:MSH','anechoic_coating'], // Anechoic coating
       ['MSH','2','raw:MSH','automatic_mine_burial'], // Automatic mine burial
       ['MSH','3','raw:MSH','irregular_shaping'], // Irregular shaping
       ['MSH','4','raw:MSH','acoustic_impedance'], // Acoustic impedance
       ['MSH','5','raw:MSH','acoustic_transparency'], // Acoustic transparency
       ['MSH','6','raw:MSH','non-metallic_case'], // Non-metallic case
       ['MSH','7','raw:MSH','sonar_decoys'], // Sonar decoys
       ['MSH','999','raw:MSH','other'], // Other

       // MSI - Mine Special Information Classification
       ['MSI','1','raw:MSI','classif._of_mine_special_info_usefulness_(msu)'], // Classif. of mine special info usefulness (MSU)
       ['MSI','2','raw:MSI','classif._of_mine_special_info_special_mine_types_(mmt)'], // Classif. of mine special info special mine types (MMT)
       ['MSI','3','raw:MSI','classif._of_mine_special_info_special_devices_(msd)'], // Classif. of mine special info special devices (MSD)

       // MSR - Mine Special Information Special Devices Anti-Recovery Classification
       ['MSR','1','raw:MSR','switch'], // Switch
       ['MSR','2','raw:MSR','mooring_level_switch'], // Mooring level switch
       ['MSR','3','raw:MSR','stripping_equipment'], // Stripping equipment
       ['MSR','999','raw:MSR','other'], // Other

       // MST - Missile Site Type
       ['MST','0','raw:MST','unknown'], // Unknown
       ['MST','1','raw:MST','abm'], // ABM
       ['MST','2','raw:MST','icbm'], // ICBM
       ['MST','3','raw:MST','irbm'], // IRBM
       ['MST','4','raw:MST','sa1'], // SA1
       ['MST','5','raw:MST','sa2'], // SA2
       ['MST','6','raw:MST','sa3'], // SA3
       ['MST','7','raw:MST','sa4'], // SA4
       ['MST','8','raw:MST','sa5'], // SA5
       ['MST','9','raw:MST','sa6'], // SA6
       ['MST','10','raw:MST','sa7'], // SA7
       ['MST','11','raw:MST','sa8'], // SA8
       ['MST','12','raw:MST','sa9'], // SA9
       ['MST','13','raw:MST','mrbm'], // MRBM
       ['MST','14','raw:MST','ssm'], // SSM
       ['MST','15','raw:MST','sam'], // SAM
       ['MST','17','raw:MST','sa10'], // Sa10
       ['MST','999','raw:MST','other'], // Other

       // MSU - Mine Special Information Usefulness Classification
       ['MSU','1','raw:MSU','general_purpose_ground'], // General purpose ground
       ['MSU','2','raw:MSU','deep_water'], // Deep water
       ['MSU','3','raw:MSU','medium_depth_anti-submarine'], // Medium depth anti-submarine
       ['MSU','4','raw:MSU','continental_shelf'], // Continental shelf
       ['MSU','5','raw:MSU','maritime_anti-invasion'], // Maritime anti-invasion
       ['MSU','6','raw:MSU','anti-surface_effect_vehicle'], // Anti-surface effect vehicle

       // MSW - Mine Special Information Special Devices Anti-Sweep Wire Classification
       ['MSW','1','raw:MSW','chain_moorings'], // Chain moorings
       ['MSW','2','raw:MSW','sprocket'], // Sprocket
       ['MSW','3','raw:MSW','grapnel'], // Grapnel
       ['MSW','4','raw:MSW','cutters'], // Cutters
       ['MSW','5','raw:MSW','sensitive_tubing'], // Sensitive Tubing
       ['MSW','999','raw:MSW','other'], // Other

       // M_ANT_9D066 - Hierarchy 9D066 Annotation Type
       ['M_ANT_9D066','0','annotation:type','unknown'], // Unknown
       ['M_ANT_9D066','1','annotation:type','target_reference_point'], // Target Reference Point
       ['M_ANT_9D066','2','annotation:type','special_area'], // Special Area
       ['M_ANT_9D066','3','annotation:type','radar_significant_power_line_(rspl)'], // Radar Significant Power Line (RSPL)
       ['M_ANT_9D066','4','annotation:type','precise_radar_significant_location_(prsl)'], // Precise Radar Significant Location (PRSL)
       ['M_ANT_9D066','5','annotation:type','point_target_not_defined_under_existing_facs_code'], // Point Target not defined under existing FACS code
       ['M_ANT_9D066','8','annotation:type','unconfirmed_target'], // Unconfirmed Target
       ['M_ANT_9D066','9','annotation:type','na'], // NA

       // M_AS1_1U030 - (unnamed attribute)
       ['M_AS1_1U030','0','raw:M_AS1_1U030','unknown'], // Unknown
       ['M_AS1_1U030','1','raw:M_AS1_1U030','none'], // None
       ['M_AS1_1U030','2','raw:M_AS1_1U030','highway_strip'], // Highway Strip
       ['M_AS1_1U030','3','raw:M_AS1_1U030','ice_strip'], // Ice Strip
       ['M_AS1_1U030','4','raw:M_AS1_1U030','snow_strip'], // Snow Strip
       ['M_AS1_1U030','5','raw:M_AS1_1U030','decoy'], // Decoy
       ['M_AS1_1U030','6','raw:M_AS1_1U030','na'], // NA

       // M_ATN_1Q100 - Hierarchy 1Q100 ATN
       ['M_ATN_1Q100','0','raw:M_ATN_1Q100','unknown'], // Unknown
       ['M_ATN_1Q100','1','raw:M_ATN_1Q100','marked'], // Marked
       ['M_ATN_1Q100','2','raw:M_ATN_1Q100','unmarked'], // Unmarked
       ['M_ATN_1Q100','997',undefined,undefined], // Unpopulated
       // ['M_ATN_1Q100','998',undefined,undefined], // Not Applicable
       ['M_ATN_1Q100','998','raw:M_ATN_1Q100','not_applicable'], // Not Applicable
       ['M_ATN_1Q100','999','raw:M_ATN_1Q100','other'], // Other

       // M_ATN_6C165 - Hierarchy 6C165 ATN
       ['M_ATN_6C165','0','raw:M_ATN_6C165','unknown'], // Unknown
       ['M_ATN_6C165','1','raw:M_ATN_6C165','marked'], // Marked
       ['M_ATN_6C165','2','raw:M_ATN_6C165','unmarked'], // Unmarked
       ['M_ATN_6C165','997',undefined,undefined], // Unpopulated
       // ['M_ATN_6C165','998',undefined,undefined], // Not Applicable
       ['M_ATN_6C165','998','raw:M_ATN_6C165','not_applicableot_a'], // Not Applicable
       ['M_ATN_6C165','999','raw:M_ATN_6C165','other'], // Other

       // M_AVB_1P031 - Hierarchy 1P031 AVB
       ['M_AVB_1P031','0','raw:M_AVB_1P031','unknown'], // Unknown
       ['M_AVB_1P031','5','raw:M_AVB_1P031','access_road_to_gravel_highway'], // Access Road to Gravel Highway
       ['M_AVB_1P031','6','raw:M_AVB_1P031','access_road_to_secondary_highway'], // Access Road to Secondary Highway
       ['M_AVB_1P031','7','raw:M_AVB_1P031','access_road_to_primary_highway'], // Access Road to Primary Highway
       ['M_AVB_1P031','9','raw:M_AVB_1P031','access_road_to_all_weather_highway'], // Access Road to All Weather Highway
       ['M_AVB_1P031','24','raw:M_AVB_1P031','na'], // NA

       // M_EXS_6C165 - Hierarchy 6C165 EXS
       ['M_EXS_6C165','0','raw:M_EXS_6C165','unknown'], // Unknown
       ['M_EXS_6C165','22','raw:M_EXS_6C165','one-way'], // One-Way
       ['M_EXS_6C165','23','raw:M_EXS_6C165','two-way'], // Two-Way
       ['M_EXS_6C165','997',undefined,undefined], // Unpopulated
       // ['M_EXS_6C165','998',undefined,undefined], // Not Applicable
       ['M_EXS_6C165','998','raw:M_EXS_6C165','not_applicable'], // Not Applicable
       ['M_EXS_6C165','999','raw:M_EXS_6C165','other'], // Other

       // M_HAF_9D066 - Hierarchy 9D066 Horizontal Accuracy Flag
       ['M_HAF_9D066','0','raw:M_HAF_9D066','unknown'], // Unknown
       ['M_HAF_9D066','1','raw:M_HAF_9D066','<=200_feet'], // <=200 feet
       ['M_HAF_9D066','2','raw:M_HAF_9D066','>200_feet'], // >200 feet

       // M_HDI_6C165 - Hierarchy 6C165 HDI
       ['M_HDI_6C165','0','raw:M_HDI_6C165','unknown'], // Unknown
       ['M_HDI_6C165','9','raw:M_HDI_6C165','depth_known_by_other_than_wire_drag'], // Depth Known by Other Than Wire Drag
       ['M_HDI_6C165','10','raw:M_HDI_6C165','depth_known_by_wire_drag'], // Depth Known by Wire Drag
       ['M_HDI_6C165','11','raw:M_HDI_6C165','depth_unknown_but_safe_to_depth_shown'], // Depth Unknown But Safe to Depth Shown
       ['M_HDI_6C165','12','raw:M_HDI_6C165','depth_unknown'], // Depth Unknown
       ['M_HDI_6C165','13','raw:M_HDI_6C165','uncovering_height_known'], // Uncovering Height Known
       ['M_HDI_6C165','14','raw:M_HDI_6C165','uncovering_height_unknown'], // Uncovering Height Unknown
       // ['M_HDI_6C165','15',undefined,undefined], // Not Applicable
       ['M_HDI_6C165','15','raw:M_HDI_6C165','not_applicable'], // Not Applicable
       ['M_HDI_6C165','997',undefined,undefined], // Unpopulated
       ['M_HDI_6C165','999','raw:M_HDI_6C165','other'], // Other

       // M_IAS_6C160 - Hierarchy 6C160 IAS
       ['M_IAS_6C160','0','raw:M_IAS_6C160','unknown'], // Unknown
       ['M_IAS_6C160','1','raw:M_IAS_6C160','adopted'], // Adopted
       ['M_IAS_6C160','2','raw:M_IAS_6C160','not_adopted'], // Not Adopted
       ['M_IAS_6C160','997',undefined,undefined], // Unpopulated
       // ['M_IAS_6C160','998',undefined,undefined], // Not Applicable
       ['M_IAS_6C160','998','raw:M_IAS_6C160','not_applicable'], // Not Applicable
       ['M_IAS_6C160','999','raw:M_IAS_6C160','other'], // Other

       // M_IAS_6C180 - Hierarchy 6C160 IAS
       ['M_IAS_6C180','0','raw:M_IAS_6C180','unknown'], // Unknown
       ['M_IAS_6C180','1','raw:M_IAS_6C180','adopted'], // Adopted
       ['M_IAS_6C180','2','raw:M_IAS_6C180','not_adopted'], // Not Adopted
       ['M_IAS_6C180','997',undefined,undefined], // Unpopulated
       // ['M_IAS_6C180','998',undefined,undefined], // Not Applicable
       ['M_IAS_6C180','998','raw:M_IAS_6C180','not_applicable'], // Not Applicable
       ['M_IAS_6C180','999','raw:M_IAS_6C180','other'], // Other

       // M_RIT_1P020 - Hierarchy 1P020 RIT
       ['M_RIT_1P020','0','raw:M_RIT_1P020','unknown'], // Unknown
       ['M_RIT_1P020','1','raw:M_RIT_1P020','cloverleaf'], // Cloverleaf
       ['M_RIT_1P020','2','raw:M_RIT_1P020','diamond'], // Diamond
       ['M_RIT_1P020','997',undefined,undefined], // Unpopulated
       // ['M_RIT_1P020','998',undefined,undefined], // Not Applicable
       ['M_RIT_1P020','998','raw:M_RIT_1P020','not_applicable'], // Not Applicable
       ['M_RIT_1P020','999','raw:M_RIT_1P020','other'], // Other

       // M_RPA_2H020 - Hierarchy 2H020 RPA
       ['M_RPA_2H020','0','raw:M_RPA_2H020','unknown'], // Unknown
       ['M_RPA_2H020','1','raw:M_RPA_2H020','access_required'], // Access Required
       ['M_RPA_2H020','2','raw:M_RPA_2H020','access_not_required'], // Access Not Required
       ['M_RPA_2H020','997',undefined,undefined], // Unpopulated
       // ['M_RPA_2H020','998',undefined,undefined], // Not Applicable
       ['M_RPA_2H020','998','raw:M_RPA_2H020','not_applicable'], // Not Applicable
       ['M_RPA_2H020','999','raw:M_RPA_2H020','other'], // Other

       // M_RPA_2H141 - Hierarchy 2H141 RPA
       ['M_RPA_2H141','0','raw:M_RPA_2H141','unknown'], // Unknown
       ['M_RPA_2H141','1','raw:M_RPA_2H141','access_required'], // Access Required
       ['M_RPA_2H141','2','raw:M_RPA_2H141','access_not_required'], // Access Not Required
       ['M_RPA_2H141','997',undefined,undefined], // Unpopulated
       // ['M_RPA_2H141','998',undefined,undefined], // Not Applicable
       ['M_RPA_2H141','998','raw:M_RPA_2H141','not_applicable'], // Not Applicable
       ['M_RPA_2H141','999','raw:M_RPA_2H141','other'], // Other

       // M_RTT_6C165 - Hierarchy 6C165 RTT
       ['M_RTT_6C165','0','raw:M_RTT_6C165','unknown'], // Unknown
       ['M_RTT_6C165','2','raw:M_RTT_6C165','recommended_track_for_other_than_deep_draft_vessels'], // Recommended Track for Other Than Deep Draft Vessels
       ['M_RTT_6C165','3','raw:M_RTT_6C165','recommended_track_for_deep_draft_vessels'], // Recommended Track for Deep Draft Vessels
       ['M_RTT_6C165','4','raw:M_RTT_6C165','deep_water_route'], // Deep Water Route
       ['M_RTT_6C165','11','raw:M_RTT_6C165','two-way_route'], // Two-way Route
       ['M_RTT_6C165','13','raw:M_RTT_6C165','recommended_direction_of_traffic_flow'], // Recommended Direction of Traffic Flow
       ['M_RTT_6C165','18','raw:M_RTT_6C165','recommended_route'], // Recommended Route
       ['M_RTT_6C165','96','raw:M_RTT_6C165','recommended_traffic_lane_part'], // Recommended Traffic Lane Part
       ['M_RTT_6C165','997',undefined,undefined], // Unpopulated
       // ['M_RTT_6C165','998',undefined,undefined], // Not Applicable
       ['M_RTT_6C165','998','raw:M_RTT_6C165','not_applicable'], // Not Applicable
       ['M_RTT_6C165','999','raw:M_RTT_6C165','other'], // Other

       // M_SEP_1P031 - Hierarchy 1P031 SEP
       ['M_SEP_1P031','0',undefined,undefined], // Unknown
       ['M_SEP_1P031','1',undefined,undefined], // Not Separated
       ['M_SEP_1P031','2','is_divided','yes'], // Separated

       // M_TSP_6C160 - Hierarchy 6C160 TSP
       ['M_TSP_6C160','0','raw:M_TSP_6C160','unknown'], // Unknown
       ['M_TSP_6C160','1','raw:M_TSP_6C160','arrow'], // Arrow
       ['M_TSP_6C160','2','raw:M_TSP_6C160','outer_boundary'], // Outer Boundary
       ['M_TSP_6C160','3','raw:M_TSP_6C160','separation_zone_area'], // Separation Zone Area
       ['M_TSP_6C160','4','raw:M_TSP_6C160','separation_zone_line'], // Separation Zone Line
       ['M_TSP_6C160','5','raw:M_TSP_6C160','separation_zone_point'], // Separation Zone Point
       ['M_TSP_6C160','6','raw:M_TSP_6C160','inbound_area'], // Inbound Area
       ['M_TSP_6C160','7','raw:M_TSP_6C160','outbound_area'], // Outbound Area
       ['M_TSP_6C160','997',undefined,undefined], // Unpopulated
       // ['M_TSP_6C160','998',undefined,undefined], // Not Applicable
       ['M_TSP_6C160','998','raw:M_TSP_6C160','not_applicable'], // Not Applicable
       ['M_TSP_6C160','999','raw:M_TSP_6C160','other'], // Other

       // M_TSP_6C180 - Hierarchy 6C180 TSP
       ['M_TSP_6C180','0','raw:M_TSP_6C180','unknown'], // Unknown
       ['M_TSP_6C180','1','raw:M_TSP_6C180','arrow'], // Arrow
       ['M_TSP_6C180','2','raw:M_TSP_6C180','outer_boundary'], // Outer Boundary
       ['M_TSP_6C180','3','raw:M_TSP_6C180','separation_zone_area'], // Separation Zone Area
       ['M_TSP_6C180','4','raw:M_TSP_6C180','separation_zone_line'], // Separation Zone Line
       ['M_TSP_6C180','5','raw:M_TSP_6C180','separation_zone_point'], // Separation Zone Point
       ['M_TSP_6C180','6','raw:M_TSP_6C180','inbound_area'], // Inbound Area
       ['M_TSP_6C180','7','raw:M_TSP_6C180','outbound_area'], // Outbound Area
       ['M_TSP_6C180','997',undefined,undefined], // Unpopulated
       // ['M_TSP_6C180','998',undefined,undefined], // Not Applicable
       ['M_TSP_6C180','998','raw:M_TSP_6C180','not_applicable'], // Not Applicable
       ['M_TSP_6C180','999','raw:M_TSP_6C180','other'], // Other

       // M_TUC_1P020 - Hierarchy 1P020 TUC
       ['M_TUC_1P020','0',undefined,undefined], // Unknown
       ['M_TUC_1P020','4','transport:type','road'], // Road
       ['M_TUC_1P020','7','highway:through_route','yes'], // Through Route
       ['M_TUC_1P020','997',undefined,undefined], // Unpopulated
       // ['M_TUC_1P020','998',undefined,undefined], // Not Applicable
       ['M_TUC_1P020','998','IP023_TUC','not_applicable'], // Not Applicable
       ['M_TUC_1P020','999','transport:type','other'], // Other

       // M_UNI_1Q100 - Hierarchy 1Q100 UNI
       ['M_UNI_1Q100','0','raw:M_UNI_1Q100','unknown'], // Unknown
       ['M_UNI_1Q100','11','raw:M_UNI_1Q100','nautical_miles'], // Nautical Miles
       ['M_UNI_1Q100','23','raw:M_UNI_1Q100','kilometers'], // Kilometers
       ['M_UNI_1Q100','28','raw:M_UNI_1Q100','fathoms'], // Fathoms
       ['M_UNI_1Q100','997',undefined,undefined], // Unpopulated
       // ['M_UNI_1Q100','998',undefined,undefined], // Not Applicable
       ['M_UNI_1Q100','998','raw:M_UNI_1Q100','not_applicable'], // Not Applicable
       ['M_UNI_1Q100','999','raw:M_UNI_1Q100','other'], // Other

       // M_USE_1P030 - Hierarchy 1P030 USE
       ['M_USE_1P030','0',undefined,undefined], // Unknown
       ['M_USE_1P030','4','ref:road:class','national'], // National
       ['M_USE_1P030','5','ref:road:class','secondary'], // State
       ['M_USE_1P030','23','ref:road:class','international'], // International
       ['M_USE_1P030','990',undefined,undefined], // Unpopulated
       // ['M_USE_1P030','991',undefined,undefined], // Not Applicable
       ['M_USE_1P030','991','IP023_USE','not_applicable'], // Not Applicable
       ['M_USE_1P030','999','ref:road:class','other'], // Other

       // M_VDC_2H020 - Hierarchy 2H020 VDC
       ['M_VDC_2H020','0','source:datum:sounding','unknown'], // Unknown
       ['M_VDC_2H020','4','source:datum:sounding','indian_spring_low_water'], // Indian Spring Low Water
       ['M_VDC_2H020','11','source:datum:sounding','mean_low_water'], // Mean Low Water
       ['M_VDC_2H020','13','source:datum:sounding','mean_low_water_springs'], // Mean Low Water Springs
       ['M_VDC_2H020','14','source:datum:sounding','mean_lower_low_water'], // Mean Lower Low Water
       ['M_VDC_2H020','15','source:datum:sounding','mean_sea_level'], // Mean Sea Level
       ['M_VDC_2H020','19','source:datum:sounding','mean_lower_low_water_springs'], // Mean Lower Low Water Springs
       ['M_VDC_2H020','27','source:datum:sounding','lowest_normal_low_water'], // Lowest Normal Low Water
       ['M_VDC_2H020','90','source:datum:sounding','lowest_low_water'], // Lowest Low Water
       ['M_VDC_2H020','997',undefined,undefined], // Unpopulated
       // ['M_VDC_2H020','998',undefined,undefined], // Not Applicable
       ['M_VDC_2H020','998','source:datum:sounding','not_applicable'], // Not Applicable
       ['M_VDC_2H020','999','source:datum:sounding','other'], // Other

       // M_VDC_6C165 - Hierarchy 6C165 VDC
       ['M_VDC_6C165','0','source:datum:sounding','unknown'], // Unknown
       ['M_VDC_6C165','4','source:datum:sounding','indian_spring_low_water'], // Indian Spring Low Water
       ['M_VDC_6C165','11','source:datum:sounding','mean_low_water'], // Mean Low Water
       ['M_VDC_6C165','13','source:datum:sounding','mean_low_water_springs'], // Mean Low Water Springs
       ['M_VDC_6C165','14','source:datum:sounding','mean_lower_low_water'], // Mean Lower Low Water
       ['M_VDC_6C165','15','source:datum:sounding','mean_sea_level'], // Mean Sea Level
       ['M_VDC_6C165','19','source:datum:sounding','mean_lower_low_water_springs'], // Mean Lower Low Water Springs
       ['M_VDC_6C165','27','source:datum:sounding','lowest_normal_low_water'], // Lowest Normal Low Water
       ['M_VDC_6C165','90','source:datum:sounding','lowest_low_water'], // Lowest Low Water
       ['M_VDC_6C165','997',undefined,undefined], // Unpopulated
       // ['M_VDC_6C165','998',undefined,undefined], // Not Applicable
       ['M_VDC_6C165','998','source:datum:sounding','not_applicable'], // Not Applicable
       ['M_VDC_6C165','999','source:datum:sounding','other'], // Other

       // NAS - Native Settlement Type
       ['NAS','0','raw:NAS','unknown'], // Unknown
       ['NAS','1','raw:NAS','centralized_habitation'], // Centralized Habitation
       ['NAS','2','raw:NAS','continuous_habitation'], // Continuous Habitation
       ['NAS','997',undefined,undefined], // Unpopulated

       // NS2 - Navigation System Types (2)
       ['NS2','0','seamark:radio_station:category:2','unknown'], // Unknown
       ['NS2','1','seamark:radio_station:category:2','circular_radio_beacon'], // Circular Radio Beacon
       ['NS2','2','seamark:radio_station:category:2','consol'], // CONSOL
       ['NS2','4','seamark:radio_station:category:2','radio_direction_finding'], // Radio Direction Finding
       ['NS2','5','seamark:radio_station:category:2','directional_radio_beacon'], // Directional Radio Beacon
       ['NS2','10','seamark:radio_station:category:2','radar_responder_beacon_(racon)'], // Radar Responder Beacon (RACON)
       ['NS2','17','seamark:radio_station:category:2','non-directional_radio_beacon_(ndb)'], // Non-Directional Radio Beacon (NDB)
       ['NS2','40','seamark:radio_station:category:2','radio_beacon'], // Radio Beacon
       ['NS2','41','seamark:radio_station:category:2','rotating_loop_radio_beacon'], // Rotating Loop Radio Beacon
       ['NS2','45','seamark:radio_station:category:2','radar_station'], // Radar Station
       ['NS2','51','seamark:radio_station:category:2','radiobeacon,_type_unknown'], // Radiobeacon, Type Unknown
       ['NS2','52','seamark:radio_station:category:2','none'], // None
       ['NS2','53','seamark:radio_station:category:2','qtg_station_(r)'], // QTG Station (R)
       ['NS2','54','seamark:radio_station:category:2','ramark_(ramark)'], // Ramark (Ramark)
       ['NS2','997',undefined,undefined], // Unpopulated

       // NST - Navigation System Types
       ['NST','0','seamark:radio_station:category','unknown'], // Unknown
       ['NST','1','seamark:radio_station:category','circular_radio_beacon'], // Circular Radio Beacon
       ['NST','2','seamark:radio_station:category','consol'], // CONSOL
       ['NST','3','seamark:radio_station:category','decca'], // DECCA
       ['NST','4','seamark:radio_station:category','radio_direction_finding'], // Radio Direction Finding
       ['NST','5','seamark:radio_station:category','directional_radio_beacon'], // Directional Radio Beacon
       ['NST','7','seamark:radio_station:category','loran'], // Long Range Air Navigation System (LORAN)
       ['NST','8','seamark:radio_station:category','omega'], // OMEGA
       ['NST','10','seamark:radio_station:category','radar_responder_beacon_(racon)'], // Radar Responder Beacon (RACON)
       ['NST','11','seamark:radio_station:category','radar'], // Radar
       ['NST','12','seamark:radio_station:category','radio'], // Radio
       ['NST','13','seamark:radio_station:category','radio_telephone'], // Radio Telephone
       ['NST','15','seamark:radio_station:category','television'], // TV
       ['NST','16','seamark:radio_station:category','microwave'], // Microwave
       ['NST','17','seamark:radio_station:category','ndb'], // Non-Directional Radio Beacon (NDB)
       ['NST','18','seamark:radio_station:category','non-directional_radio_beacon/distance_measuring_equipment'], // NDB/Distance Measuring Equipment (NDB/DME)
       ['NST','19','seamark:radio_station:category','radio_range'], // Radio Range (RNG)
       ['NST','20','seamark:radio_station:category','vor'], // VHF Omni Directional Radio Range (VOR)
       ['NST','21','seamark:radio_station:category','vor-dme'], // VHF Omni Directional Radio Range/Distance Measuring Equipment (VOR/DME)
       ['NST','22','seamark:radio_station:category','vortac'], // VHF Omni Directional Radio Range and TACAN (VORTAC)
       ['NST','23','seamark:radio_station:category','tacan'], // Tactical Air Navigation Equipment (TACAN)
       ['NST','24','seamark:radio_station:category','ils'], // Instrument Landing System (ILS)
       ['NST','25','seamark:radio_station:category','ils-dme'], // Instrument Landing System/Distance Measuring Equipment (ILS/DME)
       ['NST','26','seamark:radio_station:category','localizer'], // Localizer (LOC)
       ['NST','27','seamark:radio_station:category','localizer/distance_measuring_equipment'], // Localizer/Distance Measuring Equipment (LOC/DME)
       ['NST','30','seamark:radio_station:category','microwave_landing_system'], // Microwave Landing System (MLS)
       ['NST','31','seamark:radio_station:category','fan_marker'], // Fan Marker
       ['NST','32','seamark:radio_station:category','bone_marker'], // Bone Marker
       ['NST','33','seamark:radio_station:category','radio_telegraph'], // Radio Telegraph
       ['NST','35','seamark:radio_station:category','radar_antenna'], // Radar Antenna
       ['NST','37','seamark:radio_station:category','precision_approach_radar'], // Precision Approach Radar (PAR)
       ['NST','38','seamark:radio_station:category','aeronautical_radio'], // Aeronautical Radio
       ['NST','40','seamark:radio_station:category','radio_beacon'], // Radio Beacon
       ['NST','41','seamark:radio_station:category','rotating_loop_radio_beacon'], // Rotating Loop Radio Beacon
       ['NST','42','seamark:radio_station:category','visual_flight_rules_test_signal_maker'], // Visual Flight Rules (VFR) Test Signal Maker
       ['NST','45','seamark:radio_station:category','radar_station'], // Radar Station
       ['NST','46','seamark:radio_station:category','aeronautical_radio_range'], // Aeronautical Radio Range
       ['NST','51','seamark:radio_station:category','radiobeacon,_type_unknown'], // Radiobeacon, Type Unknown
       ['NST','52','seamark:radio_station:category','none'], // None
       ['NST','53','seamark:radio_station:category','qtg_station_(r)'], // QTG Station (R)
       ['NST','54','seamark:radio_station:category','ramark_(ramark)'], // Ramark (Ramark)
       ['NST','55','seamark:radio_station:category','radar_reflector'], // Radar reflector
       ['NST','58','seamark:radio_station:category','dme'], // DME (Distance Measuring Equipment)
       ['NST','74','seamark:radio_station:category','ils_back_course'], // Ils Back Course
       ['NST','75','seamark:radio_station:category','loc_back_course'], // Loc Back Course
       ['NST','76','seamark:radio_station:category','marker'], // Marker
       ['NST','77','seamark:radio_station:category','fm_-_fan_marker'], // Fm - Fan Marker
       ['NST','78','seamark:radio_station:category','outer_marker'], // Om - Outer Marker
       ['NST','79','seamark:radio_station:category','middle_marker'], // Mm - Middle Marker
       ['NST','80','seamark:radio_station:category','locator_outer_marker'], // Lom - Locator Outer Marker
       ['NST','81','seamark:radio_station:category','locator_middle_marker'], // Lmm - Locator Middle Marker
       ['NST','82','seamark:radio_station:category','rotating_beacon_light'], // Rotating Beacon Light
       ['NST','997',undefined,undefined], // Unpopulated
       // ['NST','998',undefined,undefined], // Not Applicable
       ['NST','998','seamark:radio_station:category','not_applicable'], // Not Applicable
       ['NST','999','seamark:radio_station:category','other'], // Other

       // OAC - Ordnance Area Construction Activity
       ['OAC','0','raw:OAC','unknown'], // Unknown
       ['OAC','1','raw:OAC','1_-_area_is_under_construction.'], // 1 - Area is under construction.
       ['OAC','2','raw:OAC','2_-_area_is_not_under_construction.'], // 2 - Area is not under construction.
       ['OAC','997',undefined,undefined], // Unpopulated

       // OBS - Ordnance Bunker Storage
       ['OBS','0','raw:OBS','unknown'], // Unknown
       ['OBS','1','raw:OBS','1_-_ordnance_is_stored_in_a_bunker.'], // 1 - Ordnance is stored in a bunker.
       ['OBS','2','raw:OBS','2_-_ordnance_is_not_bunker_stored.'], // 2 - Ordnance is not bunker stored.
       ['OBS','997',undefined,undefined], // Unpopulated

       // OCC - Overhead Clearance Category Code
       ['OCC','0','overhead_clearance','unknown'], // Unknown
       ['OCC','1','overhead_clearance','restricted'], // Restricted
       ['OCC','2','overhead_clearance','unlimited'], // Unlimited

       // OFT - Ordnance Facility Type
       ['OFT','1','raw:OFT','main_facility'], // Main Facility
       ['OFT','997',undefined,undefined], // Unpopulated
       ['OFT','999','raw:OFT','other'], // Other

       // OHD - Derived Obstacle Height/Depth Category
       ['OHD','0','raw:OHD','unknown'], // Unknown
       ['OHD','1','raw:OHD','>1.5_and_<=5.0'], // >1.5 and <=5.0
       ['OHD','2','raw:OHD','>5.0_and_<=10.0'], // >5.0 and <=10.0
       ['OHD','3','raw:OHD','>10.0_and_<=20.0'], // >10.0 and <=20.0
       ['OHD','4','raw:OHD','>20.0_and_<=40.0'], // >20.0 and <=40.0
       ['OHD','5','raw:OHD','>40.0'], // >40.0

       // OLQ - Obstruction Light Quality
       ['OLQ','0','obstruction_light','unknown'], // Unknown
       ['OLQ','1','obstruction_light','one_light'], // One Light Present
       ['OLQ','2','obstruction_light','multiple_lights'], // Multiple Lights Present
       ['OLQ','997',undefined,undefined], // Unpopulated

       // OOC - Overhead Obstruction Category
       ['OOC','0','raw:OOC','unknown'], // Unknown
       ['OOC','1','raw:OOC','viaduct,_frame_construction'], // Viaduct, frame construction
       ['OOC','2','raw:OOC','viaduct,_arc_construction'], // Viaduct, arc construction
       ['OOC','3','raw:OOC','roof'], // Roof
       ['OOC','4','raw:OOC','powerline_of_railway'], // Powerline of railway
       ['OOC','5','raw:OOC','high-tension_powerline'], // High-Tension powerline
       ['OOC','6','raw:OOC','bridge_superstructure'], // Bridge Superstructure
       ['OOC','999','raw:OOC','other'], // Other

       // OPC - Offshore Platform Classification
       ['OPC','0','offshore_construction','undefined'], // Undefined
       ['OPC','1','offshore_construction','oil_derrick/rig'], // Oil derrick/rig
       ['OPC','2','offshore_construction','production_platform'], // Production Platform
       ['OPC','3','offshore_construction','observation/research_platform'], // Observation/Research Platform
       ['OPC','4','offshore_construction','articulated_loading_platform_(alp)'], // Articulated Loading Platform (ALP)
       ['OPC','5','offshore_construction','single_anchor_leg_mooring_(salm)'], // Single anchor leg mooring (SALM)
       ['OPC','6','offshore_construction','mooring_tower'], // Mooring Tower
       ['OPC','7','offshore_construction','artificial_island'], // Artificial Island
       ['OPC','8','offshore_construction','floating_production,_storage_and_off-loading_vessel_(fpso)'], // Floating production, storage and off-loading vessel (FPSO)
       ['OPC','9','offshore_construction','accommodation_platform'], // Accommodation Platform
       ['OPC','10','offshore_construction','navigation_aid_support'], // Navigation Aid Support
       ['OPC','999','offshore_construction','other'], // Other

       // OPS - Operational Status
       ['OPS','0','operational_status','unknown'], // Unknown
       ['OPS','1','operational_status','operational'], // Operational
       ['OPS','2','operational_status','non-operational'], // Non-Operational
       ['OPS','997',undefined,undefined], // Unpopulated
       // ['OPS','998',undefined,undefined], // Not Applicable
       ['OPS','998','operational_status','not_applicable'], // Not Applicable

       // ORD - Ordinal Category
       ['ORD','0','raw:ORD','unknown'], // Unknown
       ['ORD','1','raw:ORD','primary/1st_order'], // Primary/1st Order
       ['ORD','2','raw:ORD','secondary/2nd_order'], // Secondary/2nd Order
       ['ORD','3','raw:ORD','tertiary/3rd_order'], // Tertiary/3rd Order
       ['ORD','4','raw:ORD','quaternary/4th_order'], // Quaternary/4th Order
       ['ORD','5','raw:ORD','quintary/5th_order'], // Quintary/5th Order
       ['ORD','6','raw:ORD','national'], // National
       ['ORD','997',undefined,undefined], // Unpopulated
       ['ORD','999','raw:ORD','other'], // Other

       // ORIGINATING_SOURCE - Originating Source Name
       ['ORIGINATING_SOURCE','Arc Digitized Raster Graphic (ADRG)','source:name','arc_digitized_raster_graphic_(adrg)'], // Arc Digitized Raster Graphic (ADRG)
       ['ORIGINATING_SOURCE','Automated Aeronautical Facilities Information File (AAFIF)','source:name','automated_aeronautical_facilities_information_file_(aafif)'], // Automated Aeronautical Facilities Information File (AAFIF)
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
       ['ORIGINATING_SOURCE','Foundation Feature Data/Relocatable Target Data (FFD/RTAD)','source:name','foundation_feature_data/relocatable_target_data_(ffd/rtad)'], // Foundation Feature Data/Relocatable Target Data (FFD/RTAD)
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
       ['ORIGINATING_SOURCE','Nominally Attributed Topographic Evaluation Map (NATE Map)','source:name','nominally_attributed_topographic_evaluation_map_(nate_map)'], // Nominally Attributed Topographic Evaluation Map (NATE Map)
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
       ['OWO','997',undefined,undefined], // Unpopulated

       // PAT - Color Pattern Category
       ['PAT','0','raw:PAT','unknown'], // Unknown
       ['PAT','1','raw:PAT','checkered'], // Checkered
       ['PAT','2','raw:PAT','diagonal_bands'], // Diagonal Bands
       ['PAT','3','raw:PAT','single_color'], // Single Color
       ['PAT','4','raw:PAT','horizontal_bands'], // Horizontal Bands
       ['PAT','6','raw:PAT','vertical_stripes'], // Vertical Stripes
       ['PAT','98','raw:PAT','squared'], // Squared
       ['PAT','99','raw:PAT','horizontal_bands_from_top_to_bottom'], // Horizontal bands from top to bottom
       ['PAT','999','raw:PAT','other'], // Other

       // PBV - Pilot Boarding Vehicle
       ['PBV','0','pilot_boarding_vehicle','unknown'], // Unknown
       ['PBV','1','pilot_boarding_vehicle','boat'], // By Boat
       ['PBV','2','pilot_boarding_vehicle','helicopter'], // By Helicopter
       ['PBV','997',undefined,undefined], // Unpopulated
       // ['PBV','998',undefined,undefined], // Not Applicable
       ['PBV','998','pilot_boarding_vehicle','not_applicable'], // Not Applicable

       // PCI - Point of Change Identifier
       ['PCI','0','raw:PCI','unknown'], // Unknown
       ['PCI','1','raw:PCI','transportation_/_road_or_railroad'], // Transportation / road or railroad
       ['PCI','2','raw:PCI','hydrography_/_drainage'], // Hydrography / drainage
       ['PCI','3','raw:PCI','boundaries'], // Boundaries
       ['PCI','4','raw:PCI','road_width_change'], // Road width change
       ['PCI','5','raw:PCI','obstacles'], // Obstacles
       ['PCI','997',undefined,undefined], // Unpopulated
       // ['PCI','998',undefined,undefined], // Not Applicable
       ['PCI','998','raw:PCI','not_applicable'], // Not Applicable
       ['PCI','999','raw:PCI','other'], // Other

       // PEC - Port of Entry Classification
       ['PEC','0','raw:PEC','unknown'], // Unknown
       ['PEC','1','raw:PEC','port_of_entry'], // Port Of Entry
       ['PEC','2','raw:PEC','not_a_port_of_entry'], // Not A Port Of Entry
       ['PEC','997',undefined,undefined], // Unpopulated

       // PEV - Position Evaluation
       ['PEV','1','raw:PEV','accuracy_<=_1_nautical_mile'], // Accuracy <= 1 nautical mile
       ['PEV','2','raw:PEV','accuracy_>_1_nautical_mile_and_<=_3_nautical_miles'], // Accuracy > 1 nautical mile and <= 3 nautical miles
       ['PEV','3','raw:PEV','accuracy_>_3_nautical_miles_and_<=_5_nautical_miles'], // Accuracy > 3 nautical miles and <= 5 nautical miles
       ['PEV','4','raw:PEV','accuracy_>_5_nautical_miles_and_<=_10_nautical_miles'], // Accuracy > 5 nautical miles and <= 10 nautical miles
       ['PEV','5','raw:PEV','accuracy_uncertain'], // Accuracy uncertain

       // PH4 - Predominant Height (10 m Range)
       ['PH4','0','raw:PH4','unknown'], // Unknown
       ['PH4','1','raw:PH4','<=_10'], // <= 10
       ['PH4','2','raw:PH4','>_10_and_<=_20'], // > 10 and <= 20
       ['PH4','3','raw:PH4','>_20_and_<=_30'], // > 20 and <= 30
       ['PH4','4','raw:PH4','>_30_and_<=_40'], // > 30 and <= 40
       ['PH4','5','raw:PH4','>_40_and_<=_50'], // > 40 and <= 50
       ['PH4','6','raw:PH4','>_50_and_<=_60'], // > 50 and <= 60
       ['PH4','7','raw:PH4','>_60_and_<=_70'], // > 60 and <= 70
       ['PH4','8','raw:PH4','>_70_and_<=_80'], // > 70 and <= 80
       ['PH4','9','raw:PH4','>_80_and_<=_90'], // > 80 and <= 90
       ['PH4','10','raw:PH4','>_90_and_<=_100'], // > 90 and <= 100
       ['PH4','11','raw:PH4','>_100'], // > 100
       ['PH4','997',undefined,undefined], // Unpopulated

       // PIA - Port Index Attribute
       ['PIA','1','raw:PIA','port_is_in_world_port_index'], // Port Is In World Port Index
       ['PIA','2','raw:PIA','port_is_not_in_world_port_index'], // Port Is Not In World Port Index

       // PLT - Pipeline Type
       ['PLT','0','pipeline:type','undefined'], // Undefined
       ['PLT','1','pipeline:type','transport'], // Transport
       ['PLT','2','pipeline:type','outfall'], // Outfall
       ['PLT','3','pipeline:type','intake'], // Intake
       ['PLT','997',undefined,undefined], // Unpopulated

       // PPC - Power Plant Category
       ['PPC','0','raw:PPC','unknown'], // Unknown
       ['PPC','1','raw:PPC','hydro-electric'], // Hydro-electric
       ['PPC','2','raw:PPC','nuclear'], // Nuclear
       ['PPC','3','raw:PPC','solar'], // Solar
       ['PPC','4','raw:PPC','thermal'], // Thermal
       ['PPC','5','raw:PPC','wind'], // Wind
       ['PPC','6','raw:PPC','tidal'], // Tidal
       ['PPC','7','raw:PPC','internal_combustion'], // Internal Combustion
       ['PPC','9','raw:PPC','substation'], // Substation
       ['PPC','10','raw:PPC','transformer_yard'], // Transformer Yard
       ['PPC','997',undefined,undefined], // Unpopulated
       // ['PPC','998',undefined,undefined], // Not Applicable
       ['PPC','998','raw:PPC','not_applicable'], // Not Applicable
       ['PPC','999','raw:PPC','other'], // Other

       // PPT - Populated Place Type
       ['PPT','0','populated_place:type','unknown'], // Unknown
       ['PPT','3','populated_place:type','tent_dwellings'], // Tent Dwellings
       ['PPT','999','populated_place:type','other'], // Other

       // PR1 - Periodic Restriction Beginning
       ['PR1','1','raw:PR1','beginning_seasonal_limit_-_jan.'], // Beginning seasonal limit - Jan.
       ['PR1','2','raw:PR1','beginning_seasonal_limit_-_feb.'], // Beginning seasonal limit - Feb.
       ['PR1','3','raw:PR1','beginning_seasonal_limit_-_mar.'], // Beginning seasonal limit - Mar.
       ['PR1','4','raw:PR1','beginning_seasonal_limit_-_apr.'], // Beginning seasonal limit - Apr.
       ['PR1','5','raw:PR1','beginning_seasonal_limit_-_may'], // Beginning seasonal limit - May
       ['PR1','6','raw:PR1','beginning_seasonal_limit_-_jun.'], // Beginning seasonal limit - Jun.
       ['PR1','7','raw:PR1','beginning_seasonal_limit_-_jul.'], // Beginning seasonal limit - Jul.
       ['PR1','8','raw:PR1','beginning_seasonal_limit_-_aug.'], // Beginning seasonal limit - Aug.
       ['PR1','9','raw:PR1','beginning_seasonal_limit_-_sep.'], // Beginning seasonal limit - Sep.
       ['PR1','10','raw:PR1','beginning_seasonal_limit_-_oct.'], // Beginning seasonal limit - Oct.
       ['PR1','11','raw:PR1','beginning_seasonal_limit_-_nov.'], // Beginning seasonal limit - Nov.
       ['PR1','12','raw:PR1','beginning_seasonal_limit_-_dec.'], // Beginning seasonal limit - Dec.
       // ['PR1','998',undefined,undefined], // Not Applicable
       ['PR1','998','raw:PR1','not_applicable'], // Not Applicable

       // PR2 - Periodic Restriction Ending
       ['PR2','1','raw:PR2','ending_seasonal_limit_-_jan.'], // Ending seasonal limit - Jan.
       ['PR2','2','raw:PR2','ending_seasonal_limit_-_feb.'], // Ending seasonal limit - Feb.
       ['PR2','3','raw:PR2','ending_seasonal_limit_-_mar.'], // Ending seasonal limit - Mar.
       ['PR2','4','raw:PR2','ending_seasonal_limit_-_apr.'], // Ending seasonal limit - Apr.
       ['PR2','5','raw:PR2','ending_seasonal_limit_-_may'], // Ending seasonal limit - May
       ['PR2','6','raw:PR2','ending_seasonal_limit_-_jun.'], // Ending seasonal limit - Jun.
       ['PR2','7','raw:PR2','ending_seasonal_limit_-_jul.'], // Ending seasonal limit - Jul.
       ['PR2','8','raw:PR2','ending_seasonal_limit_-_aug.'], // Ending seasonal limit - Aug.
       ['PR2','9','raw:PR2','ending_seasonal_limit_-_sep.'], // Ending seasonal limit - Sep.
       ['PR2','10','raw:PR2','ending_seasonal_limit_-_oct.'], // Ending seasonal limit - Oct.
       ['PR2','11','raw:PR2','ending_seasonal_limit_-_nov.'], // Ending seasonal limit - Nov.
       ['PR2','12','raw:PR2','ending_seasonal_limit_-_dec.'], // Ending seasonal limit - Dec.
       // ['PR2','998',undefined,undefined], // Not Applicable
       ['PR2','998','raw:PR2','not_applicable'], // Not Applicable

       // PRC - Periodic Restriction Category
       ['PRC','0','raw:PRC','unknown'], // Unknown
       ['PRC','1','raw:PRC','perennially_open,_not_subject_to_ice'], // Perennially Open, Not Subject to Ice
       ['PRC','2','raw:PRC','subject_to_ice'], // Subject to Ice
       ['PRC','3','raw:PRC','permanent_ice'], // Permanent Ice
       ['PRC','4','raw:PRC','seasonal_limit_-_jan.'], // Seasonal limit - Jan.
       ['PRC','5','raw:PRC','seasonal_limit_-_feb.'], // Seasonal limit - Feb.
       ['PRC','6','raw:PRC','seasonal_limit_-_mar.'], // Seasonal limit - Mar.
       ['PRC','7','raw:PRC','seasonal_limit_-_apr.'], // Seasonal limit - Apr.
       ['PRC','8','raw:PRC','seasonal_limit_-_may'], // Seasonal limit - May
       ['PRC','9','raw:PRC','seasonal_limit_-_jun.'], // Seasonal limit - Jun.
       ['PRC','10','raw:PRC','seasonal_limit_-_jul.'], // Seasonal limit - Jul.
       ['PRC','11','raw:PRC','seasonal_limit_-_aug.'], // Seasonal limit - Aug.
       ['PRC','12','raw:PRC','seasonal_limit_-_sep.'], // Seasonal limit - Sep.
       ['PRC','13','raw:PRC','seasonal_limit_-_oct.'], // Seasonal limit - Oct.
       ['PRC','14','raw:PRC','seasonal_limit_-_nov.'], // Seasonal limit - Nov.
       ['PRC','15','raw:PRC','seasonal_limit_-_dec.'], // Seasonal limit - Dec.
       ['PRC','16','raw:PRC','closed'], // Closed
       ['PRC','997',undefined,undefined], // Unpopulated
       ['PRC','999','raw:PRC','other'], // Other

       // PRM - Permanency
       ['PRM','0','raw:PRM','unknown'], // Unknown
       ['PRM','1','raw:PRM','permanent'], // Permanent
       ['PRM','2','raw:PRM','temporary'], // Temporary
       ['PRM','999','raw:PRM','other'], // Other

       // PRO - Product Category
       ['PRO','0','product','unknown'], // Unknown
       ['PRO','1','product','aircraft'], // Aircraft
       ['PRO','2','product','aluminium'], // Aluminum
       ['PRO','3','product','ammunition'], // Ammunition
       ['PRO','5','product','asphalt'], // Asphalt
       ['PRO','9','product','brick'], // Brick
       ['PRO','11','product','cement'], // Cement
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
       ['PRO','31','product','electric'], // Electric
       ['PRO','33','product','explosives'], // Explosives
       ['PRO','35','product','food'], // Food
       ['PRO','38','product','gas'], // Gas
       ['PRO','39','product','gasoline'], // Gasoline
       ['PRO','40','product','glass'], // Glass
       ['PRO','42','product','gold'], // Gold
       ['PRO','43','product','granite'], // Granite
       ['PRO','46','product','gravel'], // Gravel
       ['PRO','50','product','heat'], // Heat
       ['PRO','51','product','iron'], // Iron
       ['PRO','54','product','lead'], // Lead
       ['PRO','56','product','lumber'], // Lumber
       ['PRO','57','product','macadam'], // Macadam
       ['PRO','59','product','manganese'], // Manganese
       ['PRO','60','product','marble'], // Marble
       ['PRO','64','product','metal'], // Metal
       ['PRO','67','product','oil'], // Oil
       ['PRO','71','product','paper'], // Paper
       ['PRO','72','product','part_metal'], // Part Metal
       ['PRO','74','product','plastic'], // Plastic
       ['PRO','80','product','quartz'], // Quartz
       ['PRO','82','product','radioactive_material'], // Radioactive Material
       ['PRO','84','product','rock/rocky'], // Rock/Rocky
       ['PRO','85','product','rubber'], // Rubber
       ['PRO','87','product','salt'], // Salt
       ['PRO','88','product','sand'], // Sand
       ['PRO','89','product','sandstone'], // Sandstone
       ['PRO','95','product','sewage'], // Sewage
       ['PRO','100','product','silver'], // Silver
       ['PRO','101','product','slag'], // Slag
       ['PRO','103','product','snow/ice'], // Snow/Ice
       ['PRO','107','product','steel'], // Steel
       ['PRO','108','product','stone'], // Stone
       ['PRO','109','product','sugar'], // Sugar
       ['PRO','110','product','travertine'], // Travertine
       ['PRO','112','product','uranium'], // Uranium
       ['PRO','113','product','vegetation_products'], // Vegetation Products
       ['PRO','116','product','water'], // Water
       ['PRO','117','product','wood'], // Wood
       ['PRO','118','product','zinc'], // Zinc
       ['PRO','119','product','bauxite'], // Bauxite
       ['PRO','120','product','bananas'], // Bananas
       ['PRO','121','product','cotton'], // Cotton
       ['PRO','122','product','bamboo'], // Bamboo
       ['PRO','123','product','coffee'], // Coffee
       ['PRO','124','product','common_fruit_and/or_nuts'], // Common fruit and/or nuts
       ['PRO','125','product','palms'], // Palms
       ['PRO','126','product','palmetto'], // Palmetto
       ['PRO','127','product','tailings'], // Tailings
       ['PRO','128','product','refuse'], // Refuse
       ['PRO','129','product','tobacco'], // Tobacco
       ['PRO','130','product','none'], // None
       ['PRO','131','product','personnel'], // Personnel
       ['PRO','133','product','telecommunications'], // Telecommunications
       ['PRO','134','product','fish'], // Fish
       ['PRO','135','product','textile'], // Textile
       ['PRO','137','product','automobiles_and_trucks'], // Automobiles and Trucks
       ['PRO','140','product','ore'], // Ore
       ['PRO','148','product','grain'], // Grain
       ['PRO','996',undefined,undefined], // Unpopulated
       // ['PRO','997',undefined,undefined], // Not Applicable
       ['PRO','997','product','not_applicable'], // Not Applicable
       ['PRO','998','product','multiple'], // Multiple
       ['PRO','999','product','other'], // Other

       // PRT - Power Line Category
       ['PRT','1','raw:PRT','aa_-_feature_is_metal_construction,_height_>_149'], // Aa - Feature Is Metal Construction, Height > 149
       ['PRT','2','raw:PRT','ab_-_feature_is_metal_construction,_height_100-149.'], // Ab - Feature Is Metal Construction, Height 100-149.
       ['PRT','3','raw:PRT','ac_-feature_is_metal_construction,_height_<_100.'], // Ac -Feature Is Metal Construction, Height < 100.
       ['PRT','4','raw:PRT','ba_-feature_is_concrete_construction,_height_>_149.'], // Ba -Feature Is Concrete Construction, Height > 149.
       ['PRT','5','raw:PRT','bb_-_feature_is_concrete_construction,_height_100-149.'], // Bb - Feature Is Concrete Construction, Height 100-149.
       ['PRT','6','raw:PRT','bc_-feature_is_concrete_construction,_height_<_100.'], // Bc -Feature Is Concrete Construction, Height < 100.
       ['PRT','7','raw:PRT','ca_-feature_is_wood_construction,_height_>_149.'], // Ca -Feature Is Wood Construction, Height > 149.
       ['PRT','8','raw:PRT','cb_-feature_is_wood_construction,_height_100-_149.'], // Cb -Feature Is Wood Construction, Height 100- 149.
       ['PRT','9','raw:PRT','cc_-_feature_is_wood_construction,_height_<_100.'], // Cc - Feature Is Wood Construction, Height < 100.

       // PSC - Physical Surface Characteristics
       ['PSC','0','physical:characteristic','unknown'], // Unknown
       ['PSC','1','physical:characteristic','broken'], // Broken
       ['PSC','2','physical:characteristic','coarse'], // Coarse
       ['PSC','3','physical:characteristic','decayed'], // Decayed
       ['PSC','4','physical:characteristic','fine'], // Fine, minute particles
       ['PSC','5','physical:characteristic','gritty'], // Gritty
       ['PSC','6','physical:characteristic','hard'], // Hard
       ['PSC','7','physical:characteristic','rotten'], // Rotten
       ['PSC','8','physical:characteristic','soft'], // Soft
       ['PSC','9','physical:characteristic','sticky'], // Sticky
       ['PSC','10','physical:characteristic','stiff'], // Stiff
       ['PSC','11','physical:characteristic','streaky'], // Streaky
       ['PSC','12','physical:characteristic','tenacious'], // Tenacious
       ['PSC','13','physical:characteristic','uneven'], // Uneven
       ['PSC','17','physical:characteristic','calcareous'], // Calcareous
       ['PSC','18','physical:characteristic','flinty'], // Flinty
       ['PSC','19','physical:characteristic','glacial'], // Glacial
       ['PSC','20','physical:characteristic','ground'], // Ground
       ['PSC','21','physical:characteristic','large'], // Large
       ['PSC','22','physical:characteristic','rocky'], // Rocky
       ['PSC','23','physical:characteristic','small'], // Small
       ['PSC','24','physical:characteristic','speckled'], // Speckled
       ['PSC','25','physical:characteristic','varied'], // Varied
       ['PSC','26','physical:characteristic','volcanic'], // Volcanic
       ['PSC','27','physical:characteristic','medium'], // Medium
       ['PSC','28','physical:characteristic','springs_in_seabed'], // Springs in Seabed
       ['PSC','29','physical:characteristic','mobile_bottom'], // Mobile Bottom
       ['PSC','997',undefined,undefined], // Unpopulated

       // PWC - Pier/Wharf /Quay Classification
       ['PWC','0','raw:PWC','unknown'], // Unknown
       ['PWC','1','raw:PWC','pier'], // Pier
       ['PWC','2','raw:PWC','wharf'], // Wharf
       ['PWC','3','raw:PWC','quay'], // Quay

       // PXT - Pipeline or Pipe Crossing Point Type
       ['PXT','1','raw:PXT','below_ground_surface'], // Below Ground Surface
       ['PXT','2','raw:PXT','on_ground_surface'], // On Ground Surface
       ['PXT','3','raw:PXT','elevated'], // Elevated

       // RAG - Non-submarine Contact Reporting Agency Type
       ['RAG','0','raw:RAG','unknown'], // Unknown
       ['RAG','1','raw:RAG','survivor_report'], // Survivor Report
       ['RAG','2','raw:RAG','salvage_report'], // Salvage Report
       ['RAG','3','raw:RAG','casualty_report'], // Casualty Report
       ['RAG','4','raw:RAG','action_report'], // Action Report
       ['RAG','5','raw:RAG','photograph_report'], // Photograph Report
       ['RAG','6','raw:RAG','us_coast_and_geodetic_survey/national_ocean_service_report'], // US Coast & Geodetic Survey/National Ocean Service Report
       ['RAG','7','raw:RAG','us_naval_headquarters/commands_report'], // US Naval Headquarters/Commands Report
       ['RAG','8','raw:RAG','hydrographic/admiralty_office'], // Hydrographic/Admiralty Office
       ['RAG','9','raw:RAG','wreck_list'], // Wreck List
       ['RAG','10','raw:RAG','chart_records'], // Chart Records
       ['RAG','11','raw:RAG','coast_guard_records'], // Coast Guard Records
       ['RAG','12','raw:RAG','notice_to_mariners'], // Notice to Mariners
       ['RAG','13','raw:RAG','north_sea_fishing_charts'], // North Sea Fishing Charts
       ['RAG','14','raw:RAG','chart'], // Chart
       ['RAG','15','raw:RAG','minesweeper'], // Minesweeper
       ['RAG','16','raw:RAG','survey'], // Survey
       ['RAG','17','raw:RAG','position_accurate_field_check'], // Position Accurate Field Check
       ['RAG','18','raw:RAG','sonar_report'], // Sonar Report
       ['RAG','19','raw:RAG','mad_report'], // MAD Report
       ['RAG','20','raw:RAG','undifferentiated_sonar_or_mad_report'], // Undifferentiated Sonar or MAD Report
       ['RAG','997',undefined,undefined], // Unpopulated
       // ['RAG','998',undefined,undefined], // Not Applicable
       ['RAG','998','raw:RAG','not_applicable'], // Not Applicable
       ['RAG','999','raw:RAG','other'], // Other

       // RAH - Runway High End Approach Lights
       ['RAH','0','raw:RAH','unknown'], // Unknown
       ['RAH','1','raw:RAH','available'], // Available
       ['RAH','2','raw:RAH','not_available'], // Not Available
       ['RAH','997',undefined,undefined], // Unpopulated

       // RAL - Runway Low End Approach Lights
       ['RAL','0','raw:RAL','unknown'], // Unknown
       ['RAL','1','raw:RAL','available'], // Available
       ['RAL','2','raw:RAL','not_available'], // Not Available
       ['RAL','997',undefined,undefined], // Unpopulated

       // RBC - Reliability of Bridge
       ['RBC','0','bridge:reliability','unknown'], // Unknown
       ['RBC','1','bridge:reliability','known'], // Known
       ['RBC','2','bridge:reliability','estimated'], // Estimated
       ['RBC','997',undefined,undefined], // Unpopulated

       // RCC - Radio Class Code
       ['RCC','0','raw:RCC','unknown'], // Unknown
       ['RCC','1','raw:RCC','non-directional_radio_beacon_(homing),_power_50_to_less_than_2000_watts'], // Non-directional radio beacon (homing), power 50 to less than 2000 watts
       ['RCC','2','raw:RCC','normal_anticipated_interference-free_service,_40nm_up_to_18,000_feet'], // Normal anticipated interference-free service, 40NM up to 18,000 feet
       ['RCC','3','raw:RCC','normal_anticipated_interference-free_service,_25nm_up_to_12,000_feet'], // Normal anticipated interference-free service, 25NM up to 12,000 feet

       // RCL - Runway Centerline Lights
       ['RCL','0','raw:RCL','unknown'], // Unknown
       ['RCL','1','raw:RCL','available'], // Available
       ['RCL','2','raw:RCL','not_available'], // Not Available
       ['RCL','997',undefined,undefined], // Unpopulated

       // REF_ - Radar Reflector Attribute
       ['REF_','1','radar_reflector','radar_reflector_present'], // Radar Reflector Present
       ['REF_','2','radar_reflector','radar_reflector_absent'], // Radar Reflector Absent
       ['REF_','997',undefined,undefined], // Unpopulated

       // REL - Religious Denomination
       ['REL','0','religion','unknown'], // Unknown
       ['REL','1','religion','buddhist'], // Buddhist
       ['REL','2','religion','muslim'], // Moslem
       ['REL','3','religion','roman_catholic'], // Roman Catholic
       ['REL','4','religion','christian'], // Christian (undefined)
       ['REL','5','religion','jewish'], // Judaism
       ['REL','6','religion','orthodox'], // Greek Orthodox
       ['REL','7','religion','protestant'], // Protestant
       ['REL','8','religion','shinto'], // Shinto
       // ['REL','998',undefined,undefined], // Not Applicable
       ['REL','998','religious_denomination','not_applicable'], // Not Applicable
       ['REL','999','religion','other'], // Other

       // RFL - Runway Flood Lights
       ['RFL','0','raw:RFL','unknown'], // Unknown
       ['RFL','1','raw:RFL','available'], // Available
       ['RFL','2','raw:RFL','not_available'], // Not Available
       ['RFL','997',undefined,undefined], // Unpopulated

       // RGC - Railroad Gauge Category
       ['RGC','0','gauge:type','unknown'], // Unknown
       ['RGC','1','gauge:type','broad'], // Broad
       ['RGC','2','gauge:type','narrow'], // Narrow
       ['RGC','3','gauge:type','standard'], // Normal (Country Specific)
       ['RGC','997',undefined,undefined], // Unpopulated

       // RGS - Range Significance
       ['RGS','1','raw:RGS','front'], // Front
       ['RGS','2','raw:RGS','middle'], // Middle
       ['RGS','3','raw:RGS','rear'], // Rear
       ['RGS','4','raw:RGS','shared'], // Shared

       // RHI - Runway High End Identifier Lights
       ['RHI','0','raw:RHI','unknown'], // Unknown
       ['RHI','1','raw:RHI','available'], // Available
       ['RHI','2','raw:RHI','not_available'], // Not Available
       ['RHI','997',undefined,undefined], // Unpopulated

       // RIT - Road Interchange Type
       ['RIT','0','raw:RIT','unknown'], // Unknown
       ['RIT','1','raw:RIT','cloverleaf'], // Cloverleaf
       ['RIT','2','raw:RIT','diamond'], // Diamond
       ['RIT','3','raw:RIT','fork'], // Fork
       ['RIT','4','raw:RIT','rotary/traffic_circle/roundabout'], // Rotary/Traffic Circle/Roundabout
       ['RIT','5','raw:RIT','staggered_ramps'], // Staggered Ramps
       ['RIT','6','raw:RIT','standard_ramps'], // Standard Ramps
       ['RIT','7','raw:RIT','symmetrical_ramps'], // Symmetrical Ramps
       ['RIT','8','raw:RIT','trumpet'], // Trumpet
       ['RIT','9','raw:RIT','turban'], // Turban
       ['RIT','10','raw:RIT','wye'], // Wye
       ['RIT','997',undefined,undefined], // Unpopulated
       ['RIT','999','raw:RIT','other'], // Other

       // RKF - Rock Strata Formation
       ['RKF','0','rock_formation','unknown'], // Unknown
       ['RKF','1','rock_formation','columnar'], // Columnar
       ['RKF','3','rock_formation','pinnacle'], // Pinnacle
       ['RKF','997',undefined,undefined], // Unpopulated

       // RLI - Runway Low End Identifier Lights
       ['RLI','0','raw:RLI','unknown'], // Unknown
       ['RLI','1','raw:RLI','available'], // Available
       ['RLI','2','raw:RLI','not_available'], // Not Available
       ['RLI','997',undefined,undefined], // Unpopulated

       // RPA - Required Port Access
       ['RPA','0','port:access','unknown'], // Unknown
       ['RPA','1','port:access','required'], // Access Required
       ['RPA','2','port:access','not_required'], // Access Not Required

       // RPC - Boundary Representation Policy Code
       ['RPC','0','raw:RPC','unknown'], // Unknown
       ['RPC','1','raw:RPC','show_no_line'], // Show No Line
       ['RPC','2','raw:RPC','definite'], // Definite
       ['RPC','3','raw:RPC','indefinite'], // Indefinite
       ['RPC','4','raw:RPC','in_dispute'], // In Dispute
       ['RPC','5','raw:RPC','administrative_as_international'], // Administrative As International
       ['RPC','999','raw:RPC','other'], // Other

       // RPD - Reference Point Determination Method
       ['RPD','1','raw:RPD','one_runway'], // One Runway
       ['RPD','2','raw:RPD','two_parallel_or_non-connecting_semi-parallel_runways'], // Two Parallel Or Non-Connecting Semi-Parallel Runways
       ['RPD','3','raw:RPD','three_or_more_parallel_or_non-connecting_semi-parallel_runways'], // Three Or More Parallel Or Non-Connecting Semi-Parallel Runways
       ['RPD','4','raw:RPD','runways_intersecting_at_a_single_point'], // Runways Intersecting At A Single Point
       ['RPD','5','raw:RPD','intersecting_runways_forming_one_or_more_interior_triangles'], // Intersecting Runways Forming One Or More Interior Triangles
       ['RPD','6','raw:RPD','two_runways_diverging_90_degrees_or_less'], // Two Runways Diverging 90 Degrees Or Less
       ['RPD','7','raw:RPD','two_runways_diverging_over_90_degrees.'], // Two Runways Diverging Over 90 Degrees.
       ['RPD','8','raw:RPD','widely_separated_runways'], // Widely Separated Runways
       ['RPD','997',undefined,undefined], // Unpopulated

       // RRA - Railroad Power Source
       ['RRA','0','electrified','unknown'], // Unknown
       ['RRA','1','electrified','rail'], // Electrified Track
       ['RRA','3','electrified','contact_line'], // Overhead Electrified
       ['RRA','4','electrified','no'], // Non-electrified
       ['RRA','997',undefined,undefined], // Unpopulated
       ['RRA','999','electrified','other'], // Other

       // RRC - Railroad Categories
       ['RRC','0','raw:RRC','unknown'], // Unknown
       ['RRC','2','raw:RRC','car-line'], // Car-Line
       ['RRC','3','raw:RRC','monorail'], // Monorail
       ['RRC','8','raw:RRC','logging'], // Logging
       ['RRC','11','raw:RRC','rapid_transit_route_-_rail'], // Rapid Transit Route - Rail
       ['RRC','14','raw:RRC','tramway'], // Tramway
       ['RRC','15','raw:RRC','inclined_railway'], // Inclined Railway
       ['RRC','16','raw:RRC','main_line'], // Main Line
       ['RRC','17','raw:RRC','branch_line'], // Branch Line
       ['RRC','18','raw:RRC','main_line/branch_line'], // Main Line/Branch Line
       ['RRC','21','raw:RRC','railroad_in_road'], // Railroad in Road
       ['RRC','31','raw:RRC','magnetic_levitation'], // Magnetic Levitation
       ['RRC','997',undefined,undefined], // Unpopulated
       ['RRC','999','raw:RRC','other'], // Other

       // RSA - Rail Siding/Spur Attribute
       ['RSA','0','service','unknown'], // Unknown
       ['RSA','1','service','spur'], // Spur
       ['RSA','2','service','siding'], // Siding
       ['RSA','3','service','passing'], // Passing
       ['RSA','997',undefined,undefined], // Unpopulated

       // RSL - Runway Strip Lights
       ['RSL','0','raw:RSL','unknown'], // Unknown
       ['RSL','1','raw:RSL','available'], // Available
       ['RSL','2','raw:RSL','not_available'], // Not Available
       ['RSL','997',undefined,undefined], // Unpopulated

       // RST - Road/Runway Surface Type
       ['RST','0',undefined,undefined], // Unknown
       ['RST','1','surface','paved'], // Hard/Paved
       ['RST','2','surface','unpaved'], // Loose/Unpaved
       ['RST','5','surface','grass'], // Grass/Sod (Soft)
       ['RST','6','surface','ground'], // Natural
       ['RST','997',undefined,undefined], // Unpopulated
       ['RST','999','surface','other'], // Other

       // RTP - Reservoir Type
       ['RTP','0','reservoir:type','unknown'], // Unknown
       ['RTP','1','reservoir:type','constructed_basin'], // Constructed Basin
       ['RTP','2','reservoir:type','back-up_water_impounded_by_a_dam'], // Back-up Water Impounded by a Dam

       // RTT - Route Intended Use
       ['RTT','0','seamark:route','unknown'], // Unknown
       ['RTT','2','seamark:route','recommended_track_for_other_than_deep_draft_vessels'], // Recommended Track for Other Than Deep Draft Vessels
       ['RTT','3','seamark:route','recommended_track_for_deep_draft_vessels'], // Recommended Track for Deep Draft Vessels
       ['RTT','4','seamark:route','deep_water_route'], // Deep Water Route
       ['RTT','5','seamark:route','transit_route'], // Transit Route
       ['RTT','11','seamark:route','two-way_route'], // Two-way Route
       ['RTT','13','seamark:route','recommended_direction_of_traffic_flow'], // Recommended Direction of Traffic Flow
       ['RTT','14','highway','primary'], // Primary Route
       ['RTT','15','highway','secondary'], // Secondary Route
       ['RTT','16','highway','motorway'], // Limited Access Route (e.g. Motorway/Autobahn/Interstate)
       ['RTT','17','seamark:route','q-route'], // Q-Route
       ['RTT','18','seamark:route','recommended_route'], // Recommended Route
       ['RTT','997',undefined,undefined], // Unpopulated
       ['RTT','999','seamark:route','other'], // Other

       // SBC - Shelter Belt Condition
       ['SBC','0','shelter_belt','unknown'], // Unknown
       ['SBC','1','shelter_belt','yes'], // Functions as a shelter belt
       ['SBC','2','shelter_belt','no'], // Does not function as a shelter belt

       // SCB - Surface Condition Attribute
       ['SCB','0','surface:condition','unknown'], // Unknown
       ['SCB','1','surface:condition','good'], // Good - No cracks or potholes.
       ['SCB','2','surface:condition','fair'], // Fair - Some cracks or potholes.
       ['SCB','3','surface:condition','poor'], // Poor - Extensive cracks or potholes.
       ['SCB','997',undefined,undefined], // Unpopulated

       // SCC - Spring/Well Characteristic Category
       ['SCC','0','water:type','unknown'], // Unknown
       ['SCC','1','water:type','alkaline'], // Alkaline
       // ['SCC','2',undefined,undefined], // Not Applicable
       ['SCC','2','water:type','not_applicable'], // Not Applicable
       ['SCC','4','water:type','mineral'], // Mineral
       ['SCC','9','water:type','potable'], // Freshwater/Potable
       ['SCC','10','water:type','salt'], // Salt
       ['SCC','11','water:type','fresh'], // Fresh
       ['SCC','997',undefined,undefined], // Unpopulated
       ['SCC','999','water:type','other'], // Other

       // SD1 - Stem Diameter Size Range (1)
       ['SD1','0','raw:SD1','unknown'], // Unknown
       ['SD1','1','raw:SD1','>_0_and_<=_5.00'], // > 0 and <= 5.00
       ['SD1','2','raw:SD1','>_5.00_and_<=_10.00'], // > 5.00 and <= 10.00
       ['SD1','3','raw:SD1','>_10.00_and_<=_20.00'], // > 10.00 and <= 20.00
       ['SD1','4','raw:SD1','>_20.00_and_<=_30.00'], // > 20.00 and <= 30.00
       ['SD1','5','raw:SD1','>_30.00_and_<=_40.00'], // > 30.00 and <= 40.00
       ['SD1','6','raw:SD1','>_40.00_and_<=_60.00'], // > 40.00 and <= 60.00
       ['SD1','7','raw:SD1','>_60.00'], // > 60.00
       // ['SD1','8',undefined,undefined], // Not Applicable
       ['SD1','8','raw:SD1','not_applicable'], // Not Applicable
       ['SD1','997',undefined,undefined], // Unpopulated

       // SD2 - Stem Diameter Size Range (2)
       ['SD2','0','raw:SD2','unknown'], // Unknown
       ['SD2','1','raw:SD2','>_0_and_<=_10.00'], // > 0 and <= 10.00
       ['SD2','2','raw:SD2','>_10.00_and_<=_30.00'], // > 10.00 and <= 30.00
       ['SD2','3','raw:SD2','>_30.00_and_<=_60.00'], // > 30.00 and <= 60.00
       ['SD2','4','raw:SD2','>_60.00_and_<=_100.00'], // > 60.00 and <= 100.00
       ['SD2','5','raw:SD2','>_100.00'], // > 100.00
       // ['SD2','6',undefined,undefined], // Not Applicable
       ['SD2','6','raw:SD2','not_applicable'], // Not Applicable
       ['SD2','997',undefined,undefined], // Unpopulated

       // SDE - Soil Depth With Greater Precision
       ['SDE','0','raw:SDE','unknown'], // Unknown
       ['SDE','1','raw:SDE','<=0.25'], // <=0.25
       ['SDE','2','raw:SDE','>0.25_and_<=0.5'], // >0.25 and <=0.5
       ['SDE','3','raw:SDE','>0.5_and_<=1.5'], // >0.5 and <=1.5
       ['SDE','4','raw:SDE','>1.5_and_<=2.5'], // >1.5 and <=2.5
       ['SDE','5','raw:SDE','>2.5_and_<=5.0'], // >2.5 and <=5.0
       ['SDE','6','raw:SDE','>5.0_and_<=10.0'], // >5.0 and <=10.0
       ['SDE','7','raw:SDE','>10.0'], // >10.0

       // SEA - Sea Area Classification
       ['SEA','1','raw:SEA','sea_area_in_general'], // Sea area in general
       // ['SEA','998',undefined,undefined], // Not Applicable
       ['SEA','998','raw:SEA','not_applicable'], // Not Applicable

       // SEC - Security Classification
       ['SEC','0','security:classification','unknown'], // Unknown
       ['SEC','1','security:classification','TOP_SECRET'], // Top Secret
       ['SEC','2','security:classification','SECRET'], // Secret
       ['SEC','3','security:classification','CONFIDENTIAL'], // Confidential
       ['SEC','4','security:classification','RESTRICTED'], // Restricted
       ['SEC','5','security:classification','UNCLASSIFIED'], // Unclassified
       ['SEC','999','security:classification','other'], // Other

       // SEC_CLASS - Security Classification
       ['SEC_CLASS','01','security:classification','cosmic_top_secret-bohemia'], // COSMIC Top Secret-Bohemia
       ['SEC_CLASS','02','security:classification','cosmic_top_secret-balk'], // COSMIC Top Secret-Balk
       ['SEC_CLASS','03','security:classification','nato_secret'], // NATO Secret
       ['SEC_CLASS','04','security:classification','nato_secret-savate'], // NATO Secret-Savate
       ['SEC_CLASS','05','security:classification','nato_secret-avicula'], // NATO Secret-Avicula
       ['SEC_CLASS','06','security:classification','nato_confidential'], // NATO Confidential
       ['SEC_CLASS','07','security:classification','nato_restricted'], // NATO Restricted
       ['SEC_CLASS','08','security:classification','nato_unclassified'], // NATO Unclassified
       ['SEC_CLASS','09','security:classification','cosmic_top_secret_atom'], // COSMIC Top Secret Atom
       ['SEC_CLASS','10','security:classification','secret_atomal'], // Secret Atomal
       ['SEC_CLASS','11','security:classification','confidential_atomal'], // Confidential Atomal
       ['SEC_CLASS','C','security:classification','CONFIDENTIAL'], // Confidential
       ['SEC_CLASS','S','security:classification','SECRET'], // Secret
       ['SEC_CLASS','TS','security:classification','TOP_SECRET'], // Top Secret
       ['SEC_CLASS','U','security:classification','UNCLASSIFIED'], // Unclassified

       // SEC_CLS_AUTHORITY - Classification Authority
       ['SEC_CLS_AUTHORITY','D_IMGPOL','security:classification:authority','D_IMGPOL'], // Classification Derived from Imagery Policy Series Classification Guide
       ['SEC_CLS_AUTHORITY','D_MIDB','security:classification:authority','D_MIDB'], // Classification Derived from DoD MIDB Security Classification Guide
       ['SEC_CLS_AUTHORITY','D_NGA','security:classification:authority','D_NGA'], // Classification Derived from NGA Security Classification Guide
       ['SEC_CLS_AUTHORITY','D_OPNAV','security:classification:authority','D_OPNAV'], // Classification Derived from Op Nav Inst S5513.5B-24
       ['SEC_CLS_AUTHORITY','I','security:classification:authority','I'], // Classified by Director, Central Intelligence
       ['SEC_CLS_AUTHORITY','M','security:classification:authority','M'], // Classification Derived from Multiple Sources
       ['SEC_CLS_AUTHORITY','O','security:classification:authority','O'], // Classified by some other means

       // SEP - Road Separation Category
       ['SEP','1',undefined,undefined], // Not Separated
       ['SEP','2','is_divided','yes'], // Separated

       // SFC - Sea Floor Feature Category
       ['SFC','0','sea_floor_category','unknown_(obstruction)'], // Unknown (Obstruction)
       ['SFC','3','sea_floor_category','fish_haven'], // Fish Haven
       ['SFC','4','sea_floor_category','well'], // Well
       ['SFC','5','sea_floor_category','submerged_production_platform'], // Submerged Production Platform
       ['SFC','6','sea_floor_category','wreckage'], // Wreckage
       ['SFC','7','sea_floor_category','shoaling'], // Shoaling
       ['SFC','8','sea_floor_category','less_water_reported'], // Less Water Reported
       ['SFC','9','sea_floor_category','unexploded_ordnance'], // Unexploded Ordnance
       ['SFC','10','sea_floor_category','unspecified_non-submarine_contact'], // Unspecified Non-submarine Contact
       ['SFC','11','sea_floor_category','pinnacle'], // Pinnacle
       ['SFC','997',undefined,undefined], // Unpopulated
       ['SFC','999','sea_floor_category','other'], // Other

       // SHA - Sounding Horizontal Accuracy Code
       ['SHA','0','raw:SHA','0_-_unknown'], // 0 - Unknown
       ['SHA','1','raw:SHA','1_-_best'], // 1 - Best
       ['SHA','2','raw:SHA','2_-_good'], // 2 - Good
       ['SHA','3','raw:SHA','3_-_fair'], // 3 - Fair
       ['SHA','4','raw:SHA','4_-_poor'], // 4 - Poor
       ['SHA','5','raw:SHA','n/a'], // N/A
       ['SHA','997',undefined,undefined], // Unpopulated

       // SHL - Shoreline Type Left
       ['SHL','0','raw:SHL','unknown'], // Unknown
       ['SHL','6','raw:SHL','mangrove/nipa'], // Mangrove/Nipa
       ['SHL','8','raw:SHL','marsh,_swamp'], // Marsh, Swamp
       ['SHL','10','raw:SHL','rocky'], // Rocky
       ['SHL','11','raw:SHL','rubble'], // Rubble
       ['SHL','13','raw:SHL','sandy'], // Sandy
       ['SHL','14','raw:SHL','stony,_shingly'], // Stony, Shingly
       ['SHL','997',undefined,undefined], // Unpopulated
       ['SHL','999','raw:SHL','other'], // Other

       // SHO - Shoreline Category
       ['SHO','2','raw:SHO','flat'], // Flat
       ['SHO','6','raw:SHO','steep'], // Steep
       ['SHO','7','raw:SHO','glacier'], // Glacier
       ['SHO','8','raw:SHO','ice_coast'], // Ice Coast

       // SHP - Shape of Beacon
       ['SHP','0','seamark:beacon:shape','undefined'], // Undefined
       ['SHP','1','seamark:beacon:shape','stake_or_pole'], // Stake/Pole
       ['SHP','2','seamark:beacon:shape','withy'], // Withy
       ['SHP','3','seamark:beacon:shape','tower'], // Beacon Tower
       ['SHP','4','seamark:beacon:shape','lattice'], // Lattice Beacon
       ['SHP','5','seamark:beacon:shape','pile'], // Pile Beacon
       ['SHP','7','seamark:beacon:shape','buoyant'], // Buoyant Beacon
       ['SHP','8','seamark:daymark:shape','square'], // Daymark Board - Square
       ['SHP','9','seamark:daymark:shape','triangle'], // Daymark Board - Triangle
       ['SHP','10','seamark:daymark:shape','rectangle'], // Daymark Board - Rectangle

       // SHR - Shoreline Type Right
       ['SHR','0','raw:SHR','unknown'], // Unknown
       ['SHR','6','raw:SHR','mangrove/nipa'], // Mangrove/Nipa
       ['SHR','8','raw:SHR','marsh,_swamp'], // Marsh, Swamp
       ['SHR','10','raw:SHR','rocky'], // Rocky
       ['SHR','11','raw:SHR','rubble'], // Rubble
       ['SHR','13','raw:SHR','sandy'], // Sandy
       ['SHR','14','raw:SHR','stony,_shingly'], // Stony, Shingly
       ['SHR','997',undefined,undefined], // Unpopulated
       ['SHR','999','raw:SHR','other'], // Other

       // SIC - Snow/Ice Category
       ['SIC','0','landcover','unknown'], // Unknown
       ['SIC','1','landcover','snowfield'], // Snow
       ['SIC','2','landcover','ice-field'], // Ice

       // SL1 - Slope Gradient Left (1)
       ['SL1','0','raw:SL1','unknown'], // Unknown
       ['SL1','1','raw:SL1','<=_30'], // <= 30
       ['SL1','2','raw:SL1','>_30_and_<=_45'], // > 30 and <= 45
       ['SL1','3','raw:SL1','>_45_and_<=_60'], // > 45 and <= 60
       ['SL1','4','raw:SL1','>_60'], // > 60
       ['SL1','997',undefined,undefined], // Unpopulated

       // SLT - Shoreline Type Category
       ['SLT','0','shoreline:type','unknown'], // Unknown
       ['SLT','6','shoreline:type','mangrove'], // Mangrove/Nipa
       ['SLT','8','shoreline:type','marsh'], // Marsh, Swamp
       ['SLT','10','shoreline:type','stony'], // Rocky
       ['SLT','11','shoreline:type','building_rubble'], // Rubble
       ['SLT','13','shoreline:type','sandy'], // Sandy
       ['SLT','14','shoreline:type','shingly'], // Stony, Shingly
       ['SLT','997',undefined,undefined], // Unpopulated
       ['SLT','999','shoreline:type','other'], // Other

       // SM1 - Surficial Material Depth Category
       ['SM1','0','raw:SM1','unknown'], // Unknown
       ['SM1','1','raw:SM1','<_0.5_(metres)'], // < 0.5 (Metres)
       ['SM1','2','raw:SM1','>=_0.5_(metres)'], // >= 0.5 (Metres)
       ['SM1','997',undefined,undefined], // Unpopulated

       // SMC - Surface Material Category
       ['SMC','0','surface','unknown'], // Unknown
       ['SMC','2','surface','aluminium'], // Aluminum
       ['SMC','4','surface','ash'], // Ash
       ['SMC','5','surface','asphalt'], // Asphalt
       ['SMC','6','surface','basalt'], // Basalt
       ['SMC','7','surface','bedrock'], // Bedrock
       ['SMC','8','surface','boulders'], // Boulders
       ['SMC','9','surface','brick'], // Brick
       ['SMC','10','surface','calcareous'], // Calcareous
       ['SMC','12','surface','chalk'], // Chalk
       ['SMC','14','surface','cinders'], // Cinders
       ['SMC','15','surface','cirripedia'], // Cirripedia
       ['SMC','16','surface','clay'], // Clay
       ['SMC','17','surface','coal'], // Coal
       ['SMC','18','surface','cobble'], // Cobble
       ['SMC','20','surface','composition'], // Composition
       ['SMC','21','surface','concrete'], // Concrete
       ['SMC','22','surface','conglomerate'], // Conglomerate
       ['SMC','24','surface','coral'], // Coral
       ['SMC','25','surface','coral_head'], // Coral Head
       ['SMC','28','surface','diatoms'], // Diatoms
       ['SMC','29','surface','dolomite'], // Dolomite
       ['SMC','30','surface','earthen'], // Earthen
       ['SMC','36','surface','foraminifera'], // Foraminifera
       ['SMC','40','surface','glass'], // Glass
       ['SMC','41','surface','globigerina'], // Globigerina
       ['SMC','43','surface','granite'], // Granite
       ['SMC','45','surface','grass'], // Grass/Thatch
       ['SMC','46','surface','gravel'], // Gravel
       ['SMC','47','surface','green_rocks'], // Green Rocks
       ['SMC','48','surface','ground'], // Ground
       ['SMC','49','surface','ground_(shells)'], // Ground (Shells)
       ['SMC','52','surface','lava'], // Lava
       ['SMC','55','surface','loess'], // Loess
       ['SMC','58','surface','madrepores'], // Madrepores
       ['SMC','61','surface','marl'], // Marl
       ['SMC','62','surface','masonry_(brick/stone)'], // Masonry (Brick/Stone)
       ['SMC','64','surface','metal'], // Metal
       ['SMC','65','surface','mud'], // Mud
       ['SMC','66','surface','mussels'], // Mussels
       ['SMC','68','surface','oil_blister'], // Oil Blister
       ['SMC','69','surface','ooze'], // Ooze
       ['SMC','70','surface','oysters'], // Oysters
       ['SMC','72','surface','part_metal'], // Part Metal
       ['SMC','73','surface','pebbles'], // Pebbles
       ['SMC','74','surface','plastic'], // Plastic
       ['SMC','76','surface','porphyry'], // Porphyry
       ['SMC','78','surface','pteropods'], // Pteropods
       ['SMC','79','surface','pumice'], // Pumice
       ['SMC','80','surface','quartz'], // Quartz
       ['SMC','81','surface','radiolaria'], // Radiolaria
       ['SMC','83','surface','reinforced_concrete'], // Reinforced Concrete
       ['SMC','84','surface','rock'], // Rock/Rocky
       ['SMC','86','surface','rubble'], // Rubble
       ['SMC','87','surface','salt'], // Salt
       ['SMC','88','surface','sand'], // Sand
       ['SMC','89','surface','sandstone'], // Sandstone
       ['SMC','90','surface','schist'], // Schist
       ['SMC','92','surface','scoria'], // Scoria
       ['SMC','96','surface','shells'], // Shells
       ['SMC','98','surface','shingle'], // Shingle
       ['SMC','99','surface','silt'], // Silt
       ['SMC','103','surface','snow'], // Snow/Ice
       ['SMC','104','surface','soil'], // Soil
       ['SMC','106','surface','sponge'], // Sponge
       ['SMC','107','surface','steel'], // Steel
       ['SMC','108','surface','stone'], // Stone
       ['SMC','110','surface','travertine'], // Travertine
       ['SMC','111','surface','tufa'], // Tufa
       ['SMC','113','surface','vegetation_products'], // Vegetation Products
       ['SMC','114','surface','volcanic'], // Volcanic
       ['SMC','115','surface','volcanic_ash'], // Volcanic Ash
       ['SMC','116','surface','water'], // Water
       ['SMC','117','surface','wood'], // Wood
       ['SMC','119','surface','distorted_surface'], // Distorted Surface
       ['SMC','120','surface','sand_and_gravel'], // Sand and Gravel
       ['SMC','121','surface','rip-rap'], // Rip-Rap
       ['SMC','122','surface','evaporites'], // Evaporites
       ['SMC','124','surface','sand_and_boulders'], // Sand and Boulders
       ['SMC','126','surface','sand_and_mud'], // Sand and Mud
       ['SMC','127','surface','karst'], // Karst
       ['SMC','200','surface','herbaceous/scrub_vegetation_(excluding_trees)'], // Herbaceous/Scrub Vegetation (Excluding trees)
       ['SMC','201','surface','trees'], // Trees
       ['SMC','202','surface','wetland_vegetation'], // Wetland Vegetation
       ['SMC','203','surface','herbaceous_vegetation'], // Herbaceous Vegetation
       ['SMC','204','surface','treed_vegetation'], // Treed Vegetation
       ['SMC','250','surface','composite_-_50_%_or_more_of_the_runway_length_is_permanent'], // Composite - 50 % or more of the runway length is permanent
       ['SMC','251','surface','pem_-_part_concrete,_part_asphalt_or_part_bitumen_bound_macadam'], // PEM - part concrete, part asphalt or part bitumen bound macadam
       ['SMC','252','surface','paved'], // Permanent - hard surface type unknown
       ['SMC','253','surface','bituminous'], // Bituminous - tar or asphalt mixed in place, oiled
       ['SMC','254','surface','composite_soft_-_less_than_50%_of_the_runway_length_is_permanent'], // Composite Soft - less than 50% of the runway length is permanent
       ['SMC','255','surface','graded'], // Graded or rolled earth, grass on graded earth
       ['SMC','256','surface','grass'], // Grass or earth not graded or rolled
       ['SMC','257','surface','ice'], // Ice
       ['SMC','258','surface','snow'], // Snow
       ['SMC','259','surface','macadam'], // Macadam - crushed rock water bound
       ['SMC','260','surface','membrane_-_plastic_or_other_coated_fiber_material'], // Membrane - plastic or other coated fiber material
       ['SMC','261','surface','mix_-_mix_in_place_using_non-bituminous_binder_such_as_portland_cement'], // Mix - mix in place using non-bituminous binder such as portland cement
       ['SMC','262','surface','laterite'], // Laterite
       ['SMC','263','surface','sand'], // Sand - sand graded, rolled or oiled
       ['SMC','264','surface','combination'], // Combination
       ['SMC','265','surface','asphalt_over_concrete'], // Asphalt Over Concrete
       ['SMC','997',undefined,undefined], // Unpopulated
       ['SMC','999','surface','other'], // Other

       // SND - Sounding Category
       ['SND','1','sounding:type','drying_heights'], // Drying Heights
       ['SND','2','sounding:type','no_bottom_found'], // No Bottom Found
       ['SND','13','sounding:type','normal'], // Normal (IHO)
       ['SND','14','sounding:type','hairline'], // Hairline (IHO)
       ['SND','93','sounding:type','doubtful'], // Doubtful Sounding
       ['SND','96','sounding:type','least_depth_unknown,_safe_clearance_at_value_shown'], // Least depth unknown, safe clearance at value shown
       ['SND','997',undefined,undefined], // Unpopulated
       ['SND','999','raw:SND','other'], // Other

       // SOH - Severity of Hazard
       ['SOH','1','hazard:severity','dangerous'], // Dangerous
       ['SOH','2','hazard:severity','non_dangerous'], // Non-Dangerous
       ['SOH','997',undefined,undefined], // Unpopulated

       // SOU - Exposition of Sounding
       ['SOU','0','raw:SOU','undefined'], // Undefined
       ['SOU','1','raw:SOU','within_the_range_of_depth_of_the_surrounding_depth_area'], // Within the range of depth of the surrounding depth area
       ['SOU','2','raw:SOU','shallower_than_the_range_of_depth_of_the_surrounding_depth_area'], // Shallower than the range of depth of the surrounding depth area
       ['SOU','3','raw:SOU','deeper_than_the_range_of_depth_of_the_surrounding_depth_area'], // Deeper than the range of depth of the surrounding depth area

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

       // SPR - Slope Polygon Range
       ['SPR','0','raw:SPR','unknown'], // Unknown
       ['SPR','1','raw:SPR','<=_3'], // <= 3
       ['SPR','2','raw:SPR','>_3_and_<=_10'], // > 3 and <= 10
       ['SPR','3','raw:SPR','>_10_and_<=_15'], // > 10 and <= 15
       ['SPR','4','raw:SPR','>_15_and_<=_20'], // > 15 and <= 20
       ['SPR','5','raw:SPR','>_20_and_<=_30'], // > 20 and <= 30
       ['SPR','6','raw:SPR','>_30_and_<=_45'], // > 30 and <= 45
       ['SPR','7','raw:SPR','>_45_and_<=_60'], // > 45 and <= 60
       ['SPR','8','raw:SPR','>_60_and_<=_85'], // > 60 and <= 85
       ['SPR','9','raw:SPR','>_85'], // > 85
       ['SPR','10','raw:SPR','culturally_or_naturally_dissected_land_(0_to_>85)'], // Culturally or Naturally Dissected Land (0 to >85)
       ['SPR','997',undefined,undefined], // Unpopulated
       ['SPR','999','raw:SPR','other'], // Other

       // SPV - Special Purpose Vehicle Type
       ['SPV','1','raw:SPV','wreck_removal_associated'], // Wreck Removal Associated
       ['SPV','2','raw:SPV','crash_removal_associated'], // Crash Removal Associated
       ['SPV','3','raw:SPV','snow_removal_associated'], // Snow Removal Associated
       ['SPV','4','raw:SPV','wreck_removal_main_storage_site'], // Wreck Removal Main Storage Site
       ['SPV','5','raw:SPV','crash_removal_main_storage_site'], // Crash Removal Main Storage Site
       ['SPV','6','raw:SPV','snow_removal_main_storage_site'], // Snow Removal Main Storage Site
       ['SPV','997',undefined,undefined], // Unpopulated

       // SR1 - Slope Gradient Right (1)
       ['SR1','0','raw:SR1','unknown'], // Unknown
       ['SR1','1','raw:SR1','<=_30'], // <= 30
       ['SR1','2','raw:SR1','>_30_and_<=_45'], // > 30 and <= 45
       ['SR1','3','raw:SR1','>_45_and_<=_60'], // > 45 and <= 60
       ['SR1','4','raw:SR1','>_60'], // > 60
       ['SR1','997',undefined,undefined], // Unpopulated

       // SRD - Surface Roughness Description
       ['SRD','0','raw:SRD','unknown'], // Unknown
       ['SRD','1','raw:SRD','no_surface_roughness_effect'], // No surface roughness effect
       ['SRD','2','raw:SRD','area_of_high_landslide_potential'], // Area of high landslide potential
       ['SRD','3','raw:SRD','uncohesive_surface_material/flat'], // Uncohesive surface material/flat
       ['SRD','11','raw:SRD','surface_of_numerous_cobbles_and_boulders'], // Surface of numerous cobbles and boulders
       ['SRD','12','raw:SRD','areas_of_stony_terrain'], // Areas of stony terrain
       ['SRD','13','raw:SRD','stony_soil_with_surface_rock'], // Stony soil with surface rock
       ['SRD','14','raw:SRD','stony_soil_with_scattered_boulders'], // Stony soil with scattered boulders
       ['SRD','15','raw:SRD','stony_soil_with_numerous_boulders'], // Stony soil with numerous boulders
       ['SRD','16','raw:SRD','numerous_boulders'], // Numerous boulders
       ['SRD','17','raw:SRD','numerous_rock_outcrops'], // Numerous rock outcrops
       ['SRD','18','raw:SRD','area_of_scattered_boulders'], // Area of scattered boulders
       ['SRD','19','raw:SRD','talus_slope'], // Talus slope
       ['SRD','20','raw:SRD','boulder_field'], // Boulder Field
       ['SRD','31','raw:SRD','highly_fractured_rock_surface'], // Highly fractured rock surface
       ['SRD','32','raw:SRD','weathered_lava_flows'], // Weathered lava flows
       ['SRD','33','raw:SRD','unweathered_lava_flows'], // Unweathered lava flows
       ['SRD','34','raw:SRD','stony_soil_with_numerous_rock_outcrops'], // Stony soil with numerous rock outcrops
       ['SRD','35','raw:SRD','irregular_surface_with_deep_fractures_of_foliation'], // Irregular surface with deep fractures of foliation
       ['SRD','36','raw:SRD','rugged_terrain_with_numerous_rock_outcrops'], // Rugged terrain with numerous rock outcrops
       ['SRD','37','raw:SRD','rugged_bedrock_surface'], // Rugged bedrock surface
       ['SRD','38','raw:SRD','sand_dunes'], // Sand dunes
       ['SRD','39','raw:SRD','sand_dunes/low'], // Sand dunes/low
       ['SRD','40','raw:SRD','sand_dunes/high'], // Sand dunes/high
       ['SRD','41','raw:SRD','active_sand_dunes'], // Active sand dunes
       ['SRD','42','raw:SRD','stabilized_sand_dunes'], // Stabilized sand dunes
       ['SRD','43','raw:SRD','highly_distorted_area,_sharp_rocky_ridges'], // Highly distorted area, sharp rocky ridges
       ['SRD','51','raw:SRD','stony_soil_cut_by_numerous_gullies'], // Stony soil cut by numerous gullies
       ['SRD','52','raw:SRD','moderately_dissected_terrain'], // Moderately dissected terrain
       ['SRD','53','raw:SRD','moderately_dissected_terrain_with_scattered_rock_outcrops'], // Moderately dissected terrain with scattered rock outcrops
       ['SRD','54','raw:SRD','dissected_floodplain'], // Dissected floodplain
       ['SRD','55','raw:SRD','highly_dissected_terrain'], // Highly dissected terrain
       ['SRD','56','raw:SRD','area_with_deep_erosional_gullies'], // Area with deep erosional gullies
       ['SRD','57','raw:SRD','steep,_rugged,_dissected_terrain_with_narrow_gullies'], // Steep, rugged, dissected terrain with narrow gullies
       ['SRD','58','raw:SRD','karst_areas_of_numerous_sinkholes_and_solution_valleys'], // Karst areas of numerous sinkholes and solution valleys
       ['SRD','59','raw:SRD','karst_area_of_numerous_sinkholes'], // Karst area of numerous sinkholes
       ['SRD','60','raw:SRD','karst/hummocky_terrain_covered_with_large_conical_hills'], // Karst/hummocky terrain covered with large conical hills
       ['SRD','61','raw:SRD','karst/hummocky_terrain_covered_with_low,_broad-based_mounds'], // Karst/hummocky terrain covered with low, broad-based mounds
       ['SRD','62','raw:SRD','arroyo/wadi/wash'], // Arroyo/wadi/wash
       ['SRD','63','raw:SRD','playa/dry_lake'], // Playa/dry lake
       ['SRD','64','raw:SRD','area_of_numerous_meander_scars_and/or_oxbow_lakes'], // Area of numerous meander scars and/or oxbow lakes
       ['SRD','65','raw:SRD','solifluction_lobes_and_frost_scars'], // Solifluction lobes and frost scars
       ['SRD','66','raw:SRD','hummocky_ground,_areas_of_frost_heaving'], // Hummocky ground, areas of frost heaving
       ['SRD','67','raw:SRD','area_of_frost_polygons'], // Area of frost polygons
       ['SRD','68','raw:SRD','area_containing_sabkhas'], // Area containing sabkhas
       ['SRD','69','raw:SRD','area_of_numerous_small_lakes_and_ponds'], // Area of numerous small lakes and ponds
       ['SRD','70','raw:SRD','area_of_numerous_crevasses'], // Area of numerous crevasses
       ['SRD','81','raw:SRD','area_of_numerous_terraces'], // Area of numerous terraces
       ['SRD','82','raw:SRD','quarries'], // Quarries
       ['SRD','83','raw:SRD','strip_mines'], // Strip mines
       ['SRD','84','raw:SRD','quarry/gravel_pit'], // Quarry/gravel pit
       ['SRD','85','raw:SRD','quarry/sand_pit'], // Quarry/sand pit
       ['SRD','86','raw:SRD','mine_tailings/waste_piles'], // Mine tailings/waste piles
       ['SRD','87','raw:SRD','salt_evaporators'], // Salt evaporators
       ['SRD','88','raw:SRD','area_of_numerous_dikes'], // Area of numerous dikes
       ['SRD','89','raw:SRD','area_of_numerous_diked_fields'], // Area of numerous diked fields
       ['SRD','90','raw:SRD','area_of_numerous_fences'], // Area of numerous fences
       ['SRD','91','raw:SRD','area_of_numerous_stone_walls'], // Area of numerous stone walls
       ['SRD','92','raw:SRD','area_of_numerous_man-made_canals/drains/ditches'], // Area of numerous man-made canals/drains/ditches
       ['SRD','93','raw:SRD','area_of_numerous_terraced_fields'], // Area of numerous terraced fields
       ['SRD','94','raw:SRD','parallel_earthen_mounds_(row_crops)'], // Parallel earthen mounds (row crops)
       ['SRD','95','raw:SRD','area_of_numerous_hedgerows'], // Area of numerous hedgerows
       ['SRD','997',undefined,undefined], // Unpopulated
       ['SRD','999','raw:SRD','other'], // Other

       // SSC - Structure Shape Category
       ['SSC','0','shape','unknown'], // Unknown
       ['SSC','1','shape','barrel,_ton'], // Barrel, Ton
       ['SSC','2','shape','blimp'], // Blimp
       ['SSC','4','shape','bullet'], // Bullet
       ['SSC','6','shape','conical/peaked/nun'], // Conical/Peaked/NUN
       ['SSC','7','shape','cylindrical_(upright)/can'], // Cylindrical (Upright)/CAN
       ['SSC','10','shape','pillar/spindle'], // Pillar/Spindle
       ['SSC','12','shape','pyramid'], // Pyramid
       ['SSC','15','shape','solid/filled'], // Solid/filled
       ['SSC','16','shape','spar'], // Spar
       ['SSC','17','shape','spherical_(hemispherical)'], // Spherical (Hemispherical)
       ['SSC','18','shape','truss'], // Truss
       ['SSC','21','attraction','artificial_mountain'], // Artificial Mountain
       ['SSC','22','shape','crescent'], // Crescent
       ['SSC','23','attraction','big_wheel'], // Ferris Wheel
       ['SSC','24','shape','enclosed'], // Enclosed
       ['SSC','25','attraction','roller_coaster'], // Roller Coaster
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
       ['SSC','59','shape','telescoping_gasholder_(gasometer)'], // Telescoping Gasholder (Gasometer)
       ['SSC','60','shape','mast'], // Mast
       ['SSC','65','shape','cylindrical_with_flat_top'], // Cylindrical with flat top
       ['SSC','66','shape','cylindrical_with_domed_top'], // Cylindrical with domed top
       ['SSC','71','shape','cylindrical/peaked'], // Cylindrical/Peaked
       ['SSC','73','shape','superbuoy'], // Superbuoy
       ['SSC','74','shape','t_frame'], // T Frame
       ['SSC','76','shape','funnel'], // Funnel
       ['SSC','77','shape','arch'], // Arch
       ['SSC','79','shape','round'], // Round
       ['SSC','80','shape','rectangular'], // Rectangular
       ['SSC','83','shape','square'], // Square
       ['SSC','84','shape','irregular'], // Irregular
       ['SSC','87','shape','dome'], // Dome
       ['SSC','88','shape','spherical_with_column_support'], // Spherical with Column Support
       ['SSC','107','shape','tower'], // Tower
       ['SSC','108','shape','scanner'], // Scanner
       ['SSC','109','shape','obelisk'], // Obelisk
       ['SSC','110','shape','radome,_tower_mounted'], // Radome, Tower Mounted
       ['SSC','997',undefined,undefined], // Unpopulated
       ['SSC','999','shape','other'], // Other

       // SSR - Structure Shape of Roof
       ['SSR','0','building:roof_shape','unknown'], // Unknown
       ['SSR','6','building:roof_shape','conical/peaked/nun'], // Conical/Peaked/NUN
       ['SSR','38','building:roof_shape','curved/round_(quonset)'], // Curved/Round (Quonset)
       ['SSR','40','building:roof_shape','dome'], // Dome
       ['SSR','41','building:roof_shape','flat'], // Flat
       ['SSR','42','building:roof_shape','gable_(pitched)'], // Gable (Pitched)
       ['SSR','47','building:roof_shape','sawtooth'], // Sawtooth
       ['SSR','50','building:roof_shape','with_monitor'], // With Monitor
       ['SSR','51','building:roof_shape','with_steeple'], // With Steeple
       ['SSR','55','building:roof_shape','flat_with_monitor'], // Flat with Monitor
       ['SSR','64','building:roof_shape','gable_with_monitor'], // Gable with Monitor
       ['SSR','77','building:roof_shape','with_cupola'], // With Cupola
       ['SSR','78','building:roof_shape','with_turret'], // With Turret
       ['SSR','79','building:roof_shape','with_tower'], // With Tower
       ['SSR','80','building:roof_shape','with_minaret'], // With Minaret
       ['SSR','997',undefined,undefined], // Unpopulated
       // ['SSR','998',undefined,undefined], // Not Applicable
       ['SSR','998','building:roof_shape','not_applicable'], // Not Applicable
       ['SSR','999','building:roof_shape','other'], // Other

       // SST - Sound Signal Type
       ['SST','0','sound_signal','unknown'], // Unknown
       ['SST','1','sound_signal','bell'], // Bell
       ['SST','2','sound_signal','diaphone'], // Diaphone
       ['SST','3','sound_signal','explosive'], // Explosive Fog Signal
       ['SST','4','sound_signal','gong'], // Gong
       ['SST','6','sound_signal','horn'], // Horn
       ['SST','7','sound_signal','nautophone'], // Nautophone
       ['SST','9','sound_signal','siren'], // Siren
       ['SST','14','sound_signal','whistle'], // Whistle
       ['SST','15','sound_signal','reed'], // Reed
       ['SST','16','sound_signal','none'], // None
       ['SST','997',undefined,undefined], // Unpopulated
       ['SST','999','sound_signal','other'], // Other

       // STA - Station Type Category (Maritime)
       ['STA','0','seamark:signal_station','unknown'], // Unknown
       ['STA','1','seamark:signal_station','coast_guard'], // Coast Guard
       ['STA','2','seamark:signal_station','fireboat'], // Fireboat
       ['STA','3','seamark:signal_station','marine_police'], // Marine Police
       ['STA','4','seamark:signal_station','ice_signal'], // Ice Signal
       ['STA','5','seamark:signal_station','lifeboat/rescue'], // Lifeboat/Rescue
       ['STA','6','seamark:signal_station','port_control'], // Port Control
       ['STA','11','seamark:signal_station','pilot'], // Pilot
       ['STA','13','seamark:signal_station','signal'], // Signal
       ['STA','15','seamark:signal_station','storm_signal'], // Storm Signal
       ['STA','17','seamark:signal_station','tide_signal'], // Tide Signal
       ['STA','19','seamark:signal_station','time_signal'], // Time Signal
       ['STA','21','seamark:signal_station','weather_signal'], // Weather Signal
       ['STA','25','seamark:signal_station','semaphore'], // Semaphore
       ['STA','27','seamark:signal_station','tidal_current_signal'], // Tidal Current Signal
       ['STA','28','seamark:signal_station','traffic_signal'], // Traffic Signal
       ['STA','29','seamark:signal_station','bridge_signal'], // Bridge Signal
       ['STA','30','seamark:signal_station','lock_signal'], // Lock Signal
       ['STA','32','seamark:signal_station','international_port_signals'], // International Port Signals
       ['STA','33','seamark:signal_station','firing_practice_signal'], // Firing Practice Signal Station
       ['STA','35','seamark:signal_station','warning'], // Warning
       ['STA','36','seamark:signal_station','radar_surveillance'], // Radar Surveillance Station
       ['STA','37','seamark:signal_station','pilot_lookout'], // Pilot Lookout Station
       ['STA','38','seamark:signal_station','theodolite'], // Theodolite Station
       ['STA','39','seamark:signal_station','camera'], // Camera Station
       ['STA','40','seamark:signal_station','radar_target'], // RADAR Target
       ['STA','41','seamark:signal_station','sonar_target'], // SONAR Target
       ['STA','42','seamark:signal_station','uqc/wqc'], // UQC/WQC Station
       ['STA','43','seamark:signal_station','uews'], // UEWS
       ['STA','44','seamark:signal_station','bomis'], // BOMIS
       ['STA','45','seamark:signal_station','transit_hut'], // Transit Hut
       ['STA','46','seamark:signal_station','foracs_transducer_77_(ft77)'], // FORACS Transducer 77 (FT77)
       ['STA','47','seamark:signal_station','nmh'], // NMH
       ['STA','997',undefined,undefined], // Unpopulated
       // ['STA','998',undefined,undefined], // Not Applicable
       ['STA','998','seamark:signal_station','not_applicable'], // Not Applicable

       // STC - Source Type Code
       ['STC','0','source:type','unknown'], // Unknown
       ['STC','1','source:type','survey_source'], // Survey Source
       ['STC','2','source:type','stereo_imagery_exploitation'], // Stereo Imagery Exploitation
       ['STC','3','source:type','mono_imagery_exploitation'], // Mono Imagery Exploitation
       ['STC','4','source:type','cartographic_source'], // Cartographic Source
       ['STC','5','source:type','reported_information'], // Reported Information
       ['STC','6','source:type','multispectral'], // Multispectral
       ['STC','7','source:type','hyperspectral'], // Hyperspectral
       ['STC','8','source:type','synthetic_aperature_radar_(sar)'], // Synthetic Aperature Radar (SAR)
       ['STC','9','source:type','bathymetric_archive_data'], // Bathymetric Archive Data
       ['STC','10','source:type','bathymetric_model_data'], // Bathymetric Model Data
       ['STC','999','source:type','other'], // Other

       // STG - Soil Trafficability Group (Derived from STP)
       ['STG','0','raw:STG','unknown'], // Unknown
       ['STG','1','raw:STG','a_[gw,gp,sw,sp]'], // A [GW,GP,SW,SP]
       ['STG','2','raw:STG','b_[ch]'], // B [CH]
       ['STG','3','raw:STG','c_[gc,sc,cl]'], // C [GC,SC,CL]
       ['STG','4','raw:STG','d_[gm,sm,ml,ml-cl,mh,ol,oh]'], // D [GM,SM,ML,ML-CL,MH,OL,OH]
       ['STG','5','raw:STG','e_[pt]'], // E [PT]
       ['STG','6','raw:STG','x_[not_evaluated]'], // X [Not Evaluated]
       ['STG','997',undefined,undefined], // Unpopulated

       // STL - Seasonal Tent Location
       ['STL','0','raw:STL','unknown'], // Unknown
       ['STL','1','raw:STL','winter_location'], // Winter Location
       ['STL','2','raw:STL','summer_location'], // Summer Location
       // ['STL','998',undefined,undefined], // Not Applicable
       ['STL','998','raw:STL','not_applicable'], // Not Applicable

       // STP - Soil Type
       // ['STP','-999999',undefined,undefined], // No Information
       ['STP','1','soil:type','gw:well-graded_gravel'], // GW: Well-graded Gravel
       ['STP','2','soil:type','gp:poorly-graded_gravel'], // GP: Poorly-graded Gravel
       ['STP','3','soil:type','gm:silty_gravel_sand'], // GM: Silty Gravel Sand
       ['STP','4','soil:type','gc:clayey_gravel'], // GC: Clayey Gravel
       ['STP','5','soil:type','sw:well-graded_sand'], // SW: Well-graded Sand
       ['STP','6','soil:type','sp:poorly-graded_sand'], // SP: Poorly-graded Sand
       ['STP','7','soil:type','sm:silty_sand'], // SM: Silty Sand
       ['STP','8','soil:type','sc:clayey_sand'], // SC: Clayey Sand
       ['STP','9','soil:type','ml:silt_and_fine_sand'], // ML: Silt and Fine Sand
       ['STP','10','soil:type','cl:lean_clay'], // CL: Lean Clay
       ['STP','11','soil:type','ol:organic_silt_and_clay'], // OL: Organic Silt and Clay
       ['STP','12','soil:type','ch:fat_clay'], // CH: Fat Clay
       ['STP','13','soil:type','mh:micraceous'], // MH: Micraceous
       ['STP','14','soil:type','oh:organic_clay'], // OH: Organic Clay
       ['STP','15','soil:type','pt:peat'], // PT: Peat
       ['STP','17','soil:type','ml-cl:silt_fine_sand_and_lean_clay'], // ML-CL: Silt, Fine Sand and Lean Clay
       ['STP','18','soil:type','evaporite'], // Evaporite
       ['STP','99','soil:type','not_evaluated'], // Not Evaluated
       ['STP','999','soil:type','other'], // Other

       // STQ - Summer Tree Cover Density Code
       ['STQ','0','raw:STQ','unknown'], // Unknown
       ['STQ','1','raw:STQ','<=25'], // <=25
       ['STQ','2','raw:STQ','>25_and_<=50'], // >25 and <=50
       ['STQ','3','raw:STQ','>50_and_<=75'], // >50 and <=75
       ['STQ','4','raw:STQ','>75'], // >75
       ['STQ','997',undefined,undefined], // Unpopulated
       // ['STQ','998',undefined,undefined], // Not Applicable
       ['STQ','998','raw:STQ','not_applicable'], // Not Applicable

       // STY - Style of Text
       ['STY','1','raw:STY','kern'], // Kern
       ['STY','2','raw:STY','proportional'], // Proportional
       ['STY','3','raw:STY','constant'], // Constant

       // SUR - Survey Category
       ['SUR','0','raw:SUR','unknown_(see_qua)'], // Unknown (See QUA)
       ['SUR','1','raw:SUR','surveyed'], // Surveyed
       ['SUR','2','raw:SUR','inadequately_surveyed'], // Inadequately Surveyed
       ['SUR','3','raw:SUR','unexamined/unsurveyed'], // Unexamined/Unsurveyed
       ['SUR','997',undefined,undefined], // Unpopulated
       ['SUR','999','raw:SUR','other'], // Other

       // SV1 - Sounding Vertical Accuracy Code
       ['SV1','0','raw:SV1','o_-_unknown'], // O - Unknown
       ['SV1','1','raw:SV1','a_-_best'], // A - Best
       ['SV1','2','raw:SV1','b_-_good'], // B - Good
       ['SV1','3','raw:SV1','c_-_fair'], // C - Fair
       ['SV1','4','raw:SV1','d_-_poor'], // D - Poor
       ['SV1','997',undefined,undefined], // Unpopulated

       // SVC - Sounding Velocity
       ['SVC','0','sounding:correction','unknown'], // Unknown
       ['SVC','1','sounding:correction','echo_sounder_calibrated_at_4800_ft/sec_uncorrected'], // Echo Sounder Calibrated at 4800 ft/sec Uncorrected
       ['SVC','2','sounding:correction','echo_sounder_calibrated_at_1500_m/sec_uncorrected'], // Echo Sounder Calibrated at 1500 m/sec Uncorrected
       ['SVC','3','sounding:correction','mathews_tables_(np_139)_corrected'], // Mathews Tables (NP 139) Corrected
       ['SVC','4','sounding:correction','sound_velocity_meter_(svm)_corrected'], // Sound Velocity Meter (SVM) Corrected
       ['SVC','5','sounding:correction','corrected_by_other_means_of_calibration'], // Corrected by other means of calibration

       // SWC - Soil Wetness Condition
       ['SWC','0','raw:SWC','unknown'], // Unknown
       ['SWC','1','raw:SWC','dry'], // Dry
       ['SWC','2','raw:SWC','moist'], // Moist
       ['SWC','3','raw:SWC','wet'], // Wet
       ['SWC','4','raw:SWC','frozen/permafrost'], // Frozen/Permafrost
       ['SWC','997',undefined,undefined], // Unpopulated
       ['SWC','999','raw:SWC','other'], // Other

       // SWT - Well/Spring Feature Type
       ['SWT','0',undefined,undefined], // Unknown
       ['SWT','1','natural','geyser'], // Geyser
       ['SWT','2','natural','hot_spring'], // Hot Spring
       ['SWT','3','natural','fumarole'], // Fumarole
       ['SWT','4','natural','artesian'], // Artesian
       ['SWT','5','water','water-hole'], // Water-hole
       ['SWT','6','spring:type','walled-in'], // Walled-In Spring
       ['SWT','997',undefined,undefined], // Unpopulated

       // T01 - Refueling Units Type
       ['T01','0','raw:T01','unknown'], // Unknown
       ['T01','1','raw:T01','c_-_truck'], // C - Truck
       ['T01','2','raw:T01','m_-_manual'], // M - Manual
       ['T01','3','raw:T01','h_-_hydrant'], // H - Hydrant
       ['T01','997',undefined,undefined], // Unpopulated

       // TBT - Training/Operations Area Boundary Type
       ['TBT','0','raw:TBT','unknown'], // Unknown
       ['TBT','1','raw:TBT','training_area_(overall_boundary)'], // Training Area (Overall Boundary)
       ['TBT','2','raw:TBT','training_area_(subsection_boundary)'], // Training Area (Subsection Boundary)
       ['TBT','997',undefined,undefined], // Unpopulated
       // ['TBT','998',undefined,undefined], // Not Applicable
       ['TBT','998','raw:TBT','not_applicable'], // Not Applicable
       ['TBT','999','raw:TBT','other'], // Other

       // TDT - Air Facility Terrain or Drainage Type
       ['TDT','1','raw:TDT','river/lake'], // River/Lake
       ['TDT','2','raw:TDT','terrain'], // Terrain
       ['TDT','997',undefined,undefined], // Unpopulated
       ['TDT','999','raw:TDT','other'], // Other

       // TID - Tidal/Non-Tidal Category
       ['TID','0','tidal','unknown'], // Unknown
       ['TID','1','tidal','no'], // Non-Tidal
       ['TID','2','tidal','yes'], // Tidal/Tidal Fluctuating

       // TLA - Taxiway Lighting Availability.
       ['TLA','0','raw:TLA','unknown'], // Unknown
       ['TLA','1','raw:TLA','available'], // Available
       ['TLA','2','raw:TLA','not_available'], // Not Available
       ['TLA','997',undefined,undefined], // Unpopulated

       // TMC - Top Mark Characteristic
       ['TMC','0','seamark:topmark:shape','unknown'], // Unknown
       ['TMC','1','seamark:topmark:shape','east_mark_(2_cones_-_base_together)'], // East Mark (2 cones - base together)
       ['TMC','2','seamark:topmark:shape','isolated_danger_(2_balls)'], // Isolated Danger (2 balls)
       ['TMC','3','seamark:topmark:shape','north_mark_(2_cones_-_pointing_up)'], // North Mark (2 cones - pointing up)
       ['TMC','4','seamark:topmark:shape','port_hand_(can_or_cylinder)'], // Port Hand (can or cylinder)
       ['TMC','5','seamark:topmark:shape','safe_water_(1_ball)'], // Safe Water (1 ball)
       ['TMC','6','seamark:topmark:shape','special_(x)'], // Special (X)
       ['TMC','7','seamark:topmark:shape','starboard_hand_(1_cone_-_pointing_up)'], // Starboard Hand (1 cone - pointing up)
       ['TMC','8','seamark:topmark:shape','south_mark_(2_cones_-_pointing_down)'], // South Mark (2 cones - pointing down)
       ['TMC','9','seamark:topmark:shape','west_mark_(2_cones_-_points_together)'], // West Mark (2 cones - points together)
       ['TMC','10','seamark:topmark:shape','nun'], // Nun
       ['TMC','12','seamark:topmark:shape','ball'], // Ball
       ['TMC','13','seamark:topmark:shape','can'], // Can
       ['TMC','14','seamark:topmark:shape','st_andrews_cross'], // St. Andrews Cross
       ['TMC','15','seamark:topmark:shape','ball_over_cone'], // Ball Over Cone
       ['TMC','16','seamark:topmark:shape','cone_over_ball'], // Cone Over Ball
       ['TMC','17','seamark:topmark:shape','broom_point_up'], // Broom Point Up
       ['TMC','18','seamark:topmark:shape','perch'], // Perch
       ['TMC','19','seamark:topmark:shape','diamond'], // Diamond
       ['TMC','20','seamark:topmark:shape','broom_point_down'], // Broom Point Down
       ['TMC','21','seamark:topmark:shape','cone,_point_up'], // Cone (Point Upwards)
       ['TMC','22','seamark:topmark:shape','cone,_point_down'], // Cone (Point Downwards)
       ['TMC','23','seamark:topmark:shape','cross'], // Upright Cross
       ['TMC','24','seamark:topmark:shape','optical_reflector'], // Optical Reflector
       ['TMC','25','seamark:topmark:shape','can_(open)'], // Can (Open)
       ['TMC','26','seamark:topmark:shape','can_(filled)'], // Can (Filled)
       ['TMC','27','seamark:topmark:shape','ball_(open)'], // Ball (Open)
       ['TMC','28','seamark:topmark:shape','ball_(filled)'], // Ball (Filled)
       ['TMC','29','seamark:topmark:shape','can_over_ball_(open)'], // Can Over Ball (Open)
       ['TMC','30','seamark:topmark:shape','cross_over_ball_(filled)'], // Cross Over Ball (Filled)
       ['TMC','31','seamark:topmark:shape','diamond_over_ball_(filled)'], // Diamond Over Ball (Filled)
       ['TMC','32','seamark:topmark:shape','double_cone,_points_apart_(open)'], // Double Cone, Points Apart (Open)
       ['TMC','33','seamark:topmark:shape','none'], // None
       ['TMC','35','seamark:topmark:shape','t_shape'], // T Shape
       ['TMC','36','seamark:topmark:shape','cross_over_ball_(open)'], // Cross Over Ball (Open)
       ['TMC','37','seamark:topmark:shape','double_ball_(open)'], // Double ball (Open)
       ['TMC','40','seamark:topmark:shape','square'], // Square
       ['TMC','997',undefined,undefined], // Unpopulated
       ['TMC','999','seamark:topmark:shape','other'], // Other

       // TRA - Traversability
       ['TRA','0','foot','unknown'], // Unknown
       ['TRA','1','foot','yes'], // Traversable
       ['TRA','2','foot','no'], // Non-Traversable

       // TRE - Tree Type Category
       ['TRE','0','wood','unknown'], // Unknown
       ['TRE','1','wood','deciduous'], // Deciduous
       ['TRE','2','wood','evergreen'], // Evergreen
       ['TRE','3','wood','mixed'], // Mixed
       ['TRE','997',undefined,undefined], // Unpopulated

       // TRF - Traffic Flow
       ['TRF','0',undefined,undefined], // Undefined
       ['TRF','3','oneway','yes'], // One-way
       ['TRF','4',undefined,undefined], // Two-way

       // TS1 - Tree Spacing Range (1)
       ['TS1','0','raw:TS1','unknown'], // Unknown
       ['TS1','1','raw:TS1','>_0_and_<=_5.0'], // > 0 and <= 5.0
       ['TS1','2','raw:TS1','>_5.0_and_<=_10.0'], // > 5.0 and <= 10.0
       ['TS1','3','raw:TS1','>_10.0_and_<=_15.0'], // > 10.0 and <= 15.0
       ['TS1','4','raw:TS1','>_15.0_and_<=_20.0'], // > 15.0 and <= 20.0
       ['TS1','5','raw:TS1','>_20.0_and_<=_25.0'], // > 20.0 and <= 25.0
       ['TS1','6','raw:TS1','>_25.0_and_<=_30.0'], // > 25.0 and <= 30.0
       ['TS1','7','raw:TS1','>_30.0_and_<=_35.0'], // > 30.0 and <= 35.0
       ['TS1','8','raw:TS1','>_35.0_and_<=_40.0'], // > 35.0 and <= 40.0
       ['TS1','9','raw:TS1','>_40.0_and_<=_50.0'], // > 40.0 and <= 50.0
       ['TS1','10','raw:TS1','>_50.0_and_<=_60.0'], // > 50.0 and <= 60.0
       ['TS1','11','raw:TS1','>_60.0_and_<=_80.0'], // > 60.0 and <= 80.0
       ['TS1','12','raw:TS1','>_80.0_and_<=_100.0'], // > 80.0 and <= 100.0
       ['TS1','13','raw:TS1','>_100.0_and_<=_150.0'], // > 100.0 and <= 150.0
       ['TS1','14','raw:TS1','>_150.0'], // > 150.0
       // ['TS1','15',undefined,undefined], // Not Applicable
       ['TS1','15','raw:TS1','not_applicable'], // Not Applicable

       // TS2 - Tree Spacing Range (2)
       ['TS2','0','raw:TS2','unknown'], // Unknown
       ['TS2','1','raw:TS2','>_0_and_<=_30.0'], // > 0 and <= 30.0
       ['TS2','2','raw:TS2','>_30.0_and_<=_70.0'], // > 30.0 and <= 70.0
       ['TS2','3','raw:TS2','>_70.0_and_<=_100.0'], // > 70.0 and <= 100.0
       ['TS2','4','raw:TS2','>_100.0'], // > 100.0
       // ['TS2','5',undefined,undefined], // Not Applicable
       ['TS2','5','raw:TS2','not_applicable'], // Not Applicable
       ['TS2','997','raw:TS2',undefined,undefined], // Unpopulated

       // TS3 - Tree Spacing Range (3)
       ['TS3','0','raw:TS3','unknown'], // Unknown
       ['TS3','1','raw:TS3','>0_and_<=10.0'], // >0 and <=10.0
       ['TS3','2','raw:TS3','>10.0_and_<=20.0'], // >10.0 and <=20.0
       ['TS3','3','raw:TS3','>20.0_and_<=30.0'], // >20.0 and <=30.0
       ['TS3','4','raw:TS3','>30.0_and_<=50.0'], // >30.0 and <=50.0
       ['TS3','5','raw:TS3','>50.0_and_<=70.0'], // >50.0 and <=70.0
       ['TS3','6','raw:TS3','>70.0_and_<=100.0'], // >70.0 and <=100.0
       ['TS3','7','raw:TS3','>100.0_and_<=150.0'], // >100.0 and <=150.0
       ['TS3','8','raw:TS3','>150.0'], // >150.0
       // ['TS3','998','raw:TS3',undefined,undefined], // Not Applicable
       ['TS3','998','raw:TS3','raw:TS3','not_applicable'], // Not Applicable

       // TSH - Runway High End Threshold Lights
       ['TSH','0','raw:TSH','unknown'], // Unknown
       ['TSH','1','raw:TSH','available'], // Available
       ['TSH','2','raw:TSH','not_available'], // Not Available
       ['TSH','997',undefined,undefined], // Unpopulated

       // TSL - Runway Low End Threshold Lights
       ['TSL','0','raw:TSL','unknown'], // Unknown
       ['TSL','1','raw:TSL','available'], // Available
       ['TSL','2','raw:TSL','not_available'], // Not Available
       ['TSL','997',undefined,undefined], // Unpopulated

       // TSP - Traffic Scheme Part
       ['TSP','0','seamark:traffic_separation_scheme','unknown'], // Unknown
       ['TSP','1','seamark:traffic_separation_scheme','arrow'], // Arrow
       ['TSP','2','seamark:traffic_separation_scheme','outer_boundary'], // Outer Boundary
       ['TSP','3','seamark:traffic_separation_scheme','separation_zone_area'], // Separation Zone Area
       ['TSP','4','seamark:traffic_separation_scheme','separation_zone_line'], // Separation Zone Line
       ['TSP','5','seamark:traffic_separation_scheme','separation_zone_point'], // Separation Zone Point
       ['TSP','6','seamark:traffic_separation_scheme','inbound_area'], // Inbound Area
       ['TSP','7','seamark:traffic_separation_scheme','outbound_area'], // Outbound Area

       // TST - Transmission Line Suspension
       ['TST','0','raw:TST','unknown'], // Unknown
       ['TST','1','raw:TST','normal_suspension'], // Normal Suspension
       ['TST','2','raw:TST','catenary_(over_mountains)'], // Catenary (Over Mountains)
       ['TST','3','raw:TST','catenary_(over_water)'], // Catenary (Over Water)
       // ['TST','998',undefined,undefined], // Not Applicable
       ['TST','998','raw:TST','not_applicable'], // Not Applicable
       ['TST','999','raw:TST','other'], // Other

       // TTC - Tower Type Category
       ['TTC','0','raw:TTC','unknown'], // Unknown
       ['TTC','2','raw:TTC','observation/lookout'], // Observation/Lookout
       ['TTC','4','raw:TTC','undefined'], // Undefined
       ['TTC','12','raw:TTC','fire'], // Fire
       ['TTC','999','raw:TTC','other'], // Other

       // TUC - Transportation Use Category  ## Check This
       ['TUC','0',undefined,undefined], // Unknown
       ['TUC','1','transport:type','both_road_and_railroad'], // Both Road and Railroad
       ['TUC','2','transport:type','highway'], // Highway
       ['TUC','3','transport:type','railroad'], // Railroad
       ['TUC','4','transport:type','road'], // Road
       ['TUC','6','transport:type','street'], // Street
       ['TUC','7','highway:through_route','yes'], // Through Route
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
       // ['TUC','998',undefined,undefined], // Not Applicable
       ['TUC','998','transport:type','not_applicable'], // Not Applicable
       ['TUC','999','transport:type','other'], // Other

       // TXP - Taxiway Type
       ['TXP','0','taxiway:type','unknown'], // Unknown
       ['TXP','4','taxiway:type','dispersal'], // Dispersal
       ['TXP','5','taxiway:type','link'], // Link
       ['TXP','6','taxiway:type','loop'], // Loop
       ['TXP','7','taxiway:type','parallel'], // Parallel
       ['TXP','8','taxiway:type','perimeter'], // Perimeter
       ['TXP','997',undefined,undefined], // Unpopulated

       // UMC - Underlying Material Characteristics
       ['UMC','0','underlying:characteristic','unknown'], // Unknown
       ['UMC','1','underlying:characteristic','broken'], // Broken
       ['UMC','2','underlying:characteristic','coarse'], // Coarse
       ['UMC','3','underlying:characteristic','decayed'], // Decayed
       ['UMC','4','underlying:characteristic','fine'], // Fine, Minute Particles
       ['UMC','5','underlying:characteristic','gritty'], // Gritty
       ['UMC','6','underlying:characteristic','hard'], // Hard
       ['UMC','7','underlying:characteristic','rotten'], // Rotten
       ['UMC','8','underlying:characteristic','soft'], // Soft
       ['UMC','9','underlying:characteristic','sticky'], // Sticky
       ['UMC','10','underlying:characteristic','stiff'], // Stiff
       ['UMC','11','underlying:characteristic','streaky'], // Streaky
       ['UMC','12','underlying:characteristic','tenacious'], // Tenacious
       ['UMC','13','underlying:characteristic','uneven'], // Uneven
       ['UMC','17','underlying:characteristic','calcareous'], // Calcareous
       ['UMC','18','underlying:characteristic','flinty'], // Flinty
       ['UMC','19','underlying:characteristic','glacial'], // Glacial
       ['UMC','20','underlying:characteristic','ground'], // Ground
       ['UMC','21','underlying:characteristic','large'], // Large
       ['UMC','22','underlying:characteristic','rocky'], // Rocky
       ['UMC','23','underlying:characteristic','small'], // Small
       ['UMC','24','underlying:characteristic','speckled'], // Speckled
       ['UMC','25','underlying:characteristic','varied'], // Varied
       ['UMC','26','underlying:characteristic','volcanic'], // Volcanic
       ['UMC','27','underlying:characteristic','medium'], // Medium
       ['UMC','997',undefined,undefined], // Unpopulated

       // UNI - Units
       ['UNI','1','raw:UNI','metres'], // Metres
       ['UNI','11','raw:UNI','nautical_miles'], // Nautical Miles
       ['UNI','22','raw:UNI','feet'], // Feet
       ['UNI','23','raw:UNI','kilometers'], // Kilometers
       ['UNI','28','raw:UNI','statute_miles'], // Statute Miles
       ['UNI','29','raw:UNI','fathoms'], // Fathoms
       ['UNI','997',undefined,undefined], // Unpopulated

       // UPDATE_SOURCE - Update/Review Source Name
       ['UPDATE_SOURCE','Arc Digitized Raster Graphic (ADRG)','source:review_source:type','arc_digitized_raster_graphic_(adrg)'], // Arc Digitized Raster Graphic (ADRG)
       ['UPDATE_SOURCE','Automated Aeronautical Facilities Information File (AAFIF)','source:review_source:type','automated_aeronautical_facilities_information_file_(aafif)'], // Automated Aeronautical Facilities Information File (AAFIF)
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
       ['UPDATE_SOURCE','Foundation Feature Data/Relocatable Target Data (FFD/RTAD)','source:review_source:type','foundation_feature_data/relocatable_target_data_(ffd/rtad)'], // Foundation Feature Data/Relocatable Target Data (FFD/RTAD)
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
       ['UPDATE_SOURCE','Nominally Attributed Topographic Evaluation Map (NATE Map)','source:review_source:type','nominally_attributed_topographic_evaluation_map_(nate_map)'], // Nominally Attributed Topographic Evaluation Map (NATE Map)
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

       // USE_ - Usage
       ['USE_','0','use','unknown'], // Unknown
       ['USE_','4','use','national'], // National
       ['USE_','5','use','state'], // State
       ['USE_','6','use','private'], // Private
       ['USE_','7','use','tribal'], // Tribal
       ['USE_','8','use','military'], // Military
       ['USE_','11','use','motel/hotel'], // Motel/Hotel
       ['USE_','12','use','apartment'], // Apartment
       ['USE_','13','use','open'], // Open
       ['USE_','16','use','city'], // City
       ['USE_','17','use','advertising_billboard'], // Advertising Billboard
       ['USE_','18','use','scoreboard'], // Scoreboard
       ['USE_','20','use','closed'], // Closed
       ['USE_','21','use','restricted'], // Restricted
       ['USE_','22','use','joint_military/civilian'], // Joint Military/Civilian
       ['USE_','23','use','international'], // International
       ['USE_','25','use','federal'], // Federal
       ['USE_','32','use','insular'], // Insular
       ['USE_','33','use','provincial'], // Provincial
       ['USE_','37','use','interstate'], // Interstate
       ['USE_','41','use','industrial'], // Industrial
       ['USE_','42','use','commercial'], // Commercial
       ['USE_','43','use','institutional'], // Institutional
       ['USE_','44','use','residential'], // Residential
       ['USE_','45','use','agricultural'], // Agricultural
       ['USE_','48','use','decoy'], // Decoy
       ['USE_','49','use','civilian/public'], // Civilian/Public
       ['USE_','50','use','limited'], // Limited
       ['USE_','51','use','telegraph'], // Telegraph
       ['USE_','52','use','telephone'], // Telephone
       ['USE_','53','use','power'], // Power
       ['USE_','57','use','marine'], // Marine
       ['USE_','69','use','levee/dike'], // Levee/Dike
       ['USE_','70','use','reserve/reservation'], // Reserve/Reservation
       ['USE_','73','use','terminus/terminal'], // Terminus/Terminal
       ['USE_','74','use','low_altitude_enroute'], // Low Altitude Enroute
       ['USE_','75','use','high_altitude_enroute'], // High Altitude Enroute
       ['USE_','76','use','low_and_high_altitude_enroute'], // Low and High Altitude Enroute
       ['USE_','91','use','navaid_changeover'], // NAVAID Changeover
       ['USE_','95','use','alert_apron/hardstand'], // Alert Apron/Hardstand
       ['USE_','96','use','operational_apron/hardstand'], // Operational Apron/Hardstand
       ['USE_','97','use','hangar/apron'], // Hangar/Apron
       ['USE_','98','use','base_flight_apron'], // Base Flight Apron
       ['USE_','99','use','engine_test_pad/apron'], // Engine Test Pad/Apron
       ['USE_','100','use','transient_apron'], // Transient Apron
       ['USE_','101','use','depot_apron'], // Depot Apron
       ['USE_','102','use','stub_apron'], // Stub Apron
       ['USE_','103','use','dispersal_hardstand'], // Dispersal Hardstand
       ['USE_','104','use','pad_hardstand'], // Pad Hardstand
       ['USE_','105','use','refueling_hardstand'], // Refueling Hardstand
       ['USE_','106','use','parking_hardstand'], // Parking Hardstand
       ['USE_','107','use','engine_run-up_hardstand'], // Engine Run-up Hardstand
       ['USE_','108','use','firing-in_hardstand'], // Firing-In Hardstand
       ['USE_','109','use','compass_rose_hardstand'], // Compass Rose Hardstand
       ['USE_','110','use','maintenance_hardstand'], // Maintenance Hardstand
       ['USE_','113','use','regional'], // Regional
       ['USE_','115','use','snow_shed'], // Snow Shed
       ['USE_','116','use','rock_shed'], // Rock Shed
       ['USE_','119','use','berthing_of_vessels'], // Berthing of vessels
       ['USE_','120','use','recreational'], // Recreational
       ['USE_','128','use','mixed_urban_or_built-up_land'], // Mixed Urban or built-up Land
       ['USE_','130','use','transportation'], // Transportation
       ['USE_','132','use','container'], // Container
       ['USE_','133','use','single_point_mooring'], // Single Point Mooring
       ['USE_','134','use','utilities_and_communication'], // Utilities and Communication
       ['USE_','136','use','as_a_fill'], // As a Fill
       ['USE_','140','use','medical'], // Medical
       ['USE_','141','use','forest_preserve'], // Forest Preserve
       ['USE_','142','use','flood_control_and/or_rate_measurement'], // Flood Control and/or Rate Measurement
       ['USE_','145','use','nav_check_point'], // Nav Check Point
       ['USE_','146','use','exit_or_end'], // Exit or End
       ['USE_','147','use','entry_or_starting'], // Entry or Starting
       ['USE_','148','use','anchor_point'], // Anchor Point
       ['USE_','150','use','alternate_entry'], // Alternate Entry
       ['USE_','151','use','alternate_exit'], // Alternate Exit
       ['USE_','152','use','alternate_entry_or_exit'], // Alternate Entry or Exit
       ['USE_','153','use','turning'], // Turning
       ['USE_','154','use','compass_adjustment'], // Compass Adjustment
       ['USE_','155','use','prohibited_area'], // Prohibited Area
       ['USE_','158','use','reserved'], // Reserved
       ['USE_','161','use','roll-on_roll-off_berth_(roro)'], // Roll-On Roll-Off Berth (RORO)
       ['USE_','990',undefined,undefined], // Unpopulated
       // ['USE_','991',undefined,undefined], // Not Applicable
       ['USE_','991','use:type','not_applicable'], // Not Applicable
       ['USE_','999','use','other'], // Other

       // USP - Urban Street Pattern
       ['USP','0','raw:USP','unknown'], // Unknown
       ['USP','2','raw:USP','rectangular/grid-regular'], // Rectangular/Grid-Regular
       ['USP','3','raw:USP','rectangular/grid-irregular'], // Rectangular/Grid-Irregular
       ['USP','4','raw:USP','curvilinear_(cluster)'], // Curvilinear (cluster)
       ['USP','6','raw:USP','concentric/radial-regular'], // Concentric/Radial-Regular
       ['USP','7','raw:USP','concentric/radial-irregular'], // Concentric/Radial-Irregular
       ['USP','9','raw:USP','mixed-curvilinear_(cluster)_and_rectangular_(grid)'], // Mixed-Curvilinear (cluster) and Rectangular (grid)
       ['USP','10','raw:USP','mixed-concentric/radial_and_rectangular_(grid)'], // Mixed-Concentric/Radial and Rectangular (grid)
       ['USP','11','raw:USP','mixed-curvilinear_(cluster)_and_concentric/radial'], // Mixed-Curvilinear (cluster) and Concentric/Radial
       ['USP','13','raw:USP','linear_strip'], // Linear Strip
       ['USP','999','raw:USP','other'], // Other

       // VAC - Visual Aids Category
       ['VAC','0','raw:VAC','unknown'], // Unknown
       ['VAC','1','raw:VAC','shore_(marker)'], // Shore (Marker)
       ['VAC','2','raw:VAC','land_(marker)'], // Land (Marker)

       // VCA - Void Collection Attribute
       ['VCA','0','raw:VCA','unknown'], // Unknown
       ['VCA','1','raw:VCA','data_not_requested_by_user'], // Data Not Requested By User
       ['VCA','2','raw:VCA','area_too_rough_to_collect'], // Area Too Rough to Collect
       ['VCA','3','raw:VCA','no_available_imagery'], // No Available Imagery
       ['VCA','4','raw:VCA','different_height_threshold_within_data_block'], // Different Height Threshold Within Data Block
       ['VCA','5','raw:VCA','low_data_collection_criteria'], // Low Data Collection Criteria
       ['VCA','6','raw:VCA','no_available_map_source'], // No Available Map Source
       ['VCA','7','raw:VCA','no_suitable_imagery'], // No Suitable Imagery
       ['VCA','8','raw:VCA','data_not_required'], // Data Not Required
       ['VCA','997',undefined,undefined], // Unpopulated

       // VCD - Vertical Clearance Datum
       ['VCD','0','safe_clearance:vertical:reference','unknown'], // Unknown
       ['VCD','7','safe_clearance:vertical:reference','mean_high_water'], // Mean High Water
       ['VCD','9','safe_clearance:vertical:reference','mean_high_water_springs'], // Mean High Water Springs
       ['VCD','10','safe_clearance:vertical:reference','mean_higher_high_water'], // Mean Higher High Water
       ['VCD','15','safe_clearance:vertical:reference','mean_sea_level'], // Mean Sea Level
       ['VCD','24','safe_clearance:vertical:reference','mean_higher_high_water_springs'], // Mean Higher High Water Springs
       ['VCD','26','safe_clearance:vertical:reference','highest_normal_high_water'], // Highest Normal High Water
       ['VCD','28','safe_clearance:vertical:reference','highest_high_water'], // Highest High Water
       ['VCD','30','safe_clearance:vertical:reference','indian_spring_high_water'], // Indian Spring High Water
       ['VCD','997',undefined,undefined], // Unpopulated
       ['VCD','999','safe_clearance:vertical:reference','other'], // Other

       // VCT - Void Collection Type
       ['VCT','0','raw:VCT','unknown'], // Unknown
       ['VCT','1','raw:VCT','relief'], // Relief
       ['VCT','997',undefined,undefined], // Unpopulated
       ['VCT','999','raw:VCT','other'], // Other

       // VDC - Vertical (Sounding) Datum Category
       ['VDC','0','source:datum:sounding','unknown'], // Unknown
       ['VDC','2','source:datum:sounding','high_water'], // High Water
       ['VDC','3','source:datum:sounding','higher_high_water'], // Higher High Water
       ['VDC','4','source:datum:sounding','indian_spring_low_water'], // Indian Spring Low Water
       ['VDC','5','source:datum:sounding','low_water'], // Low Water
       ['VDC','6','source:datum:sounding','lower_low_water'], // Lower Low Water
       ['VDC','7','source:datum:sounding','mean_high_water'], // Mean High Water
       ['VDC','8','source:datum:sounding','mean_high_water_neaps'], // Mean High Water Neaps
       ['VDC','9','source:datum:sounding','mean_high_water_springs'], // Mean High Water Springs
       ['VDC','10','source:datum:sounding','mean_higher_high_water'], // Mean Higher High Water
       ['VDC','11','source:datum:sounding','mean_low_water'], // Mean Low Water
       ['VDC','12','source:datum:sounding','mean_low_water_neaps'], // Mean Low Water Neaps
       ['VDC','13','source:datum:sounding','mean_low_water_springs'], // Mean Low Water Springs
       ['VDC','14','source:datum:sounding','mean_lower_low_water'], // Mean Lower Low Water
       ['VDC','15','source:datum:sounding','mean_sea_level'], // Mean Sea Level
       ['VDC','16','source:datum:sounding','mean_tide_level'], // Mean Tide Level
       ['VDC','17','source:datum:sounding','neap_tide'], // Neap Tide
       ['VDC','18','source:datum:sounding','spring_tide'], // Spring Tide
       ['VDC','19','source:datum:sounding','mean_lower_low_water_springs'], // Mean Lower Low Water Springs
       ['VDC','20','source:datum:sounding','lowest_astronomical_tide'], // Lowest Astronomical Tide
       ['VDC','21','source:datum:sounding','chart_datum_(unspecified)'], // Chart Datum (Unspecified)
       ['VDC','22','source:datum:sounding','highest_astronomical_tide_(hat)'], // Highest Astronomical Tide (HAT)
       ['VDC','24','source:datum:sounding','mean_higher_high_water_springs'], // Mean Higher High Water Springs
       ['VDC','26','source:datum:sounding','highest_normal_high_water'], // Highest Normal High Water
       ['VDC','27','source:datum:sounding','lowest_normal_low_water'], // Lowest Normal Low Water
       ['VDC','28','source:datum:sounding','highest_high_water'], // Highest High Water
       ['VDC','30','source:datum:sounding','indian_spring_high_water'], // Indian Spring High Water
       ['VDC','90','source:datum:sounding','lowest_low_water'], // Lowest Low Water
       ['VDC','91','source:datum:sounding','lowest_low_water_springs'], // Lowest Low Water Springs
       ['VDC','92','source:datum:sounding','approximate_mean_low_water_springs'], // Approximate Mean Low Water Springs
       ['VDC','93','source:datum:sounding','low_water_springs'], // Low Water Springs
       ['VDC','94','source:datum:sounding','approximate_lowest_astronomical_tide'], // Approximate Lowest Astronomical Tide
       ['VDC','95','source:datum:sounding','nearly_lowest_low_water'], // Nearly Lowest Low Water
       ['VDC','96','source:datum:sounding','approximate_mean_low_water'], // Approximate Mean Low Water
       ['VDC','97','source:datum:sounding','approximate_mean_lower_low_water'], // Approximate Mean Lower Low Water
       ['VDC','98','source:datum:sounding','approximate_mean_sea_level'], // Approximate Mean Sea Level
       ['VDC','99','source:datum:sounding','high_water_springs'], // High Water Springs
       ['VDC','100','source:datum:sounding','equinoctial_spring_low_water'], // Equinoctial Spring Low Water
       ['VDC','101','source:datum:sounding','local_datum'], // Local Datum
       ['VDC','102','source:datum:sounding','international_great_lakes_datum_1985'], // International Great Lakes Datum 1985
       ['VDC','103','source:datum:sounding','mean_water_level'], // Mean Water Level
       ['VDC','104','source:datum:sounding','lower_low_water_large_tide'], // Lower Low Water Large Tide
       ['VDC','105','source:datum:sounding','higher_high_water_large_tide'], // Higher High Water Large Tide
       ['VDC','106','source:datum:sounding','highest_astronomical_tide'], // Highest Astronomical Tide
       ['VDC','107','source:datum:sounding','nearly_highest_high_water'], // Nearly Highest High Water
       ['VDC','997',undefined,undefined], // Unpopulated
       // ['VDC','998',undefined,undefined], // Not Applicable
       ['VDC','998','source:datum:sounding','not_applicable'], // Not Applicable
       ['VDC','999','source:datum:sounding','other'], // Other

       // VEG - Vegetation Characteristics
       ['VEG','0','vegetation:type','unknown'], // Unknown
       ['VEG','1','vegetation:type','dry_crops'], // Dry Crops
       ['VEG','4','vegetation:type','rice_paddies'], // Rice Paddies
       ['VEG','5','vegetation:type','agriculture_with_scattered_forests_or_rows_of_trees'], // Agriculture with scattered forests or rows of trees
       ['VEG','6','vegetation:type','cranberry'], // Cranberry
       ['VEG','7','vegetation:type','peat'], // Peat
       ['VEG','8','vegetation:type','pasture,_meadow,_steppe'], // Pasture, meadow, steppe
       ['VEG','9','vegetation:type','grassland_with_scattered_trees'], // Grassland with scattered trees
       ['VEG','11','vegetation:type','casuarina'], // Casuarina
       ['VEG','12','vegetation:type','coniferous'], // Coniferous
       ['VEG','16','vegetation:type','nipa'], // Nipa Palm
       ['VEG','17','vegetation:type','palm'], // Palm
       ['VEG','18','vegetation:type','filao'], // Filao
       ['VEG','19','vegetation:type','mangrove'], // Mangrove
       ['VEG','24','vegetation:type','deciduous'], // Deciduous
       ['VEG','25','vegetation:type','evergreen'], // Evergreen
       ['VEG','29','vegetation:type','eucalyptus'], // Eucalyptus
       ['VEG','32','vegetation:type','walnut'], // Walnut
       ['VEG','36','vegetation:type','chestnut'], // Chestnut
       ['VEG','38','vegetation:type','cypress'], // Cypress
       ['VEG','39','vegetation:type','peach'], // Peach
       ['VEG','40','vegetation:type','apple'], // Apple
       ['VEG','41','vegetation:type','carob'], // Carob
       ['VEG','42','vegetation:type','almond'], // Almond
       ['VEG','43','vegetation:type','citrus'], // Citrus
       ['VEG','50','vegetation:type','mixed_trees'], // Mixed Trees
       ['VEG','52','vegetation:type','forest_clearing'], // Forest Clearing
       ['VEG','56','vegetation:type','without_trees'], // Without Trees
       ['VEG','57','vegetation:type','agriculture_with_scattered_trees_or_rows_of_trees'], // Agriculture with scattered trees or rows of trees
       ['VEG','997',undefined,undefined], // Unpopulated
       ['VEG','999','vegetation:type','other'], // Other

       // VGT - Volcanic Geologic Type
       ['VGT','1','raw:VGT','volcano'], // Volcano
       ['VGT','2','raw:VGT','cinder_cone'], // Cinder Cone
       ['VGT','3','raw:VGT','shield'], // Shield
       ['VGT','4','raw:VGT','caldera'], // Caldera
       ['VGT','5','raw:VGT','composite'], // Composite

       // VH1 - Predominant Vegetation Height Range (1)
       ['VH1','0','raw:VH1','unknown'], // Unknown
       ['VH1','1','raw:VH1','<=_2'], // <= 2
       ['VH1','2','raw:VH1','>_2_and_<=_5'], // > 2 and <= 5
       ['VH1','3','raw:VH1','>_5_and_<=_10'], // > 5 and <= 10
       ['VH1','4','raw:VH1','>_10_and_<=_15'], // > 10 and <= 15
       ['VH1','5','raw:VH1','>_15_and_<=_20'], // > 15 and <= 20
       ['VH1','6','raw:VH1','>_20_and_<=_30'], // > 20 and <= 30
       ['VH1','7','raw:VH1','>_30_and_<=_40'], // > 30 and <= 40
       ['VH1','8','raw:VH1','>_40'], // > 40
       ['VH1','9','raw:VH1','>_20_and_<=_25'], // > 20 and <= 25
       ['VH1','10','raw:VH1','>_25_and_<=_30'], // > 25 and <= 30
       ['VH1','11','raw:VH1','>_30_and_<=_35'], // > 30 and <= 35
       ['VH1','12','raw:VH1','>_35'], // > 35
       // ['VH1','13',undefined,undefined], // Not Applicable
       ['VH1','13','raw:VH1','not_applicable'], // Not Applicable

       // VH3 - Predominant Vegetation Height Range (3)
       ['VH3','0','raw:VH3','unknown'], // Unknown
       ['VH3','1','raw:VH3','>0_and_<=5'], // >0 and <=5
       ['VH3','2','raw:VH3','>5_and_<=10'], // >5 and <=10
       ['VH3','3','raw:VH3','>10_and_<=20'], // >10 and <=20
       ['VH3','4','raw:VH3','>20_and_<=40'], // >20 and <=40
       ['VH3','5','raw:VH3','>40'], // >40
       ['VH3','997',undefined,undefined], // Unpopulated
       // ['VH3','998',undefined,undefined], // Not Applicable
       ['VH3','998','raw:VH3','not_applicable'], // Not Applicable

       // VOL - Volume
       ['VOL','1','raw:VOL','empty'], // Empty
       ['VOL','2','raw:VOL','light'], // Light
       ['VOL','3','raw:VOL','moderate'], // Moderate
       ['VOL','4','raw:VOL','heavy'], // Heavy

       // VPH - Runway High End PAPI Lights
       ['VPH','0','raw:VPH','unknown'], // Unknown
       ['VPH','1','raw:VPH','available'], // Available
       ['VPH','2','raw:VPH','not_available'], // Not Available
       ['VPH','997',undefined,undefined], // Unpopulated

       // VPL - Runway Low End PAPI Lights
       ['VPL','0','raw:VPL','unknown'], // Unknown
       ['VPL','1','raw:VPL','available'], // Available
       ['VPL','2','raw:VPL','not_available'], // Not Available
       ['VPL','997',undefined,undefined], // Unpopulated

       // VRC - Vegetation Roughness Category
       ['VRC','1','raw:VRC','0.00_100%_reduction'], // 0.00 100% reduction
       ['VRC','2','raw:VRC','0.05'], // 0.05
       ['VRC','3','raw:VRC','0.1'], // 0.1
       ['VRC','4','raw:VRC','0.15'], // 0.15
       ['VRC','5','raw:VRC','0.2'], // 0.2
       ['VRC','6','raw:VRC','0.25'], // 0.25
       ['VRC','7','raw:VRC','0.3'], // 0.3
       ['VRC','8','raw:VRC','0.35'], // 0.35
       ['VRC','9','raw:VRC','0.4'], // 0.4
       ['VRC','10','raw:VRC','0.45'], // 0.45
       ['VRC','11','raw:VRC','0.50_50%_reduction'], // 0.50 50% reduction
       ['VRC','12','raw:VRC','0.55'], // 0.55
       ['VRC','13','raw:VRC','0.6'], // 0.6
       ['VRC','14','raw:VRC','0.65'], // 0.65
       ['VRC','15','raw:VRC','0.7'], // 0.7
       ['VRC','16','raw:VRC','0.75'], // 0.75
       ['VRC','17','raw:VRC','0.8'], // 0.8
       ['VRC','18','raw:VRC','0.85'], // 0.85
       ['VRC','19','raw:VRC','0.9'], // 0.9
       ['VRC','20','raw:VRC','0.95'], // 0.95
       ['VRC','21','raw:VRC','1.00_0%_reduction'], // 1.00 0% reduction
       ['VRC','22','raw:VRC','not_evaluated_area_where_development_has_precluded_evaluation_of_soil'], // Not evaluated area where development has precluded evaluation of soil

       // VRR - Vertical Reference Category
       ['VRR','0',undefined,undefined], // Unknown
       ['VRR','1','hydrographic_vertical_position','always_dry'], // Above Surface/Does Not Cover (at High Water)
       ['VRR','2','hydrographic_vertical_position','awash_at_chart_datum'], // Awash at Sounding Datum
       ['VRR','4','hydrographic_vertical_position','always_submerged'], // Below Surface/Submerged
       ['VRR','8','hydrographic_vertical_position','covers_and_uncovers'], // Covers and Uncovers
       // ['VRR','9',undefined,undefined], // Not Applicable
       ['VRR','9','hydrographic_vertical_position','not_applicable'], // Not Applicable
       ['VRR','997',undefined,undefined], // Unpopulated

       // VSH - Runway High End VASI Lights
       ['VSH','0','raw:VSH','unknown'], // Unknown
       ['VSH','1','raw:VSH','available'], // Available
       ['VSH','2','raw:VSH','not_available'], // Not Available
       ['VSH','997',undefined,undefined], // Unpopulated

       // VSL - Runway Low End VASI Lights
       ['VSL','0','raw:VSL','unknown'], // Unknown
       ['VSL','1','raw:VSL','available'], // Available
       ['VSL','2','raw:VSL','not_available'], // Not Available
       ['VSL','997',undefined,undefined], // Unpopulated

       // WFT - Well Feature Type
       ['WFT','0','water_well:type','unknown'], // Unknown
       ['WFT','2','water_well:type','walled-in'], // Walled-in
       ['WFT','3','water_well:type','artesian'], // Artesian Well
       ['WFT','5','water_well:type','dug_or_drilled'], // Dug or Drilled Well
       ['WFT','6','water_well:type','dug'], // Dug
       ['WFT','7','water_well:type','drilled'], // Drilled
       ['WFT','997',undefined,undefined], // Unpopulated
       ['WFT','999','water_well:type','other'], // Other

       // WKT - Wreck Type
       ['WKT','0','raw:WKT','unknown'], // Unknown
       ['WKT','1','raw:WKT','anomalies'], // Anomalies
       ['WKT','2','raw:WKT','aircraft'], // Aircraft
       ['WKT','3','raw:WKT','auxiliary'], // Auxiliary
       ['WKT','4','raw:WKT','battleship'], // Battleship
       ['WKT','5','raw:WKT','barge'], // Barge
       ['WKT','6','raw:WKT','buoy'], // Buoy
       ['WKT','7','raw:WKT','caisson'], // Caisson
       ['WKT','8','raw:WKT','aircraft_carrier'], // Aircraft Carrier
       ['WKT','9','raw:WKT','cargo'], // Cargo
       ['WKT','10','raw:WKT','subchaser'], // Subchaser
       ['WKT','11','raw:WKT','coaster'], // Coaster
       ['WKT','12','raw:WKT','cruiser'], // Cruiser
       ['WKT','13','raw:WKT','ship_debris_(plates,_misc._metal)'], // Ship Debris (plates, misc. metal)
       ['WKT','14','raw:WKT','destroyer,_destroyer_escort,_corvette'], // Destroyer, Destroyer Escort, Corvette
       ['WKT','15','raw:WKT','dredge'], // Dredge
       ['WKT','16','raw:WKT','drill_vessel_or_drill_rig'], // Drill Vessel or Drill Rig
       ['WKT','17','raw:WKT','explosives'], // Explosives
       ['WKT','18','raw:WKT','fishing_vessel'], // Fishing Vessel
       ['WKT','19','raw:WKT','fishing_reef'], // Fishing Reef
       ['WKT','20','raw:WKT','ferry'], // Ferry
       ['WKT','21','raw:WKT','frigate'], // Frigate
       ['WKT','22','raw:WKT','gunboat'], // Gunboat
       ['WKT','23','raw:WKT','hospital_ship'], // Hospital Ship
       ['WKT','24','raw:WKT','hydrographic_survey,_air_cushion,_hydrofoil'], // Hydrographic Survey, Air Cushion, Hydrofoil
       ['WKT','25','raw:WKT','junk'], // Junk
       ['WKT','26','raw:WKT','lash_vessel'], // Lash Vessel
       ['WKT','27','raw:WKT','landing_craft,_infantry_(lci)'], // Landing Craft, Infantry (LCI)
       ['WKT','28','raw:WKT','gas_carrier,_natural_petroleum'], // Gas Carrier, Natural Petroleum
       ['WKT','29','raw:WKT','landing_ship,_infantry_(lsi)'], // Landing Ship, Infantry (LSI)
       ['WKT','30','raw:WKT','landing_ship,_tank_(lst)'], // Landing Ship, Tank (LST)
       ['WKT','31','raw:WKT','lightship'], // Lightship
       ['WKT','32','raw:WKT','minesweeper,_minelayer'], // Minesweeper, Minelayer
       ['WKT','33','raw:WKT','net_tender'], // Net Tender
       ['WKT','34','raw:WKT','bulk_carrier'], // Bulk Carrier
       ['WKT','35','raw:WKT','obstruction'], // Obstruction
       ['WKT','36','raw:WKT','passenger_cargo'], // Passenger Cargo
       ['WKT','37','raw:WKT','patrol_boat'], // Patrol Boat
       ['WKT','38','raw:WKT','pinnacle_rock'], // Pinnacle Rock
       ['WKT','39','raw:WKT','roll_on_-_roll_off'], // Roll On - Roll Off
       ['WKT','40','raw:WKT','sailing_ship'], // Sailing Ship
       ['WKT','41','raw:WKT','sea_bee,_lash_barge'], // Sea Bee, Lash Barge
       ['WKT','42','raw:WKT','submarine_float'], // Submarine Float
       ['WKT','43','raw:WKT','submarine'], // Submarine
       ['WKT','44','raw:WKT','survey_vessel'], // Survey Vessel
       ['WKT','45','raw:WKT','tanker'], // Tanker
       ['WKT','46','raw:WKT','tender,_submarine,_airplane,_oil_rig'], // Tender, Submarine, Airplane, Oil Rig
       ['WKT','47','raw:WKT','target'], // Target
       ['WKT','48','raw:WKT','torpedo_boat'], // Torpedo Boat
       ['WKT','49','raw:WKT','transport'], // Transport
       ['WKT','50','raw:WKT','tug'], // Tug
       ['WKT','51','raw:WKT','trawler'], // Trawler
       ['WKT','52','raw:WKT','very_large_container'], // Very Large Container
       ['WKT','53','raw:WKT','well_head'], // Well Head
       ['WKT','54','raw:WKT','yacht'], // Yacht
       // ['WKT','998',undefined,undefined], // Not Applicable
       ['WKT','998','raw:WKT','not_applicable'], // Not Applicable
       ['WKT','999','raw:WKT','other'], // Other

       // WPC - Work in Progress Category
       ['WPC','0','work:type','unknown'], // Unknown
       ['WPC','1','work:type','land_reclamation'], // Land Reclamation
       ['WPC','2','work:type','construction_of_structures'], // Construction of Structures
       ['WPC','997',undefined,undefined], // Unpopulated

       // WPT - Waypoint Description Code
       ['WPT','0','raw:WPT','unknown'], // Unknown
       ['WPT','5','raw:WPT','non-essential_waypoint'], // Non-essential waypoint
       ['WPT','10','raw:WPT','atc_compulsory_waypoint'], // ATC compulsory waypoint

       // WSR - Source of report
       ['WSR','0','raw:WSR','unknown'], // Unknown
       ['WSR','8','raw:WSR','report_sonar_verified'], // Report SONAR verified
       ['WSR','9','raw:WSR','report_mad_verified'], // Report MAD verified
       ['WSR','10','raw:WSR','japanese_naval_and_merchant_losses_during_ww_ii_by_all_causes_navexos_p-468'], // Japanese Naval and Merchant Losses during WW II By All Causes NAVEXOS P-468
       ['WSR','11','raw:WSR','us_losses_-_director,_fleet_operations'], // US Losses - Director, Fleet Operations
       ['WSR','12','raw:WSR','h.o._files'], // H.O. Files
       ['WSR','13','raw:WSR','comnavpac'], // COMNAVPAC
       ['WSR','14','raw:WSR','comnavfe'], // COMNAVFE
       ['WSR','15','raw:WSR','h.o._chart_reports_and_charts'], // H.O. Chart Reports and Charts
       ['WSR','16','raw:WSR','h.o._notice_to_mariners'], // H.O. Notice to Mariners
       ['WSR','17','raw:WSR','lloyds_and_marine_underwriters_reports'], // Lloyds and Marine Underwriters Reports
       ['WSR','18','raw:WSR','the_imperial_japanese_navy_in_ww_ii'], // The Imperial Japanese Navy in WW II
       ['WSR','19','raw:WSR','u.s._navy_at_war_1941-1945'], // U.S. Navy at War 1941-1945
       ['WSR','20','raw:WSR','german,_japanese,_and_italian_submarine_losses_in_ww_ii'], // German, Japanese, and Italian Submarine Losses in WW II
       ['WSR','21','raw:WSR','swedish_merchant_losses_1914-1920'], // Swedish Merchant Losses 1914-1920
       ['WSR','22','raw:WSR','h.o._wreck_information_list_and_supplement_dated_10_mar_1945_and_30_sep_1946'], // H.O. Wreck Information List and Supplement dated 10 MAR 1945 and 30 SEP 1946
       ['WSR','23','raw:WSR','american_ship_casualties_of_ww_i'], // American Ship Casualties of WW I
       ['WSR','24','raw:WSR','u.s._coast_guard_and_geodetic_survey_records'], // U.S. Coast Guard and Geodetic Survey Records
       ['WSR','25','raw:WSR','tenth_fleet_records_(op-374)'], // Tenth Fleet Records (OP-374)
       ['WSR','26','raw:WSR','naval_losses_of_all_nations_9/3/39_-_8/15/45'], // Naval Losses of All Nations 9/3/39 - 8/15/45
       ['WSR','27','raw:WSR','italian_naval_and_merchant_losses,_ww_ii'], // Italian Naval and Merchant Losses, WW II
       ['WSR','28','raw:WSR','list_of_danish_war_losses_1914_-_1918'], // List of Danish War Losses 1914 - 1918
       ['WSR','29','raw:WSR','british_merchant_vessels_captured_or_destroyed_by_enemy_action_1914_-_1918'], // British Merchant Vessels Captured or Destroyed by Enemy Action 1914 - 1918
       ['WSR','30','raw:WSR','abstracts_of_losses_1914_-_1918'], // Abstracts of Losses 1914 - 1918
       ['WSR','31','raw:WSR','british_and_foreign_merchant_ships_lost_during_ww_ii'], // British and Foreign Merchant Ships Lost During WW II
       ['WSR','32','raw:WSR','a_list_of_neutral_ships_sunk_by_germany'], // A List of Neutral Ships Sunk by Germany
       ['WSR','33','raw:WSR','greek_losses_ww_ii'], // Greek Losses WW II
       ['WSR','34','raw:WSR','italian_naval_losses_-_ww_i'], // Italian Naval Losses - WW I
       ['WSR','35','raw:WSR','spanish_merchant_losses_-_ww_i'], // Spanish Merchant Losses - WW I
       ['WSR','36','raw:WSR','netherlands_merchant_losses_-_ww_i'], // Netherlands Merchant Losses - WW I
       ['WSR','37','raw:WSR','italian_merchant_losses_-_ww_i'], // Italian Merchant Losses - WW I
       ['WSR','38','raw:WSR','maritime_declarations_for_norwegian_ships_for_war_losses_1914_-_1918'], // Maritime Declarations for Norwegian Ships for War Losses 1914 - 1918
       ['WSR','39','raw:WSR','reports_of_known_wrecks_by_u.s._coast_guard_districts_1950'], // Reports of Known Wrecks by U.S. Coast Guard Districts 1950
       ['WSR','40','raw:WSR','france_-_naval_and_merchant_losses_during_ww_i'], // France - Naval and Merchant Losses during WW I
       ['WSR','41','raw:WSR','sonar_reports_and_mad_reports,_u.s._navy'], // Sonar Reports and MAD Reports, U.S. Navy
       ['WSR','42','raw:WSR','maritime_commission_also_oni_list_ww_i'], // Maritime Commission also ONI List WW I
       ['WSR','43','raw:WSR','smaling_soforklaringer_-_danish_ship_war_losses_1914_-_1918'], // Smaling Soforklaringer - Danish Ship War Losses 1914 - 1918
       ['WSR','44','raw:WSR','merchant_vessels_of_the_u.s._lost_(commerce_department)'], // Merchant Vessels of the U.S. Lost (Commerce Department)
       ['WSR','45','raw:WSR','naval_chronology_ww_ii'], // Naval Chronology WW II
       ['WSR','46','raw:WSR','ship_visit_reports'], // Ship Visit Reports
       ['WSR','47','raw:WSR','spanish_hydrographic_office'], // Spanish Hydrographic Office
       ['WSR','48','raw:WSR','british_admiralty_wreck_charts_1921'], // British Admiralty Wreck Charts 1921
       ['WSR','49','raw:WSR','swedish_board_of_shipping_and_navigation,_hydrographic_dept.'], // Swedish Board of Shipping and Navigation, Hydrographic Dept.
       ['WSR','50','raw:WSR','british_admiralty_wreck_charts,_1961'], // British Admiralty Wreck Charts, 1961
       ['WSR','51','raw:WSR','portugal_hydrographic_office'], // Portugal Hydrographic Office
       ['WSR','52','raw:WSR','italian_navy_survey'], // Italian Navy Survey
       ['WSR','53','raw:WSR','state_of_shipping_casualties_(resulting_in_total_loss_in_st._lawrence_river_and_gulf,_on_the_atlantic_coast_from_1896_up_to_date)'], // State of Shipping Casualties (Resulting in total loss in St. Lawrence River and Gulf, on the Atlantic Coast from 1896 up to date)
       ['WSR','54','raw:WSR','italian_naval_surveys_charts_and_lists'], // Italian Naval Surveys Charts and Lists
       ['WSR','55','raw:WSR','oni'], // ONI
       ['WSR','57','raw:WSR','r.c.n._wreck_list_(rcn_pub._272)'], // R.C.N. Wreck List (RCN Pub. 272)
       ['WSR','58','raw:WSR','dutch_wreck_list'], // Dutch Wreck List
       ['WSR','59','raw:WSR','north_sea_fishing_charts'], // North Sea Fishing Charts
       ['WSR','60','raw:WSR','photographs_of_pinnacles_and_miscellaneous_metals'], // Photographs of Pinnacles and Miscellaneous Metals
       ['WSR','61','raw:WSR','coast_and_geodetic_survey_(national_ocean_survey)_charts'], // Coast and Geodetic Survey (National Ocean Survey) Charts
       ['WSR','62','raw:WSR','radio_navigation_warning'], // Radio Navigation Warning
       ['WSR','63','raw:WSR','oman_national_hydrographic_office'], // OMAN National Hydrographic Office
       // ['WSR','998',undefined,undefined], // Not Applicable
       ['WSR','998','raw:WSR','not_applicable'], // Not Applicable
       ['WSR','999','raw:WSR','other'], // Other

       // WTC - Weather Type Category
       ['WTC','0',undefined,undefined], // Unknown
       ['WTC','1','seasonal','no'], // All Weather
       ['WTC','2','seasonal','fair'], // Fair/Dry Weather
       ['WTC','3','seasonal','winter'], // Winter Only
       ['WTC','4','seasonal','no'], // All Weather (Limited Traffic Due to Weather)
       ['WTC','997',undefined,undefined], // Unpopulated
       ['WTC','999','seasonal','other'], // Other

       // WTI - Wall Type Identifier
       ['WTI','0','raw:WTI','unknown'], // Unknown
       ['WTI','1','raw:WTI','standing'], // Standing
       ['WTI','2','raw:WTI','retaining'], // Retaining
       ['WTI','999','raw:WTI','other'], // Other

       // WTR - Winter Tree Cover Density Code
       ['WTR','0','raw:WTR','unknown'], // Unknown
       ['WTR','1','raw:WTR','<=_25'], // <= 25
       ['WTR','2','raw:WTR','>_25_and_<=_50'], // > 25 and <= 50
       ['WTR','3','raw:WTR','>_50_and_<=_75'], // > 50 and <= 75
       ['WTR','4','raw:WTR','>_75'], // > 75
       // ['WTR','5',undefined,undefined], // Not Applicable
       ['WTR','5','raw:ETR','not_applicable'], // Not Applicable
       ['WTR','997',undefined,undefined], // Unpopulated

       // WV1 - Water Velocity Average 1
       ['WV1','0','raw:WV1','unknown'], // Unknown
       ['WV1','1','raw:WV1','<=_1.5'], // <= 1.5
       ['WV1','2','raw:WV1','>_1.5'], // > 1.5
       ['WV1','997',undefined,undefined], // Unpopulated

       // YDH - Water Depth Mean (Seasonal High Water)
       ['YDH','0','raw:YDH','unknown'], // Unknown
       ['YDH','1','raw:YDH','<=0.8'], // <=0.8
       ['YDH','2','raw:YDH','>0.8_and_<=1.6'], // >0.8 and <=1.6
       ['YDH','3','raw:YDH','>1.6_and_<=2.4'], // >1.6 and <=2.4
       ['YDH','4','raw:YDH','>2.4'], // >2.4

       // YDL - Water Depth Mean (Seasonal Low Water)
       ['YDL','0','raw:YDL','unknown'], // Unknown
       ['YDL','1','raw:YDL','<=0.8'], // <=0.8
       ['YDL','2','raw:YDL','>0.8_and_<=1.6'], // >0.8 and <=1.6
       ['YDL','3','raw:YDL','>1.6_and_<=2.4'], // >1.6 and <=2.4
       ['YDL','4','raw:YDL','>2.4'], // >2.4

       // YSU - Service Branch
       ['YSU','0','raw:YSU','unknown'], // Unknown
       ['YSU','1','raw:YSU','air_force'], // Air Force
       ['YSU','2','raw:YSU','army'], // Army
       ['YSU','3','raw:YSU','coast_guard'], // Coast Guard
       ['YSU','4','raw:YSU','marines'], // Marines
       ['YSU','5','raw:YSU','navy'], // Navy
       ['YSU','7','raw:YSU','joint'], // Joint
       // ['YSU','998',undefined,undefined], // Not Applicable
       ['YSU','998','raw:YSU','not_applicable'], // Not Applicable
       ['YSU','999','raw:YSU','other'], // Other

       // YVH - Water Velocity Mean (Seasonal High Water)
       ['YVH','0','raw:YVH','unknown'], // Unknown
       ['YVH','1','raw:YVH','<=0.5'], // <=0.5
       ['YVH','2','raw:YVH','>0.5_and_<=1.5'], // >0.5 and <=1.5
       ['YVH','3','raw:YVH','>1.5_and_<=2.5'], // >1.5 and <=2.5
       ['YVH','4','raw:YVH','>2.5'], // >2.5

       // YVL - Water Velocity Mean (Seasonal Low Water)
       ['YVL','0','raw:YVL','unknown'], // Unknown
       ['YVL','1','raw:YVL','<=0.5'], // <=0.5
       ['YVL','2','raw:YVL','>0.5_and_<=1.5'], // >0.5 and <=1.5
       ['YVL','3','raw:YVL','>1.5_and_<=2.5'], // >1.5 and <=2.5
       ['YVL','4','raw:YVL','>2.5'], // >2.5

       // YWQ - Water Quality Attribute
       ['YWQ','0','raw:YWQ','unknown'], // Unknown
       ['YWQ','1','raw:YWQ','potable'], // Potable
       ['YWQ','2','raw:YWQ','treatable'], // Treatable
       ['YWQ','3','raw:YWQ','contaminated'], // Contaminated

       // YWT - Depth to Water Table
       ['YWT','0','raw:YWT','unknown'], // Unknown
       ['YWT','1','raw:YWT','>0_and_<=0.3'], // >0 and <=0.3
       ['YWT','2','raw:YWT','>0.3_and_<=1.2'], // >0.3 and <=1.2
       ['YWT','3','raw:YWT','>1.2'], // >1.2
       ['YWT','4','raw:YWT','at_ground_surface'], // At ground surface

       // Z_VALUE_TYPE - Z-Coordinate Type
       ['Z_VALUE_TYPE','2D data','source:vertical_source:type','2d_data'], // 2D data
       ['Z_VALUE_TYPE','DTED1 derived','source:vertical_source:type','dted1'], // DTED1 derived
       ['Z_VALUE_TYPE','DTED2 derived','source:vertical_source:type','dted2'], // DTED2 derived
       ['Z_VALUE_TYPE','Other','source:vertical_source:type','other'], // Other
       ['Z_VALUE_TYPE','Reflective surface derived','source:vertical_source:type','reflective_surface'], // Reflective surface derived
       ['Z_VALUE_TYPE','Stereo derived','source:vertical_source:type','stereoscopic_imagery'], // Stereo derived
       ['Z_VALUE_TYPE','TIN derived','source:vertical_source:type','tin'], // TIN derived
       ['Z_VALUE_TYPE','Unknown','source:vertical_source:type','unknown'], // Unknown
       ], // End one2one

    // ##### End of One2One Rules #####

} // End of gifd.rules
