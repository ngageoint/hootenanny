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
        "place without photos":{'place':'yes'}, //  Count: 22963702
        "place without description":{'place':'yes'}, //  Count: 14858547
        "place without category":{'place':'yes'}, //  Count: 13606916
        "building without address":{'building':'yes'}, //  Count: 6671102
        "place without polygon":{'place':'yes'}, //  Count: 3542484
        "building":{'building':'yes'}, //  Count: 1917432
        "home":{'building':'residential'}, //  Count: 1898890
        "residence":{'building':'residential'}, //  Count: 1898890
        "house":{'building':'house'}, //  Count: 1898890
        "villa":{'building':'villa'}, //  Count: 1898890
        "dwelling":{'building':'residential'}, //  Count: 1898890
        "tenement":{'building':'apartments'}, //  Count: 1212206
        "apartment building":{'building':'apartments'}, //  Count: 1212206
        "apartments":{'building':'apartments'}, //  Count: 1212206
        "block of flats":{'building':'apartments'}, //  Count: 1212206
        "tower block":{'building':'apartments'}, //  Count: 1212206
        "place with triangular polygon":{'place':'yes'}, //  Count: 1175369
        "village":{'place':'village'}, //  Count: 1112678
        "shopping and services":{'shop':'yes'}, //  Count: 1036724
        "store / shop":{'shop':'yes'}, //  Count: 1028282
        "dining and leisure":{'amenity':'restaurant'}, //  Count: 810321
        "education":{'amenity':'education'}, //  Count: 771971
        "school":{'amenity':'school'}, //  Count: 771971
        "schoolhouse":{'building':'school'}, //  Count: 771971
        "schooling":{'amenity':'school'}, //  Count: 771971
        "religion":{'amenity':'religion'}, //  Count: 714451
        "faith":{'amenity':'religion'}, //  Count: 714451 // Not great
        "religious":{'amenity':'religion'}, //  Count: 714451
        "place of worship":{'amenity':'place_of_worship'}, //  Count: 614034
        "water":{'natural':'water'}, //  Count: 594749
        "nonresidential building":{'building':'yes','residential':'no'}, //  Count: 489750
        "restaurant":{'amenity':'restaurant'}, //  Count: 465340
        "eatery":{'amenity':'restaurant'}, //  Count: 465340
        "administrative division":{'boundary':'administrative'}, //  Count: 436874
        "lake":{'natural':'water', 'water':'lake'}, //  Count: 428926
        "production":{'industrial':'yes'}, //  Count: 395727
        "industry":{'industrial':'yes'}, //  Count: 395727
        "sports center":{'leisure':'sports_centre'}, //  Count: 347537
        "sports venue":{'amenity':'event_venue'}, //  Count: 347537
        "sports centre":{'leisure':'sports_centre'}, //  Count: 347537
        "park":{'leisure':'park'}, //  Count: 343914
        "christianity":{'religion':'christian'}, //  Count: 318848
        "medical":{'amenity':'medical'}, //  Count: 316473
        "hotel":{'building':'hotel'}, //  Count: 288783
        "motor vehicles":{'motor_vehicle':'yes'}, //  Count: 283740
        "office building":{'building':'office'}, //  Count: 271671
        "offices":{'building':'office'}, //  Count: 271671
        "pont":{'bridge':'yes'}, //  Count: 260050 // French according to Wikipedia
        "bridge":{'bridge':'yes'}, //  Count: 260050
        "church":{'building':'church'}, //  Count: 259701
        "[automobile maintenance]":{'shop':'car_repair'}, //  Count: 250570
        "electric power":{'power':'electric'}, //  Count: 232202
        "burial monument/structure":{'tomb':'vault'}, //  Count: 228810
//        "do not draw title":{'rawWmCategory':'do_not_draw_title'}, //  Count: 221314
        "place with historical importance":{'place':'yes','historic':'yes'}, //  Count: 220790
        "interesting place":{'place':'yes'}, //  Count: 210694
        "cemetery":{'landuse':'cemetery'}, //  Count: 205955
        "graveyard":{'amenity':'grave_yard'}, //  Count: 205955
        "graves":{'cemetery':'grave'}, //  Count: 205955
        "gravesite":{'cemetery':'grave'}, //  Count: 205955
        "neighborhood":{'place':'neighbourhood'}, //  Count: 186988
        "neighbourhood":{'place':'neighbourhood'}, //  Count: 186988
        "invisible":{'invisible':'yes'}, //  Count: 176159 // ?????
        "railway":{'railway':'rail'}, //  Count: 173698
        "railroad":{'railway':'rail'}, //  Count: 173698
        "public transport":{'public_transport':'yes'}, //  Count: 170298
        "military":{'military':'yes'}, //  Count: 165392
        "islam":{'religion':'muslim'}, //  Count: 163004
        "muslim":{'religion':'muslim'}, //  Count: 163004
        "petrol / gas station":{'amenity':'fuel'}, //  Count: 162428
        "gasbar":{'amenity':'fuel'}, //  Count: 162428
        "filling station":{'amenity':'fuel'}, //  Count: 162428
        "fueling station":{'amenity':'fuel'}, //  Count: 162428
        "temple":{'building':'temple'}, //  Count: 160515
        "financial services":{'office':'financial_services'}, //  Count: 155041
        "city":{'place':'city'}, //  Count: 153051
        "mosque":{'building':'mosque'}, //  Count: 149014
        "masjid":{'building':'mosque'}, //  Count: 149014
        "hospital":{'amenity':'hospital'}, //  Count: 145644
        "sport court":{'leisure':'pitch'}, //  Count: 131193
        "pitch (sports field / ground)":{'leisure':'pitch'}, //  Count: 131193
        "sports ground":{'leisure':'pitch'}, //  Count: 131193
        // "draw only border":{'rawWmCategory':'draw_only_border'}, //  Count: 116226
        "agriculture":{'use':'agriculture'}, //  Count: 114935
        "mountain":{'natural':'mountain'}, //  Count: 111493
        "mount":{'natural':'mountain'}, //  Count: 111493
        "power transmission line":{'power':'line'}, //  Count: 109333
        "bank":{'amenity':'bank'}, //  Count: 108073
        "savings bank":{'amenity':'bank'}, //  Count: 108073
        "pond":{'water':'pond'}, //  Count: 104459
        "island":{'place':'island'}, //  Count: 104294
        "key (island)":{'place':'key'}, //  Count: 104294
        "isle":{'place':'island'}, //  Count: 104294
        "islet":{'place':'islet'}, //  Count: 104294
        "cay":{'natural':'cay'}, //  Count: 104294
        "islands":{'place':'island'}, //  Count: 104294
        "aviation":{'amenity':'aviation'}, //  Count: 103024
        "electricity pylon":{'power':'tower'}, //  Count: 101862
        "transmission tower (electric / electricity)":{'power':'tower'}, //  Count: 101862
        "hydro tower":{'man_made':'water_tower'}, //  Count: 101862
        "bazaar":{'amenity':'marketplace','highway':'pedestrian'}, //  Count: 97500
        "indoor market":{'amenity':'marketplace'}, //  Count: 97500
        "souk":{'amenity':'marketplace'}, //  Count: 97500
        "marketplace":{'amenity':'marketplace'}, //  Count: 97500
        "chowk (marketplace)":{'amenity':'marketplace'}, //  Count: 97500
        "market square":{'amenity':'marketplace','highway':'pedestrian'}, //  Count: 97500
        "stadium":{'leisure':'stadium'}, //  Count: 89455
        "bus stop":{'public_transport':'stop_position','bus':'yes'}, //  Count: 88819
        "bus stand":{'public_transport':'stop_position','bus':'yes'}, //  Count: 88819
        "law enforcement":{'amenity':'police'}, //  Count: 82748
        "bar":{'amenity':'bar'}, //  Count: 82675
        "cocktail lounge":{'amenity':'bar','cocktails':'yes'}, //  Count: 82675
        "saloon (bar)":{'amenity':'bar'}, //  Count: 82675
        "commercial building":{'building':'commercial'}, //  Count: 82373
        "[constructions by dates]":{'construction':'yes'}, //  Count: 81190
        "train station":{'railway':'station'}, //  Count: 76868
        "railroad station":{'railway':'station'}, //  Count: 76868
        "railway halt":{'railway':'halt'}, //  Count: 76868
        "railway station":{'railway':'station'}, //  Count: 76868
        "region":{'place':'region'}, //  Count: 76675
        "regions":{'place':'region'}, //  Count: 76675
        "university":{'amenity':'university'}, //  Count: 75637
        "parking":{'amenity':'parking'}, //  Count: 75437
        "water supply network":{'amenity':'water_supply'}, //  Count: 71065
        "police station":{'amenity':'police'}, //  Count: 70419
        "residential neighborhood":{'landuse':'residential','place':'neighbourhood'}, //  Count: 70223
        "residential area":{'landuse':'residential'}, //  Count: 70223
        "residential neighbourhood":{'landuse':'residential','place':'neighbourhood'}, //  Count: 70223
        "drugstore":{'amenity':'pharmacy'}, //  Count: 69933
        "pharmacy":{'amenity':'pharmacy'}, //  Count: 69933
        "medical dispensary":{'amenity':'pharmacy'}, //  Count: 69933
        "medical store / shop":{'shop':'medical'}, //  Count: 69933
        "chemist":{'shop':'chemist'}, //  Count: 69933
        "garden":{'leisure':'garden'}, //  Count: 69078
        "abandoned / shut down / defunct":{'abandoned':'yes'}, //  Count: 66928
        "cafe":{'amenity':'cafe'}, //  Count: 65767
        "museum":{'amenity':'museum'}, //  Count: 65518
        "apartment complex":{'building':'apartments'}, //  Count: 63638
        "housing complex":{'building':'apartments'}, //  Count: 63638
        "housing / apartment colony":{'building':'apartments'}, //  Count: 63638
        "listed building / architectural heritage":{'historic':'building'}, //  Count: 63379
        "urotshistshe (stow)":{'rawWmCategory':'urotshistshe_(stow)'}, //  Count: 63368 // ???
        "nursery school":{'amenity':'nursery'}, //  Count: 62531
        "kindergarten":{'amenity':'kindergarten'}, //  Count: 62531
        "preschool education":{'amenity':'kindergarten'}, //  Count: 62531
        "playgroup":{'amenity':'creche'}, //  Count: 62531
        "pre-school":{'amenity':'kindergarten'}, //  Count: 62531
        "playschool":{'amenity':'kindergarten'}, //  Count: 62531
        "supermarket":{'shop':'supermarket'}, //  Count: 62404
        "monument":{'historic':'monument'}, //  Count: 60409
        "historical layer / disappeared object":{'rawWmCategory':'historical_layer_/_disappeared_object'}, //  Count: 60345
        "electrical sub-station":{'power':'sub_station'}, //  Count: 57539
        "electrical substation":{'power':'sub_station'}, //  Count: 57539
        "electrical grid station":{'power':'sub_station'}, //  Count: 57539
        "electrical converter station":{'power':'sub_station'}, //  Count: 57539
        "primary education":{'amenity':'school','isced:level':'1'}, //  Count: 57444
        "elementary school":{'amenity':'school','isced:level':'1'}, //  Count: 57444
        "junior school":{'amenity':'school','isced:level':'1'}, //  Count: 57444
        "primary school":{'amenity':'school','isced:level':'1'}, //  Count: 57444
        "road bridge":{'bridge':'yes','transport:type':'road'}, //  Count: 56124
        "accommodation bridge":{'bridge':'yes','accomodation':'yes'}, //  Count: 56124
        "professional services":{'office':'professional_services'}, //  Count: 55893
        "fenced area":{'landuse':'fenced_area'}, //  Count: 54857
        "grocery / grocery store":{'shop':'grocery'}, //  Count: 52808
        "fort":{'historic':'fort'}, //  Count: 52679
        "fortress":{'historic':'castle','castle_type':'fortress'}, //  Count: 52679
        "forts":{'historic':'fort'}, //  Count: 52679
        "fortification":{'fortified':'yes'}, //  Count: 52679
        "ruins":{'ruins':'yes'}, //  Count: 52159
        "takeaway":{'takeaway':'yes'}, //  Count: 51993
        "carryout":{'takeaway':'yes'}, //  Count: 51993
        "take away / take out / carry out food":{'takeaway':'yes'}, //  Count: 51993
        "takeout":{'takeaway':'yes'}, //  Count: 51993
        "post office":{'amenity':'post_office'}, //  Count: 50312
        "farm":{'place':'farm'}, //  Count: 49882
        "farming":{'landuse':'farming'}, //  Count: 49882
        "square":{'highway':'pedestrian'}, //  Count: 49635
        "car repair/ maintenance service":{'shop':'car_repair'}, //  Count: 49448
        "automobile repair shop":{'shop':'car_repair'}, //  Count: 49448
        "garage (automobile repair)":{'shop':'car_repair)'}, //  Count: 49448
        "car park":{'amenity':'parking'}, //  Count: 49028
        "parking lot":{'amenity':'parking'}, //  Count: 49028
        "lorry park":{'amenity':'parking'}, //  Count: 49028
        "summer cottage area":{'tourism':'chalet'}, //  Count: 48183
        "fast food restaurant":{'amenity':'fast_food'}, //  Count: 48011
        "warehouse":{'building':'warehouse'}, //  Count: 47836
        "secondary education":{'amenity':'school','icsed':'2'}, //  Count: 47012
        "high school":{'amenity':'school','icsed':'2'}, //  Count: 47012
        "gymnasium - education":{'amenity':'school','icsed':'2'}, //  Count: 47012
        "university-preparatory school":{'amenity':'school','icsed':'4'}, //  Count: 47012
        "secondary school":{'amenity':'school','icsed':'2'}, //  Count: 47012
        "road":{'highway':'road'}, //  Count: 45153
        "beach":{'natural':'beach'}, //  Count: 44915
        "children's play area":{'leisure':'playground'}, //  Count: 43646
        "playground":{'leisure':'playground'}, //  Count: 43646
        "playscape":{'leisure':'playground'}, //  Count: 43646
        "play area":{'leisure':'playground'}, //  Count: 43646
        "swimming pool":{'leisure':'swimming_pool'}, //  Count: 41030
        "hamlet":{'place':'hamlet'}, //  Count: 40176
        "sub-village":{'place':'village'}, //  Count: 40176
        "town":{'place':'town'}, //  Count: 40017
        "20th century construction":{'rawWmCategory':'20th_century_construction'}, //  Count: 39586 // ????
        "catholicism":{'religion':'catholic'}, //  Count: 39576
        "catholic":{'religion':'catholic'}, //  Count: 39576
        "shopping mall":{'shop':'mall'}, //  Count: 39478
        "shopping center":{'shop':'mall'}, //  Count: 39478
        "shopping arcade":{'shop':'mall'}, //  Count: 39478
        "shopping centre":{'shop':'mall'}, //  Count: 39478
        "retail shopping center":{'shop':'mall'}, //  Count: 39478
        "library":{'amenity':'library'}, //  Count: 38139
        "archaeological site":{'historic':'archaeological_site'}, //  Count: 37551
        "archeological site":{'historic':'archaeological_site'}, //  Count: 37551
        "firefighters":{'amenity':'fire_station'}, //  Count: 37279
        "fire station":{'amenity':'fire_station'}, //  Count: 37279
        "fire service":{'amenity':'fire_station'}, //  Count: 37279
        "fireman":{'amenity':'fire_station'}, //  Count: 37279
        "fire brigade":{'amenity':'fire_station'}, //  Count: 37279
        "fire department":{'amenity':'fire_station'}, //  Count: 37279
        "intersection (road)":{'junction':'yes','highway':'road'}, //  Count: 37047
        "crossroad":{'junction':'yes'}, //  Count: 37047
        "chowk (road junction)":{'junction':'yes','highway':'road'}, //  Count: 37047
        "road junction":{'junction':'yes','highway':'road'}, //  Count: 37047
        "motorway junction":{'highway':'motorway_junction'}, //  Count: 37047
        "airport":{'aeroway':'aerodrome'}, //  Count: 37023
        "airfield":{'aeroway':'aerodrome'}, //  Count: 37023
        "airstrip":{'aeroway':'aerodrome'}, //  Count: 37023
        "water tower":{'man_made':'water_tower'}, //  Count: 36653
        "football pitch":{'leisure':'pitch','sport':'football'}, //  Count: 36625
        "football - soccer field":{'leisure':'pitch','sport':'soccer'}, //  Count: 36625
        "football field":{'leisure':'pitch','sport':'football'}, //  Count: 36625
        "coastal construction":{'landuse':'coastal_construction'}, //  Count: 36311
        "animals":{'amenity':'animals'}, //  Count: 34688
        "water treatment":{'man_made':'water_works'}, //  Count: 34600
        "water purification":{'man_made':'water_works'}, //  Count: 34600
        "transformer substation":{'power':'sub_station'}, //  Count: 34283
        "condo":{'building':'apartments'}, //  Count: 34023
        "condominium":{'building':'apartments'}, //  Count: 34023
        "academic institution":{'amenity':'school'}, //  Count: 33972
        "[automotive industry]":{'rawWmCategory':'[automotive_industry]'}, //  Count: 33099 // ????
        "apartment":{'building':'apartments'}, //  Count: 33029
        "flat":{'building':'appartments'}, //  Count: 33029
        "factory":{'man_made':'works'}, //  Count: 32509
        "plant":{'man_made':'works'}, //  Count: 32509 // Close???
        "wastewater treatment plant":{'man_made':'wastewater_plant'}, //  Count: 31851
        "sewage treatment plant":{'man_made':'wastewater_plant'}, //  Count: 31851
        "effluent treatment plant":{'man_made':'wastewater_plant'}, //  Count: 31851
        "water reclamation plant":{'man_made':'water_works'}, //  Count: 31851 // ????
        "forest":{'landuse':'forest'}, //  Count: 31796
        "mine":{'landuse':'mineral_mining'}, //  Count: 31657
        "garage":{'building':'garage'}, //  Count: 31375
        "car dealership":{'shop':'car'}, //  Count: 30946
        "car showroom":{'shop':'car'}, //  Count: 30946
        "automobile / car dealer (sales)":{'shop':'car'}, //  Count: 30946
        "memorial":{'historic':'memorial'}, //  Count: 30596
        "Catholic church":{'amenity':'place_of_worship','religion':'catholic'}, //  Count: 30497
        "clothes store / boutique":{'shop':'clothes'}, //  Count: 30087
        "clothing store":{'shop':'clothes'}, //  Count: 30087
        "oil and gas industry":{'landuse':'oil_and_gas_industry'}, //  Count: 29928 // ????
        "petroleum industry":{'landuse':'petroleum_industry'}, //  Count: 29928 // ????
        "municipality":{'place':'municipality'}, //  Count: 29445
        "indoor tennis court(s)":{'leisure':'pitch','sport':'tennis'}, //  Count: 28201
        "tennis court(s)":{'leisure':'pitch','sport':'tennis'}, //  Count: 28201
        "household yard":{'landuse':'residential'}, //  Count: 28047
        "atm / cash machine":{'amenity':'atm'}, //  Count: 27953
        "district, hamlet":{'place':'hamlet'}, //  Count: 27493
        "ortsteil":{'place':'neighbourhood'}, //  Count: 27493
        "cinema":{'amenity':'cinema'}, //  Count: 26916
        "cineworld":{'amenity':'cinema'}, //  Count: 26916
        "movie theater":{'amenity':'cinema'}, //  Count: 26916
        "talkies (cinema)":{'amenity':'cinema'}, //  Count: 26916
        "golf course":{'leisure':'golf_course'}, //  Count: 26599
        "golf link":{'leisure':'golf_course'}, //  Count: 26599
        "retailer":{'shop':'yes'}, //  Count: 26573
        "district office":{'office':'administrative'}, //  Count: 26161
        "town hall":{'amenity':'townhall'}, //  Count: 26161
        "civic hall":{'amenity':'townhall'}, //  Count: 26161
        "civic centre":{'amenity':'community_centre'}, //  Count: 26161
        "city hall":{'amenity':'townhall'}, //  Count: 26161
        "county hall":{'amenity':'townhall'}, //  Count: 26161
        "council house - local government":{'office':'administrative'}, //  Count: 26161
        "municipal hall":{'amenity':'townhall'}, //  Count: 26161
        "dorm":{'building':'dormitory'}, //  Count: 26039
        "dormitory":{'building':'dormitory'}, //  Count: 26039
        "hall of residence":{'building':'dormitory'}, //  Count: 26039 // Close
        "student accommodation":{'building':'dormitory'}, //  Count: 26039
        "residence hall":{'building':'dormitory'}, //  Count: 26039
        "sari-sari store (Philippines)":{'shop':'convenience'}, //  Count: 25378
        "convenience store":{'shop':'convenience'}, //  Count: 25378
        "hinduism":{'religion':'hindu'}, //  Count: 25373
        "hairdresser":{'shop':'hairdresser'}, //  Count: 25237
        "hairstylist":{'shop':'hairdresser'}, //  Count: 25237
        "hair salon":{'shop':'hairdresser'}, //  Count: 25237
        "household goods store / shop":{'shop':'houseware'}, //  Count: 24467
        "quarter (urban subdivision)":{'place':'quarter'}, //  Count: 24430
        "urban settlement or rural community":{'place':'village'}, //  Count: 24430
        "railway bridge":{'bridge':'yes','transport:type':'railway'}, //  Count: 24239
        "heating":{'amenity':'heating'}, //  Count: 23997
        "bog":{'natural':'wetland','wetland':'bog'}, //  Count: 23667
        "quagmire":{'natural':'wetland','wetland':'bog'}, //  Count: 23667
        "mire":{'natural':'wetland','wetland':'bog'}, //  Count: 23667
        "wetland":{'natural':'wetland'}, //  Count: 23667
        "medical clinic":{'amenity':'clinic'}, //  Count: 23176
        "medical centre":{'amenity':'clinic'}, //  Count: 23176
        "health centre":{'amenity':'health_centre'}, //  Count: 23176
        "health center":{'amenity':'health_centre'}, //  Count: 23176
        "sculpture":{'tourism':'artwork','artwork_type':'sculpture'}, //  Count: 23044
        "night club":{'amenity':'nightclub'}, //  Count: 23002
        "theater":{'amenity':'theatre'}, //  Count: 22754
        "theatre":{'amenity':'theatre'}, //  Count: 22754
        "green area":{'leisure':'park'}, //  Count: 22733
        "administrative building":{'building':'administrative'}, //  Count: 22642
        "orthodox christianity":{'religion':'christian','denomination':'orthodox'}, //  Count: 22357
        "conglomerate":{'rawWmCategory':'conglomerate'}, //  Count: 22132 // ????
        "company":{'office':'company'}, //  Count: 22132
        "firm":{'office':'company'}, //  Count: 22132
        "under construction":{'construction':'yes'}, //  Count: 22112
        "settlement":{'place':'settlement'}, //  Count: 21422
        "dam":{'waterway':'dam'}, //  Count: 21370
        "Second World War 1939-1945":{'rawWmCategory':'Second_World_War_1939-1945'}, //  Count: 21182 // ????
        "pub / public house":{'amenity':'pub'}, //  Count: 20965
        "community center / hall":{'amenity':'community_centre'}, //  Count: 20717
        "community centre / hall":{'amenity':'community_centre'}, //  Count: 20717
        "communal house":{'building':'communal_house'}, //  Count: 20717
        "tower":{'man_made':'tower'}, //  Count: 20648
        "hindu temple":{'building':'temple','amenity':'place_of_worship','religion':'hindu'}, //  Count: 20177
        "mandir":{'building':'temple','amenity':'place_of_worship','religion':'hindu'}, //  Count: 20177
        "workshop":{'building':'workshop'}, //  Count: 19477
        "swamp":{'natural':'wetland','wetland':'swamp'}, //  Count: 19165
        "reservoir":{'landuse':'reservoir'}, //  Count: 18983
        "artificial lake":{'natural':'water','water':'reservoir'}, //  Count: 18983
        "manmade lake":{'natural':'water','water':'reservoir'}, //  Count: 18983
        "gym / gymnasium - sports":{'amenity':'gym'}, //  Count: 18864
        "school yard":{'amenity':'school'}, //  Count: 18727
        "abandoned settlement":{'abandoned:place':'settlement'}, //  Count: 18677
        "ghost town":{'abandoned:place':'ghost_town'}, //  Count: 18677
        "pizzeria":{'amenity':'restaurant','cuisine':'pizza'}, //  Count: 18535
        "pizza shop / parlor":{'amenity':'restaurant','cuisine':'pizza'}, //  Count: 18535
        "foot over bridge (fob)":{'highway':'footway','bridge':'yes'}, //  Count: 18399
        "footbridge":{'highway':'footway','bridge':'yes'}, //  Count: 18399
        "pedestrian overpass":{'highway':'footway','bridge':'yes'}, //  Count: 18399
        "earthworks":{'man_made':'earthworks'}, //  Count: 18348
        "earthwork (archaeology)":{'historic':'archaeological_site','site_type':'earthworks'}, //  Count: 18348
        "fountain":{'amenity':'fountain'}, //  Count: 18306
        "peak":{'natural':'peak'}, //  Count: 18133
        "mountaintop":{'natural':'peak'}, //  Count: 18133
        "summit":{'natural':'peak'}, //  Count: 18133
        "pinnacle":{'natural':'rock_pinnacle'}, //  Count: 18133
        "hill":{'natural':'hill'}, //  Count: 17950
        "knoll":{'natural':'knoll'}, //  Count: 17950
        "hillock":{'natural':'hillock'}, //  Count: 17950
        "hills":{'natural':'hill'}, //  Count: 17950
        "street":{'highway':'road','ref:road:type':'street'}, //  Count: 17551
        "avenue":{'highway':'road','ref:road:type':'avenue'}, //  Count: 17551
        "alley":{'highway':'service','service':'alley'}, //  Count: 17551
        "lane":{'highway':'unclassified','ref:road:type':'lane'}, //  Count: 17551
        "boulevard":{'highway':'road','ref:road:type':'boulevard'}, //  Count: 17551
        "farm land / agricultural area":{'landuse':'farm_land'}, //  Count: 17525
        "open-pit mine":{'landuse':'mineral_mining','mine:type':'opencast'}, //  Count: 17520
        "mobile phone transmitter / mast":{'man_made':'mast','tower:type':'communications','communication:mobile_phone':'yes'}, //  Count: 17505
        "cell site / cell tower":{'man_made':'tower','tower:type':'communications','communication:mobile_phone':'yes'}, //  Count: 17505
        "bts":{'rawWmCategory':'bts'}, //  Count: 17505 // ???
        "base transceiver/ GSM station":{'man_made':'mast','tower:type':'communications','communication:mobile_phone':'gsm'}, //  Count: 17505
        "pets":{'amenity':'pets'}, //  Count: 17493
        "subdivision":{'place':'neighbourhood'}, //  Count: 17489
        "chapel":{'building':'chapel','amenity':'place_of_worship'}, //  Count: 17456
        "middle school":{'amenity':'school','isced:level':'2'}, //  Count: 17429
        "junior high school":{'amenity':'school','isced:level':'3'}, //  Count: 17429
        "intermediate school":{'amenity':'school','isced:level':'3'}, //  Count: 17429
        "power plant":{'power':'plant','landuse':'industrial'}, //  Count: 17409
        "power house":{'power':'generator'}, //  Count: 17409
        "power station":{'power':'plant','landuse':'industrial'}, //  Count: 17409
        "generating station":{'power':'generator'}, //  Count: 17409
        "railroad crossing":{'railway':'crossing'}, //  Count: 17397
        "level crossing":{'railway':'level_crossing'}, //  Count: 17397
        "railway crossing":{'railway':'crossing'}, //  Count: 17397
        "train crossing":{'railway':'crossing'}, //  Count: 17397
        "grade crossing":{'rawWmCategory':'grade_crossing'}, //  Count: 17397
        "sports club":{'club':'sport'}, //  Count: 17126
        "club":{'club':'yes'}, //  Count: 17031
        "automotive parts dealer":{'shop':'car_parts'}, //  Count: 17013
        "car accessories store":{'shop':'car_parts'}, //  Count: 17013
        "RV / caravan park":{'tourism':'caravan_site'}, //  Count: 16940
        "campsite":{'tourism':'campsite'}, //  Count: 16940
        "camping pitch":{'tourism':'campsite'}, //  Count: 16940
        "campgrounds":{'tourism':'campsite'}, //  Count: 16940
        "RV Resort":{'tourism':'campsite','RV':'yes'}, //  Count: 16940
        "bus station":{'public_transport':'station','bus':'yes'}, //  Count: 16935
        "coach station":{'public_transport':'station','bus':'yes'}, //  Count: 16935
        "bus terminal":{'public_transport':'station','bus':'yes'}, //  Count: 16935
        "semi-detached house":{'building':'house'}, //  Count: 16909
        "quarry":{'landuse':'quarry'}, //  Count: 16904
        "furniture store / shop":{'shop':'furniture'}, //  Count: 16709
        "tunnel":{'tunnel':'yes'}, //  Count: 16687
        "display":{'rawWmCategory':'display'}, //  Count: 16591 // ????
        "auto wash":{'amenity':'car_wash'}, //  Count: 16573
        "car wash":{'amenity':'car_wash'}, //  Count: 16573
        "bakery":{'shop':'bakery'}, //  Count: 16323
        "bakehouse":{'shop':'bakery'}, //  Count: 16323
        "wind turbine":{'power':'generator','generator:source':'wind','generator:method':'wind_turbine','generator:output':'electricity'}, //  Count: 16139
        "fourth-level administrative division":{'rawWmCategory':'fourth-level_administrative_division'}, //  Count: 15913 // ????
        "protestantism":{'religion':'christian','denomination':'protestant'}, //  Count: 15849
        "protestant":{'religion':'christian','denomination':'protestant'}, //  Count: 15849
        "hardware store":{'shop':'hardware'}, //  Count: 15494
        "DIY store":{'shop':'doityourself'}, //  Count: 15494
        "home improvement / hardware store":{'shop':'doityourself'}, //  Count: 15494
        "painting and decorating shop":{'shop':'paint'}, //  Count: 15494
        "home improvement center":{'shop':'doityourself'}, //  Count: 15494
        "historical building":{'historic':'building'}, //  Count: 15202
        "NAVAID - navigational aid":{'aeroway':'navigationalaid'}, //  Count: 15187
        "ATON - aid to navigation":{'aeroway':'aid_to_navigation'}, //  Count: 15187
        "bathhouse":{'amenity':'public_bath'}, //  Count: 15148
        "thermae":{'amenity':'public_bath'}, //  Count: 15148
        "public baths":{'amenity':'public_bath'}, //  Count: 15148
        "banya":{'leisure':'sauna'}, //  Count: 15148
        "steambath":{'leisure':'sauna'}, //  Count: 15148
        "hostel":{'tourism':'hostel'}, //  Count: 15013
        "basketball court":{'leisure':'pitch','sport':'basketball'}, //  Count: 14881
        "dentist":{'amenity':'dentist'}, //  Count: 14804
        "dental practice":{'amenity':'dentist'}, //  Count: 14804
        "dental clinic":{'amenity':'dentist'}, //  Count: 14804
        "bay":{'natural':'bay'}, //  Count: 14748
        "gulf":{'natural':'gulf'}, //  Count: 14748
        "service":{'rawWmCategory':'service'}, //  Count: 14701 // ????
        "commune - administrative division":{'boundary':'administrative','admin_level':'7'}, //  Count: 14475
        "roundabout / traffic circle":{'junction':'roundabout'}, //  Count: 14417
        "chowk (roundabout / traffic circle)":{'junction':'roundabout'}, //  Count: 14417
        "colony / nagar - neighbourhood in India":{'place':'neighbourhood'}, //  Count: 14392
        "castle":{'historic':'castle'}, //  Count: 14087
        "checkpoint":{'barrier':'checkpoint'}, //  Count: 14087
        "spring":{'natural':'spring'}, //  Count: 14063
        "war memorial":{'historic':'memorial','memorial:type':'war_memorial'}, //  Count: 13898
        "building materials":{'rawWmCategory':'building_materials'}, //  Count: 13816 // ????
        "boiler house":{'building':'boiler_house'}, //  Count: 13748
        "heat-only boiler station":{'rawWmCategory':'heat-only_boiler_station'}, //  Count: 13748 // ????
        "architectural style":{'rawWmCategory':'architectural_style'}, //  Count: 13636 // ????
        "football / soccer stadium":{'leisure':'stadium'}, //  Count: 13230
        "gate":{'barrier':'gate'}, //  Count: 13172
        "courthouse":{'amenity':'courthouse'}, //  Count: 13129
        "transportation":{'amenity':'transportation'}, //  Count: 13021
        "jeepney stop":{'public_transport':'stop_position','jeepney':'yes'}, //  Count: 12833
        "share taxi stop":{'public_transport':'stop_position','taxi':'yes'}, //  Count: 12833
        "bush taxi stop":{'public_transport':'stop_position','taxi':'yes'}, //  Count: 12833
        "ancient civilization":{'historic':'ancient_civilization'}, //  Count: 12726
        "abbey":{'building':'abbey','amenity':'place_of_worship'}, //  Count: 12599
        "convent":{'building':'convent','amenity':'place_of_worship'}, //  Count: 12599
        "convento":{'building':'convent','amenity':'place_of_worship'}, //  Count: 12599
        "monastery":{'building':'monastery','amenity':'place_of_worship'}, //  Count: 12599
        "priory":{'building':'priory','amenity':'place_of_worship'}, //  Count: 12599
        "leisure":{'leisure':'yes'}, //  Count: 12556
        "barangay":{'place':'village'}, //  Count: 12478
        "industrial estate":{'landuse':'industrial'}, //  Count: 12241
        "industrial area":{'landuse':'industrial'}, //  Count: 12241
        "industrial zone":{'landuse':'industrial'}, //  Count: 12241
        "industrial district":{'landuse':'industrial'}, //  Count: 12241
        "aeroplane on display":{'historic':'aircraft'}, //  Count: 12238
        "aircraft on display":{'historic':'aircraft'}, //  Count: 12238
        "helicopter on display":{'historic':'helicopter'}, //  Count: 12238
        "public garden":{'leisure':'garden'}, //  Count: 12168
        "green space":{'leisure':'park'}, //  Count: 12168
        "urban open space":{'rawWmCategory':'urban_open_space'}, //  Count: 12168
        "district":{'place':'district'}, //  Count: 12161
        "chimney":{'man_made':'chimney'}, //  Count: 12091
        "smokestack":{'man_made':'chimney'}, //  Count: 12091
        "terraced / row house - townhome / townhouse":{'building':'terrace'}, //  Count: 11967
        "townhome":{'building':'terrace'}, //  Count: 11967
        "townhouse":{'building':'terrace'}, //  Count: 11967
        "runway":{'aeroway':'runway'}, //  Count: 11949
        "landing strip":{'aeroway':'runway'}, //  Count: 11949
        "aircraft":{'aircraft':'yes'}, //  Count: 11852
        "organization":{'rawWmCategory':'organization'}, //  Count: 11846 // ????
        "organisation":{'rawWmCategory':'organisation'}, //  Count: 11846 // ????
        "pumping station":{'man_made':'pumping_station'}, //  Count: 11661
        "tube station":{'railway':'station','subway':'yes'}, //  Count: 11618
        "metro station":{'railway':'station','subway':'yes'}, //  Count: 11618
        "underground station":{'railway':'station','subway':'yes'}, //  Count: 11618
        "subway station":{'railway':'station','subway':'yes'}, //  Count: 11618
        "food industry":{'rawWmCategory':'food_industry'}, //  Count: 11580 // ????
        "protected area":{'boundary':'protected_area'}, //  Count: 11392
        "nature reserve":{'leisure':'nature_reserve'}, //  Count: 11392
        "nature conservation park / area":{'boundary':'protected_area','protect_class':'7'}, //  Count: 11392
        "wilderness area":{'boundary':'protected_area','protect_class':'1'}, //  Count: 11392
        "wildlife sanctuary":{'boundary':'protected_area','protect_class':'4'}, //  Count: 11392
        "nature preserve":{'boundary':'protected_area','protect_class':'7'}, //  Count: 11392
        "policlinic":{'amenity':'clinic'}, //  Count: 11391
        "outpatient clinic":{'amenity':'clinic'}, //  Count: 11391
        "polyclinic":{'amenity':'clinic'}, //  Count: 11391
        "mill":{'man_made':'mill'}, //  Count: 11390
        "trolleybus stop":{'public_transport':'stop_position','trolleybus':'yes'}, //  Count: 11372
        "headland":{'natural':'headland'}, //  Count: 11218
        "campus":{'rawWmCategory':'campus'}, //  Count: 11190 // ????
        "watercraft":{'rawWmCategory':'watercraft'}, //  Count: 11137 // ????
        "tram stop":{'public_transport':'stop_position','tram':'yes'}, //  Count: 11090
        "streetcar stop":{'public_transport':'stop_position','trolleybus':'yes'}, //  Count: 11090
        "telecom":{'rawWmCategory':'telecom'}, //  Count: 11044
        "telecommunication":{'rawWmCategory':'telecommunication'}, //  Count: 11044
        "beauty salon":{'shop':'beauty'}, //  Count: 10972
        "beauty parlor":{'shop':'beauty'}, //  Count: 10972
        "embassy / consulate / diplomatic mission":{'amenity':'embassy'}, //  Count: 10925
        "dining hall":{'amenity':'dining_hall'}, //  Count: 10924
        "banqueting hall":{'amenity':'events_venue'}, //  Count: 10924
        "canteen":{'amenity':'canteen'}, //  Count: 10924
        "nuragic civilization - archeology":{'historic':'archaeological_site','historic:civilization':'nuragic'}, //  Count: 10885
        "government":{'office':'government'}, //  Count: 10839
        "shed":{'building':'shed'}, //  Count: 10774
        "prison":{'amenity':'prison'}, //  Count: 10638
        "jail":{'amenity':'prison'}, //  Count: 10638
        "jailhouse":{'amenity':'prison'}, //  Count: 10638
        "area - to be replaced":{'rawWmCategory':'area_-_to_be_replaced'}, //  Count: 10568 // ????
        "overpass / flyover":{'overpass':'yes'}, //  Count: 10560
        "buddhism":{'religion':'buddhist'}, //  Count: 10553
        "cottage":{'building':'cottage'}, //  Count: 10531
        "footwear shop":{'shop':'shoes'}, //  Count: 10473
        "shoe shop / store":{'shop':'shoes'}, //  Count: 10471
        "business centre/ building":{'building':'commercial'}, //  Count: 10444
        "round barrow":{'site_type':'round_barrow'}, //  Count: 10261 // ????
        "barrow/tumulus/burial mound":{'site_type':'barrow/tumulus/burial_mound'}, //  Count: 10261
        "long barrow":{'site_type':'long_barrow'}, //  Count: 10261
        "burial mound":{'site_type':'burial_mound'}, //  Count: 10261
        "howe":{'rawWmCategory':'howe'}, //  Count: 10261 // ????
        "bowl barrow":{'site_type':'bowl_barrow'}, //  Count: 10261
        "bank barrow":{'site_type':'bank_barrow'}, //  Count: 10261
        "bell barrow":{'site_type':'bell_barrow'}, //  Count: 10261
        "electronic goods retailer / manufacturer":{'shop':'electronics'}, //  Count: 10198
        "third-level administrative division":{'rawWmCategory':'third-level_administrative_division'}, //  Count: 10193 // ????
        "playing field":{'leisure':'pitch'}, //  Count: 10154
        "recreation area / ground":{'leisure':'park'}, //  Count: 10154
        "lab":{'amenity':'laboratory'}, //  Count: 10150
        "laboratories":{'amenity':'laboratory'}, //  Count: 10150
        "labs":{'amenity':'laboratory'}, //  Count: 10150
        "river":{'waterway':'river'}, //  Count: 10050
        "water supply":{'amenity':'water_supply'}, //  Count: 10024
        "college - DONT USE OR TRANSLATE THIS CATEGORY IS PLANNED TO BE REMOVED":{'rawWmCategory':'college_-_DONT_USE_OR_TRANSLATE_THIS_CATEGORY_IS_PLANNED_TO_BE_REMOVED'}, //  Count: 9988
        "tire repair service":{'shop':'tyres','repair':'yes'}, //  Count: 9931
        "tyre repair service":{'shop':'tyres','repair':'yes'}, //  Count: 9931
        "tire assembly":{'industrial':'tire_assembly'}, //  Count: 9931
        "local government":{'office':'local_government'}, //  Count: 9851
        "trading":{'shop':'trade'}, //  Count: 9777
        "trade":{'shop':'trade'}, //  Count: 9777
        "commerce":{'amenity':'commerce'}, //  Count: 9777
        "kindergarten yard":{'amenity':'kindergarten'}, //  Count: 9611
        "sign":{'man_made':'sign'}, //  Count: 9523
        "construction firm / organisation":{'office':'construction'}, //  Count: 9513
        "construction / building company":{'office':'construction'}, //  Count: 9513
        "waste management industry":{'office':'waste_management_industry'}, //  Count: 9498
        "[waste management]":{'amenity':'waste_management'}, //  Count: 9498
        "fitness suite":{'leisure':'fitness_centre'}, //  Count: 9467
        "health club":{'rawWmCategory':'health_club'}, //  Count: 9467
        "fitness centre / center":{'amenity':'fitness_centre'}, //  Count: 9467
        "fitness club":{'amenity':'fitness_centre'}, //  Count: 9467
        "gym - fitness / health":{'amenity':'fitness_centre'}, //  Count: 9467
        "public toilet":{'amenity':'toilets'}, //  Count: 9430
        "restroom":{'amenity':'toilets'}, //  Count: 9430
        "public washroom":{'amenity':'toilets'}, //  Count: 9430
        "resort":{'leisure':'resort'}, //  Count: 9422
        "confluence":{'water:morphology':'confluence'}, //  Count: 9377
        "statue":{'tourism':'artwork','artwork_type':'statue'}, //  Count: 9316
        "liquor store":{'store':'alcohol'}, //  Count: 9194
        "off-licence":{'shop':'alcohol'}, //  Count: 9194
        "McDonald&#039;s":{'amenity':'fast_food','name':"McDonald's"}, //  Count: 9175
        "McDrive":{'amenity':'fast_food','name':"McDonald's"}, //  Count: 9175 // ???? Guess
        "buddhist temple":{'building':'temple','religion':'buddhist','amenity':'place_of_worship'}, //  Count: 9155
        "wooden":{'rawWmCategory':'wooden'}, //  Count: 8998 // ????
        "travel agency":{'shop':'travel_agency'}, //  Count: 8925
        "bookstore":{'shop':'books'}, //  Count: 8903
        "bookshop":{'shop':'books'}, //  Count: 8903
        "1960s construction":{'rawWmCategory':'1960s_construction'}, //  Count: 8743 // ????
        "real estate agency":{'office':'estate_agent'}, //  Count: 8686
        "estate agents":{'office':'estate_agent'}, //  Count: 8686
        "vocational school":{'amenity':'school','icsed':'4'}, //  Count: 8684
        "vocational training":{'amenity':'school','icsed':'4'}, //  Count: 8684
        "sciences":{'rawWmCategory':'sciences'}, //  Count: 8611 // ????
        "science":{'rawWmCategory':'science'}, //  Count: 8611 // ????
        "scientist":{'rawWmCategory':'scientist'}, //  Count: 8611 // ????
        "scientific":{'rawWmCategory':'scientific'}, //  Count: 8611 // ????
        "airplane":{'rawWmCategory':'airplane'}, //  Count: 8581 // ????
        "aeroplane":{'rawWmCategory':'aeroplane'}, //  Count: 8581 // ????
        "fishing area":{'leisure':'fishing_area'}, //  Count: 8569
        "fishery":{'shop':'seafood'}, //  Count: 8569
        "mansion / manor house / villa":{'historic':'castle','castle_type':'manor'}, //  Count: 8564
        "stately home":{'historic':'castle','castle_type':'stately'}, //  Count: 8564
        "country house":{'building':'country_house'}, //  Count: 8564
        "highway interchange / exit":{'junction':'interchange'}, //  Count: 8464
        "oil/gas well":{'man_made':'petroleum_well'}, //  Count: 8456
        "pumpjack":{'man_made':'pumpjack'}, //  Count: 8456
        "aid to navigation - marine":{'seamark:navigation_aid':'yes'}, //  Count: 8430
        "industrial building":{'building':'industrial'}, //  Count: 8420
        "carport":{'amenity':'parking','parking':'carport'}, //  Count: 8420
        "residential garage":{'building':'garage'}, //  Count: 8420
        "garage cooperative society":{'building':'garage'}, //  Count: 8420
        "factory building":{'rawWmCategory':'factory_building'}, //  Count: 8420
        "internet cafe":{'amenity':'internet_cafe'}, //  Count: 8255
        "cybercafe":{'amenity':'internet_cafe'}, //  Count: 8255
        "net cafe":{'amenity':'internet_cafe'}, //  Count: 8255
        "sporting goods store":{'shop':'sports'}, //  Count: 8236
        "sporting/sports equipment":{'shop':'sports'}, //  Count: 8236
        "sports store / shop":{'shop':'sports'}, //  Count: 8236
        "sports retailer":{'shop':'sports'}, //  Count: 8236
        "embassy":{'amenity':'embassy'}, //  Count: 8214
        "high commission (embassy)":{'amenity':'embassy'}, //  Count: 8214
        "conjunto":{'rawWmCategory':'conjunto'}, //  Count: 8184 // ????
        "jewellery":{'shop':'jewelry'}, //  Count: 8146
        "jeweler":{'shop':'jewelry'}, //  Count: 8146
        "jewelry":{'shop':'jewelry'}, //  Count: 8146
        "jeweller":{'shop':'jewelry'}, //  Count: 8146
        "estate (manor / mansion land)":{'landuse':'estate'}, //  Count: 8080
        "manor (land)":{'landuse':'manor'}, //  Count: 8080
        "21st century construction":{'rawWmCategory':'21st_century_construction'}, //  Count: 8052 // ????
        "food - to be replaced":{'shop':'food'}, //  Count: 7948
        "cellphone shop / store":{'shop':'mobile_phone'}, //  Count: 7938
        "mobile phone retailer":{'shop':'mobile_phone'}, //  Count: 7938
        "palace":{'building':'palace'}, //  Count: 7833
        "kiosk":{'shop':'kiosk'}, //  Count: 7680
        "storage":{'shop':'storage_rental'}, //  Count: 7666
        "educational building":{'building':'school'}, //  Count: 7643
        "academic building":{'building':'school'}, //  Count: 7643
        "community":{'amenity':'community'}, //  Count: 7618
        "coffeehouse":{'cuisine':'coffee_shop'}, //  Count: 7616
        "espresso bar":{'cuisine':'coffee_shop'}, //  Count: 7616
        "fuel storage tank":{'man_made':'storage_tank','content':'fuel'}, //  Count: 7612
        "junction - Do not use this category, the category is under cleanup":{'rawWmCategory':'junction_-_Do_not_use_this_category,_the_category_is_under_cleanup'}, //  Count: 7566
        "bed and breakfast":{'tourism':'guest_housr','guest_houst':'bed_and_breakfast'}, //  Count: 7562
        "Grade II Listed (UK)":{'rawWmCategory':'Grade_II_Listed_(UK)'}, //  Count: 7511 // ????
        "horse":{'rawWmCategory':'horse'}, //  Count: 7504
        "children":{'rawWmCategory':'children'}, //  Count: 7423 // ????
        "lighthouse":{'man_made':'lighthouse'}, //  Count: 7398
        "grassland":{'natural':'grassland'}, //  Count: 7371
        "natural gas industry":{'rawWmCategory':'natural_gas_industry'}, //  Count: 7357 // ???
        "prehistoric":{'historic:civilization':'prehistoric'}, //  Count: 7270
        "public transport terminus":{'public_transport':'station'}, //  Count: 7192
        "birds":{'rawWmCategory':'birds'}, //  Count: 7162 // ????
        "allotment (gardening)":{'landuse':'allotments'}, //  Count: 7147
        "seaport":{'harbour:category':'seaport'}, //  Count: 7136
        "sea port":{'harbour:category':'seaport'}, //  Count: 7136
        "port":{'landuse':'port'}, //  Count: 7136
        "teleferic":{'aerialway':'cable_car'}, //  Count: 7073
        "aerial tramway/ ropeway/ gondola lift/ skilift / cable car station (transport in the air)":{'aerialway':'station'}, //  Count: 7073
        "cableway":{'aerialway':'cableway'}, //  Count: 7073
        "child care":{'amenity':'childcare'}, //  Count: 7053
        "day care":{'amenity':'childcare'}, //  Count: 7053
        "creche":{'amenity':'childcare'}, //  Count: 7053
        "motorcycle":{'rawWmCategory':'motorcycle'}, //  Count: 7046
        "flower shop / florist":{'shop':'florist'}, //  Count: 7045
        "motel":{'tourism':'motel'}, //  Count: 7021
        "insurance company":{'office':'insurance'}, //  Count: 7006
        "culvert":{'tunnel':'culvert'}, //  Count: 6995
// // // //         "society":{'rawWmCategory':'society'}, //  Count: 6988 //
        "NRHP - National Register of Historic Places":{'rawWmCategory':'NRHP_-_National_Register_of_Historic_Places'}, //  Count: 6971
        "inn":{'amenity':'pub'}, //  Count: 6933
        "pedestrian crossing":{'highway':'crossing','crossing':'unknown'}, //  Count: 6915
        "zebra crossing / pelican crossing":{'highway':'crossing','crossing':'uncontrolled'}, //  Count: 6915
        "crosswalk":{'highway':'crossing'}, //  Count: 6915
        "[military branch]":{'rawWmCategory':'[military_branch]'}, //  Count: 6883
        "Russian Orthodox Church":{'religion':'christian','denomination':'russian_orthodox'}, //  Count: 6881
        "shelter":{'amenity':'shelter'}, //  Count: 6794
        "historic landmark":{'historic':'landmark'}, //  Count: 6720
        "department store":{'shop':'department_store'}, //  Count: 6718
        "memorial plaque":{'historic':'menorial','menorial:type':'plaque'}, //  Count: 6710
        "aid to navigation - aviation":{'aeroway':'aid_to_navigation'}, //  Count: 6689
        "lookout tower":{'man_made':'tower','tower':'observation'}, //  Count: 6668
        "1970s construction":{'rawWmCategory':'1970s_construction'}, //  Count: 6661
        "homestead":{'landuse':'homestead'}, //  Count: 6567
        "institute":{'amenity':'institute'}, //  Count: 6543
        "railway platform":{'public_transport':'platform','railway':'platform','train':'yes'}, //  Count: 6530
        "sports complex":{'leisure':'sports_complex'}, //  Count: 6507
        "sportsplex":{'leisure':'sports_complex'}, //  Count: 6507
        "artillery battery":{'military':'battery'}, //  Count: 6497
        "notable event":{'rawWmCategory':'notable_event'}, //  Count: 6413
        "bicycle / bike":{'rawWmCategory':'bicycle_/_bike'}, //  Count: 6353
        "falls":{'waterway':'waterfall'}, //  Count: 6301
        "waterfall":{'waterway':'waterfall'}, //  Count: 6301
        "entrance":{'entrance':'yes'}, //  Count: 6291
        "college of further education / higher education":{'amenity':'school','isced:level':'4'}, //  Count: 6277
        "cegep":{'amenity':'school','isced:level':'4'}, //  Count: 6277
        "post-secondary education":{'amenity':'school','isced:level':'4'}, //  Count: 6277
        "tertiary education":{'amenity':'school','isced:level':'5'}, //  Count: 6277
        "town district":{'place':'suburb'}, //  Count: 6245
        "city district":{'place':'suburb'}, //  Count: 6245
        "detached house":{'building':'detached'}, //  Count: 6238
        "cape (geography)":{'natural':'cape'}, //  Count: 6224
        "entertainment attraction / ride":{'tourism':'attraction'}, //  Count: 6223
        "district heating substation":{'rawWmCategory':'district_heating_substation'}, //  Count: 6200
        "advertising":{'advertising':'yes'}, //  Count: 6190
        "veterinarian":{'amenity':'veterinary'}, //  Count: 6180
        "veterinary  clinic / surgery":{'amenity':'veterinary'}, //  Count: 6180
        "animal hospital":{'amenity':'veterinary'}, //  Count: 6180
        "19th century construction":{'rawWmCategory':'19th_century_construction'}, //  Count: 6178
        "ice cream parlor":{'amenity':'ice_cream'}, //  Count: 6158
        "frozen yogurt shop":{'amenity':'ice_cream'}, //  Count: 6158
        "gelato parlor":{'amenity':'ice_cream'}, //  Count: 6158
        "housing services":{'office':'housing_services'}, //  Count: 6147
        "1950s construction":{'rawWmCategory':'1950s_construction'}, //  Count: 6039
        "farmhouse":{'building':'farm'}, //  Count: 6003
        "bunker":{'military':'bunker'}, //  Count: 5992
        "water well":{'man_made':'water_well'}, //  Count: 5947
        "artesian well":{'man_made':'water_well','well:type':'artesian'}, //  Count: 5947
        "viewpoint":{'tourism':'viewpoint'}, //  Count: 5939
        "panoramic view":{'tourism':'viewpoint'}, //  Count: 5939
        "travel plaza":{'highway':'services'}, //  Count: 5917
        "service area":{'highway':'services'}, //  Count: 5917
        "rest stop":{'highway':'rest_area'}, //  Count: 5917
        "motorway service area":{'highway':'services'}, //  Count: 5917
        "rest area":{'highway':'rest_area'}, //  Count: 5917
        "computers":{'rawWmCategory':'computers'}, //  Count: 5916
        "computer":{'rawWmCategory':'computer'}, //  Count: 5916
        "wholesale business":{'whop':'wholesale'}, //  Count: 5913
        "wholesalers":{'shop':'wholesale'}, //  Count: 5913
        "commercial":{'building':'commercial'}, //  Count: 5890
        "barracks":{'military':'barracks'}, //  Count: 5866
        "public vehicle depot":{'landuse':'depot'}, //  Count: 5840
        "cars - to be replaced":{'rawWmCategory':'cars_-_to_be_replaced'}, //  Count: 5793
        "canal":{'waterway':'canal'}, //  Count: 5779
        "healthcare":{'amenity':'healthcare'}, //  Count: 5775
        "scientific research institute / centre":{'amenity':'research_institute'}, //  Count: 5743
        "headquarters":{'rawWmCategory':'headquarters'}, //  Count: 5727
        "head office":{'rawWmCategory':'head_office'}, //  Count: 5727
        "main office":{'rawWmCategory':'main_office'}, //  Count: 5727
        "grave":{'cemetery':'grave'}, //  Count: 5684
        "amusement park":{'tourism':'theme_park'}, //  Count: 5681
        "theme park":{'tourism':'theme_park'}, //  Count: 5681
        "underground facility":{'location':'underground','facility':'yes'}, //  Count: 5651
        "disaster":{'rawWmCategory':'disaster'}, //  Count: 5583
        "construction site":{'landuse':'construction'}, //  Count: 5580
        "tomb":{'historic':'tomb'}, //  Count: 5579
        "rocks":{'natural':'rocks'}, //  Count: 5578
        "forest belt":{'natural':'forest_belt'}, //  Count: 5496
        "stairs":{'highway':'steps'}, //  Count: 5486
        "spa":{'amenity':'spa'}, //  Count: 5485
        "radio tower":{'man_made':'tower','communication:radio':'yes'}, //  Count: 5456
        "Jehovah's Witnesses":{'religion':'christian','denomination':'jehovahs_witness'}, //  Count: 5442
        "war cemetery":{'rawWmCategory':'war_cemetery'}, //  Count: 5425
        "office":{'office':'yes'}, //  Count: 5410
        "landmark":{'landmark':'yes'}, //  Count: 5410
        "landmarks":{'landmark':'yes'}, //  Count: 5410
        "water tank":{'man_made':'water_tank'}, //  Count: 5408
        "1980s construction":{'rawWmCategory':'1980s_construction'}, //  Count: 5304
        "art museum / art gallery":{'tourism':'gallery'}, //  Count: 5255
        "portrait gallery":{'tourism':'gallery'}, //  Count: 5255
        "coffee - to be replaced":{'rawWmCategory':'coffee_-_to_be_replaced'}, //  Count: 5238
        "public institution":{'rawWmCategory':'public_institution'}, //  Count: 5232
        "neighbourhood development":{'rawWmCategory':'neighbourhood_development'}, //  Count: 5221
        "nuraghe":{'historic':'archaeological_site','site_type':'megalith','megalith_type':'nuraghe'}, //  Count: 5191
        "wedding / marriage garden":{'leisure':'garden','wedding':'yes'}, //  Count: 5188
        "matrimonial hall / venue":{'amenity':'events_venue'}, //  Count: 5188
        "wedding / marriage venue":{'amenity':'events_venue'}, //  Count: 5188
        "wedding / marriage hall":{'amenity':'events_venue'}, //  Count: 5188
        "ship":{'rawWmCategory':'ship'}, //  Count: 5150
        "boat":{'rawWmCategory':'boat'}, //  Count: 5134
        "open-air swimming pool":{'leisure':'swimming_pool'}, //  Count: 5130
        "outdoor swimming pool":{'leisure':'swimming_pool'}, //  Count: 5130
        "recycling centre / collection":{'amenity':'recycling'}, //  Count: 5107
        "branch office of Jehovah's Witnesses":{'amenity':'place_of_worship','religion':'christian','denomination':'jehovahs_witness'}, //  Count: 5098
        "Kingdom Hall of Jehovah's Witnesses":{'amenity':'place_of_worship','religion':'christian','denomination':'jehovahs_witness'}, //  Count: 5098
        "assembly hall of Jehovah's Witnesses":{'amenity':'place_of_worship','religion':'christian','denomination':'jehovahs_witness'}, //  Count: 5098
        "gift shop":{'shop':'gift'}, //  Count: 5080
        "souvenir shop":{'shop':'gift'}, //  Count: 5080
        "cottage complex":{'rawWmCategory':'cottage_complex'}, //  Count: 5076
        "brickworks":{'industrial':'brickworks'}, //  Count: 5072
        "brick factory":{'industrial':'brickworks'}, //  Count: 5072
        "brick kiln":{'man_made':'kiln'}, //  Count: 5072
        "brickyard":{'industrial':'brickyard'}, //  Count: 5072
        "lawyer":{'office':'lawyer'}, //  Count: 5048
        "attorney":{'office':'lawyer'}, //  Count: 5048
        "attorneys":{'office':'lawyer'}, //  Count: 5048
        "solicitors":{'office':'lawyer'}, //  Count: 5048
        "law firm":{'office':'lawyer'}, //  Count: 5048
        "telephone kiosk":{'building':'kiosk','amenity':'telephone'}, //  Count: 5019
        "phone box":{'amenity':'telephone'}, //  Count: 5019
        "public telephone":{'amenity':'telephone'}, //  Count: 5019
        "payphone":{'amenity':'telephone'}, //  Count: 5019
        "phone booth":{'amenity':'telephone'}, //  Count: 5019
        "management":{'rawWmCategory':'management'}, //  Count: 5012
        "optician":{'shop':'optician'}, //  Count: 4996
        "spectacles":{'shop':'optician'}, //  Count: 4996
        "contact lens store":{'shop':'optician'}, //  Count: 4996
        "optical retailer":{'shop':'optician'}, //  Count: 4996
        "eye care shop":{'shop':'optician'}, //  Count: 4996
        "border":{'border':'yes'}, //  Count: 4977
        "doctor":{'amenity':'doctors'}, //  Count: 4976
        "tourism":{'tourism':'yes'}, //  Count: 4969
        "unincorporated area / community":{'place':'unincorporated_area'}, //  Count: 4955
        "open 24 hours":{'rawWmCategory':'open_24_hours'}, //  Count: 4951
        "telephone exchange building":{'man_made':'telephone_exchange'}, //  Count: 4943
        "charitable organization":{'office':'charity'}, //  Count: 4914
        "charitable organisation":{'office':'charity'}, //  Count: 4914
        "social club":{'leisure':'club'}, //  Count: 4907
        "strip mall":{'shop':'mall'}, //  Count: 4902
        "Électricité de France":{'rawWmCategory':'Électricité_de_France'}, //  Count: 4902
        "shopping plaza":{'landuse':'plaza'}, //  Count: 4902
        "mini-mall":{'shop':'mall'}, //  Count: 4902
        "gun emplacement":{'military':'gun_emplacement'}, //  Count: 4901
        "helipad":{'aeroway':'helipad'}, //  Count: 4874
        "helicopter landing pad":{'aeroway':'helipad'}, //  Count: 4874
        "reef":{'natural':'reef'}, //  Count: 4866
        "manufacturing":{'industrial':'manufacturing'}, //  Count: 4862
        "manufacturer":{'industrial':'manufacturing'}, //  Count: 4862
        "destroyed":{'condition':'destroyed'}, //  Count: 4838
        "Baptist church":{'religion':'christian','denomination':'baptist'}, //  Count: 4810
        "extreme / adventure sport":{'tourism':'outdoor_adventure'}, //  Count: 4810
        "ranch":{'landuse':'farm'}, //  Count: 4775
        "heliport":{'aeroway':'heliport'}, //  Count: 4774
        "pillbox":{'military':'bunker','bunker_type':'pillbox'}, //  Count: 4768
        "used car dealer / dealership":{'shop':'car'}, //  Count: 4753
        "cricket oval":{'leisure':'pitch','sport':'cricket'}, //  Count: 4693
        "cricket field":{'leisure':'pitch','sport':'cricket'}, //  Count: 4693
        "cricket ground":{'leisure':'pitch','sport':'cricket'}, //  Count: 4693
        "plot - to be deleted":{'rawWmCategory':'plot_-_to_be_deleted'}, //  Count: 4673
        "electronic components":{'shop':'electronics'}, //  Count: 4665
        "electronics":{'shop':'electronics'}, //  Count: 4665
        "public housing estate":{'landuse':'public_housing_estate'}, //  Count: 4645
        "tree":{'natural':'tree'}, //  Count: 4624
        "horseback riding":{'leisure':'equestrian'}, //  Count: 4602
        "horse riding":{'leisure':'equestrian'}, //  Count: 4602
        "pavilion":{'building':'pavilion'}, //  Count: 4602
        "equitation":{'leisure':'equestrian'}, //  Count: 4602
        "equestrianism":{'leisure':'equestrian'}, //  Count: 4602
        "ordnance depot":{'military':'ordnance_depot'}, //  Count: 4584
        "arsenal / weapon and ammunition storage":{'military':'ammunition'}, //  Count: 4584
        "gas distribution station":{'amenity':'gas_distribution_station'}, //  Count: 4559
        "2000s construction":{'rawWmCategory':'2000s_construction'}, //  Count: 4558
        "township":{'place':'township'}, //  Count: 4553
        "unfinished / unbuilt":{'rawWmCategory':'unfinished_/_unbuilt'}, //  Count: 4506
        "underpass":{'rawWmCategory':'underpass'}, //  Count: 4489
        "marinas":{'leisure':'marina'}, //  Count: 4485
        "marina":{'leisure':'marina'}, //  Count: 4485
        "chocolatier":{'shop':'confectionery'}, //  Count: 4467
        "confectionery":{'shop':'confectionery'}, //  Count: 4467
        "sweets / sweet shop":{'shop':'confectionery'}, //  Count: 4467
        "candy shop":{'shop':'confectionery'}, //  Count: 4467
        "palm house":{'rawWmCategory':'palm_house'}, //  Count: 4429
        "greenhouse":{'building':'greenhouse'}, //  Count: 4429
        "glasshouse":{'building':'greenhouse'}, //  Count: 4429
        "berth":{'seamark:type':'berth'}, //  Count: 4374
        "radio station":{'amenity':'radio'}, //  Count: 4370
        "ventilating shaft":{'rawWmCategory':'ventilating_shaft'}, //  Count: 4366
        "social security":{'office':'social_security'}, //  Count: 4366
        "pet shop/store":{'shop':'pet'}, //  Count: 4364
        "suburb":{'place':'suburb'}, //  Count: 4360
        "zoological garden":{'tourism':'zoo'}, //  Count: 4353
        "zoo":{'tourism':'zoo'}, //  Count: 4353
        "lutheranism":{'religion':'christian','denomination':'lutheran'}, //  Count: 4344
        "mountain pass":{'mountain_pass':'yes'}, //  Count: 4342
        "navy":{'military:service':'navy'}, //  Count: 4339
        "toll booth":{'barrier':'toll_booth'}, //  Count: 4331
        "toll gate":{'barrier':'toll_booth'}, //  Count: 4331
        "closed":{'rawWmCategory':'closed'}, //  Count: 4323
        "civil defense facility":{'military':'civil_defense_facility'}, //  Count: 4320
        "civil protection":{'amenity':'civil_protection'}, //  Count: 4320
        "perfumes":{'shop':'perfumery'}, //  Count: 4313
        "perfume / cosmetics shop":{'shop':'cosmetics'}, //  Count: 4313
        "cosmetics":{'shop':'cosmetics'}, //  Count: 4313
        "cultural center / centre":{'amenity':'arts_centre'}, //  Count: 4305
        "driving school":{'amenity':'driving_school'}, //  Count: 4274
        "shipbuilding":{'rawWmCategory':'shipbuilding'}, //  Count: 4272
        "Mediterranean cuisine":{'cuisine':'mediterranean'}, //  Count: 4258
        "frazione":{'place':'village'}, //  Count: 4254
        "flower bed":{'man_made':'flower_bed'}, //  Count: 4236
        "barge slip":{'amenity':'barge_slip'}, //  Count: 4233
        "ferry terminal":{'amenity':'ferry_terminal'}, //  Count: 4233
        "ferry slip":{'amenity':'ferry_slip'}, //  Count: 4233
        "cavern":{'rawWmCategory':'cavern'}, //  Count: 4230
        "cave":{'rawWmCategory':'cave'}, //  Count: 4230
        "aparthotel":{'tourism':'apartment'}, //  Count: 4224
        "apartment hotel / serviced apartment":{'tourism':'apartment'}, //  Count: 4224
        "apartelle":{'tourism':'apartment'}, //  Count: 4224
        "crash site":{'place':'crash_site'}, //  Count: 4221
        "sanatorium":{'amenity':'sanatorium'}, //  Count: 4220
        "computer services":{'amenity':'computer_services'}, //  Count: 4210
        "ministry / government department":{'office':'government'}, //  Count: 4200
        "hypermarket":{'shop':'hypermarket'}, //  Count: 4169
        "superstore":{'shop':'hypermarket'}, //  Count: 4169
        "megastore":{'shop':'hypermarket'}, //  Count: 4169
        "supercenter":{'shop':'hypermarket'}, //  Count: 4169
        "big-box store":{'shop':'hypermarket'}, //  Count: 4169
        "butcher's shop":{'shop':'butcher'}, //  Count: 4168
        "butcher shop / butchery":{'shop':'butcher'}, //  Count: 4168
        "department":{'rawWmCategory':'department'}, //  Count: 4165
        "christian school":{'amenity':'school','religion':'christian'}, //  Count: 4128
        "hydroelectric power station":{'power':'generator','generator:source':'hydro'}, //  Count: 4120
        "gully":{'natural':'gully'}, //  Count: 4091
        "baseball field":{'leisure':'pitch','sport':'baseball'}, //  Count: 4090
        "Walmart":{'shop':'department_store','name':'Walmart'}, //  Count: 4086
        "Wal-mart":{'shop':'department_store','name':'Walmart'}, //  Count: 4086
        "jewellery store/shop":{'shop':'jewelry'}, //  Count: 4082
        "jewelry store/shop":{'shop':'jewelry'}, //  Count: 4082
        "lock (water transport)":{'lock':'yes'}, //  Count: 4028
        "aircraft shelter":{'aeroway':'aircraft_shelter'}, //  Count: 4010
        "aircraft hangar":{'aeroway':'hangar'}, //  Count: 4010
        "sector":{'rawWmCategory':'sector'}, //  Count: 4005
        "coal industry":{'rawWmCategory':'coal_industry'}, //  Count: 3995
        "interstate":{'highway':'motorway'}, //  Count: 3981
        "1990s construction":{'rawWmCategory':'1990s_construction'}, //  Count: 3968
        "photographic studio":{'craft':'photographer'}, //  Count: 3953
        "Japanese cuisine":{'cuisine':'japanese'}, //  Count: 3936
        "volleyball court":{'leisure':'pitch','sport':'volleyball'}, //  Count: 3936
        "pier":{'man_made':'pier'}, //  Count: 3933
        "Lutheran Church":{'building':'church','amenity':'place_of_worship','religion':'christian','denomination':'lutheran'}, //  Count: 3931
        "eid prayer ground":{'amenity':'place_of_worship','religion':'muslim','eidgah':'yes'}, //  Count: 3928
        "eidgah":{'amenity':'place_of_worship','religion':'muslim','eidgah':'yes'}, //  Count: 3928
        "laundromat":{'shop':'laundry'}, //  Count: 3910
        "laundry":{'shop':'laundry'}, //  Count: 3910
        "laundrette":{'shop':'laundry'}, //  Count: 3910
        "toy shop":{'shop':'toy'}, //  Count: 3893
        "bungalow":{'building':'bungalow'}, //  Count: 3882
        "association":{'rawWmCategory':'association'}, //  Count: 3866
        "engineering":{'rawWmCategory':'engineering'}, //  Count: 3865
        "vehicle inspection":{'amenity':'vehicle_inspection'}, //  Count: 3864
        "valley":{'natural':'valley'}, //  Count: 3863
        "summer camp":{'leisure':'summer_camp'}, //  Count: 3853
        "river mouth":{'waterway':'river_mouth'}, //  Count: 3851
        "estuary":{'estuary':'yes'}, //  Count: 3851
        "lodge":{'rawWmCategory':'lodge'}, //  Count: 3824
        "professional sports leagues":{'rawWmCategory':'professional_sports_leagues'}, //  Count: 3803
        "antenna":{'man_made':'antenna'}, //  Count: 3791
        "retirement home":{'amenity':'retirement_home'}, //  Count: 3790
        "senior citizen home":{'amenity':'retirement_home'}, //  Count: 3790
        "ICBM launch facility":{'missile':'intercontinental_ballistic_missile','launch_pad':'yes'}, //  Count: 3784
        "missile launch facility":{'launch_pad':'yes'}, //  Count: 3784
        "missile silo":{'missile_silo':'yes'}, //  Count: 3784
        "churchyard":{'landuse':'churchyard'}, //  Count: 3775
        "[oil and gas companies]":{'rawWmCategory':'[oil_and_gas_companies]'}, //  Count: 3760
        "cattle ranch":{'landuse':'farmland','produce':'cattle'}, //  Count: 3746
        "cattle farm":{'landuse':'farmland','produce':'cattle'}, //  Count: 3746
        "camp":{'tourism':'camp_site'}, //  Count: 3742
        "bottle bank":{'rawWmCategory':'bottle_bank'}, //  Count: 3737
        "waste collection point / civic amenity centre / drop-off centre":{'amenity':'waste_transfer_station'}, //  Count: 3737
        "railway company":{'rawWmCategory':'railway_company'}, //  Count: 3733
        "railway operator":{'rawWmCategory':'railway_operator'}, //  Count: 3733
        "care home":{'amenity':'nursing_home'}, //  Count: 3730
        "nursing home":{'amenity':'nursing_home'}, //  Count: 3730
        "alternative medicine":{'rawWmCategory':'alternative_medicine'}, //  Count: 3728
        "bus depot":{'landuse':'depot','bus':'yes'}, //  Count: 3719
        "bus garage":{'building':'garage','bus':'yes'}, //  Count: 3719
        "ice arena":{'leisure':'ice_rink'}, //  Count: 3707
        "ice skating rink":{'leisure':'ice_rink'}, //  Count: 3707
        "ice hockey rink":{'leisure':'ice_rink','sport':'ice_hockey'}, //  Count: 3707
        "ice rink":{'leisure':'ice_rink'}, //  Count: 3707
        "mound":{'natural':'mound'}, //  Count: 3678
        "outbuilding":{'building':'outbuilding'}, //  Count: 3667
        "cross":{'rawWmCategory':'cross'}, //  Count: 3665
        "wall":{'barrier':'wall'}, //  Count: 3650
        "newspaper publisher":{'office':'newspaper'}, //  Count: 3644
        "bake shop":{'shop':'bakery'}, //  Count: 3640
        "patisserie":{'shop':'pastry'}, //  Count: 3640
        "pastry shop":{'shop':'pastry'}, //  Count: 3640
        "printing":{'amenity':'printing'}, //  Count: 3636
        "domestic dogs":{'rawWmCategory':'domestic_dogs'}, //  Count: 3628
        "border guard":{'rawWmCategory':'border_guard'}, //  Count: 3623
        "landfill site":{'landuse':'landfill'}, //  Count: 3618
        "volcano":{'natural':'volcano'}, //  Count: 3617
        "gazebo":{'building':'gazebo'}, //  Count: 3614
        "heritage":{'rawWmCategory':'heritage'}, //  Count: 3606
        "drive-in/ drive-through":{'drive_through':'yes'}, //  Count: 3597
        "Chinese cuisine":{'cuisine':'chinese'}, //  Count: 3590
        "drinking fountain":{'amenity':'drinking_water'}, //  Count: 3585
        "water pump":{'man_made':'water_pump'}, //  Count: 3585
        "civil parish":{'place':'parish'}, //  Count: 3582
        "parish":{'place':'parish'}, //  Count: 3582
        "TV":{'rawWmCategory':'TV'}, //  Count: 3582
        "television":{'rawWmCategory':'television'}, //  Count: 3582
        "training center":{'amenity':'training'}, //  Count: 3571
        "training centre":{'amenity':'training'}, //  Count: 3571
        "snack bar / concession stand":{'amenity':'fast_food'}, //  Count: 3553
        "CDP - Census Designated Place":{'place':'census_designated_place'}, //  Count: 3549
        "highway patrol":{'amenity':'police','police':'highway_patrol'}, //  Count: 3546
        "traffic police":{'amenity':'police','police':'traffic_police'}, //  Count: 3546
        "grain elevator / grain silos":{'man_made':'silo'}, //  Count: 3528
        "billboard":{'advertising':'billboard'}, //  Count: 3517
        "music school":{'amenity':'music_school'}, //  Count: 3495
        "shrine":{'amenity':'place_of_worship','building':'shrine'}, //  Count: 3488
        "Shiv / Mahadev mandir":{'amenity':'place_of_worship','religion':'hindu','building':'temple'}, //  Count: 3480
        "Shankar temple":{'amenity':'place_of_worship','religion':'hindu','building':'temple'}, //  Count: 3480
        "Sivalayam":{'amenity':'place_of_worship','religion':'hindu','building':'temple'}, //  Count: 3480
        "Shiva temple":{'amenity':'place_of_worship','religion':'hindu','building':'temple'}, //  Count: 3480
        "2010s construction":{'rawWmCategory':'2010s_construction'}, //  Count: 3462
        "electricity - to be replaced / deleted":{'rawWmCategory':'electricity_-_to_be_replaced_/_deleted'}, //  Count: 3445
        "demolished":{'demolished':'yes'}, //  Count: 3443
        "computer shop":{'shop':'computer'}, //  Count: 3440
        "orchard / fruit garden":{'landuse':'orchard'}, //  Count: 3436
        "fruit farm":{'landuse':'orchard'}, //  Count: 3436
        "second-level administrative division":{'rawWmCategory':'second-level_administrative_division'}, //  Count: 3429
        "boarding school":{'amenity':'school','boarding':'yes'}, //  Count: 3411
        "hall (Don't use this category, instead help removing it by refining)":{'rawWmCategory':'hall_(Dont_use_this_category,_instead_help_removing_it_by_refining)'}, //  Count: 3402
        "quarry pond":{'rawWmCategory':'quarry_pond'}, //  Count: 3389
        "evangelicalism":{'religion':'christian','denomination':'evangelical'}, //  Count: 3384
        "well_(do not use this category)":{'rawWmCategory':'well_(do_not_use_this_category)'}, //  Count: 3370
        "geodesy":{'rawWmCategory':'geodesy'}, //  Count: 3359
        "geodetic point":{'man_made':'survey_point'}, //  Count: 3359
        "research":{'amenity':'research'}, //  Count: 3358
        "dump":{'rawWmCategory':'dump'}, //  Count: 3356
        "dacha":{'building':'bungalow'}, //  Count: 3354
        "cathedral":{'amenity':'place_of_worship','building':'cathedral'}, //  Count: 3340
        "arts centre":{'amenity':'arts_centre'}, //  Count: 3299
        "arts center":{'amenity':'arts_centre'}, //  Count: 3299
        "poultry farm":{'landuse':'farmland','produce':'poultry'}, //  Count: 3292
        "poultry farming":{'landuse':'farmland','produce':'poultry'}, //  Count: 3292
        "stationers":{'shop':'stationery'}, //  Count: 3291
        "stationery store / shop":{'shop':'stationery'}, //  Count: 3291
        "service center":{'rawWmCategory':'service_center'}, //  Count: 3283
        "chowk (town square)":{'landuse':'square'}, //  Count: 3272
        "town square":{'landuse':'square'}, //  Count: 3272
        "townhouses":{'building':'terrace'}, //  Count: 3263
        "townhomes":{'building':'terrace'}, //  Count: 3263
        "distribution center":{'building':'warehouse'}, //  Count: 3262
        "distribution centre":{'building':'warehouse'}, //  Count: 3262
        "distributor":{'rawWmCategory':'distributor'}, //  Count: 3262
        "architecture - Do not use this category":{'rawWmCategory':'architecture_-_Do_not_use_this_category'}, //  Count: 3256
        "boarding house":{'tourism':'guest_house'}, //  Count: 3252
        "bell chair":{'rawWmCategory':'bell_chair'}, //  Count: 3230
        "bell tower":{'man_made':'tower','tower:type':'bell_tower'}, //  Count: 3230
        "belfry":{'man_made':'tower','tower:type':'bell_tower','bell_tower':'belfry'}, //  Count: 3230
        "shanty town":{'place':'shanty_town'}, //  Count: 3203
        "slum":{'place':'slum'}, //  Count: 3203
        "lagoon":{'water':'lagoon'}, //  Count: 3179
        "Maginot line":{'historic':'Maginot_line'}, //  Count: 3151
        "entertainment":{'amenity':'entertainment'}, //  Count: 3147
        "vineyard / grape plantation":{'landuse':'vineyard'}, //  Count: 3139
        "judaism":{'religion':'jewish'}, //  Count: 3093
        "station building (railway)":{'building':'train_station'}, //  Count: 3091
        "head house (train station)":{'rawWmCategory':'head_house_(train_station)'}, //  Count: 3091
        "cab rank":{'amenity':'taxi'}, //  Count: 3090
        "taxi rank":{'amenity':'taxi'}, //  Count: 3090
        "taxi stop":{'amenity':'taxi'}, //  Count: 3090
        "hack stand":{'amenity':'taxi'}, //  Count: 3090
        "cab stand":{'amenity':'taxi'}, //  Count: 3090
        "taxicab stand":{'amenity':'taxi'}, //  Count: 3090
        "taxi stand":{'amenity':'taxi'}, //  Count: 3090
        "lawn":{'rawWmCategory':'lawn'}, //  Count: 3087
        "geography":{'rawWmCategory':'geography'}, //  Count: 3075
        "geographical":{'rawWmCategory':'geographical'}, //  Count: 3075
        "golf hole":{'leisure':'golf_course','golf':'hole'}, //  Count: 3043
        "Mexican cuisine":{'cuisine':'mexican'}, //  Count: 3040
        "viaduct (bridge construction style)":{'bridge':'viaduct'}, //  Count: 3025
        "internet company":{'office':'internet_company'}, //  Count: 3020
        "sports school":{'amenity':'school','sports':'yes'}, //  Count: 3011
        "ancient ruins":{'historic':'ruins'}, //  Count: 3000
        "historic ruins":{'historic':'ruins'}, //  Count: 3000
        "ambulance station":{'emergency':'ambulance_station'}, //  Count: 2997
        "ship wreck":{'historic':'wreck'}, //  Count: 2986
        "shipwreck":{'historic':'wreck'}, //  Count: 2986
        "classroom":{'amenity':'school','building':'school'}, //  Count: 2982
        "classrooms":{'amenity':'school','building':'school'}, //  Count: 2982
        "Catholic school":{'amenity':'school','religion':'catholic'}, //  Count: 2965
        "futsal court":{'leisure':'pitch','sport':'futsal'}, //  Count: 2963
        "tourist information board":{'tourism':'information','information':'board'}, //  Count: 2945
        "tourist information centre/center":{'tourism':'information','information':'office'}, //  Count: 2945
        "Italian cuisine":{'cuisine':'italian'}, //  Count: 2943
        "cargo transportation":{'rawWmCategory':'cargo_transportation'}, //  Count: 2915
        "premises":{'rawWmCategory':'premises'}, //  Count: 2913
        "taxiway":{'aeroway':'taxiway'}, //  Count: 2911
        "fallout shelter":{'amenity':'shelter','shelter:type':'fallout'}, //  Count: 2904
        "bombshelter":{'amenity':'shelter','shelter:type':'bomb'}, //  Count: 2904
        "country club":{'rawWmCategory':'country_club'}, //  Count: 2900
        "ticket / box office":{'shop':'ticket'}, //  Count: 2899
        "Sberbank":{'amenity':'bank','name':'Sberbank'}, //  Count: 2895
        "barn":{'building':'barn'}, //  Count: 2889
        "pawnshop / pawnbroker":{'shop':'pawnbroker'}, //  Count: 2884
        "airport terminal":{'rawWmCategory':'airport_terminal'}, //  Count: 2883
        "motorcycle dealer / shop":{'shop':'motorcycle'}, //  Count: 2873
        "picnic area":{'tourism':'picnic_site'}, //  Count: 2873
        "camera shop":{'shop':'camera'}, //  Count: 2864
        "photography shop":{'shop':'photo'}, //  Count: 2864
        "garden center":{'shop':'garden_centre'}, //  Count: 2861
        "garden centre":{'shop':'garden_centre'}, //  Count: 2861
        "non-profit organisation":{'rawWmCategory':'non-profit_organisation'}, //  Count: 2856
        "non-profit organization":{'rawWmCategory':'non-profit_organization'}, //  Count: 2856
        "accommodation":{'amenity':'accommodation'}, //  Count: 2854
        "closed - military":{'access':'no'}, //  Count: 2849
        "consultation":{'rawWmCategory':'consultation'}, //  Count: 2841
        "consultancy":{'rawWmCategory':'consultancy'}, //  Count: 2841
        "consulting":{'rawWmCategory':'consulting'}, //  Count: 2841
        "silo (structure for storing bulk materials)":{'man_made':'silo'}, //  Count: 2837
        "bowling alley":{'leisure':'bowling_alley'}, //  Count: 2820
        "bowling lanes":{'leisure':'bowling_alley'}, //  Count: 2820
        "bowling (pin)":{'leisure':'bowling_alley'}, //  Count: 2820
        "boliche":{'sport':'bowling'}, //  Count: 2820
        "water catchment area":{'rawWmCategory':'water_catchment_area'}, //  Count: 2810
        "wind power plant":{'rawWmCategory':'wind_power_plant'}, //  Count: 2767
        "wind farm":{'landuse':'wind_farm'}, //  Count: 2767
        "fuel depot":{'amenity':'fuel_storage'}, //  Count: 2760
        "oil terminal":{'rawWmCategory':'oil_terminal'}, //  Count: 2760
        "oil depot":{'rawWmCategory':'oil_depot'}, //  Count: 2760
        "tank farm":{'landuse':'tank_farm'}, //  Count: 2760
        "[military base]":{'landuse':'military'}, //  Count: 2758
        "base level":{'rawWmCategory':'base_level'}, //  Count: 2758
        "windmill":{'man_made':'windmill'}, //  Count: 2756
        "non-ministerial government department (UK)":{'office':'government'}, //  Count: 2752
        "convention center":{'amenity':'convention_centre'}, //  Count: 2750
        "convention centre":{'amenity':'convention_centre'}, //  Count: 2750
        "congress centre":{'rawWmCategory':'congress_centre'}, //  Count: 2750
        "conference centre":{'amenity':'conference_centre'}, //  Count: 2750
        "water purification plant":{'man_made':'water_works'}, //  Count: 2747
        "water supply treatment plant":{'man_made':'water_works'}, //  Count: 2747
        "cooling tower":{'man_made':'tower','tower:type':'cooling'}, //  Count: 2744
        "HM revenue and customs (UK)":{'amenity':'customs'}, //  Count: 2742
        "internal revenue service":{'office':'tax'}, //  Count: 2742
        "tax authority":{'office':'tax'}, //  Count: 2742
        "sauna":{'leisure':'sauna'}, //  Count: 2732
        "village hall":{'amenity':'townhall','townhall:type':'village'}, //  Count: 2709
        "[hotel chains]":{'tourism':'hotel'}, //  Count: 2699
        "ford (river crossing)":{'ford':'yes'}, //  Count: 2693
        "auditorium":{'amenity':'auditorium'}, //  Count: 2688
        "compound":{'rawWmCategory':'compound'}, //  Count: 2683
        "local events":{'rawWmCategory':'local_events'}, //  Count: 2674
        "winery":{'man_made':'winery'}, //  Count: 2672
        "atelier":{'building':'workshop'}, //  Count: 2662
        "mini storage":{'shop':'storage_rental'}, //  Count: 2661
        "self storage":{'shop':'storage_rental'}, //  Count: 2661
        "chemical industry":{'rawWmCategory':'chemical_industry'}, //  Count: 2657
        "nursery":{'rawWmCategory':'nursery'}, //  Count: 2657
        "snooker hall / room / club / parlour":{'sport':'billiards'}, //  Count: 2655
        "pool hall / room / parlor / club":{'sport':'billiards'}, //  Count: 2655
        "billiard hall / room / club / parlour":{'sport':'billiards'}, //  Count: 2655
        "parking ramp":{'amenity':'parking'}, //  Count: 2653
        "parking garage":{'amenity':'parking'}, //  Count: 2653
        "multi-storey car park":{'amenity':'parking','parking':'multi-storey'}, //  Count: 2653
        "18th century construction":{'rawWmCategory':'18th_century_construction'}, //  Count: 2649
        "employment / recruitment / jobs agency":{'office':'employment_agency'}, //  Count: 2643
        "staffing agency":{'office':'employment_agency'}, //  Count: 2643
        "brewery":{'building':'brewery'}, //  Count: 2643
        "private school":{'amenity':'school','private':'yes'}, //  Count: 2642
        "nonstate school":{'amenity':'school'}, //  Count: 2642
        "independent school":{'amenity':'school'}, //  Count: 2642
        "school of arts and design / college of arts and design":{'rawWmCategory':'school_of_arts_and_design_/_college_of_arts_and_design'}, //  Count: 2630
        "cove":{'natural':'cove'}, //  Count: 2597
        "ski":{'sport':'ski'}, //  Count: 2556
        "height":{'rawWmCategory':'height'}, //  Count: 2553
        "home appliance store / shop":{'shop':'houseware'}, //  Count: 2549
        "rifle range":{'sport':'firing_range'}, //  Count: 2532
        "shooting range":{'sport':'firing_range'}, //  Count: 2532
        "firing range":{'sport':'firing_range'}, //  Count: 2532
        "Lenin":{'rawWmCategory':'Lenin'}, //  Count: 2530
        "bust (sculpture)":{'tourism':'artwork','artwork_type':'bust'}, //  Count: 2528
        "surface-to-air missile site / sam":{'missile':'surface-to-air_missile','launch_pad':'yes'}, //  Count: 2517
        "stone":{'rawWmCategory':'stone'}, //  Count: 2505
        "UNESCO World Heritage Site":{'rawWmCategory':'UNESCO_World_Heritage_Site'}, //  Count: 2504
        "car hire agency":{'amenity':'car_rental'}, //  Count: 2496
        "automobile rental agency":{'amenity':'car_rental'}, //  Count: 2496
        "car rental agency":{'amenity':'car_rental'}, //  Count: 2496
        "rent-a-car agency":{'amenity':'car_rental'}, //  Count: 2496
        "high-rise":{'rawWmCategory':'high-rise'}, //  Count: 2496
        "haulage service":{'rawWmCategory':'haulage_service'}, //  Count: 2482
        "logistics":{'rawWmCategory':'logistics'}, //  Count: 2482
        "Islamic cemetery":{'landuse':'cemetery','religion':'muslim'}, //  Count: 2478
        "Muslim cemetery":{'landuse':'cemetery','religion':'muslim'}, //  Count: 2478
        "qabristan":{'landu':'cemetery'}, //  Count: 2478
        "furniture factory / maker":{'rawWmCategory':'furniture_factory_/_maker'}, //  Count: 2452
        "igloo":{'building':'igloo'}, //  Count: 2441
        "borough":{'place':'borough'}, //  Count: 2435
        "The district center":{'rawWmCategory':'The_district_center'}, //  Count: 2433
        "adventure/activity centre":{'rawWmCategory':'adventure/activity_centre'}, //  Count: 2431
        "radar":{'rawWmCategory':'radar'}, //  Count: 2425
        "gents":{'rawWmCategory':'gents'}, //  Count: 2424
        "mens":{'rawWmCategory':'mens'}, //  Count: 2424
        "customs":{'amenity':'customs'}, //  Count: 2419
        "customs house / area / checkpoint":{'amenity':'customs'}, //  Count: 2419
        "traction and rolling stock maintenance depot":{'rawWmCategory':'traction_and_rolling_stock_maintenance_depot'}, //  Count: 2404
        "traction maintenance depot (TMD)":{'rawWmCategory':'traction_maintenance_depot_(TMD)'}, //  Count: 2404
        "railway depot":{'landuse':'railway'}, //  Count: 2404
        "motive power depot (MPD)":{'rawWmCategory':'motive_power_depot_(MPD)'}, //  Count: 2404
        "expocentre":{'amenity':'exhibition_centre'}, //  Count: 2404
        "locomotive depot":{'rawWmCategory':'locomotive_depot'}, //  Count: 2404
        "exhibition center":{'amenity':'exhibition_centre'}, //  Count: 2404
        "exhibition centre":{'amenity':'exhibition_centre'}, //  Count: 2404
        "oxbow lake":{'natural':'water','water':'oxbow'}, //  Count: 2403
        "sandpit (quarry)":{'rawWmCategory':'sandpit_(quarry)'}, //  Count: 2395
        "hut":{'building':'hut'}, //  Count: 2390
        "Roman Empire":{'rawWmCategory':'Roman_Empire'}, //  Count: 2389
        "romans":{'rawWmCategory':'romans'}, //  Count: 2389
        "seafood restaurant":{'cuisine':'seafood'}, //  Count: 2388
        "[air force]":{'military:service':'air_force'}, //  Count: 2387
        "armoured fighting vehicle on display":{'historic':'armoured_fighting_vehicle'}, //  Count: 2387
        "tank on display":{'historic':'tank'}, //  Count: 2387
        "radio":{'rawWmCategory':'radio'}, //  Count: 2386
        "orphan asylum":{'rawWmCategory':'orphan_asylum'}, //  Count: 2385
        "orphanage":{'rawWmCategory':'orphanage'}, //  Count: 2385
        "geology":{'rawWmCategory':'geology'}, //  Count: 2370
        "dry cleaner":{'shop':'dry_cleaning'}, //  Count: 2367
        "dry cleaning":{'shop':'dry_cleaning'}, //  Count: 2367
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
        "doctors clinic":{'amenity':'doctors'}, //  Count: 2314
        "doctors office":{'amenity':'doctors'}, //  Count: 2314
        "doctors surgery":{'amenity':'doctors'}, //  Count: 2314
        "general practice / surgery":{'amenity':'doctors'}, //  Count: 2314
        "levee":{'man_made':'dyke'}, //  Count: 2309
        "dike":{'man_made':'dyke'}, //  Count: 2309
        "information technology":{'rawWmCategory':'information_technology'}, //  Count: 2308
        "tourist attraction":{'tourism':'attraction'}, //  Count: 2307
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
