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
    GeoNames rules

    These have been taken from the featureCOdes.txt file found at:
    http://download.geonames.org/export/dump/featureCodes_en.txt
*/


geonames.rules = {

    // The mapping between the GeoNames feature code and OSM+
    one2one : {
        'ADM1H':{'poi':'historical_first-order_administrative_division'}, //  historical first-order administrative division: a former first-order administrative division
        'ADM1':{'poi':'first-order_administrative_division'}, //  first-order administrative division: a primary administrative division of a country, such as a state in the United States
        'ADM2H':{'poi':'historical_second-order_administrative_division'}, //  historical second-order administrative division: a former second-order administrative division
        'ADM2':{'poi':'second-order_administrative_division'}, //  second-order administrative division: a subdivision of a first-order administrative division
        'ADM3H':{'poi':'historical_third-order_administrative_division'}, //  historical third-order administrative division: a former third-order administrative division
        'ADM3':{'poi':'third-order_administrative_division'}, //  third-order administrative division: a subdivision of a second-order administrative division
        'ADM4H':{'poi':'historical_fourth-order_administrative_division'}, //  historical fourth-order administrative division: a former fourth-order administrative division
        'ADM4':{'poi':'fourth-order_administrative_division'}, //  fourth-order administrative division: a subdivision of a third-order administrative division
        'ADM5':{'poi':'fifth-order_administrative_division'}, //  fifth-order administrative division: a subdivision of a fourth-order administrative division
        'ADMDH':{'poi':'historical_administrative_division_'}, //  historical administrative division : a former administrative division of a political entity, undifferentiated as to administrative level
        'ADMD':{'place':'administrative'}, //  administrative division: an administrative division of a country, undifferentiated as to administrative level
        'ADMF':{'building':'yes','office':'administrative'}, //  administrative facility: a government building
        'AGRC':{'landuse':'farmland'}, //  agricultural colony: a tract of land set aside for agricultural settlement
        'AGRF':{'place':'farm'}, //  agricultural facility: a building and/or tract of land used for improving agriculture
        'AIRB':{'aeroway':'aerodrome','type':'military'}, //  airbase: an area used to store supplies, provide barracks for air force personnel, hangars and runways for aircraft, and from which operations are initiated
        'AIRF':{'aeroway':'aerodrome'}, //  airfield: a place on land where aircraft land and take off; no facilities provided for the commercial handling of passengers and cargo
        'AIRH':{'aeroway':'heliport'}, //  heliport: a place where helicopters land and take off
        'AIRP':{'aeroway':'aerodrome'}, //  airport: a place where aircraft regularly land and take off, with runways, navigational aids, and major facilities for the commercial handling of passengers and cargo
        'AIRQ':{'abandoned:aeroway':'aerodrome'}, //  abandoned airfield: 
        'AIRS':{'seamark:type':'seaplane_landing_area'}, //  seaplane landing area: a place on a waterbody where floatplanes land and take off
        'AMTH':{'amenity':'theatre','theatre:type':'amphi'}, //  amphitheater: an oval or circular structure with rising tiers of seats about a stage or open space
        'AMUS':{'tourism':'theme_park'}, //  amusement park: Amusement Park are theme parks, adventure parks offering entertainment, similar to funfairs but with a fix location
        'ANCH':{'seamark:type':'anchorage'}, //  anchorage: an area where vessels may anchor
        'ANS':{'historic':'archaeological_site'}, //  ancient site: a place where archeological remains, old structures, or cultural artifacts are located
        'APNU':{'geological':'undersea_apron'}, //  apron: a gentle slope, with a generally smooth surface, particularly found around groups of islands and seamounts
        'AQC':{'landuse':'aquaculture'}, //  aquaculture facility: facility or area for the cultivation of aquatic animals and plants, especially fish, shellfish, and seaweed, in natural or controlled marine or freshwater environments; underwater agriculture
        'ARCH':{'natural':'arch'}, //  arch: a natural or man-made structure in the form of an arch
        'ARCU':{'geological':'undersea_arch'}, //  arch: a low bulge around the southeastern end of the island of Hawaii
        'AREA':{'poi':'area'}, //  area: a tract of land without homogeneous character or boundaries
        'ARRU':{'geological':'undersea_arrugado'}, //  arrugado: an area of subdued corrugations off Baja California
        'ASPH':{'natural':'asphalt_lake'}, //  asphalt lake: a small basin containing naturally occurring asphalt
        'ASTR':{'man_made':'survey_point'}, //  astronomical station: a point on the earth whose position has been determined by observations of celestial bodies
        'ASYL':{'amenity':'social_facility','social_facility:for':'mental_health'}, //  asylum: a facility where the insane are cared for and protected
        'ATHF':{'leisure':'pitch'}, //  athletic field: a tract of land used for playing team sports, and athletic track and field events
        'ATM':{'amenity':'atm'}, //  automatic teller machine: An unattended electronic machine in a public place, connected to a data system and related equipment and activated by a bank customer to obtain cash withdrawals and other banking services. 
        'ATOL':{'place':'atoll'}, //  atoll(s): a ring-shaped coral reef which has closely spaced islands on it encircling a lagoon
        'BANK':{'amenity':'bank'}, //  bank: A business establishment in which money is kept for saving or commercial purposes or is invested, supplied for loans, or exchanged. 
        'BAR':{'poi':'underwater_bar'}, //  bar: a shallow ridge or mound of coarse unconsolidated material in a stream channel, at the mouth of a stream, estuary, or lagoon and in the wave-break zone along coasts
        'BAY':{'natural':'bay'}, //  bay: a coastal indentation between two capes or headlands, larger than a cove but smaller than a gulf
        'BAYS':{'natural':'bay'}, //  bays: coastal indentations between two capes or headlands, larger than a cove but smaller than a gulf
        'BCH':{'natural':'beach'}, //  beach: a shore zone of coarse unconsolidated sediment that extends from the low-water line to the highest reach of storm waves
        'BCHS':{'natural':'beach'}, //  beaches: a shore zone of coarse unconsolidated sediment that extends from the low-water line to the highest reach of storm waves
        'BCN':{'man_made':'beacon'}, //  beacon: a fixed artificial navigation mark
        'BDG':{'bridge':'yes'}, //  bridge: a structure erected across an obstacle such as a stream, road, etc., in order to carry roads, railroads, and pedestrians across
        'BDGQ':{'destroyed:bridge':'yes'}, //  ruined bridge: a destroyed or decayed bridge which is no longer functional
        'BDLD':{'geological':'badlands'}, //  badlands: an area characterized by a maze of very closely spaced, deep, narrow, steep-sided ravines, and sharp crests and pinnacles
        'BDLU':{'geological':'undersea_borderland'}, //  borderland: a region adjacent to a continent, normally occupied by or bordering a shelf, that is highly irregular with depths well in excess of those typical of a shelf
        'BGHT':{'natural':'bight'}, //  bight(s): an open body of water forming a slight recession in a coastline
        'BKSU':{'geological':'undersea_bank'}, //  banks: elevations, typically located on a shelf, over which the depth of water is relatively shallow but sufficient for safe surface navigation
        'BLDG':{'building':'yes'}, //  building(s): a structure built for permanent use, as a house, factory, etc.
        'BLDO':{'building':'commercial'}, //  office building: commercial building where business and/or services are conducted
        'BLDR':{'natural':'boulder_field'}, //  boulder field: a high altitude or high latitude bare, flat area covered with large angular rocks
        'BLHL':{'natural':'blowhole'}, //  blowhole(s): a hole in coastal rock through which sea water is forced by a rising tide or waves and spurted through an outlet into the air
        'BLOW':{'geological':'blowout'}, //  blowout(s): a small depression in sandy terrain, caused by wind erosion
        'BNCH':{'geological':'bench'}, //  bench: a long, narrow bedrock platform bounded by steeper slopes above and below, usually overlooking a waterbody
        'BNK':{'poi':'bank_shallow_water'}, //  bank(s): an elevation, typically located on a shelf, over which the depth of water is relatively shallow but sufficient for most surface navigation
        'BNKR':{'poi':'stream_bank'}, //  stream bank: a sloping margin of a stream channel which normally confines the stream to its channel on land
        'BNKU':{'geological':'undersea_bank'}, //  bank: an elevation, typically located on a shelf, over which the depth of water is relatively shallow but sufficient for safe surface navigation
        'BNKX':{'geological':'bank'}, //  section of bank: 
        'BOG':{'wetland':'bog'}, //  bog(s): a wetland characterized by peat forming sphagnum moss, sedge, and other acid-water plants
        'BP':{'historic':'boundary_stone'}, //  boundary marker: a fixture marking a point along a boundary
        'BRKS':{'millitary':'barracks'}, //  barracks: a building for lodging military personnel
        'BRKW':{'man_made':'breakwater'}, //  breakwater: a structure erected to break the force of waves at the entrance to a harbor or port
        'BSND':{'landuse':'basin','basin':'detention'}, //  drainage basin: an area drained by a stream
        'BSNP':{'geological':'petroleum_basin'}, //  petroleum basin: an area underlain by an oil-rich structural basin
        'BSNU':{'geological':'undersea_basin'}, //  basin: a depression more or less equidimensional in plan and of variable extent
        'BSTN':{'building':'farm_auxiliary','use':'baling_station'}, //  baling station: a facility for baling agricultural products
        'BTL':{'historic':'battlefield'}, //  battlefield: a site of a land battle of historical importance
        'BTYD':{'waterway':'boatyard'}, //  boatyard: a waterside facility for servicing, repairing, and building small vessels
        'BUR':{'tomb':'cave'}, //  burial cave(s): a cave used for human burials
        'BUSH':{'natural':'bush'}, //  bush(es): a small clump of conspicuous bushes in an otherwise bare area
        'BUSTN':{'amenity':'bus_station'}, //  bus station: a facility comprising ticket office, platforms, etc. for loading and unloading passengers 
        'BUSTP':{'highway':'bus_stop'}, //  bus stop: a place lacking station facilities
        'BUTE':{'geological':'butte'}, //  butte(s): a small, isolated, usually flat-topped hill with steep sides
        'CAPE':{'natural':'cape'}, //  cape: a land area, more prominent than a point, projecting into the sea and marking a notable change in coastal direction
        'CAPG':{'natural':'glacier','gacier:type':'icecap'}, //  icecap: a dome-shaped mass of glacial ice covering an area of mountain summits or other high lands; smaller than an ice sheet
        'CARN':{'man_made':'cairn'}, //  cairn: a heap of stones erected as a landmark or for other purposes
        'CAVE':{'natural':'cave_entrance'}, //  cave(s): an underground passageway or chamber, or cavity on the side of a cliff
        'CDAU':{'geological':'undersea_cordillera'}, //  cordillera: an entire mountain system including the subordinate ranges, interior plateaus, and basins
        'CFT':{'natural':'cleft'}, //  cleft(s): a deep narrow slot, notch, or groove in a coastal cliff
        'CHNL':{'poi':'lake_channel'}, //  lake channel(s): that part of a lake having water deep enough for navigation between islands, shoals, etc.
        'CHNM':{'poi':'marine_channel'}, //  marine channel: that part of a body of water deep enough for navigation through an area otherwise not suitable
        'CHNN':{'poi':'navigation_channel'}, //  navigation channel: a buoyed channel of sufficient depth for the safe navigation of vessels
        'CHN':{'poi':'channel'}, //  channel: the deepest part of a stream, bay, lagoon, or strait, through which the main current flows
        'CH':{'building':'church'}, //  church: a building for public Christian worship
        'CLDA':{'natural':'volcano'}, //  caldera: a depression measuring kilometers across formed by the collapse of a volcanic mountain
        'CLF':{'natural':'cliff'}, //  cliff(s): a high, steep to perpendicular slope overlooking a waterbody or lower area
        'CLG':{'natural':'forest_clearing'}, //  clearing: an area in a forest with trees removed
        'CMN':{'leisure':'common'}, //  common: a park or pasture for community use
        'CMPLA':{'camp':'yes','camp:type':'labor'}, //  labor camp: a camp used by migrant or temporary laborers
        'CMPL':{'camp':'yes','camp:type':'logging'}, //  logging camp: a camp used by loggers
        'CMPMN':{'camp':'yes','camp:type':'mining'}, //  mining camp: a camp used by miners
        'CMPO':{'camp':'yes','camp:type':'oil'}, //  oil camp: a camp used by oilfield workers
        'CMP':{'tourism':'camp_site'}, //  camp(s): a site occupied by tents, huts, or other shelters for temporary use
        'CMPQ':{'abandoned:tourism':'camp_site'}, //  abandoned camp: 
        'CMPRF':{'refugee':'yes','tourism':'camp_site',}, //  refugee camp: a camp used by refugees
        'CMTY':{'landuse':'cemetery'}, //  cemetery: a burial place or ground
        'CNFL':{'poi':'confluence'}, //  confluence: a place where two or more streams or intermittent streams flow together
        'CNLA':{'bridge':'aqueduct'}, //  aqueduct: a conduit used to carry water
        'CNLB':{'poi':'canal_bend'}, //  canal bend: a conspicuously curved or bent section of a canal
        'CNLD':{'waterway':'canal','service':'drainage'}, //  drainage canal: an artificial waterway carrying water away from a wetland or from drainage ditches
        'CNLI':{'waterway':'canal','service':'irrigation'}, //  irrigation canal: a canal which serves as a main conduit for irrigation water
        'CNLN':{'waterway':'canal','service':'navigation'}, //  navigation canal(s): a watercourse constructed for navigation of vessels
        'CNL':{'waterway':'canal'}, //  canal: an artificial watercourse
        'CNLQ':{'abandoned:waterway':'canal'}, //  abandoned canal: 
        'CNLSB':{'waterway':'canal','service':'irrigation','location':'underground'}, //  underground irrigation canal(s): a gently inclined underground tunnel bringing water for irrigation from aquifers
        'CNLX':{'poi':'section_of_canal'}, //  section of canal:
        'CNS':{'poi':'concession_area'}, //  concession area: a lease of land by a government for economic development, e.g., mining, forestry
        'CNSU':{'geological':'undersea_canyons'}, //  canyons: relatively narrow, deep depressions with steep sides, the bottom of which generally has a continuous slope
        'CNYN':{'geological':'canyon'}, //  canyon: a deep, narrow valley with steep sides cutting into a plateau or mountainous area
        'CNYU':{'geological':'undersea_canyon'}, //  canyon: a relatively narrow, deep depression with steep sides, the bottom of which generally has a continuous slope
        'COLF':{'geological':'coalfield'}, //  coalfield: a region in which coal deposits of possible economic value occur
        'COMC':{'building':'communications'}, //  communication center: a facility, including buildings, antennae, towers and electronic equipment for receiving and transmitting information
        'CONE':{'geological':'cone'}, //  cone(s): a conical landform composed of mud or volcanic material
        'CONT':{'place':'continent'}, //  continent: continent: Europe, Africa, Asia, North America, South America, Oceania, Antarctica
        'COVE':{'natural':'bay'}, //  cove(s): a small coastal indentation, smaller than a bay
        'CRDR':{'natural':'corridor'}, //  corridor: a strip or area of land having significance as an access way
        'CRKT':{'waterway':'stream','tidal':'yes'}, //  tidal creek(s): a meandering channel in a coastal wetland subject to bi-directional tidal currents
        'CRNT':{'poi':'current'}, //  current: a horizontal flow of water in a given direction with uniform velocity
        'CRQ':{'natural':'glacier','glacier:type':'cirque'}, //  cirque: a bowl-like hollow partially surrounded by cliffs or steep slopes at the head of a glaciated valley
        'CRQS':{'natural':'glacier','glacier:type':'cirque'}, //  cirques: bowl-like hollows partially surrounded by cliffs or steep slopes at the head of a glaciated valley
        'CRRL':{'man_made':'corral'}, //  corral(s): a pen or enclosure for confining or capturing animals
        'CRSU':{'geological':'undersea_continental_rise'}, //  continental rise: a gentle slope rising from oceanic depths towards the foot of a continental slope
        'CRTR':{'natural':'crater'}, //  crater(s): a generally circular saucer or bowl-shaped depression caused by volcanic or meteorite explosive action
        'CSNO':{'amenity':'casino'}, //  casino: a building used for entertainment, especially gambling
        'CSTL':{'building':'castle'}, //  castle: a large fortified building or set of buildings
        'CSTM':{'amenity':'customs'}, //  customs house: a building in a port where customs and duties are paid, and where vessels are entered and cleared
        'CST':{'poi':'coast'}, //  coast: a zone of variable width straddling the shoreline
        'CSWY':{'man_made':'causeway'}, //  causeway: a raised roadway across wet ground or shallow water
        'CTHSE':{'amenity':'courthouse'}, //  courthouse: a building in which courts of law are held
        'CTRA':{'building':'nuclear_research_centre'}, //  atomic center: a facility where atomic research is carried out
        'CTRB':{'amenity':'business_centre'}, //  business center: a place where a number of businesses are located
        'CTRCM':{'amenity':'community_centre'}, //  community center: a facility for community recreation and other activities
        'CTRF':{'landuse':'facility'}, //  facility center: a place where more than one facility is situated
        'CTRM':{'amenity':'medical'}, //  medical center: a complex of health care buildings including two or more of the following: hospital, medical school, clinic, pharmacy, doctor's offices, etc.
        'CTRR':{'amenity':'religious_activities'}, //  religious center: a facility where more than one religious activity is carried out, e.g., retreat, school, monastery, worship
        'CTRS':{'amenity':'space_centre'}, //  space center: a facility for launching, tracking, or controlling satellites and space vehicles
        'CUET':{'geological':'cuesta'}, //  cuesta(s): an asymmetric ridge formed on tilted strata
        'CULT':{'cultivated':'yes'}, //  cultivated area: an area under cultivation
        'CUTF':{'poi':'cutoff'}, //  cutoff: a channel formed as a result of a stream cutting through a meander neck
        'CVNT':{'building':'convent'}, //  convent: a building where a community of nuns lives in seclusion
        'DAM':{'waterway':'dam'}, //  dam: a barrier constructed across a stream to impound water
        'DAMQ':{'destroyed:waterway':'dam'}, //  ruined dam: a destroyed or decayed dam which is no longer functional
        'DAMSB':{'waterway':'dam','location':'underground'}, //  sub-surface dam: a dam put down to bedrock in a sand river
        'DARY':{'place':'farm','crop':'dairy'}, //  dairy: a facility for the processing, sale and distribution of milk or milk products
        'DCKB':{'waterway':'dock'}, //  docking basin: a part of a harbor where ships dock
        'DCKD':{'waterway':'dock','dock':'drydock'}, //  dry dock: a dock providing support for a vessel, and means for removing the water so that the bottom of the vessel can be exposed
        'DCK':{'waterway':'dock'}, //  dock(s): a waterway between two piers, or cut into the land for the berthing of ships
        'DCKY':{'waterway':'dockyard'}, //  dockyard: a facility for servicing, building, or repairing ships
        'DEPU':{'geological':'undersea_deep'}, //  deep: a localized deep area within the confines of a larger feature, such as a trough, basin or trench
        'DEVH':{'place':'neighbourhood'}, //  housing development: a tract of land on which many houses of similar design are built according to a development plan
        'DIKE':{'man_made':'dyke'}, //  dike: an earth or stone embankment usually constructed for flood or stream control
        'DIP':{'amenity':'embassy'}, //  diplomatic facility: office, residence, or facility of a foreign government, which may include an embassy, consulate, chancery, office of charge d'affaires, or other diplomatic, economic, military, or cultural mission
        'DLTA':{'natural':'delta'}, //  delta: a flat plain formed by alluvial deposits at the mouth of a stream
        'DOMG':{'natural':'glacier','glacier:type':'icecap_dome'}, //  icecap dome: a comparatively elevated area on an icecap
        'DPOF':{'amenity':'fuel_storage'}, //  fuel depot: an area where fuel is stored
        'DPRG':{'poi':'icecap_depression'}, //  icecap depression: a comparatively depressed area on an icecap
        'DPR':{'natural':'sinkhole'}, //  depression(s): a low area surrounded by higher land and usually characterized by interior drainage
        'DSRT':{'natural':'desert'}, //  desert: a large area with little or no vegetation due to extreme environmental conditions
        'DTCHD':{'waterway':'ditch','service':'drainage'}, //  drainage ditch: a ditch which serves to drain the land
        'DTCHI':{'waterway':'ditch','service':'irrigation'}, //  irrigation ditch: a ditch which serves to distribute irrigation water
        'DTCHM':{'poi':'ditch_mouth'}, //  ditch mouth(s): an area where a drainage ditch enters a lagoon, lake or bay
        'DTCH':{'waterway':'ditch'}, //  ditch: a small artificial watercourse dug for draining or irrigating the land
        'DUNE':{'natural':'dune'}, //  dune(s): a wave form, ridge or star shape feature composed of sand
        'DVD':{'natural':'divide'}, //  divide: a line separating adjacent drainage basins
        'EDGU':{'geological':'undersea_shelf_edge'}, //  shelf edge: a line along which there is a marked increase of slope at the outer margin of a continental shelf or island shelf
        'ERG':{'natural':'desert','desert':'erg'}, //  sandy desert: an extensive tract of shifting sand and sand dunes
        'ESCU':{'geological':'undersea_escarpment'}, //  escarpment (or scarp): an elongated and comparatively steep slope separating flat or gently sloping areas
        'ESTO':{'landuse':'orchard','trees':'oil_palms'}, //  oil palm plantation: an estate specializing in the cultivation of oil palm trees
        'EST':{'landuse':'estate'}, //  estate(s): a large commercialized agricultural landholding with associated buildings and other facilities
        'ESTR':{'landuse':'orchard','trees':'rubber_trees'}, //  rubber plantation: an estate which specializes in growing and tapping rubber trees
        'ESTSG':{'crop':'sugar'}, //  sugar plantation: an estate that specializes in growing sugar cane
        'ESTT':{'landuse':'orchard','trees':'tea_plants'}, //  tea plantation: an estate which specializes in growing tea bushes
        'ESTX':{'landuse':'section_of_estate'}, //  section of estate:
        'ESTY':{'estuary':'yes'}, //  estuary: a funnel-shaped stream mouth or embayment where fresh water mixes with sea water under tidal influences
        'FAN':{'geological':'fan'}, //  fan(s): a fan-shaped wedge of coarse alluvium with apex merging with a mountain stream bed and the fan spreading out at a low angle slope onto an adjacent plain
        'FANU':{'geological':'undersea_fan'}, //  fan: a relatively smooth feature normally sloping away from the lower termination of a canyon or canyon system
        'FCL':{'landuse':'facility'}, //  facility: a building or buildings housing a center, institute, foundation, hospital, prison, mission, courthouse, etc.
        'FISH':{'leisure':'fishing'}, //  fishing area: a fishing ground, bank or area where fishermen go to catch fish
        'FJD':{'natural':'bay'}, //  fjord: a long, narrow, steep-walled, deep-water arm of the sea at high latitudes, usually along mountainous coasts
        'FJDS':{'natural':'bay'}, //  fjords: long, narrow, steep-walled, deep-water arms of the sea at high latitudes, usually along mountainous coasts
        'FLDI':{'landuse':'farmland','irrigated':'yes'}, //  irrigated field(s): a tract of level or terraced land which is irrigated
        'FLD':{'landuse':'meadow'}, //  field(s): an open as opposed to wooded area
        'FLLS':{'waterway':'waterfall'}, //  waterfall(s): a perpendicular or very steep descent of the water of a stream
        'FLLSX':{'waterway':'waterfall'}, //  section of waterfall(s): 
        'FLTM':{'wetland':'tidal_flat'}, //  mud flat(s): a relatively level area of mud either between high and low tide lines, or subject to flooding
        'FLTT':{'wetland':'tidal_flat'}, //  tidal flat(s): a large flat area of mud or sand attached to the shore and alternately covered and uncovered by the tide
        'FLTU':{'geological':'undersea_flat'}, //  flat: a small level or nearly level area
        'FNDY':{'building':'industrial','product':'metal_casting'}, //  foundry: a building or works where metal casting is carried out
        'FORD':{'ford':'yes'}, //  ford: a shallow part of a stream which can be crossed on foot or by land vehicle
        'FRM':{'place':'farm'}, //  farm: a tract of land with associated buildings devoted to agriculture
        'FRMQ':{'abandoned:place':'farm'}, //  abandoned farm: 
        'FRMS':{'place':'farm'}, //  farms: tracts of land with associated buildings devoted to agriculture
        'FRMT':{'building':'farm'}, //  farmstead: the buildings and adjacent service areas of a farm
        'FRSTF':{'natural':'fossilized_forest'}, //  fossilized forest: a forest fossilized by geologic processes and now exposed at the earth's surface
        'FRST':{'landuse':'forest'}, //  forest(s): an area dominated by tree vegetation
        'FRZU':{'geological':'undersea_fracture_zone'}, //  fracture zone: an extensive linear zone of irregular topography of the sea floor, characterized by steep-sided or asymmetrical ridges, troughs, or escarpments
        'FSR':{'natural':'volcano'}, //  fissure: a crack associated with volcanism
        'FT':{'building':'fort'}, //  fort: a defensive structure or earthworks
        'FURU':{'geological':'undersea_furrow'}, //  furrow: a closed, linear, narrow, shallow depression
        'FY':{'amenity':'ferry_terminal'}, //  ferry: a boat or other floating conveyance and terminal facilities regularly used to transport people and vehicles across a waterbody
        'GAP':{'natural':'gap'}, //  gap: a low place in a ridge, not used for transportation
        'GAPU':{'geological':'undersea_gap'}, //  gap: a narrow break in a ridge or rise
        'GASF':{'geological':'gasfield'}, //  gasfield: an area containing a subterranean store of natural gas of economic value
        'GATE':{'barrier':'gate'}, //  gate: a controlled access entrance or exit
        'GDN':{'leisure':'garden'}, //  garden(s): an enclosure for displaying selected plant or animal life
        'GHAT':{'man_made':'ghat'}, //  ghat: a set of steps leading to a river, which are of religious significance, and at their base is usually a platform for bathing
        'GHSE':{'tourism':'guest_house'}, //  guest house: a house used to provide lodging for paying guests
        'GLCR':{'natural':'glacier'}, //  glacier(s): a mass of ice, usually at high latitudes or high elevations, with sufficient thickness to flow away from the source area in lobes, tongues, or masses
        'GLYU':{'natural':'undersea_gully'}, //  gully: a small valley-like feature
        'GOSP':{'landuse':'industrial','industrial':'gas-oil_separation'}, //  gas-oil separator plant: a facility for separating gas from oil
        'GOVL':{'office':'local_government'}, //  local government office: a facility housing local governmental offices, usually a city, town, or village hall
        'GRAZ':{'landuse':'meadow','meadow':'agricultural'}, //  grazing area: an area of grasses and shrubs used for grazing
        'GRGE':{'natural':'gorge'}, //  gorge(s): a short, narrow, steep-sided section of a stream valley
        'GRSLD':{'natural':'grassland'}, //  grassland: an area dominated by grass vegetation
        'GRVC':{'landuse':'orchard','trees':'coconut_palm'}, //  coconut grove: a planting of coconut trees
        'GRVE':{'landuse':'cemetery'}, //  grave: a burial site
        'GRVO':{'landuse':'orchard','trees':'olive_trees'}, //  olive grove: a planting of olive trees
        'GRVPN':{'landuse':'orchard','trees':'pine_trees'}, //  pine grove: a planting of pine trees
        'GRVP':{'landuse':'orchard','trees':'palm_trees'}, //  palm grove: a planting of palm trees
        'GULF':{'natural':'gulf'}, //  gulf: a large recess in the coastline, larger than a bay
        'GVL':{'surface':'gravel'}, //  gravel area: an area covered with gravel
        'GYSR':{'natural':'geyser'}, //  geyser: a type of hot spring with intermittent eruptions of jets of hot water and steam
        'HBR':{'landuse':'harbor'}, //  harbor(s): a haven or space of deep water so sheltered by the adjacent land as to afford a safe anchorage for ships
        'HBRX':{'landuse':'harbor'}, //  section of harbor: 
        'HDLD':{'natural':'headland'}, //  headland: a high projection of land extending into a large body of water beyond the line of the coast
        'HERM':{'building':'hermitage'}, //  hermitage: a secluded residence, usually for religious sects
        'HLL':{'natural':'hill'}, //  hill: a rounded elevation of limited extent rising above the surrounding land with local relief of less than 300m
        'HLLS':{'natural':'hill'}, //  hills: rounded elevations of limited extent rising above the surrounding land with local relief of less than 300m
        'HLLU':{'natural':'undersea_hill'}, //  hill: an elevation rising generally less than 500 meters
        'HLSU':{'natural':'undersea_hill'}, //  hills: elevations rising generally less than 500 meters
        'HLT':{'poi':'halting_place'}, //  halting place: a place where caravans stop for rest
        'HMCK':{'natural':'hammock'}, //  hammock(s): a patch of ground, distinct from and slightly above the surrounding plain or wetland. Often occurs in groups
        'HMDA':{'natural':'desert','desert':'rock'}, //  rock desert: a relatively sand-free, high bedrock plateau in a hot desert, with or without a gravel veneer
        'HMSD':{'building':'farm'}, //  homestead: a residence, owner's or manager's, on a sheep or cattle station, woolshed, outcamp, or Aboriginal outstation, specific to Australia and New Zealand 
        'HOLU':{'geological':'undersea_hole'}, //  hole: a small depression of the sea floor
        'HSEC':{'building':'house'}, //  country house: a large house, mansion, or chateau, on a large estate
        'HSE':{'building':'house'}, //  house(s): a building used as a human habitation
        'HSPC':{'amenity':'clinic'}, //  clinic: a medical facility associated with a hospital for outpatients
        'HSPD':{'amenity':'dispensary'}, //  dispensary: a building where medical or dental aid is dispensed
        'HSPL':{'amenity':'leprosy_care'}, //  leprosarium: an asylum or hospital for lepers
        'HSP':{'building':'hospital'}, //  hospital: a building in which sick or injured, especially those confined to bed, are medically treated
        'HSTS':{'historic':'yes'}, //  historical site: a place of historical importance
        'HTH':{'natural':'heath'}, //  heath: an upland moor or sandy area dominated by low shrubby vegetation including heather
        'HTL':{'tourism':'hotel'}, //  hotel: a building providing lodging and/or meals for the public
        'HUT':{'building':'hut'}, //  hut: a small primitive house
        'HUTS':{'building':'hut'}, //  huts: small primitive houses
        'INDS':{'landuse':'industrial'}, //  industrial area: an area characterized by industrial activity
        'INLT':{'natural':'inlet'}, //  inlet: a narrow waterway extending into the land, or connecting a bay or lagoon with a larger body of water
        'INLTQ':{'poi':'former_inlet'}, //  former inlet: an inlet which has been filled in, or blocked by deposits
        'INSM':{'military':'military_installation'}, //  military installation: a facility for use of and control by armed forces
        'INTF':{'poi':'interfluve'}, //  interfluve: a relatively undissected upland between adjacent stream valleys
        'ISLET':{'place':'islet'}, //  islet: small island, bigger than rock, smaller than island.
        'ISLF':{'natural':'island','man_made':'yes'}, //  artificial island: an island created by landfill or diking and filling in a wetland, bay, or lagoon
        'ISLM':{'natural':'wetland','wetland':'mangrove'}, //  mangrove island: a mangrove swamp surrounded by a waterbody
        'ISL':{'natural':'island'}, //  island: a tract of land, smaller than a continent, surrounded by water at high water
        'ISLS':{'natural':'island'}, //  islands: tracts of land, smaller than a continent, surrounded by water at high water
        'ISLT':{'natural':'land-tied_island'}, //  land-tied island: a coastal island connected to the mainland by barrier beaches, levees or dikes
        'ISLX':{'natural':'island'}, //  section of island: 
        'ISTH':{'natural':'isthmus'}, //  isthmus: a narrow strip of land connecting two larger land masses and bordered by water
        'ITTR':{'building':'research'}, //  research institute: a facility where research is carried out
        'JTY':{'man_made':'jetty'}, //  jetty: a structure built out into the water at a river mouth or harbor entrance to regulate currents and silting
        'KNLU':{'geological':'undersea_knoll'}, //  knoll: an elevation rising generally more than 500 meters and less than 1,000 meters and of limited extent across the summit
        'KNSU':{'geological':'undersea_knoll'}, //  knolls: elevations rising generally more than 500 meters and less than 1,000 meters and of limited extent across the summits
        'KRST':{'waterway':'karst'}, //  karst area: a distinctive landscape developed on soluble rock such as limestone characterized by sinkholes, caves, disappearing streams, and underground drainage
        'LAND':{'natural':'arctic_land'}, //  arctic land: a tract of land in the Arctic
        'LAVA':{'natural':'lava'}, //  lava area: an area of solidified lava
        'LBED':{'natural':'lake_bed'}, //  lake bed(s): a dried up or drained area of a former lake
        'LCTY':{'place':'locality'}, //  locality: a minor area or place of unspecified or mixed character and indefinite boundaries
        'LDGU':{'geological':'undersea_ledge'}, //  ledge: a rocky projection or outcrop, commonly linear and near shore
        'LDNG':{'man_made':'pier'}, //  landing: a place where boats receive or discharge passengers and freight, but lacking most port facilities
        'LEPC':{'amenity':'leper_colony'}, //  leper colony: a settled area inhabited by lepers in relative isolation
        'LEV':{'man_made':'dyke'}, //  levee: a natural low embankment bordering a distributary or meandering stream; often built up artificially to control floods
        'LEVU':{'geological':'undersea_dyke'}, //  levee: an embankment bordering a canyon, valley, or seachannel
        'LGN':{'water':'lagoon'}, //  lagoon: a shallow coastal waterbody, completely or partly separated from a larger body of water by a barrier island, coral reef or other depositional feature
        'LGNS':{'water':'lagoon'}, //  lagoons: shallow coastal waterbodies, completely or partly separated from a larger body of water by a barrier island, coral reef or other depositional feature
        'LGNX':{'water':'lagoon'}, //  section of lagoon: 
        'LIBR':{'amenity':'library'}, //  library: A place in which information resources such as books are kept for reading, reference, or lending.
        'LKC':{'natural':'water','water':'lake'}, //  crater lake: a lake in a crater or caldera
        'LKI':{'natural':'water','water':'lake','intermittent':'yes'}, //  intermittent lake:
        'LKNI':{'natural':'water','water':'lake','salt':'yes','intermittent':'yes'}, //  intermittent salt lake:
        'LKN':{'natural':'water','water':'lake','salt':'yes' }, //  salt lake: an inland body of salt water with no outlet
        'LKOI':{'natural':'water','water':'oxbow','intermittent':'yes'}, //  intermittent oxbow lake:
        'LKO':{'natural':'water','water':'oxbow'}, //  oxbow lake: a crescent-shaped lake commonly found adjacent to meandering streams
        'LK':{'natural':'water','water':'lake'}, //  lake: a large inland body of standing water
        'LKSB':{'natural':'water','water':'lake','location':'underground'}, //  underground lake: a standing body of water in a cave
        'LKSC':{'natural':'water','water':'lake'}, //  crater lakes: lakes in a crater or caldera
        'LKSI':{'natural':'water','water':'lake','intermittent':'yes'}, //  intermittent lakes:
        'LKSNI':{'natural':'water','water':'lake','salt':'yes','intermittent':'yes'}, //  intermittent salt lakes:
        'LKSN':{'natural':'water','water':'lake','salt':'yes' }, //  salt lakes: inland bodies of salt water with no outlet
        'LKS':{'natural':'water','water':'lake'}, //  lakes: large inland bodies of standing water
        'LKX':{'natural':'water','water':'lake'}, //  section of lake:
        'LNDF':{'landuse':'landfill'}, //  landfill: a place for trash and garbage disposal in which the waste is buried between layers of earth to build up low-lying land
        'LOCK':{'lock':'yes'}, //  lock(s): a basin in a waterway with gates at each end by means of which vessels are passed from one water level to another
        'LTER':{'poi':'leased_area'}, //  leased area: a tract of land leased to another country, usually for military installations
        'LTHSE':{'man_made':'lighthouse'}, //  lighthouse: a distinctive structure exhibiting a major navigation light
        'MALL':{'shop':'mall'}, //  mall: A large, often enclosed shopping complex containing various stores, businesses, and restaurants usually accessible by common passageways.
        'MAR':{'leisure':'marina'}, //  marina: a harbor facility for small boats, yachts, etc.
        'MDW':{'natural':'meadow'}, //  meadow: a small, poorly drained area dominated by grassy vegetation
        'MESA':{'geological':'mesa'}, //  mesa(s): a flat-topped, isolated elevation with steep slopes on all sides, less extensive than a plateau
        'MESU':{'geological':'undersea_mesa'}, //  mesa: an isolated, extensive, flat-topped elevation on the shelf, with relatively steep sides
        'MFGB':{'building':'brewery'}, //  brewery: one or more buildings where beer is brewed
        'MFGC':{'building':'industrial','use':'cannery'}, //  cannery: a building where food items are canned
        'MFGCU':{'building':'industrial','product':'copper'}, //  copper works: a facility for processing copper ore
        'MFGLM':{'man_made':'kiln','product':'lime'}, //  limekiln: a furnace in which limestone is reduced to lime
        'MFGM':{'building':'industrial','product':'munitions'}, //  munitions plant: a factory where ammunition is made
        'MFGN':{'landuse':'salt_pond'}, //  salt evaporation ponds: diked salt ponds used in the production of solar evaporated salt
        'MFGPH':{'building':'industrial','product':'phosphate'}, //  phosphate works: a facility for producing fertilizer
        'MFG':{'building':'industrial'}, //  factory: one or more buildings where goods are manufactured, processed or fabricated
        'MFGQ':{'abandoned:building':'industrial'}, //  abandoned factory: 
        'MFGSG':{'building':'industrial','product':'sugar'}, //  sugar refinery: a facility for converting raw sugar into refined sugar
        'MGV':{'natural':'wetland','wetland':'mangrove'}, //  mangrove swamp: a tropical tidal mud flat characterized by mangrove vegetation
        'MILB':{'military':'base'}, //  military base: a place used by an army or other armed service for storing arms and supplies, and for accommodating and training troops, a base from which operations can be initiated
        'MKT':{'amenity':'marketplace'}, //  market: a place where goods are bought and sold at regular intervals
        'MLM':{'building':'industrial','use':'ore_treatment'}, //  ore treatment plant: a facility for improving the metal content of ore by concentration
        'MLO':{'building':'industrial','product':'olive_oil'}, //  olive oil mill: a mill where oil is extracted from olives
        'ML':{'building':'industrial','use':'mill'}, //  mill(s): a building housing machines for transforming, shaping, finishing, grinding, or extracting products
        'MLSG':{'building':'industrial','product':'sugar'}, //  sugar mill: a facility where sugar cane is processed into raw sugar
        'MLSGQ':{'disused:building':'industrial','disused:product':'sugar'}, //  former sugar mill: a sugar mill no longer used as a sugar mill
        'MLSW':{'building':'industrial','craft':'sawmill'}, //  sawmill: a mill where logs or lumber are sawn to specified shapes and sizes
        'MLWND':{'man_made':'windmill'}, //  windmill: a mill or water pump powered by wind
        'MLWTR':{'man_made':'water_mill'}, //  water mill: a mill powered by running water
        'MNA':{'landuse':'quarry'}, //  mining area: an area of mine sites where minerals and ores are extracted
        'MNAU':{'landuse':'mineral_mining','resource':'gold'}, //  gold mine(s): a mine where gold ore, or alluvial gold is extracted
        'MNC':{'landuse':'mineral_mining','resource':'coal'}, //  coal mine(s): a mine where coal is extracted
        'MNCR':{'landuse':'mineral_mining','resource':'chrome'}, //  chrome mine(s): a mine where chrome ore is extracted
        'MNCU':{'landuse':'mineral_mining','resource':'copper'}, //  copper mine(s): a mine where copper ore is extracted
        'MND':{'natural':'mound'}, //  mound(s): a low, isolated, rounded hill
        'MNDU':{'geological':'undersea_mound'}, //  mound: a low, isolated, rounded hill
        'MNFE':{'landuse':'mineral_mining','resource':'iron'}, //  iron mine(s): a mine where iron ore is extracted
        'MNMT':{'historic':'monument'}, //  monument: a commemorative structure or statue
        'MNN':{'landuse':'mineral_mining','resource':'salt'}, //  salt mine(s): a mine from which salt is extracted
        'MN':{'landuse':'mineral_mining'}, //  mine(s): a site where mineral ores are extracted from the ground by excavating surface pits and subterranean passages
        'MNQ':{'abandoned:landuse':'mineral_mining',}, //  abandoned mine: 
        'MNQR':{'landuse':'quarry'}, //  quarry(-ies): a surface mine where building stone or gravel and sand, etc. are extracted
        'MOLE':{'man_made':'breakwater'}, //  mole: a massive structure of masonry or large stones serving as a pier or breakwater
        'MOOR':{'natural':'moor'}, //  moor(s): an area of open ground overlaid with wet peaty soils
        'MOTU':{'geological':'undersea_moat'}, //  moat: an annular depression that may not be continuous, located at the base of many seamounts, islands, and other isolated elevations
        'MRN':{'geological':'moraine'}, //  moraine: a mound, ridge, or other accumulation of glacial till
        'MRSHN':{'natural':'wetland','wetland':'saltmarsh'}, //  salt marsh: a flat area, subject to periodic salt water inundation, dominated by grassy salt-tolerant plants
        'MRSH':{'natural':'wetland','wetland':'marsh'}, //  marsh(es): a wetland dominated by grass-like vegetation
        'MSQE':{'amenity':'place_of_worship','building':'mosque'}, //  mosque: a building for public Islamic worship
        'MSSN':{'building':'mission'}, //  mission: a place characterized by dwellings, school, church, hospital and other facilities operated by a religious group for the purpose of providing charitable services and to propagate religion
        'MSSNQ':{'abandoned:building':'abandoned_mission'}, //  abandoned mission: 
        'MSTY':{'amenity':'monastery'}, //  monastery: a building and grounds where a community of monks lives in seclusion
        'MT':{'natural':'mountain'}, //  mountain: an elevation standing high above the surrounding area with small summit area, steep slopes and local relief of 300m or more
        'MTRO':{'railway':'station','station':'subway'}, //  metro station: metro station (Underground, Tube, or Metro)
        'MTS':{'natural':'mountain'}, //  mountains: a mountain range or a group of mountains or high ridges
        'MTU':{'natural':'undersea_mountain'}, //  mountain: a well-delineated subdivision of a large and complex positive feature
        'MUS':{'tourism':'museum'}, //  museum: a building where objects of permanent interest in one or more of the arts and sciences are preserved and exhibited
        'MVA':{'military':'maneuver_area'}, //  maneuver area: a tract of land where military field exercises are carried out
        'NKM':{'poi':'meander_neck'}, //  meander neck: a narrow strip of land between the two limbs of a meander loop at its narrowest point
        'NOV':{'amenity':'place_of_worship','building':'novitiate'}, //  novitiate: a religious house or school where novices are trained
        'NRWS':{'narrow':'yes'}, //  narrows: a navigable narrow part of a bay, strait, river, etc.
        'NSY':{'landuse':'plant_nursery'}, //  nursery(-ies): a place where plants are propagated for transplanting or grafting
        'NTK':{'geological':'nunatak'}, //  nunatak: a rock or mountain peak protruding through glacial ice
        'NTKS':{'geological':'nunatak'}, //  nunataks: rocks or mountain peaks protruding through glacial ice
        'NVB':{'military':'naval_base'}, //  naval base: an area used to store supplies, provide barracks for troops and naval personnel, a port for naval vessels, and from which operations are initiated
        'OAS':{'natural':'oasis'}, //  oasis(-es): an area in a desert made productive by the availability of water
        'OBPT':{'tourism':'view_point'}, //  observation point: a wildlife or scenic observation point
        'OBS':{'landuse':'observatory'}, //  observatory: a facility equipped for observation of atmospheric or space phenomena
        'OBSR':{'landuse':'observatory','telescope:type':'radio'}, //  radio observatory: a facility equipped with an array of antennae for receiving radio waves from space
        'OCH':{'landuse':'orchard'}, //  orchard(s): a planting of fruit or nut trees
        'OCN':{'place':'ocean'}, //  ocean: one of the major divisions of the vast expanse of salt water covering part of the earth
        'OILF':{'man_made':'oilfield'}, //  oilfield: an area containing a subterranean store of petroleum of economic value
        'OILJ':{'man_made':'pipeline','pipeline:type':'oil','pipeline':'junction'}, //  oil pipeline junction: a section of an oil pipeline where two or more pipes join together
        'OILP':{'man_made':'pipeline','pipeline:type':'oil'}, //  oil pipeline: a pipeline used for transporting oil
        'OILQ':{'abandoned:man_made':'oil_well'}, //  abandoned oil well: 
        'OILR':{'landuse':'industrial','industrial':'refinery'}, //  oil refinery: a facility for converting crude oil into refined petroleum products
        'OILT':{'landuse':'tank_farm'}, //  tank farm: a tract of land occupied by large, cylindrical, metal tanks in which oil or liquid petrochemicals are stored
        'OILW':{'man_made':'oil_well'}, //  oil well: a well from which oil may be pumped
        'OPRA':{'amenity':'theatre','theatre:genre':'opera'}, //  opera house: A theater designed chiefly for the performance of operas.
        'OVF':{'poi':'overfalls'}, //  overfalls: an area of breaking waves caused by the meeting of currents or by waves moving against the current
        'PAL':{'building':'palace'}, //  palace: a large stately house, often a royal or presidential residence
        'PAN':{'natural':'pan'}, //  pan: a near-level shallow, natural depression or basin, usually containing an intermittent lake, pond, or pool
        'PANS':{'natural':'pan'}, //  pans: a near-level shallow, natural depression or basin, usually containing an intermittent lake, pond, or pool
        'PASS':{'mountain_pass':'yes'}, //  pass: a break in a mountain range or other high obstruction, used for transportation from one side to the other [See also gap]
        'PCLD':{'place':'dependent_political_entity'}, //  dependent political entity:
        'PCLF':{'place':'state'}, //  freely associated state: 
        'PCLH':{'place':'historical_political_entity'}, //  historical political entity: a former political entity
        'PCLI':{'place':'independent_political_entity'}, //  independent political entity: 
        'PCLIX':{'place':'section_of_independent_political_entity'}, //  section of independent political entity: 
        'PCL':{'place':'political_entity'}, //  political entity: 
        'PCLS':{'place':'semi-independent_political_entity'}, //  semi-independent political entity: 
        'PEAT':{'landuse':'peat_cutting'}, //  peat cutting area: an area where peat is harvested
        'PEN':{'natural':'peninsula'}, //  peninsula: an elongate area of land projecting into a body of water and nearly surrounded by water
        'PENX':{'poi':'section_of_peninsula'}, //  section of peninsula:
        'PGDA':{'amenity':'place_of_worship','building':'pagoda'}, //  pagoda: a tower-like storied structure, usually a Buddhist shrine
        'PIER':{'man_made':'pier'}, //  pier: a structure built out into navigable water on piles providing berthing for ships and recreation
        'PKLT':{'amenity':'parking'}, //  parking lot: an area used for parking vehicles
        'PK':{'natural':'peak'}, //  peak: a pointed elevation atop a mountain, ridge, or other hypsographic feature
        'PKS':{'natural':'peak'}, //  peaks: pointed elevations atop a mountain, ridge, or other hypsographic features
        'PKSU':{'natural':'undersea_peak'}, //  peaks: prominent elevations, part of a larger feature, either pointed or of very limited extent across the summit
        'PKU':{'natural':'undersea_peak'}, //  peak: a prominent elevation, part of a larger feature, either pointed or of very limited extent across the summit
        'PLAT':{'natural':'plateau'}, //  plateau: an elevated plain with steep slopes on one or more sides, and often with incised streams
        'PLATX':{'poi':'section_of_plateau'}, //  section of plateau:
        'PLDR':{'natural':'polder'}, //  polder: an area reclaimed from the sea by diking and draining
        'PLN':{'natural':'plain'}, //  plain(s): an extensive area of comparatively level to gently undulating land, lacking surface irregularities, and usually adjacent to a higher area
        'PLNU':{'natural':'undersea_plain'}, //  plain: a flat, gently sloping or nearly level region
        'PLNX':{'natural':'section_of_plain'}, //  section of plain:
        'PLTU':{'natural':'undersea_plateau'}, //  plateau: a comparatively flat-topped feature of considerable extent, dropping off abruptly on one or more sides
        'PMPO':{'man_made':'pumping_station','product':'oil'}, //  oil pumping station: a facility for pumping oil through a pipeline
        'PMPW':{'man_made':'pumping_station','product':'water'}, //  water pumping station: a facility for pumping water from a major well or through a pipeline
        'PNDI':{'water':'pond','intermittent':'yes'}, //  intermittent pond:
        'PNDNI':{'natural':'salt_pond','intermittent':'yes'}, //  intermittent salt pond(s):
        'PNDN':{'natural':'salt_pond'}, //  salt pond: a small standing body of salt water often in a marsh or swamp, usually along a seacoast
        'PND':{'natural':'water','water':'pond'}, //  pond: a small standing waterbody
        'PNDSF':{'natural':'water','water':'pond','use':'aquaculture'}, //  fishponds: ponds or enclosures in which fish are kept or raised
        'PNDSI':{'natural':'water','water':'pond','intermittent':'yes'}, //  intermittent ponds:
        'PNDSN':{'landuse':'salt_ponds'}, //  salt ponds: small standing bodies of salt water often in a marsh or swamp, usually along a seacoast
        'PNDS':{'natural':'water','water':'pond'}, //  ponds: small standing waterbodies
        'PNLU':{'geological':'undersea_rock_pinnacle'}, //  pinnacle: a high tower or spire-shaped pillar of rock or coral, alone or cresting a summit
        'POOLI':{'natural':'water','water':'pool','intermittent':'yes'}, //  intermittent pool:
        'POOL':{'natural':'water','water':'pool'}, //  pool(s): a small and comparatively still, deep part of a larger body of water such as a stream or harbor; or a small body of standing water
        'PO':{'amenity':'post_office'}, //  post office: a public building in which mail is received, sorted and distributed
        'PP':{'amenity':'police'}, //  police post: a building in which police are stationed
        'PPLA2':{'poi':'seat_of_a_second-order_administrative_division'}, //  seat of a second-order administrative division:
        'PPLA3':{'poi':'seat_of_a_third-order_administrative_division'}, //  seat of a third-order administrative division:
        'PPLA4':{'poi':'seat_of_a_fourth-order_administrative_division'}, //  seat of a fourth-order administrative division:
        'PPLA':{'poi':'seat_of_a_first-order_administrative_division'}, //  seat of a first-order administrative division: seat of a first-order administrative division (PPLC takes precedence over PPLA)
        'PPLCH':{'historic':'capital'}, //  historical capital of a political entity: a former capital of a political entity
        'PPLC':{'capital':'yes'}, //  capital of a political entity: 
        'PPLF':{'place':'village'}, //  farm village: a populated place where the population is largely engaged in agricultural activities
        'PPLG':{'poi':'seat_of_government_of_a_political_entity'}, //  seat of government of a political entity:
        'PPLH':{'historic':'place'}, //  historical populated place: a populated place that no longer exists
        'PPLL':{'place':'hamlet'}, //  populated locality: an area similar to a locality but with a small group of dwellings or other buildings
        'PPL':{'place':'populated'}, //  populated place: a city, town, village, or other agglomeration of buildings where people live and work
        'PPLQ':{'abandoned:place':'populated'}, //  abandoned populated place:
        'PPLR':{'place':'religious_community'}, //  religious populated place: a populated place whose population is largely engaged in religious occupations
        'PPLS':{'place':'populated'}, //  populated places: cities, towns, villages, or other agglomerations of buildings where people live and work
        'PPLW':{'destroyed:place':'populated'}, //  destroyed populated place: a village, town or city destroyed by a natural disaster, or by war
        'PPLX':{'place':'neighbourhood'}, //  section of populated place: 
        'PPQ':{'abandoned:amenity':'police'}, //  abandoned police post: 
        'PRKGT':{'barrier':'gate'}, //  park gate: a controlled access to a park
        'PRKHQ':{'building':'park_headquarters'}, //  park headquarters: a park administrative facility
        'PRK':{'leisure':'park'}, //  park: an area, often of forested land, maintained as a place of beauty, or for recreation
        'PRMN':{'man_made':'promenade'}, //  promenade: a place for public walking, usually along a beach front
        'PRN':{'amenity':'prison'}, //  prison: a facility for confining prisoners
        'PRNJ':{'amenity':'reformatory'}, //  reformatory: a facility for confining, training, and reforming young law offenders
        'PRNQ':{'abandoned:amenity':'prison'}, //  abandoned prison: 
        'PROM':{'natural':'promontory'}, //  promontory(-ies): a bluff or prominent hill overlooking or projecting into a lowland
        'PRSH':{'place':'parish'}, //  parish: an ecclesiastical district
        // 'PRT':{'landuse':'port'}, //  port: a place provided with terminal and transfer facilities for loading and discharging waterborne cargo or passengers, usually located in a harbor
        'PRT':{'port':'yes'}, //  port: a place provided with terminal and transfer facilities for loading and discharging waterborne cargo or passengers, usually located in a harbor
        'PRVU':{'place':'undersea_province'}, //  province: a region identifiable by a group of similar physiographic features whose characteristics are markedly in contrast with surrounding areas
        'PSH':{'power':'plant','landuse':'industrial','plant:type':'hydro'}, //  hydroelectric power station: a building where electricity is generated from water power
        'PS':{'power':'plant','landuse':'industrial'}, //  power station: a facility for generating electric power
        'PSTB':{'barrier':'border_control'}, //  border post: a post or station at an international boundary for the regulation of movement of people and goods
        'PSTC':{'amenity':'customs'}, //  customs post: a building at an international boundary where customs and duties are paid on goods
        'PSTP':{'amenity':'patrol_post'}, //  patrol post: a post from which patrols are sent out
        'PTGE':{'whitewater':'portage_way'}, //  portage: a place where boats, goods, etc., are carried overland between navigable waters
        'PT':{'natural':'point'}, //  point: a tapering piece of land projecting into a body of water, less prominent than a cape
        'PTS':{'natural':'point'}, //  points: tapering pieces of land projecting into a body of water, less prominent than a cape
        'PYR':{'tomb':'pyramid'}, //  pyramid: an ancient massive structure of square ground plan with four triangular faces meeting at a point and used for enclosing tombs
        'PYRS':{'tomb':'pyramid'}, //  pyramids: ancient massive structures of square ground plan with four triangular faces meeting at a point and used for enclosing tombs
        'QCKS':{'surface':'quicksand'}, //  quicksand: an area where loose sand with water moving through it may become unstable when heavy objects are placed at the surface, causing them to sink
        'QUAY':{'man_made':'pier'}, //  quay: a structure of solid construction along a shore or bank which provides berthing for ships and which generally provides cargo handling facilities
        'RCH':{'poi':'reach'}, //  reach: a straight section of a navigable stream or channel between two bends
        'RDA':{'historic':'road'}, //  ancient road: the remains of a road used by ancient cultures
        'RDB':{'poi':'road_bend'}, //  road bend: a conspicuously curved or bent section of a road
        'RDCR':{'juction':'roundabout'}, //  traffic circle: a road junction formed around a central circle about which traffic moves in one direction only
        'RDCUT':{'cutting':'yes'}, //  road cut: an excavation cut through a hill or ridge for a road
        'RDGB':{'natural':'ridge','ridge:type':'beach'}, //  beach ridge: a ridge of sand just inland and parallel to the beach, usually in series
        'RDGE':{'natural':'ridge'}, //  ridge(s): a long narrow elevation with steep sides, and a more or less continuous crest
        'RDGG':{'natural':'ridge','ridge:type':'icecap'}, //  icecap ridge: a linear elevation on an icecap
        'RDGU':{'natural':'undersea_ridge'}, //  ridge: a long narrow elevation with steep sides
        'RDJCT':{'junction':'yes'}, //  road junction: a place where two or more roads join
        'RD':{'highway':'road'}, //  road: an open way with improved surface for transportation of animals, people and vehicles
        'RDST':{'poi':'roadstead'}, //  roadstead: an open anchorage affording less protection than a harbor
        'RDSU':{'natural':'undersea_ridge'}, //  ridges: long narrow elevations with steep sides
        'RECG':{'leisure':'golf_course'}, //  golf course: a recreation field where golf is played
        'RECR':{'leisure':'track'}, //  racetrack: a track where races are held
        'REG':{'natural':'desert','desert':'stony'}, //  stony desert: a desert plain characterized by a surface veneer of gravel and stones
        'RESA':{'landuse':'agricultural_reserve'}, //  agricultural reserve: a tract of land reserved for agricultural reclamation and/or development
        'RESF':{'landuse':'forest_reserve'}, //  forest reserve: a forested area set aside for preservation or controlled use
        'RESH':{'landuse':'game_reserve'}, //  hunting reserve: a tract of land used primarily for hunting
        'RESN':{'leisure':'nature_reserve'}, //  nature reserve: an area reserved for the maintenance of a natural habitat
        'RES':{'landuse':'reserve'}, //  reserve: a tract of public land reserved for future use or restricted as to use
        'RESP':{'landuse':'palm_tree_reserve'}, //  palm tree reserve: an area of palm trees where use is controlled
        'REST':{'amenity':'restaurant'}, //  restaurant: A place where meals are served to the public
        'RESV':{'landuse':'reservation'}, //  reservation: a tract of land set aside for aboriginal, tribal, or native populations
        'RESW':{'leisure':'nature_reserve'}, //  wildlife reserve: a tract of public land reserved for the preservation of wildlife
        'RET':{'shop':'yes'}, //  store: a building where goods and/or services are offered for sale
        'RFC':{'natural':'reef'}, //  coral reef(s): a surface-navigation hazard composed of coral
        'RF':{'natural':'reef'}, //  reef(s): a surface-navigation hazard composed of consolidated material
        'RFSU':{'natural':'reef'}, //  reefs: surface-navigation hazards composed of consolidated material
        'RFU':{'natural':'reef'}, //  reef: a surface-navigation hazard composed of consolidated material
        'RFX':{'natural':'section_of_reef'}, //  section of reef: 
        'RGNE':{'place':'region','place:type':'economic'}, //  economic region: a region of a country established for economic development or for statistical purposes
        'RGNH':{'historic':'region'}, //  historical region: a former historic area distinguished by one or more observable physical or cultural characteristics
        'RGNL':{'place':'region','place:type':'lake'}, //  lake region: a tract of land distinguished by numerous lakes
        'RGN':{'place':'region'}, //  region: an area distinguished by one or more observable physical or cultural characteristics
        'RHSE':{'tourism':'resthouse'}, //  resthouse: a structure maintained for the rest and shelter of travelers
        'RISU':{'geological':'undersea_rise'}, //  rise: a broad elevation that rises gently, and generally smoothly, from the sea floor
        'RJCT':{'railway':'rail','junction':'yes'}, //  railroad junction: a place where two or more railroad tracks join
        'RKFL':{'natural':'rockfall'}, //  rockfall: an irregular mass of fallen rock at the base of a cliff or steep slope
        'RK':{'natural':'rock'}, //  rock: a conspicuous, isolated rocky mass
        'RKRY':{'natural':'rookery'}, //  rookery: a breeding place of a colony of birds or seals
        'RKS':{'natural':'rock'}, //  rocks: conspicuous, isolated rocky masses
        'RLG':{'historic':'religious_site'}, //  religious site: an ancient site of significant religious importance
        'RLGR':{'building':'retreat'}, //  retreat: a place of temporary seclusion, especially for religious groups
        'RNCH':{'place':'farm'}, //  ranch(es): a large farm specializing in extensive grazing of livestock
        'RNGA':{'military':'artillery_range'}, //  artillery range: a tract of land used for artillery firing practice
        'RPDS':{'waterway':'rapids'}, //  rapids: a turbulent section of a stream associated with a steep, irregular stream bed
        'RR':{'railway':'rail'}, //  railroad: a permanent twin steel-rail track on which freight and passenger cars move long distances
        'RRQ':{'railway':'abandoned'}, //  abandoned railroad: 
        'RSD':{'railway':'rail','service':'siding'}, //  railroad siding: a short track parallel to and joining the main track
        'RSGNL':{'railway':'signal'}, //  railroad signal: a signal at the entrance of a particular section of track governing the movement of trains
        'RSRT':{'leisure':'resort'}, //  resort: a specialized facility for vacation, health, or participation sports activities
        'RSTN':{'railway':'station'}, //  railroad station: a facility comprising ticket office, platforms, etc. for loading and unloading train passengers and freight
        'RSTNQ':{'disused:railway':'station'}, //  abandoned railroad station: 
        'RSTP':{'railway':'halt'}, //  railroad stop: a place lacking station facilities where trains stop to pick up and unload passengers and freight
        'RSTPQ':{'abandoned:railway':'halt'}, //  abandoned railroad stop:
        'RSVI':{'natural':'water','water':'reservoir','intermittent':'yes'}, //  intermittent reservoir:
        'RSV':{'natural':'water','water':'reservoir'}, //  reservoir(s): an artificial pond or lake
        'RSVT':{'man_made':'storage_tank','content':'water'}, //  water tank: a contained pool or tank of water at, below, or above ground level
        'RTE':{'route':'caravan'}, //  caravan route: the route taken by caravans
        'RUIN':{'ruins':'yes'}, //  ruin(s): a destroyed or decayed structure which is no longer functional
        'RVN':{'waterway':'ravine'}, //  ravine(s): a small, narrow, deep, steep-sided stream channel, smaller than a gorge
        'RYD':{'railway':'rail','service':'yard'}, //  railroad yard: a system of tracks used for the making up of trains, and switching and storing freight cars
        'SALT':{'natural':'salt_area'}, //  salt area: a shallow basin or flat where salt accumulates after periodic inundation
        'SAND':{'natural':'sand'}, //  sand area: a tract of land covered with sand
        'SBED':{'poi':'dry_stream_bed'}, //  dry stream bed: a channel formerly containing the water of a stream
        'SBKH':{'natural':'sabkha'}, //  sabkha(s): a salt flat or salt encrusted plain subject to periodic inundation from flooding or high tides
        'SCHA':{'amenity':'school','school:type':'agricultural'}, //  agricultural school: a school with a curriculum focused on agriculture
        'SCH':{'amenity':'school'}, //  school: building(s) where instruction in one or more branches of knowledge takes place
        'SCHC':{'amenity':'university'}, //  college: the grounds and buildings of an institution of higher learning
        'SCHL':{'amenity':'school','school:type':'language'}, //  language school: Language Schools & Institutions
        'SCHM':{'amenity':'school','school:type':'military'}, //  military school: a school at which military science forms the core of the curriculum
        'SCHN':{'amenity':'school','school:type':'maritime'}, //  maritime school: a school at which maritime sciences form the core of the curriculum
        'SCHT':{'amenity':'school','school:type':'technical'}, //  technical school: post-secondary school with a specifically technical or vocational curriculum
        'SCNU':{'geological':'seachannel'}, //  seachannel: a continuously sloping, elongated depression commonly found in fans or plains and customarily bordered by levees on one or two sides
        'SCRB':{'natural':'scrub'}, //  scrubland: an area of low trees, bushes, and shrubs stunted by some environmental limitation
        'SCRP':{'geological':'escarpment'}, //  escarpment: a long line of cliffs or steep slopes separating level surfaces above and below
        'SCSU':{'geological':'seachannel'}, //  seachannels: continuously sloping, elongated depressions commonly found in fans or plains and customarily bordered by levees on one or two sides
        'SDL':{'natural':'saddle'}, //  saddle: a broad, open pass crossing a ridge or between hills or mountains
        'SDLU':{'natural':'undersea_saddle'}, //  saddle: a low part, resembling in shape a saddle, in a ridge or between contiguous seamounts
        'SD':{'natural':'bay'}, //  sound: a long arm of the sea forming a channel between the mainland and an island or islands; or connecting two larger bodies of water
        'SEA':{'place':'sea'}, //  sea: a large body of salt water more or less confined by continuous land or chains of islands forming a subdivision of an ocean
        'SECP':{'building':'yes','use':'state_exam_prep_centre'}, //  State Exam Prep Centre: state exam preparation centres
        'SHFU':{'natural':'undersea_continental_shelf'}, //  shelf: a zone adjacent to a continent (or around an island) that extends from the low water line to a depth at which there is usually a marked increase of slope towards oceanic depths
        'SHLU':{'natural':'shoal'}, //  shoal: a surface-navigation hazard composed of unconsolidated material
        'SHOL':{'natural':'shoal'}, //  shoal(s): a surface-navigation hazard composed of unconsolidated material
        'SHOR':{'tidal':'yes'}, //  shore: a narrow zone bordering a waterbody which covers and uncovers at high and low water, respectively
        'SHPF':{'man_made':'sheepfold'}, //  sheepfold: a fence or wall enclosure for sheep and other small herd animals
        'SHRN':{'amenity':'place_of_worship','shrine':'yes'}, //  shrine: a structure or place memorializing a person or religious concept
        'SHSE':{'building':'warehouse'}, //  storehouse: a building for storing goods, especially provisions
        'SHSU':{'natural':'undersea_shoal'}, //  shoals: hazards to surface navigation composed of unconsolidated material
        'SHVU':{'geological':'undersea_shelf_valley'}, //  shelf valley: a valley on the shelf, generally the shoreward extension of a canyon
        'SILL':{'natural':'sill'}, //  sill: the low part of an underwater gap or saddle separating basins, including a similar feature at the mouth of a fjord
        'SILU':{'natural':'undersea_sill'}, //  sill: the low part of a gap or saddle separating basins
        'SINK':{'natural':'sinkhole'}, //  sinkhole: a small crater-shape depression in a karst area
        'SLCE':{'waterway':'flow_control'}, //  sluice: a conduit or passage for carrying off surplus water from a waterbody, usually regulated by means of a sluice gate
        'SLID':{'natural':'slide'}, //  slide: a mound of earth material, at the base of a slope and the associated scoured area
        'SLP':{'slope':'yes'}, //  slope(s): a surface with a relatively uniform slope angle
        'SLPU':{'undersea_slope':'yes'}, //  slope: the slope seaward from the shelf edge to the beginning of a continental rise or the point where there is a general reduction in slope
        'SMSU':{'geological':'seamount'}, //  seamounts: elevations rising generally more than 1,000 meters and of limited extent across the summit
        'SMU':{'geological':'seamount'}, //  seamount: an elevation rising generally more than 1,000 meters and of limited extent across the summit
        'SNOW':{'natural':'snowfield'}, //  snowfield: an area of permanent snow and ice forming the accumulation area of a glacier
        'SNTR':{'amenity':'sanatorium'}, //  sanatorium: a facility where victims of physical or mental disorders are treated
        'SPA':{'amenity':'spa'}, //  spa: a resort area usually developed around a medicinal spring
        'SPIT':{'natural':'spit'}, //  spit: a narrow, straight or curved continuation of a beach into a waterbody
        'SPLY':{'waterway':'spillway'}, //  spillway: a passage or outlet through which surplus water flows over, around or through a dam
        'SPNG':{'natural':'spring'}, //  spring(s): a place where ground water flows naturally out of the ground
        'SPNS':{'natural':'sulphur_spring'}, //  sulphur spring(s): a place where sulphur ground water flows naturally out of the ground
        'SPNT':{'natural':'hot_spring'}, //  hot spring(s): a place where hot ground water flows naturally out of the ground
        'SPRU':{'natural':'undersea_spur'}, //  spur: a subordinate elevation, ridge, or rise projecting outward from a larger feature
        'SPUR':{'natural':'spur'}, //  spur(s): a subordinate ridge projecting outward from a hill, mountain or other elevation
        'SQR':{'landuse':'square'}, //  square: a broad, open, public area near the center of a town or city
        'STBL':{'building':'stable'}, //  stable: a building for the shelter and feeding of farm animals, especially horses
        'STDM':{'leisure':'stadium'}, //  stadium: a structure with an enclosure for athletic games with tiers of seats for spectators
        'STKR':{'route':'stock'}, //  stock route: a route taken by livestock herds
        'STLMT':{'place':'settlement','settlement:type':'israeli'}, //  israeli settlement:
        'STMA':{'waterway':'river','water:morphology':'anabranch'}, //  anabranch: a diverging branch flowing out of a main stream and rejoining it downstream
        'STMB':{'waterway':'river','water:morphology':'bend'}, //  stream bend: a conspicuously curved or bent segment of a stream
        'STMC':{'waterway':'river','water:morphology':'canalized'}, //  canalized stream: a stream that has been substantially ditched, diked, or straightened
        'STMD':{'waterway':'river','water:morphology':'distributary'}, //  distributary(-ies): a branch which flows away from the main stream, as in a delta or irrigation canal
        'STMH':{'waterway':'river','water:morphology':'headwaters'}, //  headwaters: the source and upper part of a stream, including the upper drainage basin
        'STMI':{'waterway':'river','intermittent':'yes'}, //  intermittent stream:
        'STMIX':{'poi':'section_of_intermittent_stream'}, //  section of intermittent stream:
        'STMM':{'waterway':'river','water:morphology':'mouth'}, //  stream mouth(s): a place where a stream discharges into a lagoon, lake, or the sea
        'STMQ':{'waterway':'abandoned_watercourse'}, //  abandoned watercourse: a former stream or distributary no longer carrying flowing water, but still evident due to lakes, wetland, topographic or vegetation patterns
        'STMSB':{'waterway':'lost_watercourse'}, //  lost river: a surface stream that disappears into an underground channel, or dries up in an arid area
        'STMS':{'waterway':'river'}, //  streams: bodies of running water moving to a lower level in a channel on land
        'STM':{'waterway':'river'}, //  stream: a body of running water moving to a lower level in a channel on land
        'STMX':{'poi':'section_of_stream'}, //  section of stream:
        'STNB':{'building':'research'}, //  scientific research base: a scientific facility used as a base from which research is carried out or monitored
        'STNC':{'amenity':'coast_guard'}, //  coast guard station: a facility from which the coast is guarded by armed vessels
        'STNE':{'amenity':'experiment_station'}, //  experiment station: a facility for carrying out experiments
        'STNF':{'amenity':'forest_station'}, //  forest station: a collection of buildings and facilities for carrying out forest management
        'STNI':{'use':'inspection_station'}, //  inspection station: a station at which vehicles, goods, and people are inspected
        'STNM':{'man_made':'weather_station'}, //  meteorological station: a station at which weather elements are recorded
        'STNR':{'man_made':'radio_station'}, //  radio station: a facility for producing and transmitting information by radio waves
        'STNS':{'man_made':'ground_station'}, //  satellite station: a facility for tracking and communicating with orbiting satellites
        'STNW':{'man_made':'whaling_station'}, //  whaling station: a facility for butchering whales and processing train oil
        'ST':{'highway':'road'}, //  street: a paved urban thoroughfare
        'STPS':{'highway':'steps'}, //  steps: stones or slabs placed for ease in ascending or descending a steep slope
        'STRT':{'natural':'strait'}, //  strait: a relatively narrow waterway, usually narrower and less extensive than a sound, connecting two larger bodies of water
        'SWMP':{'natural':'wetland','wetland':'swamp'}, //  swamp: a wetland dominated by tree vegetation
        'SWT':{'man_made':'wastewater_plant'}, //  sewage treatment plant: facility for the processing of sewage and/or wastewater
        'SYSI':{'waterway':'canal','service':'irrigation'}, //  irrigation system: a network of ditches and one or more of the following elements: water supply, reservoir, canal, pump, well, drain, etc.
        'TAL':{'geological':'talus_slope'}, //  talus slope: a steep concave slope formed by an accumulation of loose rock fragments at the base of a cliff or steep slope
        'TERR':{'place':'territory'}, //  territory: 
        'TERU':{'geological':'undersea_terrace'}, //  terrace: a relatively flat horizontal or gently inclined surface, sometimes long and narrow, which is bounded by a steeper ascending slope on one side and by a steep descending slope on the opposite side
        'THTR':{'amenity':'theater'}, //  theater: A building, room, or outdoor structure for the presentation of plays, films, or other dramatic performances
        'TMB':{'tomb':'yes'}, //  tomb(s): a structure for interring bodies
        'TMPL':{'amenity':'place_of_worship','building':'temple'}, //  temple(s): an edifice dedicated to religious worship
        'TMSU':{'geological':'undersea_tablemounts_or_guyot'}, //  tablemounts (or guyots): seamounts having a comparatively smooth, flat top
        'TMTU':{'geological':'undersea_tablemount_or_guyot'}, //  tablemount (or guyot): a seamount having a comparatively smooth, flat top
        'TNGU':{'geological':'undersea_tongue'}, //  tongue: an elongate (tongue-like) extension of a flat sea floor into an adjacent higher feature
        'TNKD':{'landuse':'pond','use':'cattle_dipping'}, //  cattle dipping tank: a small artificial pond used for immersing cattle in chemically treated water for disease control
        'TNLC':{'waterway':'canal','tunnel':'yes'}, //  canal tunnel: a tunnel through which a canal passes
        'TNLN':{'tunnel':'yes'}, //  natural tunnel: a cave that is open at both ends
        'TNL':{'tunnel':'yes'}, //  tunnel: a subterranean passageway for transportation
        'TNLRD':{'highway':'road','tunnel':'yes'}, //  road tunnel: a tunnel through which a road passes
        'TNLRR':{'railway':'rail','tunnel':'yes'}, //  railroad tunnel: a tunnel through which a railroad passes
        'TNLS':{'tunnel':'yes'}, //  tunnels: subterranean passageways for transportation
        'TOWR':{'man_made':'tower'}, //  tower: a high conspicuous structure, typically much higher than its diameter
        'TRANT':{'amenity':'transit_terminal'}, //  transit terminal: facilities for the handling of vehicular freight and passengers
        'TRB':{'place':'tribal_area'}, //  tribal area: a tract of land used by nomadic or other tribes
        'TREE':{'natural':'tree'}, //  tree(s): a conspicuous tree used as a landmark
        'TRGD':{'natural':'interdune_trough'}, //  interdune trough(s): a long wind-swept trough between parallel longitudinal dunes
        'TRGU':{'geological':'undersea_trough'}, //  trough: a long depression of the sea floor characteristically flat bottomed and steep sided, and normally shallower than a trench
        'TRIG':{'man_made':'survey_point','survey_point:type':'triangulation'}, //  triangulation station: a point on the earth whose position has been determined by triangulation
        'TRL':{'highway':'path'}, //  trail: a path, track, or route used by pedestrians, animals, or off-road vehicles
        'TRMO':{'man_made':'oil_pipeline_terminal'}, //  oil pipeline terminal: a tank farm or loading facility at the end of an oil pipeline
        'TRNU':{'geological':'undersea_trench'}, //  trench: a long, narrow, characteristically very deep and asymmetrical depression of the sea floor, with relatively steep sides
        'TRR':{'geological':'terrace'}, //  terrace: a long, narrow alluvial platform bounded by steeper slopes above and below, usually overlooking a waterbody
        'TUND':{'natural':'tundra'}, //  tundra: a marshy, treeless, high latitude plain, dominated by mosses, lichens, and low shrub vegetation under permafrost conditions
        'TWO':{'office':'temp_work'}, //  temp work office: Temporary Work Offices
        'UNIP':{'amenity':'school','school:type':'university_preparation'}, //  university prep school: University Preparation Schools & Institutions
        'UNIV':{'amenity':'university'}, //  university: An institution for higher learning with teaching and research facilities constituting a graduate school and professional schools that award master's degrees and doctorates and an undergraduate division that awards bachelor's degrees.
        'UPLD':{'natural':'upland'}, //  upland: an extensive interior region of high land with low to moderate surface relief
        'USGE':{'poi':'united_states_government_establishment'}, //  united states government establishment: a facility operated by the United States Government in Panama
        'VALG':{'natural':'hanging_valley'}, //  hanging valley: a valley the floor of which is notably higher than the valley or shore to which it leads; most common in areas that have been glaciated
        'VAL':{'natural':'valley'}, //  valley: an elongated depression usually traversed by a stream
        'VALS':{'natural':'valleys'}, //  valleys: elongated depressions usually traversed by a stream
        'VALU':{'natural':'undersea_valley'}, //  valley: a relatively shallow, wide depression, the bottom of which usually has a continuous gradient
        'VALX':{'natural':'valley'}, //  section of valley: 
        'VETF':{'amenity':'veterinary'}, //  veterinary facility: a building or camp at which veterinary services are available
        'VIN':{'landuse':'vineyard'}, //  vineyard: a planting of grapevines
        'VINS':{'landuse':'vineyard'}, //  vineyards: plantings of grapevines
        'VLC':{'natural':'volcano'}, //  volcano: a conical elevation composed of volcanic materials with a crater at the top
        'VLSU':{'natural':'undersea_valleys'}, //  valleys: a relatively shallow, wide depression, the bottom of which usually has a continuous gradient
        'WADB':{'waterway':'river','intermittent':'yes','wadi':'yes','water:morphology':'bend'}, //  wadi bend: a conspicuously curved or bent segment of a wadi
        'WADJ':{'poi':'wadi_junction'}, //  wadi junction: a place where two or more wadies join
        'WADM':{'waterway':'river','intermittent':'yes','wadi':'yes','water:morphology':'mouth'}, //  wadi mouth: the lower terminus of a wadi where it widens into an adjoining floodplain, depression, or waterbody
        'WADS':{'waterway':'river','intermittent':'yes','wadi':'yes'}, //  wadies: valleys or ravines, bounded by relatively steep banks, which in the rainy season become watercourses; found primarily in North Africa and the Middle East
        'WAD':{'waterway':'river','intermittent':'yes','wadi':'yes'}, //  wadi: a valley or ravine, bounded by relatively steep banks, which in the rainy season becomes a watercourse; found primarily in North Africa and the Middle East
        'WADX':{'poi':'wadi_section'}, //  section of wadi:
        'WALLA':{'historic':'wall'}, //  ancient wall: the remains of a linear defensive stone structure
        'WALL':{'barrier':'wall'}, //  wall: a thick masonry structure, usually enclosing a field or building, or forming the side of a structure
        'WEIR':{'waterway':'weir'}, //  weir(s): a small dam in a stream, designed to raise the water level or to divert stream flow through a desired channel
        'WHRF':{'waterway':'dock'}, //  wharf(-ves): a structure of open rather than solid construction along a shore or a bank which provides berthing for ships and cargo-handling facilities
        'WHRL':{'water':'whirlpool'}, //  whirlpool: a turbulent, rotating movement of water in a stream
        'WLLQ':{'abandoned:man_made':'well'}, //  abandoned well: 
        'WLLS':{'man_made':'well'}, //  wells: cylindrical holes, pits, or tunnels drilled or dug down to a depth from which water, oil, or gas can be pumped or brought to the surface
        'WLL':{'man_made':'well'}, //  well: a cylindrical hole, pit, or tunnel drilled or dug down to a depth from which water, oil, or gas can be pumped or brought to the surface
        'WRCK':{'historic':'wreck'}, //  wreck: the site of the remains of a wrecked vessel
        'WTLDI':{'natural':'wetland','intermittent':'yes'}, //  intermittent wetland:
        'WTLD':{'natural':'wetland'}, //  wetland: an area subject to inundation, usually characterized by bog, marsh, or swamp vegetation
        'WTRC':{'waterway':'river'}, //  watercourse: a natural, well-defined channel produced by flowing water, or an artificial channel designed to carry flowing water
        'WTRH':{'landuse':'pond'}, //  waterhole(s): a natural hole, hollow, or small depression that contains water, used by man and animals, especially in arid areas
        'WTRW':{'man_made':'water_works'}, //  waterworks: a facility for supplying potable water through a water source and a system of pumps and filtration beds
        'ZNB':{'place':'buffer_zone'}, //  buffer zone: a zone recognized as a buffer between two nations in which military presence is minimal or absent
        'ZNF':{'place':'free_trade_zone'}, //  free trade zone: an area, usually a section of a port, where goods may be received and shipped free of customs duty and of most customs regulations
        'ZN':{'poi':'zone'}, //  zone:
        'ZOO':{'tourism':'zoo'}, //  zoo: a zoological garden or park where wild animals are kept for exhibition
    } // End of classRules

} // End of geonames.rules
