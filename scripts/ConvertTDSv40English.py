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

# ConvertTDSv40English.py
#
# Convert a csv file made from the TDS Spec to a Javascript data structure
#
# This is a modification of ConvertTDSv40Schema.py.  A lot of the print statements from the
# schema generation are commented out. They could probably be removed.
#
# To get the .csv file: 
#   1. Open the spec in Excel
#   2. Goto the Feature_Schema tab
#   3. Select the second to eighth columns, "Table Name" -> "Range"
#   4. Copy the columns
#   5. Paste the columns into a new sheet as "Values"
#   6. Save the new sheet as a csv file
#   7. Do the same for the Dataset_Metadata_Schema tab and the Feature_CollectMetadata_Schema and copy
#      them into the same csv file
# 
# Note: This script is not pretty and does not do any error or sanity checking of 
#       the input file....
#
# Mattj Mar 14
#

import sys,os,csv,argparse,gzip

def printHeader():
    print notice
    print ''
    print '/*'
    print '    TDSv40 "To English" Lookup Tables\n'
    print '    Huge piles of Ugly JSON!'
    print '*/\n'
    #print '__setupPackage__(__extension__);\n'
    print 'etds.rules = {'
    print '    // Tables built from %s\n' % (main_csv_file)


def printEnum():
    print '    // Enumerated Values'
    print '    engEnum : %s , // End of engEnum\n' % (enumDict)


def printSingle():
    print '    // Single Values'
    print '    engSingle : %s , // End of engSingle\n' % (singleDict)


def printDefault():
    print '    // Default values for attributes'
    print '    engDefault : %s , // End of engDefault\n' % (defaultDict)


def printFcode():
    print '    // FCode Lookup Table'
    print '    fcodeLookup : %s ,  // End of fcodeLookup\n' % (fcodeDict)


def printFooter():
    print '} // End of etds.rules\n'

def openFile(path, mode):
    if path.endswith(".gz"):
        return gzip.GzipFile(path, mode)
    else:
        return open(path, mode)


# So Ugly
def dumpISMC():
    ismcList = { "U":"Unclassified", "R":"Restricted", "C":"Confidential", "S":"Secret", "TS":"Top Secret"}

    for i in ismcList:
        enumDict['ZSAX_RS0'][i] = ['Restriction Information : Security Attributes Group (resource classification)',ismcList[i]] 
# End dumpISMC

def dumpNSP():
    nspList = { "ngaAutoAirFacInfoFile":"AAFIF", "ngaControlledImageBase1":"CIB 1", "commercialData":"Commercial Data",
            "ngaDigitalAirFltInfoFile":"DAFIF", "digitalGlobeImagery":"DigitalGlobe Imagery", "ngaDigitalNauticalChart":"DNC", 
            "ngaDigitalVertObstruction":"DVOF", "ngaFoundationFeatureData":"FFD", "ngaGeoNames":"GeoNames", 
            "gpsBasedFieldCollect":"GPS", "ikonosImagery":"Ikonos Imagery", "imageryUnspecified":"Imagery",
            "ngaInterimTerrainData":"ITD", "ngaInterimVectorData":"IVD", "landsatImagery":"Landsat Imagery",
            "usModernizedIntegratedDB":"MIDB", "militaryMapData":"Military Map Data", "nonMilitaryMap":"Non-military Map",
            "usNtmImagery":"NTM Imagery", "openSourceInfo":"open_", "operationsData":"Operations Data",
            "orbimageImagery":"ORBIMAGE Imagery", "quickBirdImagery":"QuickBird Imagery", "ngaStereoAirfieldCollect":"SAC",
            "ngaUrbanVectorMap":"UVMap", "ngaVectorInterimTerrain":"VITD", "ngaVectorMap2":"VMap 2",
            "other":"Other" }

    for i in nspList:
        enumDict['ZI001_NSP'][i] = ['Source Information : Non-spatial Source Type',nspList[i]] 
# End dumpNSP

def dumpSSY():
    ssyList = { "ngaControlledImageBase1":"CIB 1", "digitalGlobeImagery":"DigitalGlobe Imagery", 
            "ngaDigitalNauticalChart":"DNC", "ngaFoundationFeatureData":"FFD", "geoEye1":"GeoEye-1 Imagery", 
            "gpsBasedFieldCollect":"GPS", "ikonosImagery":"Ikonos Imagery", "imageryUnspecified":"Imagery", 
            "ngaInterimTerrainData":"ITD", "usModernizedIntegratedDB":"MIDB", "usNtmImagery":"NTM Imagery", 
            "operationsData":"Operations Data", "orbimageImagery":"ORBIMAGE Imagery", "quickBirdImagery":"QuickBird Imagery" }

    for i in ssyList:
        enumDict['ZI001_SSY'][i] = ['Source Information : Spatial Source Type',ssyList[i]] 
# End dumpSSY

def dumpVSC():
    vscList = { "interpolatedDted1":"DTED 1", "interpolatedDted2":"DTED 2", "noElevations":"No Elevations", 
            "reflectiveSurface":"Reflective Surface", "stereoscopicImagery":"Stereoscopic Imagery", "tinData":"TIN Data" }

    for i in vscList:
        enumDict['ZI001_VSC'][i] = ['Source Information : Vertical Source Category',vscList[i]] 
# End dumpVSC

def dumpCPS():
    cpsList = { "fixed0r25":"Fixed Cells, 0.25 Arc Degree", "fixed0r5":"Fixed Cells, 0.5 Arc Degree", 
            "fixed1r0":"Fixed Cells, 1 Arc Degree", "fixed5r0":"Fixed Cells, 5 Arc Degree", 
            "variable":"Variable Cells" }

    for i in cpsList:
        enumDict['CPS'][i] = ['Cell Partition Scheme',cpsList[i]] 
# End dumpCPS

def dumpEQC():
    eqcList = { "scale1m":"1:1,000,000", "scale100k":"1:100,000", "scale12r5k":"1:12,500", "scale2m":"1:2,000,000", 
            "scale25k":"1:25,000", "scale250k":"1:250,000", "scale5k":"1:5,000", "scale5m":"1:5,000,000", 
            "scale50k":"1:50,000", "scale500k":"1:500,000" }

    for i in eqcList:
        enumDict['EQC'][i] = ['Equivalent Scale Category',eqcList[i]] 
# End dumpEQC

def dumpETS():
    etsList = { "tpc":"1AA-TPC", "onc":"1AB-ONC", "jogAirGround":"1AE-JOG-A/G", "dted1":"1CD-DTED-1", "dfad1":"1CE-DFAD-1",
            "dted2":"1CF-DTED-2", "dfad2":"1CG-DFAD-2", "hac1":"2AA/001-HAC-1", "hac2":"2AA/002-HAC-2", "hac3":"2AA/003-HAC-3",
            "hac4":"2AA/004-HAC-4", "hac5":"2AA/005-HAC-5", "hac6":"2AA/006-HAC-6", "hac7":"2AA/007-HAC-7",
            "hac8":"2AA/008-HAC-8", "hac9":"2AA/009-HAC-9", "combat":"2AD-Combat", "tlm50":"3AA-TLM50", "tlm100":"3AG-TLM100",
            "vitd":"3KA-VITD", "dtop":"3KC/001-DTOP", "vmap2":"3KH-VMap-2", "vmap0":"3KL-VMap-0", "vmap1":"3KM-VMap-1",
            "uvmap":"3KU-UVMap", "atc":"4AA-ATC", "jogRadar":"4AC-JOG-R", "tercomL":"4GE-TERCOM-L", "tercomE":"4GF-TERCOM-E",
            "tercomT":"4GG-TERCOM-T", "ffd":"5EE-FFD", "digitalFeg":"DFEG", "dnc":"DNC", "globalTdsEg":"GTDS-EG",
            "localTdsEg":"LTDS-EG", "mgcpTrd":"MGCP TRD", "msd1":"MSD1", "msd2":"MSD2", "msd3":"MSD3", "msd4":"MSD4",
            "msd5":"MSD5", "regionalTdsEg":"RTDS-EG", "specUrbanTdsEg":"S-UTDS-EG", "tod0":"TOD0", "tod1":"TOD1",
            "tod2":"TOD2", "tod3":"TOD3", "tod4":"TOD4" }

    for i in etsList:
        enumDict['ETS'][i] = ['Extraction Specification',etsList[i]] 
# End dumpETS

def dumpHZD():
    hzdList = { "adindanBurkinaFaso":"Adindan (Burkina Faso)", "adindanCameroon":"Adindan (Cameroon)",
                "adindanEthiopia":"Adindan (Ethiopia)", "adindanMali":"Adindan (Mali)",
                "adindanMeanValue":"Adindan (mean value)", "adindanSenegal":"Adindan (Senegal)",
                "adindanSudan":"Adindan (Sudan)", "afgooyeSomalia":"Afgooye (Somalia)",
                "ainelAbd1970BahrainIsland":"Ain el Abd 1970 (Bahrain Island)",
                "ainelAbd1970SaudiArabia":"Ain el Abd 1970 (Saudi Arabia)",
                "americanSamoaDatum1962":"American Samoa Datum 1962",
                "amersfoort1885dash1903":"Amersfoort 1885/1903 (Netherlands)",
                "anna1Astro1965CocosIslands":"Anna 1 Astro 1965 (Cocos Islands)",
                "antiguaIslandAstro1943":"Antigua Island Astro 1943",
                "approximateLuzonDatum":"Approximate Luzon Datum (Philippines)",
                "arc1935Africa":"Arc 1935 (Africa)",
                "arc1950Botswana":"Arc 1950 (Botswana)",
                "arc1950Burundi":"Arc 1950 (Burundi)",
                "arc1950Lesotho":"Arc 1950 (Lesotho)",
                "arc1950Malawi":"Arc 1950 (Malawi)",
                "arc1950MeanValue":"Arc 1950 (mean value)",
                "arc1950Swaziland":"Arc 1950 (Swaziland)",
                "arc1950Zaire":"Arc 1950 (Zaire)",
                "arc1950Zambia":"Arc 1950 (Zambia)",
                "arc1950Zimbabwe":"Arc 1950 (Zimbabwe)",
                "arc1960Kenya":"Arc 1960 (Kenya)",
                "arc1960MeanValue":"Arc 1960 (mean value)",
                "arc1960Tanzania":"Arc 1960 (Tanzania)",
                "ascensionIsland1958":"Ascension Island 1958 (Ascension Island)",
                "astroBeaconEIwoJimaIsland":"Astro Beacon 'E' (Iwo Jima Island)",
                "astroDos71dash4StHelena":"Astro DOS 71/4 (St. Helena Island)",
                "astroStation1952MarcusIs":"Astro Station 1952 (Marcus Island)",
                "astroTernIsland1961Hawaii":"Astro Tern Island 1961 (Tern Island, Hawaii)",
                "australianGeodetic1966":"Australian Geod. 1966 (Australia and Tasmania Island)",
                "australianGeodetic1984":"Australian Geod. 1984 (Australia and Tasmania Island)",
                "averageTerrestrialSys1977":"Average Terrestrial System 1977, New Brunswick",
                "ayabelleLighthouseDjibouti":"Ayabelle Lighthouse (Djibouti)",
                "bekaaBaseSouthEndLebanon":"Bekaa Base South End (Lebanon)",
                "belgium1950SystemLommelSig":"Belgium 1950 System (Lommel Signal, Belgium)",
                "belgium1972Observatoire":"Belgium 1972 (Observatoire d'Uccle)",
                "bellevueIgnEfateErromango":"Bellevue (IGN) (Efate and Erromango Islands)",
                "bermuda1957BermudaIslands":"Bermuda 1957 (Bermuda Islands)",
                "bern1898Switzerland":"Bern 1898 (Switzerland)",
                "bern1898ZeroMeridian":"Bern 1898 (Switzerland) with Zero Meridian Bern",
                "bissauGuineaBissau":"Bissau (Guinea-Bissau)",
                "bjz54A954BeijingCoord":"BJZ54 (A954 Beijing Coordinates) (China)",
                "bogotaObservatoryColombia":"Bogota Observatory (Colombia)",
                "bogotaObsZeroMeridian":"Bogota Observatory (Colombia) with Zero Meridian Bogota",
                "bukitRimpahIndonesia":"Bukit Rimpah (Bangka and Belitung Islands, Indonesia)",
                "camacupaBaseSwEndAngola":"Camacupa Base SW End (Campo De Aviacao, Angola)",
                "campAreaAstroAntarctica":"Camp Area Astro (Camp McMurdo Area, Antarctica)",
                "campoInchauspeArgentina":"Campo Inchauspe (Argentina)",
                "cantonAstro1966PhoenixIs":"Canton Astro 1966 (Phoenix Islands)",
                "capeSouthAfrica":"Cape (South Africa)",
                "capeCanaveralMeanValue":"Cape Canaveral (mean value)",
                "carthageTunisia":"Carthage (Tunisia)",
                "chatham1971NewZealand":"Chatham 1971 (Chatham Island, New Zealand)",
                "chuaAstroParaguay":"Chua Astro (Paraguay)",
                "compensationGeoQuebec1977":"Compensation Geodetique du Quebec 1977",
                "conakryPyramidGuinea":"Conakry Pyramid of the Service Geographique (Guinea)",
                "estonia1937":"Co-ordinate System 1937 of Estonia",
                "corregoAlegreBrazil":"Corrego Alegre (Brazil)",
                "dabolaGuinea":"Dabola (Guinea)",
                "dcs3LighthouseLesserAnt":"DCS-3 Lighthouse, Saint Lucia, Lesser Antilles",
                "deceptionIslAntarctica":"Deception Island, Antarctica",
                "djakartaBataviaIndonesia":"Djakarta (Batavia) (Sumatra Island, Indonesia)",
                "djakartaBataviaZeroMerid":"Djakarta (Batavia) (Sumatra Island, Indonesia) with Zero Meridian Djakarta",
                "dominicaAstroM12LesserAnt":"Dominica Astro M-12, Dominica, Lesser Antilles",
                "dos1968GizoNewGeorgiaIs":"DOS 1968 (Gizo Island, New Georgia Islands)",
                "easterIsland1967EasterIs":"Easter Island 1967 (Easter Island)",
                "european1950BritishIsles":"European 1950 (British Isles)",
                "european1950Cyprus":"European 1950 (Cyprus)",
                "european1950Egypt":"European 1950 (Egypt)",
                "european1950England":"European 1950 (England, Channel Islands, Scotland, and Shetland Islands)",
                "european1950Greece":"European 1950 (Greece)",
                "european1950Iran":"European 1950 (Iran)",
                "european1950IraqSyria":"European 1950 (Iraq, Israel, Jordan, Kuwait, Lebanon, Saudi Arabia, and Syria)",
                "european1950Malta":"European 1950 (Malta)",
                "european1950MeanValue":"European 1950 (mean value)",
                "european1950NorwayFinland":"European 1950 (Norway and Finland)",
                "european1950PortugalSpain":"European 1950 (Portugal and Spain)",
                "european1950Sardinia":"European 1950 (Sardinia)",
                "european1950Sicily":"European 1950 (Sicily)",
                "european1950Tunisia":"European 1950 (Tunisia)",
                "european1950WesternEurope":"European 1950 (Western Europe)",
                "european1979MeanValue":"European 1979 (mean value)",
                "etrs1989":"European Terrestrial Reference System 1989 (ETRS89)",
                "fortThomas1955LeewardIs":"Fort Thomas 1955 (Nevis, St Kitts, Leeward Islands)",
                "gan1970AdduAtoll":"Gan 1970 (Addu Atoll, Republic of Maldives)",
                "gandajikaBaseZaire":"Gandajika Base (Zaire)",
                "gdz80China":"GDZ80 (China)",
                "geocentricDatumOfAustralia":"Geocentric Datum of Australia (GDA)",
                "geodeticDatum1949Zealand":"Geodetic Datum 1949 (New Zealand)",
                "graciosaBaseSWFaialAzores":"Graciosa Base SW (Faial, Graciosa, Pico, Sao Jorge, and Terceira Island, Azores)",
                "greekDatumGreece":"Greek Datum, Greece",
                "greekGeodeticRefSystem1987":"Greek Geodetic Reference System 1987 (GGRS 87)",
                "guam1963":"Guam 1963",
                "gunongSegaraKalimantanIs":"Gunong Segara (Kalimantan Island, Indonesia)",
                "gunongSerindung":"Gunong Serindung",
                "gux1AstroGuadacanalIs":"GUX 1 Astro (Guadacanal Island)",
                "guyanaCSG67":"Guyana CSG67",
                "heratNorthAfganistan":"Herat North (Afganistan)",
                "hermannskogel":"Hermannskogel",
                "hjorsey1955Iceland":"Hjorsey 1955 (Iceland)",
                "hongKong1929":"Hong Kong 1929",
                "hongKong1963HongKong":"Hong Kong 1963 (Hong Kong)",
                "hungarian1972":"Hungarian 1972",
                "huTzuShan":"Hu-Tzu-Shan",
                "indianBangladesh":"Indian (Bangladesh)",
                "indianIndiaNepal":"Indian (India and Nepal)",
                "indianPakistan":"Indian (Pakistan)",
                "indianThailandVietnam":"Indian (Thailand and Vietnam)",
                "indian1954Thailand":"Indian 1954 (Thailand)",
                "indian1960ConSonIsland":"Indian 1960 (Con Son Island (Vietnam))",
                "indian1960Vietnam":"Indian 1960 (Vietnam: near 16 degrees North)",
                "indian1975Thailand":"Indian 1975 (Thailand)",
                "indian1975ThailandCycle1":"Indian 1975 (Thailand) - Cycle 1",
                "indonesian1974":"Indonesian 1974",
                "ireland1965IrelandNorthern":"Ireland 1965 (Ireland and Northern Ireland)",
                "ists061Astro1968GeorgiaIs":"ISTS 061 Astro 1968 (South Georgia Islands)",
                "ists073Astro1969DiegoGar":"ISTS 073 Astro 1969 (Diego Garcia)",
                "johnstonIsland1961":"Johnston Island 1961 (Johnston Island)",
                "kalianpurIndia":"Kalianpur (India)",
                "kandawalaSriLanka":"Kandawala (Sri Lanka)",
                "kcs2SierraLeone":"KCS 2, Sierra Leone",
                "kerguelenIsland1949":"Kerguelen Island 1949 (Kerguelen Island)",
                "kertau1948RevisedMalaysia":"Kertau 1948 (or Revised Kertau) (West Malaysia and Singapore)",
                "kkjFinland":"KKJ (or Kartastokoordinaattijarjestelma), Finland",
                "koreanGeodeticSystem1995":"Korean Geodetic System 1995 (South Korea)",
                "kusaieAstro1951":"Kusaie Astro 1951",
                "kuwaitOilCompanyK28":"Kuwait Oil Company (K28)",
                "lc5Astro1961CaymanBracIs":"L.C. 5 Astro 1961 (Cayman Brac Island)",
                "leigonGhana":"Leigon (Ghana)",
                "liberia1964Liberia":"Liberia 1964 (Liberia)",
                "lisbonCastelodiSaoJorge":"Lisbon (Castelo di Sao Jorge), Portugal",
                "localAstro":"Local Astro",
                "lomaQuintanaVenezuela":"Loma Quintana (Venezuela)",
                "luzonMindanaoIsland":"Luzon (Mindanao Island)",
                "luzonPhilipinesNotMindanao":"Luzon (Philipines except Mindanao Island)",
                "mahe1971MaheIsland":"Mahe 1971 (Mahe Island)",
                "manokwariWestIrian":"Manokwari (West Irian)",
                "marcoAstroSalvageIslands":"Marco Astro (Salvage Islands)",
                "martiniqueFortDesaix":"Martinique Fort-Desaix",
                "massawEritreaEthiopia":"Massawa (Eritrea, Ethiopia)",
                "mayotteCombani":"Mayotte Combani",
                "merchichMorocco":"Merchich (Morocco)",
                "midwayAstro1961MidwayIs":"Midway Astro 1961 (Midway Island)",
                "minnaCameroon":"Minna (Cameroon)",
                "minnaNigeria":"Minna (Nigeria)",
                "modifiedBJZ54China":"Modified BJZ54 (China)",
                "montjongLowe":"Montjong Lowe",
                "montserratIslandAstro1958":"Montserrat Island Astro 1958",
                "mountDillonTobago":"Mount Dillon, Tobago",
                "mPoralokoGabon":"M'Poraloko (Gabon)",
                "nahrwanMasirahIslandOman":"Nahrwan (Masirah Island, Oman)",
                "nahrwanSaudiArabia":"Nahrwan (Saudi Arabia)",
                "nahrwanUnitedArabEmirates":"Nahrwan (United Arab Emirates)",
                "naparimaBwiTrinidadTobago":"Naparima (BWI, Trinidad and Tobago)",
                "newFrenchZeroMeridianParis":"New French or Nouvelle Triangulation Francaise (NTF) with Zero Meridian Paris",
                "northAm1927Alaska":"North American 1927 (Alaska)",
                "northAm1927Alberta":"North American 1927 (Alberta and British Columbia)",
                "northAm1927AleutianE":"North American 1927 (Aleutian Islands East of 180 degrees West)",
                "northAm1927AleutianW":"North American 1927 (Aleutian Islands West of 180 degrees West)",
                "northAm1927Bahamas":"North American 1927 (Bahamas, except San Salvador Island)",
                "northAm1927CanadMean":"North American 1927 (Canada mean)",
                "northAm1927CanalZone":"North American 1927 (Canal Zone)",
                "northAm1927Caribbean":"North American 1927 (Caribbean)",
                "northAm1927CentAmer":"North American 1927 (Central America)",
                "northAm1927ConusMean":"North American 1927 (CONUS mean)",
                "northAm1927Cuba":"North American 1927 (Cuba)",
                "northAm1927EasternUs":"North American 1927 (Eastern US)",
                "northAm1927HayesPen":"North American 1927 (Hayes Peninsula, Greenland)",
                "northAm1927Manitoba":"North American 1927 (Manitoba and Ontario)",
                "northAm1927Mexico":"North American 1927 (Mexico)",
                "northAm1927Newfound":"North American 1927 (Newfoundland, New Brunswick, Nova Scotia and Quebec)",
                "northAm1927NwTerSaskat":"North American 1927 (Northwest Territories and Saskatchewan)",
                "northAm1927Salvador":"North American 1927 (San Salvador Island)",
                "northAm1927WesternUs":"North American 1927 (Western US)",
                "northAm1927Yukon":"North American 1927 (Yukon)",
                "northAm1983AlaskaExAleut":"North American 1983 (Alaska, excluding Aleutian Islands)",
                "northAm1983Aleutian":"North American 1983 (Aleutian Islands)",
                "northAm1983Canada":"North American 1983 (Canada)",
                "northAm1983Conus":"North American 1983 (CONUS)",
                "northAm1983Hawaii":"North American 1983 (Hawaii)",
                "northAm1983Mexico":"North American 1983 (Mexico and Central America))",
                "northSahara1959":"North Sahara 1959",
                "observMeteorologico1939":"Observatorio Meteorologico 1939 (Corvo and Flores Islands, Azores)",
                "ocotopequeGuatemala":"Ocotopeque, Guatemala",
                "oldEgyptian":"Old Egyptian (Egypt)",
                "oldHawaiianHawaiiIsland":"Old Hawaiian (Hawaii)",
                "oldHawaiianKauaiIsland":"Old Hawaiian (Kauai)",
                "oldHawaiianMauiIsland":"Old Hawaiian (Maui)",
                "oldHawaiianMeanValue":"Old Hawaiian (mean value)",
                "oldHawaiianOahuIsland":"Old Hawaiian (Oahu)",
                "oman":"Oman (Oman)",
                "ordnanceSurvGB1936England":"Ordnance Survey G.B. 1936 (England)",
                "ordnanceSurvGB1936ScotWale":"Ordnance Survey G.B. 1936 (England, Isle of Man, and Wales)",
                "ordnanceSurvGB1936MeanVal":"Ordnance Survey G.B. 1936 (mean value)",
                "ordnanceSurvGB1936ScotShet":"Ordnance Survey G.B. 1936 (Scotland and Shetland Islands)",
                "ordnanceSurvGB1936Wales":"Ordnance Survey G.B. 1936 (Wales)",
                "osloObservatoryOld":"Oslo Observatory (Old), Norway",
                "padangBaseWestEnd":"Padang Base West End (Sumatra, Indonesia)",
                "padangBaseWestEndZeroMerid":"Padang Base West End (Sumatra, Indonesia) with Zero Meridian Djakarta",
                "palestine1928":"Palestine 1928 (Israel, Jordan)",
                "picoDeLasNievesCanaryIs":"Pico de las Nieves (Canary Islands)",
                "pitcairnAstro1967":"Pitcairn Astro 1967 (Pitcairn Island)",
                "point58MeanSolution":"Point 58 Mean Solution (Burkina Faso and Niger)",
                "pointeNoire1948":"Pointe Noire 1948",
                "potsdamHelmertturmGermany":"Potsdam or Helmertturm (Germany)",
                "provSouthAm1956Bolivia":"Prov. S. American 1956 (Bolivia)",
                "provSouthAm1956Columbia":"Prov. S. American 1956 (Columbia)",
                "provSouthAm1956Ecuador":"Prov. S. American 1956 (Ecuador)",
                "provSouthAm1956Guyana":"Prov. S. American 1956 (Guyana)",
                "provSouthAm1956MeanValue":"Prov. S. American 1956 (mean value)",
                "provSouthAm1956NChile":"Prov. S. American 1956 (Northern Chile near 19 degrees South)",
                "provSouthAm1956Peru":"Prov. S. American 1956 (Peru)",
                "provSouthAm1956SChile":"Prov. S. American 1956 (Southern Chile near 43 degrees South)",
                "provSouthAm1956Venezuela":"Prov. S. American 1956 (Venezuela)",
                "provSouthChilean1963":"Provisional South Chilean 1963 (or Hito XVIII 1963) (S. Chile, 53 degrees South)",
                "puertoRicoVirginIslands":"Puerto Rico (Puerto Rico and Virgin Islands)",
                "pulkovo1942Albania":"Pulkovo 1942 (Albania)",
                "pulkovo1942Czechoslovakia":"Pulkovo 1942 (Czechoslovakia)",
                "pulkovo1942Hungary":"Pulkovo 1942 (Hungary)",
                "pulkovo1942Kazakhstan":"Pulkovo 1942 (Kazakhstan)",
                "pulkovo1942Latvia":"Pulkovo 1942 (Latvia)",
                "pulkovo1942Poland":"Pulkovo 1942 (Poland)",
                "pulkovo1942Romania":"Pulkovo 1942 (Romania)",
                "pulkovo1942Russia":"Pulkovo 1942 (Russia)",
                "qatarNationalQatar":"Qatar National (Qatar)",
                "qornoqSouthGreenland":"Qornoq (South Greenland)",
                "rauenbergBerlinGermany":"Rauenberg (Berlin, Germany)",
                "reconTriangulationMorocco":"Reconnaissance Triangulation, Morocco",
                "reunion1947":"Reunion 1947",
                "revisedNahrwan":"Revised Nahrwan",
                "rome1940":"Rome 1940 (or Monte Mario 1940), Italy",
                "rome1940ZeroMeridianRome":"Rome 1940 (or Monte Mario 1940), Italy, with Zero Meridian Rome",
                "rt90StockholmSweden":"RT90, Stockholm, Sweden",
                "sainteAnneI1984Guadeloupe":"Sainte Anne I 1984 (Guadeloupe)",
                "santoDos1965EspiritoSanto":"Santo (DOS) 1965 (Espirito Santo Island)",
                "saoBrazSaoMiguelAzores":"Sao Braz (Sao Miguel, Santa Maria Islands, Azores)",
                "sapperHill1943EastFalkland":"Sapper Hill 1943 (East Falkland Islands)",
                "schwarzeckNamibia":"Schwarzeck (Namibia)",
                "seBasePortoSanto":"SE Base (Porto Santo) (Porto Santo and Madeira Islands)",
                "selvagemGrande1938Salvage":"Selvagem Grande 1938 (Salvage Islands)",
                "sierraLeone1960":"Sierra Leone 1960",
                "sJtsk":"S-JTSK",
                "southAfricanSouthAfrica":"South African (South Africa)",
                "southAmerican1969Argentina":"South American 1969 (Argentina)",
                "southAmerican1969BaltraIs":"South American 1969 (Baltra, Galapagos Islands)",
                "southAmerican1969Bolivia":"South American 1969 (Bolivia)",
                "southAmerican1969Brazil":"South American 1969 (Brazil)",
                "southAmerican1969Chile":"South American 1969 (Chile)",
                "southAmerican1969Columbia":"South American 1969 (Columbia)",
                "southAmerican1969Ecuador":"South American 1969 (Ecuador)",
                "southAmerican1969Guyana":"South American 1969 (Guyana)",
                "southAmerican1969MeanValue":"South American 1969 (mean value)",
                "southAmerican1969Paraguay":"South American 1969 (Paraguay)",
                "southAmerican1969Peru":"South American 1969 (Peru)",
                "southAmerican1969Trinidad":"South American 1969 (Trinidad and Tobago)",
                "southAmerican1969Venezuela":"South American 1969 (Venezuela)",
                "sirgas":"South American Geocentric Reference System (SIRGAS)",
                "southAsiaSingapore":"South Asia (Southeast Asia, Singapore)",
                "sovietGeodeticSystem1985":"Soviet Geodetic System 1985",
                "sovietGeodeticSystem1990":"Soviet Geodetic System 1990",
                "stPierreetMiquelon1950":"St. Pierre et Miquelon 1950",
                "stockholm1938Sweden":"Stockholm 1938 (Sweden)",
                "sydneyObservatoryNewSouth":"Sydney Observatory, New South Wales, Australia",
                "tananariveObservatory1925":"Tananarive Observatory 1925",
                "tananariveObs1925ZerMerPar":"Tananarive Observatory 1925, with Zero Meridian Paris",
                "timbalai1948BruneiMalaysia":"Timbalai 1948 (Brunei and East Malaysia - Sarawak and Sabah)",
                "timbalai1968":"Timbalai 1968",
                "tokyoJapan":"Tokyo (Japan)",
                "tokyoKorea":"Tokyo (Korea)",
                "tokyoKoreaCycle1":"Tokyo (Korea) - Cycle 1",
                "tokyoMeanValue":"Tokyo (mean value)",
                "tokyoOkinawa":"Tokyo (Okinawa)",
                "trinidad1903":"Trinidad 1903",
                "tristanAstro1968Cunha":"Tristan Astro 1968 (Tristan da Cunha)",
                "vitiLevu1916FijiIslands":"Viti Levu 1916 (Viti Levu Island, Fiji Islands)",
                "voirol1875":"Voirol 1875",
                "voirol1875ZeroMeridParis":"Voirol 1875 with Zero Meridian Paris",
                "voirol1960Algeria":"Voirol 1960, Algeria",
                "voirol1960ZeroMeridParis":"Voirol 1960, Algeria, with Zero Meridian Paris",
                "wakeIslandAstro1952":"Wake Island Astro 1952",
                "wakeEniwetok1960MarshallIs":"Wake-Eniwetok 1960 (Marshall Islands)",
                "worldGeodeticSystem1960":"World Geodetic System 1960",
                "worldGeodeticSystem1966":"World Geodetic System 1966",
                "worldGeodeticSystem1972":"World Geodetic System 1972",
                "worldGeodeticSystem1984":"World Geodetic System 1984",
                "yacareUruguay":"Yacare (Uruguay)",
                "zanderijSurinam":"Zanderij (Surinam)" }

    for i in hzdList:
        enumDict['HZD'][i] = ['Geodetic Datum',hzdList[i]] 
# End dumpHZD

def dumpVDT():
    vdtList = { "groundLevel":"Ground Level", "meanSeaLevel":"Mean Sea Level (MSL)",
            "ngvd29":"National Geodetic Vertical Datum (NGVD) 1929", "navd88":"North American Vertical Datum (NAVD) 1988",
            "wgs84Egm08":"WGS 84 EGM08 Geoid", "wgs84Egm96":"WGS 84 EGM96 Geoid", "wgs84":"WGS 84 Ellipsoid" }

    for i in vdtList:
        enumDict['VDT'][i] = ['Vertical Datum',vdtList[i]] 
# End dumpVDT

def dumpNonIc():
    nonIcList = { 
            "SC":"Special Category", "SINFO":"Sensitive Information", 
            "DS":"Limited Distribution", "XD":"Exclusive Distribution", 
            "ND":"No Distribution", "SBU":"Sensitive But Unclassified", 
            "SBU-NF":"Sensitive But Unclassified Noforn", "LES":"Law Enforcement Sensitive", 
            "LES-NF":"Law Enforcement Sensitive Noforn" 
            }

    for i in nonIcList:
        enumDict['ZSAX_RX3'][i] = ['Restriction Information : Security Attributes Group (resource non-intelligence community markings)',nonIcList[i]] 
# End dumpNonIc

        
# Yes, Ugly
def dumpTrigraph():
    triList = {
        "AFGHANISTAN":"AFG", "ALAND ISLANDS":"ALA", "ALBANIA":"ALB", "ALGERIA":"DZA", 
        "AMERICAN SAMOA":"ASM", "ANDORRA":"AND", "ANGOLA":"AGO", "ANGUILLA":"AIA", 
        "ANTARCTICA":"ATA", "ANTIGUA AND BARBUDA":"ATG", "ARGENTINA":"ARG", "ARMENIA":"ARM", 
        "ARUBA":"ABW", "AUSTRALIA":"AUS", "AUSTRIA":"AUT", "AZERBAIJAN":"AZE", 
        "BAHAMAS":"BHS", "BAHRAIN":"BHR", "BANGLADESH":"BGD", "BARBADOS":"BRB", 
        "BELARUS":"BLR", "BELGIUM":"BEL", "BELIZE":"BLZ", "BENIN":"BEN", "BERMUDA":"BMU", 
        "BHUTAN":"BTN", "BOLIVIA, PLURINATIONAL STATE OF":"BOL", 
        "BONAIRE, SINT EUSTATIUS AND SABA":"BES", "BOSNIA AND HERZEGOVINA":"BIH", 
        "BOTSWANA":"BWA", "BOUVET ISLAND":"BVT", "BRAZIL":"BRA", 
        "BRITISH INDIAN OCEAN TERRITORY":"IOT", "BRUNEI DARUSSALAM":"BRN", "BULGARIA":"BGR", 
        "BURKINA FASO":"BFA", "BURUNDI":"BDI", "CAMBODIA":"KHM", "CAMEROON":"CMR", 
        "CANADA":"CAN", "CAPE VERDE":"CPV", "CAYMAN ISLANDS":"CYM", 
        "CENTRAL AFRICAN REPUBLIC":"CAF", "CHAD":"TCD", "CHILE":"CHL", "CHINA":"CHN", 
        "CHRISTMAS ISLAND":"CXR", "COCOS (KEELING) ISLANDS":"CCK", "COLOMBIA":"COL", 
        "COMOROS":"COM", "CONGO":"COG", "CONGO, DEMOCRATIC REPUBLIC OF THE":"COD", 
        "COOK ISLANDS":"COK", "COSTA RICA":"CRI", "COTE D IVOIRE":"CIV", "CROATIA":"HRV", 
        "CUBA":"CUB", "CURACAO":"CUW", "CYPRUS":"CYP", 
        "CZECH REPUBLIC":"CZE", "DENMARK":"DNK", "DJIBOUTI":"DJI", "DOMINICA":"DMA", 
        "DOMINICAN REPUBLIC":"DOM", "ECUADOR":"ECU", "EGYPT":"EGY", "EL SALVADOR":"SLV", 
        "EQUATORIAL GUINEA":"GNQ", "ERITREA":"ERI", "ESTONIA":"EST", "ETHIOPIA":"ETH", 
        "FALKLAND ISLANDS (MALVINAS)":"FLK", "FAROE ISLANDS":"FRO", "FIJI":"FJI", 
        "FINLAND":"FIN", "FRANCE":"FRA", "FRENCH GUIANA":"GUF", "FRENCH POLYNESIA":"PYF", 
        "FRENCH SOUTHERN TERRITORIES":"ATF", "GABON":"GAB", "GAMBIA":"GMB", "GEORGIA":"GEO", 
        "GERMANY":"DEU", "GHANA":"GHA", "GIBRALTAR":"GIB", "GREECE":"GRC", "GREENLAND":"GRL", 
        "GRENADA":"GRD", "GUADELOUPE":"GLP", "GUAM":"GUM", "GUATEMALA":"GTM", 
        "GUERNSEY":"GGY", "GUINEA":"GIN", "GUINEA-BISSAU":"GNB", "GUYANA":"GUY", "HAITI":"HTI", 
        "HEARD ISLAND AND MCDONALD ISLANDS":"HMD", "HOLY SEE (VATICAN CITY STATE)":"VAT", 
        "HONDURAS":"HND", "HONG KONG":"HKG", "HUNGARY":"HUN", "ICELAND":"ISL", "INDIA":"IND", 
        "INDONESIA":"IDN", "IRAN, ISLAMIC REPUBLIC OF":"IRN", "IRAQ":"IRQ", "IRELAND":"IRL", 
        "ISLE OF MAN":"IMN", "ISRAEL":"ISR", "ITALY":"ITA", "JAMAICA":"JAM", "JAPAN":"JPN", 
        "JERSEY":"JEY", "JORDAN":"JOR", "KAZAKHSTAN":"KAZ", "KENYA":"KEN", "KIRIBATI":"KIR", 
        "KOREA, DEMOCRATIC PEOPLES REPUBLIC OF":"PRK", "KOREA, REPUBLIC OF":"KOR", 
        "KUWAIT":"KWT", "KYRGYZSTAN":"KGZ", "LAO PEOPLES DEMOCRATIC REPUBLIC":"LAO", 
        "LATVIA":"LVA", "LEBANON":"LBN", "LESOTHO":"LSO", "LIBERIA":"LBR", "LIBYA":"LBY", 
        "LIECHTENSTEIN":"LIE", "LITHUANIA":"LTU", "LUXEMBOURG":"LUX", "MACAO":"MAC", 
        "MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF":"MKD", "MADAGASCAR":"MDG", "MALAWI":"MWI", 
        "MALAYSIA":"MYS", "MALDIVES":"MDV", "MALI":"MLI", "MALTA":"MLT", 
        "MARSHALL ISLANDS":"MHL", "MARTINIQUE":"MTQ", "MAURITANIA":"MRT", "MAURITIUS":"MUS", 
        "MAYOTTE":"MYT", "MEXICO":"MEX", "MICRONESIA, FEDERATED STATES OF":"FSM", 
        "MOLDOVA, REPUBLIC OF":"MDA", "MONACO":"MCO", "MONGOLIA":"MNG", "MONTENEGRO":"MNE", 
        "MONTSERRAT":"MSR", "MOROCCO":"MAR", "MOZAMBIQUE":"MOZ", "MYANMAR":"MMR", 
        "NAMIBIA":"NAM", "NAURU":"NRU", "NEPAL":"NPL", "NETHERLANDS":"NLD", 
        "NEW CALEDONIA":"NCL", "NEW ZEALAND":"NZL", "NICARAGUA":"NIC", "NIGER":"NER", 
        "NIGERIA":"NGA", "NIUE":"NIU", "NORFOLK ISLAND":"NFK", "NORTHERN MARIANA ISLANDS":"MNP", 
        "NORWAY":"NOR", "OMAN":"OMN", "PAKISTAN":"PAK", "PALAU":"PLW", 
        "PALESTINIAN TERRITORY, OCCUPIED":"PSE", "PANAMA":"PAN", "PAPUA NEW GUINEA":"PNG", 
        "PARAGUAY":"PRY", "PERU":"PER", "PHILIPPINES":"PHL", "PITCAIRN":"PCN", "POLAND":"POL", 
        "PORTUGAL":"PRT", "PUERTO RICO":"PRI", "QATAR":"QAT", "REUNION":"REU", "ROMANIA":"ROU", 
        "RUSSIAN FEDERATION":"RUS", "RWANDA":"RWA", "SAINT BARTHELEMY":"BLM",
        "SAINT HELENA, ASCENSION AND TRISTAN DA CUNHA":"SHN", "SAINT KITTS AND NEVIS":"KNA", 
        "SAINT LUCIA":"LCA", "SAINT MARTIN (FRENCH PART)":"MAF", 
        "SAINT PIERRE AND MIQUELON":"SPM", "SAINT VINCENT AND THE GRENADINES":"VCT", 
        "SAMOA":"WSM", "SAN MARINO":"SMR", "SAO TOME AND PRINCIPE":"STP", "SAUDI ARABIA":"SAU", 
        "SENEGAL":"SEN", "SERBIA":"SRB", "SEYCHELLES":"SYC", "SIERRA LEONE":"SLE", 
        "SINGAPORE":"SGP", "SINT MAARTEN (DUTCH PART)":"SXM", "SLOVAKIA":"SVK", 
        "SLOVENIA":"SVN", "SOLOMON ISLANDS":"SLB", "SOMALIA":"SOM", "SOUTH AFRICA":"ZAF", 
        "SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS":"SGS", "SOUTH SUDAN":"SSD", 
        "SPAIN":"ESP", "SRI LANKA":"LKA", "SUDAN":"SDN", "SURINAME":"SUR", 
        "SVALBARD AND JAN MAYEN":"SJM", "SWAZILAND":"SWZ", "SWEDEN":"SWE", "SWITZERLAND":"CHE", 
        "SYRIAN ARAB REPUBLIC":"SYR", "TAIWAN, PROVINCE OF CHINA":"TWN", "TAJIKISTAN":"TJK", 
        "TANZANIA, UNITED REPUBLIC OF":"TZA", "THAILAND":"THA", "TIMOR-LESTE":"TLS", 
        "TOGO":"TGO", "TOKELAU":"TKL", "TONGA":"TON", "TRINIDAD AND TOBAGO":"TTO", 
        "TUNISIA":"TUN", "TURKEY":"TUR", "TURKMENISTAN":"TKM", "TURKS AND CAICOS ISLANDS":"TCA", 
        "TUVALU":"TUV", "UGANDA":"UGA", "UKRAINE":"UKR", "UNITED ARAB EMIRATES":"ARE", 
        "UNITED KINGDOM":"GBR", "UNITED STATES":"USA", 
        "UNITED STATES MINOR OUTLYING ISLANDS":"UMI", "URUGUAY":"URY", "UZBEKISTAN":"UZB", 
        "VANUATU":"VUT", "VENEZUELA, BOLIVARIAN REPUBLIC OF":"VEN", "VIET NAM":"VNM", 
        "VIRGIN ISLANDS, BRITISH":"VGB", "VIRGIN ISLANDS, U.S.":"VIR", 
        "WALLIS AND FUTUNA":"WLF", "WESTERN SAHARA":"ESH", "YEMEN":"YEM", "ZAMBIA":"ZMB", 
        "ZIMBABWE":"ZWE" }

    for i in triList:
        enumDict['IC2'][triList[i]] = ['ISO 3166-1 Country Code (alpha3)',i] 
        #enumDict['IC2'][i] = ['ISO 3166-1 Country Code (alpha3)',triList[i]] 

# End dumpTrigraph
        
# So Very Ugly
def dumpRCO():
    rcoList = {
        "Army (Australia)":"armyAustralia", 
        "Army Geographic Agency (Netherlands)":"armyGeoAgencyNetherlands", 
        "Army Geographic Centre (Spain)":"armyGeographicCentreSpain", 
        "Army Geographic Institute (Portugal)":"armyGeoInstitutePortugal", 
        "Bundeswehr Geoinformation Office (Germany)":"bundeswehrGeoinfoOffice", 
        "Defence Acquisition and Logistics Organization (Denmark)":"defenceAcqLogOrgDenmark", 
        "Defence Geographic Centre Intelligence Collection Group (United Kingdom)":"defenceGeoCentreIntColGrp", 
        "Defence Imagery and Geospatial Organisation (Australia)":"defenceImageryGeoOrg", 
        "Defense Information Security (Italy)":"defenseInfoSecurityItaly", 
        "General Command of Mapping (Turkey)":"generalCommandMapping", 
        "Geospatial Information Agency (Latvia)":"geoInfoAgencyLatvia", 
        "Geospatial Intelligence Organisation (New Zealand)":"geoIntelOrgNewZealand", 
        "Geographic Service of the Czech Armed Forces (Czech Republic)":"geoServCzechArmedForces", 
        "Geographic Service (Belgium)":"geoServiceBelgium", 
        "Hellenic Military Geographic Service (Greece)":"hellenicMilitaryGeoServ", 
        "Joint Geography Bureau (France)":"jointGeoBureauFrance", 
        "Mapping and Charting Establishment (Canada)":"mapChartEstablishment", 
        "Mapping Service (Hungary)":"mappingServiceHungary", 
        "Military Geography Division (Poland)":"milGeogDivisionPoland", 
        "Military Geographic Group (Estonia)":"milGeoGroupEstonia", 
        "Military Geographic Service (Norway)":"milGeoServiceNorway", 
        "Military Mapping Centre (Lithuania)":"milMapCentreLithuania", 
        "Military Topographic Directorate (Romania)":"milTopoDirectRomania", 
        "Military Topographic Service (Bulgaria)":"milTopoServiceBulgaria", 
        "National Army Topographic Service (Moldova)":"natArmyTopoServiceMoldova", 
        "Swedish Armed Forces (Sweden)":"swedishArmedForces", 
        "Topographic Institute (Slovakia)":"topoInstituteSlovakia", 
        "Topographic Service (Finland)":"topoServiceFinland", 
        "U.S. Africa Command (USAFRICOM)":"usAfricaCommand", 
        "U.S. Air Force":"usAirForce", 
        "U.S. Army Geospatial Center (AGC)":"usArmyGeospatialCenter", 
        "U.S. Army":"usArmy", 
        "U.S. Central Command (USCENTCOM)":"usCentralCommand", 
        "U.S. Central Intelligence Agency (CIA)":"usCentralIntelAgency", 
        "U.S. Coast Guard":"usCoastGuard", 
        "U.S. Defense Intelligence Agency (DIA)":"usDefenseIntelAgency", 
        "U.S. Department of Energy (DOE)":"usDeptOfEnergy", 
        "U.S. Department of Homeland Security (DHS)":"usDeptOfHomelandSecurity", 
        "U.S. Department of State":"usDeptOfState", 
        "U.S. European Command (USEUCOM)":"usEuropeanCommand", 
        "U.S. Federal Bureau of Investigation (FBI)":"usFedBurOfInvestigation", 
        "U.S. Geological Survey (USGS)":"usGeologicalSurvey", 
        "U.S. Joint Forces Command (USJFCOM)":"usJointForcesCommand", 
        "U.S. Marine Corps":"usMarineCorps", 
        "U.S. National Civil Applications Program (NCAP)":"usNatCivilAppsProgram", 
        "U.S. National Geospatial-Intelligence Agency (NGA)":"usNationalGeoIntelAgency", 
        "U.S. National Reconnaissance Office (NRO)":"usNationalReconnOffice", 
        "U.S. National Security Agency (NSA)":"usNationalSecurityAgency", 
        "U.S. National Oceanic and Atmospheric Administration":"usNatOceanAtmosAdmin", 
        "U.S. Navy":"usNavy", 
        "U.S. Northern Command (USNORTHCOM)":"usNorthernCommand", 
        "U.S. Pacific Command (PACOM)":"usPacificCommand", 
        "U.S. Southern Command (USSOUTHCOM)":"usSouthernCommand", 
        "U.S. Special Operations Command (USSOCOM)":"usSpecialOperCommand", 
        "U.S. Strategic Command (USSTRATCOM)":"usStrategicCommand", 
        "U.S. Transportation Command (USTRANSCOM)":"usTransportationCommand" }

    # We could push this into the enumDict but it is easy to just print it
    for i in rcoList:
        enumDict['ZI004_RCG'][rcoList[i]] = ['Process Step Information : Resource Content Originator',i] 
        enumDict['RCG'][rcoList[i]] = ['Resource Content Originator',i] # WHY?????
# End dumpRCO

# Bleeh!
def dumpResOwn():
    resOwnList = {
			"FGI":"Foreign Government Information", "ABW":"Aruba", "AFG":"Afghanistan",
			"AGO":"Angola", "AIA":"Anguilla", "ALA":"Aland Islands", "ALB":"Albania",
			"AND":"Andorra", "ANT":"Netherlands Antilles", "ARE":"United Arab Emirates",
			"ARG":"Argentina", "ARM":"Armenia", "ASM":"American Samoa", "ATA":"Antarctica",
			"ATF":"French Southern Territories", "ATG":"Antigua and Barbuda", "AUS":"Australia",
			"AUT":"Austria", "AZE":"Azerbaijan", "BDI":"Burundi", "BEL":"Belgium",
			"BEN":"Benin", "BFA":"Burkina Faso", "BGD":"Bangladesh", "BGR":"Bulgaria",
			"BHR":"Bahrain", "BHS":"Bahamas", "BIH":"Bosnia and Herzegovina",
			"BLM":"Saint Barthelemy", "BLR":"Belarus", "BLZ":"Belize", "BMU":"Bermuda",
			"BOL":"Bolivia", "BRA":"Brazil", "BRB":"Barbados", "BRN":"Brunei Darussalam",
			"BTN":"Bhutan", "BVT":"Bouvet Island", "BWA":"Botswana",
			"CAF":"Central African Republic", "CAN":"Canada", "CCK":"Cocos (Keeling) Islands",
			"CHE":"Switzerland", "CHL":"Chile", "CHN":"China", "CIV":"Cote dIvoire",
			"CMR":"Cameroon", "COD":"Congo The Democratic Republic of the", "COG":"Congo",
			"COK":"Cook Islands", "COL":"Colombia", "COM":"Comoros", "CPV":"Cape Verde",
			"CRI":"Costa Rica", "CUB":"Cuba", "CXR":"Christmas Island", "CYM":"Cayman Islands",
			"CYP":"Cyprus", "CZE":"Czech Republic", "DEU":"Germany", "DJI":"Djibouti",
			"DMA":"Dominica", "DNK":"Denmark", "DOM":"Dominican Republic", "DZA":"Algeria",
			"ECU":"Eucador", "EGY":"Egypt", "ERI":"Eritrea", "ESH":"Western Sahara",
			"ESP":"Spain", "EST":"Estonia", "ETH":"Ethiopia", "FIN":"Finland", "FJI":"Fiji",
			"FLK":"Falkland Islands (Malvinas)", "FRA":"France", "FRO":"Faroe Islands",
			"FSM":"Micronesia", "GAB":"Gabon", "GBR":"United Kingdom", "GEO":"Georgia",
			"GGY":"Guernsey", "GHA":"Ghana", "GIB":"Gibraltar", "GIN":"Guinea",
			"GLP":"Guadeloupe", "GMB":"Gambia", "GNB":"Guinea-Bissau", "GNQ":"Equatorial Guinea",
			"GRC":"Greece", "GRD":"Grenada", "GRL":"Greenland", "GTM":"Guatemala",
			"GUF":"French Guiana", "GUM":"Guam", "GUY":"Guyana", "HKG":"Hong Kong",
			"HMD":"Heard Island and McDonald Islands", "HND":"Honduras", "HRV":"Croatia",
			"HTI":"Haiti", "HUN":"Hungary", "IDN":"Indonesia", "IMN":"Isle of Man", "IND":"India",
			"IOT":"British Indian Ocean Territory", "IRL":"Ireland", "IRN":"Iran", "IRQ":"Iraq",
			"ISL":"Iceland", "ISR":"Israel", "ITA":"Italy", "JAM":"Jamaica", "JEY":"Jersey",
			"JOR":"Jordan", "JPN":"Japan", "KAZ":"Kazakhstan", "KEN":"Kenya", "KGZ":"Kyrgyzstan",
			"KHM":"Cambodia", "KIR":"Kiribati", "KNA":"Saint Kitts and Nevis",
			"KOR":"Korea Republic of", "KWT":"Kuwait", "LAO":"Lao Peoples Democratic Republic",
			"LBN":"Lebanon", "LBR":"Liberia", "LBY":"Libyan Arab Jamahiriya", "LCA":"Saint Lucia",
			"LIE":"Liechtenstein", "LKA":"Sri Lanka", "LSO":"Lesotho", "LTU":"Lithuania",
			"LUX":"Luxembourg", "LVA":"Latvia", "MAC":"Macao", "MAF":"Saint Martin (French part)",
			"MAR":"Morocco", "MCO":"Monaco", "MDA":"Moldova (the Republic of)", "MDG":"Madagascar",
			"MDV":"Maldives", "MEX":"Mexico", "MHL":"Marshall Islands", "MKD":"Macedonia",
			"MLI":"Mali", "MLT":"Malta", "MMR":"Myanmar", "MNE":"Montenegro", "MNG":"Mongolia",
			"MNP":"Northern Mariana Islands", "MOZ":"Mozambique", "MRT":"Mauritania",
			"MSR":"Montserrat", "MTQ":"Martinique", "MUS":"Mauritius", "MWI":"Malawi",
			"MYS":"Malaysia", "MYT":"Mayotte", "NAM":"Namibia", "NCL":"New Caledonia",
			"NER":"Niger", "NFK":"Norfolk Island", "NGA":"Nigeria", "NIC":"Nicaragua", "NIU":"Niue",
			"NLD":"Netherlands", "NOR":"Norway", "NPL":"Nepal", "NRU":"Nauru", "NZL":"New Zealand",
			"OMN":"Oman", "PAK":"Pakistan", "PAN":"Panama", "PCN":"Pitcairn", "PER":"Peru",
			"PHL":"Philippines", "PLW":"Palau", "PNG":"Papua New Guinea", "POL":"Poland",
			"PRI":"Puerto Rico", "PRK":"Korea Democratic Peoples Republic of", "PRT":"Portugal",
			"PRY":"Paraguay", "PSE":"Palestinian Territory", "PYF":"French Polynesia",
			"QAT":"Qatar", "REU":"Reunion", "ROU":"Romania", "RUS":"Russian Federation",
			"RWA":"Rwanda", "SAU":"Saudi Arabia", "SDN":"Sudan", "SEN":"Senegal", "SGP":"Singapore",
			"SGS":"South Georgia and the South Sandwich Islands", "SHN":"Saint Helena",
			"SJM":"Svalbard and Jan Mayen", "SLB":"Solomon Islands", "SLE":"Sierra Leone",
			"SLV":"El Salvador", "SMR":"San Marino", "SOM":"Somalia",
			"SPM":"Saint Pierre and Miquelon", "SRB":"Serbia", "STP":"Sao Tome and Principe",
			"SUR":"Suriname", "SVK":"Slovakia", "SVN":"Slovenia", "SWE":"Sweden", "SWZ":"Swaziland",
			"SYC":"Seychelles", "SYR":"Syrian Arab Republic", "TCA":"Turks and Caicos Islands",
			"TCD":"Chad", "TGO":"Togo", "THA":"Thailand", "TJK":"Tajikistan", "TKL":"Tokelau",
			"TKM":"Turkmenistan", "TLS":"Timor-Leste", "TON":"Tonga", "TTO":"Trinidad and Tobago",
			"TUN":"Tunisia", "TUR":"Turkey", "TUV":"Tuvalu", "TWN":"Taiwan", "TZA":"Tanzania",
			"UGA":"Uganda", "UKR":"Ukraine", "UMI":"United States Minor Outlying Islands",
			"URY":"Uruguay", "USA":"United States", "UZB":"Uzbekistan",
			"VAT":"Holy See (Vatican City State)", "VCT":"Saint Vincent and the Grenadines",
			"VEN":"Venezuela", "VGB":"Virgin Islands British", "VIR":"Virgin Islands U.S.",
			"VNM":"Viet Nam", "VUT":"Vanuatu", "WLF":"Wallis and Futuna", "WSM":"Samoa",
			"YEM":"Yemen", "ZAF":"South Africa", "ZMB":"Zambia", "ZWE":"Zimbabwe",
			"ACGU":"FOUR EYES", "APFS":"Suppressed 1", "BWCS":"Biological Weapons Convention States",
			"CFCK":"ROK/US Combined Forces Command", "CMFC":"Combined Maritime Forces",
			"CMFP":"Cooperative Maritime Forces Pacific",
			"CPMT":"Civilian Protection Monitoring Team for Sudan",
			"CWCS":"Chemical Weapons Convention States",
			"EFOR":"European Union Stabilization Forces in Bosnia", "EUDA":"European Union DARFUR",
			"FVEY":"FIVE EYES", "GCTF":"Global Counter-Terrorism Forces",
			"GMIF":"Global Maritime Interception Forces",
			"IESC":"International Events Security Coalition",
			"ISAF":"International Security Assistance Force for Afghanistan",
			"KFOR":"Stabilization Forces in Kosovo", "MCFI":"Multinational Coalition Forces - Iraq",
			"MIFH":"Multinational Interim Force Haiti", "MLEC":"Multi-Lateral Enduring Contingency",
			"NACT":"North African Counter-Terrorism Forces", "NATO":"North Atlantic Treaty Organization",
			"SPAA":"Suppressed 2", "TEYE":"THREE EYES", "UNCK":"United Nations Command"
            }

    for i in resOwnList:
        enumDict['ZSAX_RX4'][i] = ['Restriction Information : Security Attributes Group (resource owner-producer)',resOwnList[i]] 
# End dumpResOwn

# So Very Very Ugly
def dumpFFN():
    ffnList = {
        "Agriculture":"2", "Growing of Crops":"3", "Raising of Animals":"9", "Grazing":"14", 
        "Forest Warden":"27", "Aquaculture":"35", "Mining and Quarrying":"40", "Ore Dressing":"95", 
        "Manufacturing":"99", "Food Product Manufacture":"100", "Food Processing":"101", 
        "Meat Processing":"102", "Seafood Processing":"103", "Fruit and/or Vegetable Processing":"104", 
        "Oil-mill":"105", "Dairying":"106", "Grain Milling":"107", "Baking":"110", 
        "Sugar Manufacture":"111", "Sugar Milling":"112", "Sugar Refining":"113", 
        "Confection Manufacture":"114", "Pasta Manufacture":"115", "Prepared Meal Manufacture":"116", 
        "Animal Feed Manufacture":"119", "Ice Manufacture":"120", "Beverage Manufacture":"118", 
        "Spirit Distillery":"121", "Winery":"122", "Brewing":"123", "Soft Drink Manufacture":"124", 
        "Tobacco Product Manufacture":"125", "Textile, Apparel and Leather Manufacture":"129", 
        "Textile Manufacture":"130", "Apparel Manufacture":"140", "Leather Product Manufacture":"150", 
        "Footwear Manufacturing":"155", "Wood-based Manufacturing":"160", "Sawmilling":"161", 
        "Wooden Construction Product Manufacture":"165", "Paper-mill":"171", "Printing":"181", 
        "Petroleum and Coal Products Manufacturing":"190", "Petroleum Refining":"192", 
        "Coke Manufacture":"191", "Chemical Manufacture":"195", "Medicinal Product Manufacture":"210", 
        "Rubber Product Manufacture":"221", "Plastic Product Manufacture":"225", 
        "Nonmetallic Mineral Product Manufacturing":"230", "Glass Product Manufacture":"231", 
        "Refractory Product Manufacture":"232", "Clay Product Manufacture":"233", 
        "Ceramic Product Manufacture":"234", "Cement Mill":"235", "Cement Product Manufacture":"236", 
        "Stone Product Manufacture":"237", "Primary Metal Manufacturing":"240", "Steel Mill":"241", 
        "Metal Refining":"242", "Foundry":"243", "Metal Product Manufacturing":"250", 
        "Structural Metal Product Manufacture":"251", "Fabricated Metal Product Manufacture":"257", 
        "Munitions Manufacture":"255", "Electronic Equipment Manufacture":"260", 
        "Electrical Equipment Manufacture":"270", "Machinery Manufacture":"280", 
        "Transportation Equipment Manufacturing":"289", "Motor Vehicle Manufacture":"290", 
        "Ship Construction":"301", "Railway Vehicle Manufacture":"304", "Aircraft Manufacture":"305", 
        "Military Vehicle Manufacture":"306", "Furniture Manufacture":"310", 
        "Miscellaneous Manufacturing":"320", "Jewellery Manufacture":"321", 
        "Musical Instrument Manufacture":"322", "Sports Goods Manufacture":"323", 
        "Game and/or Toy Manufacture":"324", "Medical and/or Dental Equipment Manufacture":"325", 
        "General Repair":"330", "Fabricated Metal Product Repair":"331", 
        "Electronic Equipment Repair":"332", "Electrical Equipment Repair":"333", 
        "Machinery Repair":"334", "Railway Vehicle Repair":"342", "Motor Vehicle Repair":"343", 
        "Ship Repair":"340", "Aircraft Repair":"341", "Utilities":"350", "Power Generation":"351", 
        "Climate Control":"352", "Cooling":"355", "Heating":"356", "Water Supply":"360", 
        "Water Treatment":"362", "Water Distribution":"363", "Sewerage":"370", 
        "Sewerage Screening":"372", "Restroom":"382", "Waste Treatment and Disposal":"383", 
        "Materials Recovery":"385", "Commerce":"440", "Wholesale Merchant":"459", "Retail Sale":"460", 
        "Specialized Store":"464", "Petrol Sale":"470", "Propane Sale":"272", 
        "Precious Metal Merchant":"474", "Pharmacy":"477", "Pet-shop":"478", 
        "Non-specialized Store":"465", "Convenience Store":"466", "Grocery":"476", "Market":"475", 
        "Sales Yard":"473", "Transport":"480", "Transportation Hub":"489", "Station":"482", 
        "Stop":"483", "Terminal":"481", "Transfer Hub":"484", "Railway Transport":"490", 
        "Railway Passenger Transport":"491", "Pedestrian Transport":"494", "Road Transport":"495", 
        "Road Freight Transport":"497", "Road Passenger Transport":"496", "Pipeline Transport":"500", 
        "Pumping":"501", "Water Transport":"505", "Inland Waters Transport":"507", 
        "Canal Transport":"508", "Harbour Control":"513", "Port Control":"510", 
        "Maritime Pilotage":"511", "Pilot Station":"512", "Air Transport":"520", 
        "Air Traffic Control":"525", "Mail and Package Transport":"541", "Postal Activities":"540", 
        "Courier Activities":"545", "Transportation Support":"529", "Navigation":"488", 
        "Signalling":"486", "Transport System Maintenance":"487", "Warehousing and Storage":"530", 
        "Motor Vehicle Parking":"535", "Cargo Handling":"536", "Customs Checkpoint":"537", 
        "Inspection Station":"538", "Inspection":"539", "Short-term Accommodation":"548", 
        "Accommodation":"550", "Guest-house":"554", "Hotel":"551", "Resort Hotel":"552", "Motel":"553", 
        "Hostel":"555", "Vacation Cottage":"557", "Long-term Accommodation":"549", "Dormitory":"556", 
        "Dependents Housing":"558", "Residence":"563", "Food Service":"570", "Restaurant":"572", 
        "Bar":"573", "Dining Hall":"574", "Banquet Hall":"578", "Convention Centre":"579",
        "Publishing and Broadcasting":"580", "Print Publishing":"582", "Radio Broadcasting":"601", 
        "Television Broadcasting":"604", "Information Service":"632", "Public Records":"633", 
        "Telecommunications":"610", "Retail Telecommunications":"612", "Wired Telecommunications":"614", 
        "Main Telephone Exchange":"615", "Branch Telephone Exchange":"616", "Wired Repeater":"617", 
        "Wireless Telecommunications":"620", "Mobile Phone Service":"621", "Wireless Repeater":"622", 
        "Satellite Telecommunications":"625", "Satellite Ground Control":"626", 
        "Financial Services":"640", "Central Banking":"642", "Retail Banking":"643", "Insurance":"651", 
        "Financial Market Administration":"662", "Security Brokerage":"663", "Fund Management":"671", 
        "Real Estate Activities":"680", "Professional, Scientific and Technical":"681", 
        "Accounting":"696", "Advertising":"741", "Architecture Consulting":"711", 
        "Business Management":"706", "Engineering Design":"714", "Head Office":"701", 
        "Legal Activities":"691", "Photography":"752", "Scientific Research and Development":"720", 
        "Nuclear Research Centre":"725", "Observation Station":"721", "Weather Station":"722", 
        "Wind Tunnel":"730", "Surveying":"717", "Veterinary":"757", 
        "Business and Personal Support Services":"760", "Administration":"810", "Animal Boarding":"919", 
        "Beauty Treatment":"962", "Call Centre":"807", "Custodial Service":"791", 
        "Employment Agency":"770", "Headquarters":"809", "Landscaping Service":"795", 
        "Laundry":"961", "Motor Vehicle Rental":"761", "Office Administration":"801", 
        "Travel Agency":"775", "Public Administration":"808", "Government":"811", 
        "National Government":"814", "Subnational Government":"813", "Local Government":"812", 
        "Executive Activities":"818", "Legislative Activities":"819", "Civil Activities":"822", 
        "Capitol":"817", "Palace":"815", "Polling Station":"821", "Diplomacy":"825", "Consul":"828", 
        "Diplomatic Mission":"826", "Embassy":"827", "Defence Activities":"835", "Armory":"836", 
        "Maritime Defense":"829", "Military Recruitment":"838", "Military Reserve Activities":"837", 
        "Public Order, Safety and Security Services":"830", "Public Order":"831", 
        "Immigration Control":"842", "Imprisonment":"843", "Judicial Activities":"840", 
        "Juvenile Corrections":"844", "Law Enforcement":"841", "Safety":"832", 
        "Firefighting":"845", "Rescue and Paramedical Services":"846", 
        "Emergency Operations":"847", "Emergency Relief Services":"888", 
        "Civil Intelligence":"848", "CBRNE Civilian Support":"839", "Security Services":"833", 
        "Guard":"781", "Security Enforcement":"780", "Education":"850", 
        "Primary Education":"851", "Secondary Education":"852", "Higher Education":"855", 
        "Vocational Education":"857", "Human Health Activities":"860", "In-patient Care":"861", 
        "Intermediate Care":"871", "Psychiatric In-patient Care":"873", 
        "Residential Care":"875", "Out-patient Care":"862", "Urgent Medical Care":"863", 
        "Human Tissue Repository":"864", "Leprosy Care":"866", "Public Health Activities":"865", 
        "Social Work":"887", "Day Care":"885", "Emergency Shelter":"881", 
        "Emergency Youth Shelter":"884", "Homeless Shelter":"882", "Refugee Shelter":"883", 
        "Cultural, Arts and Entertainment":"890", "Aquarium":"906", "Auditorium":"892", 
        "Botanical and/or Zoological Reserve Activities":"907", "Cinema":"594", "Library":"902", 
        "Museum":"905", "Night Club":"895", "Opera House":"894", "Theatre":"891", 
        "Sports, Amusement and Recreation":"900", "Adult Entertainment":"966", "Amusement":"922", 
        "Fitness Centre":"913", "Gambling":"909", "Hobbies and/or Leisure Activities":"923", 
        "Recreation":"921", "Shooting Range":"914", "Sports Centre":"912", 
        "Religious Activities":"930", "Place of Worship":"931", "Islamic Prayer Hall":"932", 
        "Membership Organization":"950", "Club":"954", "Yacht-club":"955", 
        "Death Care Services":"980", "Funeral Services":"963", "Cremation":"964", 
        "Mortuary Services":"965", "Storage of Human Remains":"967", "Meeting Place":"970", 
        "Community Centre":"893", "Other":"999", "No Information":"-999999" 
     }
    for i in ffnList:
        # This list accounts for the "Hierarchical Enumerants" in the spec that link to a 
        # seperate page of FFN codes.
        # Could probably do a test to see if it is already in the list before jamming it in
        enumDict['FFN'][ffnList[i]] = ['Feature Function',i] 
        enumDict['FFN2'][ffnList[i]] = ['Feature Function [2]',i] 
        enumDict['FFN3'][ffnList[i]] = ['Feature Function [3]',i] 

        #enumDict['FFN']['enum'][int(ffnList[i])] = i
        #enumDict['FFN2']['enum'][int(ffnList[i])] = i
        #enumDict['FFN3']['enum'][int(ffnList[i])] = i
# End dumpFFN



######
# Main Starts Here
# 
parser = argparse.ArgumentParser(description='Process NFDD.csv file and build data tables')
parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
parser.add_argument('mainfile', help='The main NFDD csv file', action='store')

args = parser.parse_args()

main_csv_file = args.mainfile

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

singleDict = {}
enumDict = {}
fcodeDict = {}
defaultDict = {}
attrDict = {}

# Add the eTDS attributes so they get converted to english
singleDict['SCAMIN'] = 'eLTDS: Minimum Scale'
defaultDict['SCAMIN'] = '-999999'
singleDict['SCAMAX'] = 'eLTDS: Maximum Scale'
defaultDict['SCAMAX'] = '-999999'
singleDict['LINK_ID'] = 'eLTDS: LINK_ID'
defaultDict['LINK_ID'] = 'No Information'

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
        if fName == '': # Skip blank lines
            continue

        if aName == '':
            if inFeature:
                if inEnum:
                    inEnum = False

            inFeature = True # grubby but it works, could flip the loop around?
            inAttrib = False

            (geomType,tmpName) = fName.split(':')
            #geomType = geomType[-1]

            tmpName = tmpName.lstrip()
            (fullName,fCode) = tmpName.split('  ')
            fullName = fullName.strip()
            fCode = fCode[2:-1] # Funky but it works

            #fcodeDict[fCode] = fullName.replace("'","")
            fcodeDict[fCode] = {'desc':fullName.replace("'",""), 'enum': [] }

            # backward compatibility for a while
            #geoList = {'P':'Point', 'S':'Surface', 'C':'Curve'}
            #geoList = {'P':'Point', 'S':'Area', 'C':'Line'}
            #geomType = geoList[geomType]
            #print '          geom:"%s",' % (geomType)
            continue

        if dType == 'Enumeration' or dType == 'CodeList':
            inAttrib = True
            inEnum = True
            enumName = aName # Save for the list
            enumDesc = aValue

            if not aName in enumDict:
                enumDict[aName] = {}

            if not aName in fcodeDict[fCode]['enum']: 
                fcodeDict[fCode]['enum'].append(aName) 

            #print '                     { name:"%s",' % (aName);
            #print '                       desc:"%s",' % (aValue);

            if aName in ['ZI004_RCG','ZI020_IC2','ZI001_NSP','ZI001_SSY','ZI001_VSC','CPS','EQC','ETS','HZD','IC2','RCG','VDT']: 
                defaultDict[aName] = 'noInformation' 

            elif aName in ['GSGCHC','GSGCLC','RMWC','SGCC','BPWHAC','BPWHBC','BPWSAC','BPWSBC','BWVCAC','BWVCBC']: 
                defaultDict[aName] = '5' 

            elif aName in ['ZSAX_RS0']: 
                defaultDict[aName] = 'U' # Best guess 

            else: 
                defaultDict[aName] = '-999999' 

            continue

        if dType == '' or dType == 'Local specification':
            if aValue == 'Hierarchical Enumerants':
                continue

            if dType == 'Local specification':
                if aValue.find('metadata.ces') > -1:
                    continue
            else:
                (eValue,eName) = aValue.split("=")
                eValue = eValue.strip()
                eName = eName.strip()
                enumDict[enumName][eValue] = [enumDesc,eName] 
                #if eValue == 'other' or eValue == 'noInformation':
                    #enumDict[enumName]['enum'][eValue] = eName
                    #enumDict[enumName][eValue] = [enumDesc,eName] 
                #else: 
                    #enumDict[enumName]['enum'][int(eValue)] = eName
                    #enumDict[enumName][int(eValue)] = [enumDesc,eName] 
                #print '                           { name:"%s", value:"%s" },' % (eName.strip(),eValue.strip())
                #print '[name:"%s", value:"%s" },' % (eName.strip(),eValue.strip())
                continue
        
        if dType in ['Double','LongInteger']:
            inAttrib = True
            inEnum = False
            #print '                     { name:"%s",' % (aName)
            #print '                       desc:"%s",' % (aValue)
            if not aName in fcodeDict[fCode]['enum']: 
                fcodeDict[fCode]['enum'].append(aName) 

            if not aName in singleDict:
                singleDict[aName] = aValue

            if dType == 'Double': 
                defaultDict[aName] = '-999999.0'
            else: 
                defaultDict[aName] = '-999999'

            continue

        if dType.find('Text') > -1:
            inAttrib = True
            inEnum = False

            # These are text enumerations but they get sucked into the Text Single 
            if aName == 'ZSAX_RX3': 
                if not aName in enumDict:
                    enumDict[aName] = {}

                defaultDict[aName] = 'noInformation' # TDS v52 & TDSv60
                continue 

            if aName == 'ZSAX_RX4': 
                if not aName in enumDict:
                    enumDict[aName] = {}

                defaultDict[aName] = 'USA' # Not great but stops GAIT errors
                continue

            #print '                     { name:"%s",' % (aName)
            #print '                       desc:"%s",' % (aValue)
            if not aName in fcodeDict[fCode]['enum']: 
                fcodeDict[fCode]['enum'].append(aName) 

            if not aName in singleDict:
                singleDict[aName] = aValue
            if aName == 'OTH': 
                defaultDict[aName] = 'No Information' # TDSv40
            else: 
                defaultDict[aName] = 'noInformation' # To help GAIT

            continue
        
        # Bit of sanity checking. If this prints, we have done something wrong
        print "##### fName: %s  aName: %s  aValue: %s" % (fName,aName,aValue)
    # End of For
finally:
    csvfile.close()


# Push the Extra values into the enumDict
dumpISMC()
dumpRCO()
dumpFFN()  
dumpResOwn()  
dumpNonIc()
dumpNSP()
dumpSSY()
dumpVSC()
dumpCPS()
dumpEQC()
dumpETS()
dumpHZD()
dumpVDT()
dumpTrigraph()


# Now print out the Javascript structure
printHeader()
printEnum()
printSingle()
printDefault()
printFcode()
printFooter()

# End
