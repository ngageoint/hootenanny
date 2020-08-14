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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

/*
    FMIv1 One2one rules
*/

enc311.rules = {
  // ##### Start of One2One Rules #####

  // Layer rules for Import
  fCodeOne2oneIn : [
    // ['F_CODE','AP030','highway','road'], // Road
  ], // End fcodeOne2oneIn


  // These are for finding a layer for export.
  // Note: These get swapped around before being used
  fCodeOne2one : [

  ], // End fcodeOne2oneOut


  // Additional rules for FCODES
  fCodeOne2oneOut : [
  ], // End fcodeOne2oneOut


  // One2one rules for Text Fields
  txtBiased : {
    // '$CHARS':'raw:$CHARS', // Character specification
    // '$NTXST':'raw:$NTXST', // Text string in national language
    // '$SCODE':'raw:$SCODE', // Symbolization code
    // '$TXSTR':'raw:$TXSTR', // Text string
    // 'AGENCY':'raw:AGENCY', // Agency responsible for production
    'CALSGN':'seamark:radio_station:callsign', // Call sign
    'CLSDEF':'s57:class_definition', // Object class definition
    'CLSNAM':'s57:class_name', // Object class name
    'COMCHA':'COMCHA', // Communication channel
    // 'CPDATE':'raw:CPDATE', // Compilation date
    'DATEND':'end_date', // Date end
    'DATSTA':'start_date', // Date start
    'FFPT_RIND':'s57:relationship_indicators', // Relationship Indicators
    'INFORM':'note', // Information
    'LNAM':'s57:long_name', // Long Name
    'LNAM_REFS':'s57:long_name_reference', // Long Name Reference List
    'NATION':'addr:country', // Nationality
    'NINFOM':'note:national_language', // Information in national language
    // 'NMDATE':'raw:NMDATE', // Notice to Mariners date
    'NOBJNM':'name:national_language', // Object name in national language
    'NPLDST':'pilot_district:national_language', // Pilot district in national language
    'NTXTDS':'note:formatted:national_language', // Textual description in national language
    'OBJNAM':'name', // Object name
    'PEREND':'end_date:periodic', // Periodic date end
    'PERSTA':'start_date:periodic', // Periodic date start
    'PICREP':'PICREP', // Pictorial representation
    'PILDST':'pilot_district', // Pilot district
    // 'PRCTRY':'raw:PRCTRY', // Producing country
    'PUBREF':'publication_reference', // Publication reference
    'RADWAL':'RADWAL', // Radar wave length
    // 'RECDAT':'raw:RECDAT', // Recording date
    // 'RECIND':'raw:RECIND', // Recording indication
    'RYRMGV':'magnetic_variation:date', // Reference year for magnetic variation
    'SHIPAM':'datum_shift_parameters', // Shift parameters
    'SIGGRP':'SIGGRP', // Signal group
    'SIGSEQ':'SIGSEQ', // Signal sequence
    'SORDAT':'source:date', // Source date
    'SORIND':'source', // Source indication
    'SURATH':'source:survey_authority', // Survey authority
    'SUREND':'source:survey_date:end', // Survey date - end
    'SURSTA':'source:survey_date:start', // Survey date - start
    'SYMINS':'s57:symbol', // Symbol instruction
    'T_HWLW':'tide:high_low_water', // Tide - high and low water values
    'T_THDF':'tide:time_height_difference', // Tide - time and height differences
    'T_TSVL':'tide:tide_time_series_values', // Tide - time series values
    'T_VAHC':'tide:harmonic_constituents', // Tide - value of harmonic constituents
    'TIMEND':'end_time', // Time end
    'TIMSTA':'start_time', // Time start
    'TS_TSP':'tidal_stream:panel_values', // Tidal stream - panel values
    'TS_TSV':'tidal_stream:current_time_series_values', // Tidal stream, current - time series values
    'TXTDSC':'note:formatted', // Textual description
  }, // End txtBiased


  // One2one rules for Number fields
  numBiased : {
    // '$CSIZE':'raw:$CSIZE', // Compass size
    // '$SCALE':'raw:$SCALE', // Symbol scaling factor
    'BURDEP':'BURDEP', // Buried depth
    'CSCALE':'compilation_scale', // Compilation scale
    'CURVEL':'water:velocity:average', // Current velocity
    'DRVAL1':'DRVAL1', // Depth range value 1
    'DRVAL2':'DRVAL2', // Depth range value 2
    'ELEVAT':'ELEVAT', // Elevation
    'ESTRNG':'ESTRNG', // Estimated range of transmission
    'FIDN':'s57:feature_identification_number', // Feature Identification Number
    'FIDS':'s57:feature_identification_subdivision', // Feature Identification Subdivision
    'HEIGHT':'HEIGHT', // Height
    'HORACC':'source:accuracy:horizontal', // Horizontal accuracy
    'HORCLR':'HORCLR', // Horizontal clearance
    'HORLEN':'HORLEN', // Horizontal length
    'HORWID':'HORWID', // Horizontal width
    'ICEFAC':'clearance:ice_factor', // Ice factor
    'LIFCAP':'LIFCAP', // Lifting capacity
    'MLTYLT':'MLTYLT', // Multiplicity of lights
    'ORIENT':'ORIENT', // Orientation
    'POSACC':'source:accuracy:positional', // Positional Accuracy
    'RADIUS':'RADIUS', // Radius
    'RCID':'s57:record_id', // Record Id
    'RVER':'s57:record_version', // Record Version
    // 'SCAMAX':'cartographic_scale:upper', // Scale maximum - Not to be used for ENC
    'SCAMIN':'cartographic_scale:lower', // Scale minimum
    'SCVAL1':'source:scale:largest', // Scale value one
    'SCVAL2':'source:scale:smallest', // Scale value two
    'SDISMN':'sounding:distance_minimum', // Sounding distance - minimum
    'SDISMX':'sounding:distance_maximum', // Sounding distance - maximum
    'SECTR1':'SECTR1', // Sector limit one
    'SECTR2':'SECTR2', // Sector limit two
    'SIGFRQ':'SIGFRQ', // Signal frequency
    'SIGPER':'SIGPER', // Signal period
    'SOUACC':'sounding:accuracy', // Sounding accuracy
    'T_TINT':'tide:current_time_interval', // Tide, current - time interval of values
    'VALACM':'magnetic_variation:annual_change', // Value of annual change in magnetic variation
    'VALDCO':'depth:value', // Value of depth contour
    'VALLMA':'magnetic_variation:local', // Value of local magnetic anomaly
    'VALMAG':'magnetic_variation', // Value of magnetic variation
    'VALMXR':'VALMXR', // Value of maximum range
    'VALNMR':'VALNMR', // Value of nominal range
    'VALSOU':'depth', // Value of sounding
    'VERACC':'source:accuracy:vertical', // Vertical accuracy
    'VERCCL':'VERCCL', // Vertical clearance, closed
    'VERCLR':'VERCLR', // Vertical clearance
    'VERCOP':'VERCOP', // Vertical clearance, open
    'VERCSA':'VERCSA', // Vertical clearance, safe
    'VERLEN':'VERLEN', // Vertical length
  }, // End numBiased


  // Common one2one rules. Used for both import and export
  // NOTE: If it is commented out then it was valid but isn't currently valid in ENC 
  one2one : [
    // PRIM - Primitive type
    ['PRIM','1','s57:primitive','Point'], // Point
    ['PRIM','2','s57:primitive','Line'], // Line
    ['PRIM','3','s57:primitive','Area'], // Area
    ['PRIM','255','s57:primitive','no_geometry'],

    // $JUSTH - Justification - horizontal
    // ['$JUSTH','1','raw:$JUSTH','centre_justified'], // centre justified
    // ['$JUSTH','2','raw:$JUSTH','right_justified'], // right justified
    // ['$JUSTH','3','raw:$JUSTH','left_justified'], // left justified

    // $JUSTV - Justification - vertical
    // ['$JUSTV','1','raw:$JUSTV','bottom_justified'], // bottom justified
    // ['$JUSTV','2','raw:$JUSTV','centre_justified'], // centre justified
    // ['$JUSTV','3','raw:$JUSTV','top_justified'], // top justified

    // $SPACE - Character spacing
    // ['$SPACE','1','raw:$SPACE','expanded/condensed'], // expanded/condensed
    // ['$SPACE','2','raw:$SPACE','standard'], // standard

    // $TINTS - Tint
    // ['$TINTS','1','raw:$TINTS','darkest_blue'], // darkest blue
    // ['$TINTS','2','raw:$TINTS','medium_blue'], // medium blue
    // ['$TINTS','3','raw:$TINTS','lightest_blue'], // lightest blue

    // AGEN - Agency Code
    ['AGEN','0','attribution','UKHO test and sample datasets'],
    ['AGEN','1','attribution','Argentina - Servicio de Hidrografía Naval (SHN)'],
    ['AGEN','10','attribution','Australia - Australian Hydrographic Service (AHS)'],
    ['AGEN','11','attribution','Australia - Australian Hydrographic Service - Navy and Defence'],
    ['AGEN','20','attribution','Bahrain - Hydrographic Survey Office'],
    ['AGEN','30','attribution','Belgium - MDK – Afdeling Kust – Division Coast'],
    ['AGEN','40','attribution','Brazil - Directorate of Hydrography and Navigation (DHN)'],
    ['AGEN','50','attribution','Canada - Canadian Hydrographic Service (CHS)'],
    ['AGEN','51','attribution','Canada - Canadian Forces'],
    ['AGEN','60','attribution','Chile - Servicio Hidrográfico y Oceanográfico de la Armada (SHOA)'],
    ['AGEN','70','attribution','China - Maritime Safety Administration (MSA)'],
    ['AGEN','71','attribution','China - The Navigation Guarantee Department of The Chinese Navy Headquarters'],
    ['AGEN','72','attribution','China - Hong Kong Special Administrative Region'],
    ['AGEN','73','attribution','China - Macau Special Administrative Region'],
    ['AGEN','80','attribution','Croatia - Hrvatski  Hidrografski Institut'],
    ['AGEN','90','attribution','Cuba - Oficina Nacional de Hidrografia y Geodesia'],
    ['AGEN','100','attribution','Cyprus - Hydrographic Unit of the Department of Lands and Surveys'],
    ['AGEN','110','attribution','Denmark - Geodatastyrelsen (GST)'],
    ['AGEN','111','attribution','Denmark - Farvandsvaesenet'],
    ['AGEN','120','attribution','Dominican Rep. - Instituto Cartografico Militar'],
    ['AGEN','130','attribution','Ecuador - Instituto Oceanográfico de la Armada (INOCAR)'],
    ['AGEN','140','attribution','Egypt - Shobat al Misaha al Baharia'],
    ['AGEN','150','attribution','Fiji - Fiji Islands Maritime Safety Administration (FIMSA)'],
    ['AGEN','160','attribution','Finland - Finnish Transport and Communications Agency (Traficom)'],
    ['AGEN','170','attribution','France - Service Hydrographique et Océanographique de la Marine (SHOM)'],
    ['AGEN','180','attribution','Germany - Bundesamt  für Seeschiffahrt und Hydrographie (BSH)'],
    ['AGEN','190','attribution','Greece - Hellenic Navy Hydrographic Service (HNHS)'],
    ['AGEN','200','attribution','Guatemala - Ministerio de la Defensa Nacional'],
    ['AGEN','201','attribution','Guatemala - Comisión Portuaria Nacional'],
    ['AGEN','210','attribution','Iceland - Icelandic Coast Guard'],
    ['AGEN','220','attribution','India - National Hydrographic Office'],
    ['AGEN','221','attribution','India - Indian Navy Specific'],
    ['AGEN','230','attribution','Indonesia - Jawatan Hidro-Oseanografi (JANHIDROS)'],
    ['AGEN','240','attribution','Islamic Rep. of Iran - Ports and Shipping Organization (PSO)'],
    ['AGEN','250','attribution','Italy - Istituto Idrografico della Marina (IIM)'],
    ['AGEN','260','attribution','Japan - Japan Hydrographic and Oceanographic Department (JHOD)'],
    ['AGEN','270','attribution','Korea (DPR of) - Hydrographic Department'],
    ['AGEN','280','attribution','Korea (Rep. of) - Korea Hydrographic and Oceanographic Agency (KHOA)'],
    ['AGEN','290','attribution','Malaysia - National Hydrographic Centre'],
    ['AGEN','300','attribution','Monaco - Direction des Affaires Maritimes'],
    ['AGEN','310','attribution','Netherlands - Koninklijke Marine Dienst der Hydrografie / CZSK'],
    ['AGEN','320','attribution','New Zealand - Land Information New Zealand (LINZ)'],
    ['AGEN','321','attribution','New Zealand - Geospatial Intelligence New Zealand'],
    ['AGEN','330','attribution','Nigeria - Nigerian Navy Hydrographic Office'],
    ['AGEN','340','attribution','Norway - Norwegian Hydrographic Service'],
    ['AGEN','341','attribution','Norway - Electronic Chart Centre'],
    ['AGEN','342','attribution','Norway - Norwegian Defence'],
    ['AGEN','350','attribution','Oman - National Hydrographic  Office'],
    ['AGEN','360','attribution','Pakistan - Pakistan Hydrographic Department'],
    ['AGEN','370','attribution','Papua New Guinea - Hydrographic Division, National Maritime Safety Authority (NMSA)'],
    ['AGEN','380','attribution','Peru - Dirección  de Hidrografía y Navegación (DHN)'],
    ['AGEN','390','attribution','Philippines - National Mapping and Resource Information Authority, Coast & Geodetic Survey Department'],
    ['AGEN','400','attribution','Poland - Biuro Hydrograficzne'],
    ['AGEN','401','attribution','Poland - Inland Navigation Office in Szczecin (Urzad Zeglugi Srodladowej w Szczecinie)'],
    ['AGEN','410','attribution','Portugal - Instituto Hidrografico, Portugal (IHP)'],
    ['AGEN','420','attribution','Russian Federation - Head Department of Navigation & Oceanography (DNO)'],
    ['AGEN','425','attribution','Russian Federation - Federal State Unitary Hydrographc Department'],
    ['AGEN','430','attribution','Singapore - Hydrographic Department, Maritime and Port Authority (MPA)'],
    ['AGEN','440','attribution','South Africa (Rep. of) - South African Navy Hydrographic Office (SANHO)'],
    ['AGEN','450','attribution','Spain - Instituto Hidrográfico de la Marina (IHM)'],
    ['AGEN','460','attribution','Sri Lanka - National Hydrographic Office, National Aquatic Resources Research and Development Agency (NARA)'],
    ['AGEN','470','attribution','Suriname - Maritieme Autoriteit Suriname (MAS)'],
    ['AGEN','480','attribution','Sweden - Sjöfartsverket, Swedish Maritime Administration'],
    ['AGEN','490','attribution','Syrian Arab Republic - General Directorate of Ports'],
    ['AGEN','500','attribution','Thailand - Hydrographic Department, Royal Thai Navy'],
    ['AGEN','505','attribution','Tonga - Tonga Defence Services'],
    ['AGEN','510','attribution','Trinidad & Tobago - Trinidad & Tobago Hydrographic Unit'],
    ['AGEN','520','attribution','Turkey - Seyir, Hidrografi ve Osinografi Dairesi Baskanligi, Office of Navigation, Hydrography and Oceanography'],
    ['AGEN','530','attribution','United Arab Emirates - Ministry of Communications'],
    ['AGEN','540','attribution','UK - United Kingdom Hydrographic Office'],
    ['AGEN','550','attribution','USA - Office of Coast Survey, National Ocean Service, National Oceanic and Atmospheric Administration (NOS)'],
    ['AGEN','551','attribution','USA - National Geospatial-Intelligence Agency Department of Defense (NGA) U1'],
    ['AGEN','552','attribution','USA - Commander, Naval Meteorology and Oceanography Command (CNMOC)'],
    ['AGEN','553','attribution','USA - U.S. Army Corps of Engineers (USACE)'],
    ['AGEN','554','attribution','USA - National Geospatial-Intelligence Agency Department of Defense (NGA) U4'],
    ['AGEN','560','attribution','Uruguay - Oceanographic, Hydrographic and Meteorological Service of the Uruguayan Navy'],
    ['AGEN','570','attribution','Venezuela - Commandancia General de la Armada, Dirección de Hidrografía y Navegación (DHN)'],
    ['AGEN','580','attribution','Serbia - Direkcija Za Unutrašnje Plovne Puteve'],
    ['AGEN','590','attribution','Congo (Dem. Rep. of) - Ministère des Transports et Communications'],
    ['AGEN','600','attribution','Albania - Albanian Hydrographic Service'],
    ['AGEN','620','attribution','Angola - Not known'],
    ['AGEN','625','attribution','Anguilla - Ministry of Infrastructure, Communications & Utilities'],
    ['AGEN','630','attribution','Antigua and Barbuda - Antigua and Barbuda Port Authority'],
    ['AGEN','640','attribution','Aruba - Netherlands ENC charting responsibility'],
    ['AGEN','645','attribution','Azerbaijan - Azerbaijan Navy'],
    ['AGEN','650','attribution','Bahamas - Port Department, Ministry of Transport and Aviation'],
    ['AGEN','660','attribution','Bangladesh - Hydrographic Department'],
    ['AGEN','670','attribution','Barbados - Barbados Port Inc'],
    ['AGEN','680','attribution','Belize - Belize Port Authority'],
    ['AGEN','690','attribution','Benin - Direction Générale du Port Autonome de Cotonou'],
    ['AGEN','695','attribution','Bermuda - Ministry of Works, Engineering and Housing'],
    ['AGEN','700','attribution','Bolivia - Servicio  Nacional  de  Hidrografia  Naval  de  Bolivia'],
    ['AGEN','705','attribution','British Virgin Islands - Chief Minister’s Office'],
    ['AGEN','710','attribution','Brunei Darussalam - Department of Marine'],
    ['AGEN','715','attribution','Brunei Darussalam - Survey Department'],
    ['AGEN','720','attribution','Bulgaria - Hidrografska Sluzhba Pri Ministerstvo Na Otbranata'],
    ['AGEN','730','attribution','Cambodia - Service de l’Hydrologie et des grands barrages'],
    ['AGEN','740','attribution','Cameroon - Port Autonome de Douala (PAD)'],
    ['AGEN','750','attribution','Cape Verde - Instituto Marityimo e Portuário (IMP)'],
    ['AGEN','755','attribution','The Cayman Islands - Governor’s Office'],
    ['AGEN','760','attribution','Colombia - Ministerio de Defensa Nacional'],
    ['AGEN','770','attribution','Comoros - Not Known'],
    ['AGEN','780','attribution','Congo (Rep. of) - Port Autonome de Pointe Noire'],
    ['AGEN','790','attribution','Cook Islands - Maritime Division, Ministry of Tourism and Transport'],
    ['AGEN','800','attribution','Costa-Rica - Instituto Geografico Nacional (IGN)'],
    ['AGEN','810','attribution','Côte d’Ivoire - Direction Générale du Port Autonome d’Abidjan'],
    ['AGEN','820','attribution','Djibouti - Ministère de l’Equipement et des  Transports, Direction des Affaires Maritimes'],
    ['AGEN','830','attribution','Dominica - Not known'],
    ['AGEN','840','attribution','El Salvador - Gerente de Geodesia, Centro Nacional de Registros, Instituto Geografico y del Catastro Nacional'],
    ['AGEN','850','attribution','Equatorial Guinea - Ministry of Transportation and Civil Aviation'],
    ['AGEN','860','attribution','Eritrea - Department of Marine Transport'],
    ['AGEN','870','attribution','Estonia - Estonian Maritime Administration (EMA)'],
    ['AGEN','880','attribution','Ethiopia - Ministry of Transport and Communications Marine Transport Authority'],
    ['AGEN','890','attribution','Gabon - Direction Générale de la Marine Marchande'],
    ['AGEN','900','attribution','Gambia - Gambia Ports Authority'],
    ['AGEN','905','attribution','Georgia - State Hydrographic Service of Georgia'],
    ['AGEN','910','attribution','Ghana - Ghana Ports and Harbours Authority'],
    ['AGEN','920','attribution','Grenada - Grenada Ports Authority'],
    ['AGEN','930','attribution','Guinea - Port Autonome de Conakry'],
    ['AGEN','940','attribution','Guinea-Bissau - Administração dos Portos da Guiné-Bissau'],
    ['AGEN','950','attribution','Guyana - Maritime Administration Department Hydrographic Office'],
    ['AGEN','960','attribution','Haiti - Service Maritime et de Navigation d’Haïti'],
    ['AGEN','970','attribution','Honduras - Empresa Nacional Portuaria'],
    ['AGEN','980','attribution','Iraq - Marine Department, General  Company  for Iraki Ports'],
    ['AGEN','990','attribution','Ireland - Maritime Safety Directorate'],
    ['AGEN','1000','attribution','Israel - Administration of Shipping and Ports'],
    ['AGEN','1001','attribution','Israel - Survey of Israel'],
    ['AGEN','1002','attribution','Israel - Israel Navy'],
    ['AGEN','1010','attribution','Jamaica - Surveys and Mapping Division'],
    ['AGEN','1020','attribution','Jordan - The Ports Corporation, Jordan'],
    ['AGEN','1030','attribution','Kenya - Survey of Kenya, Kenya  Ports Authority'],
    ['AGEN','1040','attribution','Kiribati - Ministry of Transport and Communications'],
    ['AGEN','1050','attribution','Kuwait - Ministry of Communications'],
    ['AGEN','1060','attribution','Latvia - Maritime Administration of Latvia'],
    ['AGEN','1070','attribution','Lebanon - Ministry of Public Works & Transport'],
    ['AGEN','1080','attribution','Liberia - Ministry of Lands, Mines and Energy'],
    ['AGEN','1090','attribution','Libyan Arab Jamahiriya - Not known'],
    ['AGEN','1100','attribution','Lithuania - Lithuanian Maritime Safety Administration'],
    ['AGEN','1110','attribution','Madagascar - Institut Géographique et Hydrographique National'],
    ['AGEN','1120','attribution','Malawi - Hydrographic Survey Unit'],
    ['AGEN','1121','attribution','Malawi - Marine Department'],
    ['AGEN','1130','attribution','Maldives - Department of Information and Broadcasting'],
    ['AGEN','1140','attribution','Malta - Malta Maritime Authority Ports Directorate, Hydrographic Unit'],
    ['AGEN','1150','attribution','Marshall Islands - Ministry of Resources and Development'],
    ['AGEN','1160','attribution','Mauritania - Ministère de la Défense Nationale'],
    ['AGEN','1170','attribution','Mauritius - Ministry of Housing and Land, Hydrographic Unit'],
    ['AGEN','1180','attribution','Mexico - Secretaria de Marina – Armada de Mexico, Direccion General Adjunta de Oceanografia, Hidrografia y Meteorologia'],
    ['AGEN','1190','attribution','Micronesia (Federated States of) - Not known'],
    ['AGEN','1197','attribution','Montserrat - Montserrat Port Authority'],
    ['AGEN','1200','attribution','Morocco - Division Hydrographie et Cartographie (DHC) de la Marine Royale'],
    ['AGEN','1210','attribution','Mozambique - Instituto Nacional de Hidrografia e Navegação (INAHINA)'],
    ['AGEN','1220','attribution','Myanmar - Central Naval Hydrographic Depot (CNHD)'],
    ['AGEN','1225','attribution','Montenegro - Ministry of Defence, Navy Headquarters'],
    ['AGEN','1226','attribution','Montenegro - Institute of Hydrometeorology and Seismology'],
    ['AGEN','1230','attribution','Namibia - Ministry of Works, Transports and Communications'],
    ['AGEN','1240','attribution','Nauru - Nauru Phosphate Corporation'],
    ['AGEN','1250','attribution','Nicaragua - Ministero  de  la  Presidencia, Instituto  Nicaragüense  de  Estudios  Territoriales, Dirección de Recursos Hídricos, Departamento de Hidrografía'],
    ['AGEN','1255','attribution','Niue - Lands and Survey Division'],
    ['AGEN','1260','attribution','Palau - Bureau of Domestic Affairs'],
    ['AGEN','1270','attribution','Panama - Autoridad  Maritima  de  Panama'],
    ['AGEN','1280','attribution','Paraguay - Fuerzas Armadas de la Nacion, Armada Paraguaya, Comando de apoyo de combate'],
    ['AGEN','1290','attribution','Qatar - Urban Planning & Development Authority, Hydrographic Section'],
    ['AGEN','1300','attribution','Romania - Directia Hidrografica Maritima'],
    ['AGEN','1310','attribution','Saint Kitts and Nevis - St. Christopher Air and Sea Ports Authority, Maritime Division'],
    ['AGEN','1320','attribution','Saint Lucia - Saint Lucia Air and Sea Ports Authority, Division of Maritime Affairs'],
    ['AGEN','1330','attribution','Saint Vincent and the Grenadines - Ministry of Communications and Works'],
    ['AGEN','1340','attribution','Samoa - Ministry of Transport, Marine and Shipping Division'],
    ['AGEN','1350','attribution','Sao Tome and Principe - Not known'],
    ['AGEN','1360','attribution','Saudi Arabia - General Directorate of Military Survey (GDMS)'],
    ['AGEN','1365','attribution','Saudi Arabia - General Commission for Survey (GCS)'],
    ['AGEN','1370','attribution','Senegal - Service de sécurité maritime du Sénégal, Port autonome de Dakar'],
    ['AGEN','1380','attribution','Seychelles - Hydrographic and Topographic Brigade of the Seychelles'],
    ['AGEN','1390','attribution','Sierra Leone - Sierra Leone Maritime Administration, Sierra Leone Ports Authority'],
    ['AGEN','1400','attribution','Slovenia - Ministry of Transport Maritime  Office'],
    ['AGEN','1410','attribution','Solomon Islands - Solomon Islands Maritime Safety Administration (SIMSA)'],
    ['AGEN','1420','attribution','Somalia - Somali Hydrographic Office'],
    ['AGEN','1430','attribution','Sudan - Survey Department'],
    ['AGEN','1440','attribution','Tanzania - Hydrographic Surveys Section, Surveys and Mapping Division, Ministry of Lands, Housing and Human Settlements Development'],
    ['AGEN','1441','attribution','Tanzania - Tanzania Ports Authority (TPA)'],
    ['AGEN','1450','attribution','Togo - University of Lome, Research Department'],
    ['AGEN','1460','attribution','Tokelau - Not known'],
    ['AGEN','1470','attribution','Tunisia - Service Hydrographique et Océanographique (SHO), Armée de Mer'],
    ['AGEN','1475','attribution','Turks & Caicos Islands - Governor’s Office'],
    ['AGEN','1480','attribution','Tuvalu - Ministry of Labour, Works and Communications'],
    ['AGEN','1485','attribution','Uganda - Commissioner for Transport Regulation'],
    ['AGEN','1490','attribution','Ukraine - State Hydrographic Service of Ukraine'],
    ['AGEN','1500','attribution','Vanuatu - Vanuatu Hydrographic Unit'],
    ['AGEN','1510','attribution','Viet Nam - Viet Nam Maritime Peoples Navy'],
    ['AGEN','1511','attribution','Viet Nam - Viet Nam Maritime Safety-North (VMS-N)'],
    ['AGEN','1512','attribution','Viet Nam - Viet Nam Maritime Safety-South (VMS-S)'],
    ['AGEN','1520','attribution','Yemen - Ministry of Transport, Yemen Ports Authority, Maritime Affairs Authority'],
    ['AGEN','1600','attribution','Antarctic Treaty Consultative Committee'],
    ['AGEN','1610','attribution','International Radio Consultative Committee'],
    ['AGEN','1620','attribution','Comite International Radio-Maritime'],
    ['AGEN','1630','attribution','IHO Data Centre for Digital Bathymetry'],
    ['AGEN','1640','attribution','Digital Geographic Information Working Group'],
    ['AGEN','1650','attribution','European Communities Commission'],
    ['AGEN','1660','attribution','European Harbour Masters Association'],
    ['AGEN','1670','attribution','Food and Agriculture Organization'],
    ['AGEN','1680','attribution','Federation Internationale des Geometres'],
    ['AGEN','1690','attribution','International Atomic Energy Agency'],
    ['AGEN','1700','attribution','International Association of Geodesy'],
    ['AGEN','1710','attribution','International Association of Institutes of Navigation'],
    ['AGEN','1720','attribution','International Association of Lighthouse Authorities'],
    ['AGEN','1730','attribution','International Association of Ports and Harbours'],
    ['AGEN','1740','attribution','International Cartographic Association'],
    ['AGEN','1750','attribution','International Cable Protection Committee'],
    ['AGEN','1760','attribution','International Chamber of Shipping'],
    ['AGEN','1770','attribution','International Commission for the Scientific Exploration of the Mediterranean'],
    ['AGEN','1780','attribution','International Council of Scientific Unions'],
    ['AGEN','1790','attribution','International Electrotechnical Commission'],
    ['AGEN','1800','attribution','International Geographical Union'],
    ['AGEN','1810','attribution','International Hydrographic Organization (IHO)'],
    ['AGEN','1820','attribution','International Maritime Academy'],
    ['AGEN','1830','attribution','International Maritime Organization'],
    ['AGEN','1840','attribution','International Maritime Satellite Organization'],
    ['AGEN','1850','attribution','Intergovernmental Oceanographic Commission'],
    ['AGEN','1860','attribution','International Organization for Standardization'],
    ['AGEN','1870','attribution','International Society for Photogrammetry and Remote Sensing'],
    ['AGEN','1880','attribution','International Telecommunication Union'],
    ['AGEN','1890','attribution','International Union of Geodesy and Geophysics'],
    ['AGEN','1900','attribution','International Union of Surveying and Mapping'],
    ['AGEN','1910','attribution','Oil Companies International Marine Forum'],
    ['AGEN','1920','attribution','Pan American Institute of Geography and History'],
    ['AGEN','1930','attribution','Radio Technical Commission for Maritime Services'],
    ['AGEN','1940','attribution','Scientific Commission on Antarctic Research'],
    ['AGEN','1950','attribution','The Hydrographic Society'],
    ['AGEN','1960','attribution','World Meteorological Organization'],
    ['AGEN','1970','attribution','United Nations, Office for Ocean Affairs and Law of the Sea'],
    ['AGEN','2010','attribution','Co-operating Hydrographic Offices in the Malacca and Singapore Straits (Indonesia, Japan, Malaysia and Singapore)'],
    ['AGEN','2020','attribution','PRIMAR - European ENC Coordinating Centre'],
    ['AGEN','2030','attribution','International Centre for ENC (IC-ENC)'],
    ['AGEN','2040','attribution','East Asia Hydrographic Commission (EAHC)'],
    ['AGEN','3817','attribution','BMT ARGOSS Ltd'],
    ['AGEN','3878','attribution','OpenSeaMap.Org'],
    ['AGEN','3879','attribution','Naval Meteorological &Oceanographic Office, R.O.C. Taiwan'],
    ['AGEN','6682','attribution','ARAMCO'],
    ['AGEN','7196','attribution','CARIS'],
    ['AGEN','7453','attribution','Amt fuer Geoinformationswesen der Bundeswehr'],
    ['AGEN','7710','attribution','TerraNautical Data'],
    ['AGEN','7967','attribution','Force Technology, Danish Maritime Institute'],
    ['AGEN','7968','attribution','Canadian Coast Guard'],
    ['AGEN','7969','attribution','OVF (General Directorate of Water Management)'],
    ['AGEN','7970','attribution','Navionics S.p.A.'],
    ['AGEN','7971','attribution','Navionics test and sample datasets'],
    ['AGEN','7972','attribution','Kingway Technology Co'],
    ['AGEN','7973','attribution','Laser-Scan Ltd'],
    ['AGEN','7974','attribution','Channel of Moscow'],
    ['AGEN','7975','attribution','Nautical Data International, Inc.'],
    ['AGEN','7976','attribution','Offshore Systems Ltd.'],
    ['AGEN','7977','attribution','Port Of London Authority'],
    ['AGEN','7978','attribution','Quality Positioning Services'],
    ['AGEN','7979','attribution','Rijkswaterstaat'],
    ['AGEN','7980','attribution','Austrian Supreme Shipping Authority'],
    ['AGEN','7981','attribution','UKHO - private production'],
    ['AGEN','7982','attribution','ENC Center, National Taiwan Ocean University'],
    ['AGEN','7983','attribution','The Volga-Baltic State Territorial Department for Waterways'],
    ['AGEN','7984','attribution','Wasser- und Schiffahrtsverwaltung des Bundes - Direktion SW'],
    ['AGEN','7985','attribution','Noorderzon Software'],
    ['AGEN','7986','attribution','Tér-Team Ltd., Budapest'],
    ['AGEN','7987','attribution','JS Co Geocentre-Consulting, Moscow'],
    ['AGEN','10794','attribution','Azienda Regionale Navigazione Interna (ARNI)'],
    ['AGEN','11051','attribution','Bundesanstalt für Wasserbau, Karlsruhe'],
    ['AGEN','11308','attribution','IIC Technologies'],
    ['AGEN','11565','attribution','SVP, s.p., OZ Bratislava'],
    ['AGEN','11822','attribution','Euronav Ltd UK'],
    ['AGEN','11973','attribution','Chart Pilot Ltd.'],
    ['AGEN','12004','attribution','QUADRANT-ENC'],
    ['AGEN','12055','attribution','DMER, Zagreb'],
    ['AGEN','12056','attribution','Innovative Navigation GmbH'],
    ['AGEN','12060','attribution','MARIN (Maritime Research Institute Netherlands)'],
    ['AGEN','12061','attribution','Hochschule Bremen (Nautik)'],
    ['AGEN','12063','attribution','PLOVPUT Beograd'],
    ['AGEN','12065','attribution','Port of Rotterdam'],
    ['AGEN','12079','attribution','Ssangyong Information & Communications Corp.'],
    ['AGEN','12083','attribution','C-Map'],
    ['AGEN','12084','attribution','Kamvodput'],
    ['AGEN','12093','attribution','Wartsila Voyage Limited'],
    ['AGEN','12094','attribution','ULTRANS TM srl'],
    ['AGEN','12095','attribution','The Volga State Territorial Department for Waterways'],
    ['AGEN','12096','attribution','via donau - Osterreichische Wasserstrassen-Gesellschaft mbH'],
    ['AGEN','12097','attribution','e-MLX, Korea'],
    ['AGEN','12099','attribution','Land Information New Zealand Hydrographic Services'],
    ['AGEN','14906','attribution','Azovo-Donskoe State Basin Waterway and Shipping Authority'],
    ['AGEN','15163','attribution','AEMDR, Rousse, Bulgaria'],
    ['AGEN','15420','attribution','Command & Control Technologies GmbH'],
    ['AGEN','15934','attribution','Development Centre for Ship Technology and Transport Systems, Germany'],
    ['AGEN','16012','attribution','Public Works and Government Services Canada - Pacific Region'],
    ['AGEN','16035','attribution','FPAEMDR DRIIREST'],
    ['AGEN','16038','attribution','Aero Karta Complex Ltd'],
    ['AGEN','16191','attribution','Finnish Navy'],
    ['AGEN','16193','attribution','HYPACK, Inc.'],
    ['AGEN','16194','attribution','ICAN'],
    ['AGEN','16199','attribution','Tridentnav Systems'],
    ['AGEN','16200','attribution','Ocean Surveys Inc.'],
    ['AGEN','16201','attribution','Pechora Waterways and Navigation Board'],
    ['AGEN','16203','attribution','A.F.D.J. R.A. Galati'],
    ['AGEN','16204','attribution','Leidos'],
    ['AGEN','16205','attribution','Tresco Navigation Systems'],
    ['AGEN','16206','attribution','US Army Corps of Engineers - Channel Condition Data'],
    ['AGEN','16207','attribution','The Volga-Don Waterways And Navigation Board'],
    ['AGEN','16208','attribution','Wasserschutzpolizei-Schule'],
    ['AGEN','19018','attribution','Arctic and Antarctic Research Institute (AARI) of the Russian Federal Service for Hydrometeorology and Environmental Monitoring (Roshydromet)'],
    ['AGEN','19275','attribution','BaikalChart, Russia'],
    ['AGEN','19532','attribution','Rheinschifffahrtsdirektion (RSD) Basel'],
    ['AGEN','20046','attribution','Environmental Systems Research Institute (ESRI)'],
    ['AGEN','20122','attribution','GEOMOD'],
    ['AGEN','20127','attribution','SHOM test data'],
    ['AGEN','20306','attribution','Canadian Ice Service'],
    ['AGEN','20311','attribution','MeteoConsult'],
    ['AGEN','20315','attribution','MD Atlantic Technologies'],
    ['AGEN','20316','attribution','ADVETO Advanced Technology AB'],
    ['AGEN','20317','attribution','OOO Tekhpromcomplect'],
    ['AGEN','20318','attribution','Center for Coastal & Ocean Mapping/Joint Hydrographic Center, University of New Hampshire'],
    ['AGEN','20319','attribution','Voies Navigables de France (VNF)'],
    ['AGEN','20320','attribution','Fachstelle fuer Geoinformationen Sued beim WSA Regensburg'],
    ['AGEN','20323','attribution','C-Map Russia'],
    ['AGEN','23130','attribution','American Commercial Lines (ACL), Inc.'],
    ['AGEN','23644','attribution','CRUP d.o.o., Croatia'],
    ['AGEN','24158','attribution','River Transport Authority (RTA), Egypt'],
    ['AGEN','24168','attribution','SeaZone Solutions'],
    ['AGEN','24181','attribution','Bremer Schiffsmeldedienst'],
    ['AGEN','24201','attribution','Vstep B.V.'],
    ['AGEN','24203','attribution','Wasser- und Schifffahrtsverwaltung des Bundes - WSA Bremerhaven'],
    ['AGEN','24418','attribution','European Inland ECDIS Expert Group'],
    ['AGEN','24422','attribution','Hydrographic Office of the Sarawak Marine Department'],
    ['AGEN','24423','attribution','NAVTRON SRL'],
    ['AGEN','24425','attribution','Safe Trip SA, Argentina'],
    ['AGEN','24427','attribution','State Federal Unitary Enterprise NW Regional Production Centre of Geoinformation and Mine Surveying Centre, "Sevzapgeoinform" (Russia)'],
    ['AGEN','24430','attribution','United Kingdom Royal Navy'],
    ['AGEN','24432','attribution','Wasser- und Schifffahrtsverwaltung des Bundes - WSA Cuxhaven'],
    ['AGEN','24455','attribution','TEC Asociados'],
    ['AGEN','27242','attribution','AMEC'],
    ['AGEN','27499','attribution','C-Tech SRL, Romania'],
    ['AGEN','27756','attribution','Guoy Consultancy Sdn Bhd'],
    ['AGEN','28233','attribution','Navtor AS'],
    ['AGEN','28243','attribution','Panama Canal Authority'],
    ['AGEN','28249','attribution','Mercuries Data Systems Ltd.'],
    ['AGEN','28266','attribution','Hochschule Wismar Bereich Seefahrt (MSCW)'],
    ['AGEN','28542','attribution','U.S. Geological Survey (USGS) - Coastal and Marine Geology'],
    ['AGEN','31354','attribution','Azerbaijan Navy, Service of Navigation and Oceanography'],
    ['AGEN','31868','attribution','SevenCs AG & Co KG'],
    ['AGEN','32264','attribution','Latincomp'],
    ['AGEN','32298','attribution','ChartCo Limited'],
    ['AGEN','32320','attribution','Marine Technology sp. o.o.'],
    ['AGEN','32323','attribution','PETROBRAS'],
    ['AGEN','32328','attribution','Australian Hydrographic Service - Provisional and sample datasets'],
    ['AGEN','32641','attribution','Petroslav Hydroservice, Russia'],
    ['AGEN','32651','attribution','The Federal Service of Geodesy and Cartography of Russia'],
    ['AGEN','32652','attribution','Centre Sevzapgeoinform (SZGI)'],
    ['AGEN','32653','attribution','Terra Corp'],
    ['AGEN','32655','attribution','De Vlaamse Waterweg'],
    ['AGEN','32656','attribution','Waterwegen en Zeekanaal'],
    ['AGEN','35466','attribution','L-3 Communications Oceania Limited'],
    ['AGEN','35980','attribution','Seebyte Ltd.'],
    ['AGEN','36363','attribution','Abris, Llc'],
    ['AGEN','36376','attribution','ATLAS ELEKTRONIK GmbH'],
    ['AGEN','39578','attribution','CherSoft Ltd'],
    ['AGEN','39624','attribution','National Navigation Authority of the Czech Republic'],
    ['AGEN','40092','attribution','Chartworld Gmbh'],
    ['AGEN','40865','attribution','Hamburg Port Authority'],
    ['AGEN','40876','attribution','Solutions from Silicon, Sydney'],
    ['AGEN','40877','attribution','Tresco Engineering bvba'],
    ['AGEN','40883','attribution','Nobeltec, Inc'],
    ['AGEN','42000','attribution','Marine Geodesy LLC'],
    ['AGEN','42001','attribution','PD Ports'],
    ['AGEN','42002','attribution','The Enisei State Territorial Department for Waterways'],
    ['AGEN','42003','attribution','David Evans and Associates, Inc. | Marine Services Division'],
    ['AGEN','42004','attribution','The Severnaya Dvina State Territorial Department for Waterways'],
    ['AGEN','42005','attribution','TCarta Marine'],
    ['AGEN','42006','attribution','Terra OOO'],
    ['AGEN','42007','attribution','Australia - Department of Transport Western Australia'],
    ['AGEN','42008','attribution','Maju Geohydro Sdn Bhd'],
    ['AGEN','42009','attribution','Service Public de Wallonie'],
    ['AGEN','42010','attribution','Nautical Publications Gmbh'],
    ['AGEN','42011','attribution','OceanWise'],
    ['AGEN','42012','attribution','Port of Antwerp (Havenbedrijf Antwerpen NV van publiek recht)'],
    ['AGEN','42013','attribution','Grand Port Maritime du Havre'],
    ['AGEN','42014','attribution','India - India National Hydrographic Office - Miscellaneous'],
    ['AGEN','42015','attribution','Germany - Elbe Pilots Germany'],
    ['AGEN','42016','attribution','Grand Port Maritime de Rouen'],
    ['AGEN','42017','attribution','Imray Laurie Norie & Wilson Ltd'],
    ['AGEN','42018','attribution','Federal Agency for Maritime and River Transport (FAMRT)'],
    ['AGEN','42019','attribution','Port de Barcelona, España'],
    ['AGEN','42020','attribution','CoVadem'],
    ['AGEN','42021','attribution','Sorlingas Data Services Ltd'],
    ['AGEN','43456','attribution','VietNav Ltd, VIETNAM'],
    ['AGEN','44444','attribution','Associated British Ports Southampton'],
    ['AGEN','44445','attribution','ConcENC OOO'],
    ['AGEN','44446','attribution','CTC JSC'],

    // BCNSHP - Beacon shape
    ['BCNSHP','1','BCNSHP','stake pole perch post'], // stake, pole, perch, post
    ['BCNSHP','2','BCNSHP','withy'], // withy
    ['BCNSHP','3','BCNSHP','tower'], // beacon tower
    ['BCNSHP','4','BCNSHP','lattice'], // lattice beacon
    ['BCNSHP','5','BCNSHP','pile'], // pile beacon
    ['BCNSHP','6','BCNSHP','cairn'], // cairn
    ['BCNSHP','7','BCNSHP','buoyant'], // buoyant beacon

    // BOYSHP - Buoy shape
    ['BOYSHP','1','BOYSHP','conical'], // conical (nun, ogival)
    ['BOYSHP','2','BOYSHP','can'], // can (cylindrical)
    ['BOYSHP','3','BOYSHP','spherical'], // spherical
    ['BOYSHP','4','BOYSHP','pillar'], // pillar
    ['BOYSHP','5','BOYSHP','spar'], // spar (spindle)
    ['BOYSHP','6','BOYSHP','barrel'], // barrel (tun)
    ['BOYSHP','7','BOYSHP','super-buoy'], // super-buoy
    ['BOYSHP','8','BOYSHP','ice_buoy'], // ice buoy

    // BUISHP - Building shape
    // ['BUISHP','1','raw:BUISHP','no_specific_shape'], // no specific shape
    // ['BUISHP','2','raw:BUISHP','tower'], // tower
    // ['BUISHP','3','raw:BUISHP','spire'], // spire
    // ['BUISHP','4','raw:BUISHP','cupola_(dome)'], // cupola (dome)
    ['BUISHP','5','BUISHP','high-rise_building'], // high-rise building - Not great at all.
    ['BUISHP','6','BUISHP','pyramidal'], // pyramid
    ['BUISHP','7','BUISHP','cylindrical'], // cylindrical
    ['BUISHP','8','BUISHP','spherical'], // spherical
    ['BUISHP','9','BUISHP','cubic'], // cubic

    // CATACH - Category of anchorage - Moved to StringList

    // CATAIR - Category of airport/airfield - Moved to StringList

    // CATBRG - Category of bridge - Moved to StringList

    // CATBUA - Category of built-up area
    ['CATBUA','1','landuse:category','urban'], // urban area
    ['CATBUA','2','place','settlement'], // settlement
    ['CATBUA','3','place','village'], // village
    ['CATBUA','4','place','town'], // town
    ['CATBUA','5','place','city'], // city
    ['CATBUA','6','place','holiday_village'], // holiday village

    // CATCAM - Category of cardinal mark
    ['CATCAM','1','CATCAM','north'], // north cardinal mark
    ['CATCAM','2','CATCAM','east'], // east cardinal mark
    ['CATCAM','3','CATCAM','south'], // south cardinal mark
    ['CATCAM','4','CATCAM','west'], // west cardinal mark

    // CATCAN - Category of canal
    ['CATCAN','1','use','canal_transportation'], // transportation
    ['CATCAN','2','water:use','drainage'], // drainage
    ['CATCAN','3','water:use','agricultiral_irrigation'], // irrigation

    // CATCBL - Category of cable
    ['CATCBL','1','CATCBL','power'], // power line
    ['CATCBL','2','CATCBL','telephone_telegraph'], // telephone/telegraph
    ['CATCBL','3','CATCBL','transmission'], // transmission line
    ['CATCBL','4','CATCBL','telephone'], // telephone
    ['CATCBL','5','CATCBL','telegraph'], // telegraph
    ['CATCBL','6','CATCBL','mooring'], // mooring cable/chain

    // CATCHP - Category of checkpoint
    ['CATCHP','1','seamark:checkpoint:category','customs'], // custom

    // CATCOA - Category of coastline
    ['CATCOA','1','shoreline:type','steep'], // steep coast
    ['CATCOA','2','shoreline:type','flat'], // flat coast
    ['CATCOA','3','shoreline:type','sandy'], // sandy shore
    ['CATCOA','4','shoreline:type','stony'], // stony shore
    ['CATCOA','5','shoreline:type','shingly'], // shingly shore
    ['CATCOA','6','shoreline:type','glacier'], // glacier (seaward end)
    ['CATCOA','7','shoreline:type','mangrove'], // mangrove
    ['CATCOA','8','shoreline:type','marshy'], // marshy shore
    ['CATCOA','9','shoreline:type','coral_reef'], // coral reef
    ['CATCOA','10','shoreline:type','ice'], // ice coast
    ['CATCOA','11','shoreline:type','shelly'], // shelly shore

    // CATCON - Category of conveyor
    ['CATCON','1','conveyer:type','aerial_cableway'], // aerial cableway (telepheric)
    ['CATCON','2','conveyer:type','belt'], // belt conveyor

    // CATCOV - Category of coverage
    ['CATCOV','1','coverage','yes'], // coverage available
    ['CATCOV','2','coverage','no'], // no coverage available

    // CATCRN - Category of crane
    ['CATCRN','1','crane:type','other'], // crane without specific construction
    ['CATCRN','2','crane:type','container'], // container crane/gantry
    ['CATCRN','3','crane:type','sheer-legs'], // sheerlegs
    ['CATCRN','4','crane:type','travelling'], // travelling crane
    ['CATCRN','A-5','crane:type','a-frame'], // frame

    // CATCTR - Category of control point
    ['CATCTR','1','seamark:control_point:category','triangulation'], // triangulation point
    ['CATCTR','2','seamark:control_point:category','observation'], // observation spot
    ['CATCTR','3','seamark:control_point:category','fixed'], // fixed point
    ['CATCTR','4','seamark:control_point:category','benchmark'], // bench-mark
    ['CATCTR','5','seamark:control_point:category','boundary'], // boundary mark
    ['CATCTR','6','seamark:control_point:category','horizontal_main'], // horizontal control, main station
    ['CATCTR','7','seamark:control_point:category','horizontal_secondary'], // horizontal control, secondary station

    // CATDAM - Category of dam
    ['CATDAM','1','dam:type','weir'], // weir
    // ['CATDAM','2','waterway','dam'], // dam - default?
    ['CATDAM','3','dam:type','flood_control'], // flood barrage

    // CATDIS - Category of distance mark
    ['CATDIS','1','seamark:distance_mark:category','not_installed'], // distance mark not physically installed
    ['CATDIS','2','seamark:distance_mark:category','pole'], // visible mark, pole
    ['CATDIS','3','seamark:distance_mark:category','board'], // visible mark, board
    ['CATDIS','4','seamark:distance_mark:category','unknown_shape'], // visible mark, unknown shape

    // CATDOC - Category of dock
    ['CATDOC','1','tidal','yes'], // tidal
    ['CATDOC','2','tidal','no'], // non-tidal (wet dock)

    // CATDPG - Category of dumping ground - Moved to StringList

    // CATDYK - Category of dyke
    // ['CATDYK','1','raw:CATDYK','summer_dyke'], // summer dyke
    // ['CATDYK','2','raw:CATDYK','winter_dyke'], // winter dyke

    // CATFIF - Category of fishing facility
    ['CATFIF','1','seamark:fishing_facility:category','stake'], // fishing stake
    ['CATFIF','2','seamark:fishing_facility:category','trap'], // fish trap
    ['CATFIF','3','seamark:fishing_facility:category','weir'], // fish weir
    ['CATFIF','4','seamark:fishing_facility:category','tunny'], // tunny net

    // CATFNC - Category of fence/wall
    ['CATFNC','1','barrier','fence'], // fence
    // ['CATFNC','2','seamark:fenceline:category','muir'], // muir
    ['CATFNC','3','barrier','hedge'], // hedge
    ['CATFNC','4','barrier','wall'], // wall

    // CATFOG - Category of fog signal
    ['CATFOG','1','seamark:fog_signal:category','explosive'], // explosive
    ['CATFOG','2','seamark:fog_signal:category','diaphone'], // diaphone
    ['CATFOG','3','seamark:fog_signal:category','siren'], // siren
    ['CATFOG','4','seamark:fog_signal:category','nautophone'], // nautophone
    ['CATFOG','5','seamark:fog_signal:category','reed'], // reed
    ['CATFOG','6','seamark:fog_signal:category','tyfon'], // tyfon
    ['CATFOG','7','seamark:fog_signal:category','bell'], // bell
    ['CATFOG','8','seamark:fog_signal:category','whistle'], // whistle
    ['CATFOG','9','seamark:fog_signal:category','gong'], // gong
    ['CATFOG','10','seamark:fog_signal:category','horn'], // horn

    // CATFOR - Category of fortified structure
    ['CATFOR','1','seamark:fortified_structure:category','castle'], // castle
    ['CATFOR','2','seamark:fortified_structure:category','fort'], // fort
    ['CATFOR','3','seamark:fortified_structure:category','battery'], // battery
    ['CATFOR','4','seamark:fortified_structure:category','blockhouse'], // blockhouse
    ['CATFOR','5','seamark:fortified_structure:category','martello_tower'], // Martello tower
    ['CATFOR','6','seamark:fortified_structure:category','redoubt'], // redoubt

    // CATFRY - Category of ferry
    ['CATFRY','1','seamark:ferry_route:category','free'], // free-moving= ferry
    ['CATFRY','2','seamark:ferry_route:category','cable'], // cable ferry
    ['CATFRY','3','seamark:ferry_route:category','ice'], // ice ferry

    // CATGAT - Category of gate
    ['CATGAT','1','seamark:gate:category','general'], // gate in general
    ['CATGAT','2','seamark:gate:category','flood_barrage'], // flood barrage gate
    ['CATGAT','3','seamark:gate:category','caisson'], // caisson
    ['CATGAT','4','seamark:gate:category','lock'], // lock gate
    ['CATGAT','5','seamark:gate:category','dyke'], // dyke gate
    ['CATGAT','6','seamark:gate:category','sluice'], // sluice

    // CATHAF - Category of harbour facility - Moved to StringList

    // CATHLK - Category of hulk - Moved to StringList

    // CATICE - Category of ice
    ['CATICE','1','ice:type','fast'], // fast ice
    ['CATICE','2','ice:type','sea'], // sea ice
    ['CATICE','3','ice:type','growler_area'], // growler area
    ['CATICE','4','ice:type','pancake'], // pancake ice
    ['CATICE','5','ice:type','glacier'], // glacier
    ['CATICE','6','ice:type','ice_peak'], // ice peak
    ['CATICE','7','ice:type','pack'], // pack ice
    ['CATICE','8','ice:type','polar'], // polar ice

    // CATINB - Category of installation buoy
    ['CATINB','1','seamark:buoy_installation:category','calm'], // catenary anchor leg mooring (CALM)
    ['CATINB','2','seamark:buoy_installation:category','sbm'], // single buoy mooring (SBM or SPM)

    // CATLAM - Category of lateral mark
    ['CATLAM','1','CATLAM','port'], // port-hand lateral mark
    ['CATLAM','2','CATLAM','starboard'], // starboard-hand lateral mark
    ['CATLAM','3','CATLAM','preferred_channel_starboard'], // preferred channel to starboard lateral mark
    ['CATLAM','4','CATLAM','preferred_channel_port'], // preferred channel to port lateral mark

    // CATLIT - Category of light - Moved to StringList

    // CATLMK - Category of landmark - Moved to StringList

    // CATLND - Category of land region - Moved to StringList

    // CATMFA - Category of marine farm/culture
    ['CATMFA','1','seamark:marine_farm:category','crustaceans'], // crustaceans
    ['CATMFA','2','seamark:marine_farm:category','oysters/mussels'], // oysters/mussels
    ['CATMFA','3','seamark:marine_farm:category','fish'], // fish
    ['CATMFA','4','seamark:marine_farm:category','seaweed'], // seaweed
    ['CATMFA','5','seamark:marine_farm:category','pearl_culture'], // pearl culture

    // CATMNT - Category of monument
    // ['CATMNT','1','raw:CATMNT','column'], // column
    // ['CATMNT','2','raw:CATMNT','memorial_plaque'], // memorial plaque
    // ['CATMNT','3','raw:CATMNT','obelisk'], // obelisk
    // ['CATMNT','4','raw:CATMNT','pillar'], // pillar
    // ['CATMNT','5','raw:CATMNT','statue'], // statue

    // CATMOR - Category of mooring/warping facility
    ['CATMOR','1','seamark:mooring:category','dolphin'], // dolphin
    ['CATMOR','2','seamark:mooring:category','deviation_dolphin'], // deviation dolphin
    ['CATMOR','3','seamark:mooring:category','bollard'], // bollard
    ['CATMOR','4','seamark:mooring:category','wall'], // tie-up wall
    ['CATMOR','5','seamark:mooring:category','pile'], // post or pile
    ['CATMOR','6','seamark:mooring:category','chain'], // chain/wire/cable
    ['CATMOR','7','seamark:mooring:category','buoy'], // mooring buoy

    // CATMPA - Category of military practice area - Moved to StringList

    // CATMST - Category of mast
    // ['CATMST','1','raw:CATMST','radio_mast_/_television_mast'], // radio mast / television mast
    // ['CATMST','2','raw:CATMST','mooring_mast'], // mooring mast
    // ['CATMST','3','raw:CATMST','radar_mast'], // radar mast
    // ['CATMST','4','raw:CATMST','wind_sock'], // wind sock

    // CATNAV - Category of navigation line
    ['CATNAV','1','seamark:navigation_line:category','clearing'], // clearing line
    ['CATNAV','2','seamark:navigation_line:category','transit'], // transit line
    ['CATNAV','3','seamark:navigation_line:category','leading'], // leading line bearing a recommended track

    // CATOBS - Category of obstruction
    ['CATOBS','1','seamark:obstruction:category','stump'], // snag/stump
    ['CATOBS','2','seamark:obstruction:category','wellhead'], // wellhead
    ['CATOBS','3','seamark:obstruction:category','diffuser'], // diffuser
    ['CATOBS','4','seamark:obstruction:category','crib'], // crib
    ['CATOBS','5','seamark:obstruction:category','fish_haven'], // fish haven
    ['CATOBS','6','seamark:obstruction:category','foul_area'], // foul area
    ['CATOBS','7','seamark:obstruction:category','foul_ground'], // foul ground
    ['CATOBS','8','seamark:obstruction:category','ice_boom'], // ice boom
    ['CATOBS','9','seamark:obstruction:category','ground_tackle'], // ground tackle
    ['CATOBS','10','seamark:obstruction:category','boom'], // boom

    // CATOFP - Category of offshore platform - Moved to StringList

    // CATOLB - Category of oil barrier
    ['CATOLB','1','seamark:oil_barrier:category','retention'], // oil retention (high pressure pipe)
    ['CATOLB','2','seamark:oil_barrier:category','floating'], // floating oil barrier

    // CATPIL - Category of pilot boarding place
    ['CATPIL','1','seamark:pilot_boarding:category','cruising_vessel'], // boarding by pilot-cruising vessel
    ['CATPIL','2','seamark:pilot_boarding:category','helicopter'], // boarding by helicopter
    ['CATPIL','3','seamark:pilot_boarding:category','from_shore'], // pilot comes out from shore

    // CATPIP - Category of pipeline/pipe - Moved to StringList

    // CATPLE - Category of pile
    ['CATPLE','1','seamark:pile:category','stake'], // stake
    ['CATPLE','2','seamark:pile:category','snag'], // snag
    ['CATPLE','3','seamark:pile:category','post'], // post
    ['CATPLE','4','seamark:pile:category','tripodal'], // tripodal

    // CATPRA - Category of production area
    ['CATPRA','1','seamark:production_area:category','quarry'], // quarry
    ['CATPRA','2','seamark:production_area:category','mine'], // mine
    ['CATPRA','3','seamark:production_area:category','stockpile'], // stockpile
    ['CATPRA','4','seamark:production_area:category','power_station'], // power station area
    ['CATPRA','5','seamark:production_area:category','refinery'], // refinery area
    ['CATPRA','6','seamark:production_area:category','timber_yard'], // timber yard
    ['CATPRA','7','seamark:production_area:category','factory'], // factory area
    ['CATPRA','8','seamark:production_area:category','tank_farm'], // tank farm
    ['CATPRA','9','seamark:production_area:category','wind_farm'], // wind farm
    ['CATPRA','10','seamark:production_area:category','slag_heap'], // slag heap/spoil heap

    // CATPRI - Category of production installation
    // ['CATPRI','1','raw:CATPRI','quarry'], // quarry
    // ['CATPRI','2','raw:CATPRI','mine'], // mine
    // ['CATPRI','3','raw:CATPRI','wellhead'], // wellhead
    // ['CATPRI','4','raw:CATPRI','production_well'], // production well

    // CATPYL - Category of pylon
    ['CATPYL','1','seamark:pylon:category','power/pole'], // power transmission pylon/pole
    ['CATPYL','2','seamark:pylon:category','telecom'], // telephone/telegraph pylon/pole
    ['CATPYL','3','seamark:pylon:category','aerial'], // aerial cableway/sky pylon
    ['CATPYL','4','seamark:pylon:category','bridge'], // bridge pylon/tower
    ['CATPYL','5','seamark:pylon:category','bridge_pier'], // bridge pier

    // CATQUA - Category of quality of data
    // ['CATQUA','1','raw:CATQUA','data_quality_a'], // data quality A
    // ['CATQUA','2','raw:CATQUA','data_quality_b'], // data quality B
    // ['CATQUA','3','raw:CATQUA','data_quality_c'], // data quality C
    // ['CATQUA','4','raw:CATQUA','data_quality_d'], // data quality D
    // ['CATQUA','5','raw:CATQUA','data_quality_e'], // data quality E
    // ['CATQUA','6','raw:CATQUA','quality_not_evaluated'], // quality not evaluated

    // CATRAS - Category of radar station
    ['CATRAS','1','seamark:radar_station:category','surveillance'], // radar surveillance station
    ['CATRAS','2','seamark:radar_station:category','coast'], // coast radar station

    // CATREA - Category of restricted area - Moved to StringList

    // CATREB - Category of religious building
    // ['CATREB','1','raw:CATREB','church'], // church
    // ['CATREB','2','raw:CATREB','chapel'], // chapel
    // ['CATREB','3','raw:CATREB','cross;_calvary'], // cross; calvary
    // ['CATREB','4','raw:CATREB','temple'], // temple
    // ['CATREB','5','raw:CATREB','pagoda'], // pagoda
    // ['CATREB','6','raw:CATREB','shinto-shrine'], // shinto-shrine
    // ['CATREB','7','raw:CATREB','buddhist_temple'], // buddhist temple
    // ['CATREB','8','raw:CATREB','mosque'], // mosque
    // ['CATREB','9','raw:CATREB','marabout'], // marabout

    // CATROD - Category of road
    ['CATROD','1','highway','motorway'], // motorway
    ['CATROD','2','highway','trunk'], // major road
    ['CATROD','3','highway','primary'], // minor road
    ['CATROD','4','highway','track'], // track/path
    ['CATROD','5','highway','secondary'], // major street
    ['CATROD','6','highway','tertiary'], // minor street
    ['CATROD','7','highway','crossing'], // crossing

    // CATROS - Category of radio station - Moved to StringList

    // CATRSC - Category of rescue station - Moved to StringList

    // CATRTB - Category of radar transponder beacon
    ['CATRTB','1','seamark:radar_transponder:category','ramark'], // ramark, radar beacon transmitting continuously
    ['CATRTB','2','seamark:radar_transponder:category','racon'], // racon, radar transponder beacon
    ['CATRTB','3','seamark:radar_transponder:category','leading'], // leading racon/radar transponder beacon

    // CATRUN - Category of runway
    ['CATRUN','1','aeroway','runway'], // aeroplane runway
    ['CATRUN','2','aeroway','helipad'], // helicopter landing pad

    // CATSCF - Category of small craft facility - Moved to StringList

    // CATSEA - Category of sea area
    ['CATSEA','1','seamark:sea_area:category','sea_area_in_general'], // sea area in general
    ['CATSEA','2','seamark:sea_area:category','gat'], // gat
    ['CATSEA','3','seamark:sea_area:category','bank'], // bank
    ['CATSEA','4','seamark:sea_area:category','deep'], // deep
    ['CATSEA','5','seamark:sea_area:category','bay'], // bay
    ['CATSEA','6','seamark:sea_area:category','trench'], // trench
    ['CATSEA','7','seamark:sea_area:category','basin'], // basin
    ['CATSEA','8','seamark:sea_area:category','mud_flats'], // mud flats
    ['CATSEA','9','seamark:sea_area:category','reef'], // reef
    ['CATSEA','10','seamark:sea_area:category','ledge'], // ledge
    ['CATSEA','11','seamark:sea_area:category','canyon'], // canyon
    ['CATSEA','12','seamark:sea_area:category','narrows'], // narrows
    ['CATSEA','13','seamark:sea_area:category','shoal'], // shoal
    ['CATSEA','14','seamark:sea_area:category','knoll'], // knoll
    ['CATSEA','15','seamark:sea_area:category','ridge'], // ridge
    ['CATSEA','16','seamark:sea_area:category','seamount'], // seamount
    ['CATSEA','17','seamark:sea_area:category','pinnacle'], // pinnacle
    ['CATSEA','18','seamark:sea_area:category','abyssal_plain'], // abyssal plain
    ['CATSEA','19','seamark:sea_area:category','plateau'], // plateau
    ['CATSEA','20','seamark:sea_area:category','spur'], // spur
    ['CATSEA','21','seamark:sea_area:category','shelf'], // shelf
    ['CATSEA','22','seamark:sea_area:category','trough'], // trough
    ['CATSEA','23','seamark:sea_area:category','saddle'], // saddle
    ['CATSEA','24','seamark:sea_area:category','abyssal_hills'], // abyssal hills
    ['CATSEA','25','seamark:sea_area:category','apron'], // apron
    ['CATSEA','26','seamark:sea_area:category','archipelagic_apron'], // archipelagic apron
    ['CATSEA','27','seamark:sea_area:category','borderland'], // borderland
    ['CATSEA','28','seamark:sea_area:category','continental_margin'], // continental margin
    ['CATSEA','29','seamark:sea_area:category','continental_rise'], // continental rise
    ['CATSEA','30','seamark:sea_area:category','escarpment'], // escarpment
    ['CATSEA','31','seamark:sea_area:category','fan'], // fan
    ['CATSEA','32','seamark:sea_area:category','fracture_zone'], // fracture zone
    ['CATSEA','33','seamark:sea_area:category','gap'], // gap
    ['CATSEA','34','seamark:sea_area:category','guyot'], // guyot
    ['CATSEA','35','seamark:sea_area:category','hill'], // hill
    ['CATSEA','36','seamark:sea_area:category','hole'], // hole
    ['CATSEA','37','seamark:sea_area:category','levee'], // levee
    ['CATSEA','38','seamark:sea_area:category','median_valley'], // median valley
    ['CATSEA','39','seamark:sea_area:category','moat'], // moat
    ['CATSEA','40','seamark:sea_area:category','mountains'], // mountains
    ['CATSEA','41','seamark:sea_area:category','peak'], // peak
    ['CATSEA','42','seamark:sea_area:category','province'], // province
    ['CATSEA','43','seamark:sea_area:category','rise'], // rise
    ['CATSEA','44','seamark:sea_area:category','sea_channel'], // sea channel
    ['CATSEA','45','seamark:sea_area:category','seamount_chain'], // seamount chain
    ['CATSEA','46','seamark:sea_area:category','shelf-edge'], // shelf-edge
    ['CATSEA','47','seamark:sea_area:category','sill'], // sill
    ['CATSEA','48','seamark:sea_area:category','slope'], // slope
    ['CATSEA','49','seamark:sea_area:category','terrace'], // terrace
    ['CATSEA','50','seamark:sea_area:category','valley'], // valley
    ['CATSEA','51','seamark:sea_area:category','canal'], // canal
    ['CATSEA','52','seamark:sea_area:category','lake'], // lake
    ['CATSEA','53','seamark:sea_area:category','river'], // river
    ['CATSEA','54','seamark:sea_area:category','reach'], // reach

    // CATSIL - Category of silo/tank
    ['CATSIL','1','man_made','silo'], // silo in general
    ['CATSIL','2','man_made','storage_tank'], // tank in general
    ['CATSIL','3','man_made','grain_elevator'], // grain elevator
    ['CATSIL','4','man_made','water_tower'], // water tower

    // CATSIT - Category of signal station, traffic - Moved to StringList

    // CATSIW - Category of signal station, warning - Moved to StringList

    // CATSLC - Category of shoreline construction
    ['CATSLC','1','seamark:shoreline_construction:category','breakwater'], // breakwater
    ['CATSLC','2','seamark:shoreline_construction:category','groyne'], // groyne (groin)
    ['CATSLC','3','seamark:shoreline_construction:category','mole'], // mole
    ['CATSLC','4','seamark:shoreline_construction:category','pier'], // pier (jetty)
    ['CATSLC','5','seamark:shoreline_construction:category','promenade_pier'], // promenade pier
    ['CATSLC','6','seamark:shoreline_construction:category','wharf'], // wharf (quay)
    ['CATSLC','7','seamark:shoreline_construction:category','training_wall'], // training wall
    ['CATSLC','8','seamark:shoreline_construction:category','rip_rap'], // rip rap
    ['CATSLC','9','seamark:shoreline_construction:category','revetment'], // revetment
    ['CATSLC','10','seamark:shoreline_construction:category','sea_wall'], // sea wall
    ['CATSLC','11','seamark:shoreline_construction:category','landing_steps'], // landing steps
    ['CATSLC','12','seamark:shoreline_construction:category','ramp'], // ramp
    ['CATSLC','13','seamark:shoreline_construction:category','slipway'], // slipway
    ['CATSLC','14','seamark:shoreline_construction:category','fender'], // fender
    ['CATSLC','15','seamark:shoreline_construction:category','solid_face_wharf'], // solid face wharf
    ['CATSLC','16','seamark:shoreline_construction:category','open_face_wharf'], // open face wharf
    ['CATSLC','17','seamark:shoreline_construction:category','log_ramp'], // log ramp

    // CATSLO - Category of slope
    ['CATSLO','1','cutting','yes'], // cutting
    ['CATSLO','2','embankment','yes'], // embankment
    ['CATSLO','3','natural','dune'], // dune
    ['CATSLO','4','natural','hill'], // hill
    ['CATSLO','5','landform','pingo'], // pingo
    ['CATSLO','6','natural','cliff'], // cliff
    ['CATSLO','7','natural','scree'], // scree

    // CATSPM - Category of special purpose mark - Moved to StringList

    // CATTOW - Category of tower
    // ['CATTOW','1','raw:CATTOW','light_tower'], // light tower
    // ['CATTOW','2','raw:CATTOW','water_tower'], // water tower
    // ['CATTOW','3','raw:CATTOW','radio/television_tower'], // radio/television tower
    // ['CATTOW','4','raw:CATTOW','cooling_tower'], // cooling tower
    // ['CATTOW','5','raw:CATTOW','radar_tower'], // radar tower
    // ['CATTOW','6','raw:CATTOW','lookout_tower'], // lookout tower
    // ['CATTOW','7','raw:CATTOW','observation_tower'], // observation tower

    // CATTRE - Category of tree
    // ['CATTRE','1','raw:CATTRE','evergreen'], // evergreen
    // ['CATTRE','2','raw:CATTRE','conifer'], // conifer
    // ['CATTRE','3','raw:CATTRE','palm'], // palm
    // ['CATTRE','4','raw:CATTRE','nipa_palm'], // nipa palm
    // ['CATTRE','5','raw:CATTRE','casuarina'], // casuarina
    // ['CATTRE','6','raw:CATTRE','filao'], // filao
    // ['CATTRE','7','raw:CATTRE','eucalypt'], // eucalypt
    // ['CATTRE','8','raw:CATTRE','deciduous'], // deciduous
    // ['CATTRE','9','raw:CATTRE','mangrove'], // mangrove

    // CATTRK - Category of recommended track
    ['CATTRK','1','seamark:recommended_track:category','fixed_marks'], // based on a system of fixed marks
    ['CATTRK','2','seamark:recommended_track:category','no_fixed_marks'], // not based on a system of fixed marks

    // CATTSS - Category of Traffic Separation Scheme
    ['CATTSS','1','seamark:separation_zone:category','imo-adopted'], // IMO-adopted
    ['CATTSS','2','seamark:separation_zone:category','not_imo-adopted'], // not IMO-adopted

    // CATVEG - Category of vegetation - Moved to StringList

    // CATWAT - Category of water turbulence
    ['CATWAT','1','seamark:water_turbulence:category','breakers'], // breakers
    ['CATWAT','2','seamark:water_turbulence:category','eddies'], // eddies
    ['CATWAT','3','seamark:water_turbulence:category','overfalls'], // overfalls
    ['CATWAT','4','seamark:water_turbulence:category','tide_rips'], // tide rips
    ['CATWAT','5','seamark:water_turbulence:category','bombora'], // bombora

    // CATWED - Category of weed/kelp
    ['CATWED','1','seamark:vegeation:category','kelp'], // kelp
    ['CATWED','2','seamark:vegetation:category','sea_weed'], // sea weed
    ['CATWED','3','seamark:vegetation:category','sea_grass'], // sea grass
    ['CATWED','4','seamark:vegetation:category','saragasso'], // saragasso

    // CATWRK - Category of wreck
    ['CATWRK','1','seamark:wreck:category','non-dangerous'], // non-dangerous wreck
    ['CATWRK','2','seamark:wreck:category','dangerous'], // dangerous wreck
    ['CATWRK','3','seamark:wreck:category','distributed_remains'], // distributed remains of wreck
    ['CATWRK','4','seamark:wreck:category','wreck_showing'], // wreck showing mast/masts
    ['CATWRK','5','seamark:wreck:category','hull_showing'], // wreck showing any portion of hull or superstructure

    // CATZOC - Category of zone of confidence in data
    ['CATZOC','1','zone_of_confidence','a1'], // zone of confidence A1
    ['CATZOC','2','zone_of_confidence','a2'], // zone of confidence A2
    ['CATZOC','3','zone_of_confidence','b'], // zone of confidence B
    ['CATZOC','4','zone_of_confidence','c'], // zone of confidence C
    ['CATZOC','5','zone_of_confidence','d'], // zone of confidence D
    ['CATZOC','6','zone_of_confidence','data_not_assessed'], // zone of confidence U (data not assessed)

    // CAT_TS - Category of Tidal stream
    ['CAT_TS','1','current','flood_stream'], // flood stream
    ['CAT_TS','2','current','ebb_stream'], // ebb stream
    ['CAT_TS','3','current','other_tidal_flow'], // Other tidal flow

    // COLOUR - Colour - Moved to StringList

    // COLPAT - Colour pattern - Moved to StringList

    // CONDTN - Condition
    ['CONDTN','1','condition','construction'], // under construction
    ['CONDTN','2','condition','destroyed'], // ruined
    ['CONDTN','3','condition','under_reclamation'], // under reclamation
    ['CONDTN','4','condition','wingless'], // wingless
    ['CONDTN','5','condition','planned'], // planned construction

    // CONRAD - Conspicuous, radar
    ['CONRAD','1','radar_conspicuous','yes'], // radar conspicuous
    ['CONRAD','2','radar_conspicuous','no'], // not radar conspicuous
    ['CONRAD','3','radar_conspicuous','reflector'], // radar conspicuous (has radar reflector)

    // CONVIS - Conspicuous, visually
    ['CONVIS','1','CONVIS','conspicuous'], // visually conspicuous
    ['CONVIS','2','CONVIS','not_conspicuous'], // not visually conspicuous

    // DUNITS - Depth units
    // ['DUNITS','1','raw:DUNITS','metres'], // metres
    // ['DUNITS','2','raw:DUNITS','fathoms_and_feet'], // fathoms and feet
    // ['DUNITS','3','raw:DUNITS','feet'], // feet
    // ['DUNITS','4','raw:DUNITS','fathoms_and_fractions'], // fathoms and fractions

    // EXCLIT - Exhibition condition of light
    ['EXCLIT','1','seamark:light:exhibition','24h'], // light shown without change of character
    ['EXCLIT','2','seamark:light:exhibition','day'], // daytime light
    ['EXCLIT','3','seamark:light:exhibition','fog'], // fog light
    ['EXCLIT','4','seamark:light:exhibition','night'], // night light

    // EXPSOU - Exposition of sounding
    ['EXPSOU','1','exposition_of_sounding','within_the_range_of_depth_of_the_surrounding_depth_area'], // within the range of depth of the surrounding depth area
    ['EXPSOU','2','exposition_of_sounding','shoaler_than_the_range_of_depth_of_the_surrounding_depth_area'], // shoaler than the range of depth of the surrounding depth area
    ['EXPSOU','3','exposition_of_sounding','deeper_than_the_range_of_depth_of_the_surrounding_depth_area'], // deeper than the range of depth of the surrounding depth area

    // FUNCTN - Function - Moved to StringList

    // GRUP - Group  
    ['GRUP','1','s57:group','skin_of_the_earth'], 
    // ['GRUP','2','s57:group','all_other_objects'], // This is the default
    ['GRUP','2',undefined,undefined], // This is the default


    // HORDAT - Horizontal datum
    ['HORDAT','1','source:datum:geodetic','world_geodetic_system_1972'], // WGS 72
    ['HORDAT','2','source:datum:geodetic','world_geodetic_system_1984'], // WGS 84
    ['HORDAT','3','source:datum:geodetic','european_1950_(mean_value)'], // European 1950
    ['HORDAT','4','source:datum:geodetic','potsdam_or_helmertturm_(germany)'], // Potsdam Datum
    ['HORDAT','5','source:datum:geodetic','adindan_(mean_value)'], // Adindan
    ['HORDAT','6','source:datum:geodetic','afgooye_(somalia)'], // Afgooye
    ['HORDAT','7','source:datum:geodetic','ain_el_abd_1970_(bahrain_island)'], // Ain el Abd 1970
    ['HORDAT','8','source:datum:geodetic','anna_1_astro_1965_(cocos_islands)'], // Anna 1 Astro 1965
    ['HORDAT','9','source:datum:geodetic','antigua_island_astro_1943'], // Antigua Island Astro 1943
    ['HORDAT','10','source:datum:geodetic','arc_1950_(mean_value)'], // Arc 1950
    ['HORDAT','11','source:datum:geodetic','arc_1960_(mean_value)'], // Arc 1960
    ['HORDAT','12','source:datum:geodetic','ascension_island_1958'], // Ascension Island 1958
    ['HORDAT','13','source:datum:geodetic','astro_beacon_e_(iwo_jima_island)'], // Astro beacon "E" 1945
    ['HORDAT','14','source:datum:geodetic','astro_dos_71/4_(st._helena_island)'], // Astro DOS 71/4
    ['HORDAT','15','source:datum:geodetic','astro_tern_island_1961_(tern_island,_hawaii)'], // Astro Tern Island (FRIG) 1961
    ['HORDAT','16','source:datum:geodetic','astro_station_1952_(marcus_island)'], // Astronomical Station 1952
    ['HORDAT','17','source:datum:geodetic','australian_geod._1966_(australia_and_tasmania_island)'], // Australian Geodetic 1966
    ['HORDAT','18','source:datum:geodetic','australian_geod._1984_(australia_and_tasmania_island)'], // Australian Geodetic 1984
    ['HORDAT','19','source:datum:geodetic','ayabelle_lighthouse_(djibouti)'], // Ayabelle Lighthouse
    ['HORDAT','20','source:datum:geodetic','bellevue_(ign)_(efate_and_erromango_islands)'], // Bellevue (IGN)
    ['HORDAT','21','source:datum:geodetic','bermuda_1957_(bermuda_islands)'], // Bermuda 1957
    ['HORDAT','22','source:datum:geodetic','bissau_(guinea-bissau)'], // Bissau
    ['HORDAT','23','source:datum:geodetic','bogota_observatory_(colombia)'], // Bogota Observatory
    ['HORDAT','24','source:datum:geodetic','bukit_rimpah_(bangka_and_belitung_islands,_indonesia)'], // Bukit Rimpah
    ['HORDAT','25','source:datum:geodetic','camp_area_astro_(camp_mcmurdo_area,_antarctica)'], // Camp Area Astro
    ['HORDAT','26','source:datum:geodetic','campo_inchauspe_(argentina)'], // Campo Inchauspe 1969
    ['HORDAT','27','source:datum:geodetic','canton_astro_1966_(phoenix_islands)'], // Canton Astro 1966
    ['HORDAT','28','source:datum:geodetic','cape_(south_africa)'], // Cape
    ['HORDAT','29','source:datum:geodetic','cape_canaveral_(mean_value)'], // Cape Canaveral
    ['HORDAT','30','source:datum:geodetic','carthage_(tunisia)'], // Carthage
    ['HORDAT','31','source:datum:geodetic','chatham_1971_(chatham_island,_new_zealand)'], // Chatam Island Astro 1971
    ['HORDAT','32','source:datum:geodetic','chua_astro_(paraguay)'], // Chua Astro
    ['HORDAT','33','source:datum:geodetic','corrego_alegre_(brazil)'], // Corrego Alegre
    ['HORDAT','34','source:datum:geodetic','dabola_(guinea)'], // Dabola
    ['HORDAT','35','source:datum:geodetic','djakarta_(batavia)_(sumatra_island,_indonesia)'], // Djakarta (Batavia)
    ['HORDAT','36','source:datum:geodetic','dos_1968_(gizo_island,_new_georgia_islands)'], // DOS 1968
    ['HORDAT','37','source:datum:geodetic','easter_island_1967_(easter_island)'], // Easter Island 1967
    ['HORDAT','38','source:datum:geodetic','european_1979_(mean_value)'], // European 1979
    ['HORDAT','39','source:datum:geodetic','fort_thomas_1955_(nevis,_st_kitts,_leeward_islands)'], // Fort Thomas 1955
    ['HORDAT','40','source:datum:geodetic','gan_1970_(addu_atoll,_republic_of_maldives)'], // Gan 1970
    ['HORDAT','41','source:datum:geodetic','geodetic_datum_1949_(new_zealand)'], // Geodetic Datum 1949
    ['HORDAT','42','source:datum:geodetic','graciosa_base_sw_(faial,_graciosa,_pico,_sao_jorge,_and_terceira_island,_azores)'], // Graciosa Base SW 1948
    ['HORDAT','43','source:datum:geodetic','guam_1963'], // Guam 1963
    ['HORDAT','44','source:datum:geodetic','gunong_segara_(kalimantan_island,_indonesia)'], // Gunung Segara
    ['HORDAT','45','source:datum:geodetic','gux_1_astro_(guadacanal_island)'], // GUX 1 Astro
    ['HORDAT','46','source:datum:geodetic','herat_north_(afganistan)'], // Herat North
    ['HORDAT','47','source:datum:geodetic','hjorsey_1955_(iceland)'], // Hjorsey 1955
    ['HORDAT','48','source:datum:geodetic','hong_kong_1963_(hong_kong)'], // Hong Kong 1963
    ['HORDAT','49','source:datum:geodetic','hu-tzu-shan'], // Hu-Tzu-Shan
    ['HORDAT','50','source:datum:geodetic','indian_(thailand_and_vietnam)'], // Indian
    ['HORDAT','51','source:datum:geodetic','indian_1954_(thailand)'], // Indian 1954
    ['HORDAT','52','source:datum:geodetic','indian_1975_(thailand)'], // Indian 1975
    ['HORDAT','53','source:datum:geodetic','ireland_1965_(ireland_and_northern_ireland)'], // Ireland 1965
    ['HORDAT','54','source:datum:geodetic','ists_061_astro_1968_(south_georgia_islands)'], // ISTS 061 Astro 1968
    ['HORDAT','55','source:datum:geodetic','ists_073_astro_1969_(diego_garcia)'], // ISTS 073 Astro 1969
    ['HORDAT','56','source:datum:geodetic','johnston_island_1961_(johnston_island)'], // Johnston Island 1961
    ['HORDAT','57','source:datum:geodetic','kandawala_(sri_lanka)'], // Kandawala
    ['HORDAT','58','source:datum:geodetic','kerguelen_island_1949_(kerguelen_island)'], // Kerguelen Island 1949
    ['HORDAT','59','source:datum:geodetic','kertau_1948_(or_revised_kertau)_(west_malaysia_and_singapore)'], // Kertau 1948
    ['HORDAT','60','source:datum:geodetic','kusaie_astro_1951'], // Kusaie Astro 1951
    ['HORDAT','61','source:datum:geodetic','l.c._5_astro_1961_(cayman_brac_island)'], // L. C. 5 Astro 1961
    ['HORDAT','62','source:datum:geodetic','leigon_(ghana)'], // Leigon
    ['HORDAT','63','source:datum:geodetic','liberia_1964_(liberia)'], // Liberia 1964
    ['HORDAT','64','source:datum:geodetic','luzon_(philipines_except_mindanao_island)'], // Luzon
    ['HORDAT','65','source:datum:geodetic','mahe_1971_(mahe_island)'], // Mahe 1971
    ['HORDAT','66','source:datum:geodetic','massawa_(eritrea,_ethiopia)'], // Massawa
    ['HORDAT','67','source:datum:geodetic','merchich_(morocco)'], // Merchich
    ['HORDAT','68','source:datum:geodetic','midway_astro_1961_(midway_island)'], // Midway Astro 1961
    ['HORDAT','69','source:datum:geodetic','minna_(cameroon)'], // Minna
    ['HORDAT','70','source:datum:geodetic','montserrat_island_astro_1958'], // Montserrat Island Astro 1958
    ['HORDAT','71','source:datum:geodetic','mporaloko_(gabon)'], // M'Poraloko
    ['HORDAT','72','source:datum:geodetic','nahrwan_(united_arab_emirates)'], // Nahrwan
    ['HORDAT','73','source:datum:geodetic','naparima_(bwi,_trinidad_and_tobago)'], // Naparima, BWI
    ['HORDAT','74','source:datum:geodetic','north_american_1927_(conus_mean)'], // North American 1927
    ['HORDAT','75','source:datum:geodetic','north_american_1983_(conus)'], // North American 1983
    ['HORDAT','76','source:datum:geodetic','observatorio_meteorologico_1939_(corvo_and_flores_islands,_azores)'], // Observatorio Meteorologico 1939
    ['HORDAT','77','source:datum:geodetic','old_egyptian_(egypt)'], // Old Egyptian 1907
    ['HORDAT','78','source:datum:geodetic','old_hawaiian_(mean_value)'], // Old Hawaiian
    ['HORDAT','79','source:datum:geodetic','oman_(oman)'], // Oman
    ['HORDAT','80','source:datum:geodetic','ordnance_survey_g.b._1936_(mean_value)'], // Ordnance Survey of Great Britain 1936
    ['HORDAT','81','source:datum:geodetic','pico_de_las_nieves_(canary_islands)'], // Pico de las Nieves
    ['HORDAT','82','source:datum:geodetic','pitcairn_astro_1967_(pitcairn_island)'], // Pitcairn Astro 1967
    ['HORDAT','83','source:datum:geodetic','point_58_mean_solution_(burkina_faso_and_niger)'], // Point 58
    ['HORDAT','84','source:datum:geodetic','pointe_noire_1948'], // Pointe Noire 1948
    ['HORDAT','85','source:datum:geodetic','se_base_(porto_santo)_(porto_santo_and_madeira_islands)'], // Porto Santo 1936
    ['HORDAT','86','source:datum:geodetic','prov._s._american_1956_(mean_value)'], // Provisional South American 1956
    ['HORDAT','87','source:datum:geodetic','provisional_south_chilean_1963_(also_known_as_hito_xviii_1963)'], // Provisional South Chilean 1963 (also known as Hito XVIII 1963)
    ['HORDAT','88','source:datum:geodetic','puerto_rico_(puerto_rico_and_virgin_islands)'], // Puerto Rico
    ['HORDAT','89','source:datum:geodetic','qatar_national_(qatar)'], // Qatar national
    ['HORDAT','90','source:datum:geodetic','qornoq_(south_greenland)'], // Qornoq
    ['HORDAT','91','source:datum:geodetic','reunion_1947'], // Reunion
    ['HORDAT','92','source:datum:geodetic','rome_1940_(or_monte_mario_1940),_italy'], // Rome 1940
    ['HORDAT','93','source:datum:geodetic','santo_(dos)_1965_(espirito_santo_island)'], // Santo (DOS) 1965
    ['HORDAT','94','source:datum:geodetic','sao_braz_(sao_miguel,_santa_maria_islands,_azores)'], // Sao Braz
    ['HORDAT','95','source:datum:geodetic','sapper_hill_1943_(east_falkland_islands)'], // Sapper Hill 1943
    ['HORDAT','96','source:datum:geodetic','schwarzeck_(namibia)'], // Schwarzeck
    ['HORDAT','97','source:datum:geodetic','selvagem_grande_1938_(salvage_islands)'], // Selvagem Grande 1938
    ['HORDAT','98','source:datum:geodetic','south_american_1969_(mean_value)'], // South American 1969
    ['HORDAT','99','source:datum:geodetic','south_asia_(southeast_asia,_singapore)'], // South Asia
    ['HORDAT','100','source:datum:geodetic','tananarive_observatory_1925'], // Tananarive Observatory 1925
    ['HORDAT','101','source:datum:geodetic','timbalai_1948_(brunei_and_east_malaysia_-_sarawak_and_sabah)'], // Timbalai 1948
    ['HORDAT','102','source:datum:geodetic','tokyo_(mean_value)'], // Tokyo
    ['HORDAT','103','source:datum:geodetic','tristan_astro_1968_(tristan_da_cunha)'], // Tristan Astro 1968
    ['HORDAT','104','source:datum:geodetic','viti_levu_1916_(viti_levu_island,_fiji_islands)'], // Viti Levu 1916
    ['HORDAT','105','source:datum:geodetic','wake-eniwetok_1960'], // Wake-Eniwetok 1960
    ['HORDAT','106','source:datum:geodetic','wake_island_astro_1952'], // Wake Island Astro 1952
    ['HORDAT','107','source:datum:geodetic','yacare_(uruguay)'], // Yacare
    ['HORDAT','108','source:datum:geodetic','zanderij_(surinam)'], // Zanderij
    ['HORDAT','109','source:datum:geodetic','american_samoa_datum_1962'], // American Samoa 1962
    ['HORDAT','110','source:datum:geodetic','deception_island,_antarctica'], // Deception Island
    ['HORDAT','111','source:datum:geodetic','indian_1960_(vietnam:_near_16_degrees_north)'], // Indian 1960
    ['HORDAT','112','source:datum:geodetic','indonesian_1974'], // Indonesian 1974
    ['HORDAT','113','source:datum:geodetic','north_sahara_1959'], // North Sahara 1959
    ['HORDAT','114','source:datum:geodetic','pulkovo_1942_(russia)'], // Pulkovo 1942
    ['HORDAT','115','source:datum:geodetic','s-42_(pulkovo_1942)'], // S-42 (Pulkovo 1942)
    ['HORDAT','116','source:datum:geodetic','s-jysk'], // S-JYSK
    ['HORDAT','117','source:datum:geodetic','voirol_1950'], // Voirol 1950
    ['HORDAT','118','source:datum:geodetic','average_terrestrial_system_1977,_new_brunswick'], // Average Terrestrial System 1977
    ['HORDAT','119','source:datum:geodetic','compensation_geodetique_du_quebec_1977'], // Compensation Géodésique du Québec 1977
    ['HORDAT','120','source:datum:geodetic','kkj_(or_kartastokoordinaattijarjestelma),_finland'], // Finnish (KKJ)
    ['HORDAT','121','source:datum:geodetic','ordnance_survey_of_ireland'], // Ordnance Survey of Ireland
    ['HORDAT','122','source:datum:geodetic','revised_kertau_1948'], // Revised Kertau
    ['HORDAT','123','source:datum:geodetic','revised_nahrwan'], // Revised Nahrwan
    ['HORDAT','124','source:datum:geodetic','greek_geodetic_reference_system_1987_(ggrs_87)'], // GGRS 76 (Greece)
    ['HORDAT','125','source:datum:geodetic','new_french_or_nouvelle_triangulation_francaise_(ntf)_with_zero_meridian_paris'], // Nouvelle Triangulation de France
    ['HORDAT','126','source:datum:geodetic','rt90,_stockholm,_sweden'], // RT 90 (Sweden)
    ['HORDAT','127','source:datum:geodetic','geocentric_datum_of_australia_(gda)'], // Geocentric Datum of Australia (GDA)
    ['HORDAT','128','source:datum:geodetic','bjz54_(a954_beijing_coordinates)_(china)'], // BJZ54 (A954 Beijing Coordinates)
    ['HORDAT','129','source:datum:geodetic','modified_bjz54_(china)'], // Modified BJZ54
    ['HORDAT','130','source:datum:geodetic','gdz80_(china)'], // GDZ80
    ['HORDAT','131','source:datum:geodetic','local_astro'], // Local datum

    // HUNITS - Height/length units
    // ['HUNITS','1','s57:units','metres'], // metres
    // ['HUNITS','2','s57:units','feet'], // feet

    // JRSDTN - Jurisdiction
    ['JRSDTN','1','s57:jurisdiction','international'], // international
    ['JRSDTN','2','s57:jurisdiction','national'], // national
    ['JRSDTN','3','s57:jurisdiction','national_sub-division'], // national sub-division

    // LITCHR - Light characteristic
    ['LITCHR','1','seamark:light:character','F'], // fixed
    ['LITCHR','2','seamark:light:character','FL'], // flashing
    ['LITCHR','3','seamark:light:character','LFl'], // long-flashing
    ['LITCHR','4','seamark:light:character','Q'], // quick-flashing
    ['LITCHR','5','seamark:light:character','VQ'], // very quick-flashing
    ['LITCHR','6','seamark:light:character','UQ'], // ultra quick-flashing
    ['LITCHR','7','seamark:light:character','Iso'], // isophased
    ['LITCHR','8','seamark:light:character','Oc'], // occulting
    ['LITCHR','9','seamark:light:character','IQ'], // interrupted quick-flashing
    ['LITCHR','10','seamark:light:character','IVQ'], // interrupted very quick-flashing
    ['LITCHR','11','seamark:light:character','IUQ'], // interrupted ultra quick-flashing
    ['LITCHR','12','seamark:light:character','Mo'], // morse
    ['LITCHR','13','seamark:light:character','FFL'], // fixed/flash
    ['LITCHR','14','seamark:light:character','FILFI'], // flash/long-flash
    ['LITCHR','15','seamark:light:character','OcFl'], // occulting/flash
    ['LITCHR','16','seamark:light:character','FLFl'], // fixed/long-flash
    ['LITCHR','17','seamark:light:character','Al.Oc'], // occulting alternating
    ['LITCHR','18','seamark:light:character','Al.LFl'], // long-flash alternating
    ['LITCHR','19','seamark:light:character','Al.Fl'], // flash alternating
    // ['LITCHR','20','seamark:light:character','AL.Gr'], // group alternating
    // ['LITCHR','21','seamark:light:character','2_fixed_(vertical)'], // 2 fixed (vertical)
    // ['LITCHR','22','seamark:light:character','2_fixed_(horizontal)'], // 2 fixed (horizontal)
    // ['LITCHR','23','seamark:light:character','3_fixed_(vertical)'], // 3 fixed (vertical)
    // ['LITCHR','24','seamark:light:character','3_fixed_(horizontal)'], // 3 fixed (horizontal)
    ['LITCHR','25','seamark:light:character','Q+LFl'], // quick-flash plus long-flash
    ['LITCHR','26','seamark:light:character','VQ+LFl'], // very quick-flash plus long-flash
    ['LITCHR','27','seamark:light:character','UQ+LFl'], // ultra quick-flash plus long-flash
    ['LITCHR','28','seamark:light:character','Al'], // alternating
    ['LITCHR','29','seamark:light:character','Al.FFl'], // fixed and alternating flashing

    // LITVIS - Light visibility - Moved to StringList

    // MARSYS - Marks navigational - System of
    ['MARSYS','1','MARSYS','iala-a'], // IALA A
    ['MARSYS','2','MARSYS','iala-b'], // IALA B
    // ['MARSYS','3','MARSYS','modified_us'], // modified US
    // ['MARSYS','4','MARSYS','old_us'], // old US
    // ['MARSYS','5','MARSYS','us_intracoastal_waterway'], // US intracoastal waterway
    // ['MARSYS','6','MARSYS','us_uniform_state'], // US uniform state
    // ['MARSYS','7','MARSYS','us_western_rivers'], // US western rivers
    // ['MARSYS','8','MARSYS','signi'], // SIGNI
    ['MARSYS','9','MARSYS','none'], // no system
    ['MARSYS','10','MARSYS','other'], // other system

    // NATCON - Nature of construction - Moved to StringList

    // NATQUA - Nature of surface - qualifying terms - Moved to StringList

    // NATSUR - Nature of surface - Moved to StringList

    // PRODCT - Product - Moved to StringList

    // PUNITS - Positional accuracy units
    // Not valid

    // OBJL - Object Label
    ['OBJL','1','seamark:type','administration_area'], // ADMARE Administration area (Named)
    ['OBJL','2','seamark:type','airport'], // AIRARE Airport / airfield
    ['OBJL','3','seamark:type','anchor_berth'], // ACHBRT Anchor berth
    ['OBJL','4','seamark:type','anchorage'], // ACHARE Anchorage area
    ['OBJL','5','seamark:type','beacon_cardinal'], // BCNCAR Beacon, cardinal
    ['OBJL','6','seamark:type','beacon_isolated_danger'], // BCNISD Beacon, isolated danger
    ['OBJL','7','seamark:type','beacon_lateral'], // BCNLAT Beacon, lateral
    ['OBJL','8','seamark:type','beacon_safe_water'], // BCNSAW Beacon, safe water
    ['OBJL','9','seamark:type','beacon_special_purpose'], // BCNSPP Beacon, special purpose/general
    ['OBJL','10','seamark:type','berth'], // BERTHS Berth
    ['OBJL','11','seamark:type','bridge'], // BRIDGE Bridge
    ['OBJL','12','seamark:type','building'], // BUISGL Building, single
    ['OBJL','13','seamark:type','built_up_area'], // BUAARE Built-up area
    ['OBJL','14','seamark:type','buoy_cardinal'], // BOYCAR Buoy, cardinal
    ['OBJL','15','seamark:type','buoy_installation'], // BOYINB Buoy, installation
    ['OBJL','16','seamark:type','buoy_isolated_danger'], // BOYISD Buoy, isolated danger
    ['OBJL','17','seamark:type','buoy_lateral'], // BOYLAT Buoy, lateral
    ['OBJL','18','seamark:type','buoy_safe_water'], // BOYSAW Buoy, safe water
    ['OBJL','19','seamark:type','buoy_special_purpose'], // BOYSPP Buoy, special purpose/general
    ['OBJL','20','seamark:type','cable_area'], // CBLARE Cable area
    ['OBJL','21','seamark:type','cable_overhead'], // CBLOHD Cable, overhead
    ['OBJL','22','seamark:type','cable_submarine'], // CBLSUB Cable, submarine
    ['OBJL','23','seamark:type','canal'], // CANALS Canal
    // ['OBJL','24','seamark:type','canal_bank'], // CANBNK Canal bank
    ['OBJL','25','seamark:type','cargo_transhipment_area'], // CTSARE Cargo transshipment area
    ['OBJL','26','seamark:type','causeway'], // CAUSWY Causeway
    ['OBJL','27','seamark:type','caution_area'], // CTNARE Caution area
    ['OBJL','28','seamark:type','checkpoint'], // CHKPNT Checkpoint
    ['OBJL','29','seamark:type','coastguard_station'], // CGUSTA Coastguard station
    ['OBJL','30','seamark:type','coastline'], // COALNE Coastline
    ['OBJL','31','seamark:type','contiguous_zone'], // CONZNE Contiguous zone
    ['OBJL','32','seamark:type','continental_shelf_area'], // COSARE Continental shelf area
    ['OBJL','33','seamark:type','control_point'], // CTRPNT Control point
    ['OBJL','34','seamark:type','conveyor'], // CONVYR Conveyor
    ['OBJL','35','seamark:type','crane'], // CRANES Crane
    ['OBJL','36','seamark:type','non_gravitational_current'], // CURENT Current - non - gravitational
    ['OBJL','37','seamark:type','custom_zone'], // CUSZNE Custom zone
    ['OBJL','38','seamark:type','dam'], // DAMCON Dam
    ['OBJL','39','seamark:type','daymark'], // DAYMAR Daymark
    ['OBJL','40','seamark:type','deep_water_route_centerline'], // DWRTCL Deep water route centerline
    ['OBJL','41','seamark:type','deep_water_route_part'], // DWRTPT Deep water route part
    ['OBJL','42','seamark:type','depth_area'], // DEPARE Depth area
    ['OBJL','43','seamark:type','depth_contour'], // DEPCNT Depth contour
    ['OBJL','44','seamark:type','distance_mark'], // DISMAR Distance mark
    ['OBJL','45','seamark:type','dock'], // DOCARE Dock area
    ['OBJL','46','seamark:type','dredged_area'], // DRGARE Dredged area
    ['OBJL','47','seamark:type','dry_dock'], // DRYDOC Dry dock
    ['OBJL','48','seamark:type','dumping_ground'], // DMPGRD Dumping ground
    ['OBJL','49','seamark:type','dyke'], // DYKCON Dyke
    ['OBJL','50','seamark:type','exclusive_economic_zone'], // EXEZNE Exclusive Economic Zone
    ['OBJL','51','seamark:type','fairway'], // FAIRWY Fairway
    ['OBJL','52','seamark:type','wall'], // FNCLNE Fence/wall
    ['OBJL','53','seamark:type','ferry_route'], // FERYRT Ferry route
    ['OBJL','54','seamark:type','fishery_zone'], // FSHZNE Fishery zone
    ['OBJL','55','seamark:type','fishing_facility'], // FSHFAC Fishing facility
    ['OBJL','56','seamark:type','fishing_ground'], // FSHGRD Fishing ground
    ['OBJL','57','seamark:type','floating_dock'], // FLODOC Floating dock
    ['OBJL','58','seamark:type','fog_signal'], // FOGSIG Fog signal
    ['OBJL','59','seamark:type','fortified_structure'], // FORSTC Fortified structure
    ['OBJL','60','seamark:type','free_port_area'], // FRPARE Free port area
    ['OBJL','61','seamark:type','gate'], // GATCON Gate
    ['OBJL','62','seamark:type','gridiron'], // GRIDRN Gridiron
    ['OBJL','63','seamark:type','harbour_area'], // HRBARE Harbour area (administrative)
    ['OBJL','64','seamark:type','harbour'], // HRBFAC Harbour facility
    ['OBJL','65','seamark:type','hulk'], // HULKES Hulk
    ['OBJL','66','seamark:type','ice_area'], // ICEARE Ice area
    ['OBJL','67','seamark:type','incineration_area'], // ICNARE Incineration area
    ['OBJL','68','seamark:type','inshore_traffic_zone'], // ISTZNE Inshore traffic zone
    ['OBJL','69','seamark:type','lake'], // LAKARE Lake
    // ['OBJL','70','seamark:type','lake_shore'], // LAKSHR Lake shore
    ['OBJL','71','seamark:type','land_area'], // LNDARE Land area
    ['OBJL','72','seamark:type','land_elevation'], // LNDELV Land elevation
    ['OBJL','73','seamark:type','land_region'], // LNDRGN Land region
    ['OBJL','74','seamark:type','landmark'], // LNDMRK Landmark
    ['OBJL','75','seamark:type','light'], // LIGHTS Light
    ['OBJL','76','seamark:type','light_float'], // LITFLT Light float
    ['OBJL','77','seamark:type','light_vessel'], // LITVES Light vessel
    ['OBJL','78','seamark:type','local_magnetic_anomaly'], // LOCMAG Local magnetic anomaly
    ['OBJL','79','seamark:type','lock_basin'], // LOKBSN Lock basin
    ['OBJL','80','seamark:type','log_pond'], // LOGPON Log pond
    ['OBJL','81','seamark:type','magnetic_variation'], // MAGVAR Magnetic variation
    ['OBJL','82','seamark:type','marine_farm'], // MARCUL Marine farm/culture
    ['OBJL','83','seamark:type','military_area'], // MIPARE Military practice area
    ['OBJL','84','seamark:type','mooring'], // MORFAC Mooring/warping facility
    ['OBJL','85','seamark:type','navigation_line'], // NAVLNE Navigation line
    ['OBJL','86','seamark:type','obstruction'], // OBSTRN Obstruction
    ['OBJL','87','seamark:type','platform'], // OFSPLF Offshore platform
    ['OBJL','88','seamark:type','production_area'], // OSPARE Offshore production area
    ['OBJL','89','seamark:type','oil_barrier'], // OILBAR Oil barrier
    ['OBJL','90','seamark:type','pile'], // PILPNT Pile
    ['OBJL','91','seamark:type','pilot_boarding'], // PILBOP Pilot boarding place
    ['OBJL','92','seamark:type','pipeline_area'], // PIPARE Pipeline area
    ['OBJL','93','seamark:type','pipeline_overhead'], // PIPOHD Pipeline, overhead
    ['OBJL','94','seamark:type','pipeline_submarine'], // PIPSOL Pipeline, submarine/on land
    ['OBJL','95','seamark:type','pontoon'], // PONTON Pontoon
    ['OBJL','96','seamark:type','precautionary_area'], // PRCARE Precautionary area
    ['OBJL','97','seamark:type','production_storage_area'], // PRDARE Production / storage area
    ['OBJL','98','seamark:type','pylon'], // PYLONS Pylon/bridge support
    ['OBJL','99','seamark:type','radar_line'], // RADLNE Radar line
    ['OBJL','100','seamark:type','radar_range'], // RADRNG Radar range
    ['OBJL','101','seamark:type','radar_reflector'], // RADRFL Radar reflector
    ['OBJL','102','seamark:type','radar_station'], // RADSTA Radar station
    ['OBJL','103','seamark:type','radar_transponder'], // RTPBCN Radar transponder beacon
    ['OBJL','104','seamark:type','calling_in_point'], // RDOCAL Radio calling-in point
    ['OBJL','105','seamark:type','radio_station'], // RDOSTA Radio station
    ['OBJL','106','seamark:type','rail'], // RAILWY Railway
    ['OBJL','107','seamark:type','rapids'], // RAPIDS Rapids
    ['OBJL','108','seamark:type','reccomended_route_centerline'], // RCRTCL Recommended route centerline
    ['OBJL','109','seamark:type','reccomended_track'], // RECTRC Recommended track
    ['OBJL','110','seamark:type','reccomended_traffic_lane'], // RCTLPT Recommended Traffic Lane Part
    ['OBJL','111','seamark:type','rescue_station'], // RSCSTA Rescue station
    ['OBJL','112','seamark:type','restricted_area'], // RESARE Restricted area
    ['OBJL','113','seamark:type','retro_reflector'], // RETRFL Retro-reflector
    ['OBJL','114','seamark:type','river'], // RIVERS River
    // ['OBJL','115','seamark:type','riverbank'], // RIVBNK River bank
    ['OBJL','116','seamark:type','road'], // ROADWY Road
    ['OBJL','117','seamark:type','runway'], // RUNWAY Runway
    ['OBJL','118','seamark:type','sand_waves'], // SNDWAV Sand waves
    ['OBJL','119','seamark:type','sea_area'], // SEAARE Sea area / named water area
    ['OBJL','120','seamark:type','seaplane_landing_area'], // SPLARE Sea-plane landing area
    ['OBJL','121','seamark:type','seabed_area'], // SBDARE Seabed area
    ['OBJL','122','seamark:type','shoreline_construction'], // SLCONS Shoreline Construction
    ['OBJL','123','seamark:type','signal_station_traffic'], // SISTAT Signal station, traffic
    ['OBJL','124','seamark:type','signal_station_warning'], // SISTAW Signal station, warning
    ['OBJL','125','seamark:type','silo'], // SILTNK Silo / tank
    ['OBJL','126','seamark:type','slope_topline'], // SLOTOP Slope topline
    ['OBJL','127','seamark:type','sloping_ground'], // SLOGRD Sloping ground
    ['OBJL','128','seamark:type','small_craft_facility'], // SMCFAC Small craft facility
    ['OBJL','129','seamark:type','sounding'], // SOUNDG Sounding
    ['OBJL','130','seamark:type','spring'], // SPRING Spring
    // ['OBJL','131','seamark:type','square'], // SQUARE Square
    ['OBJL','132','seamark:type','straight_territorial_sea_baseline'], // STSLNE Straight territorial sea baseline
    ['OBJL','133','seamark:type','submarine_transit_lane'], // SUBTLN Submarine transit lane
    ['OBJL','134','seamark:type','swept_area'], // SWPARE Swept Area
    ['OBJL','135','seamark:type','territorial_sea_area'], // TESARE Territorial sea area
    ['OBJL','136','seamark:type','tidal_stream_harmonic_prediction'], // TS_PRH Tidal stream - harmonic prediction
    ['OBJL','137','seamark:type','tidal_stream_non-harmonic_prediction'], // TS_PNH Tidal stream - non-harmonic prediction
    ['OBJL','138','seamark:type','tidal_stream_panel_data'], // TS_PAD Tidal stream panel data
    ['OBJL','139','seamark:type','tidal_stream_time_series'], // TS_TIS Tidal stream - time series
    ['OBJL','140','seamark:type','tide_harmonic_prediction'], // T_HMON Tide - harmonic prediction
    ['OBJL','141','seamark:type','tide_non-harmonic_prediction'], // T_NHMN Tide - non-harmonic prediction
    ['OBJL','142','seamark:type','tide_time_series'], // T_TIMS Tidal stream - time series
    ['OBJL','143','seamark:type','tideway'], // TIDEWY Tideway
    ['OBJL','144','seamark:type','topmark'], // TOPMAR Top mark
    ['OBJL','145','seamark:type','separation_line'], // TSELNE Traffic Separation Line
    ['OBJL','146','seamark:type','separation_boundary'], // TSSBND Traffic Separation Scheme  Boundary
    ['OBJL','147','seamark:type','separation_crossing'], // TSSCRS Traffic Separation Scheme Crossing
    ['OBJL','148','seamark:type','separation_lane'], // TSSLPT Traffic Separation Scheme  Lane part
    ['OBJL','149','seamark:type','separation_roundabout'], // TSSRON Traffic Separation Scheme  Roundabout
    ['OBJL','150','seamark:type','separation_zone'], // TSEZNE Traffic Separation Zone
    ['OBJL','151','seamark:type','tunnel'], // TUNNEL Tunnel
    ['OBJL','152','seamark:type','two-way_route'], // TWRTPT Two-way route  part
    ['OBJL','153','seamark:type','rock'], // UWTROC Underwater rock / awash rock
    ['OBJL','154','seamark:type','unsurveyed_area'], // UNSARE Unsurveyed area
    ['OBJL','155','seamark:type','vegetation'], // VEGATN Vegetation
    ['OBJL','156','seamark:type','water_turbulence'], // WATTUR Water turbulence
    ['OBJL','157','seamark:type','waterfall'], // WATFAL Waterfall
    ['OBJL','158','seamark:type','weed'], // WEDKLP Weed/Kelp
    ['OBJL','159','seamark:type','wreck'], // WRECKS Wreck
    ['OBJL','160','seamark:type','tidal_stream'], // TS_FEB Tidal stream - flood/ebb
    ['OBJL','161','seamark:type','archipelagic_sea_lane'], // ARCSLN Archipelagic sea lane
    ['OBJL','162','seamark:type','archipelagic_sea_lane_axis'], // ASLXIS Archipelagic sea lane axis
    ['OBJL','163','seamark:type','new_object'], // NEWOBJ New object
    ['OBJL','300','seamark:type','data_accuracy'], // M_ACCY Accuracy of data
    ['OBJL','301','seamark:type','compilation_scale'], // M_CSCL Compilation scale of data
    ['OBJL','302','seamark:type','coverage'], // M_COVR Coverage
    // ['OBJL','303','seamark:type','XTYPE'], // M_HDAT Horizontal datum of data
    ['OBJL','304','seamark:type','horizontal_datum_shift_parameters'], // M_HOPA Horizontal datum shift parameters
    ['OBJL','305','seamark:type','nautical_publication'], // M_NPUB Nautical publication information
    ['OBJL','306','seamark:type','navigation_system_of_marks'], // M_NSYS Navigational system of marks
    // ['OBJL','307','seamark:type','production_information'], // M_PROD Production information
    ['OBJL','308','seamark:type','data_quality'], // M_QUAL Quality of data
    ['OBJL','309','seamark:type','sounding_datum'], // M_SDAT Sounding datum
    ['OBJL','310','seamark:type','survey_reliability'], // M_SREL Survey reliability
    // ['OBJL','311','seamark:type','XTYPE'], // M_UNIT Units of measurement of data
    ['OBJL','312','seamark:type','vertical_datum'], // M_VDAT Vertical datum of data
    ['OBJL','400','seamark:type','aggregation'], // C_AGGR Aggregation
    ['OBJL','401','seamark:type','association'], // C_ASSO Association
    // ['OBJL','402','seamark:type','XTYPE'], // C_STAC Stacked on/stacked under
    // ['OBJL','500','seamark:type','XTYPE'], // $AREAS Cartographic area
    // ['OBJL','501','seamark:type','XTYPE'], // $LINES Cartographic line
    // ['OBJL','502','seamark:type','XTYPE'], // $CSYMB Cartographic symbol
    // ['OBJL','503','seamark:type','XTYPE'], // $COMPS Compass
    // ['OBJL','504','seamark:type','XTYPE'], // $TEXTS Text

    // QUAPOS - Quality of position
    ['QUAPOS','1','position:quality','surveyed'], // surveyed
    ['QUAPOS','2','position:quality','unsurveyed'], // unsurveyed
    ['QUAPOS','3','position:quality','inadequately_surveyed'], // inadequately surveyed
    ['QUAPOS','4','position:quality','approximate'], // approximate
    ['QUAPOS','5','position:quality','position_doubtful'], // position doubtful
    ['QUAPOS','6','position:quality','unreliable'], // unreliable
    ['QUAPOS','7','position:quality','reported_not_surveyed'], // reported (not surveyed)
    ['QUAPOS','8','position:quality','reported_not_confirmed'], // reported (not confirmed)
    ['QUAPOS','9','position:quality','estimated'], // estimated
    ['QUAPOS','10','position:quality','precisely_known'], // precisely known
    ['QUAPOS','11','position:quality','calculated'], // calculated

    // QUASOU - Quality of sounding measurement - Moved to StringList

    // QUAVEM - Quality of vertical measurement
    // ['QUAVEM','1','raw:QUAVEM','measured'], // measured
    // ['QUAVEM','2','raw:QUAVEM','estimated'], // estimated

    // RESTRN - Restriction - Moved to StringList

    // SIGGEN - Signal generation
    ['SIGGEN','1','seamark:fog_signal:generation','automatic'], // automatically
    ['SIGGEN','2','seamark:fog_signal:generation','wave'], // by wave action
    ['SIGGEN','3','seamark:fog_signal:generation','hand'], // by hand
    ['SIGGEN','4','seamark:fog_signal:generation','wind'], // by wind

    // STATUS - Status - Moved to StringList

    // SURTYP - Survey type - Moved to StringList

    // TECSOU - Technique of sounding measurement - Moved to StringList

    // TOPSHP - Topmark/daymark shape
    ['TOPSHP','1','TOPSHP','cone, point up'], // cone, point up
    ['TOPSHP','2','TOPSHP','cone, point down'], // cone, point down
    ['TOPSHP','3','TOPSHP','sphere'], // sphere
    ['TOPSHP','4','TOPSHP','2_spheres'], // 2 spheres
    ['TOPSHP','5','TOPSHP','cylinder'], // cylinder (can)
    ['TOPSHP','6','TOPSHP','board'], // board
    ['TOPSHP','7','TOPSHP','x-shape'], // x-shape (St. Andrew=s cross)
    ['TOPSHP','8','TOPSHP','cross'], // upright cross (St George=s cross)
    ['TOPSHP','9','TOPSHP','cube, point up'], // cube, point up
    ['TOPSHP','10','TOPSHP','2 cones point together'], // 2 cones, point to point
    ['TOPSHP','11','TOPSHP','2 cones base together'], // 2 cones, base to base
    ['TOPSHP','12','TOPSHP','rhombus'], // rhombus (diamond)
    ['TOPSHP','13','TOPSHP','2 cones up'], // 2 cones (points upward)
    ['TOPSHP','14','TOPSHP','2 cones down'], // 2 cones (points downward)
    ['TOPSHP','15','TOPSHP','besom, point up'], // besom, point up (broom or perch)
    ['TOPSHP','16','TOPSHP','besom, point down'], // besom, point down (broom or perch)
    ['TOPSHP','17','TOPSHP','flag'], // flag
    ['TOPSHP','18','TOPSHP','sphere over rhombus'], // sphere over rhombus
    ['TOPSHP','19','TOPSHP','square'], // square
    ['TOPSHP','20','TOPSHP','rectangle, horizontal'], // rectangle, horizontal
    ['TOPSHP','21','TOPSHP','rectangle, vertical'], // rectangle, vertical
    ['TOPSHP','22','TOPSHP','trapezium, up'], // trapezium, up
    ['TOPSHP','23','TOPSHP','trapezium, down'], // trapezium, down
    ['TOPSHP','24','TOPSHP','triangle, point up'], // triangle, point up
    ['TOPSHP','25','TOPSHP','triangle, point down'], // triangle, point down
    ['TOPSHP','26','TOPSHP','circle'], // circle
    ['TOPSHP','27','TOPSHP','2 upright crosses'], // two upright crosses (one over the other)
    ['TOPSHP','28','TOPSHP','t-shape'], // T-shape
    ['TOPSHP','29','TOPSHP','triangle, point up over circle'], // triangle pointing up over a circle
    ['TOPSHP','30','TOPSHP','upright cross over circle'], // upright cross over a circle
    ['TOPSHP','31','TOPSHP','rhombus over circle'], // rhombus over a circle
    ['TOPSHP','32','TOPSHP','circle over triangle, point up'], // circle over a triangle pointing up
    ['TOPSHP','33','TOPSHP','other'], // other shape (see INFORM)

    // TRAFIC - Traffic flow
    ['TRAFIC','1','TRAFIC','inbound'], // inbound
    ['TRAFIC','2','TRAFIC','outbound'], // outbound
    ['TRAFIC','3','TRAFIC','one-way'], // one-way
    ['TRAFIC','4','TRAFIC','two-way'], // two-way

    // T_ACWL - Tide - accuracy of water level
    ['T_ACWL','1','T_ACWL','better_than_0.1_m_and_10_minutes'], // better than 0.1 m and 10 minutes
    ['T_ACWL','2','T_ACWL','worse_than_0.1_m_or_10_minutes'], // worse than 0.1 m or 10 minutes

    // T_MTOD - Tide - method of tidal prediction
    ['T_MTOD','1','T_MTOD','simplified_harmonic_method'], // simplified harmonic method of tidal prediction
    ['T_MTOD','2','T_MTOD','full_harmonic_method'], // full harmonic method of tidal prediction
    ['T_MTOD','3','T_MTOD','time_and_height_difference_non-harmonic_method'], // time and height difference non-harmonic method

    // VERDAT - Vertical datum
    ['VERDAT','1','VERDAT','mlws'], // Mean low water springs
    ['VERDAT','2','VERDAT','mllws'], // Mean lower low water springs
    ['VERDAT','3','VERDAT','msl'], // Mean sea level
    ['VERDAT','4','VERDAT','llw'], // Lowest low water
    ['VERDAT','5','VERDAT','mlw'], // Mean low water
    ['VERDAT','6','VERDAT','llws'], // Lowest low water springs
    ['VERDAT','7','VERDAT','amlws'], // Approximate mean low water springs
    ['VERDAT','8','VERDAT','islw'], // Indian spring low water
    ['VERDAT','9','VERDAT','lws'], // Low water springs
    ['VERDAT','10','VERDAT','alat'], // Approximate lowest astronomical tide
    ['VERDAT','11','VERDAT','nllw'], // Nearly lowest low water
    ['VERDAT','12','VERDAT','mllw'], // Mean lower low water
    ['VERDAT','13','VERDAT','lw'], // Low water
    ['VERDAT','14','VERDAT','amlw'], // Approximate mean low water
    ['VERDAT','15','VERDAT','amllw'], // Approximate mean lower low water
    ['VERDAT','16','VERDAT','mhw'], // Mean high water
    ['VERDAT','17','VERDAT','mhws'], // Mean high water springs
    ['VERDAT','18','VERDAT','hw'], // High water
    ['VERDAT','19','VERDAT','amsl'], // Approximate mean sea level
    ['VERDAT','20','VERDAT','hws'], // High water springs
    ['VERDAT','21','VERDAT','mhhw'], // Mean higher high water
    ['VERDAT','22','VERDAT','eslw'], // Equinoctial spring low water
    ['VERDAT','23','VERDAT','lat'], // Lowest astronomical tide
    ['VERDAT','24','VERDAT','local'], // Local datum
    ['VERDAT','25','VERDAT','igld1985'], // International Great Lakes Datum 1985
    ['VERDAT','26','VERDAT','mwl'], // Mean water level
    ['VERDAT','27','VERDAT','llwlt'], // Lower low water large tide
    ['VERDAT','28','VERDAT','hhwlt'], // Higher high water large tide
    ['VERDAT','29','VERDAT','nhhw'], // Nearly highest high water
    ['VERDAT','30','VERDAT','hat'], // Highest astronomical tide (HAT)

    // WATLEV - Water level effect
    ['WATLEV','1','WATLEV','partly_submerged'], // partly submerged at high water
    ['WATLEV','2','WATLEV','dry'], // always dry
    ['WATLEV','3','WATLEV','submerged'], // always under water/submerged
    ['WATLEV','4','WATLEV','covers'], // covers and uncovers
    ['WATLEV','5','WATLEV','awash'], // awash
    ['WATLEV','6','WATLEV','floods'], // subject to inundation or flooding
    ['WATLEV','7','WATLEV','floating'], // floating
    ], // End one2one

  // StringList - The list of attributes that can have more than one "value"
  // These attributes make a tag that is a list of values
  // E.g. color = white;blue;while
  stringList : ['CATACH','CATAIR','CATBRG','CATDPG','CATHAF', 
  'CATHLK','CATLIT','CATLMK','CATLND','CATMPA','CATOFP','CATPIP','CATREA','CATROS',
  'CATRSC','CATSCF','CATSIT','CATSIW','CATSPM','CATVEG','COLOUR','COLPAT','FUNCTN',
  'LITVIS','NATCON','NATQUA','NATSUR','PRODCT','QUASOU','RESTRN','STATUS','SURTYP',
  'TECSOU'],

  // The translation for StringList attributes
  // These make seamark:<object>:<translation>  tags
  seamarkTypeTag : {
  'BCNSHP':'shape','BOYSHP':'shape','BUISHP':'shape','BURDEP':'depth_buried','CATACH':'category',
  'CATAIR':'category','CATBRG':'category','CATCBL':'category','CATCAM':'category','CATDPG':'category',
  'CATHAF':'category','CATHLK':'category','CATLAM':'category','CATLIT':'category','CATLMK':'category',
  'CATMPA':'category','CATOFP':'category','CATPIP':'category','CATREA':'category','CATROS':'category',
  'CATRSC':'category','CATSCF':'category','CATSIT':'category','CATSIW':'category','CATSPM':'category',
  'CATVEG':'category','COLOUR':'colour','COLPAT':'colour_pattern','COMCHA':'channel',
  'CONVIS':'conspicuity','DRVAL1':'minimum_depth','DRVAL2':'maximum_depth','ELEVAT':'elevation',
  'ESTRNG':'range','FUNCTN':'function','HEIGHT':'height','HORCLR':'clearance_width','HORLEN':'length',
  'HORWID':'width','LIFCAP':'maximum_load','LITVIS':'visibility','MARSYS':'system','MLTYLT':'multiple',
  'NATCON':'construction','NATQUA':'surface_qualification','NATSUR':'surface','ORIENT':'orientation',
  'PICREP':'picture','PRODCT':'product','QUASOU':'sounding_quality','RADIUS':'radius','RADWAL':'wavelength',
  'RESTRN':'restriction','SECTR1':'sector_start','SECTR2':'sector_end','SIGFRQ':'frequency',
  'SIGGRP':'group','SIGPER':'period','SIGSEQ':'sequence','STATUS':'status','T_ACWL':'water_level_accuracy',
  'T_MTOD':'tidal_prediction_method',  'TECSOU':'sounding_technique','TOPSHP':'shape','TRAFIC':'traffic_flow',
  'VALMXR':'maximum_range','VALNMR':'range','VALSOU':'depth','VERCCL':'clearance_height_closed',
  'VERCLR':'clearance_height','VERCOP':'clearance_height_open','VERCSA':'clearance_height_safe',
  'VERDAT':'vertical_datum','VERLEN':'vertical_length','WATLEV':'water_level',
  },


  // This is the opposite of "seamarkTypeTag" and is used to convert back to a seamark type
  tagToSeamarkType : {
  "channel":"COMCHA","clearance_height":"VERCLR","clearance_height_closed":"VERCCL",
  "clearance_height_open":"VERCOP","clearance_height_safe":"VERCSA","clearance_width":"HORCLR",
  "colour":"COLOUR","colour_pattern":"COLPAT","conspicuity":"CONVIS",
  "construction":"NATCON","depth":"VALSOU","depth_buried":"BURDEP",
  "elevation":"ELEVAT","frequency":"SIGFRQ","function":"FUNCTN",
  "group":"SIGGRP","height":"HEIGHT","length":"HORLEN",
  "maximum_depth":"DRVAL2","maximum_load":"LIFCAP","maximum_range":"VALMXR",
  "minimum_depth":"DRVAL1","multiple":"MLTYLT","orientation":"ORIENT",
  "period":"SIGPER","picture":"PICREP","product":"PRODCT",
  "radius":"RADIUS","range":"VALNMR","restriction":"RESTRN",
  "sector_end":"SECTR2","sector_start":"SECTR1","sequence":"SIGSEQ",
  "sounding_quality":"QUASOU","sounding_technique":"TECSOU","status":"STATUS",
  "surface":"NATSUR","surface_qualification":"NATQUA","system":"MARSYS",
  "tidal_prediction_method":"T_MTOD","traffic_flow":"TRAFIC","vertical_datum":"VERDAT",
  "vertical_length":"VERLEN","visibility":"LITVIS","water_level":"WATLEV",
  "water_level_accuracy":"T_ACWL","wavelength":"RADWAL","width":"HORWID",
  },

  // There are quite a few different "category" lists
  categoryTagToSeamarkType : {
  'seamark:daymark:shape':'TOPSHP','seamark:topmark:shape':'TOPSHP','seamark:beacon_cardinal:shape':'BCNSHP',
  'seamark:beacon_isolated_danger:shape':'BCNSHP','seamark:beacon_lateral:shape':'BCNSHP','seamark:beacon_safe_water:shape':'BCNSHP',
  'seamark:beacon_special_purpose:shape':'BCNSHP','seamark:buoy_cardinal:shape':'BOYSHP','seamark:buoy_installation:shape':'BOYSHP',
  'seamark:buoy_isolated_danger:shape':'BOYSHP','seamark:buoy_lateral:shape':'BOYSHP','seamark:buoy_safe_water:shape':'BOYSHP',
  'seamark:buoy_special_purpose:shape':'BOYSHP','seamark:mooring:shape':'BOYSHP','seamark:building:shape':'BUISHP',
  'seamark:silo:shape':'BUISHP','seamark:anchor_berth:category':'CATACH','seamark:anchorage:category':'CATACH',
  'seamark:airport:category':'CATAIR','seamark:bridge:category':'CATBRG','seamark:cable_area:category':'CATCBL',
  'seamark:cable_overhead:category':'CATCBL','seamark:cable_submarine:category':'CATCBL','seamark:beacon_cardinal:category':'CATCAM',
  'seamark:buoy_cardinal:category':'CATCAM','seamark:dumping_ground:category':'CATDPG','seamark:harbour:category':'CATHAF',
  'seamark:hulf:category':'CATHLK','seamark:beacon_lateral:category':'CATLAM','seamark:buoy_lateral:category':'CATLAM',
  'seamark:light:category':'CATLIT','seamark:landmark:category':'CATLMK','seamark:military_area:category':'CATMPA',
  'seamark:platform:category':'CATOFP','seamark:pipeline_area:category':'CATPIP','seamark:pipeline_overhead:category':'CATPIP',
  'seamark:pipeline_submarine:category':'CATPIP','seamark:restricted_area:category':'CATREA','seamark:radio_station:category':'CATROS',
  'seamark:rescue_station:category':'CATRSC','seamark:small_craft_facility:category':'CATSCF','seamark:signal_station_traffic:category':'CATSIT',
  'seamark:signal_station_warning:category':'CATSIW','seamark:beacon_special_purpose:category':'CATSPM','seamark:buoy_special_purpose:category':'CATSPM',
  'seamark:daymark:category':'CATSPM','seamark:vegetation:category':'CATVEG',
  },

  // StringList Translation Rules:
  // Some features can have a list of values, comma separated
  one2oneStringList : [
    // CATACH - Category of anchorage
    ['CATACH','1','CATACH','unrestricted'], // unrestricted anchorage
    ['CATACH','2','CATACH','deep_water'], // deep water anchorage
    ['CATACH','3','CATACH','tanker'], // tanker anchorage
    ['CATACH','4','CATACH','explosives'], // explosives anchorage
    ['CATACH','5','CATACH','quarantine'], // quarantine anchorage
    ['CATACH','6','CATACH','sea-plane'], // sea-plane anchorage
    ['CATACH','7','CATACH','small_craft'], // small craft anchorage
    ['CATACH','8','CATACH','small_craft_mooring'], // small craft mooring area
    ['CATACH','9','CATACH','24_hour'], // anchorage for periods up to 24 hours
    ['CATACH','10','CATACH','limited_period'], // anchorage for a limited period of time

    // CATAIR - Category of airport/airfield
    ['CATAIR','1','CATAIR','military_airport'], // military aeroplane airport
    ['CATAIR','2','CATAIR','civil_airport'], // civil aeroplane airport
    ['CATAIR','3','CATAIR','military_heliport'], // military heliport
    ['CATAIR','4','CATAIR','civil_heliport'], // civil heliport
    ['CATAIR','5','CATAIR','glider_airfield'], // glider airfield
    ['CATAIR','6','CATAIR','small_plane_airfield'], // small planes airfield
    // ['CATAIR','7','CATAIR','helicopter_platform'], // helicopter platform
    ['CATAIR','8','CATAIR','emergency_airfield'], // emergency airfield

    // CATBRG - Category of bridge
    ['CATBRG','1','CATBRG','fixed'], // fixed bridge
    ['CATBRG','2','CATBRG','opening'], // opening bridge
    ['CATBRG','3','CATBRG','swing'], // swing bridge
    ['CATBRG','4','CATBRG','lifting'], // lifting bridge
    ['CATBRG','5','CATBRG','bascule'], // bascule bridge
    ['CATBRG','6','CATBRG','pontoon'], // pontoon bridge
    ['CATBRG','7','CATBRG','draw'], // draw bridge
    ['CATBRG','8','CATBRG','transporter'], // transporter bridge
    ['CATBRG','9','CATBRG','footbridge'], // footbridge
    ['CATBRG','10','CATBRG','viaduct'], // viaduct
    ['CATBRG','11','CATBRG','aqueduct'], // aqueduct
    ['CATBRG','12','CATBRG','suspension'], // suspension bridge

    // CATDPG - Category of dumping ground
    ['CATDPG','1','CATDPG','general'], // general dumping ground
    ['CATDPG','2','CATDPG','chemical'], // chemical waste dumping ground
    ['CATDPG','3','CATDPG','nuclear'], // nuclear waste dumping ground
    ['CATDPG','4','CATDPG','explosives'], // explosives dumping ground
    ['CATDPG','5','CATDPG','spoil'], // spoil ground
    ['CATDPG','6','CATDPG','vessel'], // vessel dumping ground

    // CATHAF - Category of harbour facility
    ['CATHAF','1','CATHAF','roro'], // RoRo-terminal
    // ['CATHAF','2','CATHAF','timber'], // timber yard
    ['CATHAF','3','CATHAF','ferry'], // ferry terminal
    ['CATHAF','4','CATHAF','fishing'], // fishing harbour
    ['CATHAF','5','CATHAF','marina'], // yacht harbour/marina
    ['CATHAF','6','CATHAF','naval'], // naval base
    ['CATHAF','7','CATHAF','tanker'], // tanker terminal
    ['CATHAF','8','CATHAF','passenger'], // passenger terminal
    ['CATHAF','9','CATHAF','shipyard'], // shipyard
    ['CATHAF','10','CATHAF','container'], // container terminal
    ['CATHAF','11','CATHAF','bulk'], // bulk terminal
    ['CATHAF','12','CATHAF','syncrolift'], // syncrolift
    ['CATHAF','13','CATHAF','straddle_carrier'], // straddle carrier

    // CATHLK - Category of hulk
    ['CATHLK','1','CATHLK','floating_restaurant'], // floating restaurant
    ['CATHLK','2','CATHLK','historic'], // historic ship
    ['CATHLK','3','CATHLK','museum'], // museum
    ['CATHLK','4','CATHLK','accommodation'], // accommodation
    ['CATHLK','5','CATHLK','floating_breakwater'], // floating breakwater

    // CATLIT - Category of light
    ['CATLIT','1','CATLIT','directional'], // directional function
    ['CATLIT','2','CATLIT','rear_upper'], // rear/upper light
    ['CATLIT','3','CATLIT','front_lower'], // front/lower light
    ['CATLIT','4','CATLIT','leading'], // leading light
    ['CATLIT','5','CATLIT','aero'], // aero light
    ['CATLIT','6','CATLIT','air_obstruction'], // air obstruction light
    ['CATLIT','7','CATLIT','fog_detector'], // fog detector light
    ['CATLIT','8','CATLIT','floodlight'], // flood light
    ['CATLIT','9','CATLIT','strip_light'], // strip light
    ['CATLIT','10','CATLIT','subsidiary'], // subsidiary light
    ['CATLIT','11','CATLIT','spotlight'], // spotlight
    ['CATLIT','12','CATLIT','front'], // front
    ['CATLIT','13','CATLIT','rear'], // rear
    ['CATLIT','14','CATLIT','lower'], // lower
    ['CATLIT','15','CATLIT','upper'], // upper
    ['CATLIT','16','CATLIT','moire'], // moiré effect
    ['CATLIT','17','CATLIT','emergency'], // emergency
    ['CATLIT','18','CATLIT','bearing'], // bearing light
    ['CATLIT','19','CATLIT','horizontal'], // horizontally disposed
    ['CATLIT','20','CATLIT','vertical'], // vertically disposed

    // CATLMK - Category of landmark
    ['CATLMK','1','CATLMK','cairn'], // cairn
    ['CATLMK','2','CATLMK','cemetery'], // cemetery
    ['CATLMK','3','CATLMK','chimney'], // chimney
    ['CATLMK','4','CATLMK','dish_aerial'], // dish aerial
    ['CATLMK','5','CATLMK','flagstaff'], // flagstaff (flagpole)
    ['CATLMK','6','CATLMK','flare_stack'], // flare stack
    ['CATLMK','7','CATLMK','mast'], // mast
    ['CATLMK','8','CATLMK','windsock'], // windsock
    ['CATLMK','9','CATLMK','monument'], // monument
    ['CATLMK','10','CATLMK','column'], // column (pillar)
    ['CATLMK','11','CATLMK','memorial'], // memorial plaque
    ['CATLMK','12','CATLMK','obelisk'], // obelisk
    ['CATLMK','13','CATLMK','statue'], // statue
    ['CATLMK','14','CATLMK','cross'], // cross
    ['CATLMK','15','CATLMK','dome'], // dome
    ['CATLMK','16','CATLMK','radar_scanner'], // radar scanner
    ['CATLMK','17','CATLMK','tower'], // tower
    ['CATLMK','18','CATLMK','windmill'], // windmill
    ['CATLMK','19','CATLMK','windmotor'], // windmotor
    ['CATLMK','20','CATLMK','spire'], // spire/minaret
    ['CATLMK','21','CATLMK','boulder'], // large rock or boulder on land

    // CATLND - Category of land region
    ['CATLND','1','seamark:land_region','fen'], // fen
    ['CATLND','2','seamark:land_region','marsh'], // marsh
    ['CATLND','3','seamark:land_region','moor/bog'], // moor/bog
    ['CATLND','4','seamark:land_region','heathland'], // heathland
    ['CATLND','5','seamark:land_region','mountain_range'], // mountain range
    ['CATLND','6','seamark:land_region','lowlands'], // lowlands
    ['CATLND','7','seamark:land_region','canyon_lands'], // canyon lands
    ['CATLND','8','seamark:land_region','paddy_field'], // paddy field
    ['CATLND','9','seamark:land_region','agricultural_land'], // agricultural land
    ['CATLND','10','seamark:land_region','savanna/grassland'], // savanna/grassland
    ['CATLND','11','seamark:land_region','parkland'], // parkland
    ['CATLND','12','seamark:land_region','swamp'], // swamp
    ['CATLND','13','seamark:land_region','landslide'], // landslide
    ['CATLND','14','seamark:land_region','lava_flow'], // lava flow
    ['CATLND','15','seamark:land_region','salt_pan'], // salt pan
    ['CATLND','16','seamark:land_region','moraine'], // moraine
    ['CATLND','17','seamark:land_region','crater'], // crater
    ['CATLND','18','seamark:land_region','cave'], // cave
    ['CATLND','19','seamark:land_region','rock_column_or_pinnacle'], // rock column or pinnacle
    ['CATLND','20','seamark:land_region','cay'], // cay

    // CATMPA - Category of military practice area
    ['CATMPA','1','CATMPA','general'], // practice area in general
    ['CATMPA','2','CATMPA','torpedo'], // torpedo exercise area
    ['CATMPA','3','CATMPA','submarine'], // submarine exercise area
    ['CATMPA','4','CATMPA','firing'], // firing danger area
    ['CATMPA','5','CATMPA','mine-laying'], // mine-laying practice area
    ['CATMPA','6','CATMPA','small_arms'], // small arms firing range

    // CATOFP - Category of offshore platform
    ['CATOFP','1','CATOFP','oil'], // oil derrick/rig
    ['CATOFP','2','CATOFP','production'], // production platform
    ['CATOFP','3','CATOFP','observation'], // observation/research platform
    ['CATOFP','4','CATOFP','alp'], // articulated loading platform (ALP)
    ['CATOFP','5','CATOFP','salm'], // single anchor leg mooring (SALM)
    ['CATOFP','6','CATOFP','mooring'], // mooring tower
    ['CATOFP','7','CATOFP','artificial_island'], // artificial island
    ['CATOFP','8','CATOFP','fpso'], // floating production, storage and off-loading vessel (FPSO)
    ['CATOFP','9','CATOFP','accommodation'], // accommodation platform
    ['CATOFP','10','CATOFP','nccb'], // navigation, communication and control buoy (NCCB)

    // CATPIP - Category of pipeline/pipe
    ['CATPIP','1','CATPIP','general'], // pipeline in general
    ['CATPIP','2','CATPIP','outfall'], // outfall pipe
    ['CATPIP','3','CATPIP','intake'], // intake pipe
    ['CATPIP','4','CATPIP','sewer'], // sewer
    ['CATPIP','5','CATPIP','bubbler'], // bubbler system
    ['CATPIP','6','CATPIP','supply'], // supply pipe

    // CATREA - Category of restricted area
    ['CATREA','1','CATREA','safety'], // offshore safety zone
    ['CATREA','2','CATREA','anchoring_prohibition'], // anchoring prohibition area
    ['CATREA','3','CATREA','fishing_prohibition'], // fishing prohibition area
    ['CATREA','4','CATREA','nature_reserve'], // nature reserve
    ['CATREA','5','CATREA','bird_sanctuary'], // bird sanctuary
    ['CATREA','6','CATREA','game_reserve'], // game reserve
    ['CATREA','7','CATREA','seal_sanctuary'], // seal sanctuary
    ['CATREA','8','CATREA','degaussing_range'], // degaussing range
    ['CATREA','9','CATREA','military'], // military area
    ['CATREA','10','CATREA','historic_wreck'], // historic wreck area
    ['CATREA','11','CATREA','inshore_traffic_zone'], // inshore traffic zone
    ['CATREA','12','CATREA','navigational_aid_safety'], // navigational aid safety zone
    ['CATREA','13','CATREA','danger_of_stranding'], // danger of stranding area
    ['CATREA','14','CATREA','minefield'], // minefield
    ['CATREA','15','CATREA','diving_prohibition'], // diving prohibition area
    ['CATREA','16','CATREA','area_to_be_avoided'], // area to be avoided
    ['CATREA','17','CATREA','prohibited_area'], // Prohibited area
    ['CATREA','18','CATREA','swimming_area'], // swimming area
    ['CATREA','19','CATREA','waiting'], // waiting area
    ['CATREA','20','CATREA','research'], // research area
    ['CATREA','21','CATREA','dredging'], // dredging area
    ['CATREA','22','CATREA','fish_sanctuary'], // fish sanctuary
    ['CATREA','23','CATREA','ecological_reserve'], // ecological reserve
    ['CATREA','24','CATREA','no_wake'], // no wake area
    ['CATREA','25','CATREA','swinging'], // swinging area
    ['CATREA','26','CATREA','water_skiing'], // water skiing area
    ['CATREA','27','CATREA','environmentally_sensitive_sea_area_(essa)'], // Environmentally Sensitive Sea Area (ESSA)
    ['CATREA','28','CATREA','particularly_sensitive_sea_area_(pssa)'], // Particularly Sensitive Sea Area (PSSA)

    // CATROS - Category of radio station
    ['CATROS','1','CATROS','omnidirectiona'], // circular (non-directional) marine or aero-marine radiobeacon
    ['CATROS','2','CATROS','directional'], // directional radiobeacon
    ['CATROS','3','CATROS','rotating-pattern'], // rotating-pattern radiobeacon
    ['CATROS','4','CATROS','consol'], // Consol beacon
    ['CATROS','5','CATROS','rdf'], // radio direction-finding station
    ['CATROS','6','CATROS','qtg'], // coast radio station providing QTG service
    ['CATROS','7','CATROS','aeronautical'], // aeronautical radiobeacon
    ['CATROS','8','CATROS','decca'], // Decca
    ['CATROS','9','CATROS','loran'], // Loran C
    ['CATROS','10','CATROS','dgps'], // Differential GPS
    ['CATROS','11','CATROS','toran'], // Toran
    ['CATROS','12','CATROS','omega'], // Omega
    ['CATROS','13','CATROS','syledis'], // Syledis
    ['CATROS','14','CATROS','chaika'], // Chaika (Chayka)

    // CATRSC - Category of rescue station
    ['CATRSC','1','CATRSC','lifeboat'], // rescue station with lifeboat
    ['CATRSC','2','CATRSC','rocket'], // rescue station with rocket
    ['CATRSC','3','CATRSC','lifeboat_and_rocket'], // rescue station with lifeboat and rocket
    ['CATRSC','4','CATRSC','refuge_shipwrecked'], // refuge for shipwrecked mariners
    ['CATRSC','5','CATRSC','refuge_intertidal'], // refuge for intertidal area walkers
    ['CATRSC','6','CATRSC','lifeboat_on_mooring'], // lifeboat lying at a mooring
    ['CATRSC','7','CATRSC','radio'], // aid radio station
    ['CATRSC','8','CATRSC','first_aid'], // first aid equipment

    // CATSCF - Category of small craft facility
    ['CATSCF','1','CATSCF','visitor_berth'], // visitor=s berth
    ['CATSCF','2','CATSCF','nautical_club'], // nautical club
    ['CATSCF','3','CATSCF','boat_hoist'], // boat hoist
    ['CATSCF','4','CATSCF','sailmaker'], // sailmaker
    ['CATSCF','5','CATSCF','boatyard'], // boatyard
    ['CATSCF','6','CATSCF','public_inn'], // public inn
    ['CATSCF','7','CATSCF','restaurant'], // restaurant
    ['CATSCF','8','CATSCF','chandler'], // chandler
    ['CATSCF','9','CATSCF','provisions'], // provisions
    ['CATSCF','10','CATSCF','doctor'], // doctor
    ['CATSCF','11','CATSCF','pharmacy'], // pharmacy
    ['CATSCF','12','CATSCF','water_tap'], // water tap
    ['CATSCF','13','CATSCF','fuel_station'], // fuel station
    ['CATSCF','14','CATSCF','electricity'], // electricity
    ['CATSCF','15','CATSCF','bottle_gas'], // bottle gas
    ['CATSCF','16','CATSCF','showers'], // showers
    ['CATSCF','17','CATSCF','launderette'], // launderette
    ['CATSCF','18','CATSCF','toilets'], // public toilets
    ['CATSCF','19','CATSCF','post_box'], // post box
    ['CATSCF','20','CATSCF','telephone'], // public telephone
    ['CATSCF','21','CATSCF','refuse_bin'], // refuse bin
    ['CATSCF','22','CATSCF','car_park'], // car park
    ['CATSCF','23','CATSCF','boats_trailer_park'], // parking for boats and trailers
    ['CATSCF','24','CATSCF','caravan_site'], // caravan site
    ['CATSCF','25','CATSCF','camping_site'], // camping site
    ['CATSCF','26','CATSCF','pump-out'], // sewerage pump-out station
    ['CATSCF','27','CATSCF','emergency_telephone'], // emergency telephone
    ['CATSCF','28','CATSCF','slipway'], // landing/launching place for boats
    ['CATSCF','29','CATSCF','visitors_mooring'], // visitors mooring
    ['CATSCF','30','CATSCF','scrubbing_berth'], // scrubbing berth
    ['CATSCF','31','CATSCF','picnic_area'], // picnic area
    ['CATSCF','32','CATSCF','mechanics_workshop'], // mechanics workshop
    ['CATSCF','33','CATSCF','security_service'], // guard and/or security service

    // CATSIT - Category of signal station, traffic
    ['CATSIT','1','CATSIT','port_control'], // port control
    ['CATSIT','2','CATSIT','port_entry_departure'], // port entry and departure
    ['CATSIT','3','CATSIT','ipt'], // International Port Traffic
    ['CATSIT','4','CATSIT','berthing'], // berthing
    ['CATSIT','5','CATSIT','dock'], // dock
    ['CATSIT','6','CATSIT','lock'], // lock
    ['CATSIT','7','CATSIT','flood_barrage'], // flood barrage
    ['CATSIT','8','CATSIT','bridge_passage'], // bridge passage
    ['CATSIT','9','CATSIT','dredging'], // dredging
    ['CATSIT','10','CATSIT','traffic_control'], // traffic control light

    // CATSIW - Category of signal station, warning
    ['CATSIW','1','CATSIW','danger'], // danger
    ['CATSIW','2','CATSIW','maritime_obstruction'], // maritime obstruction
    ['CATSIW','3','CATSIW','cable'], // cable
    ['CATSIW','4','CATSIW','military'], // military practice
    ['CATSIW','5','CATSIW','distress'], // distress
    ['CATSIW','6','CATSIW','weather'], // weather
    ['CATSIW','7','CATSIW','storm'], // storm
    ['CATSIW','8','CATSIW','ice'], // ice
    ['CATSIW','9','CATSIW','time'], // time
    ['CATSIW','10','CATSIW','tide'], // tide
    ['CATSIW','11','CATSIW','tidal_stream'], // tidal stream
    ['CATSIW','12','CATSIW','tide_gauge'], // tide gauge
    ['CATSIW','13','CATSIW','tide_scale'], // tide scale
    ['CATSIW','14','CATSIW','diving'], // diving
    ['CATSIW','15','CATSIW','water_level_gauge'], // water level gauge

    // CATSPM - Category of special purpose mark
    ['CATSPM','1','CATSPM','firing_danger_area'], // firing danger area mark
    ['CATSPM','2','CATSPM','target'], // target mark
    ['CATSPM','3','CATSPM','marker_ship'], // marker ship mark
    ['CATSPM','4','CATSPM','degaussing_range'], // degaussing range mark
    ['CATSPM','5','CATSPM','barge'], // barge mark
    ['CATSPM','6','CATSPM','cable'], // cable mark
    ['CATSPM','7','CATSPM','spoil_ground'], // spoil ground mark
    ['CATSPM','8','CATSPM','outfall'], // outfall mark
    ['CATSPM','9','CATSPM','odas'], // ODAS (Ocean-Data-Acquisition-System)
    ['CATSPM','10','CATSPM','recording'], // recording mark
    ['CATSPM','11','CATSPM','seaplane_anchorage'], // seaplane anchorage mark
    ['CATSPM','12','CATSPM','recreation_zone'], // recreation zone mark
    ['CATSPM','13','CATSPM','private'], // private mark
    ['CATSPM','14','CATSPM','mooring'], // mooring mark
    ['CATSPM','15','CATSPM','lanby'], // LANBY (Large Automatic Navigational Buoy)
    ['CATSPM','16','CATSPM','leading'], // leading mark
    ['CATSPM','17','CATSPM','measured_distance'], // measured distance mark
    ['CATSPM','18','CATSPM','notice'], // notice mark
    ['CATSPM','19','CATSPM','tss'], // TSS mark (Traffic Separation Scheme)
    ['CATSPM','20','CATSPM','no_anchoring'], // anchoring prohibited mark
    ['CATSPM','21','CATSPM','no_berthing'], // berthing prohibited mark
    ['CATSPM','22','CATSPM','no_overtaking'], // overtaking prohibited mark
    ['CATSPM','23','CATSPM','no_two-way_traffic'], // two-way traffic prohibited mark
    ['CATSPM','24','CATSPM','reduced_wake'], // >reduced wake= mark
    ['CATSPM','25','CATSPM','speed_limit'], // speed limit mark
    ['CATSPM','26','CATSPM','stop'], // stop mark
    ['CATSPM','27','CATSPM','general_warning'], // general warning mark
    ['CATSPM','28','CATSPM','sound_ship_siren'], // >sound ship=s siren= mark
    ['CATSPM','29','CATSPM','restricted_vertical_clearance'], // restricted vertical clearance mark
    ['CATSPM','30','CATSPM','maximum_vessel_draught'], // maximum vessel=s draught mark
    ['CATSPM','31','CATSPM','restricted_horizontal_clearance'], // restricted horizontal clearance mark
    ['CATSPM','32','CATSPM','strong_current'], // strong current warning mark
    ['CATSPM','33','CATSPM','berthing'], // berthing permitted mark
    ['CATSPM','34','CATSPM','overhead_power_cable'], // overhead power cable mark
    ['CATSPM','35','CATSPM','channel_edge_gradient'], // >channel edge gradient= mark
    ['CATSPM','36','CATSPM','telephone'], // telephone mark
    ['CATSPM','37','CATSPM','ferry_crossing'], // ferry crossing mark
    ['CATSPM','38','CATSPM','marine_traffic_lights'], // marine traffic lights
    ['CATSPM','39','CATSPM','pipeline'], // pipeline mark
    ['CATSPM','40','CATSPM','anchorage'], // anchorage mark
    ['CATSPM','41','CATSPM','clearing'], // clearing mark
    ['CATSPM','42','CATSPM','control'], // control mark
    ['CATSPM','43','CATSPM','diving'], // diving mark
    ['CATSPM','44','CATSPM','refuge_beacon'], // refuge beacon
    ['CATSPM','45','CATSPM','foul_ground'], // foul ground mark
    ['CATSPM','46','CATSPM','yachting'], // yachting mark
    ['CATSPM','47','CATSPM','heliport'], // heliport mark
    ['CATSPM','48','CATSPM','gps'], // GPS mark
    ['CATSPM','49','CATSPM','seaplane_landing'], // seaplane landing mark
    ['CATSPM','50','CATSPM','no_entry'], // entry prohibited mark
    ['CATSPM','51','CATSPM','work_in_progress'], // work in progress mark
    ['CATSPM','52','CATSPM','unknown_purpose'], // mark with unknown purpose
    ['CATSPM','53','CATSPM','wellhead'], // wellhead mark
    ['CATSPM','54','CATSPM','channel_separation'], // channel separation mark
    ['CATSPM','55','CATSPM','marine_farm'], // marine farm mark
    ['CATSPM','56','CATSPM','artificial_reef'], // artificial reef mark

    // CATVEG - Category of vegetation
    ['CATVEG','1','CATVEG','grass'], // grass
    ['CATVEG','2','CATVEG','paddy_field'], // paddy field
    ['CATVEG','3','CATVEG','bush'], // bush
    ['CATVEG','4','CATVEG','deciduous_wood'], // deciduous wood
    ['CATVEG','5','CATVEG','coniferous_wood'], // coniferous wood
    ['CATVEG','6','CATVEG','wood'], // wood in general (inc mixed wood)
    ['CATVEG','7','CATVEG','mangroves'], // mangroves
    ['CATVEG','8','CATVEG','park'], // park
    ['CATVEG','9','CATVEG','parkland'], // parkland
    ['CATVEG','10','CATVEG','mixed_crops'], // mixed crops
    ['CATVEG','11','CATVEG','reed'], // reed
    ['CATVEG','12','CATVEG','moss'], // moss
    ['CATVEG','13','CATVEG','tree'], // tree in general
    ['CATVEG','14','CATVEG','evergreen_tree'], // evergreen tree
    ['CATVEG','15','CATVEG','coniferous_tree'], // coniferous tree
    ['CATVEG','16','CATVEG','palm_tree'], // palm tree
    ['CATVEG','17','CATVEG','nipa_palm'], // nipa palm tree
    ['CATVEG','18','CATVEG','casuarina_tree'], // casuarina tree
    ['CATVEG','19','CATVEG','eucalypt_tree'], // eucalypt tree
    ['CATVEG','20','CATVEG','deciduous_tree'], // deciduous tree
    ['CATVEG','21','CATVEG','mangrove_tree'], // mangrove tree
    ['CATVEG','22','CATVEG','filao_tree'], // filao tree

    // COLOUR - Colour
    ['COLOUR','1','COLOUR','white'], // white
    ['COLOUR','2','COLOUR','black'], // black
    ['COLOUR','3','COLOUR','red'], // red
    ['COLOUR','4','COLOUR','green'], // green
    ['COLOUR','5','COLOUR','blue'], // blue
    ['COLOUR','6','COLOUR','yellow'], // yellow
    ['COLOUR','7','COLOUR','grey'], // grey
    ['COLOUR','8','COLOUR','brown'], // brown
    ['COLOUR','9','COLOUR','amber'], // amber
    ['COLOUR','10','COLOUR','violet'], // violet
    ['COLOUR','11','COLOUR','orange'], // orange
    ['COLOUR','12','COLOUR','magenta'], // magenta
    ['COLOUR','13','COLOUR','pink'], // pink

    // COLPAT - Colour pattern
    ['COLPAT','1','COLPAT','horizontal'], // horizontal stripes
    ['COLPAT','2','COLPAT','vertical'], // vertical stripes
    ['COLPAT','3','COLPAT','diagonal'], // diagonal stripes
    ['COLPAT','4','COLPAT','squared'], // squared
    ['COLPAT','5','COLPAT','stripes'], // stripes (direction unknown)
    ['COLPAT','6','COLPAT','border'], // border stripe

    // FUNCTN - Function
    ['FUNCTN','1','FUNCTN','none'], // no function/service of major interest
    ['FUNCTN','2','FUNCTN','harbour_master'], // harbour-master=s office
    ['FUNCTN','3','FUNCTN','custom'], // custom office
    ['FUNCTN','4','FUNCTN','health'], // health office
    ['FUNCTN','5','FUNCTN','hospital'], // hospital
    ['FUNCTN','6','FUNCTN','post_office'], // post office
    ['FUNCTN','7','FUNCTN','hotel'], // hotel
    ['FUNCTN','8','FUNCTN','railway_station'], // railway station
    ['FUNCTN','9','FUNCTN','police_station'], // police station
    ['FUNCTN','10','FUNCTN','water-police_station'], // water-police station
    ['FUNCTN','11','FUNCTN','pilot_office'], // pilot office
    ['FUNCTN','12','FUNCTN','pilot_lookout'], // pilot lookout
    ['FUNCTN','13','FUNCTN','bank'], // bank office
    ['FUNCTN','14','FUNCTN','district_control'], // headquarters for district control
    ['FUNCTN','15','FUNCTN','transit_shed'], // transit shed/warehouse
    ['FUNCTN','16','FUNCTN','factory'], // factory
    ['FUNCTN','17','FUNCTN','power_station'], // power station
    ['FUNCTN','18','FUNCTN','administrative'], // administrative
    ['FUNCTN','19','FUNCTN','educational'], // educational facility
    ['FUNCTN','20','FUNCTN','church'], // church
    ['FUNCTN','21','FUNCTN','chapel'], // chapel
    ['FUNCTN','22','FUNCTN','temple'], // temple
    ['FUNCTN','23','FUNCTN','pagoda'], // pagoda
    ['FUNCTN','24','FUNCTN','shinto_shrine'], // shinto shrine
    ['FUNCTN','25','FUNCTN','buddhist_temple'], // buddhist temple
    ['FUNCTN','26','FUNCTN','mosque'], // mosque
    ['FUNCTN','27','FUNCTN','marabout'], // marabout
    ['FUNCTN','28','FUNCTN','lookout'], // lookout
    ['FUNCTN','29','FUNCTN','communication'], // communication
    ['FUNCTN','30','FUNCTN','television'], // television
    ['FUNCTN','31','FUNCTN','radio'], // radio
    ['FUNCTN','32','FUNCTN','radar'], // radar
    ['FUNCTN','33','FUNCTN','light_support'], // light support
    ['FUNCTN','34','FUNCTN','microwave'], // microwave
    ['FUNCTN','35','FUNCTN','cooling'], // cooling
    ['FUNCTN','36','FUNCTN','observation'], // observation
    ['FUNCTN','37','FUNCTN','time_ball'], // timeball
    ['FUNCTN','38','FUNCTN','clock'], // clock
    ['FUNCTN','39','FUNCTN','control'], // control
    ['FUNCTN','40','FUNCTN','airship_mooring'], // airship mooring
    ['FUNCTN','41','FUNCTN','stadium'], // stadium
    ['FUNCTN','42','FUNCTN','bus_station'], // bus station

    // LITVIS - Light visibility
    ['LITVIS','1','LITVIS','high'], // high intensity
    ['LITVIS','2','LITVIS','low'], // low intensity
    ['LITVIS','3','LITVIS','faint'], // faint
    ['LITVIS','4','LITVIS','intensified'], // intensified
    ['LITVIS','5','LITVIS','unintensified'], // unintensified
    ['LITVIS','6','LITVIS','restricted'], // visibility deliberately restricted
    ['LITVIS','7','LITVIS','obscured'], // obscured
    ['LITVIS','8','LITVIS','part_obscured'], // partially obscured

    // NATCON - Nature of construction
    ['NATCON','1','NATCON','masonry'], // masonry
    ['NATCON','2','NATCON','concreted'], // concreted
    ['NATCON','3','NATCON','loose_boulders'], // loose boulders
    ['NATCON','4','NATCON','hard_surfaced'], // hard surfaced
    ['NATCON','5','NATCON','unsurfaced'], // unsurfaced
    ['NATCON','6','NATCON','wooden'], // wooden
    ['NATCON','7','NATCON','metal'], // metal
    ['NATCON','8','NATCON','grp'], // glass reinforced plastic (GRP)
    ['NATCON','9','NATCON','painted'], // painted

    // NATQUA - Nature of surface - qualifying terms
    ['NATQUA','1','NATQUA','fine'], // fine
    ['NATQUA','2','NATQUA','medium'], // medium
    ['NATQUA','3','NATQUA','coarse'], // coarse
    ['NATQUA','4','NATQUA','broken'], // broken
    ['NATQUA','5','NATQUA','sticky'], // sticky
    ['NATQUA','6','NATQUA','soft'], // soft
    ['NATQUA','7','NATQUA','stiff'], // stiff
    ['NATQUA','8','NATQUA','volcanic'], // volcanic
    ['NATQUA','9','NATQUA','calcareous'], // calcareous
    ['NATQUA','10','NATQUA','hard'], // hard

    // NATSUR - Nature of surface
    ['NATSUR','1','NATSUR','mud'], // mud
    ['NATSUR','2','NATSUR','clay'], // clay
    ['NATSUR','3','NATSUR','silt'], // silt
    ['NATSUR','4','NATSUR','sand'], // sand
    ['NATSUR','5','NATSUR','stone'], // stone
    ['NATSUR','6','NATSUR','gravel'], // gravel
    ['NATSUR','7','NATSUR','pebbles'], // pebbles
    ['NATSUR','8','NATSUR','cobbles'], // cobbles
    ['NATSUR','9','NATSUR','rock'], // rock
    ['NATSUR','10','NATSUR','marsh'], // marsh
    ['NATSUR','11','NATSUR','lava'], // lava
    ['NATSUR','12','NATSUR','snow'], // snow
    ['NATSUR','13','NATSUR','ice'], // ice
    ['NATSUR','14','NATSUR','coral'], // coral
    ['NATSUR','15','NATSUR','swamp'], // swamp
    ['NATSUR','16','NATSUR','bog/moor'], // bog/moor
    ['NATSUR','17','NATSUR','shells'], // shells
    ['NATSUR','18','NATSUR','boulder'], // boulder

    // PRODCT - Product
    ['PRODCT','1','PRODCT','oil'], // oil
    ['PRODCT','2','PRODCT','gas'], // gas
    ['PRODCT','3','PRODCT','water'], // water
    ['PRODCT','4','PRODCT','stone'], // stone
    ['PRODCT','5','PRODCT','coal'], // coal
    ['PRODCT','6','PRODCT','ore'], // ore
    ['PRODCT','7','PRODCT','chemicals'], // chemicals
    ['PRODCT','8','PRODCT','drinking_water'], // drinking water
    ['PRODCT','9','PRODCT','milk'], // milk
    ['PRODCT','10','PRODCT','bauxite'], // bauxite
    ['PRODCT','11','PRODCT','coke'], // coke
    ['PRODCT','12','PRODCT','iron_ingots'], // iron ingots
    ['PRODCT','13','PRODCT','salt'], // salt
    ['PRODCT','14','PRODCT','sand'], // sand
    ['PRODCT','15','PRODCT','timber'], // timber
    ['PRODCT','16','PRODCT','sawdust'], // sawdust/wood chips
    ['PRODCT','17','PRODCT','scrap_metal'], // scrap metal
    ['PRODCT','18','PRODCT','lng'], // liquified natural gas (LNG)
    ['PRODCT','19','PRODCT','lpg'], // liquified petroleum gas (LPG)
    ['PRODCT','20','PRODCT','wine'], // wine
    ['PRODCT','21','PRODCT','cement'], // cement
    ['PRODCT','22','PRODCT','grain'], // grain

    // QUASOU - Quality of sounding measurement
    ['QUASOU','1','QUASOU','depth_known'], // depth known
    ['QUASOU','2','QUASOU','depth_unknown'], // depth unknown
    ['QUASOU','3','QUASOU','doubtful_sounding'], // doubtful sounding
    ['QUASOU','4','QUASOU','unreliable_sounding'], // unreliable sounding
    ['QUASOU','5','QUASOU','no_bottom_found_at_value_shown'], // no bottom found at value shown
    ['QUASOU','6','QUASOU','least_depth_known'], // least depth known
    ['QUASOU','7','QUASOU','least_depth_unknown_safe_clearance_at_value_shown'], // least depth unknown, safe clearance at value shown
    ['QUASOU','8','QUASOU','value_reported_(not_surveyed)'], // value reported (not surveyed)
    ['QUASOU','9','QUASOU','value_reported_(not_confirmed)'], // value reported (not confirmed)
    ['QUASOU','10','QUASOU','maintained_depth'], // maintained depth
    ['QUASOU','11','QUASOU','not_regularly_maintained'], // not regularly maintained

    // RESTRN - Restriction
    ['RESTRN','1','RESTRN','no_anchoring'], // anchoring prohibited
    ['RESTRN','2','RESTRN','restricted_anchoring'], // anchoring restricted
    ['RESTRN','3','RESTRN','no_fishing'], // fishing prohibited
    ['RESTRN','4','RESTRN','restricted_fishing'], // fishing restricted
    ['RESTRN','5','RESTRN','no_trawling'], // trawling prohibited
    ['RESTRN','6','RESTRN','restricted_trawling'], // trawling restricted
    ['RESTRN','7','RESTRN','no_entry'], // entry prohibited
    ['RESTRN','8','RESTRN','restricted_entry'], // entry restricted
    ['RESTRN','9','RESTRN','no_dredging'], // dredging prohibited
    ['RESTRN','10','RESTRN','restricted_dredging'], // dredging restricted
    ['RESTRN','11','RESTRN','no_diving'], // diving prohibited
    ['RESTRN','12','RESTRN','restricted_diving'], // diving restricted
    ['RESTRN','13','RESTRN','no_wake'], // no wake
    ['RESTRN','14','RESTRN','to_be_avoided'], // area to be avoided
    ['RESTRN','15','RESTRN','no_construction'], // construction prohibited
    ['RESTRN','16','RESTRN','no_discharging'], // discharging prohibited
    ['RESTRN','17','RESTRN','restricted_discharging'], // discharging restricted
    ['RESTRN','18','RESTRN','no_exploration_development'], // industrial or mineral exploration/development prohibited
    ['RESTRN','19','RESTRN','restricted_exploration_development'], // industrial or mineral exploration/development restricted
    ['RESTRN','20','RESTRN','no_drilling'], // drilling prohibited
    ['RESTRN','21','RESTRN','restricted_drilling'], // drilling restricted
    ['RESTRN','22','RESTRN','no_historical_artifacts_removal'], // removal of historical artifacts prohibited
    ['RESTRN','23','RESTRN','no_lightering'], // cargo transhipment (lightering) prohibited
    ['RESTRN','24','RESTRN','no_dragging'], // dragging prohibited
    ['RESTRN','25','RESTRN','no_stopping'], // stopping prohibited
    ['RESTRN','26','RESTRN','no_landing'], // landing prohibited
    ['RESTRN','27','RESTRN','restricted_speed'], // speed restricted

    // SURTYP - Survey type
    ['SURTYP','1','source:survey_type','reconnaissance/sketch'], // reconnaissance/sketch survey
    ['SURTYP','2','source:survey_type','controlled'], // controlled survey
    // ['SURTYP','3','source:survey_type','unsurveyed'], // unsurveyed
    ['SURTYP','4','source:survey_type','examination'], // examination survey
    ['SURTYP','5','source:survey_type','passage'], // passage survey
    ['SURTYP','6','source:survey_type','remotely_sensed'], // remotely sensed

    // STATUS - Status
    ['STATUS','1','STATUS','permanent'], // permanent
    ['STATUS','2','STATUS','occasional'], // occasional
    ['STATUS','3','STATUS','recommended'], // recommended
    ['STATUS','4','STATUS','not_in_use'], // not in use
    ['STATUS','5','STATUS','intermittent'], // periodic/intermittent
    ['STATUS','6','STATUS','reserved'], // reserved
    ['STATUS','7','STATUS','temporary'], // temporary
    ['STATUS','8','STATUS','private'], // private
    ['STATUS','9','STATUS','mandatory'], // mandatory
    ['STATUS','10','STATUS','destroyed'], // destroyed/ruined
    ['STATUS','11','STATUS','extinguished'], // extinguished
    ['STATUS','12','STATUS','illuminated'], // illuminated
    ['STATUS','13','STATUS','historic'], // historic
    ['STATUS','14','STATUS','public'], // public
    ['STATUS','15','STATUS','synchronized'], // synchronized
    ['STATUS','16','STATUS','watched'], // watched
    ['STATUS','17','STATUS','unwatched'], // un-watched
    ['STATUS','18','STATUS','existence_doubtful'], // existence doubtful

    // TECSOU - Technique of sounding measurement
    ['TECSOU','1','TECSOU','echo_sounder'], // found by echo-sounder
    ['TECSOU','2','TECSOU','side-scan_sonar'], // found by side scan sonar
    ['TECSOU','3','TECSOU','multi-beam_echo_sounder'], // found by multi-beam
    ['TECSOU','4','TECSOU','diver'], // found by diver
    ['TECSOU','5','TECSOU','lead-line'], // found by lead-line
    ['TECSOU','6','TECSOU','area_swept_by_wire-drag'], // swept by wire-drag
    ['TECSOU','7','TECSOU','laser'], // found by laser
    ['TECSOU','8','TECSOU','area_swept_by_vertical_acoustic_system'], // swept by vertical acoustic system
    ['TECSOU','9','TECSOU','electromagnetic_sensor'], // found by electromagnetic sensor
    ['TECSOU','10','TECSOU','photogrammetry'], // photogrammetry
    ['TECSOU','11','TECSOU','satellite_imagery'], // satellite imagery
    ['TECSOU','12','TECSOU','levelling'], // found by levelling
    ['TECSOU','13','TECSOU','area_swept_by_side-scan_sonar'], // swept by side-scan sonar
    ['TECSOU','14','TECSOU','computer_generated'], // computer generated
  ], // End one2oneStringList

  // extraLayerTagList: Geonames style If this then add these list
  extraLayerTagList : {
  'administration_area':{'boundary':'administrative'}, // ADMARE Administration area (Named)
  // 'aggregation':{'x':'x'}, // C_AGGR Aggregation
  'airport':{'aeroway':'aerodrome'}, // AIRARE Airport / airfield
  // 'anchor_berth':{'x':'x'}, // ACHBRT Anchor berth
  // 'anchorage':{'x':'x'}, // ACHARE Anchorage area
  // 'archipelagic_sea_lane':{'x':'x'}, // ARCSLN Archipelagic sea lane
  // 'archipelagic_sea_lane_axis':{'x':'x'}, // ASLXIS Archipelagic sea lane axis
  // 'association':{'x':'x'}, // C_ASSO Association
  // 'beacon_cardinal':{'x':'x'}, // BCNCAR Beacon cardinal
  // 'beacon_isolated_danger':{'x':'x'}, // BCNISD Beacon isolated danger
  // 'beacon_lateral':{'x':'x'}, // BCNLAT Beacon lateral
  // 'beacon_safe_water':{'x':'x'}, // BCNSAW Beacon safe water
  // 'beacon_special_purpose':{'x':'x'}, // BCNSPP Beacon special purpose/general
  // 'berth':{'x':'x'}, // BERTHS Berth
  'bridge':{'bridge':'yes'}, // BRIDGE Bridge
  'building':{'building':'yes'}, // BUISGL Building single
  'built_up_area':{'landuse':'built_up_area'}, // BUAARE Built-up area
  // 'buoy_cardinal':{'x':'x'}, // BOYCAR Buoy cardinal
  // 'buoy_installation':{'x':'x'}, // BOYINB Buoy installation
  // 'buoy_isolated_danger':{'x':'x'}, // BOYISD Buoy isolated danger
  // 'buoy_lateral':{'x':'x'}, // BOYLAT Buoy lateral
  // 'buoy_safe_water':{'x':'x'}, // BOYSAW Buoy safe water
  // 'buoy_special_purpose':{'x':'x'}, // BOYSPP Buoy special purpose/general
  // 'cable_area':{'x':'x'}, // CBLARE Cable area
  'cable_overhead':{'cable':'yes','location':'overhead'}, // CBLOHD Cable overhead
  'cable_submarine':{'cable':'yes','location':'underwater'}, // CBLSUB Cable submarine
  // 'calling_in_point':{'x':'x'}, // RDOCAL Radio calling-in point
  'canal':{'waterway':'canal'}, // CANALS Canal
  // 'canal_bank':{'x':'x'}, // CANBNK Canal bank
  // 'cargo_transhipment_area':{'x':'x'}, // CTSARE Cargo transshipment area
  'causeway':{'man_made':'causeway'}, // CAUSWY Causeway
  // 'caution_area':{'x':'x'}, // CTNARE Caution area
  // 'checkpoint':{'x':'x'}, // CHKPNT Checkpoint
  'coastguard_station':{'amenity':'coast_guard'}, // CGUSTA Coastguard station
  'coastline':{'natural':'coastline'}, // COALNE Coastline
  // 'compilation_scale':{'x':'x'}, // M_CSCL Compilation scale of data
  // 'contiguous_zone':{'x':'x'}, // CONZNE Contiguous zone
  // 'continental_shelf_area':{'x':'x'}, // COSARE Continental shelf area
  // 'control_point':{'x':'x'}, // CTRPNT Control point
  'conveyor':{'man_made':'goods_conveyor'}, // CONVYR Conveyor
  // 'coverage':{'x':'x'}, // M_COVR Coverage
  'crane':{'man_made':'crane'}, // CRANES Crane
  // 'custom_zone':{'x':'x'}, // CUSZNE Custom zone
  'dam':{'waterway':'dam'}, // DAMCON Dam
  // 'data_accuracy':{'x':'x'}, // M_ACCY Accuracy of data
  // 'data_quality':{'x':'x'}, // M_QUAL Quality of data
  // 'daymark':{'x':'x'}, // DAYMAR Daymark
  // 'deep_water_route_centerline':{'x':'x'}, // DWRTCL Deep water route centerline
  // 'deep_water_route_part':{'x':'x'}, // DWRTPT Deep water route part
  // 'depth_area':{'x':'x'}, // DEPARE Depth area
  'depth_contour':{'contour':'depth'}, // DEPCNT Depth contour
  // 'distance_mark':{'x':'x'}, // DISMAR Distance mark
  'dock':{'waterway':'dock'}, // DOCARE Dock area
  // 'dredged_area':{'x':'x'}, // DRGARE Dredged area
  'dry_dock':{'waterway':'dock','dock':'drydock'}, // DRYDOC Dry dock
  // 'dumping_ground':{'x':'x'}, // DMPGRD Dumping ground
  'dyke':{'man_made':'dyke'}, // DYKCON Dyke
  // 'exclusive_economic_zone':{'x':'x'}, // EXEZNE Exclusive Economic Zone
  // 'fairway':{'x':'x'}, // FAIRWY Fairway
  'ferry_route':{'route':'ferry'}, // FERYRT Ferry route
  // 'fishery_zone':{'x':'x'}, // FSHZNE Fishery zone
  // 'fishing_facility':{'x':'x'}, // FSHFAC Fishing facility
  // 'fishing_ground':{'x':'x'}, // FSHGRD Fishing ground
  'floating_dock':{'waterway':'dock','dock':'floating'}, // FLODOC Floating dock
  // 'fog_signal':{'x':'x'}, // FOGSIG Fog signal
  // 'fortified_structure':{'x':'x'}, // FORSTC Fortified structure
  // 'free_port_area':{'x':'x'}, // FRPARE Free port area
  'gate':{'barrier':'gate'}, // GATCON Gate
  // 'gridiron':{'x':'x'}, // GRIDRN Gridiron
  // 'harbour':{'x':'x'}, // HRBFAC Harbour facility
  'harbour_area':{'landuse':'harbour'}, // HRBARE Harbour area (administrative)
  // 'horizontal_datum_shift_parameters':{'x':'x'}, // M_HOPA Horizontal datum shift parameters
  // 'hulk':{'x':'x'}, // HULKES Hulk
  // 'ice_area':{'x':'x'}, // ICEARE Ice area
  // 'incineration_area':{'x':'x'}, // ICNARE Incineration area
  // 'inshore_traffic_zone':{'x':'x'}, // ISTZNE Inshore traffic zone
  'lake':{'water':'lake'}, // LAKARE Lake
  // 'lake_shore':{'x':'x'}, // LAKSHR Lake shore
  // 'land_area':{'x':'x'}, // LNDARE Land area
  // 'land_region':{'x':'x'}, // LNDRGN Land region
  // 'land_elevation':{'x':'x'}, // LNDELV Land elevation
  // 'landmark':{'x':'x'}, // LNDMRK Landmark
  // 'light':{'x':'x'}, // LIGHTS Light
  // 'light_float':{'x':'x'}, // LITFLT Light float
  // 'light_vessel':{'x':'x'}, // LITVES Light vessel
  'local_magnetic_anomaly':{'natural':'magnetic_anomaly'}, // LOCMAG Local magnetic anomaly
  // 'local_magnetic_anomaly':{'x':'x'}, // LOCMAG Local magnetic anomaly
  // 'lock_basin':{'x':'x'}, // LOKBSN Lock basin
  'log_pond':{'water':'log_pond'}, // LOGPON Log pond
  // 'magnetic_variation':{'x':'x'}, // MAGVAR Magnetic variation
  'marine_farm':{'landuse':'aquaculture'}, // MARCUL Marine farm/culture
  // 'military_area':{'x':'x'}, // MIPARE Military practice area
  // 'mooring':{'x':'x'}, // MORFAC Mooring/warping facility
  // 'nautical_publication':{'x':'x'}, // M_NPUB Nautical publication information
  // 'navigation_line':{'x':'x'}, // NAVLNE Navigation line
  // 'navigation_system_of_marks':{'x':'x'}, // M_NSYS Navigational system of marks
  // 'new_object':{'x':'x'}, // NEWOBJ New object
  // 'non_gravitational_current':{'x':'x'}, // CURENT Current - non - gravitational
  // 'obstruction':{'x':'x'}, // OBSTRN Obstruction
  // 'oil_barrier':{'x':'x'}, // OILBAR Oil barrier
  // 'pile':{'x':'x'}, // PILPNT Pile
  // 'pilot_boarding':{'x':'x'}, // PILBOP Pilot boarding place
  // 'pipeline_area':{'x':'x'}, // PIPARE Pipeline area
  // 'pipeline_overhead':{'x':'x'}, // PIPOHD Pipeline overhead
  // 'pipeline_submarine':{'x':'x'}, // PIPSOL Pipeline submarine/on land
  // 'platform':{'x':'x'}, // OFSPLF Offshore platform
  // 'pontoon':{'x':'x'}, // PONTON Pontoon
  // 'precautionary_area':{'x':'x'}, // PRCARE Precautionary area
  // 'production_area':{'x':'x'}, // OSPARE Offshore production area
  // 'production_storage_area':{'x':'x'}, // PRDARE Production / storage area
  // 'pylon':{'x':'x'}, // PYLONS Pylon/bridge support
  // 'radar_line':{'x':'x'}, // RADLNE Radar line
  // 'radar_range':{'x':'x'}, // RADRNG Radar range
  // 'radar_reflector':{'x':'x'}, // RADRFL Radar reflector
  // 'radar_station':{'x':'x'}, // RADSTA Radar station
  // 'radar_transponder':{'x':'x'}, // RTPBCN Radar transponder beacon
  // 'radio_station':{'x':'x'}, // RDOSTA Radio station
  'rail':{'railway':'rail'}, // RAILWY Railway
  'rapids':{'waterway':'rapids'}, // RAPIDS Rapids
  // 'reccomended_route_centerline':{'x':'x'}, // RCRTCL Recommended route centerline
  // 'reccomended_track':{'x':'x'}, // RECTRC Recommended track
  // 'reccomended_traffic_lane':{'x':'x'}, // RCTLPT Recommended Traffic Lane Part
  // 'rescue_station':{'x':'x'}, // RSCSTA Rescue station
  // 'restricted_area':{'x':'x'}, // RESARE Restricted area
  // 'retro_reflector':{'x':'x'}, // RETRFL Retro-reflector
  'river':{'waterway':'river'}, // RIVERS River
  // 'riverbank':{'waterway':'riverbank'}, // RIVBNK River bank
  'road':{'highway':'road'}, // ROADWY Road
  // 'rock':{'x':'x'}, // UWTROC Underwater rock / awash rock
  'runway':{'aeroway':'runway'}, // RUNWAY Runway
  // 'sand_waves':{'x':'x'}, // SNDWAV Sand waves
  // 'sea_area':{'x':'x'}, // SEAARE Sea area / named water area
  // 'seabed_area':{'x':'x'}, // SBDARE Seabed area
  // 'seaplane_landing_area':{'x':'x'}, // SPLARE Sea-plane landing area
  // 'separation_boundary':{'x':'x'}, // TSSBND Traffic Separation Scheme  Boundary
  // 'separation_crossing':{'x':'x'}, // TSSCRS Traffic Separation Scheme Crossing
  // 'separation_lane':{'x':'x'}, // TSSLPT Traffic Separation Scheme  Lane part
  // 'separation_line':{'x':'x'}, // TSELNE Traffic Separation Line
  // 'separation_roundabout':{'x':'x'}, // TSSRON Traffic Separation Scheme  Roundabout
  // 'separation_zone':{'x':'x'}, // TSEZNE Traffic Separation Zone
  // 'shoreline_construction':{'x':'x'}, // SLCONS Shoreline Construction
  // 'signal_station_traffic':{'x':'x'}, // SISTAT Signal station traffic
  // 'signal_station_warning':{'x':'x'}, // SISTAW Signal station warning
  // 'silo':{'x':'x'}, // SILTNK Silo / tank
  // 'slope_topline':{'x':'x'}, // SLOTOP Slope topline
  // 'sloping_ground':{'x':'x'}, // SLOGRD Sloping ground
  // 'small_craft_facility':{'x':'x'}, // SMCFAC Small craft facility
  // 'sounding':{'x':'x'}, // SOUNDG Sounding
  // 'sounding_datum':{'x':'x'}, // M_SDAT Sounding datum
  'spring':{'natural':'spring'}, // SPRING Spring
  // 'square':{'x':'x'}, // SQUARE Square
  // 'straight_territorial_sea_baseline':{'x':'x'}, // STSLNE Straight territorial sea baseline
  // 'submarine_transit_lane':{'x':'x'}, // SUBTLN Submarine transit lane
  // 'survey_reliability':{'x':'x'}, // M_SREL Survey reliability
  'swept_area':{'waterway':'swept_area'}, // SWPARE Swept Area
  // 'territorial_sea_area':{'x':'x'}, // TESARE Territorial sea area
  // 'tidal_stream':{'x':'x'}, // TS_FEB Tidal stream - flood/ebb
  // 'tidal_stream_harmonic_prediction':{'x':'x'}, // TS_PRH Tidal stream - harmonic prediction
  // 'tidal_stream_non-harmonic_prediction':{'x':'x'}, // TS_PNH Tidal stream - non-harmonic prediction
  // 'tidal_stream_panel_data':{'x':'x'}, // TS_PAD Tidal stream panel data
  // 'tidal_stream_time_series':{'x':'x'}, // TS_TIS Tidal stream - time series
  // 'tide_harmonic_prediction':{'x':'x'}, // T_HMON Tide - harmonic prediction
  // 'tide_non-harmonic_prediction':{'x':'x'}, // T_NHMN Tide - non-harmonic prediction
  // 'tide_time_series':{'x':'x'}, // T_TIMS Tidal stream - time series
  // 'tideway':{'x':'x'}, // TIDEWY Tideway
  // 'topmark':{'x':'x'}, // TOPMAR Top mark
  'tunnel':{'tunnel':'yes'}, // TUNNEL Tunnel
  // 'two-way_route':{'x':'x'}, // TWRTPT Two-way route  part
  // 'unsurveyed_area':{'x':'x'}, // UNSARE Unsurveyed area
  // 'vegetation':{'x':'x'}, // VEGATN Vegetation
  // 'vertical_datum':{'x':'x'}, // M_VDAT Vertical datum of data
  // 'wall':{'x':'x'}, // FNCLNE Fence/wall
  // 'water_turbulence':{'x':'x'}, // WATTUR Water turbulence
  'waterfall':{'waterway':'waterfall'}, // WATFAL Waterfall
  // 'weed':{'x':'x'}, // WEDKLP Weed/Kelp
  'wreck':{'historic':'wreck'}, // WRECKS Wreck
  }, // End extraLayerTagList

  // Extra or better tags to add based on attributes. 
  // These are the ones that can have multiple values
  extraStringListTagList : [
    // CATBRG - Category of bridge
    // ['CATBRG','1','bridge:moveable','no'], // fixed bridge
    ['CATBRG','2','bridge:moveable','opening'], // opening bridge
    ['CATBRG','3','bridge:moveable','swing'], // swing bridge
    ['CATBRG','4','bridge:moveable','lift'], // lifting bridge
    ['CATBRG','5','bridge:moveable','bascule'], // bascule bridge
    ['CATBRG','6','bridge:structure','floating'], // pontoon bridge
    ['CATBRG','7','bridge:moveable','drawbridge'], // draw bridge
    ['CATBRG','8','bridge:moveable','transporter'], // transporter bridge
    ['CATBRG','9','bridge','footbridge'], // footbridge
    ['CATBRG','10','bridge','viaduct'], // viaduct
    ['CATBRG','11','bridge','aqueduct'], // aqueduct
    ['CATBRG','12','bridge:structure','suspension'], // suspension bridge

    // CATLND - Category of land region
    ['CATLND','1','wetland','fen'], // fen
    ['CATLND','2','wetland','marsh'], // marsh
    ['CATLND','3','wetland','moor'], // moor/bog
    ['CATLND','4','natural','heath'], // heathland
    ['CATLND','5','natural','mountain_range'], // mountain range
    ['CATLND','7','geological','canyon'], // canyon lands
    ['CATLND','9','landuse','farmland'], // agricultural land
    ['CATLND','10','natural','grassland'], // savanna/grassland
    ['CATLND','11','leisure','park'], // parkland
    ['CATLND','12','wetland','swamp'], // swamp
    // ['CATLND','13','seamark:land_region','landslide'], // landslide
    ['CATLND','14','natural','lava'], // lava flow
    ['CATLND','15','natural','salt_flat'], // salt pan
    ['CATLND','16','geological','moraine'], // moraine
    ['CATLND','17','natural','crater'], // crater
    ['CATLND','18','natural','cave_entrance'], // cave
    ['CATLND','20','natural','cay'], // cay

    // CATLMK - Category of landmark
    ['CATLMK','1','man_made','cairn'], // cairn
    ['CATLMK','2','landuse','cemetery'], // cemetery
    ['CATLMK','3','man_made','chimney'], // chimney
    ['CATLMK','4','man_made','dish_aerial'], // dish aerial
    ['CATLMK','5','man_made','flag_pole'], // flagstaff (flagpole)
    ['CATLMK','6','man_made','flare'], // flare stack
    ['CATLMK','7','man_made','mast'], // mast
    ['CATLMK','8','aeroway','windsock'], // windsock
    ['CATLMK','9','memorial','monument'], // monument
    ['CATLMK','10','monument','column'], // column (pillar)
    ['CATLMK','11','historic','memorial'], // memorial plaque
    ['CATLMK','12','man_made','obelisk'], // obelisk
    ['CATLMK','13','memorial','statue'], // statue
    ['CATLMK','14','man_made','cross'], // cross
    ['CATLMK','15','man_made','dome'], // dome
    ['CATLMK','16','tower:type','radar'], // radar scanner
    ['CATLMK','17','man_made','tower'], // tower
    ['CATLMK','18','man_made','windmill'], // windmill
    ['CATLMK','19','landmark','windmotor'], // windmotor
    ['CATLMK','20','tower:type','spire'], // spire/minaret
    ['CATLMK','21','natural','stone'], // large rock or boulder on land

    // CATSEA - Category of sea area
    ['CATSEA','5','natural','bay'], // bay
    // ['CATSEA','6','seamark:sea_area:category','trench'], // trench
    // ['CATSEA','7','landuse','basin'], // basin
    ['CATSEA','8','wetland','tidal_flat'], // mud flats
    ['CATSEA','9','natural','reef'], // reef
    // ['CATSEA','10','seamark:sea_area:category','ledge'], // ledge
    // ['CATSEA','11','seamark:sea_area:category','canyon'], // canyon
    // ['CATSEA','12','seamark:sea_area:category','narrows'], // narrows
    ['CATSEA','13','natural','shoal'], // shoal
    // ['CATSEA','14','seamark:sea_area:category','knoll'], // knoll
    // ['CATSEA','15','seamark:sea_area:category','ridge'], // ridge
    // ['CATSEA','16','seamark:sea_area:category','seamount'], // seamount
    // ['CATSEA','17','seamark:sea_area:category','pinnacle'], // pinnacle
    // ['CATSEA','18','seamark:sea_area:category','abyssal_plain'], // abyssal plain
    // ['CATSEA','19','seamark:sea_area:category','plateau'], // plateau
    // ['CATSEA','20','seamark:sea_area:category','spur'], // spur
    // ['CATSEA','21','seamark:sea_area:category','shelf'], // shelf
    // ['CATSEA','22','seamark:sea_area:category','trough'], // trough
    // ['CATSEA','23','seamark:sea_area:category','saddle'], // saddle
    // ['CATSEA','24','seamark:sea_area:category','abyssal_hills'], // abyssal hills
    // ['CATSEA','25','seamark:sea_area:category','apron'], // apron
    // ['CATSEA','26','seamark:sea_area:category','archipelagic_apron'], // archipelagic apron
    // ['CATSEA','27','seamark:sea_area:category','borderland'], // borderland
    // ['CATSEA','28','seamark:sea_area:category','continental_margin'], // continental margin
    // ['CATSEA','29','seamark:sea_area:category','continental_rise'], // continental rise
    // ['CATSEA','30','seamark:sea_area:category','escarpment'], // escarpment
    // ['CATSEA','31','seamark:sea_area:category','fan'], // fan
    // ['CATSEA','32','seamark:sea_area:category','fracture_zone'], // fracture zone
    // ['CATSEA','33','seamark:sea_area:category','gap'], // gap
    // ['CATSEA','34','seamark:sea_area:category','guyot'], // guyot
    // ['CATSEA','35','seamark:sea_area:category','hill'], // hill
    // ['CATSEA','36','seamark:sea_area:category','hole'], // hole
    // ['CATSEA','37','seamark:sea_area:category','levee'], // levee
    // ['CATSEA','38','seamark:sea_area:category','median_valley'], // median valley
    // ['CATSEA','39','waterway','moat'], // moat
    // ['CATSEA','40','seamark:sea_area:category','mountains'], // mountains
    // ['CATSEA','41','seamark:sea_area:category','peak'], // peak
    // ['CATSEA','42','seamark:sea_area:category','province'], // province
    // ['CATSEA','43','seamark:sea_area:category','rise'], // rise
    // ['CATSEA','44','seamark:sea_area:category','sea_channel'], // sea channel
    // ['CATSEA','45','seamark:sea_area:category','seamount_chain'], // seamount chain
    // ['CATSEA','46','seamark:sea_area:category','shelf-edge'], // shelf-edge
    // ['CATSEA','47','seamark:sea_area:category','sill'], // sill
    // ['CATSEA','48','seamark:sea_area:category','slope'], // slope
    // ['CATSEA','49','seamark:sea_area:category','terrace'], // terrace
    // ['CATSEA','50','seamark:sea_area:category','valley'], // valley
    // ['CATSEA','51','seamark:sea_area:category','canal'], // canal
    // ['CATSEA','52','seamark:sea_area:category','lake'], // lake
    // ['CATSEA','53','seamark:sea_area:category','river'], // river
    // ['CATSEA','54','seamark:sea_area:category','reach'], // reach

    // CATSLC - Category of shoreline construction
    ['CATSLC','1','man_made','breakwater'], // breakwater
    ['CATSLC','2','man_made','groyne'], // groyne (groin)
    ['CATSLC','3','man_made','mole'], // mole
    ['CATSLC','4','man_made','pier'], // pier (jetty)
    // ['CATSLC','5','seamark:shoreline_construction:category','promenade_pier'], // promenade pier
    // ['CATSLC','6','seamark:shoreline_construction:category','wharf'], // wharf (quay)
    // ['CATSLC','7','seamark:shoreline_construction:category','training_wall'], // training wall
    // ['CATSLC','8','seamark:shoreline_construction:category','rip_rap'], // rip rap
    // ['CATSLC','9','seamark:shoreline_construction:category','revetment'], // revetment
    // ['CATSLC','10','seamark:shoreline_construction:category','sea_wall'], // sea wall
    // ['CATSLC','11','seamark:shoreline_construction:category','landing_steps'], // landing steps
    // ['CATSLC','12','seamark:shoreline_construction:category','ramp'], // ramp
    ['CATSLC','13','leisure','slipway'], // slipway
    // ['CATSLC','14','seamark:shoreline_construction:category','fender'], // fender
    // ['CATSLC','15','seamark:shoreline_construction:category','solid_face_wharf'], // solid face wharf
    // ['CATSLC','16','seamark:shoreline_construction:category','open_face_wharf'], // open face wharf
    ['CATSLC','17','shoreline:ramp','log'], // log ramp

    // CATVEG - Category of vegetation
    ['CATVEG','1','natural','grassland'], // grass
    ['CATVEG','2','crop','rice'], // paddy field
    ['CATVEG','3','natural','scrub'], // bush
    ['CATVEG','4','natural','deciduous_wood'], // deciduous wood
    ['CATVEG','5','natural','coniferous_wood'], // coniferous wood
    ['CATVEG','6','natural','wood'], // wood in general (inc mixed wood)
    ['CATVEG','7','wetland','mangrove'], // mangroves
    ['CATVEG','8','leisure','park'], // park
    ['CATVEG','9','natural','parkland'], // parkland
    ['CATVEG','10','crop','mixed'], // mixed crops
    ['CATVEG','11','vegetation:type','reed'], // reed
    ['CATVEG','12','natural','moss'], // moss
    ['CATVEG','13','natural','tree'], // tree in general
    ['CATVEG','14','leaf_cycle','evergreen'], // evergreen tree
    ['CATVEG','15','leaf_type','needleleaved'], // coniferous tree
    ['CATVEG','16','vegetation:type','palm'], // palm tree
    ['CATVEG','17','vegetation:type','nipa'], // nipa palm tree
    ['CATVEG','18','vegetation:type','casuarina'], // casuarina tree
    ['CATVEG','19','vegetation:type','eucalypt'], // eucalypt tree
    ['CATVEG','20','leaf_cycle','deciduous'], // deciduous tree
    ['CATVEG','21','vegetation:type','mangrove_tree'], // mangrove tree
    ['CATVEG','22','vegetation:type','filao_tree'], // filao tree

    // FUNCTN - Function
    ['FUNCTN','1','building','yes'], // no function/service of major interest
    ['FUNCTN','2','office','harbour_master'], // harbour-master=s office
    ['FUNCTN','3','amenity','customs'], // custom office
    ['FUNCTN','4','use','health_office'], // health office
    ['FUNCTN','5','building','hospital'], // hospital
    ['FUNCTN','6','amenity','post_office'], // post office
    ['FUNCTN','7','tourism','hotel'], // hotel
    ['FUNCTN','8','railway','station'], // railway station
    ['FUNCTN','9','amenity','police'], // police station
    ['FUNCTN','10','amenity','water_police'], // water-police station
    ['FUNCTN','11','building','pilot_office'], // pilot office
    ['FUNCTN','12','building','pilot_lookout'], // pilot lookout
    ['FUNCTN','13','amenity','bank'], // bank office
    ['FUNCTN','14','use','headquarters_for_district_control'], // headquarters for district control
    ['FUNCTN','15','building','warehouse'], // transit shed/warehouse
    ['FUNCTN','16','industrial','factory'], // factory
    ['FUNCTN','17','use','power_station'], // power station
    ['FUNCTN','18','use','administrative'], // administrative
    ['FUNCTN','19','use','educational_facility'], // educational facility
    ['FUNCTN','20','building','church'], // church
    ['FUNCTN','21','building','chapel'], // chapel
    ['FUNCTN','22','Building','temple'], // temple
    ['FUNCTN','23','building','pagoda'], // pagoda
    ['FUNCTN','24','use','shinto_shrine'], // shinto shrine
    ['FUNCTN','25','use','buddhist_temple'], // buddhist temple
    ['FUNCTN','26','building','mosque'], // mosque
    ['FUNCTN','27','building','marabout'], // marabout
    ['FUNCTN','28','use','lookout'], // lookout
    ['FUNCTN','29','use','communication'], // communication
    ['FUNCTN','30','use','television'], // television
    ['FUNCTN','31','use','radio'], // radio
    ['FUNCTN','32','use','radar'], // radar
    ['FUNCTN','33','use','light_support'], // light support
    ['FUNCTN','34','use','microwave'], // microwave
    ['FUNCTN','35','amenity','cooling'], // cooling
    ['FUNCTN','36','use','observation'], // observation
    ['FUNCTN','37','use','timeball'], // timeball
    ['FUNCTN','38','amenity','clock'], // clock
    ['FUNCTN','39','use','control'], // control
    ['FUNCTN','40','building','airship_mooring'], // airship mooring
    ['FUNCTN','41','leisure','stadium'], // stadium
    ['FUNCTN','42','amenity','bus_station'], // bus station

    // NATSUR - Nature of surface
    ['NATSUR','1','surface','mud'], // mud
    ['NATSUR','2','surface','clay'], // clay
    ['NATSUR','3','surface','silt'], // silt
    ['NATSUR','4','surface','sand'], // sand
    ['NATSUR','5','surface','stone'], // stone
    ['NATSUR','6','surface','gravel'], // gravel
    ['NATSUR','7','surface','pebbles'], // pebbles
    ['NATSUR','8','surface','cobblestone'], // cobbles
    ['NATSUR','9','surface','rock'], // rock
    // ['NATSUR','10','surface','marsh'], // marsh
    ['NATSUR','11','surface','lava'], // lava
    ['NATSUR','12','surface','snow'], // snow
    ['NATSUR','13','surface','ice'], // ice
    ['NATSUR','14','surface','coral'], // coral
    // ['NATSUR','15','surface','swamp'], // swamp
    // ['NATSUR','16','surface','bog/moor'], // bog/moor
    ['NATSUR','17','surface','shells'], // shells
    ['NATSUR','18','surface','boulders'], // boulder

    // PRODCT - Product
    ['PRODCT','1','product','oil'], // oil
    ['PRODCT','2','product','gas'], // gas
    ['PRODCT','3','product','water'], // water
    ['PRODCT','4','product','stone'], // stone
    ['PRODCT','5','product','coal'], // coal
    ['PRODCT','6','product','ore'], // ore
    ['PRODCT','7','product','chemical'], // chemicals
    ['PRODCT','8','product','drinking_water'], // drinking water
    ['PRODCT','9','product','milk'], // milk
    ['PRODCT','10','product','bauxite'], // bauxite
    ['PRODCT','11','product','coke'], // coke
    ['PRODCT','12','product','iron_ingot'], // iron ingots
    ['PRODCT','13','product','salt'], // salt
    ['PRODCT','14','product','sand'], // sand
    ['PRODCT','15','product','timber'], // timber
    ['PRODCT','16','product','sawdust'], // sawdust/wood chips
    ['PRODCT','17','product','scrap_metal'], // scrap metal
    ['PRODCT','18','product','liquefied_natural_gas_(lng)'], // liquified natural gas (LNG)
    ['PRODCT','19','product','liquefied_petroleum_gas_(lpg)'], // liquified petroleum gas (LPG)
    ['PRODCT','20','product','wine'], // wine
    ['PRODCT','21','product','cement'], // cement
    ['PRODCT','22','product','grain'], // grain

    // TECSOU - Technique of sounding measurement
    ['TECSOU','1','source:datum:sounding:method','echo_sounder'], // found by echo-sounder
    ['TECSOU','2','source:datum:sounding:method','side-scan_sonar'], // found by side scan sonar
    ['TECSOU','3','source:datum:sounding:method','multi-beam_echo_sounder'], // found by multi-beam
    ['TECSOU','4','source:datum:sounding:method','diver'], // found by diver
    ['TECSOU','5','source:datum:sounding:method','lead-line'], // found by lead-line
    ['TECSOU','6','source:datum:sounding:method','area_swept_by_wire-drag'], // swept by wire-drag
    ['TECSOU','7','source:datum:sounding:method','laser'], // found by laser
    ['TECSOU','8','source:datum:sounding:method','area_swept_by_vertical_acoustic_system'], // swept by vertical acoustic system
    ['TECSOU','9','source:datum:sounding:method','electromagnetic_sensor'], // found by electromagnetic sensor
    ['TECSOU','10','source:datum:sounding:method','photogrammetry'], // photogrammetry
    ['TECSOU','11','source:datum:sounding:method','satellite_imagery'], // satellite imagery
    ['TECSOU','12','source:datum:sounding:method','levelling'], // found by levelling
    ['TECSOU','13','source:datum:sounding:method','area_swept_by_side-scan_sonar'], // swept by side-scan sonar
    ['TECSOU','14','source:datum:sounding:method','computer_generated'], // computer generated
  ], // End extraStringListTagList


  // Extra or better tags to add based on attributes. 
  // These build on the generic ones added by the layername
  extraTagList : [
    // seamark:building:shape
    // ['BUISHP','1','raw:BUISHP','no_specific_shape'], // no specific shape
    // ['BUISHP','2','raw:BUISHP','tower'], // tower
    // ['BUISHP','3','raw:BUISHP','spire'], // spire
    // ['BUISHP','4','raw:BUISHP','cupola_(dome)'], // cupola (dome)
    ['BUISHP','5','shape','high-rise_building'], // high-rise building - Not great at all.
    ['BUISHP','6','shape','pyramidal'], // pyramid
    ['BUISHP','7','shape','cylindrical'], // cylindrical
    ['BUISHP','8','shape','spherical'], // spherical
    ['BUISHP','9','shape','cubic'], // cubic

    // CATCBL - Category of cable
    ['CATCBL','1','cable:type','power'], // power line
    ['CATCBL','2','cable:type','telephone/telegraph'], // telephone/telegraph
    ['CATCBL','3','cable:type','transmission'], // transmission line
    ['CATCBL','4','cable:type','telephone'], // telephone
    ['CATCBL','5','cable:type','telegraph'], // telegraph
    ['CATCBL','6','cable:type','tethering'], // mooring cable/chain

    // CATFOR - Category of fortified structure
    ['CATFOR','1','historic','castle'], // castle
    ['CATFOR','2','historic','fort'], // fort
    ['CATFOR','3','historic','battery'], // battery
    ['CATFOR','4','bunker_type','blockhouse'], // blockhouse
    ['CATFOR','5','defensive','martello_tower'], // Martello tower
    ['CATFOR','6','fortified:type','redoubt'], // redoubt

    // CATFRY - Category of ferry
    ['CATFRY','1','ferry:type','free-moving'], // free-moving= ferry
    ['CATFRY','2','ferry:cable','yes'], // cable ferry
    ['CATFRY','3','ferry:type','ice'], // ice ferry

    // CATGAT - Category of gate
    ['CATGAT','2','use','flood_barrage'], // flood barrage gate
    ['CATGAT','3','gate','caisson'], // caisson
    ['CATGAT','4','lock','yes'], // lock gate
    ['CATGAT','6','waterway','sluice_gate'], // sluice

    // CATMFA - Category of marine farm/culture
    ['CATMFA','1','aquaculture','crustaceans'], // crustaceans
    ['CATMFA','2','aquaculture','oyster'], // oysters/mussels
    ['CATMFA','3','aquaculture','fish'], // fish
    ['CATMFA','4','aquaculture','seaweed'], // seaweed
    ['CATMFA','5','aquaculture','pearl_culture'], // pearl culture

    // CATROD - Category of road
    // ['CATROD','1','highway','motorway'], // motorway
    // ['CATROD','2','highway','trunk'], // major road
    // ['CATROD','3','highway','primary'], // minor road
    // ['CATROD','4','highway','track'], // track/path
    // ['CATROD','5','highway','secondary'], // major street
    // ['CATROD','6','highway','tertiary'], // minor street
    // ['CATROD','7','highway','crossing'], // crossing

    // VERDAT - Vertical datum
    ['VERDAT','1','source:datum:sounding','mean_low_water_springs'], // Mean low water springs
    ['VERDAT','2','source:datum:sounding','mean_lower_low_water_springs'], // Mean lower low water springs
    ['VERDAT','3','source:datum:sounding','mean_sea_level'], // Mean sea level
    ['VERDAT','4','source:datum:sounding','lowest_low_water'], // Lowest low water
    ['VERDAT','5','source:datum:sounding','mean_low_water'], // Mean low water
    ['VERDAT','6','source:datum:sounding','lowest_low_water_springs'], // Lowest low water springs
    ['VERDAT','7','source:datum:sounding','approximate_mean_low_water_springs'], // Approximate mean low water springs
    ['VERDAT','8','source:datum:sounding','indian_spring_low_water'], // Indian spring low water
    ['VERDAT','9','source:datum:sounding','low_water_springs'], // Low water springs
    ['VERDAT','10','source:datum:sounding','approximate_lowest_astronomical_tide'], // Approximate lowest astronomical tide
    ['VERDAT','11','source:datum:sounding','nearly_lowest_low_water'], // Nearly lowest low water
    ['VERDAT','12','source:datum:sounding','mean_lower_low_water'], // Mean lower low water
    ['VERDAT','13','source:datum:sounding','low_water'], // Low water
    ['VERDAT','14','source:datum:sounding','approximate_mean_low_water'], // Approximate mean low water
    ['VERDAT','15','source:datum:sounding','approximate_mean_lower_low_water'], // Approximate mean lower low water
    ['VERDAT','16','source:datum:sounding','mean_high_water'], // Mean high water
    ['VERDAT','17','source:datum:sounding','mean_high_water_springs'], // Mean high water springs
    ['VERDAT','18','source:datum:sounding','high_water'], // High water
    ['VERDAT','19','source:datum:sounding','approximate_mean_sea_level'], // Approximate mean sea level
    ['VERDAT','20','source:datum:sounding','high_water_springs'], // High water springs
    ['VERDAT','21','source:datum:sounding','mean_higher_high_water'], // Mean higher high water
    ['VERDAT','22','source:datum:sounding','equinoctial_spring_low_water'], // Equinoctial spring low water
    ['VERDAT','23','source:datum:sounding','lowest_astronomical_tide'], // Lowest astronomical tide
    ['VERDAT','24','source:datum:sounding','local_datum'], // Local datum
    ['VERDAT','25','source:datum:sounding','international_great_lakes_datum_1985'], // International Great Lakes Datum 1985
    ['VERDAT','26','source:datum:sounding','mean_water_level'], // Mean water level
    ['VERDAT','27','source:datum:sounding','lower_low_water_large_tide'], // Lower low water large tide
    ['VERDAT','28','source:datum:sounding','higher_high_water_large_tide'], // Higher high water large tide
    ['VERDAT','29','source:datum:sounding','nearly_highest_high_water'], // Nearly highest high water
    ['VERDAT','30','source:datum:sounding','highest_astronomical_tide'], // Highest astronomical tide (HAT)
  ], // End extraTagList


  // Input Translation Rules:
  // These are only used for IMPORT and get added to the standard one2one rules
  one2oneIn : [
  ], // End one2oneIn


  // Export Translation Rules:
  // These are only used for EXPORT and get added to the standard one2one rules
  one2oneOut : [
    ['encLayerName','ADMARE','boundary','administrative'], // ADMARE Administration area (Named)
    ['encLayerName','ADMARE','place','archipelago'], // ADMARE Administration area (Named)
    ['encLayerName','AIRARE','aeroway','aerodrome'], // AIRARE Airport / airfield
    ['encLayerName','BRIDGE','bridge','yes'], // BRIDGE Bridge
    ['encLayerName','BRIDGE','man_made','bridge'], // BRIDGE Bridge - From some OSM data
    // ['encLayerName','BUISGL','building','yes'], // BUISGL Building single
    ['encLayerName','BUAARE','landuse','built_up_area'], // BUAARE Built-up area
    // ['encLayerName','CBLOHD','location','overhead'], // CBLOHD Cable overhead
    // ['encLayerName','CBLSUB','location','underwater'], // CBLSUB Cable submarine
    ['encLayerName','CANALS','waterway','canal'], // CANALS Canal
    ['encLayerName','CAUSWY','man_made','causeway'], // CAUSWY Causeway
    ['encLayerName','CGUSTA','amenity','coast_guard'], // CGUSTA Coastguard station
    ['encLayerName','COALNE','natural','coastline'], // COALNE Coastline
    ['encLayerName','CONVYR','man_made','goods_conveyor'], // CONVYR Conveyor
    ['encLayerName','CRANES','man_made','crane'], // CRANES Crane
    ['encLayerName','DAMCON','waterway','dam'], // DAMCON Dam
    ['encLayerName','DEPCNT','contour','depth'], // DEPCNT Depth contour
    ['encLayerName','DOCARE','waterway','dock'], // DOCARE Dock area
    ['encLayerName','DRYDOC','dock','drydock'], // DRYDOC Dry dock
    ['encLayerName','DYKCON','man_made','dyke'], // DYKCON Dyke
    ['encLayerName','FERYRT','route','ferry'], // FERYRT Ferry route
    ['encLayerName','FLODOC','dock','floating'], // FLODOC Floating dock
    ['encLayerName','GATCON','barrier','gate'], // GATCON Gate
    ['encLayerName','HRBARE','landuse','harbour'], // HRBARE Harbour area (administrative)
    ['encLayerName','LAKARE','water','lake'], // LAKARE Lake
    ['encLayerName','LAKARE','landuse','reservoir'], // LAKARE Lake
    ['encLayerName','LAKARE','water','reservoir'], // LAKARE Lake
    ['encLayerName','LNDMRK','man_made','lighthouse'], // LNDMRK Landmark
    ['encLayerName','LNDARE','place','island'], // LNDARE Land area
    ['encLayerName','LOCMAG','natural','magnetic_anomaly'], // LOCMAG Local magnetic anomaly
    ['encLayerName','LOGPON','water','log_pond'], // LOGPON Log pond
    ['encLayerName','MARCUL','landuse','aquaculture'], // MARCUL Marine farm/culture
    ['encLayerName','PRDARE','power','plant'], // PRDARE Production/storage area
    ['encLayerName','RAILWY','railway','funicular'], // RAILWY Railway
    ['encLayerName','RAILWY','railway','light_rail'], // RAILWY Railway
    ['encLayerName','RAILWY','railway','minitature'], // RAILWY Railway
    ['encLayerName','RAILWY','railway','monorail'], // RAILWY Railway
    ['encLayerName','RAILWY','railway','narrow_gauge'], // RAILWY Railway
    ['encLayerName','RAILWY','railway','rail'], // RAILWY Railway
    ['encLayerName','RAILWY','railway','subway'], // RAILWY Railway
    ['encLayerName','RAILWY','railway','tram'], // RAILWY Railway
    ['encLayerName','RAPIDS','waterway','rapids'], // RAPIDS Rapids
    ['encLayerName','RIVERS','waterway','river'], // RIVERS River
    // ['encLayerName','RIVERS','waterway','stream'], // RIVERS River
    ['encLayerName','ROADWY','highway','road'], // ROADWY Road
    // ['encLayerName','RUNWAY','aeroway','runway'], // RUNWAY Runway
    ['encLayerName','SPRING','natural','spring'], // SPRING Spring
    ['encLayerName','SWPARE','waterway','swept_area'], // SWPARE Swept Area
    ['encLayerName','TUNNEL','tunnel','yes'], // TUNNEL Tunnel
    ['encLayerName','WATFAL','waterway','waterfall'], // WATFAL Waterfall
    ['encLayerName','WRECKS','historic','wreck'], // WRECKS Wreck

    // CATBUA - Category of built-up area
    ['CATBUA','1','landuse','industrial'], // urban area
    ['CATBUA','1','landuse','residential'], // urban area
    ['CATBUA','1','landuse','commercial'], // urban area
    ['CATBUA','1','landuse','retail'], // urban area

    // CATFNC - Category of fence/wall
    ['CATFNC','1','barrier','guard_rail'], // Fence
    ['CATFNC','1','barrier','cable_barrier'], // Fence
    ['CATFNC','4','barrier','city_wall'], // Wall
    ['CATFNC','4','barrier','retaining_wall'], // Wall

    // CATGAT - Category of gate
    ['CATGAT','1','barrier','sliding_gate'], // gate in general
    ['CATGAT','1','barrier','swing_gate'], // gate in general
    ['CATGAT','1','barrier','hampshire_gate'], // gate in general
    ['CATGAT','1','barrier','kissing_gate'], // gate in general
    ['CATGAT','1','barrier','lift_gate'], // gate in general
    ['CATGAT','1','barrier','wicket_gate'], // gate in general

    // CATHAF - Category of harbour facility
    ['CATHAF','3','amenity','ferry_terminal'], // ferry terminal

    // CATLMK - Category of landmark
    ['CATLMK','18','historic','windmill'], // windmill
    ['CATLMK','19','plant:source','wind'], // windmotor
    ['CATLMK','19','generator:source','wind'], // windmotor
    ['CATLMK','19','generator:source',':wind_turbine:'], // windmotor

    // CATLND - Category of land region
    ['CATLND','3','wetland','bog'], // moor/bog
    ['CATLND','9','landuse','allotments'], // agricultural land
    ['CATLND','9','landuse','meadow'], // agricultural land
    ['CATLND','9','landuse','orchard'], // agricultural land
    ['CATLND','9','landuse','farmyard'], // agricultural land
    ['CATLND','12','natural','wetland'], // swamp

    // CATPYL - Category of pylon
    ['CATPYL','1','power','tower'], // power transmission pylon/pole
    ['CATPYL','2','power','pole'], // telephone/telegraph pylon/pole
    ['CATPYL','3','aerialway','pylon'], // aerial cableway/sky pylon

    // CATREA - Category of restricted area
    ['CATREA','4','leisure','nature_reserve'], // nature reserve

    // CATROD - Category of road
    ['CATROD','1','highway','motorway_link'], // motorway
    ['CATROD','2','highway','trunk_link'], // major road
    ['CATROD','3','highway','primary_link'], // minor road
    ['CATROD','5','highway','secondary_link'], // major street
    ['CATROD','6','highway','tertiary_link'], // minor street

    ['CATROD','4','highway','bridleway'], // track/path
    ['CATROD','4','highway','cycleway'], // track/path
    ['CATROD','4','highway','footway'], // track/path
    ['CATROD','4','highway','path'], // track/path
    ['CATROD','4','highway','pedestrian'], // track/path
    // ['CATROD','5','highway','secondary'], // major street
    ['CATROD','6','highway','residential'], // minor street
    ['CATROD','6','highway','service'], // minor street
    ['CATROD','6','highway','tertiary'], // minor street
    ['CATROD','6','highway','unclassified'], // minor street
    ['CATROD','6','highway','living_street'], // minor street
    // ['CATROD','7','highway','crossing'], // crossing

    // CATSEA - Category of sea area
    ['CATSEA','8','wetland','tidalflat'], // mud flats

    // CATVEG - Category of vegetation
    ['CATVEG','1','landuse','grass'], // grass
    ['CATVEG','6','landuse','forest'], // wood in general (inc mixed wood)
    ['CATVEG','6','toponym','forest'], // wood in general (inc mixed wood)

    ['FUNCTN','5','amenity','hospital'], // hospital
    ['FUNCTN','18','building','government'], // Administrative
    ['FUNCTN','18','building','public'], // Administrative
    ['FUNCTN','19','amenity','school'], // educational facility
    ['FUNCTN','19','building','school'], // educational facility
    ['FUNCTN','19','amenity','college'], // educational facility
    ['FUNCTN','19','amenity','university'], // educational facility
    ['FUNCTN','19','building','university'], // educational facility
    ['FUNCTN','19','amenity','kindergarten'], // educational facility
    ['FUNCTN','19','building','kindergarten'], // educational facility
    ['FUNCTN','19','amenity','music_school'], // educational facility

    // NATSUR - Nature of surface
    ['NATSUR','4','natural','sand'], // sand

  ], // End one2oneOut
  // ##### End of One2One Rules #####

  // Layername to seamark:type tag.
  // Also used to help find the layername on export
  layerToSeaMark :{
  'ACHARE':'anchorage','ACHBRT':'anchor_berth','AIRARE':'airport','ARCSLN':'archipelagic_sea_lane',
  'ASLXIS':'archipelagic_sea_lane_axis','BCNCAR':'beacon_cardinal','BCNISD':'beacon_isolated_danger',
  'BCNLAT':'beacon_lateral','BCNSAW':'beacon_safe_water','BCNSPP':'beacon_special_purpose',
  'BERTHS':'berth','BOYCAR':'buoy_cardinal','BOYINB':'buoy_installation','BOYISD':'buoy_isolated_danger',
  'BOYLAT':'buoy_lateral','BOYSAW':'buoy_safe_water','BOYSPP':'buoy_special_purpose','BRIDGE':'bridge',
  'BUAARE':'built_up_area','BUISGL':'building','C_AGGR':'aggregation','C_ASSO':'association',
  'CANALS':'canal','CAUSWY':'causeway','CBLARE':'cable_area','CBLOHD':'cable_overhead',
  'CBLSUB':'cable_submarine','CGUSTA':'coastguard_station','CHKPNT':'checkpoint','COALNE':'coastline',
  'CONVYR':'conveyor','CONZNE':'contiguous_zone','COSARE':'continental_shelf_area','CRANES':'crane',
  'CTNARE':'caution_area','CTRPNT':'control_point','CTSARE':'cargo_transhipment_area',
  'CURENT':'non_gravitational_current','CUSZNE':'custom_zone','DAMCON':'dam','DAYMAR':'daymark',
  'DEPARE':'depth_area','DEPCNT':'depth_contour','DISMAR':'distance_mark','DMPGRD':'dumping_ground',
  'DOCARE':'dock','DRGARE':'dredged_area','DRYDOC':'dry_dock','DWRTCL':'deep_water_route_centerline',
  'DWRTPT':'deep_water_route_part','DYKCON':'dyke','EXEZNE':'exclusive_economic_zone','FAIRWY':'fairway',
  'FERYRT':'ferry_route','FLODOC':'floating_dock','FNCLNE':'wall','FOGSIG':'fog_signal',
  'FORSTC':'fortified_structure','FRPARE':'free_port_area','FSHFAC':'fishing_facility',
  'FSHGRD':'fishing_ground','FSHZNE':'fishery_zone','GATCON':'gate','GRIDRN':'gridiron',
  'HRBARE':'harbour_area','HRBFAC':'harbour','HULKES':'hulk','ICEARE':'ice_area',
  'ICNARE':'incineration_area','ISTZNE':'inshore_traffic_zone','LAKARE':'lake','LIGHTS':'light',
  'LITFLT':'light_float','LITVES':'light_vessel','LNDARE':'land_area','LNDELV':'land_elevation',
  'LNDMRK':'landmark','LNDRGN':'land_region','LOCMAG':'local_magnetic_anomaly','LOGPON':'log_pond',
  'LOKBSN':'lock_basin','M_ACCY':'data_accuracy','M_COVR':'coverage','M_CSCL':'compilation_scale',
  'M_HOPA':'horizontal_datum_shift_parameters','M_NPUB':'nautical_publication',
  'M_NSYS':'navigation_system_of_marks','M_QUAL':'data_quality','M_SDAT':'sounding_datum',
  'M_SREL':'survey_reliability','M_VDAT':'vertical_datum','MAGVAR':'magnetic_variation',
  'MARCUL':'marine_farm','MIPARE':'military_area','MORFAC':'mooring','NAVLNE':'navigation_line',
  'NEWOBJ':'new_object','OBSTRN':'obstruction','OFSPLF':'platform','OILBAR':'oil_barrier',
  'OSPARE':'production_area','PILBOP':'pilot_boarding','PILPNT':'pile','PIPARE':'pipeline_area',
  'PIPOHD':'pipeline_overhead','PIPSOL':'pipeline_submarine','PONTON':'pontoon','PRCARE':'precautionary_area',
  'PRDARE':'production_storage_area','PYLONS':'pylon','RADLNE':'radar_line','RADRFL':'radar_reflector',
  'RADRNG':'radar_range','RADSTA':'radar_station','RAILWY':'rail','RAPIDS':'rapids',
  'RCRTCL':'reccomended_route_centerline','RCTLPT':'reccomended_traffic_lane','RDOCAL':'calling_in_point',
  'RDOSTA':'radio_station','RECTRC':'reccomended_track','RESARE':'restricted_area','RETRFL':'retro_reflector',
  'RIVERS':'river','ROADWY':'road','RSCSTA':'rescue_station','RTPBCN':'radar_transponder','RUNWAY':'runway',
  'SBDARE':'seabed_area','SEAARE':'sea_area','SILTNK':'silo','SISTAT':'signal_station_traffic',
  'SISTAW':'signal_station_warning','SLCONS':'shoreline_construction','SLOGRD':'sloping_ground',
  'SLOTOP':'slope_topline','SMCFAC':'small_craft_facility','SNDWAV':'sand_waves','SOUNDG':'sounding',
  'SPLARE':'seaplane_landing_area','SPRING':'spring','STSLNE':'straight_territorial_sea_baseline',
  'SUBTLN':'submarine_transit_lane','SWPARE':'swept_area','T_HMON':'tide_harmonic_prediction',
  'T_NHMN':'tide_non-harmonic_prediction','T_TIMS':'tide_time_series','TESARE':'territorial_sea_area',
  'TIDEWY':'tideway','TOPMAR':'topmark','TS_FEB':'tidal_stream','TS_PAD':'tidal_stream_panel_data',
  'TS_PNH':'tidal_stream_non-harmonic_prediction','TS_PRH':'tidal_stream_harmonic_prediction',
  'TS_TIS':'tidal_stream_time_series','TSELNE':'separation_line','TSEZNE':'separation_zone',
  'TSSBND':'separation_boundary','TSSCRS':'separation_crossing','TSSLPT':'separation_lane',
  'TSSRON':'separation_roundabout','TUNNEL':'tunnel','TWRTPT':'two-way_route','UNSARE':'unsurveyed_area',
  'UWTROC':'rock','VEGATN':'vegetation','WATFAL':'waterfall','WATTUR':'water_turbulence',
  'WEDKLP':'weed','WRECKS':'wreck'
  },

  // ##### Start of ignoreList #####
  // This is taken from OSM pre processing and a few added
  ignoreList : [
    'APT2','APT3',
    'APU2','APU3'
  ],
  // ##### End of ignoreList #####


  // ##### Start of swapListIn #####
  // List of attributes that have the same values so we swap them on import.
  swapListIn : {
    'DEPTH':'VALSOU',
  },
  // ##### End of swapListIn #####

  // ##### Start of swapListOut #####
  // Format is: <layer>:{<from>:<to>}
  swapListOut : {
  },
  // ##### End of swapListOut #####

  // ##### Start of txtLength #####
  // This list is for validateing the lengths of text attributes prior to export
  txtLength : {
  },
  // ##### End of txtLength #####

  // ##### Start of intList #####
  // This list is for validateing the integer attributes prior to export
  intList : ['CSCALE','MLTYLT','SCAMAX','SCAMIN','SCVAL1','SCVAL2','SDISMN','SDISMX','SIGFRQ','T_TINT'],
  // ##### End of intList #####

  // Start of attrLookup
  // This table is for validateing the attributes for each feature
  attrLookup : {},
  // End of attrLookup

}; // End of enc311.rules
