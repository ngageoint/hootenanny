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
    fcodeCommon

    FCODE translations that are common for all products. Product specific FCODES go in the individual
    product "rules" scripts.
    
    These can be used for BOTH import and export so the values need to be unique. Non-unique values go
    in the individual product rules scripts.
*/


fcodeCommon = {
    // The fcodeXXX rules are for finding/setting an FCODE
    // Common rules: import and export with "unique" values
    one2one : [
      ['F_CODE','AA010','landuse','surface_mining'], // Extraction Mine
      ['F_CODE','AA011','natural','shear_wall'], // Shear Wall
      ['F_CODE','AA020','mine:superstructure','yes'], // Mine Shaft Superstructure
      ['F_CODE','AA040','man_made','rig'], // Rig/Superstructure
      ['F_CODE','AA045','man_made','borehole'], // Borehole
      ['F_CODE','AA052','industrial','hydrocarbons_field'], // Hydrocarbons Field
      ['F_CODE','AA054','man_made','well'], // Non-water Well
      ['F_CODE','AB000','landuse','landfill'], // Disposal Site / Waste Pile
      ['F_CODE','AB010','amenity','recycling'], // Wrecking Yard/Scrap Yard, Recycling Site
      ['F_CODE','AB021','man_made','liquid_diffuser'], // Liquid Diffuser
      ['F_CODE','AB030','amenity','waste_processing'], // Waste Processing
      ['F_CODE','AB040','landuse','aeration_basin'], // Aeration Basin
      ['F_CODE','AB507','man_made','waste_heap'], // Waste Heap
      ['F_CODE','AC010','man_made','blast_furnace'], // Blast-furnace
      ['F_CODE','AC020','man_made','catalytic_cracker'], // Catalytic Cracker
      ['F_CODE','AC030','water','settling_basin'], // Settling Basin/Sludge Pond, Settling Pond
      ['F_CODE','AC040','industrial','refinery'], // Hydrocarbon Products Facility, Oil/Gas Facilities
      ['F_CODE','AC060','man_made','furnace'], // Industrial Furnace
      ['F_CODE','AC507','man_made','wastewater_plant'], // Sewage Treatment Plant
      ['F_CODE','AD010','power','plant'], // Electric Power Station, Power Plant
      ['F_CODE','AD020','generator:source','solar'], // Solar Panel
      ['F_CODE','AD025','landuse','solar_farm'], // Solar Farm
      ['F_CODE','AD030','power','substation'], // Power Substation, Substation/Transformer Yard
      ['F_CODE','AD041','building','nuclear_reactor_containment'], // Nuclear Reactor Containment
      ['F_CODE','AD050','amenity','heating'], // Heating Facility
      ['F_CODE','AD055','amenity','cooling'], // Cooling Facility
      ['F_CODE','AD060','landuse','wind_farm'], // Wind Farm
      ['F_CODE','AF010','man_made','chimney'], // Chimney/Smokestack
      ['F_CODE','AF020','man_made','goods_conveyor'], // Conveyor
      ['F_CODE','AF021','conveyer:type','bucket:'], // Bucket Elevator
      ['F_CODE','AF030','tower:type','cooling'], // Cooling Tower
      ['F_CODE','AF040','man_made','crane'], // Crane
      ['F_CODE','AF050','man_made','excavating_machine'], // Excavating Machine
      ['F_CODE','AF060','man_made','engine_test_cell'], // Engine Test Cell
      ['F_CODE','AF070','man_made','flare'], // Flare Pipe
      ['F_CODE','AF080','man_made','hopper'], // Hopper
      ['F_CODE','AG030','shop','mall'], // Shopping Complex
      ['F_CODE','AG040','landuse','commercial'], // Office Park
      ['F_CODE','AG050','advertising','billboard'], // Billboard
      ['F_CODE','AH025','military','trench'], // Engineered Earthwork
      ['F_CODE','AH055','military','bunker'], // Fortified Building
      ['F_CODE','AH060','building','underground_shelter'], // Underground Bunker
      ['F_CODE','AH070','barrier','check_point'], // Checkpoint
      ['F_CODE','AI020','tourism','caravan_site'], // Caravan Park
      ['F_CODE','AI021','amenity','trailer_park'], // Mobile Home Park / Manufactured Home Park
      ['F_CODE','AI030','camp','yes'], // Camp
      ['F_CODE','AJ030','landuse','holding_pen'], // Feedlot/Stockyard, Holding Pen
      ['F_CODE','AJ050','man_made','windmill'], // Windmill
      ['F_CODE','AJ051','generator:source','wind'], // Windmotor, Wind Turbine
      ['F_CODE','AJ055','man_made','water_mill'], // Water Mill
      ['F_CODE','AJ080','building','stable'], // Stable
      ['F_CODE','AJ085','building','barn'], // Barn
      ['F_CODE','AJ110','building','greenhouse'], // Greenhouse
      ['F_CODE','AK020','tourism','theme_park_attraction'], // Amusement Park Attraction
      ['F_CODE','AK030','tourism','theme_park'], // Amusement Park
      ['F_CODE','AK040','leisure','pitch'], // Athletic Field, Sports Ground
      // ['F_CODE','AK050','sport','tennis'], // Tennis Court - Valid NFDD but not in TDS Spec
      ['F_CODE','AK060','tourism','camp_site'], // Campground/Campsite
      ['F_CODE','AK061','tourism','picnic_site'], // Picnic Site
      ['F_CODE','AK070','drive_in','yes'], // Drive In Theater
      ['F_CODE','AK080','man_made','outdoor_theatre_screen'], // Outdoor Theatre Screen
      ['F_CODE','AK090','leisure','fairground'], // Fairground
      ['F_CODE','AK100','leisure','golf_course'], // Golf Course
      ['F_CODE','AK101','golf','driving_range'], // Golf Driving Range
      ['F_CODE','AK110','building','grandstand'], // Grandstand
      ['F_CODE','AK120','leisure','park'], // Park
      ['F_CODE','AK121','tourism','viewpoint'], // Lookout
      ['F_CODE','AK130','leisure','track'], // Racetrack
      ['F_CODE','AK150','man_made','ski_jump'], // Ski-jump
      ['F_CODE','AK155','sport','skiing'], // Ski-run # Not good
      ['F_CODE','AK160','leisure','stadium'], // Stadium/Amphitheater
      ['F_CODE','AK161','man_made','scoreboard'], // Scoreboard
      ['F_CODE','AK164','theatre:type','amphi'], // Amphitheatre
      ['F_CODE','AK170','leisure','swimming_pool'], // Swimming Pool
      ['F_CODE','AK180','tourism','zoo'], // Zoo/Safari Park
      ['F_CODE','AL010','facility','yes'], // Facility
      ['F_CODE','AL011','landuse','installation'], // Installation. This is a group of AL010 Facilities.
      ['F_CODE','AL012','historic','archaeological_site'], // Archeological Site
      ['F_CODE','AL013','building','yes'], // Building - See custom rules
      ['F_CODE','AL014','man_made','structure'], // Non-building Structure
      ['F_CODE','AL017','emergency','fire_hydrant'], // Fire Hydrant
      ['F_CODE','AL018','building:superstructure','yes'], // Building Superstructure Addition
      ['F_CODE','AL019','building','shed'], // Shed
      ['F_CODE','AL020','landuse','built_up_area'], // Built-up Area
      ['F_CODE','AL024','place','neighbourhood'], // GGDM3/NAS7 GGDM3/NAS7 NEIGHBOURHOOD
      ['F_CODE','AL025','man_made','cairn'], // Cairn
      ['F_CODE','AL030','landuse','cemetery'], // Cemetery
      ['F_CODE','AL036','tomb','yes'], // Tomb
      ['F_CODE','AL060','barrier','dragons_teeth'], // Dragon's Teeth - fixed in pre/post processing
      ['F_CODE','AL065','hazard','minefield'], // Minefield
      ['F_CODE','AL070','barrier','fence'], // Fence
      ['F_CODE','AL073','man_made','flagpole'], // Flagpole
      ['F_CODE','AL080','man_made','gantry'], // Gantry
      ['F_CODE','AL099','building','hut'], // Hut
      ['F_CODE','AL105','place','settlement'], // Settlement
      ['F_CODE','AL110','tower:type','light'], // Light Support Structure
      ['F_CODE','AL120','military','missile_site'], // Missile Site
      ['F_CODE','AL121','military','anti_air_artillery'], // GGDM3/NAS7 ANTI_AIRCRAFT_ARTILLERY_SITE
      ['F_CODE','AL130','historic','monument'], // Memorial Monument
      ['F_CODE','AL140','man_made','particle_accelerator'], // Particle Accelerator
      ['F_CODE','AL142','landuse','observatory'], // Astronomical Observatory
      ['F_CODE','AL155','overhead_obstruction','yes'], // Overhead Obstruction
      ['F_CODE','AL165','man_made','pipeline_crossing'], // Pipeline Or Pipe Crossing Point
      ['F_CODE','AL170','landuse','plaza'], // Plaza/City Square, Public Square
      ['F_CODE','AL175','man_made','courtyard'], // Courtyard
      ['F_CODE','AL180','shop','kiosk'], // Retail Stand
      ['F_CODE','AL195','ramp','yes'], // Ramp
      ['F_CODE','AL200','historic','ruins'], // Ruins
      ['F_CODE','AL201','tourism','attraction'], // Interest Site
      ['F_CODE','AL208','place','shanty_town'], // Shanty Town
      ['F_CODE','AL211','route:protection','yes'], // Transportation Route Protection Structure
      ['F_CODE','AL241','man_made','tower'], // Tower
      ['F_CODE','AL250','building:underground','yes'], // Underground Dwelling
      ['F_CODE','AL260','barrier','wall'], // Wall
      ['F_CODE','AL270','landuse','plantation'], // Industrial Farm
      ['F_CODE','AL351','space_facility','yes'], // Space Facility
      ['F_CODE','AL371','building','manor_house'], // Manor House # Not great
      ['F_CODE','AL375','historic','castle'], // Castle
      ['F_CODE','AL376','historic','castle_complex'], // Castle Complex
      ['F_CODE','AL510','man_made','tethered_balloon'], // Tethered Balloon
      ['F_CODE','AM010','landuse','depot'], // Storage Depot
      ['F_CODE','AM011','shipping_container','yes'], // Shipping Container
      ['F_CODE','AM020','man_made','silo'], // Grain Bin/Silo, Grain Storage Structure
      ['F_CODE','AM030','man_made','grain_elevator'], // Grain Elevator
      ['F_CODE','AM040','landuse','mineral_pile'], // Mineral Pile
      ['F_CODE','AM060','man_made','bunker'], // Surface Bunker
      ['F_CODE','AM065','amenity','munitions_storage'], // Munition Storage Facility
      ['F_CODE','AM070','man_made','storage_tank'], // Storage Tank
      ['F_CODE','AM071','landuse','tank_farm'], // Tank Farm
      ['F_CODE','AM075','amenity','fuel_storage'], // Fuel Storage Facility
      ['F_CODE','AM080','man_made','water_tower'], // Water Tower
      ['F_CODE','AN010','railway','rail'], // Railway
      ['F_CODE','AN050','sidetrack','yes'], // Railway Sidetrack
      ['F_CODE','AN060','service','yard'], // Railway Yard
      ['F_CODE','AN075','railway','turntable'], // Railway Turntable
      ['F_CODE','AN076','railway','roundhouse'], // Roundhouse
      ['F_CODE','AN085','railway','signal'], // Railway Signal
      ['F_CODE','AP010','highway','track'], // Cart Track
      ['F_CODE','AP020','junction','yes'], // Road Interchange - See RIT
      ['F_CODE','AP030','highway','road'], // Road - See Custom rules
      ['F_CODE','AP033','highway','turning_circle'], // Engineered Turn Around Site
      ['F_CODE','AP040','barrier','gate'], // Gate
      ['F_CODE','AP041','barrier','yes'], // Vehicle Barrier
      ['F_CODE','AP050','highway','path'], // Trail
      ['F_CODE','AP055','highway','tank_trail'], // Tank Trail
      ['F_CODE','AP056','crossing','tank'], // Tank Crossing
      ['F_CODE','AQ035','sidewalk','yes'], // Sidewalk
      ['F_CODE','AQ040','bridge','yes'], // Bridge
      ['F_CODE','AQ045','bridge:span','yes'], // Bridge Span
      ['F_CODE','AQ050','bridge:superstructure','yes'], // Bridge Superstructure
      ['F_CODE','AQ055','bridge:pylon','yes'], // Bridge Tower/Bridge Pylon
      ['F_CODE','AQ056','bridge:pier','yes'], // Bridge Pier
      ['F_CODE','AQ059','route_change','yes'], // Transportation Route Characteristic Change
      ['F_CODE','AQ060','control_tower','yes'], // Control Tower
      ['F_CODE','AQ062','crossing_point','yes'], // Crossing
      ['F_CODE','AQ063','man_made','causeway'], // Causeway Structure
      ['F_CODE','AQ065','tunnel','culvert'], // Culvert
      ['F_CODE','AQ068','barrier','block'], // Transportation Block
      ['F_CODE','AQ070','route','ferry'], // Ferry Crossing
      ['F_CODE','AQ075','ice_road','yes'], // Ice Route
      ['F_CODE','AQ080','amenity','ferry_terminal'], // Ferry Station, Ferry Site
      ['F_CODE','AQ090','entrance','yes'], // Entrance and/or exit - Valid NFDD but not in TDS Spec
      ['F_CODE','AQ095','tunnel','mouth'], // Tunnel Mouth
      ['F_CODE','AQ110','seamark:type','mooring'], // Mooring Mast
      ['F_CODE','AQ111','man_made','watercourse_crossing'], // Prepared Watercourse Crossing
      ['F_CODE','AQ113','man_made','pipeline'], // Pipeline
      ['F_CODE','AQ114','man_made','storm_drain'], // Storm Drain # In TDS, it's a point, not a line, as the entry point
      ['F_CODE','AQ115','man_made','utility_cover'], // Utility Cover
      ['F_CODE','AQ116','man_made','pumping_station'], // Pumping Station
      ['F_CODE','AQ118','sharp_curve','yes'], // Sharp Curve
      ['F_CODE','AQ120','steep_grade','yes'], // Steep Grade
      ['F_CODE','AQ125','public_transport','station'], // Transportation Station
      ['F_CODE','AQ130','tunnel','yes'], // Tunnel
      ['F_CODE','AQ135','highway','rest_area'], // Roadside Rest Area
      ['F_CODE','AQ140','amenity','parking'], // Vehicle Lot, Vehicle Storage/Parking Area
      ['F_CODE','AQ141','parking','garage'], // Parking Garage
      ['F_CODE','AQ150','highway','steps'], // Stair
      ['F_CODE','AQ151','covered','arcade'], // Arcade
      ['F_CODE','AQ160','highway','traffic_signals'], // Traffic Light
      ['F_CODE','AQ161','highway','street_lamp'], // Street Lamp
      ['F_CODE','AQ162','street_sign','yes'], // Street Sign
      ['F_CODE','AQ170','amenity','fuel'], // Motor Vehicle Station
      ['F_CODE','AQ180','man_made','weigh_station'], // Weigh Station
      ['F_CODE','AT005','cable','yes'], // Cable
      ['F_CODE','AT010','man_made','satellite_dish'], // Dish Aerial
      ['F_CODE','AT011','man_made','antenna'], // Aerial
      ['F_CODE','AT012','landuse','aerial_farm'], // Aerial Farm
      ['F_CODE','AT041','cableway','yes'], // Cableway
      ['F_CODE','AT042','pylon','yes'], // Pylon
      ['F_CODE','AT045','man_made','radar_station'], // Radar Station, Radar Transmitter
      ['F_CODE','BA010','natural','coastline'], // Coastline/Shoreline, Land Water Boundary
      ['F_CODE','BA023','foreshore','yes'], // GGDM3/NAS7 Foreshore (Precise IHO)
      ['F_CODE','BA024','shoreline','yes'], // GGDM3/NAS7 SHORELINE
      ['F_CODE','BA030','place','island'], // Island
      ['F_CODE','BA040','water','tidal'], // Tidal Water
      ['F_CODE','BA050','natural','beach'], // GGDM3/NAS7 Beach
      ['F_CODE','BB005','landuse','harbour'], // Harbor, Harbour
      ['F_CODE','BB008','harbour:waters','yes'], // GGDM3/NAS7 HARBOUR_WATERS
      ['F_CODE','BB009','landuse','port'], // Port
      ['F_CODE','BB010','seamark:type','anchorage'], // GGDM3/NAS7 Anchorage
      ['F_CODE','BB019','man_made','anchor'], // GGDM3/NAS7 ANCHOR
      ['F_CODE','BB020','seamark:type','berth'], // GGDM3/NAS7 BERTH
      ['F_CODE','BB050','seamark:type','calling-in_point'], // GGDM3/NAS7 CALLING_IN_POINT
      ['F_CODE','BB080','seamark:mooring:category','dolphin'], // GGDM3/NAS7 DOLPHIN
      ['F_CODE','BB081','shoreline_construction','yes'], // Shoreline Construction
      ['F_CODE','BB082','leisure','slipway'], // Shoreline Ramp
      ['F_CODE','BB090','dock','drydock'], // Dry Dock
      ['F_CODE','BB100','seamark:fishing_facility:category','stake'], // GGDM3/NAS7 Fishing Stakes
      ['F_CODE','BB110','man_made','fish_traps'], // Fish Traps/Fish Weirs
      ['F_CODE','BB115','seamark:type','gridiron'], // GGDM3/NAS7 NAUTICAL_GRIDIRON
      ['F_CODE','BB150','beach_landing_site','yes'], // GGDM3/NAS7 BEACH_LANDING_SITE
      ['F_CODE','BB155','seamark:type','signal_station'], // GGDM3/NAS7 Maritime Signal Station
      ['F_CODE','BB199','dock','floating'], // Floating Dry Dock
      ['F_CODE','BB201','amenity','small_craft_facility'], // Small Craft Facility
      ['F_CODE','BB241','seamark:harbour:category','shipyard'], // Shipyard
      ['F_CODE','BC010','seamark:type','beacon'], // GGDM3/NAS7 Beacon
      ['F_CODE','BC020','seamark:buoy','yes'], // GGDM3/NAS7 BUOY
      ['F_CODE','BC034','seamark:type','radio_station'], // GGDM3/NAS7 MARITIME_RADIOBEACON
      ['F_CODE','BC040','seamark:light','yes'], // GGDM3/NAS7 MARITIME_NAVIGATION_LIGHT
      ['F_CODE','BC041','seamark:type','light_support'], // GGDM3/NAS7 MARITIME_NAV_LIGHT_SUPPORT
      ['F_CODE','BC050','man_made','lighthouse'], // Lighthouse
      ['F_CODE','BC055','seamark:type','navigation_marker'], // GGDM3/NAS7 MARITIME_NAVIGATION_MARKER
      ['F_CODE','BC060','seamark:type','light_sector'], // GGDM3/NAS7 LIGHT_SECTOR
      ['F_CODE','BC070','seamark:type','light_vessel'], // Light Vessel
      ['F_CODE','BC080','seamark:type','insubstatial_navigation_marker'], // GGDM3/NAS7 INSUBSTANTIAL_NAV_MARK
      ['F_CODE','BD020','seamark:obstruction:category','crib'], // GGDM3/NAS7 Crib
      ['F_CODE','BD030','discolored_water','yes'], // GGDM3/NAS7 Discolored Water
      ['F_CODE','BD050','foul_ground','yes'], // GGDM3/NAS7 Foul Ground
      ['F_CODE','BD061','natural','aquatic_vegetation'], // GGDM3/NAS7 Aquatic Vegetation
      ['F_CODE','BD100','structural_pile','yes'], // Structural Pile
      ['F_CODE','BD115','man_made','offshore_construction'], // Offshore Construction
      ['F_CODE','BD120','natural','reef'], // GGDM3/NAS7 Reef
      ['F_CODE','BD130','seamark:obstruction:category','rock'], // GGDM3/NAS7 Hazardous Rock
      ['F_CODE','BD140','seamark:obstruction:category','stump'], // Snag / Stump
      ['F_CODE','BD180','historic','wreck'], // GGDM3/NAS7 Wreck
      ['F_CODE','BD181','seamark:type','hulk'], // Hulk
      ['F_CODE','BE010','seamark:type','depth_curve'], // GGDM3/NAS7 DEPTH_CURVE
      ['F_CODE','BE015','contour','depth'], // GGDM3/NAS7 DEPTH_CONTOUR
      ['F_CODE','BE019','seamark:type','depth_area'], // GGDM3/NAS7 DEPTH_AREA
      ['F_CODE','BE020','seamark:type','sounding'], // GGDM3/NAS7 SOUNDING
      ['F_CODE','BE050','beach_profile','yes'], // GGDM3/NAS7 BEACH_PROFILE
      ['F_CODE','BF010','seamark:type','seabed_area'], // GGDM3/NAS7 BOTTOM_CHARACTER_REGION
      ['F_CODE','BG010','seamark:type','water_movement'], // GGDM3/NAS7 WATER_MOVEMENT_DATA_LOCATION
      ['F_CODE','BG012','seamark:type','water_turbulence'], // GGDM3/NAS7 WATER_TURBULENCE
      ['F_CODE','BG030','seamark:type','observation_station'], // GGDM3/NAS7 TIDAL_STREAM_OBSERVE_STATION
      ['F_CODE','BH010','bridge','aqueduct'], // Aqueduct
      ['F_CODE','BH012','man_made','qanat_shaft'], // Qanat Shaft
      ['F_CODE','BH015','wetland','bog'], // Bog
      ['F_CODE','BH020','waterway','canal'], // Canal
      ['F_CODE','BH030','waterway','ditch'], // Ditch
      ['F_CODE','BH040','water','wastewater'], // Filtration beds/Aeration beds, Water Treatment Bed
      ['F_CODE','BH050','landuse','aquaculture'], // GGDM3/NAS7 MARICULTURE_SITE
      ['F_CODE','BH051','place','maritime_farm'], // Fish Farm Facility
      ['F_CODE','BH065','waterway','race'], // Water Race
      ['F_CODE','BH070','ford','yes'], // Ford
      ['F_CODE','BH075','amenity','fountain'], // Fountain
      ['F_CODE','BH077','natural','hummock'], // Hummock
      ['F_CODE','BH082','water','lake'], // Inland Waterbody
      ['F_CODE','BH090','water','intermittent'], // Land Subject to Inundation
      ['F_CODE','BH100','water','moat'], // Moat
      ['F_CODE','BH110','pipeline:type','penstock'], // Penstock
      ['F_CODE','BH116','natural','aquifer'], // Aquifer
      ['F_CODE','BH120','waterway','rapids'], // Rapids
      ['F_CODE','BH135','landuse','rice_field'], // Rice Field
      ['F_CODE','BH140','waterway','river'], // River - See custom rules
      ['F_CODE','BH145','waterway','vanishing_point'], // Vanishing point
      ['F_CODE','BH150','natural','salt_flat'], // Salt Flat, Salt Pan
      ['F_CODE','BH155','landuse','salt_pond'], // Salt Evaporator
      ['F_CODE','BH160','natural','sabkha'], // Sabkha
      ['F_CODE','BH165','waterway','spillway'], // Spillway
      ['F_CODE','BH170','natural','spring'], // Natural Pool, Spring/Water Hole
      ['F_CODE','BH180','waterway','waterfall'], // Waterfall
      ['F_CODE','BH190','water','lagoon'], // GGDM3/NAS7 LAGOON
      ['F_CODE','BH220','man_made','water_works'], // Waterwork
      ['F_CODE','BH230','man_made','water_well'], // Water Well
      ['F_CODE','BI005','waterway','boat_lift'], // Vessel Lift
      ['F_CODE','BI006','man_made','ship_elevator'], // Ship Elevator
      ['F_CODE','BI010','man_made','cistern'], // Cistern
      ['F_CODE','BI020','waterway','dam'], // Dam/Weir
      ['F_CODE','BI030','lock','yes'], // Lock
      ['F_CODE','BI040','waterway','flow_control'], // Sluice Gate
      ['F_CODE','BI044','flood_control','yes'], // Flood Control Structure
      ['F_CODE','BI045','man_made','basin_gate'], // Basin Gate
      ['F_CODE','BI050','tower','water_intake'], // Water Intake Tower
      ['F_CODE','BI060','waterway','fish_pass'], // Fish Ladder
      ['F_CODE','BI070','man_made','gauging_station'], // Gauging Station
      ['F_CODE','BJ020','geological','moraine'], // Moraine
      ['F_CODE','BJ030','natural','glacier'], // Glacier
      ['F_CODE','BJ031','natural','crevasse'], // Crevasse
      ['F_CODE','BJ040','natural','ice_cliff'], // Ice Cliff
      ['F_CODE','BJ060','natural','ice_peak'], // Ice Peak
      ['F_CODE','BJ065','natural','ice_shelf'], // Ice Shelf
      ['F_CODE','BJ070','natural','pack_ice'], // GGDM3/NAS7 PACK_ICE
      ['F_CODE','BJ080','natural','polar_ice'], // Polar Ice
      ['F_CODE','BJ099','glacier:type','icecap'], // Ice-cap
      ['F_CODE','BJ100','landcover','snowfield'], // Snow Field and/or Ice-field
      ['F_CODE','BJ110','natural','tundra'], // Tundra
      ['F_CODE','CA010','contour','elevation'], // Elevation Contour
      ['F_CODE','CA030','geological','spot_elevation'], // Spot Elevation
      ['F_CODE','DA005','natural','asphalt_lake'], // Asphalt Lake
      ['F_CODE','DA010','geological','soil_surface_region'], // Soil Surface Region
      ['F_CODE','DB010','natural','cliff'], // Bluff/Cliff/Escarpment, Steep Terrain Face
      ['F_CODE','DB028','geological','cave_chamber'], // Cave Chamber
      ['F_CODE','DB029','natural','cave_entrance'], // Cave Mouth
      ['F_CODE','DB061','natural','crevice'], // Crevice
      ['F_CODE','DB070','cutting','yes'], // Cut
      ['F_CODE','DB071','man_made','cut_edge'], // Cut Line ## In TDS, this is the edge of the cut
      ['F_CODE','DB080','natural','depression'], // Depression
      ['F_CODE','DB090','embankment','yes'], // Embankment, Embankment/Fill
      ['F_CODE','DB100','landform','esker'], // Esker
      ['F_CODE','DB110','geological','fault'], // Fault, Geologic Fault
      ['F_CODE','DB115','natural','geothermal_outlet'], // Geothermal Feature, Geothermal Outlet
      ['F_CODE','DB150','mountain_pass','yes'], // Mountain Pass
      ['F_CODE','DB160','natural','rock_formation'], // Rock Formation
      ['F_CODE','DB170','natural','dune'], // Sand Dune/Sand Hills
      ['F_CODE','DB180','natural','volcano'], // Volcano
      ['F_CODE','DB185','natural','crater'], // GGDM3/NAS7 CRATER
      ['F_CODE','DB190','natural','volcanic_dyke'], // Volcanic Dyke
      ['F_CODE','DB211','natural','slide'], // Landslide Mass
      ['F_CODE','EA010','landuse','farmland'], // Crop Land
      ['F_CODE','EA020','barrier','hedge'], // Hedgerow
      ['F_CODE','EA030','landuse','plant_nursery'], // Plant Nursery
      ['F_CODE','EA031','garden:type','botanical'], // Botanical Garden
      ['F_CODE','EA040','landuse','orchard'], // Orchard/Plantation
      ['F_CODE','EA050','landuse','vineyard'], // Vineyard
      ['F_CODE','EA055','landuse','hop_garden'], // Hop Field
      ['F_CODE','EB010','natural','grassland'], // Grassland
      ['F_CODE','EB020','natural','scrub'], // Thicket (was FACC Scrub/Brush/Bush)
      ['F_CODE','EB070','natural','brush'], // Brush - possibly "heath"
      ['F_CODE','EC005','natural','tree'], // Tree
      ['F_CODE','EC010','natural','cane'], // Bamboo/Cane
      ['F_CODE','EC015','landuse','forest'], // Forest
      ['F_CODE','EC020','natural','oasis'], // Oasis
      ['F_CODE','EC040','man_made','cutline'], // Cleared Way/Cut Line/Firebreak
      ['F_CODE','EC050','natural','grove'], // GGDM3/NAS7 GROVE
      ['F_CODE','EC060','natural','forest_clearing'], // Forest Clearing
      ['F_CODE','ED010','wetland','marsh'], // Marsh
      ['F_CODE','ED020','wetland','swamp'], // Swamp
      ['F_CODE','EE010','landuse','logging'], // Logging Area, Logging Site
      ['F_CODE','EE030','natural','desert'], // Desert
      ['F_CODE','FA000','boundary','administrative'], // GGDM3/NAS7 Administrative Boundary
      ['F_CODE','FA002','place','geopolitical_entity'], // GGDM3/NAS7 GEOPOLITICAL_ENTITY
      ['F_CODE','FA003','place','administrative_subdivision'], // GGDM3/NAS7 ADMINISTRATIVE_SUBDIVISION
      ['F_CODE','FA012','protect_class','16'], // Contaminated Region
      ['F_CODE','FA015','military','range'], // Firing Range/Gunnery Range
      ['F_CODE','FA100','landuse','test_area'], // Test Area, Test Site
      ['F_CODE','FA110','boundary','date_line'], // GGDM3/NAS7 International Date Line
      ['F_CODE','FA165','landuse','training_area'], // Training Area, Training Site
      ['F_CODE','FA210','boundary','protected_area'], // Conservation Area
      ['F_CODE','FC021','seamark:type','boundary'], // GGDM3/NAS7 MARITIME_LIMIT
      ['F_CODE','FC034','waterway','dredged_area'], // GGDM3/NAS7 DREDGED_AREA
      ['F_CODE','FC035','natural','divide'], // GGDM3/NAS7 WATERBODY_DIVIDER
      ['F_CODE','FC037','seamark:type','precautionary_area'], // GGDM3/NAS7 MARITIME_CAUTION_AREA
      ['F_CODE','FC041','seamark:type','traffic_separation_scheme'], // GGDM3/NAS7 TRAFFIC_SEPARATION_SCHEME
      ['F_CODE','FC100','seamark:type','measured_distance_line'], // GGDM3/NAS7 MEASURED_DISTANCE_LINE
      ['F_CODE','FC130','seamark:type','radar_line'], // GGDM3/NAS7 MARITIME_RADAR_REF_LINE
      ['F_CODE','FC165','seamark:type','route'], // GGDM3/NAS7 MARITIME_ROUTE
      ['F_CODE','FC177','waterway','swept_area'], // GGDM3/NAS7 SWEPT_AREA
      ['F_CODE','GA033','airmark','beacon'], // GGDM3/NAS7 AERO_RADIO_NAV_INSTALLATION
      ['F_CODE','GB005','aeroway','aerodrome'], // Land Aerodrome
      ['F_CODE','GB013','aeroway','navigationaid'], // GGDM3/NAS7 AERODROME_BEACON
      ['F_CODE','GB015','aeroway','apron'], // Apron/Hardstand
      ['F_CODE','GB020','aeroway','arresting_gear'], // Arresting Gear - Valid NFDD Not in TDS
      ['F_CODE','GB030','aeroway','helipad'], // Helipad
      ['F_CODE','GB035','aeroway','heliport'], // Heliport
      ['F_CODE','GB040','aeroway','launchpad'], // Launch Pad
      ['F_CODE','GB045','aeroway','stopway'], // Overrun/Stopway
      ['F_CODE','GB050','military','revetment'], // Aircraft/Defensive Revetment
      ['F_CODE','GB055','aeroway','runway'], // Runway
      ['F_CODE','GB056','aeroway','runway_endpoint'], // Runway Endpoint - Valid NFDD Not in TDS
      ['F_CODE','GB065','amenity','seaplane_terminal'], // Seaplane Base, Water Aerodrome
      ['F_CODE','GB070','seamark:type','seaplane_landing_area'], // Seaplane Run
      ['F_CODE','GB075','aeroway','taxiway'], // Taxiway
      ['F_CODE','GB230','building','hangar'], // Aircraft Hangar
      ['F_CODE','GB250','bunker_type','hardened_aircraft_shelter'], // Hardened Aircraft Shelter
      ['F_CODE','IA040','boundary','land_parcel'], // Land Parcel
      ['F_CODE','NA170','basin','drainage'], // GGDM3/NAS7 DRAINAGE_BASIN
      ['F_CODE','NM010','landuse','aqueducts'], // GGDM3/NAS7 AQUEDUCT_AON
      ['F_CODE','NM020','landuse','canals'], // GGDM3/NAS7 NAVIGABLE_CANAL_AON
      ['F_CODE','NM030','landuse','ditches'], // GGDM3/NAS7 DITCH_AON
      ['F_CODE','NM082','natural','waterbody_area'], // GGDM3/NAS7 INLAND_WATERBODY_AON
      ['F_CODE','NM140','natural','river_area'], // GGDM3/NAS7 RIVER_AON
      ['F_CODE','NM160','natural','sabkha_area'], // GGDM3/NAS7 SABKHA_AON
      ['F_CODE','NM170','natural','spring_area'], // GGDM3/NAS7 NATURAL_POOL_AON
      ['F_CODE','NM230','natural','water_well_area'], // GGDM3/NAS7 WATER_WELL_AON
      ['F_CODE','NU010','landuse','cistern_area'], // GGDM3/NAS7 CISTERN_AON
      ['F_CODE','NU070','landuse','storage_tank_area'], // GGDM3/NAS7 STORAGE_TANK_AON
      ['F_CODE','NU113','landuse','pipeline_area'], // GGDM3/NAS7 PIPELINE_AON
      ['F_CODE','NU116','landuse','pumping_station_area'], // GGDM3/NAS7 PUMPING_STATION_AON
      ['F_CODE','NU170','landuse','swimming_pool_area'], // GGDM3/NAS7 SWIMMING_POOL_AON
      ['F_CODE','SA050','slope','yes'], // GGDM3/NAS7 SLOPE_REGION
      ['F_CODE','STB23','aeroway','landing_zone'], // GGDM3/NAS7 LANDING_ZONE
      ['F_CODE','STBM1','boundary','military'], // GGDM3/NAS7 MILITARY_BOUNDARY
      ['F_CODE','STDPP','man_made','standpipe'], // GGDM3/NAS7 STANDPIPE
      ['F_CODE','SU001','military','installation'], // Military Installation
      ['F_CODE','SU004','military','cantonment'], // Cantonment Area
      ['F_CODE','SU030','boundary','installation'], // Installation Boundary
      ['F_CODE','T0181','man_made','pump'], // GGDM3/NAS7 PUMP
      ['F_CODE','ZB020','survey_point:type','benchmark'], // GGDM3/NAS7 BENCHMARK
      ['F_CODE','ZB030','historic','boundary_stone'], // Boundary Monument
      ['F_CODE','ZB036','seamark:type','distance_mark'], // GGDM3/NAS7 DISTANCE_MARK
      ['F_CODE','ZB050','man_made','survey_point'], // Survey Point
      ['F_CODE','ZB060','survey_point:type','geodetic_point'], // GGDM3/NAS7 GEODETIC_POINT
      ['F_CODE','ZC040','natural','magnetic_anomoly'], // GGDM3/NAS7 LOCAL_MAGNETIC_ANOMALY
      ['F_CODE','ZD020','void_collection_area','yes'], // Void Collection Area
      ['F_CODE','ZD030','cultural_context_location','yes'], // GGDM3/NAS7 Cultural Context Location
      ['F_CODE','ZD040','named_location','yes'], // Named Location
      ['F_CODE','ZD045','annotation','yes'], // Annotated Location, Text Description
      ['F_CODE','ZD070','man_made','water_measurement_location'], // Water Measurement Location
      ['F_CODE','ZI002','source:metadata','restriction'], // Restriction Information
      ['F_CODE','ZI026','source:metadata','feature'], // Feature Metadata
      ['F_CODE','ZI027','source:metadata','feature_attribute'], // Feature Atribute Metadata
      ['F_CODE','ZI031','source:metadata','dataset'], // Dataset Metadata
      ['F_CODE','ZI039','source:metadata','entity'], // Entity Collection Metadata
      ['F_CODE','ZI040','source:metadata','spatial_entity'], // Spatial Metadata Entity
      ['F_CODE','ZI041','source:metadata','non_spatial_entity'], // Non-Spatial Metadata Entity
      ['F_CODE','ZJ001','source:metadata','relation'] // Resource Metadata Relation
    ] // End fcodeOne2one
} // End of fcodeCommon
