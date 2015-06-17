#!/usr/bin/python

 #/*
 #* This file is part of Hootenanny.
 #*
 #* Hootenanny is free software: you can redistribute it and/or modify
 #* it under the terms of the GNU General Public License as published by
 #* the Free Software Foundation, either version 3 of the License, or
 #* (at your option) any later version.
 #*
 #* This program is distributed in the hope that it will be useful,
 #* but WITHOUT ANY WARRANTY; without even the implied warranty of
 #* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #* GNU General Public License for more details.
 #*
 #* You should have received a copy of the GNU General Public License
 #* along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #*
 #* --------------------------------------------------------------------
 #*
 #* The following copyright notices are generated automatically. If you
 #* have a new notice to add, please use the format:
 #* " * @copyright Copyright ..."
 #* This will properly maintain the copyright information. DigitalGlobe
 #* copyrights will be updated automatically.
 #*
 #* @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com/)
 #*/


# ConvertTDSv40Schema.py
#
# Convert a csv file imade from the TDS Spec to a Javascript data structure
#
# To get the .csv file: 
#   1. Open the spec in Excel
#   2. Goto the Feature_Schema tab
#   3. Select the second to eighth columns, "Table Name" -> "Range"
#   4. Copy the columns
#   5. Paste the columns into a new sheet as "Values"
#   6. Save the new sheet as a csv file
# 
# Note: This script does not do any error or sanity checking of the input file....
#
# Usage: ConvertNfddSchema.py  tdsv40.csv 
#
# Mattj Nov 13
#

import sys,os,csv,argparse,gzip

def openFile(path, mode):
    if path.endswith(".gz"):
        return gzip.GzipFile(path, mode)
    else:
        return open(path, mode)

# So Very Very Ugly
# This is hard coded as it lives in another tab in the spec
# This is the "common" list of FFN's. BUILDING and FACILITY share 99.9% of this list
ffnRawList = { "Agriculture":"2", "Growing of Crops":"3", "Raising of Animals":"9", "Grazing":"14",
"Forest Warden":"27", "Aquaculture":"35", "Mining and Quarrying":"40", "Ore Dressing":"95",
"Manufacturing":"99", "Food Product Manufacture":"100", "Food Processing":"101", "Meat Processing":"102",
"Seafood Processing":"103", "Fruit andor Vegetable Processing":"104", "Oil-mill":"105", "Dairying":"106",
"Grain Milling":"107", "Baking":"110", "Sugar Manufacture":"111", "Sugar Milling":"112",
"Sugar Refining":"113", "Confection Manufacture":"114", "Pasta Manufacture":"115",
"Prepared Meal Manufacture":"116", "Beverage Manufacture":"118", "Animal Feed Manufacture":"119",
"Ice Manufacture":"120", "Spirit Distillery":"121", "Winery":"122", "Brewing":"123",
"Soft Drink Manufacture":"124", "Tobacco Product Manufacture":"125",
"Textile Apparel and Leather Manufacture":"129", "Textile Manufacture":"130", "Apparel Manufacture":"140",
"Leather Product Manufacture":"150", "Footwear Manufacturing":"155", "Wood-based Manufacturing":"160",
"Sawmilling":"161", "Wooden Construction Product Manufacture":"165", "Paper-mill":"171", "Printing":"181",
"Petroleum and Coal Products Manufacturing":"190", "Coke Manufacture":"191", "Petroleum Refining":"192",
"Chemical Manufacture":"195", "Medicinal Product Manufacture":"210", "Rubber Product Manufacture":"221",
"Plastic Product Manufacture":"225", "Nonmetallic Mineral Product Manufacturing":"230",
"Glass Product Manufacture":"231", "Refractory Product Manufacture":"232",
"Clay Product Manufacture":"233", "Ceramic Product Manufacture":"234", "Cement Mill":"235",
"Cement Product Manufacture":"236", "Stone Product Manufacture":"237", "Primary Metal Manufacturing":"240",
"Steel Mill":"241", "Metal Refining":"242", "Foundry":"243", "Metal Product Manufacturing":"250",
"Structural Metal Product Manufacture":"251", "Munitions Manufacture":"255",
"Fabricated Metal Product Manufacture":"257", "Electronic Equipment Manufacture":"260",
"Electrical Equipment Manufacture":"270", "Propane Sale":"272", "Machinery Manufacture":"280",
"Transportation Equipment Manufacturing":"289", "Motor Vehicle Manufacture":"290",
"Ship Construction":"301", "Railway Vehicle Manufacture":"304", "Aircraft Manufacture":"305",
"Military Vehicle Manufacture":"306", "Furniture Manufacture":"310", "Miscellaneous Manufacturing":"320",
"Jewellery Manufacture":"321", "Musical Instrument Manufacture":"322", "Sports Goods Manufacture":"323",
"Game andor Toy Manufacture":"324", "Medical andor Dental Equipment Manufacture":"325",
"General Repair":"330", "Fabricated Metal Product Repair":"331", "Electronic Equipment Repair":"332",
"Electrical Equipment Repair":"333", "Machinery Repair":"334", "Ship Repair":"340",
"Aircraft Repair":"341", "Railway Vehicle Repair":"342", "Motor Vehicle Repair":"343",
"Utilities":"350", "Power Generation":"351", "Climate Control":"352", "Cooling":"355", "Heating":"356",
"Water Supply":"360", "Water Treatment":"362", "Water Distribution":"363", "Sewerage":"370",
"Sewerage Screening":"372", "Restroom":"382", "Materials Recovery":"385", "Commerce":"440",
"Wholesale Merchant":"459", "Retail Sale":"460", "Specialized Store":"464", "Non-specialized Store":"465",
"Convenience Store":"466", "Petrol Sale":"470", "Sales Yard":"473", "Precious Metal Merchant":"474",
"Market":"475", "Grocery":"476", "Pharmacy":"477", "Pet-shop":"478", "Transport":"480", "Terminal":"481",
"Station":"482", "Stop":"483", "Transfer Hub":"484", "Signalling":"486",
"Transport System Maintenance":"487", "Navigation":"488", "Transportation Hub":"489",
"Railway Transport":"490", "Railway Passenger Transport":"491", "Pedestrian Transport":"494",
"Road Transport":"495", "Road Passenger Transport":"496", "Road Freight Transport":"497",
"Pipeline Transport":"500", "Pumping":"501", "Water Transport":"505", "Inland Waters Transport":"507",
"Canal Transport":"508", "Port Control":"510", "Maritime Pilotage":"511", "Pilot Station":"512",
"Harbour Control":"513", "Air Transport":"520", "Air Traffic Control":"525",
"Transportation Support":"529", "Warehousing and Storage":"530", "Motor Vehicle Parking":"535",
"Cargo Handling":"536", "Customs Checkpoint":"537", "Inspection Station":"538", "Inspection":"539",
"Postal Activities":"540", "Mail and Package Transport":"541", "Courier Activities":"545",
"Short-term Accommodation":"548", "Long-term Accommodation":"549", "Accommodation":"550", "Hotel":"551",
"Resort Hotel":"552", "Motel":"553", "Guest-house":"554", "Hostel":"555", "Dormitory":"556",
"Vacation Cottage":"557", "Dependents Housing":"558", "Residence":"563", "Food Service":"570",
"Restaurant":"572", "Bar":"573", "Dining Hall":"574", "Banquet Hall":"578", "Convention Centre":"579",
"Publishing and Broadcasting":"580", "Print Publishing":"582", "Cinema":"594", "Radio Broadcasting":"601",
"Television Broadcasting":"604", "Telecommunications":"610", "Retail Telecommunications":"612",
"Wired Telecommunications":"614", "Main Telephone Exchange":"615", "Branch Telephone Exchange":"616",
"Wired Repeater":"617", "Wireless Telecommunications":"620", "Mobile Phone Service":"621",
"Wireless Repeater":"622", "Satellite Telecommunications":"625", "Satellite Ground Control":"626",
"Information Service":"632", "Public Records":"633", "Financial Services":"640",
"Central Banking":"642", "Retail Banking":"643", "Insurance":"651",
"Financial Market Administration":"662", "Security Brokerage":"663", "Fund Management":"671",
"Real Estate Activities":"680", "Professional, Scientific and Technical":"681", "Legal Activities":"691",
"Accounting":"696", "Head Office":"701", "Business Management":"706", "Architecture Consulting":"711",
"Engineering Design":"714", "Surveying":"717", "Scientific Research and Development":"720",
"Observation Station":"721", "Weather Station":"722", "Nuclear Research Centre":"725",
"Wind Tunnel":"730", "Advertising":"741", "Photography":"752", "Veterinary":"757",
"Business and Personal Support Services":"760", "Motor Vehicle Rental":"761", "Employment Agency":"770",
"Travel Agency":"775", "Security Enforcement":"780", "Guard":"781", "Custodial Service":"791",
"Landscaping Service":"795", "Office Administration":"801", "Call Centre":"807",
"Public Administration":"808", "Headquarters":"809", "Administration":"810", "Government":"811",
"Local Government":"812", "Subnational Government":"813", "National Government":"814", "Palace":"815",
"Capitol":"817", "Executive Activities":"818", "Legislative Activities":"819", "Polling Station":"821",
"Civil Activities":"822", "Diplomacy":"825", "Diplomatic Mission":"826", "Embassy":"827",
"Consul":"828", "Maritime Defense":"829", "Public Order, Safety and Security Services":"830",
"Public Order":"831", "Safety":"832", "Security Services":"833", "Defence Activities":"835", "Armory":"836",
"Military Reserve Activities":"837", "Military Recruitment":"838", "CBRNE Civilian Support":"839",
"Judicial Activities":"840", "Law Enforcement":"841", "Immigration Control":"842", "Imprisonment":"843",
"Juvenile Corrections":"844", "Firefighting":"845", "Rescue and Paramedical Services":"846",
"Emergency Operations":"847", "Civil Intelligence":"848", "Education":"850", "Primary Education":"851",
"Secondary Education":"852", "Higher Education":"855", "Vocational Education":"857",
"Human Health Activities":"860", "In-patient Care":"861", "Out-patient Care":"862",
"Urgent Medical Care":"863", "Human Tissue Repository":"864", "Public Health Activities":"865",
"Leprosy Care":"866", "Intermediate Care":"871", "Psychiatric In-patient Care":"873",
"Residential Care":"875", "Emergency Shelter":"881", "Homeless Shelter":"882", "Refugee Shelter":"883",
"Emergency Youth Shelter":"884", "Day Care":"885", "Social Work":"887", "Emergency Relief Services":"888",
"Cultural, Arts and Entertainment":"890", "Theatre":"891", "Auditorium":"892", "Community Centre":"893",
"Opera House":"894", "Night Club":"895", "Sports, Amusement and Recreation":"900", "Library":"902",
"Museum":"905", "Aquarium":"906", "Botanical andor Zoological Reserve Activities":"907",
"Gambling":"909", "Sports Centre":"912", "Fitness Centre":"913", "Shooting Range":"914",
"Animal Boarding":"919", "Recreation":"921", "Amusement":"922", "Hobbies and/or Leisure Activities":"923",
"Religious Activities":"930", "Place of Worship":"931", "Islamic Prayer Hall":"932",
"Membership Organization":"950", "Club":"954", "Yacht-club":"955", "Laundry":"961",
"Beauty Treatment":"962", "Funeral Services":"963", "Cremation":"964", "Mortuary Services":"965",
"Adult Entertainment":"966", "Storage of Human Remains":"967", "Meeting Place":"970",
"Death Care Services":"980", "Other":"999", "No Information":"-999999"}

def dumpBuildingFFN():
    # This is for building FFN's so we need to add an element in.
    ffnRawList["Waste Treatment and Disposal"] = '383'

    for i in sorted(ffnRawList.keys()):
        print '                           { name:"%s", value:"%s" },' % (i,ffnRawList[i])

    #Now we remove what we added - ready for the next feature
    del ffnRawList["Waste Treatment and Disposal"]
# End dumpBuildingFFN

def dumpFacilityFFN():
    # This is for Facility FFN's so we need to add a few elements
    facList = { "Mixed Farming":"15", "Hunting":19, "Forestry andor Logging":"20",
            "Silviculture":"21", "Fishing":"30", "Solid Mineral Fuel Mining":"50", 
            "Petroleum andor Gas Extraction":"60", "Metal Ore Mining":"70", 
            "Chemical Mining":"83", "Mineral Mining":"87", "Gas Oil Separation":"91", 
            "Water Collection":"361" }

    # Add the elements to the "raw" FFN list
    for i in facList:
        ffnRawList[i] = facList[i]

    # Time to print
    for i in sorted(ffnRawList.keys()):
        print '                           { name:"%s", value:"%s" },' % (i,ffnRawList[i])

    #Now we remove what we added - ready for the next feature
    for i in facList:
        del ffnRawList[i]

# End dumpFacilityFFN

# Setup the subtypeList
# This is an ESRI specific field
subtypeList = { "AERATION_BASIN":"100010", "AERIAL":"100201", "AERIAL_FARM":"100202",
"AIRCRAFT_HANGAR":"100456", "AIRCRAFT_REVETMENT":"100446", "AMPHITHEATRE":"100074",
"AMUSEMENT_PARK":"100054", "AMUSEMENT_PARK_ATTRACTION":"100053",
"ANNOTATED_LOCATION":"100476", "APRON":"100438", "AQUEDUCT":"100295",
"AQUIFER":"154640", "ARCADE":"100192", "ARCHEOLOGICAL_SITE":"100082",
"ASPHALT_LAKE":"100356", "ASTRONOMICAL_OBSERVATORY":"100111", "BARN":"100691",
"BASIN_GATE":"131206", "BILLBOARD":"121638", "BLAST_FURNACE":"100012",
"BOG":"100296", "BOREHOLE":"100696", "BOTANIC_GARDEN":"100383",
"BOUNDARY_MONUMENT":"100465", "BRIDGE":"100161", "BRIDGE_PIER":"100165",
"BRIDGE_SPAN":"100162", "BRIDGE_SUPERSTRUCTURE":"100163", "BRIDGE_TOWER":"100164",
"BRUSH":"100390", "BUCKET_ELEVATOR":"100027", "BUILDING":"100083",
"BUILDING_SUPERSTRUCTURE":"100087", "BUILT_UP_AREA":"100089", "CABLE":"100199",
"CABLEWAY":"100206", "CAIRN":"100091", "CAMP":"100041", "CAMP_SITE":"100057",
"CANAL":"100297", "CANE":"100393", "CANTONMENT_AREA":"170162", "CARAVAN_PARK":"100040",
"CART_TRACK":"100150", "CASTLE":"100128", "CASTLE_COMPLEX":"132642",
"CATALYTIC_CRACKER":"100013", "CAUSEWAY_STRUCTURE":"130381", "CAVE_CHAMBER":"154959",
"CAVE_MOUTH":"154961", "CEMETERY":"100092", "CHECKPOINT":"100039", "CISTERN":"100329",
"CLEARED_WAY":"100396", "CONSERVATION_AREA":"100417", "CONTAMINATED_REGION":"100409",
"CONTROL_POINT":"100466", "CONTROL_TOWER":"100167", "CONVEYOR":"100026",
"COOLING_FACILITY":"100697", "COOLING_TOWER":"100028", "COURTYARD":"100688",
"CRANE":"100029", "CREVASSE":"100342", "CREVICE":"100365", "CROP_LAND":"100380",
"CROSSING":"100168", "CULTURAL_CONTEXT_LOCATION":"100474", "CULVERT":"100170",
"CUT":"100366", "CUT_LINE":"192101", "DAM":"100330", "DATASET":"121591",
"DEFENSIVE_REVETMENT":"100446", "DEPRESSION":"100367", "DESERT":"100403",
"DISH_AERIAL":"100200", "DISPOSAL_SITE":"100007", "DITCH":"100298",
"DRAGON'S_TEETH":"100096", "DRIVE_IN_THEATRE":"100059", "DRY_DOCK":"100233",
"ELECTRIC_POWER_STATION":"100018", "ELEVATION_CONTOUR":"100353", "EMBANKMENT":"100368",
"ENGINEERED_EARTHWORK":"132596", "ENGINE_TEST_CELL":"100031", "ESKER":"100369",
"EXCAVATING_MACHINE":"100030", "EXTRACTION_MINE":"100001", "FACILITY":"100080",
"FAIRGROUND":"100061", "FENCE":"100098", "FERRY_CROSSING":"100172",
"FERRY_STATION":"100174", "FIRE_HYDRANT":"100086", "FIRING_RANGE":"100410",
"FISH_FARM_FACILITY":"191951", "FISH_LADDER":"100338", "FISH_WEIR":"100236",
"FLAGPOLE":"100099", "FLARE_PIPE":"100032", "FLOATING_DRY_DOCK":"100243",
"FLOOD_CONTROL_STRUCTURE":"131207", "FORD":"100302", "FOREST":"130380",
"FOREST_CLEARING":"100398", "FORTIFIED_BUILDING":"132626", "FOUNTAIN":"100303",
"FUEL_STORAGE_FACILITY":"100141", "GANTRY":"100101", "GATE":"100154",
"GAUGING_STATION":"100339", "GEOLOGIC_FAULT":"100370", "GEOTHERMAL_OUTLET":"100371",
"GLACIER":"100341", "GOLF_COURSE":"100062", "GOLF_DRIVING_RANGE":"100063",
"GRAIN_ELEVATOR":"100134", "GRAIN_STORAGE_STRUCTURE":"100133", "GRANDSTAND":"100064",
"GRASSLAND":"100387", "GREENHOUSE":"100052", "HARBOUR":"100222",
"HARDENED_AIRCRAFT_SHELTER":"100457", "HEATING_FACILITY":"100023", "HEDGEROW":"100381",
"HELIPAD":"100441", "HELIPORT":"100442", "HOLDING_PEN":"100043",
"HOP_FIELD":"100386", "HOPPER":"100033", "HULK":"100279",
"HUMMOCK":"100304", "HUT":"100103", "HYDROCARBON_PRODUCTS_FACILITY":"100015",
"HYDROCARBONS_FIELD":"100006", "ICE_CAP":"100348", "ICE_CLIFF":"100343",
"ICE_PEAK":"100344", "ICE_ROUTE":"100173", "ICE_SHELF":"100345",
"INDUSTRIAL_FARM":"100129", "INDUSTRIAL_FURNACE":"100016", "INLAND_WATERBODY":"130384",
"INSTALLATION":"100081", "INTEREST_SITE":"100117", "INTERNATIONAL_DATE_LINE":"100415",
"ISLAND":"100217", "LAND_AERODROME":"100436", "LAND_PARCEL":"100458",
"LANDSLIDE_MASS":"100379", "LAND_SUBJECT_TO_INUNDATION":"100307",
"LAND_WATER_BOUNDARY":"100212", "LAUNCH_PAD":"100443", "LIGHTHOUSE":"100253",
"LIGHT_SUPPORT_STRUCTURE":"100105", "LIGHT_VESSEL":"100256", "LIQUID_DIFFUSER":"100009",
"LOCK":"100331", "LOGGING_SITE":"100401", "LOOKOUT":"100066",
"MANUFACTURED_HOME_PARK":"133168", "MARSH":"100399", "MEMORIAL_MONUMENT":"100108",
"MILITARY_INSTALLATION":"100462", "MINEFIELD":"100097", "MINERAL_PILE":"100136",
"MINE_SHAFT_SUPERSTRUCTURE":"100003", "MISSILE_SITE":"100106", "MOAT":"100309",
"MOBILE_HOME_PARK":"133168", "MOORING_MAST":"100177", "MORAINE":"100340",
"MOTOR_VEHICLE_STATION":"100197", "MOUNTAIN_PASS":"100372",
"MUNITION_STORAGE_FACILITY":"100138", "NAMED_LOCATION":"100475",
"NATURAL_POOL":"100320", "NON_BUILDING_STRUCTURE":"100084",
"NON_SPATIAL_MD_ENT_COLLECT":"133102", "NON_WATER_WELL":"155023",
"NUCLEAR_REACTOR_CONTAINMENT":"100022", "OASIS":"100394",
"OFFICE_PARK":"100035", "OFFSHORE_CONSTRUCTION":"100272", "ORCHARD":"100384",
"OUTDOOR_THEATRE_SCREEN":"100060", "OVERHEAD_OBSTRUCTION":"100112", "PARK":"100065",
"PARKING_GARAGE":"100190", "PARTICLE_ACCELERATOR":"100110", "PENSTOCK":"100310",
"PICNIC_SITE":"100058", "PIPELINE":"100179", "PIPELINE_CROSSING_POINT":"100113",
"PLANT_NURSERY":"100382", "POLAR_ICE":"100347", "PORT":"100223",
"POWER_SUBSTATION":"100021", "PREPARED_WATERCOURSE_CROSSING":"100178",
"PUBLIC_SQUARE":"100114", "PUMPING_STATION":"100182", "PYLON":"100558",
"QANAT_SHAFT":"131749", "RACETRACK":"100069", "RADAR_STATION":"100207",
"RAILWAY":"100143", "RAILWAY_SIDETRACK":"100144", "RAILWAY_SIGNAL":"100149",
"RAILWAY_TURNTABLE":"100146", "RAILWAY_YARD":"100145", "RAMP":"100115",
"RAPIDS":"100311", "RECYCLING_SITE":"100008", "RETAIL_STAND":"100689",
"RICE_FIELD":"100313", "RIG":"100004", "RIVER":"100314",
"ROAD":"100152", "ROAD_INTERCHANGE":"100151", "ROADSIDE_REST_AREA":"100188",
"ROCK_FORMATION":"100373", "ROUNDHOUSE":"100147", "RUINS":"100116",
"RUNWAY":"100448", "SABKHA":"100318", "SALT_EVAPORATOR":"100317",
"SALT_FLAT":"100316", "SAND_DUNES":"100374", "SCOREBOARD":"121747",
"SEAPLANE_RUN":"100453", "SETTLEMENT":"100104", "SETTLING_POND":"100014",
"SEWAGE_TREATMENT_PLANT":"134665", "SHANTY_TOWN":"100118", "SHARP_CURVE":"100183",
"SHEAR_WALL":"100002", "SHED":"100088", "SHIP_ELEVATOR":"132749",
"SHIPPING_CONTAINER":"100132", "SHIPYARD":"100245", "SHOPPING_COMPLEX":"100034",
"SHORELINE_CONSTRUCTION":"100231", "SHORELINE_RAMP":"100232", "SIDEWALK":"100159",
"SKI_JUMP":"100072", "SKI_RUN":"100073", "SLUICE_GATE":"100334",
"SMALL_CRAFT_FACILITY":"100244", "SMOKESTACK":"100025", "SNAG":"100277",
"SNOW_FIELD_AND/OR_ICE_FIELD":"100349", "SOIL_SURFACE_REGION":"100358", "SOLAR_FARM":"100020",
"SOLAR_PANEL":"100019", "SPACE_FACILITY":"100126", "SPATIAL_MD_ENT_COLLECT":"133101",
"SPILLWAY":"100319", "SPORTS_GROUND":"100055", "SPOT_ELEVATION":"100355",
"STABLE":"100049", "STADIUM":"154703", "STAIR":"100191",
"STEEP_GRADE":"100185", "STEEP_TERRAIN_FACE":"100362", "STOPWAY":"100444",
"STORAGE_DEPOT":"100131", "STORAGE_TANK":"100139", "STORM_DRAIN":"100180",
"STREET_LAMP":"100195", "STREET_SIGN":"100196", "STRUCTURAL_PILE":"100271",
"SURFACE_BUNKER":"100137", "SURVEY_POINT":"177997", "SWAMP":"100400",
"SWIMMING_POOL":"100077", "TANK_FARM":"100140", "TAXIWAY":"100454",
"TEST_SITE":"100414", "TETHERED_BALLOON":"100130", "THICKET":"100388",
"TIDAL_WATER":"100218", "TOMB":"100094", "TOWER":"100122",
"TRAFFIC_LIGHT":"100194", "TRAIL":"100156", "TRAINING_SITE":"100416",
"TRANSPORTATION_BLOCK":"100171", "TRANSPORTATION_STATION":"100186",
"TRANSPORTATION_ROUTE_CHARACTERISTIC_CHANGE":"131083", 
"TRANSPORTATION_ROUTE_PROTECTION_STRUCTURE":"130921",
"TREE":"100392", "TUNDRA":"100350", "TUNNEL":"100187", "TUNNEL_MOUTH":"100176",
"UNDERGROUND_BUNKER":"100038", "UNDERGROUND_DWELLING":"100123",
"UTILITY_COVER":"100181", "VANISHING_POINT":"100315", "VEHICLE_BARRIER":"100155",
"VEHICLE_LOT":"100189", "VESSEL_LIFT":"100328", "VINEYARD":"100385",
"VOID_COLLECTION_AREA":"100473", "VOLCANIC_DYKE":"100377", "VOLCANO":"100375",
"WALL":"100124", "WASTE_HEAP":"177962", "WATER_AERODROME":"100452",
"WATERFALL":"100321", "WATER_INTAKE_TOWER":"100337",
"WATER_MEASUREMENT_LOCATION":"164755", "WATER_MILL":"100046",
"WATER_RACE":"131810", "WATER_TOWER":"100142", "WATER_TREATMENT_BED":"100299",
"WATER_WELL":"100326", "WATERWORK":"100325", "WEIGH_STATION":"100198",
"WIND_FARM":"100687", "WINDMILL":"100044", "WIND_TURBINE":"100045", "ZOO":"100078" }
# End of subtypeList

######
# Main Starts Here
# 
parser = argparse.ArgumentParser(description='Process TDSv40.csv file and build a schema')
parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
parser.add_argument('mainfile', help='The main NFDD csv file', action='store')

args = parser.parse_args()

main_csv_file = args.mainfile
# print 'Main: %s  Domain: %s\n' % (main_csv_file, domain_csv_file)

notice = """/*
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
 * @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

 ////
 // This file is automatically generated. Please do not modify the file 
 // directly.
 ////
 """

inFeature = False
inAttrib = False
inEnum = False


# Now start printing
print notice
print
print "var _global = (0, eval)('this');"
print 'if (!_global.tds)'
print '{'
print '  _global.tds = {};'
print '}'
print
print 'tds.schema = {'
print '// Schema built from %s' % (main_csv_file)
print 'getDbSchema: function()'
print '{'
print '    var schema = [' # And so it begins...
    
# Now go through the file and print it out
csvfile = openFile(main_csv_file, 'rb')
try:
    reader = csv.reader(csvfile, delimiter=',')
    # Rip off the header
    header = reader.next()

    # For the main file, we are expecting:
    # 1 = table Name
    # 2 = Field Name
    # 3 = Value / Domain name
    # 4 = junk - The "Eg?" column. wtf?
    # 5 = Datatype
    # 6 = Length
    # 7 = Measure Unit
    # 8 = Range
    for (fName,aName,aValue,junk,dType,dLength,mUnit,dRange) in reader:
        #print "fName: %s  aName: %s  aValue: %s" % (fName,aName,aValue)
        if fName == '': # Skip blank lines
            continue

        if aName == '':
            if inFeature:
                if inEnum:
                    print '                        ] // End of Enumerations ';
                    inEnum = False
                print '                     }' 
                print '                    ] // End of Columns' 
                print '          }, // End of Feature\n'

            inFeature = True # grubby but it works, could flip the loop around?
            inAttrib = False

            (geomType,tmpName) = fName.split(':')
            geomType = geomType[-1]

            tmpName = tmpName.lstrip()
            (fullName,fCode) = tmpName.split('  ')
            fullName = fullName.strip()
            fCode = fCode[2:-1] # Funky but it works

            # Try out the funky new names
            #print '        { name:"%s",' % (geomType[0] + fCode) # name = geom + FCODE
            print '        { name:"%s",' % (fullName.replace(' ','_').replace('-','_').upper() + '_' + geomType) # XXXXX_Y
            print '          fcode:"%s",' % (fCode)
            print '          desc:"%s",' % (fullName)

            # backward compatibility for a while
            #geoList = {'P':'Point', 'S':'Surface', 'C':'Curve'}
            geoList = {'P':'Point', 'S':'Area', 'C':'Line'}
            geomType = geoList[geomType]

            print '          geom:"%s",' % (geomType)
            print '          columns:[ '
            print '                     { name:"F_CODE",'
            print '                       desc:"Feature Code",'
            print '                       type:"String",'
            print '                       optional:"R",'
            #print '                       defValue:"%s",' % (fCode)
            print '                       defValue:"",'
            # print '                       minimum:"0",'
            # print '                       maximum:"5",'
            print '                       length:"5"'
            print '                     },'
            #print '                     { name:"FCSubtype",'
            #print '                       desc:"Feature Code Subtype",'
            #print '                       type:"Integer",'
            #print '                       optional:"R",'
            #print '                       defValue:"%s",' % (subtypeList[fullName.replace(' ','_').upper()])
            #print '                       defValue:"",'
            #print '                     },'
            continue

        if dType == 'Enumeration' or dType == 'CodeList':
            if inEnum:
                print '                        ] // End of Enumerations ';

            if inAttrib:
                print '                     },' 

            inAttrib = True
            inEnum = True

            print '                     { name:"%s",' % (aName);
            print '                       desc:"%s",' % (aValue);
            print '                       optional:"R",'
            if aName in ['ZI004_RCG','ZI020_IC2','ZI001_NSP','ZI001_SSY','ZI001_VSC','CPS','EQC','ETS','HZD','IC2','RCG','VDT']:
                print '                       type:"String",'
                if dLength != '' and dLength != 'unlimited':
                    print '                       length:"%s",' % (dLength) 
                #print '                       defValue:"No Information",'
                print '                       defValue:"noInformation",'
                inEnum = False
                continue

            # Interval Closure defaults to '5'
            elif aName in ['AQTC', 'AYRC', 'BPWHAC', 'BPWHBC', 'BPWSAC', 'BPWSBC','BWVCAC',
                           'BWVCBC', 'DMBC', 'DPAC', 'GSGCHC', 'GSGCLC', 'RMWC', 'SDCC',
                           'SDSC', 'SGCC', 'TSCC']:
                print '                       defValue:"5",'

            elif aName in ['ZSAX_RS0']:
                print '                       type:"String",'
                if dLength != '' and dLength != 'unlimited':
                    print '                       length:"%s",' % (dLength) 
                print '                       defValue:"U",'
                inEnum = False
                continue

            else:
                print '                       defValue:"-999999",'

            # Text Enumerations can have a length
            if dLength != '' and dLength != 'unlimited':
                print '                       length:"%s",' % (dLength) 

            print '                       type:"enumeration",';
            print '                       enumerations:[';
            continue

        if dType == '' or dType == 'Local specification':
            if aValue == 'Hierarchical Enumerants':
                # Buildings and Facilities have _different_ FFN lists
                # It is lucky that Buildings AL013 and Fortified Buildings AH055 have the same list
                if fCode == 'AL010': 
                    dumpFacilityFFN() # Bleah!
                else:
                    dumpBuildingFFN() # Ugggh! Building:AL013 & Fortified Building:AH055
                continue

            if dType == 'Local specification':
                continue
            else:
                (tName,junk) = aName.split(":")
                if tName not in ['ZI004_RCG','ZI020_IC2','ZI001_NSP','ZI001_SSY','ZI001_VSC','CPS','EQC','ETS','HZD','IC2','RCG','VDT']:
                    (eValue,eName) = aValue.split("=")
                    print '                           { name:"%s", value:"%s" },' % (eName.strip(),eValue.strip())
                continue
        
        if dType in ['Double','LongInteger']:
            if inEnum:
                print '                        ] // End of Enumerations ';

            if inAttrib:
                print '                     },' 
            
            inAttrib = True
            inEnum = False

            print '                     { name:"%s",' % (aName)
            print '                       desc:"%s",' % (aValue)
            print '                       optional:"R",'

            # Fix the type names as a workaround
            dTypeList = {'Double':'Real','LongInteger':'Integer'}
            print '                       type:"%s",' % (dTypeList[dType])

            #
            # Removed Min/Max for a while...
            #
            #if dRange != '':
            #    dRange = dRange[1:-1] # Stip the brackets off
            #    (dMin,dMax) = dRange.split("to")
            #    print '                       minimum:"%s",' % (dMin)
            #    print '                       maximum:"%s",' % (dMax)
           
            # Not printing Length. I don't think we need it
            #if dLength != '':
            #    print '                       length:"%s",' % (dLength) 
            #else:
            #    print '                       length:"1",'
            # Last so we can remove the comma from the end

            if dType == 'Double':
                print '                       defValue:"-999999.0"'
            else:
                print '                       defValue:"-999999"'

            continue

        if dType.find('Text') > -1:
            if inEnum:
                print '                        ] // End of Enumerations ';

            if inAttrib:
                print '                     },' 
            
            inAttrib = True
            inEnum = False

            print '                     { name:"%s",' % (aName)
            print '                       desc:"%s",' % (aValue)
            print '                       optional:"R",'
            print '                       type:"String",'

            if dRange != '':
                dRange = dRange[1:-1] # Strip the brackets off
                (dMin,dMax) = dRange.split("to")
                print '                       minimum:"%s",' % (dMin)
                print '                       maximum:"%s",' % (dMax)
            
            if dLength != '' and dLength != 'unlimited':
                print '                       length:"%s",' % (dLength) 

            if aName in ['ZSAX_RX4']:
                print '                       defValue:"USA",'
            else:
                print '                       defValue:"noInformation",'
                #print '                       defValue:"No Information",'


            continue
        
        # Bit of sanity checking. If this prints, we have done something wrong
        print "##### fName: %s  aName: %s  aValue: %s" % (fName,aName,aValue)
    # End of For
finally:
    csvfile.close()

if inEnum:
    print '                        ] // End of Enumerations ';
print '                     }' 
print '                    ] // End of Columns' 
#print '          } // End of Feature\n'
print '          }, // End of Feature\n'

#addMetadataFeatures()

print '    ]; // End of schema\n' # End of schema
print '    return schema; \n'
print '} // End of getDbSchema\n'
print '} // End of tds.schema\n'
print
print 'exports.getDbSchema = tds.schema.getDbSchema;'
print

# End
