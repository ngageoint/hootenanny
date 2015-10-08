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
    Wikimapia rules

    These have been taken from the categories via the wikimapia API
*/


wikimapia.rules = {

    // The mapping between the Wikimapia Category and OSM+
    transRules : {
        "place without photos":{'rawWmCategory':'place_without_photos'}, //  Count: 22963702
        "place without description":{'rawWmCategory':'place_without_description'}, //  Count: 14858547
        "place without category":{'rawWmCategory':'place_without_category'}, //  Count: 13606916
        "building without address":{'rawWmCategory':'building_without_address'}, //  Count: 6671102
        "place without polygon":{'rawWmCategory':'place_without_polygon'}, //  Count: 3542484
        "building":{'rawWmCategory':'building'}, //  Count: 1917432
        "home":{'rawWmCategory':'home'}, //  Count: 1898890
        "residence":{'rawWmCategory':'residence'}, //  Count: 1898890
        "house":{'rawWmCategory':'house'}, //  Count: 1898890
        "villa":{'rawWmCategory':'villa'}, //  Count: 1898890
        "dwelling":{'rawWmCategory':'dwelling'}, //  Count: 1898890
        "tenement":{'rawWmCategory':'tenement'}, //  Count: 1212206
        "apartment building":{'rawWmCategory':'apartment_building'}, //  Count: 1212206
        "apartments":{'rawWmCategory':'apartments'}, //  Count: 1212206
        "block of flats":{'rawWmCategory':'block_of_flats'}, //  Count: 1212206
        "tower block":{'rawWmCategory':'tower_block'}, //  Count: 1212206
        "place with triangular polygon":{'rawWmCategory':'place_with_triangular_polygon'}, //  Count: 1175369
        "village":{'rawWmCategory':'village'}, //  Count: 1112678
        "shopping and services":{'rawWmCategory':'shopping_and_services'}, //  Count: 1036724
        "store / shop":{'rawWmCategory':'store_/_shop'}, //  Count: 1028282
        "dining and leisure":{'rawWmCategory':'dining_and_leisure'}, //  Count: 810321
        "education":{'rawWmCategory':'education'}, //  Count: 771971
        "school":{'rawWmCategory':'school'}, //  Count: 771971
        "schoolhouse":{'rawWmCategory':'schoolhouse'}, //  Count: 771971
        "schooling":{'rawWmCategory':'schooling'}, //  Count: 771971
        "religion":{'rawWmCategory':'religion'}, //  Count: 714451
        "faith":{'rawWmCategory':'faith'}, //  Count: 714451
        "religious":{'rawWmCategory':'religious'}, //  Count: 714451
        "place of worship":{'rawWmCategory':'place_of_worship'}, //  Count: 614034
        "water":{'rawWmCategory':'water'}, //  Count: 594749
        "nonresidential building":{'rawWmCategory':'nonresidential_building'}, //  Count: 489750
        "restaurant":{'rawWmCategory':'restaurant'}, //  Count: 465340
        "eatery":{'rawWmCategory':'eatery'}, //  Count: 465340
        "administrative division":{'rawWmCategory':'administrative_division'}, //  Count: 436874
        "lake":{'rawWmCategory':'lake'}, //  Count: 428926
        "production":{'rawWmCategory':'production'}, //  Count: 395727
        "industry":{'rawWmCategory':'industry'}, //  Count: 395727
        "sports center":{'rawWmCategory':'sports_center'}, //  Count: 347537
        "sports venue":{'rawWmCategory':'sports_venue'}, //  Count: 347537
        "sports centre":{'rawWmCategory':'sports_centre'}, //  Count: 347537
        "park":{'rawWmCategory':'park'}, //  Count: 343914
        "christianity":{'rawWmCategory':'christianity'}, //  Count: 318848
        "medical":{'rawWmCategory':'medical'}, //  Count: 316473
        "hotel":{'rawWmCategory':'hotel'}, //  Count: 288783
        "motor vehicles":{'rawWmCategory':'motor_vehicles'}, //  Count: 283740
        "office building":{'rawWmCategory':'office_building'}, //  Count: 271671
        "offices":{'rawWmCategory':'offices'}, //  Count: 271671
        "pont":{'rawWmCategory':'pont'}, //  Count: 260050
        "bridge":{'rawWmCategory':'bridge'}, //  Count: 260050
        "church":{'rawWmCategory':'church'}, //  Count: 259701
        "[automobile maintenance]":{'rawWmCategory':'[automobile_maintenance]'}, //  Count: 250570
        "electric power":{'rawWmCategory':'electric_power'}, //  Count: 232202
        "burial monument/structure":{'rawWmCategory':'burial_monument/structure'}, //  Count: 228810
        "do not draw title":{'rawWmCategory':'do_not_draw_title'}, //  Count: 221314
        "place with historical importance":{'rawWmCategory':'place_with_historical_importance'}, //  Count: 220790
        "interesting place":{'rawWmCategory':'interesting_place'}, //  Count: 210694
        "cemetery":{'rawWmCategory':'cemetery'}, //  Count: 205955
        "graveyard":{'rawWmCategory':'graveyard'}, //  Count: 205955
        "graves":{'rawWmCategory':'graves'}, //  Count: 205955
        "gravesite":{'rawWmCategory':'gravesite'}, //  Count: 205955
        "neighborhood":{'rawWmCategory':'neighborhood'}, //  Count: 186988
        "neighbourhood":{'rawWmCategory':'neighbourhood'}, //  Count: 186988
        "invisible":{'rawWmCategory':'invisible'}, //  Count: 176159
        "railway":{'rawWmCategory':'railway'}, //  Count: 173698
        "railroad":{'rawWmCategory':'railroad'}, //  Count: 173698
        "public transport":{'rawWmCategory':'public_transport'}, //  Count: 170298
        "military":{'rawWmCategory':'military'}, //  Count: 165392
        "islam":{'rawWmCategory':'islam'}, //  Count: 163004
        "muslim":{'rawWmCategory':'muslim'}, //  Count: 163004
        "petrol / gas station":{'rawWmCategory':'petrol_/_gas_station'}, //  Count: 162428
        "gasbar":{'rawWmCategory':'gasbar'}, //  Count: 162428
        "filling station":{'rawWmCategory':'filling_station'}, //  Count: 162428
        "fueling station":{'rawWmCategory':'fueling_station'}, //  Count: 162428
        "temple":{'rawWmCategory':'temple'}, //  Count: 160515
        "financial services":{'rawWmCategory':'financial_services'}, //  Count: 155041
        "city":{'rawWmCategory':'city'}, //  Count: 153051
        "mosque":{'rawWmCategory':'mosque'}, //  Count: 149014
        "masjid":{'rawWmCategory':'masjid'}, //  Count: 149014
        "hospital":{'rawWmCategory':'hospital'}, //  Count: 145644
        "sport court":{'rawWmCategory':'sport_court'}, //  Count: 131193
        "pitch (sports field / ground)":{'rawWmCategory':'pitch_(sports_field_/_ground)'}, //  Count: 131193
        "sports ground":{'rawWmCategory':'sports_ground'}, //  Count: 131193
        "draw only border":{'rawWmCategory':'draw_only_border'}, //  Count: 116226
        "agriculture":{'rawWmCategory':'agriculture'}, //  Count: 114935
        "mountain":{'rawWmCategory':'mountain'}, //  Count: 111493
        "mount":{'rawWmCategory':'mount'}, //  Count: 111493
        "power transmission line":{'rawWmCategory':'power_transmission_line'}, //  Count: 109333
        "bank":{'rawWmCategory':'bank'}, //  Count: 108073
        "savings bank":{'rawWmCategory':'savings_bank'}, //  Count: 108073
        "pond":{'rawWmCategory':'pond'}, //  Count: 104459
        "island":{'rawWmCategory':'island'}, //  Count: 104294
        "key (island)":{'rawWmCategory':'key_(island)'}, //  Count: 104294
        "isle":{'rawWmCategory':'isle'}, //  Count: 104294
        "islet":{'rawWmCategory':'islet'}, //  Count: 104294
        "cay":{'rawWmCategory':'cay'}, //  Count: 104294
        "islands":{'rawWmCategory':'islands'}, //  Count: 104294
        "aviation":{'rawWmCategory':'aviation'}, //  Count: 103024
        "electricity pylon":{'rawWmCategory':'electricity_pylon'}, //  Count: 101862
        "transmission tower (electric / electricity)":{'rawWmCategory':'transmission_tower_(electric_/_electricity)'}, //  Count: 101862
        "hydro tower":{'rawWmCategory':'hydro_tower'}, //  Count: 101862
        "bazaar":{'rawWmCategory':'bazaar'}, //  Count: 97500
        "indoor market":{'rawWmCategory':'indoor_market'}, //  Count: 97500
        "souk":{'rawWmCategory':'souk'}, //  Count: 97500
        "marketplace":{'rawWmCategory':'marketplace'}, //  Count: 97500
        "chowk (marketplace)":{'rawWmCategory':'chowk_(marketplace)'}, //  Count: 97500
        "market square":{'rawWmCategory':'market_square'}, //  Count: 97500
        "stadium":{'rawWmCategory':'stadium'}, //  Count: 89455
        "bus stop":{'rawWmCategory':'bus_stop'}, //  Count: 88819
        "bus stand":{'rawWmCategory':'bus_stand'}, //  Count: 88819
        "law enforcement":{'rawWmCategory':'law_enforcement'}, //  Count: 82748
        "bar":{'rawWmCategory':'bar'}, //  Count: 82675
        "cocktail lounge":{'rawWmCategory':'cocktail_lounge'}, //  Count: 82675
        "saloon (bar)":{'rawWmCategory':'saloon_(bar)'}, //  Count: 82675
        "commercial building":{'rawWmCategory':'commercial_building'}, //  Count: 82373
        "[constructions by dates]":{'rawWmCategory':'[constructions_by_dates]'}, //  Count: 81190
        "train station":{'rawWmCategory':'train_station'}, //  Count: 76868
        "railroad station":{'rawWmCategory':'railroad_station'}, //  Count: 76868
        "railway halt":{'rawWmCategory':'railway_halt'}, //  Count: 76868
        "railway station":{'rawWmCategory':'railway_station'}, //  Count: 76868
        "region":{'rawWmCategory':'region'}, //  Count: 76675
        "regions":{'rawWmCategory':'regions'}, //  Count: 76675
        "university":{'rawWmCategory':'university'}, //  Count: 75637
        "parking":{'rawWmCategory':'parking'}, //  Count: 75437
        "water supply network":{'rawWmCategory':'water_supply_network'}, //  Count: 71065
        "police station":{'rawWmCategory':'police_station'}, //  Count: 70419
        "residential neighborhood":{'rawWmCategory':'residential_neighborhood'}, //  Count: 70223
        "residential area":{'rawWmCategory':'residential_area'}, //  Count: 70223
        "residential neighbourhood":{'rawWmCategory':'residential_neighbourhood'}, //  Count: 70223
        "drugstore":{'rawWmCategory':'drugstore'}, //  Count: 69933
        "pharmacy":{'rawWmCategory':'pharmacy'}, //  Count: 69933
        "medical dispensary":{'rawWmCategory':'medical_dispensary'}, //  Count: 69933
        "medical store / shop":{'rawWmCategory':'medical_store_/_shop'}, //  Count: 69933
        "chemist":{'rawWmCategory':'chemist'}, //  Count: 69933
        "garden":{'rawWmCategory':'garden'}, //  Count: 69078
        "abandoned / shut down / defunct":{'rawWmCategory':'abandoned_/_shut_down_/_defunct'}, //  Count: 66928
        "cafe":{'rawWmCategory':'cafe'}, //  Count: 65767
        "museum":{'rawWmCategory':'museum'}, //  Count: 65518
        "apartment complex":{'rawWmCategory':'apartment_complex'}, //  Count: 63638
        "housing complex":{'rawWmCategory':'housing_complex'}, //  Count: 63638
        "housing / apartment colony":{'rawWmCategory':'housing_/_apartment_colony'}, //  Count: 63638
        "listed building / architectural heritage":{'rawWmCategory':'listed_building_/_architectural_heritage'}, //  Count: 63379
        "urotshistshe (stow)":{'rawWmCategory':'urotshistshe_(stow)'}, //  Count: 63368
        "nursery school":{'rawWmCategory':'nursery_school'}, //  Count: 62531
        "kindergarten":{'rawWmCategory':'kindergarten'}, //  Count: 62531
        "preschool education":{'rawWmCategory':'preschool_education'}, //  Count: 62531
        "playgroup":{'rawWmCategory':'playgroup'}, //  Count: 62531
        "pre-school":{'rawWmCategory':'pre-school'}, //  Count: 62531
        "playschool":{'rawWmCategory':'playschool'}, //  Count: 62531
        "supermarket":{'rawWmCategory':'supermarket'}, //  Count: 62404
        "monument":{'rawWmCategory':'monument'}, //  Count: 60409
        "historical layer / disappeared object":{'rawWmCategory':'historical_layer_/_disappeared_object'}, //  Count: 60345
        "electrical sub-station":{'rawWmCategory':'electrical_sub-station'}, //  Count: 57539
        "electrical substation":{'rawWmCategory':'electrical_substation'}, //  Count: 57539
        "electrical grid station":{'rawWmCategory':'electrical_grid_station'}, //  Count: 57539
        "electrical converter station":{'rawWmCategory':'electrical_converter_station'}, //  Count: 57539
        "primary education":{'rawWmCategory':'primary_education'}, //  Count: 57444
        "elementary school":{'rawWmCategory':'elementary_school'}, //  Count: 57444
        "junior school":{'rawWmCategory':'junior_school'}, //  Count: 57444
        "primary school":{'rawWmCategory':'primary_school'}, //  Count: 57444
        "road bridge":{'rawWmCategory':'road_bridge'}, //  Count: 56124
        "accommodation bridge":{'rawWmCategory':'accommodation_bridge'}, //  Count: 56124
        "professional services":{'rawWmCategory':'professional_services'}, //  Count: 55893
        "fenced area":{'rawWmCategory':'fenced_area'}, //  Count: 54857
        "grocery / grocery store":{'rawWmCategory':'grocery_/_grocery_store'}, //  Count: 52808
        "fort":{'rawWmCategory':'fort'}, //  Count: 52679
        "fortress":{'rawWmCategory':'fortress'}, //  Count: 52679
        "forts":{'rawWmCategory':'forts'}, //  Count: 52679
        "fortification":{'rawWmCategory':'fortification'}, //  Count: 52679
        "ruins":{'rawWmCategory':'ruins'}, //  Count: 52159
        "takeaway":{'rawWmCategory':'takeaway'}, //  Count: 51993
        "carryout":{'rawWmCategory':'carryout'}, //  Count: 51993
        "take away / take out / carry out food":{'rawWmCategory':'take_away_/_take_out_/_carry_out_food'}, //  Count: 51993
        "takeout":{'rawWmCategory':'takeout'}, //  Count: 51993
        "post office":{'rawWmCategory':'post_office'}, //  Count: 50312
        "farm":{'rawWmCategory':'farm'}, //  Count: 49882
        "farming":{'rawWmCategory':'farming'}, //  Count: 49882
        "square":{'rawWmCategory':'square'}, //  Count: 49635
        "car repair/ maintenance service":{'rawWmCategory':'car_repair/_maintenance_service'}, //  Count: 49448
        "automobile repair shop":{'rawWmCategory':'automobile_repair_shop'}, //  Count: 49448
        "garage (automobile repair)":{'rawWmCategory':'garage_(automobile_repair)'}, //  Count: 49448
        "car park":{'rawWmCategory':'car_park'}, //  Count: 49028
        "parking lot":{'rawWmCategory':'parking_lot'}, //  Count: 49028
        "lorry park":{'rawWmCategory':'lorry_park'}, //  Count: 49028
        "summer cottage area":{'rawWmCategory':'summer_cottage_area'}, //  Count: 48183
        "fast food restaurant":{'rawWmCategory':'fast_food_restaurant'}, //  Count: 48011
        "warehouse":{'rawWmCategory':'warehouse'}, //  Count: 47836
        "secondary education":{'rawWmCategory':'secondary_education'}, //  Count: 47012
        "high school":{'rawWmCategory':'high_school'}, //  Count: 47012
        "gymnasium - education":{'rawWmCategory':'gymnasium_-_education'}, //  Count: 47012
        "university-preparatory school":{'rawWmCategory':'university-preparatory_school'}, //  Count: 47012
        "secondary school":{'rawWmCategory':'secondary_school'}, //  Count: 47012
        "road":{'rawWmCategory':'road'}, //  Count: 45153
        "beach":{'rawWmCategory':'beach'}, //  Count: 44915
        "children's play area":{'rawWmCategory':'childrens_play_area'}, //  Count: 43646
        "playground":{'rawWmCategory':'playground'}, //  Count: 43646
        "playscape":{'rawWmCategory':'playscape'}, //  Count: 43646
        "play area":{'rawWmCategory':'play_area'}, //  Count: 43646
        "swimming pool":{'rawWmCategory':'swimming_pool'}, //  Count: 41030
        "hamlet":{'rawWmCategory':'hamlet'}, //  Count: 40176
        "sub-village":{'rawWmCategory':'sub-village'}, //  Count: 40176
        "town":{'rawWmCategory':'town'}, //  Count: 40017
        "20th century construction":{'rawWmCategory':'20th_century_construction'}, //  Count: 39586
        "catholicism":{'rawWmCategory':'catholicism'}, //  Count: 39576
        "catholic":{'rawWmCategory':'catholic'}, //  Count: 39576
        "shopping mall":{'rawWmCategory':'shopping_mall'}, //  Count: 39478
        "shopping center":{'rawWmCategory':'shopping_center'}, //  Count: 39478
        "shopping arcade":{'rawWmCategory':'shopping_arcade'}, //  Count: 39478
        "shopping centre":{'rawWmCategory':'shopping_centre'}, //  Count: 39478
        "retail shopping center":{'rawWmCategory':'retail_shopping_center'}, //  Count: 39478
        "library":{'rawWmCategory':'library'}, //  Count: 38139
        "archaeological site":{'rawWmCategory':'archaeological_site'}, //  Count: 37551
        "archeological site":{'rawWmCategory':'archeological_site'}, //  Count: 37551
        "firefighters":{'rawWmCategory':'firefighters'}, //  Count: 37279
        "fire station":{'rawWmCategory':'fire_station'}, //  Count: 37279
        "fire service":{'rawWmCategory':'fire_service'}, //  Count: 37279
        "fireman":{'rawWmCategory':'fireman'}, //  Count: 37279
        "fire brigade":{'rawWmCategory':'fire_brigade'}, //  Count: 37279
        "fire department":{'rawWmCategory':'fire_department'}, //  Count: 37279
        "intersection (road)":{'rawWmCategory':'intersection_(road)'}, //  Count: 37047
        "crossroad":{'rawWmCategory':'crossroad'}, //  Count: 37047
        "chowk (road junction)":{'rawWmCategory':'chowk_(road_junction)'}, //  Count: 37047
        "road junction":{'rawWmCategory':'road_junction'}, //  Count: 37047
        "motorway junction":{'rawWmCategory':'motorway_junction'}, //  Count: 37047
        "airport":{'rawWmCategory':'airport'}, //  Count: 37023
        "airfield":{'rawWmCategory':'airfield'}, //  Count: 37023
        "airstrip":{'rawWmCategory':'airstrip'}, //  Count: 37023
        "water tower":{'rawWmCategory':'water_tower'}, //  Count: 36653
        "football pitch":{'rawWmCategory':'football_pitch'}, //  Count: 36625
        "football - soccer field":{'rawWmCategory':'football_-_soccer_field'}, //  Count: 36625
        "football field":{'rawWmCategory':'football_field'}, //  Count: 36625
        "coastal construction":{'rawWmCategory':'coastal_construction'}, //  Count: 36311
        "animals":{'rawWmCategory':'animals'}, //  Count: 34688
        "water treatment":{'rawWmCategory':'water_treatment'}, //  Count: 34600
        "water purification":{'rawWmCategory':'water_purification'}, //  Count: 34600
        "transformer substation":{'rawWmCategory':'transformer_substation'}, //  Count: 34283
        "condo":{'rawWmCategory':'condo'}, //  Count: 34023
        "condominium":{'rawWmCategory':'condominium'}, //  Count: 34023
        "academic institution":{'rawWmCategory':'academic_institution'}, //  Count: 33972
        "[automotive industry]":{'rawWmCategory':'[automotive_industry]'}, //  Count: 33099
        "apartment":{'rawWmCategory':'apartment'}, //  Count: 33029
        "flat":{'rawWmCategory':'flat'}, //  Count: 33029
        "factory":{'rawWmCategory':'factory'}, //  Count: 32509
        "plant":{'rawWmCategory':'plant'}, //  Count: 32509
        "wastewater treatment plant":{'rawWmCategory':'wastewater_treatment_plant'}, //  Count: 31851
        "sewage treatment plant":{'rawWmCategory':'sewage_treatment_plant'}, //  Count: 31851
        "effluent treatment plant":{'rawWmCategory':'effluent_treatment_plant'}, //  Count: 31851
        "water reclamation plant":{'rawWmCategory':'water_reclamation_plant'}, //  Count: 31851
        "forest":{'rawWmCategory':'forest'}, //  Count: 31796
        "mine":{'rawWmCategory':'mine'}, //  Count: 31657
        "garage":{'rawWmCategory':'garage'}, //  Count: 31375
        "car dealership":{'rawWmCategory':'car_dealership'}, //  Count: 30946
        "car showroom":{'rawWmCategory':'car_showroom'}, //  Count: 30946
        "automobile / car dealer (sales)":{'rawWmCategory':'automobile_/_car_dealer_(sales)'}, //  Count: 30946
        "memorial":{'rawWmCategory':'memorial'}, //  Count: 30596
        "Catholic church":{'rawWmCategory':'Catholic_church'}, //  Count: 30497
        "clothes store / boutique":{'rawWmCategory':'clothes_store_/_boutique'}, //  Count: 30087
        "clothing store":{'rawWmCategory':'clothing_store'}, //  Count: 30087
        "oil and gas industry":{'rawWmCategory':'oil_and_gas_industry'}, //  Count: 29928
        "petroleum industry":{'rawWmCategory':'petroleum_industry'}, //  Count: 29928
        "municipality":{'rawWmCategory':'municipality'}, //  Count: 29445
        "indoor tennis court(s)":{'rawWmCategory':'indoor_tennis_court(s)'}, //  Count: 28201
        "tennis court(s)":{'rawWmCategory':'tennis_court(s)'}, //  Count: 28201
        "household yard":{'rawWmCategory':'household_yard'}, //  Count: 28047
        "atm / cash machine":{'rawWmCategory':'atm_/_cash_machine'}, //  Count: 27953
        "district, hamlet":{'rawWmCategory':'district,_hamlet'}, //  Count: 27493
        "ortsteil":{'rawWmCategory':'ortsteil'}, //  Count: 27493
        "cinema":{'rawWmCategory':'cinema'}, //  Count: 26916
        "cineworld":{'rawWmCategory':'cineworld'}, //  Count: 26916
        "movie theater":{'rawWmCategory':'movie_theater'}, //  Count: 26916
        "talkies (cinema)":{'rawWmCategory':'talkies_(cinema)'}, //  Count: 26916
        "golf course":{'rawWmCategory':'golf_course'}, //  Count: 26599
        "golf link":{'rawWmCategory':'golf_link'}, //  Count: 26599
        "retailer":{'rawWmCategory':'retailer'}, //  Count: 26573
        "district office":{'rawWmCategory':'district_office'}, //  Count: 26161
        "town hall":{'rawWmCategory':'town_hall'}, //  Count: 26161
        "civic hall":{'rawWmCategory':'civic_hall'}, //  Count: 26161
        "civic centre":{'rawWmCategory':'civic_centre'}, //  Count: 26161
        "city hall":{'rawWmCategory':'city_hall'}, //  Count: 26161
        "county hall":{'rawWmCategory':'county_hall'}, //  Count: 26161
        "council house - local government":{'rawWmCategory':'council_house_-_local_government'}, //  Count: 26161
        "municipal hall":{'rawWmCategory':'municipal_hall'}, //  Count: 26161
        "dorm":{'rawWmCategory':'dorm'}, //  Count: 26039
        "dormitory":{'rawWmCategory':'dormitory'}, //  Count: 26039
        "hall of residence":{'rawWmCategory':'hall_of_residence'}, //  Count: 26039
        "student accommodation":{'rawWmCategory':'student_accommodation'}, //  Count: 26039
        "residence hall":{'rawWmCategory':'residence_hall'}, //  Count: 26039
        "sari-sari store (Philippines)":{'rawWmCategory':'sari-sari_store_(Philippines)'}, //  Count: 25378
        "convenience store":{'rawWmCategory':'convenience_store'}, //  Count: 25378
        "hinduism":{'rawWmCategory':'hinduism'}, //  Count: 25373
        "hairdresser":{'rawWmCategory':'hairdresser'}, //  Count: 25237
        "hairstylist":{'rawWmCategory':'hairstylist'}, //  Count: 25237
        "hair salon":{'rawWmCategory':'hair_salon'}, //  Count: 25237
        "household goods store / shop":{'rawWmCategory':'household_goods_store_/_shop'}, //  Count: 24467
        "quarter (urban subdivision)":{'rawWmCategory':'quarter_(urban_subdivision)'}, //  Count: 24430
        "urban settlement or rural community":{'rawWmCategory':'urban_settlement_or_rural_community'}, //  Count: 24430
        "railway bridge":{'rawWmCategory':'railway_bridge'}, //  Count: 24239
        "heating":{'rawWmCategory':'heating'}, //  Count: 23997
        "bog":{'rawWmCategory':'bog'}, //  Count: 23667
        "quagmire":{'rawWmCategory':'quagmire'}, //  Count: 23667
        "mire":{'rawWmCategory':'mire'}, //  Count: 23667
        "wetland":{'rawWmCategory':'wetland'}, //  Count: 23667
        "medical clinic":{'rawWmCategory':'medical_clinic'}, //  Count: 23176
        "medical centre":{'rawWmCategory':'medical_centre'}, //  Count: 23176
        "health centre":{'rawWmCategory':'health_centre'}, //  Count: 23176
        "health center":{'rawWmCategory':'health_center'}, //  Count: 23176
        "sculpture":{'rawWmCategory':'sculpture'}, //  Count: 23044
        "night club":{'rawWmCategory':'night_club'}, //  Count: 23002
        "theater":{'rawWmCategory':'theater'}, //  Count: 22754
        "theatre":{'rawWmCategory':'theatre'}, //  Count: 22754
        "green area":{'rawWmCategory':'green_area'}, //  Count: 22733
        "administrative building":{'rawWmCategory':'administrative_building'}, //  Count: 22642
        "orthodox christianity":{'rawWmCategory':'orthodox_christianity'}, //  Count: 22357
        "conglomerate":{'rawWmCategory':'conglomerate'}, //  Count: 22132
        "company":{'rawWmCategory':'company'}, //  Count: 22132
        "firm":{'rawWmCategory':'firm'}, //  Count: 22132
        "under construction":{'rawWmCategory':'under_construction'}, //  Count: 22112
        "settlement":{'rawWmCategory':'settlement'}, //  Count: 21422
        "dam":{'rawWmCategory':'dam'}, //  Count: 21370
        "Second World War 1939-1945":{'rawWmCategory':'Second_World_War_1939-1945'}, //  Count: 21182
        "pub / public house":{'rawWmCategory':'pub_/_public_house'}, //  Count: 20965
        "community center / hall":{'rawWmCategory':'community_center_/_hall'}, //  Count: 20717
        "community centre / hall":{'rawWmCategory':'community_centre_/_hall'}, //  Count: 20717
        "communal house":{'rawWmCategory':'communal_house'}, //  Count: 20717
        "tower":{'rawWmCategory':'tower'}, //  Count: 20648
        "hindu temple":{'rawWmCategory':'hindu_temple'}, //  Count: 20177
        "mandir":{'rawWmCategory':'mandir'}, //  Count: 20177
        "workshop":{'rawWmCategory':'workshop'}, //  Count: 19477
        "swamp":{'rawWmCategory':'swamp'}, //  Count: 19165
        "reservoir":{'rawWmCategory':'reservoir'}, //  Count: 18983
        "artificial lake":{'rawWmCategory':'artificial_lake'}, //  Count: 18983
        "manmade lake":{'rawWmCategory':'manmade_lake'}, //  Count: 18983
        "gym / gymnasium - sports":{'rawWmCategory':'gym_/_gymnasium_-_sports'}, //  Count: 18864
        "school yard":{'rawWmCategory':'school_yard'}, //  Count: 18727
        "abandoned settlement":{'rawWmCategory':'abandoned_settlement'}, //  Count: 18677
        "ghost town":{'rawWmCategory':'ghost_town'}, //  Count: 18677
        "pizzeria":{'rawWmCategory':'pizzeria'}, //  Count: 18535
        "pizza shop / parlor":{'rawWmCategory':'pizza_shop_/_parlor'}, //  Count: 18535
        "foot over bridge (fob)":{'rawWmCategory':'foot_over_bridge_(fob)'}, //  Count: 18399
        "footbridge":{'rawWmCategory':'footbridge'}, //  Count: 18399
        "pedestrian overpass":{'rawWmCategory':'pedestrian_overpass'}, //  Count: 18399
        "earthworks":{'rawWmCategory':'earthworks'}, //  Count: 18348
        "earthwork (archaeology)":{'rawWmCategory':'earthwork_(archaeology)'}, //  Count: 18348
        "fountain":{'rawWmCategory':'fountain'}, //  Count: 18306
        "peak":{'rawWmCategory':'peak'}, //  Count: 18133
        "mountaintop":{'rawWmCategory':'mountaintop'}, //  Count: 18133
        "summit":{'rawWmCategory':'summit'}, //  Count: 18133
        "pinnacle":{'rawWmCategory':'pinnacle'}, //  Count: 18133
        "hill":{'rawWmCategory':'hill'}, //  Count: 17950
        "knoll":{'rawWmCategory':'knoll'}, //  Count: 17950
        "hillock":{'rawWmCategory':'hillock'}, //  Count: 17950
        "hills":{'rawWmCategory':'hills'}, //  Count: 17950
        "street":{'rawWmCategory':'street'}, //  Count: 17551
        "avenue":{'rawWmCategory':'avenue'}, //  Count: 17551
        "alley":{'rawWmCategory':'alley'}, //  Count: 17551
        "lane":{'rawWmCategory':'lane'}, //  Count: 17551
        "boulevard":{'rawWmCategory':'boulevard'}, //  Count: 17551
        "farm land / agricultural area":{'rawWmCategory':'farm_land_/_agricultural_area'}, //  Count: 17525
        "open-pit mine":{'rawWmCategory':'open-pit_mine'}, //  Count: 17520
        "mobile phone transmitter / mast":{'rawWmCategory':'mobile_phone_transmitter_/_mast'}, //  Count: 17505
        "cell site / cell tower":{'rawWmCategory':'cell_site_/_cell_tower'}, //  Count: 17505
        "bts":{'rawWmCategory':'bts'}, //  Count: 17505
        "base transceiver/ GSM station":{'rawWmCategory':'base_transceiver/_GSM_station'}, //  Count: 17505
        "pets":{'rawWmCategory':'pets'}, //  Count: 17493
        "subdivision":{'rawWmCategory':'subdivision'}, //  Count: 17489
        "chapel":{'rawWmCategory':'chapel'}, //  Count: 17456
        "middle school":{'rawWmCategory':'middle_school'}, //  Count: 17429
        "junior high school":{'rawWmCategory':'junior_high_school'}, //  Count: 17429
        "intermediate school":{'rawWmCategory':'intermediate_school'}, //  Count: 17429
        "power plant":{'rawWmCategory':'power_plant'}, //  Count: 17409
        "power house":{'rawWmCategory':'power_house'}, //  Count: 17409
        "power station":{'rawWmCategory':'power_station'}, //  Count: 17409
        "generating station":{'rawWmCategory':'generating_station'}, //  Count: 17409
        "railroad crossing":{'rawWmCategory':'railroad_crossing'}, //  Count: 17397
        "level crossing":{'rawWmCategory':'level_crossing'}, //  Count: 17397
        "railway crossing":{'rawWmCategory':'railway_crossing'}, //  Count: 17397
        "train crossing":{'rawWmCategory':'train_crossing'}, //  Count: 17397
        "grade crossing":{'rawWmCategory':'grade_crossing'}, //  Count: 17397
        "sports club":{'rawWmCategory':'sports_club'}, //  Count: 17126
        "club":{'rawWmCategory':'club'}, //  Count: 17031
        "automotive parts dealer":{'rawWmCategory':'automotive_parts_dealer'}, //  Count: 17013
        "car accessories store":{'rawWmCategory':'car_accessories_store'}, //  Count: 17013
        "RV / caravan park":{'rawWmCategory':'RV_/_caravan_park'}, //  Count: 16940
        "campsite":{'rawWmCategory':'campsite'}, //  Count: 16940
        "camping pitch":{'rawWmCategory':'camping_pitch'}, //  Count: 16940
        "campgrounds":{'rawWmCategory':'campgrounds'}, //  Count: 16940
        "RV Resort":{'rawWmCategory':'RV_Resort'}, //  Count: 16940
        "bus station":{'rawWmCategory':'bus_station'}, //  Count: 16935
        "coach station":{'rawWmCategory':'coach_station'}, //  Count: 16935
        "bus terminal":{'rawWmCategory':'bus_terminal'}, //  Count: 16935
        "semi-detached house":{'rawWmCategory':'semi-detached_house'}, //  Count: 16909
        "quarry":{'rawWmCategory':'quarry'}, //  Count: 16904
        "furniture store / shop":{'rawWmCategory':'furniture_store_/_shop'}, //  Count: 16709
        "tunnel":{'rawWmCategory':'tunnel'}, //  Count: 16687
        "display":{'rawWmCategory':'display'}, //  Count: 16591
        "auto wash":{'rawWmCategory':'auto_wash'}, //  Count: 16573
        "car wash":{'rawWmCategory':'car_wash'}, //  Count: 16573
        "bakery":{'rawWmCategory':'bakery'}, //  Count: 16323
        "bakehouse":{'rawWmCategory':'bakehouse'}, //  Count: 16323
        "wind turbine":{'rawWmCategory':'wind_turbine'}, //  Count: 16139
        "fourth-level administrative division":{'rawWmCategory':'fourth-level_administrative_division'}, //  Count: 15913
        "protestantism":{'rawWmCategory':'protestantism'}, //  Count: 15849
        "protestant":{'rawWmCategory':'protestant'}, //  Count: 15849
        "hardware store":{'rawWmCategory':'hardware_store'}, //  Count: 15494
        "DIY store":{'rawWmCategory':'DIY_store'}, //  Count: 15494
        "home improvement / hardware store":{'rawWmCategory':'home_improvement_/_hardware_store'}, //  Count: 15494
        "painting and decorating shop":{'rawWmCategory':'painting_and_decorating_shop'}, //  Count: 15494
        "home improvement center":{'rawWmCategory':'home_improvement_center'}, //  Count: 15494
        "historical building":{'rawWmCategory':'historical_building'}, //  Count: 15202
        "NAVAID - navigational aid":{'rawWmCategory':'NAVAID_-_navigational_aid'}, //  Count: 15187
        "ATON - aid to navigation":{'rawWmCategory':'ATON_-_aid_to_navigation'}, //  Count: 15187
        "bathhouse":{'rawWmCategory':'bathhouse'}, //  Count: 15148
        "thermae":{'rawWmCategory':'thermae'}, //  Count: 15148
        "public baths":{'rawWmCategory':'public_baths'}, //  Count: 15148
        "banya":{'rawWmCategory':'banya'}, //  Count: 15148
        "steambath":{'rawWmCategory':'steambath'}, //  Count: 15148
        "hostel":{'rawWmCategory':'hostel'}, //  Count: 15013
        "basketball court":{'rawWmCategory':'basketball_court'}, //  Count: 14881
        "dentist":{'rawWmCategory':'dentist'}, //  Count: 14804
        "dental practice":{'rawWmCategory':'dental_practice'}, //  Count: 14804
        "dental clinic":{'rawWmCategory':'dental_clinic'}, //  Count: 14804
        "bay":{'rawWmCategory':'bay'}, //  Count: 14748
        "gulf":{'rawWmCategory':'gulf'}, //  Count: 14748
        "service":{'rawWmCategory':'service'}, //  Count: 14701
        "commune - administrative division":{'rawWmCategory':'commune_-_administrative_division'}, //  Count: 14475
        "roundabout / traffic circle":{'rawWmCategory':'roundabout_/_traffic_circle'}, //  Count: 14417
        "chowk (roundabout / traffic circle)":{'rawWmCategory':'chowk_(roundabout_/_traffic_circle)'}, //  Count: 14417
        "colony / nagar - neighbourhood in India":{'rawWmCategory':'colony_/_nagar_-_neighbourhood_in_India'}, //  Count: 14392
        "castle":{'rawWmCategory':'castle'}, //  Count: 14087
        "checkpoint":{'rawWmCategory':'checkpoint'}, //  Count: 14087
        "spring":{'rawWmCategory':'spring'}, //  Count: 14063
        "war memorial":{'rawWmCategory':'war_memorial'}, //  Count: 13898
        "building materials":{'rawWmCategory':'building_materials'}, //  Count: 13816
        "boiler house":{'rawWmCategory':'boiler_house'}, //  Count: 13748
        "heat-only boiler station":{'rawWmCategory':'heat-only_boiler_station'}, //  Count: 13748
        "architectural style":{'rawWmCategory':'architectural_style'}, //  Count: 13636
        "football / soccer stadium":{'rawWmCategory':'football_/_soccer_stadium'}, //  Count: 13230
        "gate":{'rawWmCategory':'gate'}, //  Count: 13172
        "courthouse":{'rawWmCategory':'courthouse'}, //  Count: 13129
        "transportation":{'rawWmCategory':'transportation'}, //  Count: 13021
        "jeepney stop":{'rawWmCategory':'jeepney_stop'}, //  Count: 12833
        "share taxi stop":{'rawWmCategory':'share_taxi_stop'}, //  Count: 12833
        "bush taxi stop":{'rawWmCategory':'bush_taxi_stop'}, //  Count: 12833
        "ancient civilization":{'rawWmCategory':'ancient_civilization'}, //  Count: 12726
        "abbey":{'rawWmCategory':'abbey'}, //  Count: 12599
        "convent":{'rawWmCategory':'convent'}, //  Count: 12599
        "convento":{'rawWmCategory':'convento'}, //  Count: 12599
        "monastery":{'rawWmCategory':'monastery'}, //  Count: 12599
        "priory":{'rawWmCategory':'priory'}, //  Count: 12599
        "leisure":{'rawWmCategory':'leisure'}, //  Count: 12556
        "barangay":{'rawWmCategory':'barangay'}, //  Count: 12478
        "industrial estate":{'rawWmCategory':'industrial_estate'}, //  Count: 12241
        "industrial area":{'rawWmCategory':'industrial_area'}, //  Count: 12241
        "industrial zone":{'rawWmCategory':'industrial_zone'}, //  Count: 12241
        "industrial district":{'rawWmCategory':'industrial_district'}, //  Count: 12241
        "aeroplane on display":{'rawWmCategory':'aeroplane_on_display'}, //  Count: 12238
        "aircraft on display":{'rawWmCategory':'aircraft_on_display'}, //  Count: 12238
        "helicopter on display":{'rawWmCategory':'helicopter_on_display'}, //  Count: 12238
        "public garden":{'rawWmCategory':'public_garden'}, //  Count: 12168
        "green space":{'rawWmCategory':'green_space'}, //  Count: 12168
        "urban open space":{'rawWmCategory':'urban_open_space'}, //  Count: 12168
        "district":{'rawWmCategory':'district'}, //  Count: 12161
        "chimney":{'rawWmCategory':'chimney'}, //  Count: 12091
        "smokestack":{'rawWmCategory':'smokestack'}, //  Count: 12091
        "terraced / row house - townhome / townhouse":{'rawWmCategory':'terraced_/_row_house_-_townhome_/_townhouse'}, //  Count: 11967
        "townhome":{'rawWmCategory':'townhome'}, //  Count: 11967
        "townhouse":{'rawWmCategory':'townhouse'}, //  Count: 11967
        "runway":{'rawWmCategory':'runway'}, //  Count: 11949
        "landing strip":{'rawWmCategory':'landing_strip'}, //  Count: 11949
        "aircraft":{'rawWmCategory':'aircraft'}, //  Count: 11852
        "organization":{'rawWmCategory':'organization'}, //  Count: 11846
        "organisation":{'rawWmCategory':'organisation'}, //  Count: 11846
        "pumping station":{'rawWmCategory':'pumping_station'}, //  Count: 11661
        "tube station":{'rawWmCategory':'tube_station'}, //  Count: 11618
        "metro station":{'rawWmCategory':'metro_station'}, //  Count: 11618
        "underground station":{'rawWmCategory':'underground_station'}, //  Count: 11618
        "subway station":{'rawWmCategory':'subway_station'}, //  Count: 11618
        "food industry":{'rawWmCategory':'food_industry'}, //  Count: 11580
        "protected area":{'rawWmCategory':'protected_area'}, //  Count: 11392
        "nature reserve":{'rawWmCategory':'nature_reserve'}, //  Count: 11392
        "nature conservation park / area":{'rawWmCategory':'nature_conservation_park_/_area'}, //  Count: 11392
        "wilderness area":{'rawWmCategory':'wilderness_area'}, //  Count: 11392
        "wildlife sanctuary":{'rawWmCategory':'wildlife_sanctuary'}, //  Count: 11392
        "nature preserve":{'rawWmCategory':'nature_preserve'}, //  Count: 11392
        "policlinic":{'rawWmCategory':'policlinic'}, //  Count: 11391
        "outpatient clinic":{'rawWmCategory':'outpatient_clinic'}, //  Count: 11391
        "polyclinic":{'rawWmCategory':'polyclinic'}, //  Count: 11391
        "mill":{'rawWmCategory':'mill'}, //  Count: 11390
        "trolleybus stop":{'rawWmCategory':'trolleybus_stop'}, //  Count: 11372
        "headland":{'rawWmCategory':'headland'}, //  Count: 11218
        "campus":{'rawWmCategory':'campus'}, //  Count: 11190
        "watercraft":{'rawWmCategory':'watercraft'}, //  Count: 11137
        "tram stop":{'rawWmCategory':'tram_stop'}, //  Count: 11090
        "streetcar stop":{'rawWmCategory':'streetcar_stop'}, //  Count: 11090
        "telecom":{'rawWmCategory':'telecom'}, //  Count: 11044
        "telecommunication":{'rawWmCategory':'telecommunication'}, //  Count: 11044
        "beauty salon":{'rawWmCategory':'beauty_salon'}, //  Count: 10972
        "beauty parlor":{'rawWmCategory':'beauty_parlor'}, //  Count: 10972
        "embassy / consulate / diplomatic mission":{'rawWmCategory':'embassy_/_consulate_/_diplomatic_mission'}, //  Count: 10925
        "dining hall":{'rawWmCategory':'dining_hall'}, //  Count: 10924
        "banqueting hall":{'rawWmCategory':'banqueting_hall'}, //  Count: 10924
        "canteen":{'rawWmCategory':'canteen'}, //  Count: 10924
        "nuragic civilization - archeology":{'rawWmCategory':'nuragic_civilization_-_archeology'}, //  Count: 10885
        "government":{'rawWmCategory':'government'}, //  Count: 10839
        "shed":{'rawWmCategory':'shed'}, //  Count: 10774
        "prison":{'rawWmCategory':'prison'}, //  Count: 10638
        "jail":{'rawWmCategory':'jail'}, //  Count: 10638
        "jailhouse":{'rawWmCategory':'jailhouse'}, //  Count: 10638
        "area - to be replaced":{'rawWmCategory':'area_-_to_be_replaced'}, //  Count: 10568
        "overpass / flyover":{'rawWmCategory':'overpass_/_flyover'}, //  Count: 10560
        "buddhism":{'rawWmCategory':'buddhism'}, //  Count: 10553
        "cottage":{'rawWmCategory':'cottage'}, //  Count: 10531
        "footwear shop":{'rawWmCategory':'footwear_shop'}, //  Count: 10473
        "shoe shop / store":{'rawWmCategory':'shoe_shop_/_store'}, //  Count: 10471
        "business centre/ building":{'rawWmCategory':'business_centre/_building'}, //  Count: 10444
        "round barrow":{'rawWmCategory':'round_barrow'}, //  Count: 10261
        "barrow/tumulus/burial mound":{'rawWmCategory':'barrow/tumulus/burial_mound'}, //  Count: 10261
        "long barrow":{'rawWmCategory':'long_barrow'}, //  Count: 10261
        "burial mound":{'rawWmCategory':'burial_mound'}, //  Count: 10261
        "howe":{'rawWmCategory':'howe'}, //  Count: 10261
        "bowl barrow":{'rawWmCategory':'bowl_barrow'}, //  Count: 10261
        "bank barrow":{'rawWmCategory':'bank_barrow'}, //  Count: 10261
        "bell barrow":{'rawWmCategory':'bell_barrow'}, //  Count: 10261
        "electronic goods retailer / manufacturer":{'rawWmCategory':'electronic_goods_retailer_/_manufacturer'}, //  Count: 10198
        "third-level administrative division":{'rawWmCategory':'third-level_administrative_division'}, //  Count: 10193
        "playing field":{'rawWmCategory':'playing_field'}, //  Count: 10154
        "recreation area / ground":{'rawWmCategory':'recreation_area_/_ground'}, //  Count: 10154
        "lab":{'rawWmCategory':'lab'}, //  Count: 10150
        "laboratories":{'rawWmCategory':'laboratories'}, //  Count: 10150
        "labs":{'rawWmCategory':'labs'}, //  Count: 10150
        "river":{'rawWmCategory':'river'}, //  Count: 10050
        "water supply":{'rawWmCategory':'water_supply'}, //  Count: 10024
        "college - DONT USE OR TRANSLATE THIS CATEGORY IS PLANNED TO BE REMOVED":{'rawWmCategory':'college_-_DONT_USE_OR_TRANSLATE_THIS_CATEGORY_IS_PLANNED_TO_BE_REMOVED'}, //  Count: 9988
        "tire repair service":{'rawWmCategory':'tire_repair_service'}, //  Count: 9931
        "tyre repair service":{'rawWmCategory':'tyre_repair_service'}, //  Count: 9931
        "tire assembly":{'rawWmCategory':'tire_assembly'}, //  Count: 9931
        "local government":{'rawWmCategory':'local_government'}, //  Count: 9851
        "trading":{'rawWmCategory':'trading'}, //  Count: 9777
        "trade":{'rawWmCategory':'trade'}, //  Count: 9777
        "commerce":{'rawWmCategory':'commerce'}, //  Count: 9777
        "kindergarten yard":{'rawWmCategory':'kindergarten_yard'}, //  Count: 9611
        "sign":{'rawWmCategory':'sign'}, //  Count: 9523
        "construction firm / organisation":{'rawWmCategory':'construction_firm_/_organisation'}, //  Count: 9513
        "construction / building company":{'rawWmCategory':'construction_/_building_company'}, //  Count: 9513
        "waste management industry":{'rawWmCategory':'waste_management_industry'}, //  Count: 9498
        "[waste management]":{'rawWmCategory':'[waste_management]'}, //  Count: 9498
        "fitness suite":{'rawWmCategory':'fitness_suite'}, //  Count: 9467
        "health club":{'rawWmCategory':'health_club'}, //  Count: 9467
        "fitness centre / center":{'rawWmCategory':'fitness_centre_/_center'}, //  Count: 9467
        "fitness club":{'rawWmCategory':'fitness_club'}, //  Count: 9467
        "gym - fitness / health":{'rawWmCategory':'gym_-_fitness_/_health'}, //  Count: 9467
        "public toilet":{'rawWmCategory':'public_toilet'}, //  Count: 9430
        "restroom":{'rawWmCategory':'restroom'}, //  Count: 9430
        "public washroom":{'rawWmCategory':'public_washroom'}, //  Count: 9430
        "resort":{'rawWmCategory':'resort'}, //  Count: 9422
        "confluence":{'rawWmCategory':'confluence'}, //  Count: 9377
        "statue":{'rawWmCategory':'statue'}, //  Count: 9316
        "liquor store":{'rawWmCategory':'liquor_store'}, //  Count: 9194
        "off-licence":{'rawWmCategory':'off-licence'}, //  Count: 9194
        "McDonald&#039;s":{'rawWmCategory':'McDonald&#039;s'}, //  Count: 9175
        "McDrive":{'rawWmCategory':'McDrive'}, //  Count: 9175
        "buddhist temple":{'rawWmCategory':'buddhist_temple'}, //  Count: 9155
        "wooden":{'rawWmCategory':'wooden'}, //  Count: 8998
        "travel agency":{'rawWmCategory':'travel_agency'}, //  Count: 8925
        "bookstore":{'rawWmCategory':'bookstore'}, //  Count: 8903
        "bookshop":{'rawWmCategory':'bookshop'}, //  Count: 8903
        "1960s construction":{'rawWmCategory':'1960s_construction'}, //  Count: 8743
        "real estate agency":{'rawWmCategory':'real_estate_agency'}, //  Count: 8686
        "estate agents":{'rawWmCategory':'estate_agents'}, //  Count: 8686
        "vocational school":{'rawWmCategory':'vocational_school'}, //  Count: 8684
        "vocational training":{'rawWmCategory':'vocational_training'}, //  Count: 8684
        "sciences":{'rawWmCategory':'sciences'}, //  Count: 8611
        "science":{'rawWmCategory':'science'}, //  Count: 8611
        "scientist":{'rawWmCategory':'scientist'}, //  Count: 8611
        "scientific":{'rawWmCategory':'scientific'}, //  Count: 8611
        "airplane":{'rawWmCategory':'airplane'}, //  Count: 8581
        "aeroplane":{'rawWmCategory':'aeroplane'}, //  Count: 8581
        "fishing area":{'rawWmCategory':'fishing_area'}, //  Count: 8569
        "fishery":{'rawWmCategory':'fishery'}, //  Count: 8569
        "mansion / manor house / villa":{'rawWmCategory':'mansion_/_manor_house_/_villa'}, //  Count: 8564
        "stately home":{'rawWmCategory':'stately_home'}, //  Count: 8564
        "country house":{'rawWmCategory':'country_house'}, //  Count: 8564
        "highway interchange / exit":{'rawWmCategory':'highway_interchange_/_exit'}, //  Count: 8464
        "oil/gas well":{'rawWmCategory':'oil/gas_well'}, //  Count: 8456
        "pumpjack":{'rawWmCategory':'pumpjack'}, //  Count: 8456
        "aid to navigation - marine":{'rawWmCategory':'aid_to_navigation_-_marine'}, //  Count: 8430
        "industrial building":{'rawWmCategory':'industrial_building'}, //  Count: 8420
        "carport":{'rawWmCategory':'carport'}, //  Count: 8420
        "residential garage":{'rawWmCategory':'residential_garage'}, //  Count: 8420
        "garage cooperative society":{'rawWmCategory':'garage_cooperative_society'}, //  Count: 8420
        "factory building":{'rawWmCategory':'factory_building'}, //  Count: 8420
        "internet cafe":{'rawWmCategory':'internet_cafe'}, //  Count: 8255
        "cybercafe":{'rawWmCategory':'cybercafe'}, //  Count: 8255
        "net cafe":{'rawWmCategory':'net_cafe'}, //  Count: 8255
        "sporting goods store":{'rawWmCategory':'sporting_goods_store'}, //  Count: 8236
        "sporting/sports equipment":{'rawWmCategory':'sporting/sports_equipment'}, //  Count: 8236
        "sports store / shop":{'rawWmCategory':'sports_store_/_shop'}, //  Count: 8236
        "sports retailer":{'rawWmCategory':'sports_retailer'}, //  Count: 8236
        "embassy":{'rawWmCategory':'embassy'}, //  Count: 8214
        "high commission (embassy)":{'rawWmCategory':'high_commission_(embassy)'}, //  Count: 8214
        "conjunto":{'rawWmCategory':'conjunto'}, //  Count: 8184
        "jewellery":{'rawWmCategory':'jewellery'}, //  Count: 8146
        "jeweler":{'rawWmCategory':'jeweler'}, //  Count: 8146
        "jewelry":{'rawWmCategory':'jewelry'}, //  Count: 8146
        "jeweller":{'rawWmCategory':'jeweller'}, //  Count: 8146
        "estate (manor / mansion land)":{'rawWmCategory':'estate_(manor_/_mansion_land)'}, //  Count: 8080
        "manor (land)":{'rawWmCategory':'manor_(land)'}, //  Count: 8080
        "21st century construction":{'rawWmCategory':'21st_century_construction'}, //  Count: 8052
        "food - to be replaced":{'rawWmCategory':'food_-_to_be_replaced'}, //  Count: 7948
        "cellphone shop / store":{'rawWmCategory':'cellphone_shop_/_store'}, //  Count: 7938
        "mobile phone retailer":{'rawWmCategory':'mobile_phone_retailer'}, //  Count: 7938
        "palace":{'rawWmCategory':'palace'}, //  Count: 7833
        "kiosk":{'rawWmCategory':'kiosk'}, //  Count: 7680
        "storage":{'rawWmCategory':'storage'}, //  Count: 7666
        "educational building":{'rawWmCategory':'educational_building'}, //  Count: 7643
        "academic building":{'rawWmCategory':'academic_building'}, //  Count: 7643
        "community":{'rawWmCategory':'community'}, //  Count: 7618
        "coffeehouse":{'rawWmCategory':'coffeehouse'}, //  Count: 7616
        "espresso bar":{'rawWmCategory':'espresso_bar'}, //  Count: 7616
        "fuel storage tank":{'rawWmCategory':'fuel_storage_tank'}, //  Count: 7612
        "junction - Do not use this category, the category is under cleanup":{'rawWmCategory':'junction_-_Do_not_use_this_category,_the_category_is_under_cleanup'}, //  Count: 7566
        "bed and breakfast":{'rawWmCategory':'bed_and_breakfast'}, //  Count: 7562
        "Grade II Listed (UK)":{'rawWmCategory':'Grade_II_Listed_(UK)'}, //  Count: 7511
        "horse":{'rawWmCategory':'horse'}, //  Count: 7504
        "children":{'rawWmCategory':'children'}, //  Count: 7423
        "lighthouse":{'rawWmCategory':'lighthouse'}, //  Count: 7398
        "grassland":{'rawWmCategory':'grassland'}, //  Count: 7371
        "natural gas industry":{'rawWmCategory':'natural_gas_industry'}, //  Count: 7357
        "prehistoric":{'rawWmCategory':'prehistoric'}, //  Count: 7270
        "public transport terminus":{'rawWmCategory':'public_transport_terminus'}, //  Count: 7192
        "birds":{'rawWmCategory':'birds'}, //  Count: 7162
        "allotment (gardening)":{'rawWmCategory':'allotment_(gardening)'}, //  Count: 7147
        "seaport":{'rawWmCategory':'seaport'}, //  Count: 7136
        "sea port":{'rawWmCategory':'sea_port'}, //  Count: 7136
        "port":{'rawWmCategory':'port'}, //  Count: 7136
        "teleferic":{'rawWmCategory':'teleferic'}, //  Count: 7073
        "aerial tramway/ ropeway/ gondola lift/ skilift / cable car station (transport in the air)":{'rawWmCategory':'aerial_tramway/_ropeway/_gondola_lift/_skilift_/_cable_car_station_(transport_in_the_air)'}, //  Count: 7073
        "cableway":{'rawWmCategory':'cableway'}, //  Count: 7073
        "child care":{'rawWmCategory':'child_care'}, //  Count: 7053
        "day care":{'rawWmCategory':'day_care'}, //  Count: 7053
        "creche":{'rawWmCategory':'creche'}, //  Count: 7053
        "motorcycle":{'rawWmCategory':'motorcycle'}, //  Count: 7046
        "flower shop / florist":{'rawWmCategory':'flower_shop_/_florist'}, //  Count: 7045
        "motel":{'rawWmCategory':'motel'}, //  Count: 7021
        "insurance company":{'rawWmCategory':'insurance_company'}, //  Count: 7006
        "culvert":{'rawWmCategory':'culvert'}, //  Count: 6995
        "society":{'rawWmCategory':'society'}, //  Count: 6988
        "NRHP - National Register of Historic Places":{'rawWmCategory':'NRHP_-_National_Register_of_Historic_Places'}, //  Count: 6971
        "inn":{'rawWmCategory':'inn'}, //  Count: 6933
        "pedestrian crossing":{'rawWmCategory':'pedestrian_crossing'}, //  Count: 6915
        "zebra crossing / pelican crossing":{'rawWmCategory':'zebra_crossing_/_pelican_crossing'}, //  Count: 6915
        "crosswalk":{'rawWmCategory':'crosswalk'}, //  Count: 6915
        "[military branch]":{'rawWmCategory':'[military_branch]'}, //  Count: 6883
        "Russian Orthodox Church":{'rawWmCategory':'Russian_Orthodox_Church'}, //  Count: 6881
        "shelter":{'rawWmCategory':'shelter'}, //  Count: 6794
        "historic landmark":{'rawWmCategory':'historic_landmark'}, //  Count: 6720
        "department store":{'rawWmCategory':'department_store'}, //  Count: 6718
        "memorial plaque":{'rawWmCategory':'memorial_plaque'}, //  Count: 6710
        "aid to navigation - aviation":{'rawWmCategory':'aid_to_navigation_-_aviation'}, //  Count: 6689
        "lookout tower":{'rawWmCategory':'lookout_tower'}, //  Count: 6668
        "1970s construction":{'rawWmCategory':'1970s_construction'}, //  Count: 6661
        "homestead":{'rawWmCategory':'homestead'}, //  Count: 6567
        "institute":{'rawWmCategory':'institute'}, //  Count: 6543
        "railway platform":{'rawWmCategory':'railway_platform'}, //  Count: 6530
        "sports complex":{'rawWmCategory':'sports_complex'}, //  Count: 6507
        "sportsplex":{'rawWmCategory':'sportsplex'}, //  Count: 6507
        "artillery battery":{'rawWmCategory':'artillery_battery'}, //  Count: 6497
        "notable event":{'rawWmCategory':'notable_event'}, //  Count: 6413
        "bicycle / bike":{'rawWmCategory':'bicycle_/_bike'}, //  Count: 6353
        "falls":{'rawWmCategory':'falls'}, //  Count: 6301
        "waterfall":{'rawWmCategory':'waterfall'}, //  Count: 6301
        "entrance":{'rawWmCategory':'entrance'}, //  Count: 6291
        "college of further education / higher education":{'rawWmCategory':'college_of_further_education_/_higher_education'}, //  Count: 6277
        "cegep":{'rawWmCategory':'cegep'}, //  Count: 6277
        "post-secondary education":{'rawWmCategory':'post-secondary_education'}, //  Count: 6277
        "tertiary education":{'rawWmCategory':'tertiary_education'}, //  Count: 6277
        "town district":{'rawWmCategory':'town_district'}, //  Count: 6245
        "city district":{'rawWmCategory':'city_district'}, //  Count: 6245
        "detached house":{'rawWmCategory':'detached_house'}, //  Count: 6238
        "cape (geography)":{'rawWmCategory':'cape_(geography)'}, //  Count: 6224
        "entertainment attraction / ride":{'rawWmCategory':'entertainment_attraction_/_ride'}, //  Count: 6223
        "district heating substation":{'rawWmCategory':'district_heating_substation'}, //  Count: 6200
        "advertising":{'rawWmCategory':'advertising'}, //  Count: 6190
        "veterinarian":{'rawWmCategory':'veterinarian'}, //  Count: 6180
        "veterinary  clinic / surgery":{'rawWmCategory':'veterinary__clinic_/_surgery'}, //  Count: 6180
        "animal hospital":{'rawWmCategory':'animal_hospital'}, //  Count: 6180
        "19th century construction":{'rawWmCategory':'19th_century_construction'}, //  Count: 6178
        "ice cream parlor":{'rawWmCategory':'ice_cream_parlor'}, //  Count: 6158
        "frozen yogurt shop":{'rawWmCategory':'frozen_yogurt_shop'}, //  Count: 6158
        "gelato parlor":{'rawWmCategory':'gelato_parlor'}, //  Count: 6158
        "housing services":{'rawWmCategory':'housing_services'}, //  Count: 6147
        "1950s construction":{'rawWmCategory':'1950s_construction'}, //  Count: 6039
        "farmhouse":{'rawWmCategory':'farmhouse'}, //  Count: 6003
        "bunker":{'rawWmCategory':'bunker'}, //  Count: 5992
        "water well":{'rawWmCategory':'water_well'}, //  Count: 5947
        "artesian well":{'rawWmCategory':'artesian_well'}, //  Count: 5947
        "viewpoint":{'rawWmCategory':'viewpoint'}, //  Count: 5939
        "panoramic view":{'rawWmCategory':'panoramic_view'}, //  Count: 5939
        "travel plaza":{'rawWmCategory':'travel_plaza'}, //  Count: 5917
        "service area":{'rawWmCategory':'service_area'}, //  Count: 5917
        "rest stop":{'rawWmCategory':'rest_stop'}, //  Count: 5917
        "motorway service area":{'rawWmCategory':'motorway_service_area'}, //  Count: 5917
        "rest area":{'rawWmCategory':'rest_area'}, //  Count: 5917
        "computers":{'rawWmCategory':'computers'}, //  Count: 5916
        "computer":{'rawWmCategory':'computer'}, //  Count: 5916
        "wholesale business":{'rawWmCategory':'wholesale_business'}, //  Count: 5913
        "wholesalers":{'rawWmCategory':'wholesalers'}, //  Count: 5913
        "commercial":{'rawWmCategory':'commercial'}, //  Count: 5890
        "barracks":{'rawWmCategory':'barracks'}, //  Count: 5866
        "public vehicle depot":{'rawWmCategory':'public_vehicle_depot'}, //  Count: 5840
        "cars - to be replaced":{'rawWmCategory':'cars_-_to_be_replaced'}, //  Count: 5793
        "canal":{'rawWmCategory':'canal'}, //  Count: 5779
        "healthcare":{'rawWmCategory':'healthcare'}, //  Count: 5775
        "scientific research institute / centre":{'rawWmCategory':'scientific_research_institute_/_centre'}, //  Count: 5743
        "headquarters":{'rawWmCategory':'headquarters'}, //  Count: 5727
        "head office":{'rawWmCategory':'head_office'}, //  Count: 5727
        "main office":{'rawWmCategory':'main_office'}, //  Count: 5727
        "grave":{'rawWmCategory':'grave'}, //  Count: 5684
        "amusement park":{'rawWmCategory':'amusement_park'}, //  Count: 5681
        "theme park":{'rawWmCategory':'theme_park'}, //  Count: 5681
        "underground facility":{'rawWmCategory':'underground_facility'}, //  Count: 5651
        "disaster":{'rawWmCategory':'disaster'}, //  Count: 5583
        "construction site":{'rawWmCategory':'construction_site'}, //  Count: 5580
        "tomb":{'rawWmCategory':'tomb'}, //  Count: 5579
        "rocks":{'rawWmCategory':'rocks'}, //  Count: 5578
        "forest belt":{'rawWmCategory':'forest_belt'}, //  Count: 5496
        "stairs":{'rawWmCategory':'stairs'}, //  Count: 5486
        "spa":{'rawWmCategory':'spa'}, //  Count: 5485
        "radio tower":{'rawWmCategory':'radio_tower'}, //  Count: 5456
        "Jehovah's Witnesses":{'rawWmCategory':'Jehovahs_Witnesses'}, //  Count: 5442
        "war cemetery":{'rawWmCategory':'war_cemetery'}, //  Count: 5425
        "office":{'rawWmCategory':'office'}, //  Count: 5410
        "landmark":{'rawWmCategory':'landmark'}, //  Count: 5410
        "landmarks":{'rawWmCategory':'landmarks'}, //  Count: 5410
        "water tank":{'rawWmCategory':'water_tank'}, //  Count: 5408
        "1980s construction":{'rawWmCategory':'1980s_construction'}, //  Count: 5304
        "art museum / art gallery":{'rawWmCategory':'art_museum_/_art_gallery'}, //  Count: 5255
        "portrait gallery":{'rawWmCategory':'portrait_gallery'}, //  Count: 5255
        "coffee - to be replaced":{'rawWmCategory':'coffee_-_to_be_replaced'}, //  Count: 5238
        "public institution":{'rawWmCategory':'public_institution'}, //  Count: 5232
        "neighbourhood development":{'rawWmCategory':'neighbourhood_development'}, //  Count: 5221
        "nuraghe":{'rawWmCategory':'nuraghe'}, //  Count: 5191
        "wedding / marriage garden":{'rawWmCategory':'wedding_/_marriage_garden'}, //  Count: 5188
        "matrimonial hall / venue":{'rawWmCategory':'matrimonial_hall_/_venue'}, //  Count: 5188
        "wedding / marriage venue":{'rawWmCategory':'wedding_/_marriage_venue'}, //  Count: 5188
        "wedding / marriage hall":{'rawWmCategory':'wedding_/_marriage_hall'}, //  Count: 5188
        "ship":{'rawWmCategory':'ship'}, //  Count: 5150
        "boat":{'rawWmCategory':'boat'}, //  Count: 5134
        "open-air swimming pool":{'rawWmCategory':'open-air_swimming_pool'}, //  Count: 5130
        "outdoor swimming pool":{'rawWmCategory':'outdoor_swimming_pool'}, //  Count: 5130
        "recycling centre / collection":{'rawWmCategory':'recycling_centre_/_collection'}, //  Count: 5107
        "branch office of Jehovah's Witnesses":{'rawWmCategory':'branch_office_of_Jehovahs_Witnesses'}, //  Count: 5098
        "Kingdom Hall of Jehovah's Witnesses":{'rawWmCategory':'Kingdom_Hall_of_Jehovahs_Witnesses'}, //  Count: 5098
        "assembly hall of Jehovah's Witnesses":{'rawWmCategory':'assembly_hall_of_Jehovahs_Witnesses'}, //  Count: 5098
        "gift shop":{'rawWmCategory':'gift_shop'}, //  Count: 5080
        "souvenir shop":{'rawWmCategory':'souvenir_shop'}, //  Count: 5080
        "cottage complex":{'rawWmCategory':'cottage_complex'}, //  Count: 5076
        "brickworks":{'rawWmCategory':'brickworks'}, //  Count: 5072
        "brick factory":{'rawWmCategory':'brick_factory'}, //  Count: 5072
        "brick kiln":{'rawWmCategory':'brick_kiln'}, //  Count: 5072
        "brickyard":{'rawWmCategory':'brickyard'}, //  Count: 5072
        "lawyer":{'rawWmCategory':'lawyer'}, //  Count: 5048
        "attorney":{'rawWmCategory':'attorney'}, //  Count: 5048
        "attorneys":{'rawWmCategory':'attorneys'}, //  Count: 5048
        "solicitors":{'rawWmCategory':'solicitors'}, //  Count: 5048
        "law firm":{'rawWmCategory':'law_firm'}, //  Count: 5048
        "telephone kiosk":{'rawWmCategory':'telephone_kiosk'}, //  Count: 5019
        "phone box":{'rawWmCategory':'phone_box'}, //  Count: 5019
        "public telephone":{'rawWmCategory':'public_telephone'}, //  Count: 5019
        "payphone":{'rawWmCategory':'payphone'}, //  Count: 5019
        "phone booth":{'rawWmCategory':'phone_booth'}, //  Count: 5019
        "management":{'rawWmCategory':'management'}, //  Count: 5012
        "optician":{'rawWmCategory':'optician'}, //  Count: 4996
        "spectacles":{'rawWmCategory':'spectacles'}, //  Count: 4996
        "contact lens store":{'rawWmCategory':'contact_lens_store'}, //  Count: 4996
        "optical retailer":{'rawWmCategory':'optical_retailer'}, //  Count: 4996
        "eye care shop":{'rawWmCategory':'eye_care_shop'}, //  Count: 4996
        "border":{'rawWmCategory':'border'}, //  Count: 4977
        "doctor":{'rawWmCategory':'doctor'}, //  Count: 4976
        "tourism":{'rawWmCategory':'tourism'}, //  Count: 4969
        "unincorporated area / community":{'rawWmCategory':'unincorporated_area_/_community'}, //  Count: 4955
        "open 24 hours":{'rawWmCategory':'open_24_hours'}, //  Count: 4951
        "telephone exchange building":{'rawWmCategory':'telephone_exchange_building'}, //  Count: 4943
        "charitable organization":{'rawWmCategory':'charitable_organization'}, //  Count: 4914
        "charitable organisation":{'rawWmCategory':'charitable_organisation'}, //  Count: 4914
        "social club":{'rawWmCategory':'social_club'}, //  Count: 4907
        "strip mall":{'rawWmCategory':'strip_mall'}, //  Count: 4902
        "Électricité de France":{'rawWmCategory':'Électricité_de_France'}, //  Count: 4902
        "shopping plaza":{'rawWmCategory':'shopping_plaza'}, //  Count: 4902
        "mini-mall":{'rawWmCategory':'mini-mall'}, //  Count: 4902
        "gun emplacement":{'rawWmCategory':'gun_emplacement'}, //  Count: 4901
        "helipad":{'rawWmCategory':'helipad'}, //  Count: 4874
        "helicopter landing pad":{'rawWmCategory':'helicopter_landing_pad'}, //  Count: 4874
        "reef":{'rawWmCategory':'reef'}, //  Count: 4866
        "manufacturing":{'rawWmCategory':'manufacturing'}, //  Count: 4862
        "manufacturer":{'rawWmCategory':'manufacturer'}, //  Count: 4862
        "destroyed":{'rawWmCategory':'destroyed'}, //  Count: 4838
        "Baptist church":{'rawWmCategory':'Baptist_church'}, //  Count: 4810
        "extreme / adventure sport":{'rawWmCategory':'extreme_/_adventure_sport'}, //  Count: 4810
        "ranch":{'rawWmCategory':'ranch'}, //  Count: 4775
        "heliport":{'rawWmCategory':'heliport'}, //  Count: 4774
        "pillbox":{'rawWmCategory':'pillbox'}, //  Count: 4768
        "used car dealer / dealership":{'rawWmCategory':'used_car_dealer_/_dealership'}, //  Count: 4753
        "cricket oval":{'rawWmCategory':'cricket_oval'}, //  Count: 4693
        "cricket field":{'rawWmCategory':'cricket_field'}, //  Count: 4693
        "cricket ground":{'rawWmCategory':'cricket_ground'}, //  Count: 4693
        "plot - to be deleted":{'rawWmCategory':'plot_-_to_be_deleted'}, //  Count: 4673
        "electronic components":{'rawWmCategory':'electronic_components'}, //  Count: 4665
        "electronics":{'rawWmCategory':'electronics'}, //  Count: 4665
        "public housing estate":{'rawWmCategory':'public_housing_estate'}, //  Count: 4645
        "tree":{'rawWmCategory':'tree'}, //  Count: 4624
        "horseback riding":{'rawWmCategory':'horseback_riding'}, //  Count: 4602
        "horse riding":{'rawWmCategory':'horse_riding'}, //  Count: 4602
        "pavilion":{'rawWmCategory':'pavilion'}, //  Count: 4602
        "equitation":{'rawWmCategory':'equitation'}, //  Count: 4602
        "equestrianism":{'rawWmCategory':'equestrianism'}, //  Count: 4602
        "ordnance depot":{'rawWmCategory':'ordnance_depot'}, //  Count: 4584
        "arsenal / weapon and ammunition storage":{'rawWmCategory':'arsenal_/_weapon_and_ammunition_storage'}, //  Count: 4584
        "gas distribution station":{'rawWmCategory':'gas_distribution_station'}, //  Count: 4559
        "2000s construction":{'rawWmCategory':'2000s_construction'}, //  Count: 4558
        "township":{'rawWmCategory':'township'}, //  Count: 4553
        "unfinished / unbuilt":{'rawWmCategory':'unfinished_/_unbuilt'}, //  Count: 4506
        "underpass":{'rawWmCategory':'underpass'}, //  Count: 4489
        "marinas":{'rawWmCategory':'marinas'}, //  Count: 4485
        "marina":{'rawWmCategory':'marina'}, //  Count: 4485
        "chocolatier":{'rawWmCategory':'chocolatier'}, //  Count: 4467
        "confectionery":{'rawWmCategory':'confectionery'}, //  Count: 4467
        "sweets / sweet shop":{'rawWmCategory':'sweets_/_sweet_shop'}, //  Count: 4467
        "candy shop":{'rawWmCategory':'candy_shop'}, //  Count: 4467
        "palm house":{'rawWmCategory':'palm_house'}, //  Count: 4429
        "greenhouse":{'rawWmCategory':'greenhouse'}, //  Count: 4429
        "glasshouse":{'rawWmCategory':'glasshouse'}, //  Count: 4429
        "berth":{'rawWmCategory':'berth'}, //  Count: 4374
        "radio station":{'rawWmCategory':'radio_station'}, //  Count: 4370
        "ventilating shaft":{'rawWmCategory':'ventilating_shaft'}, //  Count: 4366
        "social security":{'rawWmCategory':'social_security'}, //  Count: 4366
        "pet shop/store":{'rawWmCategory':'pet_shop/store'}, //  Count: 4364
        "suburb":{'rawWmCategory':'suburb'}, //  Count: 4360
        "zoological garden":{'rawWmCategory':'zoological_garden'}, //  Count: 4353
        "zoo":{'rawWmCategory':'zoo'}, //  Count: 4353
        "lutheranism":{'rawWmCategory':'lutheranism'}, //  Count: 4344
        "mountain pass":{'rawWmCategory':'mountain_pass'}, //  Count: 4342
        "navy":{'rawWmCategory':'navy'}, //  Count: 4339
        "toll booth":{'rawWmCategory':'toll_booth'}, //  Count: 4331
        "toll gate":{'rawWmCategory':'toll_gate'}, //  Count: 4331
        "closed":{'rawWmCategory':'closed'}, //  Count: 4323
        "civil defense facility":{'rawWmCategory':'civil_defense_facility'}, //  Count: 4320
        "civil protection":{'rawWmCategory':'civil_protection'}, //  Count: 4320
        "perfumes":{'rawWmCategory':'perfumes'}, //  Count: 4313
        "perfume / cosmetics shop":{'rawWmCategory':'perfume_/_cosmetics_shop'}, //  Count: 4313
        "cosmetics":{'rawWmCategory':'cosmetics'}, //  Count: 4313
        "cultural center / centre":{'rawWmCategory':'cultural_center_/_centre'}, //  Count: 4305
        "driving school":{'rawWmCategory':'driving_school'}, //  Count: 4274
        "shipbuilding":{'rawWmCategory':'shipbuilding'}, //  Count: 4272
        "Mediterranean cuisine":{'rawWmCategory':'Mediterranean_cuisine'}, //  Count: 4258
        "frazione":{'rawWmCategory':'frazione'}, //  Count: 4254
        "flower bed":{'rawWmCategory':'flower_bed'}, //  Count: 4236
        "barge slip":{'rawWmCategory':'barge_slip'}, //  Count: 4233
        "ferry terminal":{'rawWmCategory':'ferry_terminal'}, //  Count: 4233
        "ferry slip":{'rawWmCategory':'ferry_slip'}, //  Count: 4233
        "cavern":{'rawWmCategory':'cavern'}, //  Count: 4230
        "cave":{'rawWmCategory':'cave'}, //  Count: 4230
        "aparthotel":{'rawWmCategory':'aparthotel'}, //  Count: 4224
        "apartment hotel / serviced apartment":{'rawWmCategory':'apartment_hotel_/_serviced_apartment'}, //  Count: 4224
        "apartelle":{'rawWmCategory':'apartelle'}, //  Count: 4224
        "crash site":{'rawWmCategory':'crash_site'}, //  Count: 4221
        "sanatorium":{'rawWmCategory':'sanatorium'}, //  Count: 4220
        "computer services":{'rawWmCategory':'computer_services'}, //  Count: 4210
        "ministry / government department":{'rawWmCategory':'ministry_/_government_department'}, //  Count: 4200
        "hypermarket":{'rawWmCategory':'hypermarket'}, //  Count: 4169
        "superstore":{'rawWmCategory':'superstore'}, //  Count: 4169
        "megastore":{'rawWmCategory':'megastore'}, //  Count: 4169
        "supercenter":{'rawWmCategory':'supercenter'}, //  Count: 4169
        "big-box store":{'rawWmCategory':'big-box_store'}, //  Count: 4169
        "butcher's shop":{'rawWmCategory':'butchers_shop'}, //  Count: 4168
        "butcher shop / butchery":{'rawWmCategory':'butcher_shop_/_butchery'}, //  Count: 4168
        "department":{'rawWmCategory':'department'}, //  Count: 4165
        "christian school":{'rawWmCategory':'christian_school'}, //  Count: 4128
        "hydroelectric power station":{'rawWmCategory':'hydroelectric_power_station'}, //  Count: 4120
        "gully":{'rawWmCategory':'gully'}, //  Count: 4091
        "baseball field":{'rawWmCategory':'baseball_field'}, //  Count: 4090
        "Walmart":{'rawWmCategory':'Walmart'}, //  Count: 4086
        "Wal-mart":{'rawWmCategory':'Wal-mart'}, //  Count: 4086
        "jewellery store/shop":{'rawWmCategory':'jewellery_store/shop'}, //  Count: 4082
        "jewelry store/shop":{'rawWmCategory':'jewelry_store/shop'}, //  Count: 4082
        "lock (water transport)":{'rawWmCategory':'lock_(water_transport)'}, //  Count: 4028
        "aircraft shelter":{'rawWmCategory':'aircraft_shelter'}, //  Count: 4010
        "aircraft hangar":{'rawWmCategory':'aircraft_hangar'}, //  Count: 4010
        "sector":{'rawWmCategory':'sector'}, //  Count: 4005
        "coal industry":{'rawWmCategory':'coal_industry'}, //  Count: 3995
        "interstate":{'rawWmCategory':'interstate'}, //  Count: 3981
        "1990s construction":{'rawWmCategory':'1990s_construction'}, //  Count: 3968
        "photographic studio":{'rawWmCategory':'photographic_studio'}, //  Count: 3953
        "Japanese cuisine":{'rawWmCategory':'Japanese_cuisine'}, //  Count: 3936
        "volleyball court":{'rawWmCategory':'volleyball_court'}, //  Count: 3936
        "pier":{'rawWmCategory':'pier'}, //  Count: 3933
        "Lutheran Church":{'rawWmCategory':'Lutheran_Church'}, //  Count: 3931
        "eid prayer ground":{'rawWmCategory':'eid_prayer_ground'}, //  Count: 3928
        "eidgah":{'rawWmCategory':'eidgah'}, //  Count: 3928
        "laundromat":{'rawWmCategory':'laundromat'}, //  Count: 3910
        "laundry":{'rawWmCategory':'laundry'}, //  Count: 3910
        "laundrette":{'rawWmCategory':'laundrette'}, //  Count: 3910
        "toy shop":{'rawWmCategory':'toy_shop'}, //  Count: 3893
        "bungalow":{'rawWmCategory':'bungalow'}, //  Count: 3882
        "association":{'rawWmCategory':'association'}, //  Count: 3866
        "engineering":{'rawWmCategory':'engineering'}, //  Count: 3865
        "vehicle inspection":{'rawWmCategory':'vehicle_inspection'}, //  Count: 3864
        "valley":{'rawWmCategory':'valley'}, //  Count: 3863
        "summer camp":{'rawWmCategory':'summer_camp'}, //  Count: 3853
        "river mouth":{'rawWmCategory':'river_mouth'}, //  Count: 3851
        "estuary":{'rawWmCategory':'estuary'}, //  Count: 3851
        "lodge":{'rawWmCategory':'lodge'}, //  Count: 3824
        "professional sports leagues":{'rawWmCategory':'professional_sports_leagues'}, //  Count: 3803
        "antenna":{'rawWmCategory':'antenna'}, //  Count: 3791
        "retirement home":{'rawWmCategory':'retirement_home'}, //  Count: 3790
        "senior citizen home":{'rawWmCategory':'senior_citizen_home'}, //  Count: 3790
        "ICBM launch facility":{'rawWmCategory':'ICBM_launch_facility'}, //  Count: 3784
        "missile launch facility":{'rawWmCategory':'missile_launch_facility'}, //  Count: 3784
        "missile silo":{'rawWmCategory':'missile_silo'}, //  Count: 3784
        "churchyard":{'rawWmCategory':'churchyard'}, //  Count: 3775
        "[oil and gas companies]":{'rawWmCategory':'[oil_and_gas_companies]'}, //  Count: 3760
        "cattle ranch":{'rawWmCategory':'cattle_ranch'}, //  Count: 3746
        "cattle farm":{'rawWmCategory':'cattle_farm'}, //  Count: 3746
        "camp":{'rawWmCategory':'camp'}, //  Count: 3742
        "bottle bank":{'rawWmCategory':'bottle_bank'}, //  Count: 3737
        "waste collection point / civic amenity centre / drop-off centre":{'rawWmCategory':'waste_collection_point_/_civic_amenity_centre_/_drop-off_centre'}, //  Count: 3737
        "railway company":{'rawWmCategory':'railway_company'}, //  Count: 3733
        "railway operator":{'rawWmCategory':'railway_operator'}, //  Count: 3733
        "care home":{'rawWmCategory':'care_home'}, //  Count: 3730
        "nursing home":{'rawWmCategory':'nursing_home'}, //  Count: 3730
        "alternative medicine":{'rawWmCategory':'alternative_medicine'}, //  Count: 3728
        "bus depot":{'rawWmCategory':'bus_depot'}, //  Count: 3719
        "bus garage":{'rawWmCategory':'bus_garage'}, //  Count: 3719
        "ice arena":{'rawWmCategory':'ice_arena'}, //  Count: 3707
        "ice skating rink":{'rawWmCategory':'ice_skating_rink'}, //  Count: 3707
        "ice hockey rink":{'rawWmCategory':'ice_hockey_rink'}, //  Count: 3707
        "ice rink":{'rawWmCategory':'ice_rink'}, //  Count: 3707
        "mound":{'rawWmCategory':'mound'}, //  Count: 3678
        "outbuilding":{'rawWmCategory':'outbuilding'}, //  Count: 3667
        "cross":{'rawWmCategory':'cross'}, //  Count: 3665
        "wall":{'rawWmCategory':'wall'}, //  Count: 3650
        "newspaper publisher":{'rawWmCategory':'newspaper_publisher'}, //  Count: 3644
        "bake shop":{'rawWmCategory':'bake_shop'}, //  Count: 3640
        "patisserie":{'rawWmCategory':'patisserie'}, //  Count: 3640
        "pastry shop":{'rawWmCategory':'pastry_shop'}, //  Count: 3640
        "printing":{'rawWmCategory':'printing'}, //  Count: 3636
        "domestic dogs":{'rawWmCategory':'domestic_dogs'}, //  Count: 3628
        "border guard":{'rawWmCategory':'border_guard'}, //  Count: 3623
        "landfill site":{'rawWmCategory':'landfill_site'}, //  Count: 3618
        "volcano":{'rawWmCategory':'volcano'}, //  Count: 3617
        "gazebo":{'rawWmCategory':'gazebo'}, //  Count: 3614
        "heritage":{'rawWmCategory':'heritage'}, //  Count: 3606
        "drive-in/ drive-through":{'rawWmCategory':'drive-in/_drive-through'}, //  Count: 3597
        "Chinese cuisine":{'rawWmCategory':'Chinese_cuisine'}, //  Count: 3590
        "drinking fountain":{'rawWmCategory':'drinking_fountain'}, //  Count: 3585
        "water pump":{'rawWmCategory':'water_pump'}, //  Count: 3585
        "civil parish":{'rawWmCategory':'civil_parish'}, //  Count: 3582
        "parish":{'rawWmCategory':'parish'}, //  Count: 3582
        "TV":{'rawWmCategory':'TV'}, //  Count: 3582
        "television":{'rawWmCategory':'television'}, //  Count: 3582
        "training center":{'rawWmCategory':'training_center'}, //  Count: 3571
        "training centre":{'rawWmCategory':'training_centre'}, //  Count: 3571
        "snack bar / concession stand":{'rawWmCategory':'snack_bar_/_concession_stand'}, //  Count: 3553
        "CDP - Census Designated Place":{'rawWmCategory':'CDP_-_Census_Designated_Place'}, //  Count: 3549
        "highway patrol":{'rawWmCategory':'highway_patrol'}, //  Count: 3546
        "traffic police":{'rawWmCategory':'traffic_police'}, //  Count: 3546
        "grain elevator / grain silos":{'rawWmCategory':'grain_elevator_/_grain_silos'}, //  Count: 3528
        "billboard":{'rawWmCategory':'billboard'}, //  Count: 3517
        "music school":{'rawWmCategory':'music_school'}, //  Count: 3495
        "shrine":{'rawWmCategory':'shrine'}, //  Count: 3488
        "Shiv / Mahadev mandir":{'rawWmCategory':'Shiv_/_Mahadev_mandir'}, //  Count: 3480
        "Shankar temple":{'rawWmCategory':'Shankar_temple'}, //  Count: 3480
        "Sivalayam":{'rawWmCategory':'Sivalayam'}, //  Count: 3480
        "Shiva temple":{'rawWmCategory':'Shiva_temple'}, //  Count: 3480
        "2010s construction":{'rawWmCategory':'2010s_construction'}, //  Count: 3462
        "electricity - to be replaced / deleted":{'rawWmCategory':'electricity_-_to_be_replaced_/_deleted'}, //  Count: 3445
        "demolished":{'rawWmCategory':'demolished'}, //  Count: 3443
        "computer shop":{'rawWmCategory':'computer_shop'}, //  Count: 3440
        "orchard / fruit garden":{'rawWmCategory':'orchard_/_fruit_garden'}, //  Count: 3436
        "fruit farm":{'rawWmCategory':'fruit_farm'}, //  Count: 3436
        "second-level administrative division":{'rawWmCategory':'second-level_administrative_division'}, //  Count: 3429
        "boarding school":{'rawWmCategory':'boarding_school'}, //  Count: 3411
        "hall (Don't use this category, instead help removing it by refining)":{'rawWmCategory':'hall_(Dont_use_this_category,_instead_help_removing_it_by_refining)'}, //  Count: 3402
        "quarry pond":{'rawWmCategory':'quarry_pond'}, //  Count: 3389
        "evangelicalism":{'rawWmCategory':'evangelicalism'}, //  Count: 3384
        "well_(do not use this category)":{'rawWmCategory':'well_(do_not_use_this_category)'}, //  Count: 3370
        "geodesy":{'rawWmCategory':'geodesy'}, //  Count: 3359
        "geodetic point":{'rawWmCategory':'geodetic_point'}, //  Count: 3359
        "research":{'rawWmCategory':'research'}, //  Count: 3358
        "dump":{'rawWmCategory':'dump'}, //  Count: 3356
        "dacha":{'rawWmCategory':'dacha'}, //  Count: 3354
        "cathedral":{'rawWmCategory':'cathedral'}, //  Count: 3340
        "arts centre":{'rawWmCategory':'arts_centre'}, //  Count: 3299
        "arts center":{'rawWmCategory':'arts_center'}, //  Count: 3299
        "poultry farm":{'rawWmCategory':'poultry_farm'}, //  Count: 3292
        "poultry farming":{'rawWmCategory':'poultry_farming'}, //  Count: 3292
        "stationers":{'rawWmCategory':'stationers'}, //  Count: 3291
        "stationery store / shop":{'rawWmCategory':'stationery_store_/_shop'}, //  Count: 3291
        "service center":{'rawWmCategory':'service_center'}, //  Count: 3283
        "chowk (town square)":{'rawWmCategory':'chowk_(town_square)'}, //  Count: 3272
        "town square":{'rawWmCategory':'town_square'}, //  Count: 3272
        "townhouses":{'rawWmCategory':'townhouses'}, //  Count: 3263
        "townhomes":{'rawWmCategory':'townhomes'}, //  Count: 3263
        "distribution center":{'rawWmCategory':'distribution_center'}, //  Count: 3262
        "distribution centre":{'rawWmCategory':'distribution_centre'}, //  Count: 3262
        "distributor":{'rawWmCategory':'distributor'}, //  Count: 3262
        "architecture - Do not use this category":{'rawWmCategory':'architecture_-_Do_not_use_this_category'}, //  Count: 3256
        "boarding house":{'rawWmCategory':'boarding_house'}, //  Count: 3252
        "bell chair":{'rawWmCategory':'bell_chair'}, //  Count: 3230
        "bell tower":{'rawWmCategory':'bell_tower'}, //  Count: 3230
        "belfry":{'rawWmCategory':'belfry'}, //  Count: 3230
        "shanty town":{'rawWmCategory':'shanty_town'}, //  Count: 3203
        "slum":{'rawWmCategory':'slum'}, //  Count: 3203
        "lagoon":{'rawWmCategory':'lagoon'}, //  Count: 3179
        "Maginot line":{'rawWmCategory':'Maginot_line'}, //  Count: 3151
        "entertainment":{'rawWmCategory':'entertainment'}, //  Count: 3147
        "vineyard / grape plantation":{'rawWmCategory':'vineyard_/_grape_plantation'}, //  Count: 3139
        "judaism":{'rawWmCategory':'judaism'}, //  Count: 3093
        "station building (railway)":{'rawWmCategory':'station_building_(railway)'}, //  Count: 3091
        "head house (train station)":{'rawWmCategory':'head_house_(train_station)'}, //  Count: 3091
        "cab rank":{'rawWmCategory':'cab_rank'}, //  Count: 3090
        "taxi rank":{'rawWmCategory':'taxi_rank'}, //  Count: 3090
        "taxi stop":{'rawWmCategory':'taxi_stop'}, //  Count: 3090
        "hack stand":{'rawWmCategory':'hack_stand'}, //  Count: 3090
        "cab stand":{'rawWmCategory':'cab_stand'}, //  Count: 3090
        "taxicab stand":{'rawWmCategory':'taxicab_stand'}, //  Count: 3090
        "taxi stand":{'rawWmCategory':'taxi_stand'}, //  Count: 3090
        "lawn":{'rawWmCategory':'lawn'}, //  Count: 3087
        "geography":{'rawWmCategory':'geography'}, //  Count: 3075
        "geographical":{'rawWmCategory':'geographical'}, //  Count: 3075
        "golf hole":{'rawWmCategory':'golf_hole'}, //  Count: 3043
        "Mexican cuisine":{'rawWmCategory':'Mexican_cuisine'}, //  Count: 3040
        "viaduct (bridge construction style)":{'rawWmCategory':'viaduct_(bridge_construction_style)'}, //  Count: 3025
        "internet company":{'rawWmCategory':'internet_company'}, //  Count: 3020
        "sports school":{'rawWmCategory':'sports_school'}, //  Count: 3011
        "ancient ruins":{'rawWmCategory':'ancient_ruins'}, //  Count: 3000
        "historic ruins":{'rawWmCategory':'historic_ruins'}, //  Count: 3000
        "ambulance station":{'rawWmCategory':'ambulance_station'}, //  Count: 2997
        "ship wreck":{'rawWmCategory':'ship_wreck'}, //  Count: 2986
        "shipwreck":{'rawWmCategory':'shipwreck'}, //  Count: 2986
        "classroom":{'rawWmCategory':'classroom'}, //  Count: 2982
        "classrooms":{'rawWmCategory':'classrooms'}, //  Count: 2982
        "Catholic school":{'rawWmCategory':'Catholic_school'}, //  Count: 2965
        "futsal court":{'rawWmCategory':'futsal_court'}, //  Count: 2963
        "tourist information board":{'rawWmCategory':'tourist_information_board'}, //  Count: 2945
        "tourist information centre/center":{'rawWmCategory':'tourist_information_centre/center'}, //  Count: 2945
        "Italian cuisine":{'rawWmCategory':'Italian_cuisine'}, //  Count: 2943
        "cargo transportation":{'rawWmCategory':'cargo_transportation'}, //  Count: 2915
        "premises":{'rawWmCategory':'premises'}, //  Count: 2913
        "taxiway":{'rawWmCategory':'taxiway'}, //  Count: 2911
        "fallout shelter":{'rawWmCategory':'fallout_shelter'}, //  Count: 2904
        "bombshelter":{'rawWmCategory':'bombshelter'}, //  Count: 2904
        "country club":{'rawWmCategory':'country_club'}, //  Count: 2900
        "ticket / box office":{'rawWmCategory':'ticket_/_box_office'}, //  Count: 2899
        "Sberbank":{'rawWmCategory':'Sberbank'}, //  Count: 2895
        "barn":{'rawWmCategory':'barn'}, //  Count: 2889
        "pawnshop / pawnbroker":{'rawWmCategory':'pawnshop_/_pawnbroker'}, //  Count: 2884
        "airport terminal":{'rawWmCategory':'airport_terminal'}, //  Count: 2883
        "motorcycle dealer / shop":{'rawWmCategory':'motorcycle_dealer_/_shop'}, //  Count: 2873
        "picnic area":{'rawWmCategory':'picnic_area'}, //  Count: 2873
        "camera shop":{'rawWmCategory':'camera_shop'}, //  Count: 2864
        "photography shop":{'rawWmCategory':'photography_shop'}, //  Count: 2864
        "garden center":{'rawWmCategory':'garden_center'}, //  Count: 2861
        "garden centre":{'rawWmCategory':'garden_centre'}, //  Count: 2861
        "non-profit organisation":{'rawWmCategory':'non-profit_organisation'}, //  Count: 2856
        "non-profit organization":{'rawWmCategory':'non-profit_organization'}, //  Count: 2856
        "accommodation":{'rawWmCategory':'accommodation'}, //  Count: 2854
        "closed - military":{'rawWmCategory':'closed_-_military'}, //  Count: 2849
        "consultation":{'rawWmCategory':'consultation'}, //  Count: 2841
        "consultancy":{'rawWmCategory':'consultancy'}, //  Count: 2841
        "consulting":{'rawWmCategory':'consulting'}, //  Count: 2841
        "silo (structure for storing bulk materials)":{'rawWmCategory':'silo_(structure_for_storing_bulk_materials)'}, //  Count: 2837
        "bowling alley":{'rawWmCategory':'bowling_alley'}, //  Count: 2820
        "bowling lanes":{'rawWmCategory':'bowling_lanes'}, //  Count: 2820
        "bowling (pin)":{'rawWmCategory':'bowling_(pin)'}, //  Count: 2820
        "boliche":{'rawWmCategory':'boliche'}, //  Count: 2820
        "water catchment area":{'rawWmCategory':'water_catchment_area'}, //  Count: 2810
        "wind power plant":{'rawWmCategory':'wind_power_plant'}, //  Count: 2767
        "wind farm":{'rawWmCategory':'wind_farm'}, //  Count: 2767
        "fuel depot":{'rawWmCategory':'fuel_depot'}, //  Count: 2760
        "oil terminal":{'rawWmCategory':'oil_terminal'}, //  Count: 2760
        "oil depot":{'rawWmCategory':'oil_depot'}, //  Count: 2760
        "tank farm":{'rawWmCategory':'tank_farm'}, //  Count: 2760
        "[military base]":{'rawWmCategory':'[military_base]'}, //  Count: 2758
        "base level":{'rawWmCategory':'base_level'}, //  Count: 2758
        "windmill":{'rawWmCategory':'windmill'}, //  Count: 2756
        "non-ministerial government department (UK)":{'rawWmCategory':'non-ministerial_government_department_(UK)'}, //  Count: 2752
        "convention center":{'rawWmCategory':'convention_center'}, //  Count: 2750
        "convention centre":{'rawWmCategory':'convention_centre'}, //  Count: 2750
        "congress centre":{'rawWmCategory':'congress_centre'}, //  Count: 2750
        "conference centre":{'rawWmCategory':'conference_centre'}, //  Count: 2750
        "water purification plant":{'rawWmCategory':'water_purification_plant'}, //  Count: 2747
        "water supply treatment plant":{'rawWmCategory':'water_supply_treatment_plant'}, //  Count: 2747
        "cooling tower":{'rawWmCategory':'cooling_tower'}, //  Count: 2744
        "HM revenue and customs (UK)":{'rawWmCategory':'HM_revenue_and_customs_(UK)'}, //  Count: 2742
        "internal revenue service":{'rawWmCategory':'internal_revenue_service'}, //  Count: 2742
        "tax authority":{'rawWmCategory':'tax_authority'}, //  Count: 2742
        "sauna":{'rawWmCategory':'sauna'}, //  Count: 2732
        "village hall":{'rawWmCategory':'village_hall'}, //  Count: 2709
        "[hotel chains]":{'rawWmCategory':'[hotel_chains]'}, //  Count: 2699
        "ford (river crossing)":{'rawWmCategory':'ford_(river_crossing)'}, //  Count: 2693
        "auditorium":{'rawWmCategory':'auditorium'}, //  Count: 2688
        "compound":{'rawWmCategory':'compound'}, //  Count: 2683
        "local events":{'rawWmCategory':'local_events'}, //  Count: 2674
        "winery":{'rawWmCategory':'winery'}, //  Count: 2672
        "atelier":{'rawWmCategory':'atelier'}, //  Count: 2662
        "mini storage":{'rawWmCategory':'mini_storage'}, //  Count: 2661
        "self storage":{'rawWmCategory':'self_storage'}, //  Count: 2661
        "chemical industry":{'rawWmCategory':'chemical_industry'}, //  Count: 2657
        "nursery":{'rawWmCategory':'nursery'}, //  Count: 2657
        "snooker hall / room / club / parlour":{'rawWmCategory':'snooker_hall_/_room_/_club_/_parlour'}, //  Count: 2655
        "pool hall / room / parlor / club":{'rawWmCategory':'pool_hall_/_room_/_parlor_/_club'}, //  Count: 2655
        "billiard hall / room / club / parlour":{'rawWmCategory':'billiard_hall_/_room_/_club_/_parlour'}, //  Count: 2655
        "parking ramp":{'rawWmCategory':'parking_ramp'}, //  Count: 2653
        "parking garage":{'rawWmCategory':'parking_garage'}, //  Count: 2653
        "multi-storey car park":{'rawWmCategory':'multi-storey_car_park'}, //  Count: 2653
        "18th century construction":{'rawWmCategory':'18th_century_construction'}, //  Count: 2649
        "employment / recruitment / jobs agency":{'rawWmCategory':'employment_/_recruitment_/_jobs_agency'}, //  Count: 2643
        "staffing agency":{'rawWmCategory':'staffing_agency'}, //  Count: 2643
        "brewery":{'rawWmCategory':'brewery'}, //  Count: 2643
        "private school":{'rawWmCategory':'private_school'}, //  Count: 2642
        "nonstate school":{'rawWmCategory':'nonstate_school'}, //  Count: 2642
        "independent school":{'rawWmCategory':'independent_school'}, //  Count: 2642
        "school of arts and design / college of arts and design":{'rawWmCategory':'school_of_arts_and_design_/_college_of_arts_and_design'}, //  Count: 2630
        "cove":{'rawWmCategory':'cove'}, //  Count: 2597
        "ski":{'rawWmCategory':'ski'}, //  Count: 2556
        "height":{'rawWmCategory':'height'}, //  Count: 2553
        "home appliance store / shop":{'rawWmCategory':'home_appliance_store_/_shop'}, //  Count: 2549
        "rifle range":{'rawWmCategory':'rifle_range'}, //  Count: 2532
        "shooting range":{'rawWmCategory':'shooting_range'}, //  Count: 2532
        "firing range":{'rawWmCategory':'firing_range'}, //  Count: 2532
        "Lenin":{'rawWmCategory':'Lenin'}, //  Count: 2530
        "bust (sculpture)":{'rawWmCategory':'bust_(sculpture)'}, //  Count: 2528
        "surface-to-air missile site / sam":{'rawWmCategory':'surface-to-air_missile_site_/_sam'}, //  Count: 2517
        "stone":{'rawWmCategory':'stone'}, //  Count: 2505
        "UNESCO World Heritage Site":{'rawWmCategory':'UNESCO_World_Heritage_Site'}, //  Count: 2504
        "car hire agency":{'rawWmCategory':'car_hire_agency'}, //  Count: 2496
        "automobile rental agency":{'rawWmCategory':'automobile_rental_agency'}, //  Count: 2496
        "car rental agency":{'rawWmCategory':'car_rental_agency'}, //  Count: 2496
        "rent-a-car agency":{'rawWmCategory':'rent-a-car_agency'}, //  Count: 2496
        "high-rise":{'rawWmCategory':'high-rise'}, //  Count: 2496
        "haulage service":{'rawWmCategory':'haulage_service'}, //  Count: 2482
        "logistics":{'rawWmCategory':'logistics'}, //  Count: 2482
        "Islamic cemetery":{'rawWmCategory':'Islamic_cemetery'}, //  Count: 2478
        "Muslim cemetery":{'rawWmCategory':'Muslim_cemetery'}, //  Count: 2478
        "qabristan":{'rawWmCategory':'qabristan'}, //  Count: 2478
        "furniture factory / maker":{'rawWmCategory':'furniture_factory_/_maker'}, //  Count: 2452
        "igloo":{'rawWmCategory':'igloo'}, //  Count: 2441
        "borough":{'rawWmCategory':'borough'}, //  Count: 2435
        "The district center":{'rawWmCategory':'The_district_center'}, //  Count: 2433
        "adventure/activity centre":{'rawWmCategory':'adventure/activity_centre'}, //  Count: 2431
        "radar":{'rawWmCategory':'radar'}, //  Count: 2425
        "gents":{'rawWmCategory':'gents'}, //  Count: 2424
        "mens":{'rawWmCategory':'mens'}, //  Count: 2424
        "customs":{'rawWmCategory':'customs'}, //  Count: 2419
        "customs house / area / checkpoint":{'rawWmCategory':'customs_house_/_area_/_checkpoint'}, //  Count: 2419
        "traction and rolling stock maintenance depot":{'rawWmCategory':'traction_and_rolling_stock_maintenance_depot'}, //  Count: 2404
        "traction maintenance depot (TMD)":{'rawWmCategory':'traction_maintenance_depot_(TMD)'}, //  Count: 2404
        "railway depot":{'rawWmCategory':'railway_depot'}, //  Count: 2404
        "motive power depot (MPD)":{'rawWmCategory':'motive_power_depot_(MPD)'}, //  Count: 2404
        "expocentre":{'rawWmCategory':'expocentre'}, //  Count: 2404
        "locomotive depot":{'rawWmCategory':'locomotive_depot'}, //  Count: 2404
        "exhibition center":{'rawWmCategory':'exhibition_center'}, //  Count: 2404
        "exhibition centre":{'rawWmCategory':'exhibition_centre'}, //  Count: 2404
        "oxbow lake":{'rawWmCategory':'oxbow_lake'}, //  Count: 2403
        "sandpit (quarry)":{'rawWmCategory':'sandpit_(quarry)'}, //  Count: 2395
        "hut":{'rawWmCategory':'hut'}, //  Count: 2390
        "Roman Empire":{'rawWmCategory':'Roman_Empire'}, //  Count: 2389
        "romans":{'rawWmCategory':'romans'}, //  Count: 2389
        "seafood restaurant":{'rawWmCategory':'seafood_restaurant'}, //  Count: 2388
        "[air force]":{'rawWmCategory':'[air_force]'}, //  Count: 2387
        "armoured fighting vehicle on display":{'rawWmCategory':'armoured_fighting_vehicle_on_display'}, //  Count: 2387
        "tank on display":{'rawWmCategory':'tank_on_display'}, //  Count: 2387
        "radio":{'rawWmCategory':'radio'}, //  Count: 2386
        "orphan asylum":{'rawWmCategory':'orphan_asylum'}, //  Count: 2385
        "orphanage":{'rawWmCategory':'orphanage'}, //  Count: 2385
        "geology":{'rawWmCategory':'geology'}, //  Count: 2370
        "dry cleaner":{'rawWmCategory':'dry_cleaner'}, //  Count: 2367
        "dry cleaning":{'rawWmCategory':'dry_cleaning'}, //  Count: 2367
        "municipal":{'rawWmCategory':'municipal'}, //  Count: 2367
        "language school":{'rawWmCategory':'language_school'}, //  Count: 2363
        "solar farm":{'rawWmCategory':'solar_farm'}, //  Count: 2360
        "photovoltaic power station":{'rawWmCategory':'photovoltaic_power_station'}, //  Count: 2360
        "solar park":{'rawWmCategory':'solar_park'}, //  Count: 2360
        "river rapid / white water":{'rawWmCategory':'river_rapid_/_white_water'}, //  Count: 2352
        "secondhand shop":{'rawWmCategory':'secondhand_shop'}, //  Count: 2344
        "advertising agency":{'rawWmCategory':'advertising_agency'}, //  Count: 2343
        "solar energy":{'rawWmCategory':'solar_energy'}, //  Count: 2342
        "agricultural equipment":{'rawWmCategory':'agricultural_equipment'}, //  Count: 2333
        "pipeline":{'rawWmCategory':'pipeline'}, //  Count: 2327
        "metallurgy":{'rawWmCategory':'metallurgy'}, //  Count: 2326
        "dangerous place / area":{'rawWmCategory':'dangerous_place_/_area'}, //  Count: 2325
        "air force base":{'rawWmCategory':'air_force_base'}, //  Count: 2323
        "military airbase":{'rawWmCategory':'military_airbase'}, //  Count: 2323
        "weather / meteorological station":{'rawWmCategory':'weather_/_meteorological_station'}, //  Count: 2316
        "doctors clinic":{'rawWmCategory':'doctors_clinic'}, //  Count: 2314
        "doctors office":{'rawWmCategory':'doctors_office'}, //  Count: 2314
        "doctors surgery":{'rawWmCategory':'doctors_surgery'}, //  Count: 2314
        "general practice / surgery":{'rawWmCategory':'general_practice_/_surgery'}, //  Count: 2314
        "levee":{'rawWmCategory':'levee'}, //  Count: 2309
        "dike":{'rawWmCategory':'dike'}, //  Count: 2309
        "information technology":{'rawWmCategory':'information_technology'}, //  Count: 2308
        "tourist attraction":{'rawWmCategory':'tourist_attraction'}, //  Count: 2307
        "sawmill":{'rawWmCategory':'sawmill'}, //  Count: 2304
        "currency exchange":{'rawWmCategory':'currency_exchange'}, //  Count: 2293
        "bureau de change":{'rawWmCategory':'bureau_de_change'}, //  Count: 2293
        "money exchange":{'rawWmCategory':'money_exchange'}, //  Count: 2293
        "trail":{'rawWmCategory':'trail'}, //  Count: 2292
        "Burger King":{'rawWmCategory':'Burger_King'}, //  Count: 2290
        "county seat":{'rawWmCategory':'county_seat'}, //  Count: 2289
        "interlocking":{'rawWmCategory':'interlocking'}, //  Count: 2288
        "cafeteria":{'rawWmCategory':'cafeteria'}, //  Count: 2286
        "Magnit (store)":{'rawWmCategory':'Magnit_(store)'}, //  Count: 2285
        "undertaker":{'rawWmCategory':'undertaker'}, //  Count: 2280
        "casino":{'rawWmCategory':'casino'}, //  Count: 2280
        "funeral parlor":{'rawWmCategory':'funeral_parlor'}, //  Count: 2280
        "funeral home":{'rawWmCategory':'funeral_home'}, //  Count: 2280
        "mortuary / funeral home":{'rawWmCategory':'mortuary_/_funeral_home'}, //  Count: 2280
        "funeral director":{'rawWmCategory':'funeral_director'}, //  Count: 2280
        "auto racing":{'rawWmCategory':'auto_racing'}, //  Count: 2276
        "car racing":{'rawWmCategory':'car_racing'}, //  Count: 2276
        "newsagent&#039;s shop / newsagency / newsstand":{'rawWmCategory':'newsagent&#039;s_shop_/_newsagency_/_newsstand'}, //  Count: 2268
        "ranger station":{'rawWmCategory':'ranger_station'}, //  Count: 2253
        "autogas (LPG, propane) refuelling station":{'rawWmCategory':'autogas_(LPG,_propane)_refuelling_station'}, //  Count: 2243
        "notary":{'rawWmCategory':'notary'}, //  Count: 2242
        "horticulture":{'rawWmCategory':'horticulture'}, //  Count: 2238
        "railway stop":{'rawWmCategory':'railway_stop'}, //  Count: 2237
        "woodworking":{'rawWmCategory':'woodworking'}, //  Count: 2236
        "glade":{'rawWmCategory':'glade'}, //  Count: 2235
        "1930s construction":{'rawWmCategory':'1930s_construction'}, //  Count: 2227
        "gated community":{'rawWmCategory':'gated_community'}, //  Count: 2212
        "KFC":{'rawWmCategory':'KFC'}, //  Count: 2209
        "Kentucky Fried Chicken":{'rawWmCategory':'Kentucky_Fried_Chicken'}, //  Count: 2209
        "special needs school":{'rawWmCategory':'special_needs_school'}, //  Count: 2201
        "school for the disabled":{'rawWmCategory':'school_for_the_disabled'}, //  Count: 2201
        "special needs education":{'rawWmCategory':'special_needs_education'}, //  Count: 2201
        "milk processing factory":{'rawWmCategory':'milk_processing_factory'}, //  Count: 2198
        "dairy processing plant":{'rawWmCategory':'dairy_processing_plant'}, //  Count: 2198
        "historic district":{'rawWmCategory':'historic_district'}, //  Count: 2188
        "television broadcast station":{'rawWmCategory':'television_broadcast_station'}, //  Count: 2183
        "parade ground":{'rawWmCategory':'parade_ground'}, //  Count: 2175
        "cooperative":{'rawWmCategory':'cooperative'}, //  Count: 2174
        "county":{'rawWmCategory':'county'}, //  Count: 2173
        "skyscraper":{'rawWmCategory':'skyscraper'}, //  Count: 2170
        "watch-tower":{'rawWmCategory':'watch-tower'}, //  Count: 2150
        "plantation":{'rawWmCategory':'plantation'}, //  Count: 2146
        "ancient":{'rawWmCategory':'ancient'}, //  Count: 2144
        "glacier":{'rawWmCategory':'glacier'}, //  Count: 2141
        "quarters - to be replaced":{'rawWmCategory':'quarters_-_to_be_replaced'}, //  Count: 2139
        "exit - to be deleted":{'rawWmCategory':'exit_-_to_be_deleted'}, //  Count: 2138
        "geoglyph":{'rawWmCategory':'geoglyph'}, //  Count: 2136
        "gold mine":{'rawWmCategory':'gold_mine'}, //  Count: 2134
        "civil registry office":{'rawWmCategory':'civil_registry_office'}, //  Count: 2124
        "railway terminus":{'rawWmCategory':'railway_terminus'}, //  Count: 2114
        "railway terminal":{'rawWmCategory':'railway_terminal'}, //  Count: 2114
        "psychiatry":{'rawWmCategory':'psychiatry'}, //  Count: 2110
        "aquapark":{'rawWmCategory':'aquapark'}, //  Count: 2101
        "sikhism":{'rawWmCategory':'sikhism'}, //  Count: 2101
        "water park":{'rawWmCategory':'water_park'}, //  Count: 2101
        "fire pond":{'rawWmCategory':'fire_pond'}, //  Count: 2097
        "loteamento":{'rawWmCategory':'loteamento'}, //  Count: 2095
        "Pentecostal Church":{'rawWmCategory':'Pentecostal_Church'}, //  Count: 2085
        "pentecostalism":{'rawWmCategory':'pentecostalism'}, //  Count: 2085
        "warship":{'rawWmCategory':'warship'}, //  Count: 2084
        "video rental shop":{'rawWmCategory':'video_rental_shop'}, //  Count: 2084
        "martial arts":{'rawWmCategory':'martial_arts'}, //  Count: 2074
        "airport apron":{'rawWmCategory':'airport_apron'}, //  Count: 2074
        "exposition":{'rawWmCategory':'exposition'}, //  Count: 2067
        "aquaculture":{'rawWmCategory':'aquaculture'}, //  Count: 2053
        "downtown / central business district":{'rawWmCategory':'downtown_/_central_business_district'}, //  Count: 2052
        "coal mine":{'rawWmCategory':'coal_mine'}, //  Count: 2052
        "colliery":{'rawWmCategory':'colliery'}, //  Count: 2052
        "steakhouse":{'rawWmCategory':'steakhouse'}, //  Count: 2049
        "tree farm":{'rawWmCategory':'tree_farm'}, //  Count: 2045
        "plant nursery":{'rawWmCategory':'plant_nursery'}, //  Count: 2045
        "crane":{'rawWmCategory':'crane'}, //  Count: 2033
        "madrasah":{'rawWmCategory':'madrasah'}, //  Count: 2029
        "madarsa":{'rawWmCategory':'madarsa'}, //  Count: 2029
        "storage tank":{'rawWmCategory':'storage_tank'}, //  Count: 2024
        "delicatessen":{'rawWmCategory':'delicatessen'}, //  Count: 2024
        "state government":{'rawWmCategory':'state_government'}, //  Count: 2021
        "degree confluence project - DCP":{'rawWmCategory':'degree_confluence_project_-_DCP'}, //  Count: 2020
        "foodservice":{'rawWmCategory':'foodservice'}, //  Count: 2019
        "catering":{'rawWmCategory':'catering'}, //  Count: 2019
        "caterer":{'rawWmCategory':'caterer'}, //  Count: 2019
        "caterers":{'rawWmCategory':'caterers'}, //  Count: 2019
        "Grade I Listed (UK)":{'rawWmCategory':'Grade_I_Listed_(UK)'}, //  Count: 2017
        "mining":{'rawWmCategory':'mining'}, //  Count: 2016
        "non-governmental organization (NGO)":{'rawWmCategory':'non-governmental_organization_(NGO)'}, //  Count: 2015
        "jetty":{'rawWmCategory':'jetty'}, //  Count: 2007
        "terrace":{'rawWmCategory':'terrace'}, //  Count: 2003
        "First World War 1914-1918":{'rawWmCategory':'First_World_War_1914-1918'}, //  Count: 2002
        "railroad yard":{'rawWmCategory':'railroad_yard'}, //  Count: 1998
        "hump retarder yard":{'rawWmCategory':'hump_retarder_yard'}, //  Count: 1998
        "railyard":{'rawWmCategory':'railyard'}, //  Count: 1998
        "hot spring":{'rawWmCategory':'hot_spring'}, //  Count: 1996
        "salon":{'rawWmCategory':'salon'}, //  Count: 1992
        "fish pond":{'rawWmCategory':'fish_pond'}, //  Count: 1984
        "NDB - non-directional beacon":{'rawWmCategory':'NDB_-_non-directional_beacon'}, //  Count: 1984
        "gurdwara":{'rawWmCategory':'gurdwara'}, //  Count: 1983
        "gurudwara":{'rawWmCategory':'gurudwara'}, //  Count: 1983
        "sikh temple":{'rawWmCategory':'sikh_temple'}, //  Count: 1983
        "high mast lighting":{'rawWmCategory':'high_mast_lighting'}, //  Count: 1978
        "home furnishing / interior decoration":{'rawWmCategory':'home_furnishing_/_interior_decoration'}, //  Count: 1973
        "homeware retailer":{'rawWmCategory':'homeware_retailer'}, //  Count: 1973
        "agritourism":{'rawWmCategory':'agritourism'}, //  Count: 1973
        "procuratorate":{'rawWmCategory':'procuratorate'}, //  Count: 1972
        "control tower":{'rawWmCategory':'control_tower'}, //  Count: 1964
        "atc / air traffic control tower":{'rawWmCategory':'atc_/_air_traffic_control_tower'}, //  Count: 1964
        "megalith":{'rawWmCategory':'megalith'}, //  Count: 1963
        "funeral services":{'rawWmCategory':'funeral_services'}, //  Count: 1956
        "saline lake":{'rawWmCategory':'saline_lake'}, //  Count: 1955
        "salt lake":{'rawWmCategory':'salt_lake'}, //  Count: 1955
        "showroom":{'rawWmCategory':'showroom'}, //  Count: 1954
        "department of engineering education":{'rawWmCategory':'department_of_engineering_education'}, //  Count: 1947
        "college of engineering":{'rawWmCategory':'college_of_engineering'}, //  Count: 1947
        "To be deleted":{'rawWmCategory':'To_be_deleted'}, //  Count: 1940
        "women's clothing store":{'rawWmCategory':'womens_clothing_store'}, //  Count: 1915
        "medical laboratory":{'rawWmCategory':'medical_laboratory'}, //  Count: 1915
        "clinical laboratory":{'rawWmCategory':'clinical_laboratory'}, //  Count: 1915
        "dockyard":{'rawWmCategory':'dockyard'}, //  Count: 1914
        "shipbuilding hall":{'rawWmCategory':'shipbuilding_hall'}, //  Count: 1914
        "shipyard":{'rawWmCategory':'shipyard'}, //  Count: 1914
        "extreme skating ramp":{'rawWmCategory':'extreme_skating_ramp'}, //  Count: 1909
        "skateboard park":{'rawWmCategory':'skateboard_park'}, //  Count: 1909
        "half-pipe":{'rawWmCategory':'half-pipe'}, //  Count: 1909
        "skatepark":{'rawWmCategory':'skatepark'}, //  Count: 1909
        "forest break":{'rawWmCategory':'forest_break'}, //  Count: 1901
        "sewage lift station":{'rawWmCategory':'sewage_lift_station'}, //  Count: 1900
        "sewage pumping station":{'rawWmCategory':'sewage_pumping_station'}, //  Count: 1900
        "wastewater pumping station":{'rawWmCategory':'wastewater_pumping_station'}, //  Count: 1900
        "shooting":{'rawWmCategory':'shooting'}, //  Count: 1896
        "musholla":{'rawWmCategory':'musholla'}, //  Count: 1893
        "border crossing":{'rawWmCategory':'border_crossing'}, //  Count: 1892
        "Starbucks":{'rawWmCategory':'Starbucks'}, //  Count: 1892
        "border checkpoint":{'rawWmCategory':'border_checkpoint'}, //  Count: 1892
        "Indian cuisine":{'rawWmCategory':'Indian_cuisine'}, //  Count: 1889
        "clothing repair and alteration":{'rawWmCategory':'clothing_repair_and_alteration'}, //  Count: 1883
        "creek":{'rawWmCategory':'creek'}, //  Count: 1882
        "rehabilitation centre":{'rawWmCategory':'rehabilitation_centre'}, //  Count: 1881
        "rehabilitation":{'rawWmCategory':'rehabilitation'}, //  Count: 1881
        "massage therapy":{'rawWmCategory':'massage_therapy'}, //  Count: 1880
        "consulate / consular section / consul residence":{'rawWmCategory':'consulate_/_consular_section_/_consul_residence'}, //  Count: 1866
        "belvedere":{'rawWmCategory':'belvedere'}, //  Count: 1861
        "observatory":{'rawWmCategory':'observatory'}, //  Count: 1861
        "stage":{'rawWmCategory':'stage'}, //  Count: 1861
        "miradouro":{'rawWmCategory':'miradouro'}, //  Count: 1861
        "pen (enclosure)":{'rawWmCategory':'pen_(enclosure)'}, //  Count: 1856
        "locomotive":{'rawWmCategory':'locomotive'}, //  Count: 1852
        "stable":{'rawWmCategory':'stable'}, //  Count: 1850
        "grove":{'rawWmCategory':'grove'}, //  Count: 1846
        "business park":{'rawWmCategory':'business_park'}, //  Count: 1845
        "office park":{'rawWmCategory':'office_park'}, //  Count: 1845
        "business estate":{'rawWmCategory':'business_estate'}, //  Count: 1845
        "concrete-mixing plant":{'rawWmCategory':'concrete-mixing_plant'}, //  Count: 1838
        "sushi":{'rawWmCategory':'sushi'}, //  Count: 1835
        "manicure / pedicure salon":{'rawWmCategory':'manicure_/_pedicure_salon'}, //  Count: 1832
        "nail salon":{'rawWmCategory':'nail_salon'}, //  Count: 1832
        "ball park":{'rawWmCategory':'ball_park'}, //  Count: 1831
        "baseball stadium":{'rawWmCategory':'baseball_stadium'}, //  Count: 1831
        "baseball park / stadium":{'rawWmCategory':'baseball_park_/_stadium'}, //  Count: 1831
        "ballpark":{'rawWmCategory':'ballpark'}, //  Count: 1831
        "first aid station / first aid room":{'rawWmCategory':'first_aid_station_/_first_aid_room'}, //  Count: 1830
        "docks":{'rawWmCategory':'docks'}, //  Count: 1814
        "dock":{'rawWmCategory':'dock'}, //  Count: 1814
        "arch":{'rawWmCategory':'arch'}, //  Count: 1809
        "babycare/ babywear shop":{'rawWmCategory':'babycare/_babywear_shop'}, //  Count: 1807
        "United States Navy":{'rawWmCategory':'United_States_Navy'}, //  Count: 1803
        "Target (store)":{'rawWmCategory':'Target_(store)'}, //  Count: 1803
        "textile industry":{'rawWmCategory':'textile_industry'}, //  Count: 1801
        "software developer":{'rawWmCategory':'software_developer'}, //  Count: 1800
        "motorcycle racing":{'rawWmCategory':'motorcycle_racing'}, //  Count: 1795
        "clubhouse":{'rawWmCategory':'clubhouse'}, //  Count: 1788
        "mixed-used building":{'rawWmCategory':'mixed-used_building'}, //  Count: 1788
        "indoor swimming pool / natatorium":{'rawWmCategory':'indoor_swimming_pool_/_natatorium'}, //  Count: 1786
        "college of technology / polytechnics":{'rawWmCategory':'college_of_technology_/_polytechnics'}, //  Count: 1782
        "supplies":{'rawWmCategory':'supplies'}, //  Count: 1776
        "supplier":{'rawWmCategory':'supplier'}, //  Count: 1776
        "supply":{'rawWmCategory':'supply'}, //  Count: 1776
        "1910s construction":{'rawWmCategory':'1910s_construction'}, //  Count: 1770
        "district court (judiciary)":{'rawWmCategory':'district_court_(judiciary)'}, //  Count: 1770
        "botanical garden":{'rawWmCategory':'botanical_garden'}, //  Count: 1767
        "1900s construction":{'rawWmCategory':'1900s_construction'}, //  Count: 1764
        "golf club":{'rawWmCategory':'golf_club'}, //  Count: 1764
        "tailings":{'rawWmCategory':'tailings'}, //  Count: 1757
        "military enlistment office":{'rawWmCategory':'military_enlistment_office'}, //  Count: 1756
        "television transmission tower":{'rawWmCategory':'television_transmission_tower'}, //  Count: 1755
        "internet service provider (ISP)":{'rawWmCategory':'internet_service_provider_(ISP)'}, //  Count: 1753
        "tea stall":{'rawWmCategory':'tea_stall'}, //  Count: 1752
        "tea house":{'rawWmCategory':'tea_house'}, //  Count: 1752
        "tearoom":{'rawWmCategory':'tearoom'}, //  Count: 1752
        "guest house":{'rawWmCategory':'guest_house'}, //  Count: 1740
        "drainage":{'rawWmCategory':'drainage'}, //  Count: 1739
        "peninsula":{'rawWmCategory':'peninsula'}, //  Count: 1734
        "maternity hospital":{'rawWmCategory':'maternity_hospital'}, //  Count: 1732
        "skiing":{'rawWmCategory':'skiing'}, //  Count: 1731
        "former train/railway station":{'rawWmCategory':'former_train/railway_station'}, //  Count: 1729
        "shoe repair shop":{'rawWmCategory':'shoe_repair_shop'}, //  Count: 1726
        "boom barrier":{'rawWmCategory':'boom_barrier'}, //  Count: 1725
        "traffic control barrier":{'rawWmCategory':'traffic_control_barrier'}, //  Count: 1725
        "boom gate":{'rawWmCategory':'boom_gate'}, //  Count: 1725
        "chartered accountant":{'rawWmCategory':'chartered_accountant'}, //  Count: 1724
        "chartered accountancy":{'rawWmCategory':'chartered_accountancy'}, //  Count: 1724
        "outdoor switchgear":{'rawWmCategory':'outdoor_switchgear'}, //  Count: 1717
        "mountain resort":{'rawWmCategory':'mountain_resort'}, //  Count: 1707
        "ski resort / area":{'rawWmCategory':'ski_resort_/_area'}, //  Count: 1707
        "dispatcher office":{'rawWmCategory':'dispatcher_office'}, //  Count: 1702
        "lumber":{'rawWmCategory':'lumber'}, //  Count: 1686
        "auto wrecking yard":{'rawWmCategory':'auto_wrecking_yard'}, //  Count: 1685
        "junkyard / scrapyard":{'rawWmCategory':'junkyard_/_scrapyard'}, //  Count: 1685
        "Methodist Church":{'rawWmCategory':'Methodist_Church'}, //  Count: 1684
        "driving training area":{'rawWmCategory':'driving_training_area'}, //  Count: 1680
        "dead-end railway track":{'rawWmCategory':'dead-end_railway_track'}, //  Count: 1666
        "Tex-Mex cuisine":{'rawWmCategory':'Tex-Mex_cuisine'}, //  Count: 1658
        "slipway":{'rawWmCategory':'slipway'}, //  Count: 1654
        "boat ramp / launch":{'rawWmCategory':'boat_ramp_/_launch'}, //  Count: 1654
        "barrio hall":{'rawWmCategory':'barrio_hall'}, //  Count: 1653
        "barangay hall":{'rawWmCategory':'barangay_hall'}, //  Count: 1653
        "technical service":{'rawWmCategory':'technical_service'}, //  Count: 1641
        "historic town square":{'rawWmCategory':'historic_town_square'}, //  Count: 1640
        "mormons":{'rawWmCategory':'mormons'}, //  Count: 1638
        "mormon":{'rawWmCategory':'mormon'}, //  Count: 1638
        "courtyard":{'rawWmCategory':'courtyard'}, //  Count: 1637
        "photocopy shop":{'rawWmCategory':'photocopy_shop'}, //  Count: 1628
        "Xerox centre / shop":{'rawWmCategory':'Xerox_centre_/_shop'}, //  Count: 1628
        "copy / printing shop":{'rawWmCategory':'copy_/_printing_shop'}, //  Count: 1628
        "Pizza Hut":{'rawWmCategory':'Pizza_Hut'}, //  Count: 1623
        "bathroom accessories":{'rawWmCategory':'bathroom_accessories'}, //  Count: 1621
        "strait / channel / passage / narrows":{'rawWmCategory':'strait_/_channel_/_passage_/_narrows'}, //  Count: 1621
        "dance school / dance academy":{'rawWmCategory':'dance_school_/_dance_academy'}, //  Count: 1616
        "dance studio":{'rawWmCategory':'dance_studio'}, //  Count: 1616
        "historic city center":{'rawWmCategory':'historic_city_center'}, //  Count: 1615
        "medina":{'rawWmCategory':'medina'}, //  Count: 1615
        "Jugendstil / Art Nouveau architecture":{'rawWmCategory':'Jugendstil_/_Art_Nouveau_architecture'}, //  Count: 1614
        "archives institution":{'rawWmCategory':'archives_institution'}, //  Count: 1613
        "variety store":{'rawWmCategory':'variety_store'}, //  Count: 1612
        "street / public clock":{'rawWmCategory':'street_/_public_clock'}, //  Count: 1606
        "metalwork":{'rawWmCategory':'metalwork'}, //  Count: 1604
        "consumer services":{'rawWmCategory':'consumer_services'}, //  Count: 1602
        "railway traction substation":{'rawWmCategory':'railway_traction_substation'}, //  Count: 1601
        "wifi":{'rawWmCategory':'wifi'}, //  Count: 1601
        "wi-fi":{'rawWmCategory':'wi-fi'}, //  Count: 1601
        "plastics industry":{'rawWmCategory':'plastics_industry'}, //  Count: 1600
        "breakwater":{'rawWmCategory':'breakwater'}, //  Count: 1595
        "mole":{'rawWmCategory':'mole'}, //  Count: 1595
        "hillfort":{'rawWmCategory':'hillfort'}, //  Count: 1592
        "open air theatre":{'rawWmCategory':'open_air_theatre'}, //  Count: 1583
        "summerhouse":{'rawWmCategory':'summerhouse'}, //  Count: 1583
        "amphitheatre":{'rawWmCategory':'amphitheatre'}, //  Count: 1583
        "amphitheater":{'rawWmCategory':'amphitheater'}, //  Count: 1583
        "fish farm":{'rawWmCategory':'fish_farm'}, //  Count: 1581
        "janas":{'rawWmCategory':'janas'}, //  Count: 1581
        "sidewalk / pavement / outdoor cafe":{'rawWmCategory':'sidewalk_/_pavement_/_outdoor_cafe'}, //  Count: 1581
        "mass grave":{'rawWmCategory':'mass_grave'}, //  Count: 1577
        "edifice":{'rawWmCategory':'edifice'}, //  Count: 1566
        "blockhouse (fort)":{'rawWmCategory':'blockhouse_(fort)'}, //  Count: 1565
        "standpipe":{'rawWmCategory':'standpipe'}, //  Count: 1564
        "vegetable shop / stall":{'rawWmCategory':'vegetable_shop_/_stall'}, //  Count: 1563
        "greengrocer /  fruit stall":{'rawWmCategory':'greengrocer_/__fruit_stall'}, //  Count: 1563
        "greengrocery":{'rawWmCategory':'greengrocery'}, //  Count: 1563
        "minimarket":{'rawWmCategory':'minimarket'}, //  Count: 1562
        "quay":{'rawWmCategory':'quay'}, //  Count: 1560
        "wharf":{'rawWmCategory':'wharf'}, //  Count: 1560
        "children's wear":{'rawWmCategory':'childrens_wear'}, //  Count: 1559
        "hippodrome":{'rawWmCategory':'hippodrome'}, //  Count: 1554
        "horse racing track":{'rawWmCategory':'horse_racing_track'}, //  Count: 1554
        "subdistrict":{'rawWmCategory':'subdistrict'}, //  Count: 1553
        "polling place (election voting)":{'rawWmCategory':'polling_place_(election_voting)'}, //  Count: 1549
        "taluka headquarter":{'rawWmCategory':'taluka_headquarter'}, //  Count: 1547
        "mandal headquarter":{'rawWmCategory':'mandal_headquarter'}, //  Count: 1547
        "state school":{'rawWmCategory':'state_school'}, //  Count: 1542
        "dispensary (specialized medical facility - USSR)":{'rawWmCategory':'dispensary_(specialized_medical_facility_-_USSR)'}, //  Count: 1542
        "natural monument":{'rawWmCategory':'natural_monument'}, //  Count: 1542
        "backpacking":{'rawWmCategory':'backpacking'}, //  Count: 1542
        "hiking":{'rawWmCategory':'hiking'}, //  Count: 1542
        "household chemistry":{'rawWmCategory':'household_chemistry'}, //  Count: 1540
        "athletics (sport)":{'rawWmCategory':'athletics_(sport)'}, //  Count: 1537
        "basin (geographical)":{'rawWmCategory':'basin_(geographical)'}, //  Count: 1536
        "VOR - VHF omni-directional radio range":{'rawWmCategory':'VOR_-_VHF_omni-directional_radio_range'}, //  Count: 1534
        "assembly of god":{'rawWmCategory':'assembly_of_god'}, //  Count: 1532
        "road maintenance department / station":{'rawWmCategory':'road_maintenance_department_/_station'}, //  Count: 1523
        "Wendy&#039;s":{'rawWmCategory':'Wendy&#039;s'}, //  Count: 1520
        "switch control house":{'rawWmCategory':'switch_control_house'}, //  Count: 1515
        "eclectic architecture":{'rawWmCategory':'eclectic_architecture'}, //  Count: 1514
        "oilfield":{'rawWmCategory':'oilfield'}, //  Count: 1513
        "oil field":{'rawWmCategory':'oil_field'}, //  Count: 1513
        "synagogue":{'rawWmCategory':'synagogue'}, //  Count: 1513
        "rice mill":{'rawWmCategory':'rice_mill'}, //  Count: 1509
        "Grade II* Listed (UK)":{'rawWmCategory':'Grade_II*_Listed_(UK)'}, //  Count: 1507
        "bicycle shop":{'rawWmCategory':'bicycle_shop'}, //  Count: 1496
        "accounting":{'rawWmCategory':'accounting'}, //  Count: 1496
        "accountancy":{'rawWmCategory':'accountancy'}, //  Count: 1496
        "bike shop":{'rawWmCategory':'bike_shop'}, //  Count: 1496
        "accountant":{'rawWmCategory':'accountant'}, //  Count: 1496
        "accountants":{'rawWmCategory':'accountants'}, //  Count: 1496
        "urban area":{'rawWmCategory':'urban_area'}, //  Count: 1495
        "transmitter":{'rawWmCategory':'transmitter'}, //  Count: 1495
        "cremation":{'rawWmCategory':'cremation'}, //  Count: 1494
        "crematory":{'rawWmCategory':'crematory'}, //  Count: 1494
        "crematorium":{'rawWmCategory':'crematorium'}, //  Count: 1494
        "barber shop":{'rawWmCategory':'barber_shop'}, //  Count: 1492
        "barber":{'rawWmCategory':'barber'}, //  Count: 1492
        "saloon (barber shop)":{'rawWmCategory':'saloon_(barber_shop)'}, //  Count: 1492
        "pigeon loft":{'rawWmCategory':'pigeon_loft'}, //  Count: 1490
        "dovecote":{'rawWmCategory':'dovecote'}, //  Count: 1490
        "transport museum":{'rawWmCategory':'transport_museum'}, //  Count: 1486
        "dog park":{'rawWmCategory':'dog_park'}, //  Count: 1486
        "fifth-level administrative division":{'rawWmCategory':'fifth-level_administrative_division'}, //  Count: 1484
        "dairy farm":{'rawWmCategory':'dairy_farm'}, //  Count: 1472
        "sugar mill":{'rawWmCategory':'sugar_mill'}, //  Count: 1472
        "sugar refinery":{'rawWmCategory':'sugar_refinery'}, //  Count: 1472
        "freight / transportation / shipping company":{'rawWmCategory':'freight_/_transportation_/_shipping_company'}, //  Count: 1462
        "concert hall":{'rawWmCategory':'concert_hall'}, //  Count: 1461
        "movie / film / TV location":{'rawWmCategory':'movie_/_film_/_TV_location'}, //  Count: 1456
        "window fitters":{'rawWmCategory':'window_fitters'}, //  Count: 1452
        "scuba diving facility / area":{'rawWmCategory':'scuba_diving_facility_/_area'}, //  Count: 1452
        "window and door replacement":{'rawWmCategory':'window_and_door_replacement'}, //  Count: 1452
        "electoral ward":{'rawWmCategory':'electoral_ward'}, //  Count: 1449
        "ward":{'rawWmCategory':'ward'}, //  Count: 1449
        "doughnuts":{'rawWmCategory':'doughnuts'}, //  Count: 1448
        "donuts / doughnuts":{'rawWmCategory':'donuts_/_doughnuts'}, //  Count: 1448
        "wine merchant":{'rawWmCategory':'wine_merchant'}, //  Count: 1445
        "wine shop":{'rawWmCategory':'wine_shop'}, //  Count: 1445
        "Sitio":{'rawWmCategory':'Sitio'}, //  Count: 1443
        "Medieval / Middle Ages":{'rawWmCategory':'Medieval_/_Middle_Ages'}, //  Count: 1442
        "rescue":{'rawWmCategory':'rescue'}, //  Count: 1439
        "suspension bridge":{'rawWmCategory':'suspension_bridge'}, //  Count: 1438
        "driving range":{'rawWmCategory':'driving_range'}, //  Count: 1436
        "morgue / mortuary":{'rawWmCategory':'morgue_/_mortuary'}, //  Count: 1434
        "publishing house":{'rawWmCategory':'publishing_house'}, //  Count: 1431
        "discothèque":{'rawWmCategory':'discothèque'}, //  Count: 1431
        "hobby store/shop":{'rawWmCategory':'hobby_store/shop'}, //  Count: 1430
        "living farm museum":{'rawWmCategory':'living_farm_museum'}, //  Count: 1422
        "savings and credit cooperative":{'rawWmCategory':'savings_and_credit_cooperative'}, //  Count: 1422
        "ecomuseum":{'rawWmCategory':'ecomuseum'}, //  Count: 1422
        "living history museum":{'rawWmCategory':'living_history_museum'}, //  Count: 1422
        "living museum":{'rawWmCategory':'living_museum'}, //  Count: 1422
        "open air museum":{'rawWmCategory':'open_air_museum'}, //  Count: 1422
        "credit union":{'rawWmCategory':'credit_union'}, //  Count: 1422
        "skansen":{'rawWmCategory':'skansen'}, //  Count: 1422
        "museum of buildings":{'rawWmCategory':'museum_of_buildings'}, //  Count: 1422
        "folk museum":{'rawWmCategory':'folk_museum'}, //  Count: 1422
        "Rococo architecture (Late Baroque)":{'rawWmCategory':'Rococo_architecture_(Late_Baroque)'}, //  Count: 1421
        "Baroque architecture":{'rawWmCategory':'Baroque_architecture'}, //  Count: 1421
        "therapy":{'rawWmCategory':'therapy'}, //  Count: 1418
        "Home Depot":{'rawWmCategory':'Home_Depot'}, //  Count: 1416
        "gatehouse":{'rawWmCategory':'gatehouse'}, //  Count: 1415
        "mobile home community":{'rawWmCategory':'mobile_home_community'}, //  Count: 1414
        "arena":{'rawWmCategory':'arena'}, //  Count: 1410
        "hunting":{'rawWmCategory':'hunting'}, //  Count: 1407
        "grandstand":{'rawWmCategory':'grandstand'}, //  Count: 1405
        "stadium stand":{'rawWmCategory':'stadium_stand'}, //  Count: 1405
        "tube station entrance / exit":{'rawWmCategory':'tube_station_entrance_/_exit'}, //  Count: 1401
        "metro entrance / exit":{'rawWmCategory':'metro_entrance_/_exit'}, //  Count: 1401
        "underground station entrance / exit":{'rawWmCategory':'underground_station_entrance_/_exit'}, //  Count: 1401
        "weir":{'rawWmCategory':'weir'}, //  Count: 1401
        "city gate":{'rawWmCategory':'city_gate'}, //  Count: 1397
        "gram panchayat":{'rawWmCategory':'gram_panchayat'}, //  Count: 1393
        "1940s construction":{'rawWmCategory':'1940s_construction'}, //  Count: 1391
        "fishing tackle shop":{'rawWmCategory':'fishing_tackle_shop'}, //  Count: 1391
        "triangulation station":{'rawWmCategory':'triangulation_station'}, //  Count: 1385
        "trigonometrical station":{'rawWmCategory':'trigonometrical_station'}, //  Count: 1385
        "triangulation pillar":{'rawWmCategory':'triangulation_pillar'}, //  Count: 1385
        "pig farm":{'rawWmCategory':'pig_farm'}, //  Count: 1385
        "trig point":{'rawWmCategory':'trig_point'}, //  Count: 1385
        "water sports":{'rawWmCategory':'water_sports'}, //  Count: 1383
        "Hanuman temple":{'rawWmCategory':'Hanuman_temple'}, //  Count: 1382
        "Hanuman mandir":{'rawWmCategory':'Hanuman_mandir'}, //  Count: 1382
        "cowshed":{'rawWmCategory':'cowshed'}, //  Count: 1381
        "massage parlour":{'rawWmCategory':'massage_parlour'}, //  Count: 1378
        "sea":{'rawWmCategory':'sea'}, //  Count: 1373
        "canal bridge":{'rawWmCategory':'canal_bridge'}, //  Count: 1368
        "football - soccer club":{'rawWmCategory':'football_-_soccer_club'}, //  Count: 1367
        "fashion":{'rawWmCategory':'fashion'}, //  Count: 1363
        "Lidl":{'rawWmCategory':'Lidl'}, //  Count: 1360
        "bus stop shelter":{'rawWmCategory':'bus_stop_shelter'}, //  Count: 1358
        "Walgreens":{'rawWmCategory':'Walgreens'}, //  Count: 1357
        "barbecue":{'rawWmCategory':'barbecue'}, //  Count: 1355
        "bbq":{'rawWmCategory':'bbq'}, //  Count: 1355
        "train":{'rawWmCategory':'train'}, //  Count: 1353
        "bird hide":{'rawWmCategory':'bird_hide'}, //  Count: 1352
        "birdwatching":{'rawWmCategory':'birdwatching'}, //  Count: 1352
        "electrical engineering":{'rawWmCategory':'electrical_engineering'}, //  Count: 1348
        "autodromo":{'rawWmCategory':'autodromo'}, //  Count: 1345
        "مفيض":{'rawWmCategory':'مفيض'}, //  Count: 1338
        "spillway":{'rawWmCategory':'spillway'}, //  Count: 1338
        "beer store":{'rawWmCategory':'beer_store'}, //  Count: 1337
        "canyon / gorge":{'rawWmCategory':'canyon_/_gorge'}, //  Count: 1336
        "online shopping":{'rawWmCategory':'online_shopping'}, //  Count: 1329
        "rubbish":{'rawWmCategory':'rubbish'}, //  Count: 1320
        "waste":{'rawWmCategory':'waste'}, //  Count: 1320
        "garbage":{'rawWmCategory':'garbage'}, //  Count: 1320
        "cleaning service / company":{'rawWmCategory':'cleaning_service_/_company'}, //  Count: 1315
        "sanitation / hygiene service":{'rawWmCategory':'sanitation_/_hygiene_service'}, //  Count: 1315
        "cleaner":{'rawWmCategory':'cleaner'}, //  Count: 1315
        "kitchen":{'rawWmCategory':'kitchen'}, //  Count: 1312
        "interlocking tower":{'rawWmCategory':'interlocking_tower'}, //  Count: 1309
        "signal box":{'rawWmCategory':'signal_box'}, //  Count: 1309
        "celebrity home":{'rawWmCategory':'celebrity_home'}, //  Count: 1302
        "forest compartment":{'rawWmCategory':'forest_compartment'}, //  Count: 1300
        "coast guard":{'rawWmCategory':'coast_guard'}, //  Count: 1299
        "Subway (restaurant)":{'rawWmCategory':'Subway_(restaurant)'}, //  Count: 1299
        "antique shop":{'rawWmCategory':'antique_shop'}, //  Count: 1298
        "capital city of state/province/region":{'rawWmCategory':'capital_city_of_state/province/region'}, //  Count: 1297
        "deforested / clearcut area":{'rawWmCategory':'deforested_/_clearcut_area'}, //  Count: 1296
        "motocross / off road circuit":{'rawWmCategory':'motocross_/_off_road_circuit'}, //  Count: 1295
        "motocross track":{'rawWmCategory':'motocross_track'}, //  Count: 1295
        "dirt bike track":{'rawWmCategory':'dirt_bike_track'}, //  Count: 1295
        "electric grid company":{'rawWmCategory':'electric_grid_company'}, //  Count: 1292
        "electricity supplier":{'rawWmCategory':'electricity_supplier'}, //  Count: 1292
        "narrow gauge railway":{'rawWmCategory':'narrow_gauge_railway'}, //  Count: 1292
        "fishmonger":{'rawWmCategory':'fishmonger'}, //  Count: 1286
        "seafood shop / market":{'rawWmCategory':'seafood_shop_/_market'}, //  Count: 1286
        "cargo terminal":{'rawWmCategory':'cargo_terminal'}, //  Count: 1283
        "dry port":{'rawWmCategory':'dry_port'}, //  Count: 1283
        "container / transport / storage terminal":{'rawWmCategory':'container_/_transport_/_storage_terminal'}, //  Count: 1283
        "manufactured goods store":{'rawWmCategory':'manufactured_goods_store'}, //  Count: 1281
        "tailor/ tailoring":{'rawWmCategory':'tailor/_tailoring'}, //  Count: 1280
        "tanning":{'rawWmCategory':'tanning'}, //  Count: 1280
        "Presbyterian Church":{'rawWmCategory':'Presbyterian_Church'}, //  Count: 1277
        "presbyterianism":{'rawWmCategory':'presbyterianism'}, //  Count: 1277
        "A-league":{'rawWmCategory':'A-league'}, //  Count: 1272
        "major league soccer":{'rawWmCategory':'major_league_soccer'}, //  Count: 1272
        "football premier league":{'rawWmCategory':'football_premier_league'}, //  Count: 1272
        "truck stop":{'rawWmCategory':'truck_stop'}, //  Count: 1270
        "mailing":{'rawWmCategory':'mailing'}, //  Count: 1266
        "vicarage":{'rawWmCategory':'vicarage'}, //  Count: 1263
        "presbytery":{'rawWmCategory':'presbytery'}, //  Count: 1263
        "manse":{'rawWmCategory':'manse'}, //  Count: 1263
        "Taco Bell":{'rawWmCategory':'Taco_Bell'}, //  Count: 1263
        "parsonage":{'rawWmCategory':'parsonage'}, //  Count: 1263
        "rectory":{'rawWmCategory':'rectory'}, //  Count: 1263
        "Jainism":{'rawWmCategory':'Jainism'}, //  Count: 1257
        "new year event":{'rawWmCategory':'new_year_event'}, //  Count: 1248
        "new year celebration":{'rawWmCategory':'new_year_celebration'}, //  Count: 1248
        "payment centre / settlement centre":{'rawWmCategory':'payment_centre_/_settlement_centre'}, //  Count: 1242
        "web design":{'rawWmCategory':'web_design'}, //  Count: 1242
        "student center / student union":{'rawWmCategory':'student_center_/_student_union'}, //  Count: 1238
        "ridge":{'rawWmCategory':'ridge'}, //  Count: 1236
        "radiocommunication":{'rawWmCategory':'radiocommunication'}, //  Count: 1232
        "national park":{'rawWmCategory':'national_park'}, //  Count: 1231
        "Renfe":{'rawWmCategory':'Renfe'}, //  Count: 1225
        "sewing":{'rawWmCategory':'sewing'}, //  Count: 1224
        "tires":{'rawWmCategory':'tires'}, //  Count: 1223
        "boat rental station":{'rawWmCategory':'boat_rental_station'}, //  Count: 1220
        "boat hire":{'rawWmCategory':'boat_hire'}, //  Count: 1220
        "Devi temple":{'rawWmCategory':'Devi_temple'}, //  Count: 1219
        "city wall":{'rawWmCategory':'city_wall'}, //  Count: 1216
        "medical ward / department":{'rawWmCategory':'medical_ward_/_department'}, //  Count: 1216
        "sanitary":{'rawWmCategory':'sanitary'}, //  Count: 1209
        "crater":{'rawWmCategory':'crater'}, //  Count: 1208
        "trees":{'rawWmCategory':'trees'}, //  Count: 1206
        "Ukrainian Orthodox Church - Moscow Patriarchate":{'rawWmCategory':'Ukrainian_Orthodox_Church_-_Moscow_Patriarchate'}, //  Count: 1199
        "motorcycle repair service":{'rawWmCategory':'motorcycle_repair_service'}, //  Count: 1197
        "pedestrian area":{'rawWmCategory':'pedestrian_area'}, //  Count: 1195
        "LIGC - Long Island Gold Coast":{'rawWmCategory':'LIGC_-_Long_Island_Gold_Coast'}, //  Count: 1194
        "multipurpose hall":{'rawWmCategory':'multipurpose_hall'}, //  Count: 1191
        "assembly hall":{'rawWmCategory':'assembly_hall'}, //  Count: 1191
        "function hall":{'rawWmCategory':'function_hall'}, //  Count: 1191
        "clothing":{'rawWmCategory':'clothing'}, //  Count: 1191
        "wayside shrine / chapel-shrine / bildstock":{'rawWmCategory':'wayside_shrine_/_chapel-shrine_/_bildstock'}, //  Count: 1190
        "medical equipment":{'rawWmCategory':'medical_equipment'}, //  Count: 1189
        "fence":{'rawWmCategory':'fence'}, //  Count: 1188
        "granary":{'rawWmCategory':'granary'}, //  Count: 1187
        "political party":{'rawWmCategory':'political_party'}, //  Count: 1186
        "monastic college":{'rawWmCategory':'monastic_college'}, //  Count: 1185
        "college of theology":{'rawWmCategory':'college_of_theology'}, //  Count: 1185
        "divinity school":{'rawWmCategory':'divinity_school'}, //  Count: 1185
        "department of theology":{'rawWmCategory':'department_of_theology'}, //  Count: 1185
        "seminary":{'rawWmCategory':'seminary'}, //  Count: 1185
        "housing cooperative":{'rawWmCategory':'housing_cooperative'}, //  Count: 1184
        "nude beach":{'rawWmCategory':'nude_beach'}, //  Count: 1182
        "naturism":{'rawWmCategory':'naturism'}, //  Count: 1182
        "naturist club":{'rawWmCategory':'naturist_club'}, //  Count: 1182
        "nudism":{'rawWmCategory':'nudism'}, //  Count: 1182
        "banqueting":{'rawWmCategory':'banqueting'}, //  Count: 1179
        "security contractor":{'rawWmCategory':'security_contractor'}, //  Count: 1179
        "banquet":{'rawWmCategory':'banquet'}, //  Count: 1179
        "private security company":{'rawWmCategory':'private_security_company'}, //  Count: 1179
        "tell (mound)":{'rawWmCategory':'tell_(mound)'}, //  Count: 1176
        "CVS Pharmacy":{'rawWmCategory':'CVS_Pharmacy'}, //  Count: 1176
        "mausoleum":{'rawWmCategory':'mausoleum'}, //  Count: 1174
        "Banco do Brasil":{'rawWmCategory':'Banco_do_Brasil'}, //  Count: 1173
        "douar":{'rawWmCategory':'douar'}, //  Count: 1172
        "shoal, sandbar":{'rawWmCategory':'shoal,_sandbar'}, //  Count: 1170
        "meat processing":{'rawWmCategory':'meat_processing'}, //  Count: 1169
        "godown":{'rawWmCategory':'godown'}, //  Count: 1168
        "gendarmerie":{'rawWmCategory':'gendarmerie'}, //  Count: 1167
        "Royal Dutch Shell":{'rawWmCategory':'Royal_Dutch_Shell'}, //  Count: 1167
        "Shell":{'rawWmCategory':'Shell'}, //  Count: 1167
        "Iglesia Ni Cristo":{'rawWmCategory':'Iglesia_Ni_Cristo'}, //  Count: 1166
        "ashram":{'rawWmCategory':'ashram'}, //  Count: 1165
        "canoe club":{'rawWmCategory':'canoe_club'}, //  Count: 1162
        "yacht club / sailing club":{'rawWmCategory':'yacht_club_/_sailing_club'}, //  Count: 1162
        "kart circuit":{'rawWmCategory':'kart_circuit'}, //  Count: 1162
        "go-kart track":{'rawWmCategory':'go-kart_track'}, //  Count: 1162
        "1920s construction":{'rawWmCategory':'1920s_construction'}, //  Count: 1159
        "medical college":{'rawWmCategory':'medical_college'}, //  Count: 1156
        "medical school":{'rawWmCategory':'medical_school'}, //  Count: 1156
        "tavern":{'rawWmCategory':'tavern'}, //  Count: 1156
        "foundation":{'rawWmCategory':'foundation'}, //  Count: 1156
        "castle ruins":{'rawWmCategory':'castle_ruins'}, //  Count: 1153
        "ruined castle":{'rawWmCategory':'ruined_castle'}, //  Count: 1153
        "milk booth / shop":{'rawWmCategory':'milk_booth_/_shop'}, //  Count: 1151
        "dairy products shop":{'rawWmCategory':'dairy_products_shop'}, //  Count: 1151
        "seaside resort":{'rawWmCategory':'seaside_resort'}, //  Count: 1149
        "beach resort":{'rawWmCategory':'beach_resort'}, //  Count: 1149
        "mobile home park / trailer park":{'rawWmCategory':'mobile_home_park_/_trailer_park'}, //  Count: 1146
        "cannon":{'rawWmCategory':'cannon'}, //  Count: 1144
        "drive-in restaurant":{'rawWmCategory':'drive-in_restaurant'}, //  Count: 1142
        "buffet":{'rawWmCategory':'buffet'}, //  Count: 1141
        "meadow":{'rawWmCategory':'meadow'}, //  Count: 1141
        "scouting":{'rawWmCategory':'scouting'}, //  Count: 1141
        "river source":{'rawWmCategory':'river_source'}, //  Count: 1139
        "management company":{'rawWmCategory':'management_company'}, //  Count: 1137
        "hockey box":{'rawWmCategory':'hockey_box'}, //  Count: 1135
        "cosmetology":{'rawWmCategory':'cosmetology'}, //  Count: 1134
        "baker's shop":{'rawWmCategory':'bakers_shop'}, //  Count: 1134
        "beauty therapy":{'rawWmCategory':'beauty_therapy'}, //  Count: 1134
        "LDS Church (The Church of Jesus Christ of Latter-day Saints)":{'rawWmCategory':'LDS_Church_(The_Church_of_Jesus_Christ_of_Latter-day_Saints)'}, //  Count: 1133
        "electrical supply store":{'rawWmCategory':'electrical_supply_store'}, //  Count: 1130
        "car assembly factory":{'rawWmCategory':'car_assembly_factory'}, //  Count: 1130
        "electrical retailer":{'rawWmCategory':'electrical_retailer'}, //  Count: 1130
        "automobile factory":{'rawWmCategory':'automobile_factory'}, //  Count: 1130
        "social service department / organization":{'rawWmCategory':'social_service_department_/_organization'}, //  Count: 1130
        "DVD / CD shop":{'rawWmCategory':'DVD_/_CD_shop'}, //  Count: 1129
        "record shop":{'rawWmCategory':'record_shop'}, //  Count: 1129
        "football second league":{'rawWmCategory':'football_second_league'}, //  Count: 1129
        "music / entertainment shop":{'rawWmCategory':'music_/_entertainment_shop'}, //  Count: 1129
        "college of nursing":{'rawWmCategory':'college_of_nursing'}, //  Count: 1128
        "nursing school":{'rawWmCategory':'nursing_school'}, //  Count: 1128
        "serra":{'rawWmCategory':'serra'}, //  Count: 1123
        "international organization":{'rawWmCategory':'international_organization'}, //  Count: 1122
        "dollar store":{'rawWmCategory':'dollar_store'}, //  Count: 1121
        "price-point retailer":{'rawWmCategory':'price-point_retailer'}, //  Count: 1121
        "heating ventilation and air conditioning - HVAC":{'rawWmCategory':'heating_ventilation_and_air_conditioning_-_HVAC'}, //  Count: 1118
        "press / news agency":{'rawWmCategory':'press_/_news_agency'}, //  Count: 1117
        "tobacconist's shop":{'rawWmCategory':'tobacconists_shop'}, //  Count: 1115
        "smoke shop":{'rawWmCategory':'smoke_shop'}, //  Count: 1115
        "cigar shop":{'rawWmCategory':'cigar_shop'}, //  Count: 1115
        "tobacco shop":{'rawWmCategory':'tobacco_shop'}, //  Count: 1115
        "trestle (bridge)":{'rawWmCategory':'trestle_(bridge)'}, //  Count: 1110
        "obelisk":{'rawWmCategory':'obelisk'}, //  Count: 1106
        "private housing estate":{'rawWmCategory':'private_housing_estate'}, //  Count: 1104
        "marsh":{'rawWmCategory':'marsh'}, //  Count: 1103
        "softball field":{'rawWmCategory':'softball_field'}, //  Count: 1103
        "crazy golf":{'rawWmCategory':'crazy_golf'}, //  Count: 1102
        "miniature golf":{'rawWmCategory':'miniature_golf'}, //  Count: 1102
        "mini golf":{'rawWmCategory':'mini_golf'}, //  Count: 1102
        "interior":{'rawWmCategory':'interior'}, //  Count: 1101
        "proving ground":{'rawWmCategory':'proving_ground'}, //  Count: 1100
        "aqueduct":{'rawWmCategory':'aqueduct'}, //  Count: 1097
        "Minuteman":{'rawWmCategory':'Minuteman'}, //  Count: 1096
        "small architectural form":{'rawWmCategory':'small_architectural_form'}, //  Count: 1089
        "red light camera":{'rawWmCategory':'red_light_camera'}, //  Count: 1089
        "photo radar":{'rawWmCategory':'photo_radar'}, //  Count: 1089
        "gatso":{'rawWmCategory':'gatso'}, //  Count: 1089
        "speed trap":{'rawWmCategory':'speed_trap'}, //  Count: 1089
        "traffic enforcement camera / detector":{'rawWmCategory':'traffic_enforcement_camera_/_detector'}, //  Count: 1089
        "speed camera":{'rawWmCategory':'speed_camera'}, //  Count: 1089
        "Seventh-day Adventist Church":{'rawWmCategory':'Seventh-day_Adventist_Church'}, //  Count: 1087
        "emergency (to be replaced)":{'rawWmCategory':'emergency_(to_be_replaced)'}, //  Count: 1087
        "neolithic age":{'rawWmCategory':'neolithic_age'}, //  Count: 1086
        "trade union":{'rawWmCategory':'trade_union'}, //  Count: 1085
        "labour union":{'rawWmCategory':'labour_union'}, //  Count: 1085
        "town centre":{'rawWmCategory':'town_centre'}, //  Count: 1079
        "carpet / flooring shop":{'rawWmCategory':'carpet_/_flooring_shop'}, //  Count: 1079
        "traffic light":{'rawWmCategory':'traffic_light'}, //  Count: 1078
        "degree college":{'rawWmCategory':'degree_college'}, //  Count: 1074
        "United States Air Force":{'rawWmCategory':'United_States_Air_Force'}, //  Count: 1071
        "office supply merchant /store":{'rawWmCategory':'office_supply_merchant_/store'}, //  Count: 1068
        "rail wye":{'rawWmCategory':'rail_wye'}, //  Count: 1068
        "railway junction":{'rawWmCategory':'railway_junction'}, //  Count: 1068
        "moveable bridge":{'rawWmCategory':'moveable_bridge'}, //  Count: 1064
        "karst":{'rawWmCategory':'karst'}, //  Count: 1063
        "youth organisation":{'rawWmCategory':'youth_organisation'}, //  Count: 1057
        "youth organization":{'rawWmCategory':'youth_organization'}, //  Count: 1057
        "watch repair/sales shop":{'rawWmCategory':'watch_repair/sales_shop'}, //  Count: 1055
        "american football stadium":{'rawWmCategory':'american_football_stadium'}, //  Count: 1055
        "cement plant":{'rawWmCategory':'cement_plant'}, //  Count: 1049
        "arcade (architecture)":{'rawWmCategory':'arcade_(architecture)'}, //  Count: 1047
        "canopy":{'rawWmCategory':'canopy'}, //  Count: 1046
        "mechanical engineering":{'rawWmCategory':'mechanical_engineering'}, //  Count: 1038
        "paintball":{'rawWmCategory':'paintball'}, //  Count: 1036
        "refuge":{'rawWmCategory':'refuge'}, //  Count: 1035
        "mountaineering":{'rawWmCategory':'mountaineering'}, //  Count: 1032
        "mountain climbing":{'rawWmCategory':'mountain_climbing'}, //  Count: 1032
        "17th century construction":{'rawWmCategory':'17th_century_construction'}, //  Count: 1032
        "constructivism":{'rawWmCategory':'constructivism'}, //  Count: 1031
        "fabrics":{'rawWmCategory':'fabrics'}, //  Count: 1029
        "walkway":{'rawWmCategory':'walkway'}, //  Count: 1024
        "1960_construction":{'rawWmCategory':'1960_construction'}, //  Count: 1022
        "automotive company":{'rawWmCategory':'automotive_company'}, //  Count: 1022
        "local museum":{'rawWmCategory':'local_museum'}, //  Count: 1021
        "peatbog":{'rawWmCategory':'peatbog'}, //  Count: 1021
        "Fishing / Hunting / Wilderness Lodge":{'rawWmCategory':'Fishing_/_Hunting_/_Wilderness_Lodge'}, //  Count: 1020
        "bistro":{'rawWmCategory':'bistro'}, //  Count: 1018
        "college of arts / science / commerce and industry":{'rawWmCategory':'college_of_arts_/_science_/_commerce_and_industry'}, //  Count: 1016
        "religious organisation":{'rawWmCategory':'religious_organisation'}, //  Count: 1015
        "design studio":{'rawWmCategory':'design_studio'}, //  Count: 1015
        "missionary order":{'rawWmCategory':'missionary_order'}, //  Count: 1015
        "religious centre":{'rawWmCategory':'religious_centre'}, //  Count: 1015
        "religious mission post":{'rawWmCategory':'religious_mission_post'}, //  Count: 1015
        "religious center":{'rawWmCategory':'religious_center'}, //  Count: 1015
        "derasar":{'rawWmCategory':'derasar'}, //  Count: 1014
        "Jain temple":{'rawWmCategory':'Jain_temple'}, //  Count: 1014
        "lotto":{'rawWmCategory':'lotto'}, //  Count: 1008
        "lottery":{'rawWmCategory':'lottery'}, //  Count: 1008
        "fraternal organization":{'rawWmCategory':'fraternal_organization'}, //  Count: 1007
        "fraternity":{'rawWmCategory':'fraternity'}, //  Count: 1007
        "thermal power station":{'rawWmCategory':'thermal_power_station'}, //  Count: 1006
        "Selsoviet":{'rawWmCategory':'Selsoviet'}, //  Count: 1005
        "paint":{'rawWmCategory':'paint'}, //  Count: 1002
        "Macedonian Orthodox Church - Ohrid Archbishopric":{'rawWmCategory':'Macedonian_Orthodox_Church_-_Ohrid_Archbishopric'}, //  Count: 1001
        "justice":{'rawWmCategory':'justice'}, //  Count: 1000
        "Gothic architecture":{'rawWmCategory':'Gothic_architecture'}, //  Count: 995
        "diner":{'rawWmCategory':'diner'}, //  Count: 993
        "SNCF":{'rawWmCategory':'SNCF'}, //  Count: 993
        "landscaping":{'rawWmCategory':'landscaping'}, //  Count: 992
        "mental / lunatic asylum":{'rawWmCategory':'mental_/_lunatic_asylum'}, //  Count: 991
        "insane asylum":{'rawWmCategory':'insane_asylum'}, //  Count: 991
        "mental institution / hospital":{'rawWmCategory':'mental_institution_/_hospital'}, //  Count: 991
        "psychiatric hospital / clinic":{'rawWmCategory':'psychiatric_hospital_/_clinic'}, //  Count: 991
        "service railway building":{'rawWmCategory':'service_railway_building'}, //  Count: 989
        "fair":{'rawWmCategory':'fair'}, //  Count: 988
        "historic site":{'rawWmCategory':'historic_site'}, //  Count: 983
        "sinkhole":{'rawWmCategory':'sinkhole'}, //  Count: 983
        "beer garden":{'rawWmCategory':'beer_garden'}, //  Count: 980
        "marketing":{'rawWmCategory':'marketing'}, //  Count: 973
        "Stalinist architecture":{'rawWmCategory':'Stalinist_architecture'}, //  Count: 968
        "shaft":{'rawWmCategory':'shaft'}, //  Count: 967
        "steel mill":{'rawWmCategory':'steel_mill'}, //  Count: 964
        "steelworks":{'rawWmCategory':'steelworks'}, //  Count: 964
        "ministry of finance / department of treasury":{'rawWmCategory':'ministry_of_finance_/_department_of_treasury'}, //  Count: 963
        "sheep farm":{'rawWmCategory':'sheep_farm'}, //  Count: 963
        "gas pipeline":{'rawWmCategory':'gas_pipeline'}, //  Count: 963
        "dargah":{'rawWmCategory':'dargah'}, //  Count: 963
        "children's hospital":{'rawWmCategory':'childrens_hospital'}, //  Count: 959
        "railway tunnel":{'rawWmCategory':'railway_tunnel'}, //  Count: 959
        "pediatric hospital":{'rawWmCategory':'pediatric_hospital'}, //  Count: 959
        "truss bridge":{'rawWmCategory':'truss_bridge'}, //  Count: 957
        "building machinery and equipment":{'rawWmCategory':'building_machinery_and_equipment'}, //  Count: 955
        "state park":{'rawWmCategory':'state_park'}, //  Count: 955
        "cargo":{'rawWmCategory':'cargo'}, //  Count: 954
        "bridal shop":{'rawWmCategory':'bridal_shop'}, //  Count: 953
        "weigh station / truck scale / weighbridge":{'rawWmCategory':'weigh_station_/_truck_scale_/_weighbridge'}, //  Count: 951
        "1962_construction":{'rawWmCategory':'1962_construction'}, //  Count: 950
        "1890s construction":{'rawWmCategory':'1890s_construction'}, //  Count: 949
        "tennis club":{'rawWmCategory':'tennis_club'}, //  Count: 949
        "food court":{'rawWmCategory':'food_court'}, //  Count: 948
        "aerospace industry":{'rawWmCategory':'aerospace_industry'}, //  Count: 946
        "Fachwerk style architecture":{'rawWmCategory':'Fachwerk_style_architecture'}, //  Count: 945
        "1959_construction":{'rawWmCategory':'1959_construction'}, //  Count: 944
        "resthouse":{'rawWmCategory':'resthouse'}, //  Count: 944
        "gambling":{'rawWmCategory':'gambling'}, //  Count: 944
        "pension (lodging)":{'rawWmCategory':'pension_(lodging)'}, //  Count: 943
        "discount store":{'rawWmCategory':'discount_store'}, //  Count: 940
        "country":{'rawWmCategory':'country'}, //  Count: 939
        "pet food / supplies":{'rawWmCategory':'pet_food_/_supplies'}, //  Count: 937
        "sports wear":{'rawWmCategory':'sports_wear'}, //  Count: 932
        "sportswear":{'rawWmCategory':'sportswear'}, //  Count: 932
        "uniform supply":{'rawWmCategory':'uniform_supply'}, //  Count: 930
        "military police":{'rawWmCategory':'military_police'}, //  Count: 929
        "ghanta ghar":{'rawWmCategory':'ghanta_ghar'}, //  Count: 928
        "clock tower":{'rawWmCategory':'clock_tower'}, //  Count: 928
        "1963_construction":{'rawWmCategory':'1963_construction'}, //  Count: 928
        "hermitage":{'rawWmCategory':'hermitage'}, //  Count: 927
        "flour mill":{'rawWmCategory':'flour_mill'}, //  Count: 925
        "grist mill":{'rawWmCategory':'grist_mill'}, //  Count: 925
        "Intermarche":{'rawWmCategory':'Intermarche'}, //  Count: 924
        "Intermediate college":{'rawWmCategory':'Intermediate_college'}, //  Count: 923
        "ladies hostel":{'rawWmCategory':'ladies_hostel'}, //  Count: 922
        "Christian Congregation of Brazil (CCB)":{'rawWmCategory':'Christian_Congregation_of_Brazil_(CCB)'}, //  Count: 922
        "women's hostel":{'rawWmCategory':'womens_hostel'}, //  Count: 922
        "girls hostel":{'rawWmCategory':'girls_hostel'}, //  Count: 922
        "staff quarters":{'rawWmCategory':'staff_quarters'}, //  Count: 921
        "television studio":{'rawWmCategory':'television_studio'}, //  Count: 921
        "festival":{'rawWmCategory':'festival'}, //  Count: 921
        "DME - distance measuring equipment":{'rawWmCategory':'DME_-_distance_measuring_equipment'}, //  Count: 921
        "1958_construction":{'rawWmCategory':'1958_construction'}, //  Count: 920
        "pharmaceutical":{'rawWmCategory':'pharmaceutical'}, //  Count: 920
        "terrain":{'rawWmCategory':'terrain'}, //  Count: 920
        "film studio":{'rawWmCategory':'film_studio'}, //  Count: 918
        "film/video production studio/facility":{'rawWmCategory':'film/video_production_studio/facility'}, //  Count: 918
        "covered reservoir - water":{'rawWmCategory':'covered_reservoir_-_water'}, //  Count: 912
        "scheduled ancient monument":{'rawWmCategory':'scheduled_ancient_monument'}, //  Count: 910
        "migration service":{'rawWmCategory':'migration_service'}, //  Count: 910
        "pillar":{'rawWmCategory':'pillar'}, //  Count: 901
        "column":{'rawWmCategory':'column'}, //  Count: 901
        "pension fund office (Russia)":{'rawWmCategory':'pension_fund_office_(Russia)'}, //  Count: 898
        "submarine":{'rawWmCategory':'submarine'}, //  Count: 898
        "consumer goods marketplace":{'rawWmCategory':'consumer_goods_marketplace'}, //  Count: 897
        "floodgate":{'rawWmCategory':'floodgate'}, //  Count: 897
        "1965_construction":{'rawWmCategory':'1965_construction'}, //  Count: 896
        "track - DONT USE - CATEGORY WILL BE REMOVED":{'rawWmCategory':'track_-_DONT_USE_-_CATEGORY_WILL_BE_REMOVED'}, //  Count: 894
        "internment":{'rawWmCategory':'internment'}, //  Count: 893
        "concentration camp":{'rawWmCategory':'concentration_camp'}, //  Count: 893
        "pagoda":{'rawWmCategory':'pagoda'}, //  Count: 892
        "student":{'rawWmCategory':'student'}, //  Count: 890
        "1964_construction":{'rawWmCategory':'1964_construction'}, //  Count: 889
        "battlefield":{'rawWmCategory':'battlefield'}, //  Count: 883
        "1961_construction":{'rawWmCategory':'1961_construction'}, //  Count: 882
        "cliff":{'rawWmCategory':'cliff'}, //  Count: 881
        "Vinayagar / Pillaiyar temple":{'rawWmCategory':'Vinayagar_/_Pillaiyar_temple'}, //  Count: 876
        "swimming":{'rawWmCategory':'swimming'}, //  Count: 876
        "Ganesha / Ganapati temple":{'rawWmCategory':'Ganesha_/_Ganapati_temple'}, //  Count: 876
        "lamp store":{'rawWmCategory':'lamp_store'}, //  Count: 874
        "lighting shop":{'rawWmCategory':'lighting_shop'}, //  Count: 874
        "track and field ground":{'rawWmCategory':'track_and_field_ground'}, //  Count: 872
        "airways":{'rawWmCategory':'airways'}, //  Count: 872
        "athletics track":{'rawWmCategory':'athletics_track'}, //  Count: 872
        "airline company":{'rawWmCategory':'airline_company'}, //  Count: 872
        "airlines":{'rawWmCategory':'airlines'}, //  Count: 872
        "cable/satellite TV provider":{'rawWmCategory':'cable/satellite_TV_provider'}, //  Count: 868
        "cable TV provider":{'rawWmCategory':'cable_TV_provider'}, //  Count: 868
        "plateau":{'rawWmCategory':'plateau'}, //  Count: 866
        "youth center":{'rawWmCategory':'youth_center'}, //  Count: 865
        "youth club":{'rawWmCategory':'youth_club'}, //  Count: 865
        "youth centre":{'rawWmCategory':'youth_centre'}, //  Count: 865
        "bicycle rental":{'rawWmCategory':'bicycle_rental'}, //  Count: 864
        "cycle hire":{'rawWmCategory':'cycle_hire'}, //  Count: 864
        "rescue service":{'rawWmCategory':'rescue_service'}, //  Count: 864
        "karaoke":{'rawWmCategory':'karaoke'}, //  Count: 863
        "railroad turntable":{'rawWmCategory':'railroad_turntable'}, //  Count: 861
        "cold storage warehouse":{'rawWmCategory':'cold_storage_warehouse'}, //  Count: 859
        "Carrefour Market":{'rawWmCategory':'Carrefour_Market'}, //  Count: 854
        "helicopter":{'rawWmCategory':'helicopter'}, //  Count: 853
        "onsen":{'rawWmCategory':'onsen'}, //  Count: 850
        "indoor stadium":{'rawWmCategory':'indoor_stadium'}, //  Count: 849
        "contractor":{'rawWmCategory':'contractor'}, //  Count: 849
        "fieldhouse (athletics building)":{'rawWmCategory':'fieldhouse_(athletics_building)'}, //  Count: 849
        "ophthalmology":{'rawWmCategory':'ophthalmology'}, //  Count: 844
        "bindery":{'rawWmCategory':'bindery'}, //  Count: 844
        "grotto":{'rawWmCategory':'grotto'}, //  Count: 844
        "printing office":{'rawWmCategory':'printing_office'}, //  Count: 844
        "Dunkin Donuts":{'rawWmCategory':'Dunkin_Donuts'}, //  Count: 844
        "beekeeping":{'rawWmCategory':'beekeeping'}, //  Count: 841
        "slaughterhouse / abattoir":{'rawWmCategory':'slaughterhouse_/_abattoir'}, //  Count: 840
        "station - Australian ranch":{'rawWmCategory':'station_-_Australian_ranch'}, //  Count: 839
        "steam locomotive":{'rawWmCategory':'steam_locomotive'}, //  Count: 838
        "bicycle stand":{'rawWmCategory':'bicycle_stand'}, //  Count: 837
        "alcohol distillery":{'rawWmCategory':'alcohol_distillery'}, //  Count: 837
        "kennels":{'rawWmCategory':'kennels'}, //  Count: 836
        "YMCA":{'rawWmCategory':'YMCA'}, //  Count: 835
        "police box":{'rawWmCategory':'police_box'}, //  Count: 834
        "fairground":{'rawWmCategory':'fairground'}, //  Count: 828
        "charity shop":{'rawWmCategory':'charity_shop'}, //  Count: 828
        "thrift store":{'rawWmCategory':'thrift_store'}, //  Count: 828
        "insurance broker":{'rawWmCategory':'insurance_broker'}, //  Count: 826
        "insurance advisor / agent":{'rawWmCategory':'insurance_advisor_/_agent'}, //  Count: 826
        "anglicanism":{'rawWmCategory':'anglicanism'}, //  Count: 824
        "sanctuary":{'rawWmCategory':'sanctuary'}, //  Count: 824
        "Adif":{'rawWmCategory':'Adif'}, //  Count: 822
        "2014_construction":{'rawWmCategory':'2014_construction'}, //  Count: 819
        "ceramics":{'rawWmCategory':'ceramics'}, //  Count: 819
        "object of cultural heritage of regional importance (Russia)":{'rawWmCategory':'object_of_cultural_heritage_of_regional_importance_(Russia)'}, //  Count: 816
        "maranatha":{'rawWmCategory':'maranatha'}, //  Count: 816
        "Ukrainian Greek Catholic Church (UGCC)":{'rawWmCategory':'Ukrainian_Greek_Catholic_Church_(UGCC)'}, //  Count: 815
        "border stone":{'rawWmCategory':'border_stone'}, //  Count: 812
        "border marker":{'rawWmCategory':'border_marker'}, //  Count: 812
        "1966_construction":{'rawWmCategory':'1966_construction'}, //  Count: 812
        "boundary marker":{'rawWmCategory':'boundary_marker'}, //  Count: 812
        "game center":{'rawWmCategory':'game_center'}, //  Count: 811
        "amusement (video) arcade":{'rawWmCategory':'amusement_(video)_arcade'}, //  Count: 811
        "video arcade":{'rawWmCategory':'video_arcade'}, //  Count: 811
        "department of law":{'rawWmCategory':'department_of_law'}, //  Count: 806
        "college of law":{'rawWmCategory':'college_of_law'}, //  Count: 806
        "state":{'rawWmCategory':'state'}, //  Count: 806
        "faculty of law / law school":{'rawWmCategory':'faculty_of_law_/_law_school'}, //  Count: 806
        "breakdown cover":{'rawWmCategory':'breakdown_cover'}, //  Count: 804
        "road service":{'rawWmCategory':'road_service'}, //  Count: 804
        "emergency roadside repair":{'rawWmCategory':'emergency_roadside_repair'}, //  Count: 804
        "roadside assistance":{'rawWmCategory':'roadside_assistance'}, //  Count: 804
        "mandal":{'rawWmCategory':'mandal'}, //  Count: 804
        "1967_construction":{'rawWmCategory':'1967_construction'}, //  Count: 804
        "taluka / taluk":{'rawWmCategory':'taluka_/_taluk'}, //  Count: 804
        "tehsil":{'rawWmCategory':'tehsil'}, //  Count: 804
        "package delivery":{'rawWmCategory':'package_delivery'}, //  Count: 801
        "microwave tower / microwave transmission":{'rawWmCategory':'microwave_tower_/_microwave_transmission'}, //  Count: 798
        "university college":{'rawWmCategory':'university_college'}, //  Count: 796
        "courier":{'rawWmCategory':'courier'}, //  Count: 793
        "Carrefour":{'rawWmCategory':'Carrefour'}, //  Count: 790
        "1970_construction":{'rawWmCategory':'1970_construction'}, //  Count: 789
        "second level parliament":{'rawWmCategory':'second_level_parliament'}, //  Count: 787
        "irrigation":{'rawWmCategory':'irrigation'}, //  Count: 787
        "lienzo":{'rawWmCategory':'lienzo'}, //  Count: 785
        "cram school":{'rawWmCategory':'cram_school'}, //  Count: 783
        "tutoring services":{'rawWmCategory':'tutoring_services'}, //  Count: 783
        "passport office":{'rawWmCategory':'passport_office'}, //  Count: 782
        "portal":{'rawWmCategory':'portal'}, //  Count: 782
        "indoor bowls":{'rawWmCategory':'indoor_bowls'}, //  Count: 777
        "bowling green":{'rawWmCategory':'bowling_green'}, //  Count: 777
        "lawn bowls":{'rawWmCategory':'lawn_bowls'}, //  Count: 777
        "marine mammal park":{'rawWmCategory':'marine_mammal_park'}, //  Count: 776
        "Amtrak":{'rawWmCategory':'Amtrak'}, //  Count: 776
        "public aquarium / oceanarium / seaquarium":{'rawWmCategory':'public_aquarium_/_oceanarium_/_seaquarium'}, //  Count: 776
        "surgery":{'rawWmCategory':'surgery'}, //  Count: 775
        "PrivatBank":{'rawWmCategory':'PrivatBank'}, //  Count: 773
        "mess":{'rawWmCategory':'mess'}, //  Count: 772
        "7-Eleven":{'rawWmCategory':'7-Eleven'}, //  Count: 768
        "Banco Bradesco":{'rawWmCategory':'Banco_Bradesco'}, //  Count: 768
        "Aldi":{'rawWmCategory':'Aldi'}, //  Count: 763
        "indoor ice rink":{'rawWmCategory':'indoor_ice_rink'}, //  Count: 760
        "pesantren (islamic boarding school)":{'rawWmCategory':'pesantren_(islamic_boarding_school)'}, //  Count: 759
        "safety":{'rawWmCategory':'safety'}, //  Count: 759
        "harbour":{'rawWmCategory':'harbour'}, //  Count: 755
        "harbor":{'rawWmCategory':'harbor'}, //  Count: 755
        "haven":{'rawWmCategory':'haven'}, //  Count: 755
        "Hilton Worldwide":{'rawWmCategory':'Hilton_Worldwide'}, //  Count: 752
        "football fourth league":{'rawWmCategory':'football_fourth_league'}, //  Count: 750
        "watermill":{'rawWmCategory':'watermill'}, //  Count: 748
        "covered bridge":{'rawWmCategory':'covered_bridge'}, //  Count: 748
        "Arby&#039;s":{'rawWmCategory':'Arby&#039;s'}, //  Count: 748
        "linen":{'rawWmCategory':'linen'}, //  Count: 745
        "scenic lookout":{'rawWmCategory':'scenic_lookout'}, //  Count: 742
        "basilica":{'rawWmCategory':'basilica'}, //  Count: 740
        "Banco Itau":{'rawWmCategory':'Banco_Itau'}, //  Count: 737
        "yoga":{'rawWmCategory':'yoga'}, //  Count: 736
        "adult education":{'rawWmCategory':'adult_education'}, //  Count: 735
        "space agency":{'rawWmCategory':'space_agency'}, //  Count: 733
        "maternity welfare center":{'rawWmCategory':'maternity_welfare_center'}, //  Count: 731
        "bench (furniture)":{'rawWmCategory':'bench_(furniture)'}, //  Count: 731
        "hellenic":{'rawWmCategory':'hellenic'}, //  Count: 730
        "professional development":{'rawWmCategory':'professional_development'}, //  Count: 729
        "road tunnel":{'rawWmCategory':'road_tunnel'}, //  Count: 728
        "gravel pit":{'rawWmCategory':'gravel_pit'}, //  Count: 726
        "agro-town":{'rawWmCategory':'agro-town'}, //  Count: 726
        "1968_construction":{'rawWmCategory':'1968_construction'}, //  Count: 726
        "curtains":{'rawWmCategory':'curtains'}, //  Count: 726
        "lodging":{'rawWmCategory':'lodging'}, //  Count: 725
        "building society":{'rawWmCategory':'building_society'}, //  Count: 725
        "ditch":{'rawWmCategory':'ditch'}, //  Count: 725
        "Housing Development Board":{'rawWmCategory':'Housing_Development_Board'}, //  Count: 722
        "cellar":{'rawWmCategory':'cellar'}, //  Count: 721
        "representation":{'rawWmCategory':'representation'}, //  Count: 720
        "oil refinery":{'rawWmCategory':'oil_refinery'}, //  Count: 719
        "petroleum refinery":{'rawWmCategory':'petroleum_refinery'}, //  Count: 719
        "investment":{'rawWmCategory':'investment'}, //  Count: 719
        "pipe bridge":{'rawWmCategory':'pipe_bridge'}, //  Count: 718
        "Perumal temple":{'rawWmCategory':'Perumal_temple'}, //  Count: 717
        "Vishnalayam":{'rawWmCategory':'Vishnalayam'}, //  Count: 717
        "Vishnu temple":{'rawWmCategory':'Vishnu_temple'}, //  Count: 717
        "jewish cemetery":{'rawWmCategory':'jewish_cemetery'}, //  Count: 717
        "American Civil War 1861-1865":{'rawWmCategory':'American_Civil_War_1861-1865'}, //  Count: 717
        "rugby ground":{'rawWmCategory':'rugby_ground'}, //  Count: 716
        "wasteland":{'rawWmCategory':'wasteland'}, //  Count: 714
        "ILS - instrument landing system":{'rawWmCategory':'ILS_-_instrument_landing_system'}, //  Count: 714
        "olive oil mill":{'rawWmCategory':'olive_oil_mill'}, //  Count: 712
        "palm oil mill":{'rawWmCategory':'palm_oil_mill'}, //  Count: 712
        "oil mill":{'rawWmCategory':'oil_mill'}, //  Count: 712
        "vegetable oil mill":{'rawWmCategory':'vegetable_oil_mill'}, //  Count: 712
        "vocational courses":{'rawWmCategory':'vocational_courses'}, //  Count: 711
        "girls school":{'rawWmCategory':'girls_school'}, //  Count: 710
        "arts and crafts store":{'rawWmCategory':'arts_and_crafts_store'}, //  Count: 709
        "sandwich bar/sandwich wrap shop":{'rawWmCategory':'sandwich_bar/sandwich_wrap_shop'}, //  Count: 708
        "confectionery plant":{'rawWmCategory':'confectionery_plant'}, //  Count: 708
        "United States Coast Guard":{'rawWmCategory':'United_States_Coast_Guard'}, //  Count: 707
        "geodetics":{'rawWmCategory':'geodetics'}, //  Count: 706
        "community college":{'rawWmCategory':'community_college'}, //  Count: 705
        "forest nursery":{'rawWmCategory':'forest_nursery'}, //  Count: 705
        "tattoo shop":{'rawWmCategory':'tattoo_shop'}, //  Count: 703
        "tattoo parlor / studio":{'rawWmCategory':'tattoo_parlor_/_studio'}, //  Count: 703
        "cogeneration power plant":{'rawWmCategory':'cogeneration_power_plant'}, //  Count: 702
        "farmers market":{'rawWmCategory':'farmers_market'}, //  Count: 702
        "combined heat and power plant":{'rawWmCategory':'combined_heat_and_power_plant'}, //  Count: 702
        "oil platform":{'rawWmCategory':'oil_platform'}, //  Count: 701
        "offshore oil rig":{'rawWmCategory':'offshore_oil_rig'}, //  Count: 701
        "1974_construction":{'rawWmCategory':'1974_construction'}, //  Count: 701
        "1969_construction":{'rawWmCategory':'1969_construction'}, //  Count: 700
        "apiary":{'rawWmCategory':'apiary'}, //  Count: 699
        "metalware":{'rawWmCategory':'metalware'}, //  Count: 699
        "dancing - DONT USE - CATEGORY WILL BE REMOVED":{'rawWmCategory':'dancing_-_DONT_USE_-_CATEGORY_WILL_BE_REMOVED'}, //  Count: 698
        "Oschadbank":{'rawWmCategory':'Oschadbank'}, //  Count: 698
        "dance - DONT USE - CATEGORY WILL BE REMOVED":{'rawWmCategory':'dance_-_DONT_USE_-_CATEGORY_WILL_BE_REMOVED'}, //  Count: 698
        "party goods supplier":{'rawWmCategory':'party_goods_supplier'}, //  Count: 697
        "1972_construction":{'rawWmCategory':'1972_construction'}, //  Count: 696
        "woodland":{'rawWmCategory':'woodland'}, //  Count: 695
        "coppice":{'rawWmCategory':'coppice'}, //  Count: 695
        "alpine hut":{'rawWmCategory':'alpine_hut'}, //  Count: 694
        "mountain hostel":{'rawWmCategory':'mountain_hostel'}, //  Count: 694
        "mountain shelter":{'rawWmCategory':'mountain_shelter'}, //  Count: 694
        "mountain hut":{'rawWmCategory':'mountain_hut'}, //  Count: 694
        "caixa economica federal":{'rawWmCategory':'caixa_economica_federal'}, //  Count: 693
        "bastion (fortification part)":{'rawWmCategory':'bastion_(fortification_part)'}, //  Count: 692
        "tea and coffee merchants":{'rawWmCategory':'tea_and_coffee_merchants'}, //  Count: 692
        "church hall":{'rawWmCategory':'church_hall'}, //  Count: 691
        "parish hall":{'rawWmCategory':'parish_hall'}, //  Count: 691
        "Lowes":{'rawWmCategory':'Lowes'}, //  Count: 691
        "Lowe's":{'rawWmCategory':'Lowes'}, //  Count: 691
        "astronomy":{'rawWmCategory':'astronomy'}, //  Count: 691
        "astronomical":{'rawWmCategory':'astronomical'}, //  Count: 691
        "Taras Shevchenko":{'rawWmCategory':'Taras_Shevchenko'}, //  Count: 690
        "information centre":{'rawWmCategory':'information_centre'}, //  Count: 689
        "trailhead":{'rawWmCategory':'trailhead'}, //  Count: 688
        "balneario":{'rawWmCategory':'balneario'}, //  Count: 685
        "history museum":{'rawWmCategory':'history_museum'}, //  Count: 685
        "visitor / park center":{'rawWmCategory':'visitor_/_park_center'}, //  Count: 685
        "benchmark (survey)":{'rawWmCategory':'benchmark_(survey)'}, //  Count: 684
        "artillery":{'rawWmCategory':'artillery'}, //  Count: 683
        "webcam":{'rawWmCategory':'webcam'}, //  Count: 683
        "home electronics / appliance repair service":{'rawWmCategory':'home_electronics_/_appliance_repair_service'}, //  Count: 683
        "cam":{'rawWmCategory':'cam'}, //  Count: 683
        "coaching":{'rawWmCategory':'coaching'}, //  Count: 683
        "shawarma/ doner kebab/ gyros":{'rawWmCategory':'shawarma/_doner_kebab/_gyros'}, //  Count: 682
        "former air force base":{'rawWmCategory':'former_air_force_base'}, //  Count: 681
        "locomotive on display":{'rawWmCategory':'locomotive_on_display'}, //  Count: 681
        "1957_construction":{'rawWmCategory':'1957_construction'}, //  Count: 681
        "borehole":{'rawWmCategory':'borehole'}, //  Count: 679
        "intelligence agency":{'rawWmCategory':'intelligence_agency'}, //  Count: 679
        "borewell":{'rawWmCategory':'borewell'}, //  Count: 679
        "truck repair":{'rawWmCategory':'truck_repair'}, //  Count: 678
        "NASA":{'rawWmCategory':'NASA'}, //  Count: 678
        "music instruments":{'rawWmCategory':'music_instruments'}, //  Count: 677
        "river delta":{'rawWmCategory':'river_delta'}, //  Count: 677
        "strategic rocket forces":{'rawWmCategory':'strategic_rocket_forces'}, //  Count: 676
        "radio beacon":{'rawWmCategory':'radio_beacon'}, //  Count: 675
        "green - TO BE REMOVED":{'rawWmCategory':'green_-_TO_BE_REMOVED'}, //  Count: 672
        "health/organic food store":{'rawWmCategory':'health/organic_food_store'}, //  Count: 671
        "bungalow park":{'rawWmCategory':'bungalow_park'}, //  Count: 670
        "nunnery":{'rawWmCategory':'nunnery'}, //  Count: 670
        "broadcasting":{'rawWmCategory':'broadcasting'}, //  Count: 669
        "environmental organisation":{'rawWmCategory':'environmental_organisation'}, //  Count: 668
        "environmental protection agency":{'rawWmCategory':'environmental_protection_agency'}, //  Count: 668
        "multistorey":{'rawWmCategory':'multistorey'}, //  Count: 668
        "environmental organization":{'rawWmCategory':'environmental_organization'}, //  Count: 668
        "dentistry":{'rawWmCategory':'dentistry'}, //  Count: 666
        "menswear":{'rawWmCategory':'menswear'}, //  Count: 666
        "scrap metal dealer":{'rawWmCategory':'scrap_metal_dealer'}, //  Count: 665
        "festas":{'rawWmCategory':'festas'}, //  Count: 664
        "churrascaria":{'rawWmCategory':'churrascaria'}, //  Count: 664
        "2013_construction":{'rawWmCategory':'2013_construction'}, //  Count: 662
        "women's / girls college":{'rawWmCategory':'womens_/_girls_college'}, //  Count: 661
        "1971_construction":{'rawWmCategory':'1971_construction'}, //  Count: 661
        "masonic hall":{'rawWmCategory':'masonic_hall'}, //  Count: 661
        "freemason / masonic temple":{'rawWmCategory':'freemason_/_masonic_temple'}, //  Count: 661
        "masonic building":{'rawWmCategory':'masonic_building'}, //  Count: 661
        "backwater":{'rawWmCategory':'backwater'}, //  Count: 658
        "equipment rental":{'rawWmCategory':'equipment_rental'}, //  Count: 656
        "oil change station":{'rawWmCategory':'oil_change_station'}, //  Count: 655
        "cenote":{'rawWmCategory':'cenote'}, //  Count: 655
        "Episcopal Church":{'rawWmCategory':'Episcopal_Church'}, //  Count: 653
        "roller coaster":{'rawWmCategory':'roller_coaster'}, //  Count: 652
        "traction substation of public transport":{'rawWmCategory':'traction_substation_of_public_transport'}, //  Count: 650
        "deck":{'rawWmCategory':'deck'}, //  Count: 650
        "chawl":{'rawWmCategory':'chawl'}, //  Count: 650
        "textiles":{'rawWmCategory':'textiles'}, //  Count: 648
        "concert":{'rawWmCategory':'concert'}, //  Count: 645
        "concerts":{'rawWmCategory':'concerts'}, //  Count: 645
        "steel - to be removed":{'rawWmCategory':'steel_-_to_be_removed'}, //  Count: 644
        "bookie":{'rawWmCategory':'bookie'}, //  Count: 643
        "bookmaker":{'rawWmCategory':'bookmaker'}, //  Count: 643
        "bus rapid transit (BRT) station":{'rawWmCategory':'bus_rapid_transit_(BRT)_station'}, //  Count: 643
        "provincial":{'rawWmCategory':'provincial'}, //  Count: 643
        "webdevelopment":{'rawWmCategory':'webdevelopment'}, //  Count: 643
        "betting shop":{'rawWmCategory':'betting_shop'}, //  Count: 643
        "province":{'rawWmCategory':'province'}, //  Count: 643
        "1980_construction":{'rawWmCategory':'1980_construction'}, //  Count: 642
        "boating":{'rawWmCategory':'boating'}, //  Count: 641
        "alternative education":{'rawWmCategory':'alternative_education'}, //  Count: 639
        "professional organisation / association":{'rawWmCategory':'professional_organisation_/_association'}, //  Count: 639
        "adult - to be replaced / deleted":{'rawWmCategory':'adult_-_to_be_replaced_/_deleted'}, //  Count: 638
        "data center":{'rawWmCategory':'data_center'}, //  Count: 638
        "paddle tennis / padel court":{'rawWmCategory':'paddle_tennis_/_padel_court'}, //  Count: 638
        "data centre":{'rawWmCategory':'data_centre'}, //  Count: 638
        "general store":{'rawWmCategory':'general_store'}, //  Count: 637
        "general merchandise store":{'rawWmCategory':'general_merchandise_store'}, //  Count: 637
        "builders merchant":{'rawWmCategory':'builders_merchant'}, //  Count: 637
        "grazing area":{'rawWmCategory':'grazing_area'}, //  Count: 636
        "pasture":{'rawWmCategory':'pasture'}, //  Count: 636
        "dunes":{'rawWmCategory':'dunes'}, //  Count: 634
        "video game console store / shop":{'rawWmCategory':'video_game_console_store_/_shop'}, //  Count: 632
        "video games store / shop":{'rawWmCategory':'video_games_store_/_shop'}, //  Count: 632
        "field camp":{'rawWmCategory':'field_camp'}, //  Count: 631
        "1973_construction":{'rawWmCategory':'1973_construction'}, //  Count: 631
        "lingerie shop":{'rawWmCategory':'lingerie_shop'}, //  Count: 631
        "multipurpose":{'rawWmCategory':'multipurpose'}, //  Count: 630
        "sub-bairro":{'rawWmCategory':'sub-bairro'}, //  Count: 629
        "rice":{'rawWmCategory':'rice'}, //  Count: 629
        "butte":{'rawWmCategory':'butte'}, //  Count: 628
        "discounter":{'rawWmCategory':'discounter'}, //  Count: 628
        "open-air cage":{'rawWmCategory':'open-air_cage'}, //  Count: 627
        "prostitution place / area":{'rawWmCategory':'prostitution_place_/_area'}, //  Count: 627
        "humane society":{'rawWmCategory':'humane_society'}, //  Count: 626
        "animal shelter":{'rawWmCategory':'animal_shelter'}, //  Count: 626
        "coal-fired":{'rawWmCategory':'coal-fired'}, //  Count: 625
        "skyway":{'rawWmCategory':'skyway'}, //  Count: 624
        "sky walk":{'rawWmCategory':'sky_walk'}, //  Count: 624
        "department - administrative division":{'rawWmCategory':'department_-_administrative_division'}, //  Count: 623
        "Native American":{'rawWmCategory':'Native_American'}, //  Count: 622
        "gord (fort)":{'rawWmCategory':'gord_(fort)'}, //  Count: 620
        "first-level administrative division":{'rawWmCategory':'first-level_administrative_division'}, //  Count: 619
        "uninhabited island":{'rawWmCategory':'uninhabited_island'}, //  Count: 617
        "desert island":{'rawWmCategory':'desert_island'}, //  Count: 617
        "giants' grave":{'rawWmCategory':'giants_grave'}, //  Count: 614
        "1975_construction":{'rawWmCategory':'1975_construction'}, //  Count: 614
        "trench":{'rawWmCategory':'trench'}, //  Count: 614
        "Chinese temple":{'rawWmCategory':'Chinese_temple'}, //  Count: 612
        "architect":{'rawWmCategory':'architect'}, //  Count: 612
        "service academy":{'rawWmCategory':'service_academy'}, //  Count: 612
        "military academy":{'rawWmCategory':'military_academy'}, //  Count: 612
        "2008_construction":{'rawWmCategory':'2008_construction'}, //  Count: 610
        "1979_construction":{'rawWmCategory':'1979_construction'}, //  Count: 609
        "landline telephone service provider":{'rawWmCategory':'landline_telephone_service_provider'}, //  Count: 608
        "aircraft museum":{'rawWmCategory':'aircraft_museum'}, //  Count: 606
        "aviation museum":{'rawWmCategory':'aviation_museum'}, //  Count: 606
        "aerospace museum":{'rawWmCategory':'aerospace_museum'}, //  Count: 606
        "dismantled":{'rawWmCategory':'dismantled'}, //  Count: 606
        "air museum":{'rawWmCategory':'air_museum'}, //  Count: 606
        "Neo-Gothic architecture":{'rawWmCategory':'Neo-Gothic_architecture'}, //  Count: 605
        "Gothic revival architecture":{'rawWmCategory':'Gothic_revival_architecture'}, //  Count: 605
        "Victorian Gothic architecture":{'rawWmCategory':'Victorian_Gothic_architecture'}, //  Count: 605
        "Ford car dealer":{'rawWmCategory':'Ford_car_dealer'}, //  Count: 605
        "barrage":{'rawWmCategory':'barrage'}, //  Count: 605
        "megalithic tomb":{'rawWmCategory':'megalithic_tomb'}, //  Count: 602
        "aquatic center":{'rawWmCategory':'aquatic_center'}, //  Count: 601
        "leisure centre":{'rawWmCategory':'leisure_centre'}, //  Count: 601
        "1977_construction":{'rawWmCategory':'1977_construction'}, //  Count: 601
        "pyramid":{'rawWmCategory':'pyramid'}, //  Count: 600
        "Exxon Mobil":{'rawWmCategory':'Exxon_Mobil'}, //  Count: 599
        "bags":{'rawWmCategory':'bags'}, //  Count: 597
        "swing":{'rawWmCategory':'swing'}, //  Count: 597
        "mohalla":{'rawWmCategory':'mohalla'}, //  Count: 596
        "muhalla":{'rawWmCategory':'muhalla'}, //  Count: 596
        "mohallah":{'rawWmCategory':'mohallah'}, //  Count: 596
        "Anglican church":{'rawWmCategory':'Anglican_church'}, //  Count: 594
        "flying club":{'rawWmCategory':'flying_club'}, //  Count: 593
        "gliding club":{'rawWmCategory':'gliding_club'}, //  Count: 593
        "aero club":{'rawWmCategory':'aero_club'}, //  Count: 593
        "glass industry":{'rawWmCategory':'glass_industry'}, //  Count: 593
        "bikini bar":{'rawWmCategory':'bikini_bar'}, //  Count: 592
        "industrial park":{'rawWmCategory':'industrial_park'}, //  Count: 592
        "strip club":{'rawWmCategory':'strip_club'}, //  Count: 592
        "badminton court":{'rawWmCategory':'badminton_court'}, //  Count: 591
        "chiropractor":{'rawWmCategory':'chiropractor'}, //  Count: 590
        "observation":{'rawWmCategory':'observation'}, //  Count: 589
        "locksmith / key cutter":{'rawWmCategory':'locksmith_/_key_cutter'}, //  Count: 587
        "coal storage/terminal":{'rawWmCategory':'coal_storage/terminal'}, //  Count: 586
        "nuclear reactor":{'rawWmCategory':'nuclear_reactor'}, //  Count: 586
        "flea market":{'rawWmCategory':'flea_market'}, //  Count: 586
        "pylon":{'rawWmCategory':'pylon'}, //  Count: 586
        "2012_construction":{'rawWmCategory':'2012_construction'}, //  Count: 585
        "Check Dam":{'rawWmCategory':'Check_Dam'}, //  Count: 585
        "spit/sandspit":{'rawWmCategory':'spit/sandspit'}, //  Count: 585
        "1880s construction":{'rawWmCategory':'1880s_construction'}, //  Count: 584
        "tax preparation service":{'rawWmCategory':'tax_preparation_service'}, //  Count: 584
        "income tax preparation service":{'rawWmCategory':'income_tax_preparation_service'}, //  Count: 584
        "bairro rural":{'rawWmCategory':'bairro_rural'}, //  Count: 583
        "compressor station":{'rawWmCategory':'compressor_station'}, //  Count: 583
        "bathroom / shower facility":{'rawWmCategory':'bathroom_/_shower_facility'}, //  Count: 581
        "1981_construction":{'rawWmCategory':'1981_construction'}, //  Count: 581
        "cng":{'rawWmCategory':'cng'}, //  Count: 581
        "paper mill":{'rawWmCategory':'paper_mill'}, //  Count: 581
        "mobile phone repair":{'rawWmCategory':'mobile_phone_repair'}, //  Count: 580
        "costume jewelry":{'rawWmCategory':'costume_jewelry'}, //  Count: 580
        "cellphone repair":{'rawWmCategory':'cellphone_repair'}, //  Count: 580
        "paragliding":{'rawWmCategory':'paragliding'}, //  Count: 577
        "recorder of deeds":{'rawWmCategory':'recorder_of_deeds'}, //  Count: 576
        "land registry office":{'rawWmCategory':'land_registry_office'}, //  Count: 576
        "registrar of deeds":{'rawWmCategory':'registrar_of_deeds'}, //  Count: 576
        "bullring":{'rawWmCategory':'bullring'}, //  Count: 574
        "computer education":{'rawWmCategory':'computer_education'}, //  Count: 573
        "computer training institute":{'rawWmCategory':'computer_training_institute'}, //  Count: 573
        "1976_construction":{'rawWmCategory':'1976_construction'}, //  Count: 573
        "provider - TO BE REMOVED":{'rawWmCategory':'provider_-_TO_BE_REMOVED'}, //  Count: 571
        "handling":{'rawWmCategory':'handling'}, //  Count: 571
        "travel / outdoor shop":{'rawWmCategory':'travel_/_outdoor_shop'}, //  Count: 570
        "entreprise":{'rawWmCategory':'entreprise'}, //  Count: 569
        "liquefied petroleum gas (LPG)":{'rawWmCategory':'liquefied_petroleum_gas_(LPG)'}, //  Count: 569
        "gpl":{'rawWmCategory':'gpl'}, //  Count: 569
        "Leclerc":{'rawWmCategory':'Leclerc'}, //  Count: 568
        "letter box":{'rawWmCategory':'letter_box'}, //  Count: 567
        "pillar box":{'rawWmCategory':'pillar_box'}, //  Count: 567
        "post box":{'rawWmCategory':'post_box'}, //  Count: 567
        "mailbox":{'rawWmCategory':'mailbox'}, //  Count: 567
        "light rail":{'rawWmCategory':'light_rail'}, //  Count: 567
        "postbox":{'rawWmCategory':'postbox'}, //  Count: 567
        "GULAG":{'rawWmCategory':'GULAG'}, //  Count: 566
        "enhanced education":{'rawWmCategory':'enhanced_education'}, //  Count: 565
        "1978_construction":{'rawWmCategory':'1978_construction'}, //  Count: 565
        "Functionalism architecture":{'rawWmCategory':'Functionalism_architecture'}, //  Count: 565
        "firefighting equipment":{'rawWmCategory':'firefighting_equipment'}, //  Count: 564
        "rock/ pop music venue":{'rawWmCategory':'rock/_pop_music_venue'}, //  Count: 564
        "dermatology clinic / skin clinic":{'rawWmCategory':'dermatology_clinic_/_skin_clinic'}, //  Count: 562
        "dermatologist":{'rawWmCategory':'dermatologist'}, //  Count: 562
        "dermatology hospital":{'rawWmCategory':'dermatology_hospital'}, //  Count: 562
        "dermatology":{'rawWmCategory':'dermatology'}, //  Count: 562
        "underwater":{'rawWmCategory':'underwater'}, //  Count: 562
        "rowhouse":{'rawWmCategory':'rowhouse'}, //  Count: 561
        "forest ranger's house/park ranger's house":{'rawWmCategory':'forest_rangers_house/park_rangers_house'}, //  Count: 560
        "forest ranger's office":{'rawWmCategory':'forest_rangers_office'}, //  Count: 560
        "rotunda":{'rawWmCategory':'rotunda'}, //  Count: 559
        "school of management / business school":{'rawWmCategory':'school_of_management_/_business_school'}, //  Count: 557
        "brick gothic architecture":{'rawWmCategory':'brick_gothic_architecture'}, //  Count: 557
        "reinforced concrete structures":{'rawWmCategory':'reinforced_concrete_structures'}, //  Count: 556
        "mattress":{'rawWmCategory':'mattress'}, //  Count: 556
        "industrial equipment":{'rawWmCategory':'industrial_equipment'}, //  Count: 555
        "receptions":{'rawWmCategory':'receptions'}, //  Count: 552
        "reception":{'rawWmCategory':'reception'}, //  Count: 552
        "natural gas compressor station":{'rawWmCategory':'natural_gas_compressor_station'}, //  Count: 551
        "meeting house":{'rawWmCategory':'meeting_house'}, //  Count: 551
        "gateway":{'rawWmCategory':'gateway'}, //  Count: 551
        "natural gas pumping station":{'rawWmCategory':'natural_gas_pumping_station'}, //  Count: 551
        "factory outlet":{'rawWmCategory':'factory_outlet'}, //  Count: 549
        "personal":{'rawWmCategory':'personal'}, //  Count: 549
        "outlet store":{'rawWmCategory':'outlet_store'}, //  Count: 549
        "classicism":{'rawWmCategory':'classicism'}, //  Count: 548
        "car racing track":{'rawWmCategory':'car_racing_track'}, //  Count: 547
        "motor racing track":{'rawWmCategory':'motor_racing_track'}, //  Count: 547
        "auto racing track":{'rawWmCategory':'auto_racing_track'}, //  Count: 547
        "equestrian statue":{'rawWmCategory':'equestrian_statue'}, //  Count: 546
        "Armenian Apostolic church":{'rawWmCategory':'Armenian_Apostolic_church'}, //  Count: 546
        "biedronka":{'rawWmCategory':'biedronka'}, //  Count: 545
        "elderate":{'rawWmCategory':'elderate'}, //  Count: 545
        "eldership":{'rawWmCategory':'eldership'}, //  Count: 545
        "telegraph":{'rawWmCategory':'telegraph'}, //  Count: 545
        "fashion accessories store / shop":{'rawWmCategory':'fashion_accessories_store_/_shop'}, //  Count: 544
        "poblado":{'rawWmCategory':'poblado'}, //  Count: 544
        "haberdashery":{'rawWmCategory':'haberdashery'}, //  Count: 542
        "pilgrimage":{'rawWmCategory':'pilgrimage'}, //  Count: 542
        "necropolis":{'rawWmCategory':'necropolis'}, //  Count: 542
        "office equipment":{'rawWmCategory':'office_equipment'}, //  Count: 541
        "lift - elevator":{'rawWmCategory':'lift_-_elevator'}, //  Count: 541
        "earthquake epicenter":{'rawWmCategory':'earthquake_epicenter'}, //  Count: 541
        "call center / contact center":{'rawWmCategory':'call_center_/_contact_center'}, //  Count: 539
        "call centre":{'rawWmCategory':'call_centre'}, //  Count: 539
        "callcenter":{'rawWmCategory':'callcenter'}, //  Count: 539
        "truck dealer":{'rawWmCategory':'truck_dealer'}, //  Count: 534
        "mailing service":{'rawWmCategory':'mailing_service'}, //  Count: 532
        "art deco architectural style":{'rawWmCategory':'art_deco_architectural_style'}, //  Count: 532
        "1983_construction":{'rawWmCategory':'1983_construction'}, //  Count: 531
        "yacht":{'rawWmCategory':'yacht'}, //  Count: 531
        "legislature":{'rawWmCategory':'legislature'}, //  Count: 531
        "parliament":{'rawWmCategory':'parliament'}, //  Count: 531
        "velodrome":{'rawWmCategory':'velodrome'}, //  Count: 530
        "Tim Hortons":{'rawWmCategory':'Tim_Hortons'}, //  Count: 529
        "Old Believers":{'rawWmCategory':'Old_Believers'}, //  Count: 528
        "recovery of waste":{'rawWmCategory':'recovery_of_waste'}, //  Count: 528
        "cinder cone":{'rawWmCategory':'cinder_cone'}, //  Count: 528
        "climbing":{'rawWmCategory':'climbing'}, //  Count: 525
        "lanhouse":{'rawWmCategory':'lanhouse'}, //  Count: 525
        "polytechnic":{'rawWmCategory':'polytechnic'}, //  Count: 525
        "disability organization":{'rawWmCategory':'disability_organization'}, //  Count: 524
        "money transfer":{'rawWmCategory':'money_transfer'}, //  Count: 524
        "1982_construction":{'rawWmCategory':'1982_construction'}, //  Count: 523
        "parachuting":{'rawWmCategory':'parachuting'}, //  Count: 522
        "Neoclassical architecture":{'rawWmCategory':'Neoclassical_architecture'}, //  Count: 522
        "skydiving":{'rawWmCategory':'skydiving'}, //  Count: 522
        "graphic design":{'rawWmCategory':'graphic_design'}, //  Count: 521
        "welding":{'rawWmCategory':'welding'}, //  Count: 521
        "dharamshala":{'rawWmCategory':'dharamshala'}, //  Count: 521
        "swimming pool store":{'rawWmCategory':'swimming_pool_store'}, //  Count: 520
        "rubber":{'rawWmCategory':'rubber'}, //  Count: 520
        "retreat centre (spiritual)":{'rawWmCategory':'retreat_centre_(spiritual)'}, //  Count: 518
        "reception hall":{'rawWmCategory':'reception_hall'}, //  Count: 518
        "Toyota":{'rawWmCategory':'Toyota'}, //  Count: 518
        "foyer":{'rawWmCategory':'foyer'}, //  Count: 518
        "universal church of the kingdom of god":{'rawWmCategory':'universal_church_of_the_kingdom_of_god'}, //  Count: 518
        "shinto shrine":{'rawWmCategory':'shinto_shrine'}, //  Count: 518
        "lobby (room)":{'rawWmCategory':'lobby_(room)'}, //  Count: 518
        "retreat center (spiritual)":{'rawWmCategory':'retreat_center_(spiritual)'}, //  Count: 518
        "stormwater":{'rawWmCategory':'stormwater'}, //  Count: 518
        "taxi service":{'rawWmCategory':'taxi_service'}, //  Count: 516
        "Honda":{'rawWmCategory':'Honda'}, //  Count: 516
        "instruments":{'rawWmCategory':'instruments'}, //  Count: 516
        "pontoon bridge":{'rawWmCategory':'pontoon_bridge'}, //  Count: 515
        "sandbox":{'rawWmCategory':'sandbox'}, //  Count: 515
        "1987_construction":{'rawWmCategory':'1987_construction'}, //  Count: 514
        "fen (wetland)":{'rawWmCategory':'fen_(wetland)'}, //  Count: 514
        "студія звукозапису":{'rawWmCategory':'студія_звукозапису'}, //  Count: 513
        "psychotherapy":{'rawWmCategory':'psychotherapy'}, //  Count: 513
        "recording studio":{'rawWmCategory':'recording_studio'}, //  Count: 513
        "architectural organization":{'rawWmCategory':'architectural_organization'}, //  Count: 512
        "automation":{'rawWmCategory':'automation'}, //  Count: 512
        "rubbish / refuse / garbage dump":{'rawWmCategory':'rubbish_/_refuse_/_garbage_dump'}, //  Count: 511
        "sports hall":{'rawWmCategory':'sports_hall'}, //  Count: 510
        "diagnostic center":{'rawWmCategory':'diagnostic_center'}, //  Count: 510
        "diagnostic centre":{'rawWmCategory':'diagnostic_centre'}, //  Count: 510
        "sports bar":{'rawWmCategory':'sports_bar'}, //  Count: 509
        "2011_construction":{'rawWmCategory':'2011_construction'}, //  Count: 508
        "asilo":{'rawWmCategory':'asilo'}, //  Count: 508
        "enclave":{'rawWmCategory':'enclave'}, //  Count: 507
        "chamber of commerce":{'rawWmCategory':'chamber_of_commerce'}, //  Count: 506
        "magistrates' court":{'rawWmCategory':'magistrates_court'}, //  Count: 506
        "labyrinth / maze":{'rawWmCategory':'labyrinth_/_maze'}, //  Count: 506
        "Thai cuisine":{'rawWmCategory':'Thai_cuisine'}, //  Count: 505
        "polygon art":{'rawWmCategory':'polygon_art'}, //  Count: 503
        "2007_construction":{'rawWmCategory':'2007_construction'}, //  Count: 503
        "rowing":{'rawWmCategory':'rowing'}, //  Count: 502
        "event management company":{'rawWmCategory':'event_management_company'}, //  Count: 501
        "event manager":{'rawWmCategory':'event_manager'}, //  Count: 501
        "Greek Revival style architecture":{'rawWmCategory':'Greek_Revival_style_architecture'}, //  Count: 500
        "knitwear":{'rawWmCategory':'knitwear'}, //  Count: 499
        "Kroger":{'rawWmCategory':'Kroger'}, //  Count: 499
        "Purok":{'rawWmCategory':'Purok'}, //  Count: 498
        "Philippine/Filipino cuisine":{'rawWmCategory':'Philippine/Filipino_cuisine'}, //  Count: 498
        "eternal flame":{'rawWmCategory':'eternal_flame'}, //  Count: 497
        "Cold War 1947-1991":{'rawWmCategory':'Cold_War_1947-1991'}, //  Count: 497
        "bathing ghat":{'rawWmCategory':'bathing_ghat'}, //  Count: 495
        "1953_construction":{'rawWmCategory':'1953_construction'}, //  Count: 494
        "refrigeration":{'rawWmCategory':'refrigeration'}, //  Count: 494
        "1984_construction":{'rawWmCategory':'1984_construction'}, //  Count: 494
        "Marriott Hotels and Resorts":{'rawWmCategory':'Marriott_Hotels_and_Resorts'}, //  Count: 493
        "physiotherapist":{'rawWmCategory':'physiotherapist'}, //  Count: 490
        "physical therapy":{'rawWmCategory':'physical_therapy'}, //  Count: 490
        "physiotherapy":{'rawWmCategory':'physiotherapy'}, //  Count: 490
        "electrical contractor / electricians":{'rawWmCategory':'electrical_contractor_/_electricians'}, //  Count: 489
        "military museum":{'rawWmCategory':'military_museum'}, //  Count: 488
        "1956_construction":{'rawWmCategory':'1956_construction'}, //  Count: 488
        "war museum":{'rawWmCategory':'war_museum'}, //  Count: 488
        "park and ride":{'rawWmCategory':'park_and_ride'}, //  Count: 487
        "1988_construction":{'rawWmCategory':'1988_construction'}, //  Count: 487
        "tacos":{'rawWmCategory':'tacos'}, //  Count: 487
        "2010_construction":{'rawWmCategory':'2010_construction'}, //  Count: 482
        "gram panchayat office":{'rawWmCategory':'gram_panchayat_office'}, //  Count: 481
        "boathouse":{'rawWmCategory':'boathouse'}, //  Count: 481
        "collective zone":{'rawWmCategory':'collective_zone'}, //  Count: 480
        "roofing supplies":{'rawWmCategory':'roofing_supplies'}, //  Count: 480
        "cultural heritage / national heritage":{'rawWmCategory':'cultural_heritage_/_national_heritage'}, //  Count: 478
        "2009_construction":{'rawWmCategory':'2009_construction'}, //  Count: 477
        "vegetarian restaurant":{'rawWmCategory':'vegetarian_restaurant'}, //  Count: 477
        "apparel":{'rawWmCategory':'apparel'}, //  Count: 477
        "copper mine":{'rawWmCategory':'copper_mine'}, //  Count: 475
        "1955_construction":{'rawWmCategory':'1955_construction'}, //  Count: 474
        "retransmission":{'rawWmCategory':'retransmission'}, //  Count: 474
        "leading beacon":{'rawWmCategory':'leading_beacon'}, //  Count: 473
        "dry lake":{'rawWmCategory':'dry_lake'}, //  Count: 473
        "Sonic Drive-In":{'rawWmCategory':'Sonic_Drive-In'}, //  Count: 473
        "Dairy Queen":{'rawWmCategory':'Dairy_Queen'}, //  Count: 473
        "embankment":{'rawWmCategory':'embankment'}, //  Count: 471
        "blacksmith's workshop / forge":{'rawWmCategory':'blacksmiths_workshop_/_forge'}, //  Count: 471
        "1985_construction":{'rawWmCategory':'1985_construction'}, //  Count: 471
        "veterinary pharmacy":{'rawWmCategory':'veterinary_pharmacy'}, //  Count: 470
        "16th century construction":{'rawWmCategory':'16th_century_construction'}, //  Count: 469
        "detention centre":{'rawWmCategory':'detention_centre'}, //  Count: 468
        "detention center":{'rawWmCategory':'detention_center'}, //  Count: 468
        "1986_construction":{'rawWmCategory':'1986_construction'}, //  Count: 467
        "narcology":{'rawWmCategory':'narcology'}, //  Count: 467
        "olympic venue":{'rawWmCategory':'olympic_venue'}, //  Count: 467
        "cabin":{'rawWmCategory':'cabin'}, //  Count: 466
        "senior citizen center":{'rawWmCategory':'senior_citizen_center'}, //  Count: 465
        "limestone quarry":{'rawWmCategory':'limestone_quarry'}, //  Count: 464
        "dental laboratory":{'rawWmCategory':'dental_laboratory'}, //  Count: 463
        "handball court":{'rawWmCategory':'handball_court'}, //  Count: 463
        "jewelry repair":{'rawWmCategory':'jewelry_repair'}, //  Count: 459
        "Vietnamese cuisine":{'rawWmCategory':'Vietnamese_cuisine'}, //  Count: 458
        "1954_construction":{'rawWmCategory':'1954_construction'}, //  Count: 458
        "TB dispensary":{'rawWmCategory':'TB_dispensary'}, //  Count: 458
        "Durga temple":{'rawWmCategory':'Durga_temple'}, //  Count: 458
        "distribution point":{'rawWmCategory':'distribution_point'}, //  Count: 458
        "public control":{'rawWmCategory':'public_control'}, //  Count: 458
        "2004_construction":{'rawWmCategory':'2004_construction'}, //  Count: 458
        "Romanesque architecture":{'rawWmCategory':'Romanesque_architecture'}, //  Count: 458
        "gynecology":{'rawWmCategory':'gynecology'}, //  Count: 458
        "gynaecologist":{'rawWmCategory':'gynaecologist'}, //  Count: 458
        "gynaecology":{'rawWmCategory':'gynaecology'}, //  Count: 458
        "balloon loop / turning loop":{'rawWmCategory':'balloon_loop_/_turning_loop'}, //  Count: 457
        "radio engineering":{'rawWmCategory':'radio_engineering'}, //  Count: 457
        "shooting sport":{'rawWmCategory':'shooting_sport'}, //  Count: 457
        "table tennis":{'rawWmCategory':'table_tennis'}, //  Count: 456
        "truck rental":{'rawWmCategory':'truck_rental'}, //  Count: 456
        "ping-pong":{'rawWmCategory':'ping-pong'}, //  Count: 456
        "Chevron":{'rawWmCategory':'Chevron'}, //  Count: 456
        "optometrist":{'rawWmCategory':'optometrist'}, //  Count: 456
        "cotton mill":{'rawWmCategory':'cotton_mill'}, //  Count: 455
        "planetarium":{'rawWmCategory':'planetarium'}, //  Count: 455
        "power equipment":{'rawWmCategory':'power_equipment'}, //  Count: 454
        "tor":{'rawWmCategory':'tor'}, //  Count: 454
        "banco santander":{'rawWmCategory':'banco_santander'}, //  Count: 453
        "firearm store":{'rawWmCategory':'firearm_store'}, //  Count: 453
        "gun shop":{'rawWmCategory':'gun_shop'}, //  Count: 453
        "stone/rock feature":{'rawWmCategory':'stone/rock_feature'}, //  Count: 449
        "1870s construction":{'rawWmCategory':'1870s_construction'}, //  Count: 447
        "marine":{'rawWmCategory':'marine'}, //  Count: 447
        "aluminium / aluminum":{'rawWmCategory':'aluminium_/_aluminum'}, //  Count: 446
        "aluminum":{'rawWmCategory':'aluminum'}, //  Count: 446
        "2006_construction":{'rawWmCategory':'2006_construction'}, //  Count: 445
        "archipelago":{'rawWmCategory':'archipelago'}, //  Count: 444
        "petrochemical industry":{'rawWmCategory':'petrochemical_industry'}, //  Count: 443
        "feed-stuff industry":{'rawWmCategory':'feed-stuff_industry'}, //  Count: 442
        "chalet":{'rawWmCategory':'chalet'}, //  Count: 442
        "1989_construction":{'rawWmCategory':'1989_construction'}, //  Count: 442
        "1952_construction":{'rawWmCategory':'1952_construction'}, //  Count: 441
        "rail-trail":{'rawWmCategory':'rail-trail'}, //  Count: 440
        "heating and air conditioning services":{'rawWmCategory':'heating_and_air_conditioning_services'}, //  Count: 439
        "pollution":{'rawWmCategory':'pollution'}, //  Count: 437
        "smelter":{'rawWmCategory':'smelter'}, //  Count: 436
        "funicular / cablecar station (rail transport on the ground)":{'rawWmCategory':'funicular_/_cablecar_station_(rail_transport_on_the_ground)'}, //  Count: 436
        "moshav":{'rawWmCategory':'moshav'}, //  Count: 435
        "financial planner":{'rawWmCategory':'financial_planner'}, //  Count: 434
        "ramp":{'rawWmCategory':'ramp'}, //  Count: 434
        "financial advisor":{'rawWmCategory':'financial_advisor'}, //  Count: 434
        "training aids":{'rawWmCategory':'training_aids'}, //  Count: 432
        "obstacle course":{'rawWmCategory':'obstacle_course'}, //  Count: 431
        "atomic power station":{'rawWmCategory':'atomic_power_station'}, //  Count: 431
        "diesel generator":{'rawWmCategory':'diesel_generator'}, //  Count: 431
        "nuclear power plant":{'rawWmCategory':'nuclear_power_plant'}, //  Count: 431
        "foundry":{'rawWmCategory':'foundry'}, //  Count: 431
        "payday loan":{'rawWmCategory':'payday_loan'}, //  Count: 430
        "arboretum":{'rawWmCategory':'arboretum'}, //  Count: 430
        "2005_construction":{'rawWmCategory':'2005_construction'}, //  Count: 429
        "desert":{'rawWmCategory':'desert'}, //  Count: 429
        "boxing":{'rawWmCategory':'boxing'}, //  Count: 428
        "water slide":{'rawWmCategory':'water_slide'}, //  Count: 428
        "interior design":{'rawWmCategory':'interior_design'}, //  Count: 428
        "caravan club certified location":{'rawWmCategory':'caravan_club_certified_location'}, //  Count: 426
        "disaster management":{'rawWmCategory':'disaster_management'}, //  Count: 426
        "emergency management":{'rawWmCategory':'emergency_management'}, //  Count: 426
        "Federal style architecture":{'rawWmCategory':'Federal_style_architecture'}, //  Count: 426
        "archaeology museum":{'rawWmCategory':'archaeology_museum'}, //  Count: 425
        "Petron Corporation":{'rawWmCategory':'Petron_Corporation'}, //  Count: 422
        "driver and vehicle licensing office":{'rawWmCategory':'driver_and_vehicle_licensing_office'}, //  Count: 421
        "drivers license service":{'rawWmCategory':'drivers_license_service'}, //  Count: 421
        "Middle Eastern cuisine":{'rawWmCategory':'Middle_Eastern_cuisine'}, //  Count: 421
        "surf":{'rawWmCategory':'surf'}, //  Count: 421
        "plumbing":{'rawWmCategory':'plumbing'}, //  Count: 421
        "15th century construction":{'rawWmCategory':'15th_century_construction'}, //  Count: 420
        "decor":{'rawWmCategory':'decor'}, //  Count: 420
        "Quick_(restaurant)":{'rawWmCategory':'Quick_(restaurant)'}, //  Count: 419
        "madeireira":{'rawWmCategory':'madeireira'}, //  Count: 419
        "RAF - Royal Air Force":{'rawWmCategory':'RAF_-_Royal_Air_Force'}, //  Count: 419
        "graffiti":{'rawWmCategory':'graffiti'}, //  Count: 419
        "fish processing facility/factory":{'rawWmCategory':'fish_processing_facility/factory'}, //  Count: 418
        "seafood processing facility/factory":{'rawWmCategory':'seafood_processing_facility/factory'}, //  Count: 418
        "1993_construction":{'rawWmCategory':'1993_construction'}, //  Count: 418
        "free of charge":{'rawWmCategory':'free_of_charge'}, //  Count: 417
        "calvinist":{'rawWmCategory':'calvinist'}, //  Count: 417
        "gymnastic hall":{'rawWmCategory':'gymnastic_hall'}, //  Count: 416
        "gymnastic center":{'rawWmCategory':'gymnastic_center'}, //  Count: 416
        "gymnastic centre":{'rawWmCategory':'gymnastic_centre'}, //  Count: 416
        "estates":{'rawWmCategory':'estates'}, //  Count: 416
        "gymnastics":{'rawWmCategory':'gymnastics'}, //  Count: 416
        "ventilation":{'rawWmCategory':'ventilation'}, //  Count: 415
        "smoking area":{'rawWmCategory':'smoking_area'}, //  Count: 414
        "telescope":{'rawWmCategory':'telescope'}, //  Count: 414
        "FSB of Russia":{'rawWmCategory':'FSB_of_Russia'}, //  Count: 413
        "junior college":{'rawWmCategory':'junior_college'}, //  Count: 412
        "information technology company  / industry - IT":{'rawWmCategory':'information_technology_company__/_industry_-_IT'}, //  Count: 412
        "soft drink bottling plant":{'rawWmCategory':'soft_drink_bottling_plant'}, //  Count: 411
        "meditation":{'rawWmCategory':'meditation'}, //  Count: 411
        "soft drink production":{'rawWmCategory':'soft_drink_production'}, //  Count: 411
        "cakery":{'rawWmCategory':'cakery'}, //  Count: 410
        "cake shop":{'rawWmCategory':'cake_shop'}, //  Count: 410
        "Korean cuisine":{'rawWmCategory':'Korean_cuisine'}, //  Count: 408
        "humanitarian":{'rawWmCategory':'humanitarian'}, //  Count: 408
        "promenade":{'rawWmCategory':'promenade'}, //  Count: 407
        "monolith":{'rawWmCategory':'monolith'}, //  Count: 406
        "landscape":{'rawWmCategory':'landscape'}, //  Count: 405
        "2002_construction":{'rawWmCategory':'2002_construction'}, //  Count: 405
        "packaging company":{'rawWmCategory':'packaging_company'}, //  Count: 404
        "building contractor":{'rawWmCategory':'building_contractor'}, //  Count: 404
        "sound stage":{'rawWmCategory':'sound_stage'}, //  Count: 404
        "1990_construction":{'rawWmCategory':'1990_construction'}, //  Count: 404
        "1999_construction":{'rawWmCategory':'1999_construction'}, //  Count: 403
        "1991_construction":{'rawWmCategory':'1991_construction'}, //  Count: 403
        "Mannerist architecture":{'rawWmCategory':'Mannerist_architecture'}, //  Count: 402
        "flood protection":{'rawWmCategory':'flood_protection'}, //  Count: 402
        "Renaissance architecture":{'rawWmCategory':'Renaissance_architecture'}, //  Count: 402
        "ambassador's residence":{'rawWmCategory':'ambassadors_residence'}, //  Count: 400
        "Waffle House":{'rawWmCategory':'Waffle_House'}, //  Count: 400
        "flag":{'rawWmCategory':'flag'}, //  Count: 400
        "dome":{'rawWmCategory':'dome'}, //  Count: 400
        "murder site":{'rawWmCategory':'murder_site'}, //  Count: 399
        "speedway rack track":{'rawWmCategory':'speedway_rack_track'}, //  Count: 399
        "pharmaceutical industry / company":{'rawWmCategory':'pharmaceutical_industry_/_company'}, //  Count: 398
        "glen":{'rawWmCategory':'glen'}, //  Count: 398
        "circus":{'rawWmCategory':'circus'}, //  Count: 398
        "ferris wheel":{'rawWmCategory':'ferris_wheel'}, //  Count: 397
        "fish and chips shop":{'rawWmCategory':'fish_and_chips_shop'}, //  Count: 397
        "TACAN - tactical air navigation system":{'rawWmCategory':'TACAN_-_tactical_air_navigation_system'}, //  Count: 397
        "orthopaedic device / brace supplier":{'rawWmCategory':'orthopaedic_device_/_brace_supplier'}, //  Count: 396
        "Native American reservation/reserve":{'rawWmCategory':'Native_American_reservation/reserve'}, //  Count: 396
        "orthopedic device / brace supplier":{'rawWmCategory':'orthopedic_device_/_brace_supplier'}, //  Count: 396
        "seeds":{'rawWmCategory':'seeds'}, //  Count: 396
        "Industrial Training Institute / ITI (India)":{'rawWmCategory':'Industrial_Training_Institute_/_ITI_(India)'}, //  Count: 396
        "fuel industry":{'rawWmCategory':'fuel_industry'}, //  Count: 395
        "1992_construction":{'rawWmCategory':'1992_construction'}, //  Count: 395
        "1998_construction":{'rawWmCategory':'1998_construction'}, //  Count: 395
        "private hospital":{'rawWmCategory':'private_hospital'}, //  Count: 394
        "historical marker":{'rawWmCategory':'historical_marker'}, //  Count: 394
        "car painting":{'rawWmCategory':'car_painting'}, //  Count: 394
        "street food / drink vendor":{'rawWmCategory':'street_food_/_drink_vendor'}, //  Count: 393
        "mobile food / drink cart":{'rawWmCategory':'mobile_food_/_drink_cart'}, //  Count: 393
        "usareur":{'rawWmCategory':'usareur'}, //  Count: 393
        "nikemissile":{'rawWmCategory':'nikemissile'}, //  Count: 393
        "United States Army in Europe":{'rawWmCategory':'United_States_Army_in_Europe'}, //  Count: 393
        "towing service":{'rawWmCategory':'towing_service'}, //  Count: 392
        "microbrewery":{'rawWmCategory':'microbrewery'}, //  Count: 391
        "Kmart":{'rawWmCategory':'Kmart'}, //  Count: 391
        "department of motor vehicles (DMV)":{'rawWmCategory':'department_of_motor_vehicles_(DMV)'}, //  Count: 390
        "business process outsourcing (BPO)":{'rawWmCategory':'business_process_outsourcing_(BPO)'}, //  Count: 389
        "juvenile detention center (JDC)":{'rawWmCategory':'juvenile_detention_center_(JDC)'}, //  Count: 389
        "youth detention center":{'rawWmCategory':'youth_detention_center'}, //  Count: 389
        "2003_construction":{'rawWmCategory':'2003_construction'}, //  Count: 389
        "lumber yard":{'rawWmCategory':'lumber_yard'}, //  Count: 388
        "timber yard":{'rawWmCategory':'timber_yard'}, //  Count: 388
        "timber merchant":{'rawWmCategory':'timber_merchant'}, //  Count: 388
        "translator":{'rawWmCategory':'translator'}, //  Count: 387
        "translation company":{'rawWmCategory':'translation_company'}, //  Count: 387
        "commemorative place/plaque":{'rawWmCategory':'commemorative_place/plaque'}, //  Count: 386
        "animal breeding":{'rawWmCategory':'animal_breeding'}, //  Count: 384
        "government degree college":{'rawWmCategory':'government_degree_college'}, //  Count: 384
        "badminton":{'rawWmCategory':'badminton'}, //  Count: 383
        "academy - English school (UK)":{'rawWmCategory':'academy_-_English_school_(UK)'}, //  Count: 383
        "State Bank of India (SBI)":{'rawWmCategory':'State_Bank_of_India_(SBI)'}, //  Count: 382
        "glazier":{'rawWmCategory':'glazier'}, //  Count: 382
        "2001_construction":{'rawWmCategory':'2001_construction'}, //  Count: 381
        "2000_construction":{'rawWmCategory':'2000_construction'}, //  Count: 381
        "gewerbe":{'rawWmCategory':'gewerbe'}, //  Count: 381
        "flooring":{'rawWmCategory':'flooring'}, //  Count: 380
        "hockey field":{'rawWmCategory':'hockey_field'}, //  Count: 380
        "harvard":{'rawWmCategory':'harvard'}, //  Count: 380
        "regulatory authority":{'rawWmCategory':'regulatory_authority'}, //  Count: 379
        "regulator":{'rawWmCategory':'regulator'}, //  Count: 379
        "regulatory agency":{'rawWmCategory':'regulatory_agency'}, //  Count: 379
        "bishopric":{'rawWmCategory':'bishopric'}, //  Count: 379
        "eparchy":{'rawWmCategory':'eparchy'}, //  Count: 379
        "diocese":{'rawWmCategory':'diocese'}, //  Count: 379
        "bishop":{'rawWmCategory':'bishop'}, //  Count: 379
        "american football field":{'rawWmCategory':'american_football_field'}, //  Count: 378
        "target (military)":{'rawWmCategory':'target_(military)'}, //  Count: 378
        "1951_construction":{'rawWmCategory':'1951_construction'}, //  Count: 378
        "dishware":{'rawWmCategory':'dishware'}, //  Count: 377
        "flagpole":{'rawWmCategory':'flagpole'}, //  Count: 377
        "wagon":{'rawWmCategory':'wagon'}, //  Count: 376
        "depression (geology)":{'rawWmCategory':'depression_(geology)'}, //  Count: 376
        "electrical substation building":{'rawWmCategory':'electrical_substation_building'}, //  Count: 375
        "logging":{'rawWmCategory':'logging'}, //  Count: 374
        "conservatory (music)":{'rawWmCategory':'conservatory_(music)'}, //  Count: 374
        "coast":{'rawWmCategory':'coast'}, //  Count: 373
        "mangal karyalay":{'rawWmCategory':'mangal_karyalay'}, //  Count: 373
        "tractors (dealer / manufacturing)":{'rawWmCategory':'tractors_(dealer_/_manufacturing)'}, //  Count: 373
        "kalyana mandapam":{'rawWmCategory':'kalyana_mandapam'}, //  Count: 373
        "small borough":{'rawWmCategory':'small_borough'}, //  Count: 373
        "stone working":{'rawWmCategory':'stone_working'}, //  Count: 372
        "cannery":{'rawWmCategory':'cannery'}, //  Count: 372
        "college of agriculture":{'rawWmCategory':'college_of_agriculture'}, //  Count: 371
        "auctioneer":{'rawWmCategory':'auctioneer'}, //  Count: 371
        "sewing and needlework goods shop":{'rawWmCategory':'sewing_and_needlework_goods_shop'}, //  Count: 371
        "department of agriculture":{'rawWmCategory':'department_of_agriculture'}, //  Count: 371
        "buoy":{'rawWmCategory':'buoy'}, //  Count: 371
        "granite":{'rawWmCategory':'granite'}, //  Count: 371
        "auction house":{'rawWmCategory':'auction_house'}, //  Count: 371
        "shrubland":{'rawWmCategory':'shrubland'}, //  Count: 370
        "heathland":{'rawWmCategory':'heathland'}, //  Count: 370
        "scrubland":{'rawWmCategory':'scrubland'}, //  Count: 370
        "moorland":{'rawWmCategory':'moorland'}, //  Count: 370
        "uad / wad / wadi":{'rawWmCategory':'uad_/_wad_/_wadi'}, //  Count: 370
        "object of cultural heritage of federal importance (Russia)":{'rawWmCategory':'object_of_cultural_heritage_of_federal_importance_(Russia)'}, //  Count: 370
        "Montessori education":{'rawWmCategory':'Montessori_education'}, //  Count: 370
        "heath":{'rawWmCategory':'heath'}, //  Count: 370
        "roller skating rink":{'rawWmCategory':'roller_skating_rink'}, //  Count: 369
        "college / department of computer science":{'rawWmCategory':'college_/_department_of_computer_science'}, //  Count: 369
        "inline skating ring":{'rawWmCategory':'inline_skating_ring'}, //  Count: 369
        "Chick-fil-A (restaurant)":{'rawWmCategory':'Chick-fil-A_(restaurant)'}, //  Count: 369
        "1950_construction":{'rawWmCategory':'1950_construction'}, //  Count: 369
        "IHOP":{'rawWmCategory':'IHOP'}, //  Count: 369
        "sluice":{'rawWmCategory':'sluice'}, //  Count: 369
        "tram depot":{'rawWmCategory':'tram_depot'}, //  Count: 368
        "dolmen":{'rawWmCategory':'dolmen'}, //  Count: 368
        "shashlik-house":{'rawWmCategory':'shashlik-house'}, //  Count: 367
        "beauty supply":{'rawWmCategory':'beauty_supply'}, //  Count: 366
        "base camp (mountaineering)":{'rawWmCategory':'base_camp_(mountaineering)'}, //  Count: 365
        "revealed object of cultural heritage (Russia)":{'rawWmCategory':'revealed_object_of_cultural_heritage_(Russia)'}, //  Count: 365
        "basecamp":{'rawWmCategory':'basecamp'}, //  Count: 365
        "pharmacy school":{'rawWmCategory':'pharmacy_school'}, //  Count: 364
        "outcrop (rocks)":{'rawWmCategory':'outcrop_(rocks)'}, //  Count: 364
        "college / department of pharmacy":{'rawWmCategory':'college_/_department_of_pharmacy'}, //  Count: 364
        "1996_construction":{'rawWmCategory':'1996_construction'}, //  Count: 363
        "Ukrainian Orthodox Church – Kyiv Patriarchate (UOC-KP)":{'rawWmCategory':'Ukrainian_Orthodox_Church_–_Kyiv_Patriarchate_(UOC-KP)'}, //  Count: 362
        "indoor":{'rawWmCategory':'indoor'}, //  Count: 362
        "Red Cross / Red Crescent":{'rawWmCategory':'Red_Cross_/_Red_Crescent'}, //  Count: 361
        "American cuisine":{'rawWmCategory':'American_cuisine'}, //  Count: 361
        "FedEx":{'rawWmCategory':'FedEx'}, //  Count: 361
        "juice bar / shop":{'rawWmCategory':'juice_bar_/_shop'}, //  Count: 361
        "1997_construction":{'rawWmCategory':'1997_construction'}, //  Count: 360
        "Auto Zone":{'rawWmCategory':'Auto_Zone'}, //  Count: 360
        "AutoZone":{'rawWmCategory':'AutoZone'}, //  Count: 360
        "gun":{'rawWmCategory':'gun'}, //  Count: 360
        "aldeia indigena":{'rawWmCategory':'aldeia_indigena'}, //  Count: 359
        "modernism":{'rawWmCategory':'modernism'}, //  Count: 359
        "border outpost":{'rawWmCategory':'border_outpost'}, //  Count: 358
        "canal junction":{'rawWmCategory':'canal_junction'}, //  Count: 358
        "Publix (supermarket)":{'rawWmCategory':'Publix_(supermarket)'}, //  Count: 358
        "barge":{'rawWmCategory':'barge'}, //  Count: 357
        "statistics":{'rawWmCategory':'statistics'}, //  Count: 356
        "1995_construction":{'rawWmCategory':'1995_construction'}, //  Count: 355
        "1994 construction":{'rawWmCategory':'1994_construction'}, //  Count: 355
        "convention":{'rawWmCategory':'convention'}, //  Count: 355
        "decathlon":{'rawWmCategory':'decathlon'}, //  Count: 354
        "tile works":{'rawWmCategory':'tile_works'}, //  Count: 353
        "tile factory":{'rawWmCategory':'tile_factory'}, //  Count: 353
        "canoe":{'rawWmCategory':'canoe'}, //  Count: 352
        "polder":{'rawWmCategory':'polder'}, //  Count: 352
        "weapons":{'rawWmCategory':'weapons'}, //  Count: 352
        "cellphone accessories store / shop":{'rawWmCategory':'cellphone_accessories_store_/_shop'}, //  Count: 351
        "ophthalmic hospital":{'rawWmCategory':'ophthalmic_hospital'}, //  Count: 351
        "mobile phone accessories store / shop":{'rawWmCategory':'mobile_phone_accessories_store_/_shop'}, //  Count: 351
        "exporter":{'rawWmCategory':'exporter'}, //  Count: 351
        "eye hospital":{'rawWmCategory':'eye_hospital'}, //  Count: 351
        "diplomatic / foreign mission / representation":{'rawWmCategory':'diplomatic_/_foreign_mission_/_representation'}, //  Count: 351
        "cardiac hospital / cardiology clinic":{'rawWmCategory':'cardiac_hospital_/_cardiology_clinic'}, //  Count: 350
        "breeding kennel":{'rawWmCategory':'breeding_kennel'}, //  Count: 350
        "water refilling station":{'rawWmCategory':'water_refilling_station'}, //  Count: 350
        "autobody":{'rawWmCategory':'autobody'}, //  Count: 349
        "cadet":{'rawWmCategory':'cadet'}, //  Count: 349
        "bsnl":{'rawWmCategory':'bsnl'}, //  Count: 348
        "corner":{'rawWmCategory':'corner'}, //  Count: 348
        "municipal engineering":{'rawWmCategory':'municipal_engineering'}, //  Count: 346
        "artwork":{'rawWmCategory':'artwork'}, //  Count: 346
        "drydock":{'rawWmCategory':'drydock'}, //  Count: 345
        "cattle burial ground":{'rawWmCategory':'cattle_burial_ground'}, //  Count: 345
        "dry dock":{'rawWmCategory':'dry_dock'}, //  Count: 345
        "kund (pond)":{'rawWmCategory':'kund_(pond)'}, //  Count: 344
        "temple tank / pond":{'rawWmCategory':'temple_tank_/_pond'}, //  Count: 344
        "horse riding centre":{'rawWmCategory':'horse_riding_centre'}, //  Count: 344
        "merry-go-round (ride)":{'rawWmCategory':'merry-go-round_(ride)'}, //  Count: 344
        "Applebee&#039;s":{'rawWmCategory':'Applebee&#039;s'}, //  Count: 344
        "horse riding academy / horse riding school":{'rawWmCategory':'horse_riding_academy_/_horse_riding_school'}, //  Count: 344
        "carousel":{'rawWmCategory':'carousel'}, //  Count: 344
        "anganwadi":{'rawWmCategory':'anganwadi'}, //  Count: 344
        "shows":{'rawWmCategory':'shows'}, //  Count: 343
        "road traffic channelization":{'rawWmCategory':'road_traffic_channelization'}, //  Count: 343
        "contabilidade":{'rawWmCategory':'contabilidade'}, //  Count: 343
        "United States Army":{'rawWmCategory':'United_States_Army'}, //  Count: 343
        "railway roundhouse":{'rawWmCategory':'railway_roundhouse'}, //  Count: 343
        "tea estate":{'rawWmCategory':'tea_estate'}, //  Count: 342
        "maritime museum":{'rawWmCategory':'maritime_museum'}, //  Count: 342
        "naval museum":{'rawWmCategory':'naval_museum'}, //  Count: 342
        "bike station":{'rawWmCategory':'bike_station'}, //  Count: 342
        "tea plantation":{'rawWmCategory':'tea_plantation'}, //  Count: 342
        "paddling pool":{'rawWmCategory':'paddling_pool'}, //  Count: 341
        "wading pool":{'rawWmCategory':'wading_pool'}, //  Count: 341
        "dhaba":{'rawWmCategory':'dhaba'}, //  Count: 340
        "teacher training college":{'rawWmCategory':'teacher_training_college'}, //  Count: 339
        "auto parts supplier / manufacturer":{'rawWmCategory':'auto_parts_supplier_/_manufacturer'}, //  Count: 339
        "Jollibee":{'rawWmCategory':'Jollibee'}, //  Count: 339
        "sunday school":{'rawWmCategory':'sunday_school'}, //  Count: 339
        "blood transfusion center":{'rawWmCategory':'blood_transfusion_center'}, //  Count: 338
        "Domino's Pizza":{'rawWmCategory':'Dominos_Pizza'}, //  Count: 338
        "opera house":{'rawWmCategory':'opera_house'}, //  Count: 337
        "opera":{'rawWmCategory':'opera'}, //  Count: 337
        "operette":{'rawWmCategory':'operette'}, //  Count: 337
        "sex shop":{'rawWmCategory':'sex_shop'}, //  Count: 336
        "vihara / boudh vihar":{'rawWmCategory':'vihara_/_boudh_vihar'}, //  Count: 336
        "software park":{'rawWmCategory':'software_park'}, //  Count: 336
        "adult shop / store":{'rawWmCategory':'adult_shop_/_store'}, //  Count: 336
        "erotic shop":{'rawWmCategory':'erotic_shop'}, //  Count: 336
        "technopark":{'rawWmCategory':'technopark'}, //  Count: 336
        "science park":{'rawWmCategory':'science_park'}, //  Count: 336
        "buddhist monastery":{'rawWmCategory':'buddhist_monastery'}, //  Count: 336
        "destroyer (ship)":{'rawWmCategory':'destroyer_(ship)'}, //  Count: 334
        "survey":{'rawWmCategory':'survey'}, //  Count: 334
        "localizer antenna":{'rawWmCategory':'localizer_antenna'}, //  Count: 334
        "cheese processing plant":{'rawWmCategory':'cheese_processing_plant'}, //  Count: 333
        "fishing port":{'rawWmCategory':'fishing_port'}, //  Count: 333
        "oncology":{'rawWmCategory':'oncology'}, //  Count: 333
        "pancakes":{'rawWmCategory':'pancakes'}, //  Count: 333
        "American Legion":{'rawWmCategory':'American_Legion'}, //  Count: 332
        "railway museum":{'rawWmCategory':'railway_museum'}, //  Count: 331
        "infant clothing store":{'rawWmCategory':'infant_clothing_store'}, //  Count: 331
        "railroad museum":{'rawWmCategory':'railroad_museum'}, //  Count: 331
        "Irish pub":{'rawWmCategory':'Irish_pub'}, //  Count: 331
        "prefecture":{'rawWmCategory':'prefecture'}, //  Count: 331
        "winding hole":{'rawWmCategory':'winding_hole'}, //  Count: 330
        "ham radio":{'rawWmCategory':'ham_radio'}, //  Count: 330
        "espiritismo":{'rawWmCategory':'espiritismo'}, //  Count: 330
        "canal basin":{'rawWmCategory':'canal_basin'}, //  Count: 330
        "Victorian style architecture":{'rawWmCategory':'Victorian_style_architecture'}, //  Count: 330
        "jeans store":{'rawWmCategory':'jeans_store'}, //  Count: 329
        "certification authority":{'rawWmCategory':'certification_authority'}, //  Count: 329
        "Ethiopian Orthodox Tewahedo Church":{'rawWmCategory':'Ethiopian_Orthodox_Tewahedo_Church'}, //  Count: 329
        "Tesco":{'rawWmCategory':'Tesco'}, //  Count: 329
        "inland port":{'rawWmCategory':'inland_port'}, //  Count: 328
        "Greek cuisine":{'rawWmCategory':'Greek_cuisine'}, //  Count: 328
        "hatchery":{'rawWmCategory':'hatchery'}, //  Count: 328
        "thevara paadal petra sthalam":{'rawWmCategory':'thevara_paadal_petra_sthalam'}, //  Count: 327
        "thaevaara sthalam":{'rawWmCategory':'thaevaara_sthalam'}, //  Count: 327
        "ayurvedic medicine":{'rawWmCategory':'ayurvedic_medicine'}, //  Count: 327
        "Denny&#039;s":{'rawWmCategory':'Denny&#039;s'}, //  Count: 327
        "airsoft":{'rawWmCategory':'airsoft'}, //  Count: 327
        "ravine":{'rawWmCategory':'ravine'}, //  Count: 327
        "Rite Aid":{'rawWmCategory':'Rite_Aid'}, //  Count: 327
        "clay":{'rawWmCategory':'clay'}, //  Count: 325
        "slug":{'rawWmCategory':'slug'}, //  Count: 325
        "radiation":{'rawWmCategory':'radiation'}, //  Count: 325
        "visitor centre":{'rawWmCategory':'visitor_centre'}, //  Count: 324
        "riverport":{'rawWmCategory':'riverport'}, //  Count: 324
        "visitor center":{'rawWmCategory':'visitor_center'}, //  Count: 324
        "lightning protection mast":{'rawWmCategory':'lightning_protection_mast'}, //  Count: 323
        "headframe":{'rawWmCategory':'headframe'}, //  Count: 322
        "historic house":{'rawWmCategory':'historic_house'}, //  Count: 322
        "designated area for campfire / bonfire":{'rawWmCategory':'designated_area_for_campfire_/_bonfire'}, //  Count: 322
        "bmx track":{'rawWmCategory':'bmx_track'}, //  Count: 321
        "petting farm":{'rawWmCategory':'petting_farm'}, //  Count: 321
        "bicycle motocross track":{'rawWmCategory':'bicycle_motocross_track'}, //  Count: 321
        "AABB":{'rawWmCategory':'AABB'}, //  Count: 321
        "children's farm":{'rawWmCategory':'childrens_farm'}, //  Count: 321
        "petting zoo":{'rawWmCategory':'petting_zoo'}, //  Count: 321
        "children's zoo":{'rawWmCategory':'childrens_zoo'}, //  Count: 321
        "woods (forest)":{'rawWmCategory':'woods_(forest)'}, //  Count: 320
        "webhosting":{'rawWmCategory':'webhosting'}, //  Count: 320
        "Volkswagen car dealer":{'rawWmCategory':'Volkswagen_car_dealer'}, //  Count: 319
        "adit":{'rawWmCategory':'adit'}, //  Count: 318
        "osiedle":{'rawWmCategory':'osiedle'}, //  Count: 318
        "shamshan ghat / burning ghat":{'rawWmCategory':'shamshan_ghat_/_burning_ghat'}, //  Count: 316
        "Hardee&#039;s":{'rawWmCategory':'Hardee&#039;s'}, //  Count: 316
        "samsan bhumi / ghat":{'rawWmCategory':'samsan_bhumi_/_ghat'}, //  Count: 316
        "tile stockists store":{'rawWmCategory':'tile_stockists_store'}, //  Count: 315
        "UPS (United Parcel Service)":{'rawWmCategory':'UPS_(United_Parcel_Service)'}, //  Count: 315
        "HSBC":{'rawWmCategory':'HSBC'}, //  Count: 314
        "townland":{'rawWmCategory':'townland'}, //  Count: 314
        "marble quarry":{'rawWmCategory':'marble_quarry'}, //  Count: 314
        "water bottling plant":{'rawWmCategory':'water_bottling_plant'}, //  Count: 313
        "bottled water":{'rawWmCategory':'bottled_water'}, //  Count: 313
        "2015_construction":{'rawWmCategory':'2015_construction'}, //  Count: 313
        "Auchan":{'rawWmCategory':'Auchan'}, //  Count: 313
        "gasometer / gas holder":{'rawWmCategory':'gasometer_/_gas_holder'}, //  Count: 313
        "salt pan / flat":{'rawWmCategory':'salt_pan_/_flat'}, //  Count: 313
        "intelligence":{'rawWmCategory':'intelligence'}, //  Count: 312
        "mangrove":{'rawWmCategory':'mangrove'}, //  Count: 311
        "court bailiff":{'rawWmCategory':'court_bailiff'}, //  Count: 310
        "honours board (USSR)":{'rawWmCategory':'honours_board_(USSR)'}, //  Count: 310
        "dermatovenerologic dispensary":{'rawWmCategory':'dermatovenerologic_dispensary'}, //  Count: 309
        "kayak":{'rawWmCategory':'kayak'}, //  Count: 309
        "anchorage":{'rawWmCategory':'anchorage'}, //  Count: 309
        "changing room":{'rawWmCategory':'changing_room'}, //  Count: 308
        "carabinieri":{'rawWmCategory':'carabinieri'}, //  Count: 308
        "Church's Chicken":{'rawWmCategory':'Churchs_Chicken'}, //  Count: 307
        "zone d'essai nucléaire":{'rawWmCategory':'zone_dessai_nucléaire'}, //  Count: 307
        "ski jumping hill":{'rawWmCategory':'ski_jumping_hill'}, //  Count: 307
        "nuclear testing area":{'rawWmCategory':'nuclear_testing_area'}, //  Count: 307
        "bandstand":{'rawWmCategory':'bandstand'}, //  Count: 307
        "body shop":{'rawWmCategory':'body_shop'}, //  Count: 306
        "editorial office":{'rawWmCategory':'editorial_office'}, //  Count: 306
        "politics":{'rawWmCategory':'politics'}, //  Count: 306
        "camperstop":{'rawWmCategory':'camperstop'}, //  Count: 306
        "byzantine":{'rawWmCategory':'byzantine'}, //  Count: 305
        "IKEA":{'rawWmCategory':'IKEA'}, //  Count: 304
        "Colonial Revival architecture":{'rawWmCategory':'Colonial_Revival_architecture'}, //  Count: 304
        "Kiabi":{'rawWmCategory':'Kiabi'}, //  Count: 304
        "Veterans of Foreign Wars":{'rawWmCategory':'Veterans_of_Foreign_Wars'}, //  Count: 303
        "overlook":{'rawWmCategory':'overlook'}, //  Count: 303
        "oaks":{'rawWmCategory':'oaks'}, //  Count: 303
        "sears":{'rawWmCategory':'sears'}, //  Count: 303
        "Army Air Field US":{'rawWmCategory':'Army_Air_Field_US'}, //  Count: 302
        "hilltop":{'rawWmCategory':'hilltop'}, //  Count: 302
        "surveillance":{'rawWmCategory':'surveillance'}, //  Count: 302
        "vacation rental":{'rawWmCategory':'vacation_rental'}, //  Count: 300
        "healthcare product shop / industry":{'rawWmCategory':'healthcare_product_shop_/_industry'}, //  Count: 300
        "1860s construction":{'rawWmCategory':'1860s_construction'}, //  Count: 299
        "sundial":{'rawWmCategory':'sundial'}, //  Count: 299
        "homœopathy":{'rawWmCategory':'homœopathy'}, //  Count: 298
        "Ram temple":{'rawWmCategory':'Ram_temple'}, //  Count: 298
        "Ram mandir":{'rawWmCategory':'Ram_mandir'}, //  Count: 298
        "roundabout / junction island":{'rawWmCategory':'roundabout_/_junction_island'}, //  Count: 298
        "bed-linen":{'rawWmCategory':'bed-linen'}, //  Count: 298
        "brewpub":{'rawWmCategory':'brewpub'}, //  Count: 298
        "Ramalayam":{'rawWmCategory':'Ramalayam'}, //  Count: 298
        "homeopathy":{'rawWmCategory':'homeopathy'}, //  Count: 298
        "homoeopathy":{'rawWmCategory':'homoeopathy'}, //  Count: 298
        "federal government":{'rawWmCategory':'federal_government'}, //  Count: 296
        "scaled models (vehicles)":{'rawWmCategory':'scaled_models_(vehicles)'}, //  Count: 296
        "karate":{'rawWmCategory':'karate'}, //  Count: 296
        "Safeway (supermarket)":{'rawWmCategory':'Safeway_(supermarket)'}, //  Count: 296
        "hosting":{'rawWmCategory':'hosting'}, //  Count: 295
        "chess":{'rawWmCategory':'chess'}, //  Count: 294
        "excavation":{'rawWmCategory':'excavation'}, //  Count: 294
        "celtic":{'rawWmCategory':'celtic'}, //  Count: 293
        "The Salvation Army":{'rawWmCategory':'The_Salvation_Army'}, //  Count: 293
        "impound lot":{'rawWmCategory':'impound_lot'}, //  Count: 292
        "car pound":{'rawWmCategory':'car_pound'}, //  Count: 292
        "1912_construction":{'rawWmCategory':'1912_construction'}, //  Count: 292
        "bagels":{'rawWmCategory':'bagels'}, //  Count: 292
        "atoll":{'rawWmCategory':'atoll'}, //  Count: 291
        "ryokan":{'rawWmCategory':'ryokan'}, //  Count: 291
        "hospice":{'rawWmCategory':'hospice'}, //  Count: 291
        "explosives":{'rawWmCategory':'explosives'}, //  Count: 291
        "gas flare":{'rawWmCategory':'gas_flare'}, //  Count: 290
        "1917_construction":{'rawWmCategory':'1917_construction'}, //  Count: 290
        "flare stack":{'rawWmCategory':'flare_stack'}, //  Count: 290
        "comic book store":{'rawWmCategory':'comic_book_store'}, //  Count: 290
        "assisted living":{'rawWmCategory':'assisted_living'}, //  Count: 290
        "religious items":{'rawWmCategory':'religious_items'}, //  Count: 289
        "kitchenware":{'rawWmCategory':'kitchenware'}, //  Count: 289
        "pet grooming":{'rawWmCategory':'pet_grooming'}, //  Count: 289
        "mountain monogram":{'rawWmCategory':'mountain_monogram'}, //  Count: 289
        "kitchen accessories":{'rawWmCategory':'kitchen_accessories'}, //  Count: 289
        "oasis":{'rawWmCategory':'oasis'}, //  Count: 289
        "elderlycare":{'rawWmCategory':'elderlycare'}, //  Count: 289
        "hillside letter":{'rawWmCategory':'hillside_letter'}, //  Count: 289
        "minerals":{'rawWmCategory':'minerals'}, //  Count: 289
        "children's centre":{'rawWmCategory':'childrens_centre'}, //  Count: 288
        "landing field":{'rawWmCategory':'landing_field'}, //  Count: 288
        "Raiffeisen Bank":{'rawWmCategory':'Raiffeisen_Bank'}, //  Count: 288
        "physics":{'rawWmCategory':'physics'}, //  Count: 288
        "brothel":{'rawWmCategory':'brothel'}, //  Count: 288
        "1938_construction":{'rawWmCategory':'1938_construction'}, //  Count: 287
        "Zara":{'rawWmCategory':'Zara'}, //  Count: 287
        "trekking":{'rawWmCategory':'trekking'}, //  Count: 286
        "citadel":{'rawWmCategory':'citadel'}, //  Count: 286
        "Holocaust":{'rawWmCategory':'Holocaust'}, //  Count: 286
        "bricolage":{'rawWmCategory':'bricolage'}, //  Count: 286
        "Napoleonic Wars 1803–1815":{'rawWmCategory':'Napoleonic_Wars_1803–1815'}, //  Count: 286
        "freguesia (Portugal)":{'rawWmCategory':'freguesia_(Portugal)'}, //  Count: 286
        "halal":{'rawWmCategory':'halal'}, //  Count: 286
        "aircraft crash site":{'rawWmCategory':'aircraft_crash_site'}, //  Count: 285
        "oil pumping station":{'rawWmCategory':'oil_pumping_station'}, //  Count: 285
        "spa town":{'rawWmCategory':'spa_town'}, //  Count: 285
        "picture framing shop":{'rawWmCategory':'picture_framing_shop'}, //  Count: 284
        "picture frame maker":{'rawWmCategory':'picture_frame_maker'}, //  Count: 284
        "minaret":{'rawWmCategory':'minaret'}, //  Count: 283
        "minara":{'rawWmCategory':'minara'}, //  Count: 283
        "Kohl&#039;s":{'rawWmCategory':'Kohl&#039;s'}, //  Count: 283
        "rock climbing":{'rawWmCategory':'rock_climbing'}, //  Count: 283
        "Pushkin":{'rawWmCategory':'Pushkin'}, //  Count: 283
        "ruko":{'rawWmCategory':'ruko'}, //  Count: 282
        "tubewell":{'rawWmCategory':'tubewell'}, //  Count: 282
        "menhir":{'rawWmCategory':'menhir'}, //  Count: 281
        "biology":{'rawWmCategory':'biology'}, //  Count: 281
        "biological":{'rawWmCategory':'biological'}, //  Count: 281
        "[grill/meat restaurant]":{'rawWmCategory':'[grill/meat_restaurant]'}, //  Count: 280
        "panchayat [Dont use - to be replaced]":{'rawWmCategory':'panchayat_[Dont_use_-_to_be_replaced]'}, //  Count: 280
        "barbecue restaurant":{'rawWmCategory':'barbecue_restaurant'}, //  Count: 280
        "barbeque / BBQ restaurant":{'rawWmCategory':'barbeque_/_BBQ_restaurant'}, //  Count: 280
        "Georgian style architecture":{'rawWmCategory':'Georgian_style_architecture'}, //  Count: 280
        "psychology":{'rawWmCategory':'psychology'}, //  Count: 280
        "biotechnology":{'rawWmCategory':'biotechnology'}, //  Count: 280
        "ejido":{'rawWmCategory':'ejido'}, //  Count: 280
        "weather radar":{'rawWmCategory':'weather_radar'}, //  Count: 279
        "naval base":{'rawWmCategory':'naval_base'}, //  Count: 279
        "renovation":{'rawWmCategory':'renovation'}, //  Count: 279
        "Coptic Church":{'rawWmCategory':'Coptic_Church'}, //  Count: 279
        "shankar - to be replaced / deleted":{'rawWmCategory':'shankar_-_to_be_replaced_/_deleted'}, //  Count: 278
        "frontier post":{'rawWmCategory':'frontier_post'}, //  Count: 278
        "satellite tracking / ground station":{'rawWmCategory':'satellite_tracking_/_ground_station'}, //  Count: 277
        "mutt (Hinduism / Jainism)":{'rawWmCategory':'mutt_(Hinduism_/_Jainism)'}, //  Count: 277
        "Transilien":{'rawWmCategory':'Transilien'}, //  Count: 277
        "private garden":{'rawWmCategory':'private_garden'}, //  Count: 277
        "math (Hinduism / Jainism)":{'rawWmCategory':'math_(Hinduism_/_Jainism)'}, //  Count: 277
        "timber":{'rawWmCategory':'timber'}, //  Count: 277
        "matha":{'rawWmCategory':'matha'}, //  Count: 277
        "French cuisine":{'rawWmCategory':'French_cuisine'}, //  Count: 276
        "armoury":{'rawWmCategory':'armoury'}, //  Count: 276
        "armory":{'rawWmCategory':'armory'}, //  Count: 276
        "bleachers":{'rawWmCategory':'bleachers'}, //  Count: 276
        "drive-in theater":{'rawWmCategory':'drive-in_theater'}, //  Count: 275
        "fjord":{'rawWmCategory':'fjord'}, //  Count: 275
        "Caltex":{'rawWmCategory':'Caltex'}, //  Count: 275
        "RC airplane / helicopter field":{'rawWmCategory':'RC_airplane_/_helicopter_field'}, //  Count: 274
        "14th century construction":{'rawWmCategory':'14th_century_construction'}, //  Count: 274
        "Blockbuster store (movie rental)":{'rawWmCategory':'Blockbuster_store_(movie_rental)'}, //  Count: 274
        "model aircraft flying field":{'rawWmCategory':'model_aircraft_flying_field'}, //  Count: 274
        "jeepney terminal":{'rawWmCategory':'jeepney_terminal'}, //  Count: 273
        "bingo hall":{'rawWmCategory':'bingo_hall'}, //  Count: 272
        "rocket on display":{'rawWmCategory':'rocket_on_display'}, //  Count: 272
        "missile on display":{'rawWmCategory':'missile_on_display'}, //  Count: 272
        "self catering accommodation":{'rawWmCategory':'self_catering_accommodation'}, //  Count: 272
        "RER":{'rawWmCategory':'RER'}, //  Count: 272
        "curry restaurant":{'rawWmCategory':'curry_restaurant'}, //  Count: 272
        "prayer ground / prayer hall":{'rawWmCategory':'prayer_ground_/_prayer_hall'}, //  Count: 272
        "meteor crater":{'rawWmCategory':'meteor_crater'}, //  Count: 271
        "mountain range":{'rawWmCategory':'mountain_range'}, //  Count: 271
        "siva - to be replaced / deleted":{'rawWmCategory':'siva_-_to_be_replaced_/_deleted'}, //  Count: 271
        "trolleybus garage":{'rawWmCategory':'trolleybus_garage'}, //  Count: 271
        "trolleybus depot":{'rawWmCategory':'trolleybus_depot'}, //  Count: 271
        "impact crater":{'rawWmCategory':'impact_crater'}, //  Count: 271
        "office of the governor":{'rawWmCategory':'office_of_the_governor'}, //  Count: 271
        "astrobleme":{'rawWmCategory':'astrobleme'}, //  Count: 271
        "Murugan temple":{'rawWmCategory':'Murugan_temple'}, //  Count: 270
        "Kartikeya temple":{'rawWmCategory':'Kartikeya_temple'}, //  Count: 270
        "rodeo grounds/arena":{'rawWmCategory':'rodeo_grounds/arena'}, //  Count: 270
        "natural gas storage":{'rawWmCategory':'natural_gas_storage'}, //  Count: 270
        "kibbutz":{'rawWmCategory':'kibbutz'}, //  Count: 270
        "agricultural research farm":{'rawWmCategory':'agricultural_research_farm'}, //  Count: 270
        "heritage railway":{'rawWmCategory':'heritage_railway'}, //  Count: 269
        "cables":{'rawWmCategory':'cables'}, //  Count: 269
        "National Trust property (UK)":{'rawWmCategory':'National_Trust_property_(UK)'}, //  Count: 268
        "Appalachian trail":{'rawWmCategory':'Appalachian_trail'}, //  Count: 268
        "Metra":{'rawWmCategory':'Metra'}, //  Count: 268
        "boatyard":{'rawWmCategory':'boatyard'}, //  Count: 268
        "psychoneurologic dispensary":{'rawWmCategory':'psychoneurologic_dispensary'}, //  Count: 267
        "conservation":{'rawWmCategory':'conservation'}, //  Count: 267
        "UR-100K":{'rawWmCategory':'UR-100K'}, //  Count: 267
        "notable by news":{'rawWmCategory':'notable_by_news'}, //  Count: 266
        "imambargah":{'rawWmCategory':'imambargah'}, //  Count: 266
        "imambara":{'rawWmCategory':'imambara'}, //  Count: 266
        "holiday accommodation":{'rawWmCategory':'holiday_accommodation'}, //  Count: 265
        "nuclear waste storage":{'rawWmCategory':'nuclear_waste_storage'}, //  Count: 265
        "trade fair":{'rawWmCategory':'trade_fair'}, //  Count: 265
        "stone circle":{'rawWmCategory':'stone_circle'}, //  Count: 264
        "1910_construction":{'rawWmCategory':'1910_construction'}, //  Count: 264
        "Accor Hotels":{'rawWmCategory':'Accor_Hotels'}, //  Count: 263
        "VORTAC":{'rawWmCategory':'VORTAC'}, //  Count: 263
        "fireworks store":{'rawWmCategory':'fireworks_store'}, //  Count: 263
        "cenotaph":{'rawWmCategory':'cenotaph'}, //  Count: 263
        "Southeastern Pennsylvania Transportation Authority (SEPTA)":{'rawWmCategory':'Southeastern_Pennsylvania_Transportation_Authority_(SEPTA)'}, //  Count: 263
        "orthopaedics - orthopaedic surgery":{'rawWmCategory':'orthopaedics_-_orthopaedic_surgery'}, //  Count: 262
        "orthopedic hospital":{'rawWmCategory':'orthopedic_hospital'}, //  Count: 262
        "Tenaga Nasional Berhad":{'rawWmCategory':'Tenaga_Nasional_Berhad'}, //  Count: 262
        "search and rescue / SAR":{'rawWmCategory':'search_and_rescue_/_SAR'}, //  Count: 262
        "Chicago Transit Authority (CTA)":{'rawWmCategory':'Chicago_Transit_Authority_(CTA)'}, //  Count: 262
    } // End of transRules

} // End of wikimapia.rules
