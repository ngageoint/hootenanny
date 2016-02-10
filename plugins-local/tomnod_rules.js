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
    Tomnod to OSM+ rules

    These have been taken from the JSON list provided by the tomnod folks
*/

tomnod.rules = {

    // The mapping between the Tomnod feature type and OSM+
    one2one : {
        // 'African Tulip Tree':{'poi:type':'african_tulip_tree'}, // The African Tulip Tree is another ornamental tree invading Hawaiis native forests. It can be identified by its distinctive orange flowers. 
        // 'Aircraft':{'poi:type':'aircraft'}, // Aircraft
        'Air Defense Sites':{'poi:type':'air_defense_sites'}, // Air Defense Sites
        'Airfield':{'aeroway':'aerodrome'}, // Airfield 
        'Algeria':{'poi:type':'algeria'}, // Algeria
        'Animal Pen':{'poi:type':'animal_pen'}, // Animal Pen
        // 'animal':{'poi:type':'animal'}, // animal
        'archaeological site':{'historic':'archaeological_site'}, // Outlines of ruined buildings, burial tombs or pyramids
        'Argentina':{'poi:type':'argentina'}, // Argentina
        'Australian Tree Fern':{'natural':'fern','fern:type':'australian_tree_fern'}, // Originally used as an ornamental tree fern for landscaping, this invasive is now aggressively taking over Hawaiis native forests.
        'Australia':{'poi:type':'australia'}, // Australia
        'avalanche':{'hazard_type':'avalanche'}, // avalanche
        'Belgium':{'poi:type':'belgium'}, // Belgium
        // 'Big Truck':{'poi:type':'big_truck'}, // Big Truck
        'Blocked Road/Bridge':{'highway':'road','condition':'damaged'}, // Damaged, Destroyed, or Impassible Roads/Bridges
        'block':{'poi:type':'block'}, // block of metal
        'Blue Tarp Roof':{'building':'yes','roof:material':'blue_tarp'}, // Blue Tarp Roof
        'Bosnia And Herzegovina':{'poi:type':'bosnia_and_herzegovina'}, // Bosnia And Herzegovina
        'Brazil':{'poi:type':'brazil'}, // Brazil
        'Brick Kiln':{'man_made':'kiln'}, // Brick Kiln
        'Bridge / Dam':{'poi:type':'bridge_dam'}, // Bridge / Dam
        'Bridge/Tunnel/Dam':{'poi:type':'bridge_tunnel_dam'}, // Bridge/Tunnel/Dam
        'Building':{'building':'yes'}, // Tag all man-made structures
        'Burial Site/Cemetery':{'poi:type':'burial_site_cemetery'}, // Burial Site/Cemetery
        'Burned Area':{'damage':'fire'}, // Burned Area
        'Burned Building':{'building':'yes','condition':'damaged','damage':'fire'}, // Burned Building
        'Burned Dwelling':{'building':'yes','condition':'damaged','use':'residential','damage':'fire'}, // Burned Dwelling
        'Burned Trees':{'poi:type':'burned_trees'}, // Smaller burn scars, Sometimes Scars that look linear.  
        'Burning Fire':{'poi:type':'burning_fire'}, // Burning Fire
        'Burn Scar':{'natural':'wood','damage':'fire'}, // Tag the center of all burn scars
        'Cameroon':{'poi:type':'cameroon'}, // Cameroon
        'Camp':{'poi:type':'camp'}, // Look for shadows of tents
        'campsite':{'tourism':'camp_site'}, // campsite
        'Change':{'poi:type':'change'}, // Something has changed
        'Chile':{'poi:type':'chile'}, // Chile
        'Clearing with Man-made Objects':{'poi:type':'clearing_with_man-made_objects'}, // Any small or large clearing containing man-made tools, tents, fire pits, storage containers, or infrastructure
        'Colombia':{'poi:type':'colombia'}, // Colombia
        'Commercial Building':{'building':'yes','use':'commercial'}, // Commercial Building
        'Commercial/Industrial Building':{'building':'yes','use':'commercial_industrial'}, // Commercial/Industrial Building
        'Communication Tower':{'man_made':'tower','tower:type':'communication'}, // Communication Tower
        'Communication Tower/Satellite Dish':{'poi:type':'communication_tower_satellite_dish'}, // Communication Towers or Satellite Dishes
        'Construction':{'construction':'yes'}, // Construction
        'Contains Driveway':{'poi:type':'contains_driveway'}, // Contains Driveway
        'Costa Rica':{'poi:type':'costa_rica'}, // Costa Rica
        'Crate':{'poi:type':'crate'}, // Crates next to shipping containers.  They look much smaller than shipping containers, and are usually in groups.
        'Croatia':{'poi:type':'croatia'}, // Croatia
        "Cu00f4te d''''Ivoire":{'poi:type':'cote_divoire'}, // Cu00f4te d''''Ivoire ### Need to FIx
        'Damaged Building':{'building':'yes','condition':'damaged'}, // Damaged Building
        'damaged building / roof tarp':{'building':'yes','condition':'damaged','roof:material':'tarp'}, // damaged building / roof tarp
        'Damaged Commercial':{'building':'yes','condition':'damaged','use':'commercial'}, // Any Commercial or Industrial building that shows damage.
        'Damaged Feature':{'damage':'yes'}, // Any building, road or other feature marked as damaged
        'Damaged Feature':{'damage':'yes'}, // Tag any building or road with a damage icon.  
        'Damaged Large Building':{'building':'yes','condition':'damaged'}, // Damaged Large Building/Landmark
        'Damaged Residence':{'building':'yes','condition':'damaged','use':'residential'}, // Damaged Residence
        'Damaged Road/Bridge':{'highway':'road','condition':'damaged'}, // Impassible Roads/Bridges
        // 'Dark Boat':{'poi:type':'dark_boat'}, // Dark Boat
        'debris':{'poi:type':'debris'}, // debris
        // 'Denver Bronco':{'poi:type':'denver_bronco'}, // Denver Bronco
        'Destroyed Building':{'building':'yes','condition':'destroyed'}, // Destroyed Building
        'Destroyed/Damaged Building':{'building':'yes','condition':'damaged'}, // Destroyed/Damaged Building
        'Different Roof':{'poi:type':'different_roof'}, // A building with a different roof than in the before image
        'Dirt Road':{'highway':'track'}, // Dirt Road
        'Displaced Persons Camp':{'idp:camp_type':'unspecified'}, // Displaced Persons Camp
        'Disturbed Earth':{'poi:type':'disturbed_earth'}, // A pit, hole or any signs of digging or disturbance in the earth. 
        'Dock/Pier/Shipyard':{'poi:type':'dock_pier_shipyard'}, // Dock/Pier/Shipyard
        'Double Copter':{'poi:type':'double_copter'}, // Double Copter
        'downed trees':{'poi:type':'downed_trees'}, // downed trees
        'Dwelling':{'building':'yes','use':'residential'}, // Dwelling
        'Ecuador':{'poi:type':'ecuador'}, // Ecuador
        // 'Elephant':{'poi:type':'elephant'}, // Elephant
        'England':{'poi:type':'england'}, // England
        'Expanded Building':{'poi:type':'expanded_building'}, // Any building that looks bigger/expanded in the After image
        'Factory/Power Plant':{'poi:type':'factory_power_plant'}, // Factory/Power Plant
        'Fighter Jet':{'poi:type':'fighter_jet'}, // Fighter Jet
        'Fire':{'poi:type':'fire'}, // Fire with Smoke Plume
        'Fishing Weir':{'waterway':'weir','fishing':'yes'}, // Fishing Weir
        'Flare':{'poi:type':'flare'}, // Flare
        'Flooded Area':{'flooding':'yes'}, // Flooded Area
        'Flooded Building':{'building':'yes','damage':'flooded'}, // Flooded Building
        'Flooded Commercial':{'building':'yes','use':'commercial','damage':'flooded'}, // Flooded Commercial
        'Flooded Dwelling':{'building':'yes','use':'residential','damage':'flooded'}, // Dwellings inundated with flood water
        'Flooded Industrial / Commercial':{'building':'yes','use':'industrial_commercial','damage':'flooded'}, // Flooded Industrial/Commercial
        'Flooded Industrial':{'building':'yes','use':'industrial','damage':'flooded'}, // Flooded Industrial
        'Flooded Residence':{'building':'yes','use':'residential','damage':'flooded'}, // Flooded Residence
        'Flooded Road':{'highway':'road','condition':'flooded'}, // Flooded Road
        'Flooded Vehicle':{'poi:type':'flooded_vehicle'}, // Flooded Vehicle
        'flooding':{'flooding':'yes'}, // flooding
        'France':{'poi:type':'france'}, // France
        'Gathering of People':{'poi:type':'gathering_of_people'}, // Gathering of People
        'Gatherings of People':{'poi:type':'gatherings_of_people'}, // Gatherings of people 
        'Germany':{'poi:type':'germany'}, // Germany
        'Ghana':{'poi:type':'ghana'}, // Ghana
        'Greece':{'poi:type':'greece'}, // Greece
        // 'Group of Large Vehicles':{'poi:type':'group_of_large_vehicles'}, // Tag any large vehicles grouped together.  These could be trucks, military vehicles, industrial vehicles, etc.  
        // 'Group of Military Vehicles':{'poi:type':'group_of_military_vehicles'}, // Tag any groups of large military looking vehicles.  Often they are parked in a formation, but they could be convoying or parading.  PLEASE PLACE TAGS IN THE MIDDLE OF THE GROUP.
        // 'Groups of ships':{'poi:type':'groups_of_ships'}, // Tag any groups of more than 1 ship
        // 'Helicopter':{'poi:type':'helicopter'}, // Helicopter
        // 'Hikers/Climbers':{'poi:type':'hikers_climbers'}, // Hikers/Climbers
        'Honduras':{'poi:type':'honduras'}, // Honduras
        'House':{'building':'house'}, // House
        'human activity':{'poi:type':'human_activity'}, // human activity
        'Hut without Roof':{'building':'hut','roof':'no'}, // Hut without Roof
        'Hut With Roof':{'building':'hut'}, // Hut with Roof
        'Iconic Structure':{'landmark':'yes'}, // Landmark or other important Structure
        // 'Image Artifact':{'poi:type':'image_artifact'}, // Any imperfections on the image
        'Impassible Road/Bridge':{'highway':'road','condition':'damaged'}, // Roads/Bridges that are impassible due to damage/destruction/blockage
        // 'Interesting Formations':{'poi:type':'interesting_formations'}, // things in interesting formations with Radial Symmetry 
        'Iran':{'poi:type':'iran'}, // Iran
        'Italy':{'poi:type':'italy'}, // Italy
        'Japan':{'poi:type':'japan'}, // Japan
        'Jungle Road':{'highway':'road'}, // Jungle Road
        'Korea Republic':{'poi:type':'korea_republic'}, // Korea Republic
        'Landmark/Tourist Attraction':{'landmark':'yes'}, // Tag any items that look like landmarks or tourist attractions.  Monuments, Geographic Features, Statues, Known Landmarks, etc.  
        'Landslide':{'poi:type':'landslide'}, // Landslide
        'Large Building':{'building':'yes'}, // Large Building
        'Large Mammal Tracks':{'poi:type':'large_mammal_tracks'}, // Large Mammal Tracks
        'Large Plane':{'poi:type':'large_plane'}, // Large Plane
        'Large Ship':{'poi:type':'large_ship'}, // Large Ship
        'Launch Pad':{'poi:type':'launch_pad'}, // Geometrically Shaped Clearings
        // 'Lines of People':{'poi:type':'lines_of_people'}, // Lines of People
        'looting pit':{'poi:type':'looting_pit'}, // Evidence of a looted archaeological site. A pit in the ground, perhaps with disturbed soil or vehicle tracks nearby.
        'Major Destruction':{'destruction':'yes'}, // Major Destruction
        'Man-Made Body of Water':{'wayer':'reservoir','natural':'water'}, // Man-Made Body of Water
        'Man-Made Object in trees (NOT URBAN)':{'poi:type':'man-made_object_in_trees_(not_urban)'}, // any manmade objects in the trees.  Do not tag cities or towns.
        'Market':{'amenity':'marketplace'}, // Market
        'Mexico':{'poi:type':'mexico'}, // Mexico
        'Military Facility':{'military':'facility'}, // Military Facility
        'Military installation':{'military':'installation'}, // Facilities with equipment caches, such as planes, helicopters, and vehicles parked in formation
        // 'Military Vehicle':{'military':'vehicle'}, // Military Vehicle
        'Missile':{'missile':'other'}, // Oblong, thin, truck-length objects 
        'Missile Storage':{'poi:type':'missile_storage'}, // A group of nodal launch pad looking sites
        'Missing Building':{'poi:type':'missing_building'}, // Missing Building
        'Muscles':{'historic':'ruin','mongolian_horde':'yes'}, // To crush your enemies, to see them driven before you, and to hear the lamentations of their women.
        'Netherlands':{'poi:type':'netherlands'}, // Netherlands
        'New Building':{'poi:type':'new_building'}, // Any building that looks new or was not visible in previous images. Focus on the outskirts of villages, as well as more remote, less populated, wooded or mountainous areas.
        'Nigeria':{'poi:type':'nigeria'}, // Nigeria
        'No Buildings':{'poi:type':'no_buildings'}, // No buildings in this area.
        'No Driveway':{'poi:type':'no_driveway'}, // No Driveway
        // 'Oil Slick':{'poi:type':'oil_slick'}, // Oil Slick in Water
        'Other':{'poi:type':'other'}, // other object of interest
        'Parking Lot/Garage':{'amenity':'parking'}, // Parking Lot/Garage
        'Park / Stadium':{'leisure':'park'}, // Outdoor space for recreation, sport or leisure
        // 'Partial Australian Tree Fern':{'natural':'fern','fern:type':'australian_tree_fern'}, // A Partial Australian Tree Fern is partially hidden by other vegetation or is not fully grown.
        // 'person':{'poi:type':'person'}, // person
        'Points of Interest':{'poi:type':'points_of_interest'}, // Points of Interest
        // '_':{'poi:type':'_'}, // This. ## ???
        'Port/Dock':{'landuse':'port'}, // Port/Dock
        'Portugal':{'poi:type':'portugal'}, // Portugal
        'Radars':{'poi:type':'radars'}, // Radars
        // 'Raft':{'poi:type':'raft'}, // Life Raft
        // 'Religious Site':{'amenity':'place_of_worship'}, // Religious Site
        'Religious Site':{'amenity':'place_of_worship','building':'mosque'}, // Religious Site (Mosque)
        'Remote Industrial Site':{'poi:type':'remote_industrial_site'}, // Industrial Site
        'Residential Dwelling':{'building':'yes','use':'residential'}, // houses/apartments
        'Road Block/Barricade':{'poi:type':'road_block_barricade'}, // Road Block/Barricade
        'Road':{'highway':'road'}, // Road/Trail
        'Rubble':{'poi:type':'rubble'}, // Rubble
        'Russia':{'poi:type':'russia'}, // Russia
        'Satellite Antenna':{'poi:type':'satellite_antenna'}, // Satellite Antenna
        // 'Shawarma':{'poi:type':'shawarma'}, // Delicioso
        'Ship / Boat':{'poi:type':'ship_boat'}, // Ship / Boat
        'Shipping Container':{'poi:type':'shipping_container'}, // Shipping Container
        'Shopping Center/Market':{'poi:type':'shopping_center_market'}, // Shopping Center/Market
        // 'Small Aircraft':{'poi:type':'small_aircraft'}, // Small Plane, Aircraft, or UAV
        'Small Dwelling':{'building':'yes','use':'residential'}, // Small Dwelling
        // 'Smoke Plume':{'poi:type':'smoke_plume'}, // Smoke Plume
        // 'Something Fine!':{'poi:type':'something_fine!'}, // Signs of Super San Diego Sights
        // 'Something Interesting/Artistic':{'poi:type':'something_interesting_artistic'}, // Something Interesting/Artistic
        // 'Something Weird':{'poi:type':'something_weird'}, // Something Weird
        'Spain':{'poi:type':'spain'}, // Spain
        'Sports/Recreation Facility':{'leisure':'sports_complex'}, // Football, Soccer, Cricket, Baseball, Track, Olympics, etc.
        // 'Sriracha':{'poi:type':'sriracha'}, // Muy Picante
        'Standing Water':{'poi:type':'standing_water'}, // Standing Water
        'Swimming or Sports Facility/Park':{'poi:type':'swimming_or_sports_facility_park'}, // Football, Soccer, Swimming, Tennis, etc.
        'Swimming Pool':{'leisure':'swimming_pool'}, // Swimming Pool
        'Switzerland':{'poi:type':'switzerland'}, // Switzerland
        'Tarp':{'poi:type':'tarp'}, // Tarp
        'Tarps':{'poi:type':'tarps'}, // Look for reflective tarps laid on the ground in this pyramidal pattern.  
        'Temporary Shelter':{'poi:type':'temporary_shelter'}, // Temporary Shelter (may be other colors)
        'Tennis Court':{'leisure':'tennis_court'}, // Tennis Court
        'Tent':{'building':'tent'}, // Tent
        // 'Things that Fly':{'poi:type':'things_that_fly'}, // Things that Fly
        'tracks':{'highway':'track'}, // tracks
        'Trail':{'highway':'trail'}, // Dirt track, animal path or surfaced road
        'Train/Bus Station':{'poi:type':'train_bus_station'}, // A bus stop, train station or other public transit facility
        'truck':{'poi:type':'truck'}, // truck
        // 'UAV':{'poi:type':'uav'}, // Very small aircraft
        'Unoccupied Dwelling':{'building':'yes','use':'residential','occupied':'no'}, // Unoccupied Dwelling
        'Uruguay':{'poi:type':'uruguay'}, // Uruguay
        'USA':{'poi:type':'usa'}, // USA
        // 'Vehicle':{'poi:type':'vehicle'}, // Car, truck, bus or any other vehicle
        // 'Vehicles in Formation':{'poi:type':'vehicles_in_formation'}, // Vehicles in Formation
        // 'Vehicles Parked in Formation':{'poi:type':'vehicles_parked_in_formation'}, // Vehicles Parked in Formation
        'Vehicle Tracks':{'highway':'track'}, // Tracks from a Vehicle
        'Visible Flood Damage':{'poi:type':'visible_flood_damage'}, // Visible Flood Damage
        'Waterside Debris':{'poi:type':'waterside_debris'}, // Debris Scattered onto or close to water
        'water source':{'poi:type':'water_source'}, // water source
        // 'Wreckage':{'poi:type':'wreckage'}, // airplane wreckage
    } // End of one2one

} // End of tomnod.rules
