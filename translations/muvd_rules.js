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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2013, 2014 Maxar (http://www.maxar.com/)
 */

/*
    MUVD Rules
*/

muvd.rules = {
    // These are for finding an FCODE for export.
    // NOTE: these get swapped around before being used
    fcodeOne2oneOut : [
        []
    ],

    // Stuff to be ignored or that gets swapped later
    // This is taken from OSM pre processing and a few things added.
    ignoreList : ['FCODE'],
    // End of ignoreList

    // Attribute values that are to be dropped since they are defaults or don't add value.
    dropList : { '-32767':1,'-32767.0':1,'unk':1},
    // ###### End of dropList ######

    // Attribute names to swap on input.
    swapListIn : {},
    // ###### End of swapListIn ######

    // One2one rules for Text attributes
    txtBiased : {

    }, // End txtBiased

    // One2one rules for Number attributes
    numBiased : {

    }, // End numBiased

    // Common one2one rules. Used for both import and export
    one2one : [

    ], // End one2one

    // Static lookup tables. If the schema changes, rebuild these in getDbSchema()

    // List of all of the valid MUVD FCODEs and their english name
    fcodeNameLookup : {"AA010":"Extraction Mine",
                       "AA011":"Shear Wall",
                       "AA020":"Mine Shaft Superstructure",
                       "AA040":"Rig",
                       "AA052":"Hydrocarbons Field",
                       "AA054":"Non-water Well",
                       "AB000":"Disposal Site",
                       "AB010":"Recycling Site",
                       "AB040":"Aeration Basin",
                       "AC010":"Blast-furnace",
                       "AC020":"Catalytic Cracker",
                       "AC030":"Settling Pond",
                       "AC040":"Hydrocarbon Products Facility",
                       "AC060":"Industrial Furnace",
                       "AC507":"Sewage Treatment Plant",
                       "AD010":"Electric Power Station",
                       "AD020":"Solar Panel",
                       "AD025":"Solar Farm",
                       "AD030":"Power Substation",
                       "AD041":"Nuclear Reactor Containment",
                       "AD050":"Heating Facility",
                       "AD055":"Cooling Facility",
                       "AD060":"Wind Farm",
                       "AF010":"Smokestack",
                       "AF020":"Conveyor",
                       "AF030":"Cooling Tower",
                       "AF040":"Crane",
                       "AF050":"Excavating Machine",
                       "AF070":"Flare Pipe",
                       "AF080":"Hopper",
                       "AG030":"Shopping Complex",
                       "AG040":"Office Park",
                       "AG060":"Market",
                       "AH050":"Fortification",
                       "AH060":"Underground Bunker",
                       "AH070":"Checkpoint",
                       "AI030":"Camp",
                       "AI040":"Area Of Displaced Persons",
                       "AJ030":"Holding Pen",
                       "AJ050":"Windmill",
                       "AJ051":"Wind Turbine",
                       "AJ055":"Water Mill",
                       "AJ060":"Farm",
                       "AJ085":"Barn",
                       "AJ110":"Greenhouse",
                       "AK015":"Bandshell",
                       "AK020":"Amusement Park Attraction",
                       "AK030":"Amusement Park",
                       "AK040":"Sports Ground",
                       "AK060":"Camp-site",
                       "AK070":"Drive-in Theatre",
                       "AK080":"Outdoor Theatre Screen",
                       "AK100":"Golf Course",
                       "AK101":"Golf Driving Range",
                       "AK110":"Grandstand",
                       "AK120":"Park",
                       "AK121":"Lookout",
                       "AK130":"Racetrack",
                       "AK150":"Ski-jump",
                       "AK160":"Stadium",
                       "AK164":"Amphitheatre",
                       "AK170":"Swimming Pool",
                       "AK180":"Zoo",
                       "AL010":"Facility",
                       "AL012":"Archeological Site",
                       "AL013":"Building",
                       "AL014":"Non-building Structure",
                       "AL018":"Building Superstructure",
                       "AL019":"Shed",
                       "AL020":"Built-up Area",
                       "AL022":"Populated Place",
                       "AL025":"Cairn",
                       "AL030":"Cemetery",
                       "AL035":"Grave",
                       "AL036":"Tomb",
                       "AL037":"Mass Grave",
                       "AL050":"Display Sign",
                       "AL065":"Minefield",
                       "AL070":"Fence",
                       "AL073":"Flagpole",
                       "AL080":"Gantry",
                       "AL099":"Hut",
                       "AL110":"Light Support Structure",
                       "AL120":"Missile Site",
                       "AL121":"Anti-aircraft Artillery Site",
                       "AL131":"Monument",
                       "AL140":"Particle Accelerator",
                       "AL142":"Astronomical Observatory",
                       "AL155":"Overhead Obstruction",
                       "AL165":"Pipeline Crossing Point",
                       "AL170":"Public Square",
                       "AL200":"Ruins",
                       "AL211":"Transportation Route Protection Structure",
                       "AL241":"Tower",
                       "AL250":"Underground Dwelling",
                       "AL260":"Wall",
                       "AL270":"Industrial Farm",
                       "AL351":"Space Facility",
                       "AL510":"Tethered Balloon",
                       "AM010":"Storage Depot",
                       "AM011":"Shipping Container",
                       "AM020":"Grain Storage Structure",
                       "AM030":"Grain Elevator",
                       "AM040":"Mineral Pile",
                       "AM060":"Surface Bunker",
                       "AM065":"Munition Storage Facility",
                       "AM070":"Storage Tank",
                       "AM071":"Tank Farm",
                       "AM075":"Fuel Storage Facility",
                       "AM080":"Water Tower",
                       "AN010":"Railway",
                       "AN050":"Railway Sidetrack",
                       "AN060":"Railway Yard",
                       "AN075":"Railway Turntable",
                       "AN076":"Roundhouse",
                       "AN077":"Railway Traverser",
                       "AN078":"Bogie Exchange Site",
                       "AP040":"Gate",
                       "AP041":"Vehicle Barrier",
                       "AP075":"Land Transportation Way",
                       "AQ040":"Bridge",
                       "AQ055":"Bridge Tower",
                       "AQ056":"Bridge Pier",
                       "AQ060":"Control Tower",
                       "AQ063":"Causeway Structure",
                       "AQ065":"Culvert",
                       "AQ068":"Transportation Block",
                       "AQ070":"Ferry Crossing",
                       "AQ080":"Ferry Station",
                       "AQ095":"Tunnel Mouth",
                       "AQ110":"Mooring Mast",
                       "AQ113":"Pipeline",
                       "AQ116":"Pumping Station",
                       "AQ125":"Transportation Station",
                       "AQ130":"Tunnel",
                       "AQ135":"Roadside Rest Area",
                       "AQ140":"Vehicle Lot",
                       "AQ141":"Parking Garage",
                       "AQ150":"Stair",
                       "AQ151":"Arcade",
                       "AQ170":"Motor Vehicle Station",
                       "AQ180":"Weigh Station",
                       "AQ210":"Transportation Platform",
                       "AT005":"Cable",
                       "AT010":"Dish Aerial",
                       "AT011":"Aerial",
                       "AT012":"Aerial Farm",
                       "AT041":"Cableway",
                       "AT042":"Pylon",
                       "AT045":"Radar Station",
                       "BA010":"Land Water Boundary",
                       "BA030":"Island",
                       "BA040":"Tidal Water",
                       "BA050":"Beach",
                       "BB005":"Harbour",
                       "BB009":"Port",
                       "BB030":"Bollard",
                       "BB081":"Shoreline Construction",
                       "BB082":"Shoreline Ramp",
                       "BB090":"Dry Dock",
                       "BB110":"Fish Weir",
                       "BB199":"Floating Dry Dock",
                       "BB241":"Shipyard",
                       "BC050":"Lighthouse",
                       "BC070":"Light Vessel",
                       "BD100":"Structural Pile",
                       "BD115":"Offshore Construction",
                       "BD120":"Reef",
                       "BD123":"Floating Barrier",
                       "BD130":"Hazardous Rock",
                       "BD180":"Wreck",
                       "BD181":"Hulk",
                       "BH010":"Aqueduct",
                       "BH012":"Qanat Shaft",
                       "BH015":"Bog",
                       "BH020":"Canal",
                       "BH030":"Ditch",
                       "BH040":"Water Treatment Bed",
                       "BH050":"Mariculture Site",
                       "BH051":"Fish Farm Facility",
                       "BH065":"Water Race",
                       "BH070":"Ford",
                       "BH075":"Fountain",
                       "BH082":"Inland Waterbody",
                       "BH090":"Land Subject to Inundation",
                       "BH100":"Moat",
                       "BH110":"Penstock",
                       "BH120":"Rapids",
                       "BH135":"Rice Field",
                       "BH140":"River",
                       "BH145":"Vanishing Point",
                       "BH150":"Salt Flat",
                       "BH155":"Salt Evaporator",
                       "BH160":"Sabkha",
                       "BH165":"Spillway",
                       "BH172":"Spring",
                       "BH180":"Waterfall",
                       "BH190":"Lagoon",
                       "BH220":"Waterwork",
                       "BH230":"Water Well",
                       "BI005":"Vessel Lift",
                       "BI006":"Ship Elevator",
                       "BI010":"Cistern",
                       "BI020":"Dam",
                       "BI030":"Lock",
                       "BI040":"Sluice Gate",
                       "BI044":"Flood Control Structure",
                       "BI045":"Basin Gate",
                       "BI050":"Water Intake Tower",
                       "BJ020":"Moraine",
                       "BJ030":"Glacier",
                       "BJ031":"Crevasse",
                       "BJ040":"Ice Cliff",
                       "BJ060":"Ice Peak",
                       "BJ065":"Ice Shelf",
                       "BJ099":"Ice-cap",
                       "BJ100":"Snow Field and/or Ice-field",
                       "BJ110":"Tundra",
                       "CA010":"Elevation Contour",
                       "CA030":"Spot Elevation",
                       "DA005":"Asphalt Lake",
                       "DA010":"Soil Surface Region",
                       "DB010":"Steep Terrain Face",
                       "DB029":"Cave Mouth",
                       "DB061":"Crevice",
                       "DB070":"Cut",
                       "DB071":"Cut Line",
                       "DB090":"Embankment",
                       "DB100":"Esker",
                       "DB110":"Geologic Fault",
                       "DB115":"Geothermal Outlet",
                       "DB150":"Mountain Pass",
                       "DB160":"Rock Formation",
                       "DB170":"Sand Dunes",
                       "DB180":"Volcano",
                       "EA010":"Crop Land",
                       "EA020":"Hedgerow",
                       "EA030":"Plant Nursery",
                       "EA031":"Botanic Garden",
                       "EA040":"Orchard",
                       "EA050":"Vineyard",
                       "EA055":"Hop Field",
                       "EA060":"Allotment Area",
                       "EB010":"Grassland",
                       "EB020":"Thicket",
                       "EC005":"Tree",
                       "EC015":"Forest",
                       "EC020":"Oasis",
                       "EC040":"Cleared Way",
                       "ED010":"Marsh",
                       "ED020":"Swamp",
                       "FA000":"Administrative Boundary",
                       "FA002":"Geopolitical Entity",
                       "FA003":"Administrative Division",
                       "FA012":"Contaminated Region",
                       "FA015":"Firing Range",
                       "FA091":"Geophysical Data Track Line",
                       "FA100":"Test Site",
                       "FA165":"Training Site",
                       "FA210":"Conservation Area",
                       "FA220":"Cultural Conservation Area",
                       "GA033":"Aeronautical Radio Navigation Installation",
                       "GA036":"VHF Omnidirectional Radio (VOR) Beacon",
                       "GA037":"Tactical Air Navigation Aid (TACAN) Beacon",
                       "GB005":"Land Aerodrome",
                       "GB013":"Aerodrome Beacon",
                       "GB015":"Apron",
                       "GB025":"Blast Barrier",
                       "GB030":"Helipad",
                       "GB035":"Heliport",
                       "GB040":"Launch Pad",
                       "GB045":"Stopway",
                       "GB055":"Runway",
                       "GB065":"Water Aerodrome",
                       "GB070":"Seaplane Run",
                       "GB075":"Taxiway",
                       "GB230":"Aircraft Hangar",
                       "GB250":"Hardened Aircraft Shelter",
                       "GB485":"Approach Lighting System",
                       "SU001":"Military Installation",
                       "ZB030":"Boundary Monument",
                       "ZD019":"Miscellaneous Feature Type",
                       "ZD020":"Void Collection Area",
                       "ZD050":"General Location"
                    },

    // layerNameLookup - List of all of the valid geometry & F_CODES.
    layerNameLookup : ["SAA010","SAA020","SAA040","SAA052","SAB000","SAB010","SAB040","SAC010","SAC020","SAC030","SAC040","SAC060","SAC507","SAD010","SAD020","SAD025","SAD030","SAD041","SAD050","SAD055","SAD060","SAF030","SAF040","SAF050","SAF080","SAG030","SAG040","SAG060","SAH050","SAH060","SAH070","SAI030","SAI040","SAJ030","SAJ050","SAJ055","SAJ060","SAJ085","SAJ110","SAK015","SAK020","SAK030","SAK040","SAK060","SAK070","SAK100","SAK101","SAK110","SAK120","SAK130","SAK160","SAK164","SAK170","SAK180","SAL010","SAL012","SAL013","SAL014","SAL018","SAL019","SAL020","SAL022","SAL025","SAL030","SAL036","SAL037","SAL065","SAL099","SAL120","SAL121","SAL131","SAL140","SAL142","SAL155","SAL170","SAL200","SAL211","SAL241","SAL250","SAL270","SAL351","SAM010","SAM011","SAM020","SAM030","SAM040","SAM060","SAM065","SAM070","SAM071","SAM075","SAM080","SAN060","SAN075","SAN076","SAN077","SAN078","SAQ040","SAQ055","SAQ056","SAQ060","SAQ063","SAQ068","SAQ080","SAQ116","SAQ125","SAQ130","SAQ135","SAQ140","SAQ141","SAQ150","SAQ151","SAQ170","SAQ180","SAQ210","SAT010","SAT011","SAT012","SAT045","SBA030","SBA040","SBA050","SBB005","SBB009","SBB081","SBB082","SBB090","SBB110","SBB199","SBB241","SBC050","SBC070","SBD115","SBD120","SBD130","SBD181","SBH010","SBH015","SBH020","SBH030","SBH040","SBH050","SBH051","SBH065","SBH075","SBH082","SBH090","SBH100","SBH120","SBH135","SBH140","SBH150","SBH155","SBH160","SBH165","SBH190","SBH220","SBH230","SBI005","SBI006","SBI010","SBI020","SBI030","SBI044","SBI050","SBJ020","SBJ030","SBJ031","SBJ060","SBJ065","SBJ099","SBJ100","SBJ110","SDA005","SDA010","SDB061","SDB090","SDB115","SDB160","SDB170","SDB180","SEA010","SEA030","SEA031","SEA040","SEA050","SEA055","SEA060","SEB010","SEB020","SEC015","SEC020","SEC040","SED010","SED020","SFA002","SFA003","SFA012","SFA015","SFA100","SFA165","SFA210","SFA220","SGA033","SGB005","SGB013","SGB015","SGB025","SGB030","SGB035","SGB040","SGB045","SGB055","SGB065","SGB070","SGB075","SGB230","SGB250","SGB485","SSU001","SZD019","SZD020","CAA011","CAF020","CAF040","CAF050","CAH050","CAK020","CAK080","CAK130","CAK150","CAL014","CAL018","CAL050","CAL070","CAL080","CAL110","CAL131","CAL155","CAL211","CAL260","CAN010","CAN050","CAP040","CAP041","CAP075","CAQ040","CAQ063","CAQ065","CAQ068","CAQ070","CAQ113","CAQ130","CAQ150","CAQ151","CAT005","CAT041","CAT042","CBA010","CBB030","CBB081","CBB082","CBB110","CBD120","CBD123","CBH010","CBH020","CBH030","CBH065","CBH070","CBH100","CBH110","CBH120","CBH140","CBH165","CBH180","CBI030","CBI040","CBI044","CBI045","CBJ031","CBJ040","CCA010","CDB010","CDB061","CDB070","CDB071","CDB090","CDB100","CDB110","CEA020","CEC015","CFA000","CFA091","CGB025","CGB485","CZD019","CZD050","PAA020","PAA040","PAA054","PAD020","PAD030","PAF010","PAF040","PAF050","PAF070","PAF080","PAH050","PAH060","PAH070","PAI030","PAJ051","PAK020","PAK121","PAL014","PAL018","PAL025","PAL035","PAL036","PAL037","PAL073","PAL099","PAL099","PAL110","PAL131","PAL155","PAL165","PAL241","PAL250","PAL510","PAM080","PAQ055","PAQ056","PAQ068","PAQ095","PAQ110","PAQ116","PAQ210","PAT010","PAT011","PAT042","PBB030","PBD100","PBD115","PBD130","PBD180","PBH012","PBH070","PBH075","PBH145","PBH165","PBH172","PBH180","PBH230","PBI010","PBI045","PBI050","PBJ060","PCA030","PDB029","PDB115","PDB150","PDB180","PEC005","PGA033","PGA036","PGA037","PGB013","PGB030","PGB040","PZB030","PZD019","PZD050"],
    
    // Mapping between geometry + FCODE and the Thematic layer name
    thematicGroupLookup : {"SAA010":"CulturalSrf","SAA020":"CulturalSrf","SAA040":"CulturalSrf","SAA052":"CulturalSrf","SAB000":"CulturalSrf","SAB010":"CulturalSrf","SAB040":"CulturalSrf","SAC010":"CulturalSrf","SAC020":"CulturalSrf","SAC030":"CulturalSrf","SAC040":"CulturalSrf","SAC060":"CulturalSrf","SAC507":"CulturalSrf","SAD010":"CulturalSrf","SAD020":"CulturalSrf","SAD025":"CulturalSrf","SAD030":"CulturalSrf","SAD041":"CulturalSrf","SAD050":"CulturalSrf","SAD055":"CulturalSrf","SAD060":"CulturalSrf","SAF030":"CulturalSrf","SAF040":"CulturalSrf","SAF050":"CulturalSrf","SAF080":"CulturalSrf","SAG030":"CulturalSrf","SAG040":"CulturalSrf","SAG060":"CulturalSrf","SAH050":"MilitaryInstallationsDefensiveStructuresSrf","SAH060":"MilitaryInstallationsDefensiveStructuresSrf","SAH070":"CulturalSrf","SAI030":"PopulationSrf","SAI040":"PopulationSrf","SAJ030":"AgriculturalSrf","SAJ050":"AgriculturalSrf","SAJ055":"CulturalSrf","SAJ060":"AgriculturalSrf","SAJ085":"AgriculturalSrf","SAJ110":"AgriculturalSrf","SAK015":"CulturalSrf","SAK020":"CulturalSrf","SAK030":"CulturalSrf","SAK040":"CulturalSrf","SAK060":"CulturalSrf","SAK070":"CulturalSrf","SAK100":"CulturalSrf","SAK101":"CulturalSrf","SAK110":"CulturalSrf","SAK120":"CulturalSrf","SAK130":"CulturalSrf","SAK160":"CulturalSrf","SAK164":"CulturalSrf","SAK170":"CulturalSrf","SAK180":"CulturalSrf","SAL010":"CulturalSrf","SAL012":"CulturalSrf","SAL013":"CulturalSrf","SAL014":"CulturalSrf","SAL018":"CulturalSrf","SAL019":"CulturalSrf","SAL020":"PopulationSrf","SAL022":"PopulationSrf","SAL025":"CulturalSrf","SAL030":"CulturalSrf","SAL036":"CulturalSrf","SAL037":"CulturalSrf","SAL065":"MilitaryInstallationsDefensiveStructuresSrf","SAL099":"CulturalSrf","SAL120":"MilitaryInstallationsDefensiveStructuresSrf","SAL121":"MilitaryInstallationsDefensiveStructuresSrf","SAL131":"CulturalSrf","SAL140":"CulturalSrf","SAL142":"CulturalSrf","SAL155":"CulturalSrf","SAL170":"CulturalSrf","SAL200":"CulturalSrf","SAL211":"TransportationSrf","SAL241":"CulturalSrf","SAL250":"CulturalSrf","SAL270":"AgriculturalSrf","SAL351":"AeronauticalFacilitySrf","SAM010":"CulturalSrf","SAM011":"CulturalSrf","SAM020":"AgriculturalSrf","SAM030":"AgriculturalSrf","SAM040":"CulturalSrf","SAM060":"MilitaryInstallationsDefensiveStructuresSrf","SAM065":"CulturalSrf","SAM070":"CulturalSrf","SAM071":"CulturalSrf","SAM075":"CulturalSrf","SAM080":"CulturalSrf","SAN060":"TransportationSrf","SAN075":"TransportationSrf","SAN076":"TransportationSrf","SAN077":"TransportationSrf","SAN078":"TransportationSrf","SAQ040":"TransportationSrf","SAQ055":"TransportationSrf","SAQ056":"TransportationSrf","SAQ060":"AeronauticalFacilitySrf","SAQ063":"TransportationSrf","SAQ068":"TransportationSrf","SAQ080":"TransportationSrf","SAQ116":"TransportationSrf","SAQ125":"TransportationSrf","SAQ130":"TransportationSrf","SAQ135":"TransportationSrf","SAQ140":"CulturalSrf","SAQ141":"CulturalSrf","SAQ150":"CulturalSrf","SAQ151":"TransportationSrf","SAQ170":"TransportationSrf","SAQ180":"TransportationSrf","SAQ210":"TransportationSrf","SAT010":"CulturalSrf","SAT011":"CulturalSrf","SAT012":"CulturalSrf","SAT045":"CulturalSrf","SBA030":"PhysiographySrf","SBA040":"OceanEnvironmentSrf","SBA050":"PhysiographySrf","SBB005":"PortsHarboursSrf","SBB009":"PortsHarboursSrf","SBB081":"OceanEnvironmentSrf","SBB082":"PortsHarboursSrf","SBB090":"PortsHarboursSrf","SBB110":"OceanEnvironmentSrf","SBB199":"PortsHarboursSrf","SBB241":"PortsHarboursSrf","SBC050":"HydrographicAidsNavigationSrf","SBC070":"HydrographicAidsNavigationSrf","SBD115":"OceanEnvironmentSrf","SBD120":"OceanEnvironmentSrf","SBD130":"OceanEnvironmentSrf","SBD181":"OceanEnvironmentSrf","SBH010":"InlandWaterSrf","SBH015":"VegetationSrf","SBH020":"TransportationSrf","SBH030":"InlandWaterSrf","SBH040":"CulturalSrf","SBH050":"OceanEnvironmentSrf","SBH051":"AgriculturalSrf","SBH065":"InlandWaterSrf","SBH075":"CulturalSrf","SBH082":"InlandWaterSrf","SBH090":"InlandWaterSrf","SBH100":"InlandWaterSrf","SBH120":"InlandWaterSrf","SBH135":"InlandWaterSrf","SBH140":"InlandWaterSrf","SBH150":"PhysiographySrf","SBH155":"CulturalSrf","SBH160":"PhysiographySrf","SBH165":"InlandWaterSrf","SBH190":"OceanEnvironmentSrf","SBH220":"CulturalSrf","SBH230":"InlandWaterSrf","SBI005":"TransportationSrf","SBI006":"InlandWaterSrf","SBI010":"CulturalSrf","SBI020":"InlandWaterSrf","SBI030":"InlandWaterSrf","SBI044":"InlandWaterSrf","SBI050":"InlandWaterSrf","SBJ020":"PhysiographySrf","SBJ030":"PhysiographySrf","SBJ031":"PhysiographySrf","SBJ060":"PhysiographySrf","SBJ065":"PhysiographySrf","SBJ099":"PhysiographySrf","SBJ100":"PhysiographySrf","SBJ110":"VegetationSrf","SDA005":"PhysiographySrf","SDA010":"PhysiographySrf","SDB061":"PhysiographySrf","SDB090":"PhysiographySrf","SDB115":"PhysiographySrf","SDB160":"PhysiographySrf","SDB170":"PhysiographySrf","SDB180":"PhysiographySrf","SEA010":"AgriculturalSrf","SEA030":"AgriculturalSrf","SEA031":"CulturalSrf","SEA040":"AgriculturalSrf","SEA050":"AgriculturalSrf","SEA055":"AgriculturalSrf","SEA060":"AgriculturalSrf","SEB010":"VegetationSrf","SEB020":"VegetationSrf","SEC015":"VegetationSrf","SEC020":"PhysiographySrf","SEC040":"VegetationSrf","SED010":"VegetationSrf","SED020":"VegetationSrf","SFA002":"BoundariesSrf","SFA003":"BoundariesSrf","SFA012":"BoundariesSrf","SFA015":"CulturalSrf","SFA100":"CulturalSrf","SFA165":"CulturalSrf","SFA210":"BoundariesSrf","SFA220":"CulturalSrf","SGA033":"AeronauticalAidsNavigationSrf","SGB005":"AeronauticalFacilitySrf","SGB013":"AeronauticalFacilitySrf","SGB015":"AeronauticalFacilitySrf","SGB025":"AeronauticalFacilitySrf","SGB030":"AeronauticalFacilitySrf","SGB035":"AeronauticalFacilitySrf","SGB040":"AeronauticalFacilitySrf","SGB045":"AeronauticalFacilitySrf","SGB055":"AeronauticalFacilitySrf","SGB065":"AeronauticalFacilitySrf","SGB070":"AeronauticalFacilitySrf","SGB075":"AeronauticalFacilitySrf","SGB230":"AeronauticalFacilitySrf","SGB250":"AeronauticalFacilitySrf","SGB485":"AeronauticalFacilitySrf","SSU001":"MilitaryInstallationsDefensiveStructuresSrf","SZD019":"FoundationSrf","SZD020":"FoundationSrf","CAA011":"CulturalCrv","CAF020":"CulturalCrv","CAF040":"CulturalCrv","CAF050":"CulturalCrv","CAH050":"MilitaryInstallationsDefensiveStructuresCrv","CAK020":"CulturalCrv","CAK080":"CulturalCrv","CAK130":"CulturalCrv","CAK150":"CulturalCrv","CAL014":"CulturalCrv","CAL018":"CulturalCrv","CAL050":"CulturalCrv","CAL070":"CulturalCrv","CAL080":"CulturalCrv","CAL110":"CulturalCrv","CAL131":"CulturalCrv","CAL155":"CulturalCrv","CAL211":"TransportationCrv","CAL260":"CulturalCrv","CAN010":"TransportationCrv","CAN050":"TransportationCrv","CAP040":"TransportationCrv","CAP041":"TransportationCrv","CAP075":"TransportationCrv","CAQ040":"TransportationCrv","CAQ063":"TransportationCrv","CAQ065":"TransportationCrv","CAQ068":"TransportationCrv","CAQ070":"TransportationCrv","CAQ113":"TransportationCrv","CAQ130":"TransportationCrv","CAQ150":"CulturalCrv","CAQ151":"TransportationCrv","CAT005":"TransportationCrv","CAT041":"TransportationCrv","CAT042":"TransportationCrv","CBA010":"PhysiographyCrv","CBB030":"PortsHarboursCrv","CBB081":"OceanEnvironmentCrv","CBB082":"PortsHarboursCrv","CBB110":"OceanEnvironmentCrv","CBD120":"OceanEnvironmentCrv","CBD123":"InlandWaterCrv","CBH010":"InlandWaterCrv","CBH020":"TransportationCrv","CBH030":"InlandWaterCrv","CBH065":"InlandWaterCrv","CBH070":"TransportationCrv","CBH100":"InlandWaterCrv","CBH110":"TransportationCrv","CBH120":"InlandWaterCrv","CBH140":"InlandWaterCrv","CBH165":"InlandWaterCrv","CBH180":"InlandWaterCrv","CBI030":"InlandWaterCrv","CBI040":"InlandWaterCrv","CBI044":"InlandWaterCrv","CBI045":"TransportationCrv","CBJ031":"PhysiographyCrv","CBJ040":"PhysiographyCrv","CCA010":"ElevationCrv","CDB010":"PhysiographyCrv","CDB061":"PhysiographyCrv","CDB070":"PhysiographyCrv","CDB071":"PhysiographyCrv","CDB090":"PhysiographyCrv","CDB100":"PhysiographyCrv","CDB110":"PhysiographyCrv","CEA020":"AgriculturalCrv","CEC015":"VegetationCrv","CFA000":"BoundariesCrv","CFA091":"GeodesyGeophysicsCrv","CGB025":"AeronauticalFacilityCrv","CGB485":"AeronauticalAidsNavigationCrv","CZD019":"FoundationCrv","CZD050":"FoundationCrv","PAA020":"CulturalPnt","PAA040":"CulturalPnt","PAA054":"CulturalPnt","PAD020":"CulturalPnt","PAD030":"CulturalPnt","PAF010":"CulturalPnt","PAF040":"CulturalPnt","PAF050":"CulturalPnt","PAF070":"CulturalPnt","PAF080":"CulturalPnt","PAH050":"MilitaryInstallationsDefensiveStructuresPnt","PAH060":"MilitaryInstallationsDefensiveStructuresPnt","PAH070":"CulturalPnt","PAI030":"PopulationPnt","PAJ051":"CulturalPnt","PAK020":"CulturalPnt","PAK121":"CulturalPnt","PAL014":"CulturalPnt","PAL018":"CulturalPnt","PAL025":"CulturalPnt","PAL035":"CulturalPnt","PAL036":"CulturalPnt","PAL037":"CulturalPnt","PAL073":"CulturalPnt","PAL099":"CulturalPnt","PAL099":"CulturalPnt","PAL110":"CulturalPnt","PAL131":"CulturalPnt","PAL155":"CulturalPnt","PAL165":"TransportationPnt","PAL241":"CulturalPnt","PAL250":"CulturalPnt","PAL510":"AeronauticalFacilityPnt","PAM080":"CulturalPnt","PAQ055":"TransportationPnt","PAQ056":"TransportationPnt","PAQ068":"TransportationPnt","PAQ095":"TransportationPnt","PAQ110":"AeronauticalFacilityPnt","PAQ116":"TransportationPnt","PAQ210":"TransportationPnt","PAT010":"CulturalPnt","PAT011":"CulturalPnt","PAT042":"CulturalPnt","PBB030":"PortsHarboursPnt","PBD100":"InlandWaterPnt","PBD115":"OceanEnvironmentPnt","PBD130":"OceanEnvironmentPnt","PBD180":"OceanEnvironmentPnt","PBH012":"InlandWaterPnt","PBH070":"TransportationPnt","PBH075":"CulturalPnt","PBH145":"InlandWaterPnt","PBH165":"InlandWaterPnt","PBH172":"InlandWaterPnt","PBH180":"InlandWaterPnt","PBH230":"InlandWaterPnt","PBI010":"CulturalPnt","PBI045":"TransportationPnt","PBI050":"InlandWaterPnt","PBJ060":"PhysiographyPnt","PCA030":"ElevationPnt","PDB029":"PhysiographyPnt","PDB115":"PhysiographyPnt","PDB150":"PhysiographyPnt","PDB180":"PhysiographyPnt","PEC005":"VegetationPnt","PGA033":"AeronauticalAidsNavigationPnt","PGA036":"AeronauticalAidsNavigationPnt","PGA037":"AeronauticalAidsNavigationPnt","PGB013":"AeronauticalFacilityPnt","PGB030":"AeronauticalFacilityPnt","PGB040":"AeronauticalFacilityPnt","PZB030":"BoundariesPnt","PZD019":"FoundationPnt","PZD050":"FoundationPnt"}
}
    // End of muvd.rules