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
# This script builds a schema from a csv file
#
# Note: This script does not do any error or sanity checking of the input files.
#
# Mattj Feb 16
#

import sys,os,csv,argparse,gzip


def printJSHeader():
    print notice
    print
    print "var _global = (0, eval)('this');"
    print 'if (!_global.tds)'
    print '{'
    print '  _global.tds = {};'
    print '}'
    print
    print 'tds.schema = {'
    print '//  Schema built from %s' % (main_csv_file)
    print 'getDbSchema: function()'
    print '{'

# printJavascript: Dump out the structure as Javascript
#
# Note: This uses double quotes ( " ) around data elements in the output.  The csv files have values with
# single quotes ( ' ) in them.  These quotes are also in the DFDD and NFDD specs.
def printJavascript(schema):
    print '    var schema = [' # And so it begins...

    num_feat = len(schema.keys()) # How many features in the schema?
    for f in sorted(schema.keys()):
        # Skip all of the 'Table' features
        if schema[f]['geom'] == 'Table':
            continue

        print '        { name:"%s",' % (f); # name = geom + FCODE
        print '          fcode:"%s",' % (schema[f]['fcode'])
        print '          desc:"%s",' % (schema[f]['desc'])
        print '          geom:"%s",' % (schema[f]['geom'])
        print '          columns:[ '

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            print '                     { name:"%s",' % (k)
            print '                       desc:"%s" ,' % (schema[f]['columns'][k]['desc'])
            print '                       optional:"%s" ,' % (schema[f]['columns'][k]['optional'])

            #if schema[f]['columns'][k]['length'] != '':
            if 'length' in schema[f]['columns'][k]:
                print '                       length:"%s", ' % (schema[f]['columns'][k]['length'])

            #if schema[f]['columns'][k]['type'].find('numeration') != -1:
            if 'func' in schema[f]['columns'][k]:
                print '                       type:"enumeration",'
                print '                       defValue:"%s", ' % (schema[f]['columns'][k]['defValue'])
                print '                       enumerations: %s' % (schema[f]['columns'][k]['func'])

            elif schema[f]['columns'][k]['type'] == 'enumeration':
                #print '                       type:"%s",' % (schema[f]['columns'][k]['type'])
                print '                       type:"enumeration",'
                print '                       defValue:"%s", ' % (schema[f]['columns'][k]['defValue'])
                print '                       enumerations:['
                for l in schema[f]['columns'][k]['enum']:
                    print '                           { name:"%s", value:"%s" }, ' % (l['name'],l['value'])
                print '                        ] // End of Enumerations '

            #elif schema[f]['columns'][k]['type'] == 'textEnumeration':
                #print '                       type:"Xenumeration",'
                #print '                       defValue:"%s", ' % (schema[f]['columns'][k]['defValue'])
                #print '                       enumerations: text_%s' % (k)

            else:
                print '                       type:"%s",' % (schema[f]['columns'][k]['type'])
                print '                       defValue:"%s" ' % (schema[f]['columns'][k]['defValue'])

            if num_attrib == 1:  # Are we at the last attribute? yes = no trailing comma
                print '                     } // End of %s' % (k)
            else:
                print '                     }, // End of %s' % (k)
                num_attrib -= 1

        print '                    ] // End of Columns'

        if num_feat == 1: # Are we at the last feature? yes = no trailing comma
            print '          } // End of feature %s\n' % (schema[f]['fcode'])
        else:
            print '          }, // End of feature %s\n' % (schema[f]['fcode'])
            num_feat -= 1

    print '    ]; // End of schema\n' # End of schema


def printJSFooter():
    print '    return schema; \n'
    print '} // End of getDbSchema\n'
    print '} // End of tds.schema\n'
    print
    print 'exports.getDbSchema = tds.schema.getDbSchema;'
    print

# End printJavascript


# Print out a codelist as a JS variable
def printVariableBody(name,var):
    print '    var %s = [' % (name)

    num_vals = len(var.keys()) # How many values does the thing have?
    for l in var:
        if num_vals == 1: # Are we at the last feature? yes = no trailing comma
            print '              { name:"%s", value:"%s" } ' % (l,var[l])
        else:
            print '              { name:"%s", value:"%s" }, ' % (l,var[l])
            num_vals -= 1

    print '             ];'
    print
# End printVariableBody


# Drop all of the text enumerations and replace them with strings
def dropTextEnumerations(tschema):
    for i in tschema:
        for j in tschema[i]['columns']:
            if tschema[i]['columns'][j]['type'] == 'textEnumeration':
                del tschema[i]['columns'][j]['func']
                tschema[i]['columns'][j]['type'] = 'String'

    return tschema
# End dropTextEnumerations


def asint(s):
    try: return int(s)
    except ValueError: return s


# Print Rules
def printRules(schema):
    tList = {}
    for i in schema:
        for j in schema[i]['columns']:
            #if schema[i]['columns'][j]['type'] != 'enumeration':
            if schema[i]['columns'][j]['type'].find('numeration') == -1:
                continue

            eName = schema[i]['columns'][j]['name']

            if eName not in tList:
                tList[eName] = {}
                tList[eName]['desc'] = schema[i]['columns'][j]['desc']
                tList[eName]['enum'] = {}

            for k in schema[i]['columns'][j]['enum']:
                if k['value'] not in tList[eName]['enum']:
                    tList[eName]['enum'][k['value']] = {}

                tList[eName]['enum'][k['value']] = k['name']
            #tList[schema[i]['columns'][j]['name']] = schema[i]['columns'][j]['desc']

    for i in sorted(tList.keys()):
        print "\n     // %s - %s" % (i,tList[i]['desc'])
        for j in sorted(tList[i]['enum'].keys(), key=asint):
            rVal = tList[i]['enum'][j]
            eVal = rVal.replace(' ','_').lower()
            if j == '-999999':
                print "     // ['%s','%s',undefined,undefined], // %s" % (i,j,rVal)
                continue
            if j == '1000':
                print "     ['%s','%s','raw:%s','no']," % (i,j,i)
                continue
            if j == '1001':
                print "     ['%s','%s','raw:%s','yes']," % (i,j,i)
                continue
            print "     ['%s','%s','raw:%s','%s'], // %s" % (i,j,i,eVal,rVal)

# End printRules


# Print Attribute List
def printAttrList(schema):
    tList = {}

    for i in schema:
        for j in schema[i]['columns']:

            eName = schema[i]['columns'][j]['name']

            if eName not in tList:
                tList[eName] = schema[i]['columns'][j]['desc']

    for i in sorted(tList.keys()):
        print "    '%s':'%s'," % (i,tList[i])
# End printAttrList


# Print FCode List
def printFcodeList(schema):
    tList = {}
    for i in schema:
        if schema[i]['fcode'] not in tList:
            tList[schema[i]['fcode']] = schema[i]['desc']

    print '"FCODE","Name"'
    for i in sorted(tList.keys()):
        print '"%s","%s"' % (i,tList[i])
# End printFcodeList


# Print FCode Attribute List
def printFcodeAttrList(schema):
    tList = {}
    for i in schema:
        if schema[i]['fcode'] not in tList:
            tList[schema[i]['fcode']] = []
            for j in schema[i]['columns']:
                tList[schema[i]['fcode']].append(schema[i]['columns'][j]['name'])

    for i in sorted(tList.keys()):
        print '%s : %s' % (i,tList[i])
# End printFcodeList


# Print ToEnglish
# Dump out the set of TDS English rules
def printToEnglish(schema):
    eList = {}
    sList = {}
    dList = {}
    fList = {}
    for i in schema:
        fList[schema[i]['fcode']] = {}
        fList[schema[i]['fcode']]['desc'] = schema[i]['desc']
        fList[schema[i]['fcode']]['enum'] = []
        for j in schema[i]['columns']:
            eName = schema[i]['columns'][j]['name']
            if eName == 'F_CODE':
                continue

            if eName not in dList: # Default Values
                dList[eName] = {}
            dList[eName] = schema[i]['columns'][j]['defValue']
            fList[schema[i]['fcode']]['enum'].append(eName)

            # if schema[i]['columns'][j]['type'] == 'enumeration':
            if schema[i]['columns'][j]['type'].find('numeration') > -1:
                if eName not in eList:
                    eList[eName] = {}

                for k in schema[i]['columns'][j]['enum']:
                    if k['value'] not in eList[eName]:
                        eList[eName][k['value']] = {}
                    eList[eName][k['value']] = [schema[i]['columns'][j]['desc'],k['name']]
            else:
                if eName not in sList: # Single Values
                    sList[eName] = {}
                sList[eName] = schema[i]['columns'][j]['desc']

    print notice
    print ''
    print '/*'
    print '    TDSv40 "To English" Lookup Tables\n'
    print '    Huge piles of Ugly JSON!'
    print '*/\n'
    print 'etds40.rules = {'
    print '    // Tables built from %s' % (main_csv_file)

    # Dump out the Enumerated Values
    print '    // Enumerated Values'
    print '    engEnum : %s , // End of engEnum\n' % (eList)

    # Dump out the Single Values
    print '    // Single Values'
    print '    engSingle : %s , // End of engSingle\n' % (sList)

    # Dump out the Default Values
    print '    // Default values for attributes'
    print '    engDefault : %s , // End of engDefault\n' % (dList)

    # Dump out the FCodes
    print '    // FCode Lookup Table'
    print '    fcodeLookup : %s ,  // End of fcodeLookup\n' % (fList)

    print '} // End of etds40.rules\n'
# End printToEnglish


# Print FromEnglish
# Dump out the set of TDS English rules
def printFromEnglish(schema):
    eList = {}
    sList = {}
    for i in schema:
        for j in schema[i]['columns']:
            #if schema[i]['columns'][j]['name'] == 'F_CODE':
                #continue

            eDesc = schema[i]['columns'][j]['desc']
            if schema[i]['columns'][j]['type'].find('numeration') > -1:
                if eDesc not in eList:
                    eList[eDesc] = {}

                for k in schema[i]['columns'][j]['enum']:
                    if k['value'] not in eList[eDesc]:
                        eList[eDesc][k['name']] = {}
                    eList[eDesc][k['name']] = [schema[i]['columns'][j]['name'],k['value']]
            else:
                if eDesc not in sList: # Single Values
                    sList[eDesc] = {}
                sList[eDesc] = schema[i]['columns'][j]['name']

    print notice
    print ''
    print '/*'
    print '    TDSv40 "From English" Lookup Tables\n'
    print '    Huge piles of Ugly JSON!'
    print '*/\n'
    print 'etds40_osm_rules = {'
    print '    // Tables built from %s' % (main_csv_file)

    # Dump out the Enumerated Values
    print '    // Enumerated Values'
    print '    enumValues : %s , // End of enumValues\n' % (eList)

    # Dump out the Single Values
    print '    // Single Values'
    print '    singleValues : %s , // End of singleValues\n' % (sList)

    print '} // End of etds40_osm_rules\n'
# End printFromEnglish


# Print Text Rules
def printTxtRules(schema):
    tList = {}

    for i in schema:
        for j in schema[i]['columns']:
            if schema[i]['columns'][j]['type'] != 'String':
                continue
            tList[schema[i]['columns'][j]['name']] = schema[i]['columns'][j]['desc']

    for i in sorted(tList.keys()):
        print "     '%s':'raw:%s', // %s" % (i,i,tList[i])
# End printTxtRules


# Print Number Rules
def printNumRules(schema):
    tList = {}
    for i in schema:
        for j in schema[i]['columns']:
            if schema[i]['columns'][j]['type'].find('numeration') > -1:
                continue
            if schema[i]['columns'][j]['type'] == 'String':
                continue
            #if schema[i]['columns'][j]['name'] in tList:
                #continue
            tList[schema[i]['columns'][j]['name']] = schema[i]['columns'][j]['desc']

    for i in sorted(tList.keys()):
        print "     '%s':'raw:%s', // %s" % (i,i,tList[i])
# End printNumRules


# Print CSV Attribute List
def printAttributeCsv(schema):
    fList = {}
    fDesc = ''
    cDesc = ''
    for i in schema:
        fDesc = '"' + schema[i]['desc'] + '","' + schema[i]['fcode'] + '"'
        if fDesc not in fList:
            fList[fDesc] = {}

        for j in schema[i]['columns']:
            cDesc = schema[i]['columns'][j]['desc']

            if cDesc not in fList[fDesc]:
                fList[fDesc][cDesc] = []

            if schema[i]['columns'][j]['type'].find('numeration') > -1:
                for k in schema[i]['columns'][j]['enum']:
                    if k['name'] not in fList[fDesc][cDesc]:
                        fList[fDesc][cDesc].append(k['name'])
            else:
                fList[fDesc][cDesc].append('Value')

    print '"Feature","FCODE","Name","Value"'
    for i in sorted(fList.keys()):
        for j in sorted(fList[i].keys()):
            for k in sorted(fList[i][j]):
                #print '"%s","%s","%s"' % (i,j,k)
                print '%s,"%s","%s"' % (i,j,k)
# End printAttributeCsv


def openFile(path, mode):
    if path.endswith(".gz"):
        return gzip.GzipFile(path, mode)
    else:
        return open(path, mode)


# Data & Lists
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

geo_list = {'T':'Table', 'C':'Line', 'S':'Area', 'P':'Point' }

buildingFuncList = {
    }


textFuncList = {
    'CPS':'text_CPS',
    'EQC':'text_EQC',
    'ETS':'text_ETS',
    'HZD':'text_HZD',
    'IC2':'text_IC2',
    'RCG':'text_RCG',
    'VDT':'text_VDT',
    'ZI001_NSP':'text_NSP',
    'ZI001_SSY':'text_SSY',
    'ZI001_VSC':'text_VSC',
    'ZI004_RCG':'text_RCG',
    'ZSAX_RS0':'text_ZSAX_RS0',
    }

dataType_list = {
    'Double':'Real',
    'Enumeration':'enumeration',
    'LexicalText':'String',
    'CodeList':'textEnumeration',
    'LongInteger':'Integer',
    'NonlexicalText':'String'
    }

default_list = {
    'String':'',
    'Real':'-999999.0',
    'Integer':'-999999',
    'textEnumeration':'',
    'enumeration':'-999999'
}

# FFN for Buildings
building_FFN = {
    'No Information':'-999999', 'Other':'999', 'Agriculture':'2', 'Growing of Crops':'3', 'Raising of Animals':'9',
    'Grazing':'14', 'Mixed Farming':'15', 'Hunting':'19', 'Forestry and/or Logging':'20', 'Silviculture':'21',
    'Forest Warden':'27', 'Fishing':'30', 'Aquaculture':'35', 'Mining and Quarrying':'40', 'Solid Mineral Fuel Mining':'50',
    'Petroleum and/or Gas Extraction':'60', 'Metal Ore Mining':'70', 'Chemical Mining':'83', 'Mineral Mining':'87', 'Gas Oil Separation':'91',
    'Ore Dressing':'95', 'Manufacturing':'99', 'Food Product Manufacture':'100', 'Food Processing':'101', 'Meat Processing':'102',
    'Seafood Processing':'103', 'Fruit and/or Vegetable Processing':'104', 'Oil-mill':'105', 'Dairying':'106', 'Grain Milling':'107',
    'Baking':'110', 'Sugar Manufacture':'111', 'Sugar Milling':'112', 'Sugar Refining':'113', 'Confection Manufacture':'114',
    'Pasta Manufacture':'115', 'Prepared Meal Manufacture':'116', 'Animal Feed Manufacture':'119', 'Ice Manufacture':'120', 'Beverage Manufacture':'118',
    'Spirit Distillery':'121', 'Winery':'122', 'Brewing':'123', 'Soft Drink Manufacture':'124',
    'Tobacco Product Manufacture':'125', 'Textile, Apparel and Leather Manufacture':'129', 'Textile Manufacture':'130', 'Apparel Manufacture':'140',
    'Leather Product Manufacture':'150', 'Footwear Manufacturing':'155', 'Wood-based Manufacturing':'160', 'Sawmilling':'161',
    'Wooden Construction Product Manufacture':'165', 'Paper-mill':'171', 'Printing':'181', 'Petroleum and Coal Products Manufacturing':'190',
    'Petroleum Refining':'192', 'Coke Manufacture':'191', 'Chemical Manufacture':'195', 'Medicinal Product Manufacture':'210',
    'Rubber Product Manufacture':'221', 'Plastic Product Manufacture':'225', 'Nonmetallic Mineral Product Manufacturing':'230', 'Glass Product Manufacture':'231',
    'Refractory Product Manufacture':'232', 'Clay Product Manufacture':'233', 'Ceramic Product Manufacture':'234', 'Cement Mill':'235',
    'Cement Product Manufacture':'236', 'Stone Product Manufacture':'237', 'Primary Metal Manufacturing':'240', 'Steel Mill':'241',
    'Metal Refining':'242', 'Foundry':'243', 'Metal Product Manufacturing':'250', 'Structural Metal Product Manufacture':'251',
    'Fabricated Metal Product Manufacture':'257', 'Munitions Manufacture':'255', 'Electronic Equipment Manufacture':'260', 'Electrical Equipment Manufacture':'270',
    'Machinery Manufacture':'280', 'Transportation Equipment Manufacturing':'289', 'Motor Vehicle Manufacture':'290', 'Ship Construction':'301',
    'Railway Vehicle Manufacture':'304', 'Aircraft Manufacture':'305', 'Military Vehicle Manufacture':'306', 'Furniture Manufacture':'310',
    'Miscellaneous Manufacturing':'320', 'Jewellery Manufacture':'321', 'Musical Instrument Manufacture':'322', 'Sports Goods Manufacture':'323',
    'Game and/or Toy Manufacture':'324', 'Medical and/or Dental Equipment Manufacture':'325', 'General Repair':'330', 'Fabricated Metal Product Repair':'331',
    'Electronic Equipment Repair':'332', 'Electrical Equipment Repair':'333', 'Machinery Repair':'334', 'Railway Vehicle Repair':'342',
    'Motor Vehicle Repair':'343', 'Ship Repair':'340', 'Aircraft Repair':'341', 'Utilities':'350',
    'Power Generation':'351', 'Climate Control':'352', 'Cooling':'355', 'Heating':'356',
    'Water Supply':'360', 'Water Collection':'361', 'Water Treatment':'362', 'Water Distribution':'363',
    'Sewerage':'370', 'Sewerage Screening':'372', 'Restroom':'382', 'Waste Treatment and Disposal':'383',
    'Materials Recovery':'385', 'Commerce':'440', 'Wholesale Merchant':'459', 'Retail Sale':'460',
    'Specialized Store':'464', 'Petrol Sale':'470', 'Propane Sale':'272', 'Precious Metal Merchant':'474',
    'Pharmacy':'477', 'Pet-shop':'478', 'Non-specialized Store':'465', 'Convenience Store':'466',
    'Grocery':'476', 'Market':'475', 'Sales Yard':'473', 'Transport':'480',
    'Transportation Hub':'489', 'Station':'482', 'Stop':'483', 'Terminal':'481',
    'Transfer Hub':'484', 'Railway Transport':'490', 'Railway Passenger Transport':'491', 'Pedestrian Transport':'494',
    'Road Transport':'495', 'Road Freight Transport':'497', 'Road Passenger Transport':'496', 'Pipeline Transport':'500',
    'Pumping':'501', 'Water Transport':'505', 'Inland Waters Transport':'507', 'Canal Transport':'508',
    'Harbour Control':'513', 'Port Control':'510', 'Maritime Pilotage':'511', 'Pilot Station':'512',
    'Air Transport':'520', 'Air Traffic Control':'525', 'Mail and Package Transport':'541', 'Postal Activities':'540',
    'Courier Activities':'545', 'Transportation Support':'529', 'Navigation':'488', 'Signalling':'486',
    'Transport System Maintenance':'487', 'Warehousing and Storage':'530', 'Cargo Handling':'536', 'Motor Vehicle Parking':'535',
    'Inspection':'539', 'Customs Checkpoint':'537', 'Inspection Station':'538', 'Accommodation':'550',
    'Short-term Accommodation':'548', 'Communal Bath':'559', 'Guest-house':'554', 'Hostel':'555',
    'Hotel':'551', 'Motel':'553', 'Resort':'552', 'Vacation Cottage':'557',
    'Long-term Accommodation':'549', 'Dormitory':'556', 'Dependents Housing':'558', 'Residence':'563',
    'Food Service':'570', 'Restaurant':'572', 'Bar':'573', 'Dining Hall':'574',
    'Banquet Hall':'578', 'Publishing and Broadcasting':'580', 'Print Publishing':'582', 'Radio Broadcasting':'601',
    'Television Broadcasting':'604', 'Information Service':'632', 'Public Records':'633', 'Telecommunications':'610',
    'Retail Telecommunications':'612', 'Wired Telecommunications':'614', 'Main Telephone Exchange':'615', 'Branch Telephone Exchange':'616',
    'Wired Repeater':'617', 'Wireless Telecommunications':'620', 'Mobile Phone Service':'621', 'Wireless Repeater':'622',
    'Satellite Telecommunications':'625', 'Satellite Ground Control':'626', 'Financial Services':'640', 'Central Banking':'642',
    'Retail Banking':'643', 'Insurance':'651', 'Financial Market Administration':'662', 'Security Brokerage':'663',
    'Fund Management':'671', 'Real Estate Activities':'680', 'Professional, Scientific and Technical':'681', 'Accounting':'696',
    'Advertising':'741', 'Architecture Consulting':'711', 'Business Management':'706', 'Engineering Design':'714',
    'Head Office':'701', 'Legal Activities':'691', 'Photography':'752', 'Scientific Research and Development':'720',
    'Nuclear Research Centre':'725', 'Observation Station':'721', 'Weather Station':'722', 'Wind Tunnel':'730',
    'Surveying':'717', 'Veterinary':'757', 'Business and Personal Support Services':'760', 'Administration':'810',
    'Animal Boarding':'919', 'Beauty Treatment':'962', 'Call Centre':'807', 'Custodial Service':'791',
    'Employment Agency':'770', 'Headquarters':'809', 'Landscaping Service':'795', 'Laundry':'961',
    'Motor Vehicle Rental':'761', 'Office Administration':'801', 'Travel Agency':'775', 'Public Administration':'808',
    'Government':'811', 'National Government':'814', 'Subnational Government':'813', 'Local Government':'812',
    'Executive Activities':'818', 'Legislative Activities':'819', 'Civil Activities':'822', 'Capitol':'817',
    'Palace':'815', 'Polling Station':'821', 'Diplomacy':'825', 'Consul':'828',
    'Diplomatic Mission':'826', 'Embassy':'827', 'Defence Activities':'835', 'Armory':'836',
    'Maritime Defense':'829', 'Military Recruitment':'838', 'Military Reserve Activities':'837', 'Public Order, Safety and Security Services':'830',
    'Public Order':'831', 'Immigration Control':'842', 'Imprisonment':'843', 'Judicial Activities':'840',
    'Juvenile Corrections':'844', 'Law Enforcement':'841', 'Safety':'832', 'Firefighting':'845',
    'Rescue and Paramedical Services':'846', 'Emergency Operations':'847', 'Emergency Relief Services':'888', 'Civil Intelligence':'848',
    'CBRNE Civilian Support':'839', 'Security Services':'833', 'Guard':'781', 'Security Enforcement':'780',
    'Education':'850', 'Primary Education':'851', 'Secondary Education':'852', 'Higher Education':'855',
    'Vocational Education':'857', 'Human Health Activities':'860', 'In-patient Care':'861', 'Intermediate Care':'871',
    'Psychiatric In-patient Care':'873', 'Residential Care':'875', 'Out-patient Care':'862', 'Urgent Medical Care':'863',
    'Human Tissue Repository':'864', 'Leprosy Care':'866', 'Public Health Activities':'865', 'Social Work':'887',
    'Day Care':'885', 'Emergency Shelter':'881', 'Emergency Youth Shelter':'884', 'Homeless Shelter':'882',
    'Refugee Shelter':'883', 'Cultural, Arts and Entertainment':'890', 'Aquarium':'906', 'Auditorium':'892',
    'Botanical and/or Zoological Reserve Activities':'907', 'Cinema':'594', 'Library':'902', 'Museum':'905',
    'Night Club':'895', 'Opera House':'894', 'Theatre':'891', 'Sports, Amusement and Recreation':'900',
    'Adult Entertainment':'966', 'Amusement':'922', 'Fitness Centre':'913', 'Gambling':'909',
    'Hobbies and/or Leisure Activities':'923', 'Recreation':'921', 'Shooting Range':'914', 'Sports Centre':'912',
    'Religious Activities':'930', 'Place of Worship':'931', 'Islamic Prayer Hall':'932', 'Membership Organization':'950',
    'Club':'954', 'Yacht-club':'955', 'Death Care Services':'980', 'Funeral Services':'963',
    'Cremation':'964', 'Mortuary Services':'965', 'Storage of Human Remains':'967', 'Meeting Place':'970',
    'Community Centre':'893', 'Convention Centre':'579'
}

# FFN For Fortified BUilding
fortified_FFN = {
    'No Information':'-999999', 'Other':'999', 'Agriculture':'2', 'Growing of Crops':'3',
    'Raising of Animals':'9', 'Grazing':'14', 'Mixed Farming':'15', 'Hunting':'19',
    'Forestry and/or Logging':'20', 'Silviculture':'21', 'Forest Warden':'27', 'Fishing':'30',
    'Aquaculture':'35', 'Mining and Quarrying':'40', 'Solid Mineral Fuel Mining':'50', 'Petroleum and/or Gas Extraction':'60',
    'Metal Ore Mining':'70', 'Chemical Mining':'83', 'Mineral Mining':'87', 'Gas Oil Separation':'91',
    'Ore Dressing':'95', 'Manufacturing':'99', 'Food Product Manufacture':'100', 'Food Processing':'101',
    'Meat Processing':'102', 'Seafood Processing':'103', 'Fruit and/or Vegetable Processing':'104', 'Oil-mill':'105',
    'Dairying':'106', 'Grain Milling':'107', 'Baking':'110', 'Sugar Manufacture':'111',
    'Sugar Milling':'112', 'Sugar Refining':'113', 'Confection Manufacture':'114', 'Pasta Manufacture':'115',
    'Prepared Meal Manufacture':'116', 'Animal Feed Manufacture':'119', 'Ice Manufacture':'120', 'Beverage Manufacture':'118',
    'Spirit Distillery':'121', 'Winery':'122', 'Brewing':'123', 'Soft Drink Manufacture':'124',
    'Tobacco Product Manufacture':'125', 'Textile, Apparel and Leather Manufacture':'129', 'Textile Manufacture':'130', 'Apparel Manufacture':'140',
    'Leather Product Manufacture':'150', 'Footwear Manufacturing':'155', 'Wood-based Manufacturing':'160', 'Sawmilling':'161',
    'Wooden Construction Product Manufacture':'165', 'Paper-mill':'171', 'Printing':'181', 'Petroleum and Coal Products Manufacturing':'190',
    'Petroleum Refining':'192', 'Coke Manufacture':'191', 'Chemical Manufacture':'195', 'Medicinal Product Manufacture':'210',
    'Rubber Product Manufacture':'221', 'Plastic Product Manufacture':'225', 'Nonmetallic Mineral Product Manufacturing':'230', 'Glass Product Manufacture':'231',
    'Refractory Product Manufacture':'232', 'Clay Product Manufacture':'233', 'Ceramic Product Manufacture':'234', 'Cement Mill':'235',
    'Cement Product Manufacture':'236', 'Stone Product Manufacture':'237', 'Primary Metal Manufacturing':'240', 'Steel Mill':'241',
    'Metal Refining':'242', 'Foundry':'243', 'Metal Product Manufacturing':'250', 'Structural Metal Product Manufacture':'251',
    'Fabricated Metal Product Manufacture':'257', 'Munitions Manufacture':'255', 'Electronic Equipment Manufacture':'260', 'Electrical Equipment Manufacture':'270',
    'Machinery Manufacture':'280', 'Transportation Equipment Manufacturing':'289', 'Motor Vehicle Manufacture':'290', 'Ship Construction':'301',
    'Railway Vehicle Manufacture':'304', 'Aircraft Manufacture':'305', 'Military Vehicle Manufacture':'306', 'Furniture Manufacture':'310',
    'Miscellaneous Manufacturing':'320', 'Jewellery Manufacture':'321', 'Musical Instrument Manufacture':'322', 'Sports Goods Manufacture':'323',
    'Game and/or Toy Manufacture':'324', 'Medical and/or Dental Equipment Manufacture':'325', 'General Repair':'330', 'Fabricated Metal Product Repair':'331',
    'Electronic Equipment Repair':'332', 'Electrical Equipment Repair':'333', 'Machinery Repair':'334', 'Railway Vehicle Repair':'342',
    'Motor Vehicle Repair':'343', 'Ship Repair':'340', 'Aircraft Repair':'341', 'Utilities':'350',
    'Power Generation':'351', 'Climate Control':'352', 'Cooling':'355', 'Heating':'356',
    'Water Supply':'360', 'Water Collection':'361', 'Water Treatment':'362', 'Water Distribution':'363',
    'Sewerage':'370', 'Sewerage Screening':'372', 'Restroom':'382', 'Waste Treatment and Disposal':'383',
    'Materials Recovery':'385', 'Commerce':'440', 'Wholesale Merchant':'459', 'Retail Sale':'460',
    'Specialized Store':'464', 'Petrol Sale':'470', 'Propane Sale':'272', 'Precious Metal Merchant':'474',
    'Pharmacy':'477', 'Pet-shop':'478', 'Non-specialized Store':'465', 'Convenience Store':'466',
    'Grocery':'476', 'Market':'475', 'Sales Yard':'473', 'Transport':'480',
    'Transportation Hub':'489', 'Station':'482', 'Stop':'483', 'Terminal':'481',
    'Transfer Hub':'484', 'Railway Transport':'490', 'Railway Passenger Transport':'491', 'Pedestrian Transport':'494',
    'Road Transport':'495', 'Road Freight Transport':'497', 'Road Passenger Transport':'496', 'Pipeline Transport':'500',
    'Pumping':'501', 'Water Transport':'505', 'Inland Waters Transport':'507', 'Canal Transport':'508',
    'Harbour Control':'513',
    'Port Control':'510', 'Maritime Pilotage':'511', 'Pilot Station':'512', 'Air Transport':'520',
    'Air Traffic Control':'525', 'Mail and Package Transport':'541', 'Postal Activities':'540', 'Courier Activities':'545',
    'Transportation Support':'529', 'Navigation':'488', 'Signalling':'486', 'Transport System Maintenance':'487',
    'Warehousing and Storage':'530', 'Cargo Handling':'536', 'Motor Vehicle Parking':'535', 'Inspection':'539',
    'Customs Checkpoint':'537', 'Inspection Station':'538', 'Accommodation':'550', 'Short-term Accommodation':'548',
    'Communal Bath':'559', 'Guest-house':'554', 'Hostel':'555', 'Hotel':'551',
    'Motel':'553', 'Resort':'552', 'Vacation Cottage':'557', 'Long-term Accommodation':'549',
    'Dormitory':'556', 'Dependents Housing':'558', 'Residence':'563', 'Food Service':'570',
    'Restaurant':'572', 'Bar':'573', 'Dining Hall':'574', 'Banquet Hall':'578',
    'Publishing and Broadcasting':'580', 'Print Publishing':'582', 'Radio Broadcasting':'601', 'Television Broadcasting':'604',
    'Information Service':'632', 'Public Records':'633', 'Telecommunications':'610', 'Retail Telecommunications':'612',
    'Wired Telecommunications':'614', 'Main Telephone Exchange':'615', 'Branch Telephone Exchange':'616', 'Wired Repeater':'617',
    'Wireless Telecommunications':'620', 'Mobile Phone Service':'621', 'Wireless Repeater':'622', 'Satellite Telecommunications':'625',
    'Satellite Ground Control':'626', 'Financial Services':'640', 'Central Banking':'642', 'Retail Banking':'643',
    'Insurance':'651', 'Financial Market Administration':'662', 'Security Brokerage':'663', 'Fund Management':'671',
    'Real Estate Activities':'680', 'Professional, Scientific and Technical':'681', 'Accounting':'696', 'Advertising':'741',
    'Architecture Consulting':'711', 'Business Management':'706', 'Engineering Design':'714', 'Head Office':'701',
    'Legal Activities':'691', 'Photography':'752', 'Scientific Research and Development':'720', 'Nuclear Research Centre':'725',
    'Observation Station':'721', 'Weather Station':'722', 'Wind Tunnel':'730', 'Surveying':'717',
    'Veterinary':'757', 'Business and Personal Support Services':'760', 'Administration':'810', 'Animal Boarding':'919',
    'Beauty Treatment':'962', 'Call Centre':'807', 'Custodial Service':'791', 'Employment Agency':'770',
    'Headquarters':'809', 'Landscaping Service':'795', 'Laundry':'961', 'Motor Vehicle Rental':'761',
    'Office Administration':'801', 'Travel Agency':'775', 'Public Administration':'808', 'Government':'811',
    'National Government':'814', 'Subnational Government':'813', 'Local Government':'812', 'Executive Activities':'818',
    'Legislative Activities':'819', 'Civil Activities':'822', 'Capitol':'817', 'Palace':'815',
    'Polling Station':'821', 'Diplomacy':'825', 'Consul':'828', 'Diplomatic Mission':'826',
    'Embassy':'827', 'Defence Activities':'835', 'Armory':'836', 'Maritime Defense':'829',
    'Military Recruitment':'838', 'Military Reserve Activities':'837', 'Public Order, Safety and Security Services':'830', 'Public Order':'831',
    'Immigration Control':'842', 'Imprisonment':'843', 'Judicial Activities':'840', 'Juvenile Corrections':'844',
    'Law Enforcement':'841', 'Safety':'832', 'Firefighting':'845', 'Rescue and Paramedical Services':'846',
    'Emergency Operations':'847', 'Emergency Relief Services':'888', 'Civil Intelligence':'848', 'CBRNE Civilian Support':'839',
    'Security Services':'833', 'Guard':'781', 'Security Enforcement':'780', 'Education':'850',
    'Primary Education':'851', 'Secondary Education':'852', 'Higher Education':'855', 'Vocational Education':'857',
    'Human Health Activities':'860', 'In-patient Care':'861', 'Intermediate Care':'871', 'Psychiatric In-patient Care':'873',
    'Residential Care':'875', 'Out-patient Care':'862', 'Urgent Medical Care':'863', 'Human Tissue Repository':'864',
    'Leprosy Care':'866', 'Public Health Activities':'865', 'Social Work':'887', 'Day Care':'885',
    'Emergency Shelter':'881', 'Emergency Youth Shelter':'884', 'Homeless Shelter':'882', 'Refugee Shelter':'883',
    'Cultural, Arts and Entertainment':'890', 'Aquarium':'906', 'Auditorium':'892', 'Botanical and/or Zoological Reserve Activities':'907',
    'Cinema':'594', 'Library':'902', 'Museum':'905', 'Night Club':'895',
    'Opera House':'894', 'Theatre':'891', 'Sports, Amusement and Recreation':'900', 'Adult Entertainment':'966',
    'Amusement':'922', 'Fitness Centre':'913', 'Gambling':'909', 'Hobbies and/or Leisure Activities':'923',
    'Recreation':'921', 'Shooting Range':'914', 'Sports Centre':'912', 'Religious Activities':'930',
    'Place of Worship':'931', 'Islamic Prayer Hall':'932', 'Membership Organization':'950', 'Club':'954',
    'Yacht-club':'955', 'Death Care Services':'980', 'Funeral Services':'963', 'Cremation':'964',
    'Mortuary Services':'965', 'Storage of Human Remains':'967', 'Meeting Place':'970', 'Community Centre':'893',
    'Convention Centre':'579'
}

# FFN For Facility
facility_FFN = {
    'No Information':'-999999', 'Other':'999', 'Agriculture':'2', 'Growing of Crops':'3',
    'Raising of Animals':'9', 'Grazing':'14', 'Mixed Farming':'15', 'Hunting':'19',
    'Forestry and/or Logging':'20', 'Silviculture':'21', 'Forest Warden':'27', 'Fishing':'30',
    'Aquaculture':'35', 'Mining and Quarrying':'40', 'Solid Mineral Fuel Mining':'50', 'Petroleum and/or Gas Extraction':'60',
    'Metal Ore Mining':'70', 'Chemical Mining':'83', 'Mineral Mining':'87', 'Gas Oil Separation':'91',
    'Ore Dressing':'95', 'Manufacturing':'99', 'Food Product Manufacture':'100', 'Food Processing':'101',
    'Meat Processing':'102', 'Seafood Processing':'103', 'Fruit and/or Vegetable Processing':'104', 'Oil-mill':'105',
    'Dairying':'106', 'Grain Milling':'107', 'Baking':'110', 'Sugar Manufacture':'111',
    'Sugar Milling':'112', 'Sugar Refining':'113', 'Confection Manufacture':'114', 'Pasta Manufacture':'115',
    'Prepared Meal Manufacture':'116', 'Animal Feed Manufacture':'119', 'Ice Manufacture':'120', 'Beverage Manufacture':'118',
    'Spirit Distillery':'121', 'Winery':'122', 'Brewing':'123', 'Soft Drink Manufacture':'124',
    'Tobacco Product Manufacture':'125', 'Textile, Apparel and Leather Manufacture':'129', 'Textile Manufacture':'130', 'Apparel Manufacture':'140',
    'Leather Product Manufacture':'150', 'Footwear Manufacturing':'155', 'Wood-based Manufacturing':'160', 'Sawmilling':'161',
    'Wooden Construction Product Manufacture':'165', 'Paper-mill':'171', 'Printing':'181', 'Petroleum and Coal Products Manufacturing':'190',
    'Petroleum Refining':'192', 'Coke Manufacture':'191', 'Chemical Manufacture':'195', 'Medicinal Product Manufacture':'210',
    'Rubber Product Manufacture':'221', 'Plastic Product Manufacture':'225', 'Nonmetallic Mineral Product Manufacturing':'230', 'Glass Product Manufacture':'231',
    'Refractory Product Manufacture':'232', 'Clay Product Manufacture':'233', 'Ceramic Product Manufacture':'234', 'Cement Mill':'235',
    'Cement Product Manufacture':'236', 'Stone Product Manufacture':'237', 'Primary Metal Manufacturing':'240', 'Steel Mill':'241',
    'Metal Refining':'242', 'Foundry':'243', 'Metal Product Manufacturing':'250', 'Structural Metal Product Manufacture':'251',
    'Fabricated Metal Product Manufacture':'257', 'Munitions Manufacture':'255', 'Electronic Equipment Manufacture':'260', 'Electrical Equipment Manufacture':'270',
    'Machinery Manufacture':'280', 'Transportation Equipment Manufacturing':'289', 'Motor Vehicle Manufacture':'290', 'Ship Construction':'301',
    'Railway Vehicle Manufacture':'304', 'Aircraft Manufacture':'305', 'Military Vehicle Manufacture':'306', 'Furniture Manufacture':'310',
    'Miscellaneous Manufacturing':'320', 'Jewellery Manufacture':'321', 'Musical Instrument Manufacture':'322', 'Sports Goods Manufacture':'323',
    'Game and/or Toy Manufacture':'324', 'Medical and/or Dental Equipment Manufacture':'325', 'General Repair':'330', 'Fabricated Metal Product Repair':'331',
    'Electronic Equipment Repair':'332', 'Electrical Equipment Repair':'333', 'Machinery Repair':'334', 'Railway Vehicle Repair':'342',
    'Motor Vehicle Repair':'343', 'Ship Repair':'340', 'Aircraft Repair':'341', 'Utilities':'350',
    'Power Generation':'351', 'Climate Control':'352', 'Cooling':'355', 'Heating':'356',
    'Water Supply':'360', 'Water Collection':'361', 'Water Treatment':'362', 'Water Distribution':'363',
    'Sewerage':'370', 'Sewerage Screening':'372', 'Restroom':'382', 'Materials Recovery':'385',
    'Commerce':'440', 'Wholesale Merchant':'459', 'Retail Sale':'460', 'Specialized Store':'464',
    'Petrol Sale':'470', 'Propane Sale':'272', 'Precious Metal Merchant':'474', 'Pharmacy':'477',
    'Pet-shop':'478', 'Non-specialized Store':'465', 'Convenience Store':'466', 'Grocery':'476',
    'Market':'475', 'Sales Yard':'473', 'Transport':'480', 'Transportation Hub':'489',
    'Station':'482', 'Stop':'483', 'Terminal':'481', 'Transfer Hub':'484',
    'Railway Transport':'490', 'Railway Passenger Transport':'491', 'Pedestrian Transport':'494', 'Road Transport':'495',
    'Road Freight Transport':'497', 'Road Passenger Transport':'496', 'Pipeline Transport':'500', 'Pumping':'501',
    'Water Transport':'505', 'Inland Waters Transport':'507', 'Canal Transport':'508', 'Harbour Control':'513',
    'Port Control':'510', 'Maritime Pilotage':'511', 'Pilot Station':'512', 'Air Transport':'520',
    'Air Traffic Control':'525', 'Mail and Package Transport':'541', 'Postal Activities':'540', 'Courier Activities':'545',
    'Transportation Support':'529', 'Navigation':'488', 'Signalling':'486', 'Transport System Maintenance':'487',
    'Warehousing and Storage':'530', 'Cargo Handling':'536', 'Motor Vehicle Parking':'535', 'Inspection':'539',
    'Customs Checkpoint':'537', 'Inspection Station':'538', 'Accommodation':'550', 'Short-term Accommodation':'548',
    'Communal Bath':'559', 'Guest-house':'554', 'Hostel':'555', 'Hotel':'551',
    'Motel':'553', 'Resort':'552', 'Vacation Cottage':'557', 'Long-term Accommodation':'549',
    'Dormitory':'556', 'Dependents Housing':'558', 'Residence':'563', 'Food Service':'570',
    'Restaurant':'572', 'Bar':'573', 'Dining Hall':'574', 'Banquet Hall':'578',
    'Publishing and Broadcasting':'580', 'Print Publishing':'582', 'Radio Broadcasting':'601', 'Television Broadcasting':'604',
    'Information Service':'632', 'Public Records':'633', 'Telecommunications':'610', 'Retail Telecommunications':'612',
    'Wired Telecommunications':'614', 'Main Telephone Exchange':'615', 'Branch Telephone Exchange':'616', 'Wired Repeater':'617',
    'Wireless Telecommunications':'620', 'Mobile Phone Service':'621', 'Wireless Repeater':'622', 'Satellite Telecommunications':'625',
    'Satellite Ground Control':'626', 'Financial Services':'640', 'Central Banking':'642', 'Retail Banking':'643',
    'Insurance':'651', 'Financial Market Administration':'662', 'Security Brokerage':'663', 'Fund Management':'671',
    'Real Estate Activities':'680', 'Professional, Scientific and Technical':'681', 'Accounting':'696', 'Advertising':'741',
    'Architecture Consulting':'711', 'Business Management':'706', 'Engineering Design':'714', 'Head Office':'701',
    'Legal Activities':'691', 'Photography':'752', 'Scientific Research and Development':'720', 'Nuclear Research Centre':'725',
    'Observation Station':'721', 'Weather Station':'722', 'Wind Tunnel':'730', 'Surveying':'717',
    'Veterinary':'757', 'Business and Personal Support Services':'760', 'Administration':'810', 'Animal Boarding':'919',
    'Beauty Treatment':'962', 'Call Centre':'807', 'Custodial Service':'791', 'Employment Agency':'770',
    'Headquarters':'809', 'Landscaping Service':'795', 'Laundry':'961', 'Motor Vehicle Rental':'761',
    'Office Administration':'801', 'Travel Agency':'775', 'Public Administration':'808', 'Government':'811',
    'National Government':'814', 'Subnational Government':'813', 'Local Government':'812', 'Executive Activities':'818',
    'Legislative Activities':'819', 'Civil Activities':'822', 'Capitol':'817', 'Palace':'815',
    'Polling Station':'821', 'Diplomacy':'825', 'Consul':'828', 'Diplomatic Mission':'826',
    'Embassy':'827', 'Defence Activities':'835', 'Armory':'836', 'Maritime Defense':'829',
    'Military Recruitment':'838', 'Military Reserve Activities':'837', 'Public Order, Safety and Security Services':'830', 'Public Order':'831',
    'Immigration Control':'842', 'Imprisonment':'843', 'Judicial Activities':'840', 'Juvenile Corrections':'844',
    'Law Enforcement':'841', 'Safety':'832', 'Firefighting':'845', 'Rescue and Paramedical Services':'846',
    'Emergency Operations':'847', 'Emergency Relief Services':'888', 'Civil Intelligence':'848', 'CBRNE Civilian Support':'839',
    'Security Services':'833', 'Guard':'781', 'Security Enforcement':'780', 'Education':'850',
    'Primary Education':'851', 'Secondary Education':'852', 'Higher Education':'855', 'Vocational Education':'857',
    'Human Health Activities':'860', 'In-patient Care':'861', 'Intermediate Care':'871', 'Psychiatric In-patient Care':'873',
    'Residential Care':'875', 'Out-patient Care':'862', 'Urgent Medical Care':'863', 'Human Tissue Repository':'864',
    'Leprosy Care':'866', 'Public Health Activities':'865', 'Social Work':'887', 'Day Care':'885',
    'Emergency Shelter':'881', 'Emergency Youth Shelter':'884', 'Homeless Shelter':'882', 'Refugee Shelter':'883',
    'Cultural, Arts and Entertainment':'890', 'Aquarium':'906', 'Auditorium':'892', 'Botanical and/or Zoological Reserve Activities':'907',
    'Cinema':'594', 'Library':'902', 'Museum':'905', 'Night Club':'895',
    'Opera House':'894', 'Theatre':'891', 'Sports, Amusement and Recreation':'900', 'Adult Entertainment':'966',
    'Amusement':'922', 'Fitness Centre':'913', 'Gambling':'909', 'Hobbies and/or Leisure Activities':'923',
    'Recreation':'921', 'Shooting Range':'914', 'Sports Centre':'912', 'Water Park':'915',
    'Religious Activities':'930', 'Place of Worship':'931', 'Islamic Prayer Hall':'932', 'Membership Organization':'950',
    'Club':'954', 'Yacht-club':'955', 'Death Care Services':'980', 'Funeral Services':'963',
    'Cremation':'964', 'Mortuary Services':'965', 'Storage of Human Remains':'967', 'Meeting Place':'970',
    'Community Centre':'893', 'Convention Centre':'579'
}

# Cell Partitioning Scheme
text_CPS = {
    'Fixed Cells, 0.25 Arc Degree':'fixed0r25',
    'Fixed Cells, 0.5 Arc Degree':'fixed0r5',
    'Fixed Cells, 1 Arc Degree':'fixed1r0',
    'Fixed Cells, 5 Arc Degree':'fixed5r0',
    'Variable Cells':'variable'
}

# Equivalent Scale
text_EQC = {
    '1:5,000,000':'scale5m',
    '1:2,000,000':'scale2m',
    '1:1,000,000':'scale1m',
    '1:500,000':'scale500k',
    '1:250,000':'scale250k',
    '1:100,000':'scale100k',
    '1:50,000':'scale50k',
    '1:25,000':'scale25k',
    '1:12,500':'scale12r5k',
    '1:5,000':'scale5k'
}

# Extraction Specification
text_ETS = {
    '1AA-TPC':'tpc',
    '1AB-ONC':'onc',
    '1AE-JOG-A/G':'jogAirGround',
    '1CD-DTED-1':'dted1',
    '1CE-DFAD-1':'dfad1',
    '1CF-DTED-2':'dted2',
    '1CG-DFAD-2':'dfad2',
    '2AA/001-HAC-1':'hac1',
    '2AA/002-HAC-2':'hac2',
    '2AA/003-HAC-3':'hac3',
    '2AA/004-HAC-4':'hac4',
    '2AA/005-HAC-5':'hac5',
    '2AA/006-HAC-6':'hac6',
    '2AA/007-HAC-7':'hac7',
    '2AA/008-HAC-8':'hac8',
    '2AA/009-HAC-9':'hac9',
    '2AD-Combat':'combat',
    '3AA-TLM50':'tlm50',
    '3AG-TLM100':'tlm100',
    '3KA-VITD':'vitd',
    '3KC/001-DTOP':'dtop',
    '3KH-VMap-2':'vmap2',
    '3KL-VMap-0':'vmap0',
    '3KM-VMap-1':'vmap1',
    '3KU-UVMap':'uvmap',
    '4AA-ATC':'atc',
    '4AC-JOG-R':'jogRadar',
    '4GE-TERCOM-L':'tercomL',
    '4GF-TERCOM-E':'tercomE',
    '4GG-TERCOM-T':'tercomT',
    '5EE-FFD':'ffd',
    'DFEG':'digitalFeg',
    'DNC':'dnc',
    'GTDS-EG':'globalTdsEg',
    'LTDS-EG':'localTdsEg',
    'MGCP TRD':'mgcpTrd',
    'MSD1':'msd1',
    'MSD2':'msd2',
    'MSD3':'msd3',
    'MSD4':'msd4',
    'MSD5':'msd5',
    'RTDS-EG':'regionalTdsEg',
    'S-UTDS-EG':'specUrbanTdsEg',
    'TOD0':'tod0',
    'TOD1':'tod1',
    'TOD2':'tod2',
    'TOD3':'tod3',
    'TOD4':'tod4'
}

# Geodetic Datum
text_HZD = {
    'World Geodetic System 1984':'worldGeodeticSystem1984',
    'Adindan (Burkina Faso)':'adindanBurkinaFaso',
    'Adindan (Cameroon)':'adindanCameroon',
    'Adindan (Ethiopia)':'adindanEthiopia',
    'Adindan (Mali)':'adindanMali',
    'Adindan (mean value)':'adindanMeanValue',
    'Adindan (Senegal)':'adindanSenegal',
    'Adindan (Sudan)':'adindanSudan',
    'Afgooye (Somalia)':'afgooyeSomalia',
    'Ain el Abd 1970 (Bahrain Island)':'ainelAbd1970BahrainIsland',
    'Ain el Abd 1970 (Saudi Arabia)':'ainelAbd1970SaudiArabia',
    'American Samoa Datum 1962':'americanSamoaDatum1962',
    'Amersfoort 1885/1903 (Netherlands)':'amersfoort1885dash1903',
    'Anna 1 Astro 1965 (Cocos Islands)':'anna1Astro1965CocosIslands',
    'Antigua Island Astro 1943':'antiguaIslandAstro1943',
    'Approximate Luzon Datum (Philippines)':'approximateLuzonDatum',
    'Arc 1935 (Africa)':'arc1935Africa',
    'Arc 1950 (Botswana)':'arc1950Botswana',
    'Arc 1950 (Burundi)':'arc1950Burundi',
    'Arc 1950 (Lesotho)':'arc1950Lesotho',
    'Arc 1950 (Malawi)':'arc1950Malawi',
    'Arc 1950 (mean value)':'arc1950MeanValue',
    'Arc 1950 (Swaziland)':'arc1950Swaziland',
    'Arc 1950 (Zaire)':'arc1950Zaire',
    'Arc 1950 (Zambia)':'arc1950Zambia',
    'Arc 1950 (Zimbabwe)':'arc1950Zimbabwe',
    'Arc 1960 (Kenya)':'arc1960Kenya',
    'Arc 1960 (mean value)':'arc1960MeanValue',
    'Arc 1960 (Tanzania)':'arc1960Tanzania',
    'Ascension Island 1958 (Ascension Island)':'ascensionIsland1958',
    'Astro Beacon E (Iwo Jima Island)':'astroBeaconEIwoJimaIsland',
    'Astro DOS 71/4 (St. Helena Island)':'astroDos71dash4StHelena',
    'Astro Station 1952 (Marcus Island)':'astroStation1952MarcusIs',
    'Astro Tern Island 1961 (Tern Island, Hawaii)':'astroTernIsland1961Hawaii',
    'Australian Geod. 1966 (Australia and Tasmania Island)':'australianGeodetic1966',
    'Australian Geod. 1984 (Australia and Tasmania Island)':'australianGeodetic1984',
    'Average Terrestrial System 1977, New Brunswick':'averageTerrestrialSys1977',
    'Ayabelle Lighthouse (Djibouti)':'ayabelleLighthouseDjibouti',
    'Bekaa Base South End (Lebanon)':'bekaaBaseSouthEndLebanon',
    'Belgium 1950 System (Lommel Signal, Belgium)':'belgium1950SystemLommelSig',
    'Belgium 1972 (Observatoire dUccle)':'belgium1972Observatoire',
    'Bellevue (IGN) (Efate and Erromango Islands)':'bellevueIgnEfateErromango',
    'Bermuda 1957 (Bermuda Islands)':'bermuda1957BermudaIslands',
    'Bern 1898 (Switzerland)':'bern1898Switzerland',
    'Bern 1898 (Switzerland) with Zero Meridian Bern':'bern1898ZeroMeridian',
    'Bissau (Guinea-Bissau)':'bissauGuineaBissau',
    'BJZ54 (A954 Beijing Coordinates) (China)':'bjz54A954BeijingCoord',
    'Bogota Observatory (Colombia)':'bogotaObservatoryColombia',
    'Bogota Observatory (Colombia) with Zero Meridian Bogota':'bogotaObsZeroMeridian',
    'Bukit Rimpah (Bangka and Belitung Islands, Indonesia)':'bukitRimpahIndonesia',
    'Camacupa Base SW End (Campo De Aviacao, Angola)':'camacupaBaseSwEndAngola',
    'Camp Area Astro (Camp McMurdo Area, Antarctica)':'campAreaAstroAntarctica',
    'Campo Inchauspe (Argentina)':'campoInchauspeArgentina',
    'Canton Astro 1966 (Phoenix Islands)':'cantonAstro1966PhoenixIs',
    'Cape (South Africa)':'capeSouthAfrica',
    'Cape Canaveral (mean value)':'capeCanaveralMeanValue',
    'Carthage (Tunisia)':'carthageTunisia',
    'Chatham 1971 (Chatham Island, New Zealand)':'chatham1971NewZealand',
    'Chua Astro (Paraguay)':'chuaAstroParaguay',
    'Compensation Geodetique du Quebec 1977':'compensationGeoQuebec1977',
    'Conakry Pyramid of the Service Geographique (Guinea)':'conakryPyramidGuinea',
    'Co-ordinate System 1937 of Estonia':'estonia1937',
    'Corrego Alegre (Brazil)':'corregoAlegreBrazil',
    'Dabola (Guinea)':'dabolaGuinea',
    'DCS-3 Lighthouse, Saint Lucia, Lesser Antilles':'dcs3LighthouseLesserAnt',
    'Deception Island, Antarctica':'deceptionIslAntarctica',
    'Djakarta (Batavia) (Sumatra Island, Indonesia)':'djakartaBataviaIndonesia',
    'Djakarta (Batavia) (Sumatra Island, Indonesia) with Zero Meridian Djakarta':'djakartaBataviaZeroMerid',
    'Dominica Astro M-12, Dominica, Lesser Antilles':'dominicaAstroM12LesserAnt',
    'DOS 1968 (Gizo Island, New Georgia Islands)':'dos1968GizoNewGeorgiaIs',
    'Easter Island 1967 (Easter Island)':'easterIsland1967EasterIs',
    'European 1950 (British Isles)':'european1950BritishIsles',
    'European 1950 (Cyprus)':'european1950Cyprus',
    'European 1950 (Egypt)':'european1950Egypt',
    'European 1950 (England, Channel Islands, Scotland, and Shetland Islands)':'european1950England',
    'European 1950 (Greece)':'european1950Greece',
    'European 1950 (Iran)':'european1950Iran',
    'European 1950 (Iraq, Israel, Jordan, Kuwait, Lebanon, Saudi Arabia, and Syria)':'european1950IraqSyria',
    'European 1950 (Malta)':'european1950Malta',
    'European 1950 (mean value)':'european1950MeanValue',
    'European 1950 (Norway and Finland)':'european1950NorwayFinland',
    'European 1950 (Portugal and Spain)':'european1950PortugalSpain',
    'European 1950 (Sardinia)':'european1950Sardinia',
    'European 1950 (Sicily)':'european1950Sicily',
    'European 1950 (Tunisia)':'european1950Tunisia',
    'European 1950 (Western Europe)':'european1950WesternEurope',
    'European 1979 (mean value)':'european1979MeanValue',
    'European Terrestrial Reference System 1989 (ETRS89)':'etrs1989',
    'Fort Thomas 1955 (Nevis, St Kitts, Leeward Islands)':'fortThomas1955LeewardIs',
    'Gan 1970 (Addu Atoll, Republic of Maldives)':'gan1970AdduAtoll',
    'Gandajika Base (Zaire)':'gandajikaBaseZaire',
    'GDZ80 (China)':'gdz80China',
    'Geocentric Datum of Australia (GDA)':'geocentricDatumOfAustralia',
    'Geodetic Datum 1949 (New Zealand)':'geodeticDatum1949Zealand',
    'Graciosa Base SW (Faial, Graciosa, Pico, Sao Jorge, and Terceira Island, Azores)':'graciosaBaseSWFaialAzores',
    'Greek Datum, Greece':'greekDatumGreece',
    'Greek Geodetic Reference System 1987 (GGRS 87)':'greekGeodeticRefSystem1987',
    'Guam 1963':'guam1963',
    'Gunong Segara (Kalimantan Island, Indonesia)':'gunongSegaraKalimantanIs',
    'Gunong Serindung':'gunongSerindung',
    'GUX 1 Astro (Guadacanal Island)':'gux1AstroGuadacanalIs',
    'Guyana CSG67':'guyanaCSG67',
    'Herat North (Afganistan)':'heratNorthAfganistan',
    'Hermannskogel':'hermannskogel',
    'Hjorsey 1955 (Iceland)':'hjorsey1955Iceland',
    'Hong Kong 1929':'hongKong1929',
    'Hong Kong 1963 (Hong Kong)':'hongKong1963HongKong',
    'Hungarian 1972':'hungarian1972',
    'Hu-Tzu-Shan':'huTzuShan',
    'Indian (Bangladesh)':'indianBangladesh',
    'Indian (India and Nepal)':'indianIndiaNepal',
    'Indian (Pakistan)':'indianPakistan',
    'Indian (Thailand and Vietnam)':'indianThailandVietnam',
    'Indian 1954 (Thailand)':'indian1954Thailand',
    'Indian 1960 (Con Son Island (Vietnam))':'indian1960ConSonIsland',
    'Indian 1960 (Vietnam: near 16 degrees North)':'indian1960Vietnam',
    'Indian 1975 (Thailand)':'indian1975Thailand',
    'Indian 1975 (Thailand) - Cycle 1':'indian1975ThailandCycle1',
    'Indonesian 1974':'indonesian1974',
    'Ireland 1965 (Ireland and Northern Ireland)':'ireland1965IrelandNorthern',
    'ISTS 061 Astro 1968 (South Georgia Islands)':'ists061Astro1968GeorgiaIs',
    'ISTS 073 Astro 1969 (Diego Garcia)':'ists073Astro1969DiegoGar',
    'Johnston Island 1961 (Johnston Island)':'johnstonIsland1961',
    'Kalianpur (India)':'kalianpurIndia',
    'Kandawala (Sri Lanka)':'kandawalaSriLanka',
    'KCS 2, Sierra Leone':'kcs2SierraLeone',
    'Kerguelen Island 1949 (Kerguelen Island)':'kerguelenIsland1949',
    'Kertau 1948 (or Revised Kertau) (West Malaysia and Singapore)':'kertau1948RevisedMalaysia',
    'KKJ (or Kartastokoordinaattijarjestelma), Finland':'kkjFinland',
    'Korean Geodetic System 1995 (South Korea)':'koreanGeodeticSystem1995',
    'Kusaie Astro 1951':'kusaieAstro1951',
    'Kuwait Oil Company (K28)':'kuwaitOilCompanyK28',
    'L.C. 5 Astro 1961 (Cayman Brac Island)':'lc5Astro1961CaymanBracIs',
    'Leigon (Ghana)':'leigonGhana',
    'Liberia 1964 (Liberia)':'liberia1964Liberia',
    'Lisbon (Castelo di Sao Jorge), Portugal':'lisbonCastelodiSaoJorge',
    'Local Astro':'localAstro',
    'Loma Quintana (Venezuela)':'lomaQuintanaVenezuela',
    'Luzon (Mindanao Island)':'luzonMindanaoIsland',
    'Luzon (Philipines except Mindanao Island)':'luzonPhilipinesNotMindanao',
    'Mahe 1971 (Mahe Island)':'mahe1971MaheIsland',
    'Manokwari (West Irian)':'manokwariWestIrian',
    'Marco Astro (Salvage Islands)':'marcoAstroSalvageIslands',
    'Martinique Fort-Desaix':'martiniqueFortDesaix',
    'Massawa (Eritrea, Ethiopia)':'massawEritreaEthiopia',
    'Mayotte Combani':'mayotteCombani',
    'Merchich (Morocco)':'merchichMorocco',
    'Midway Astro 1961 (Midway Island)':'midwayAstro1961MidwayIs',
    'Minna (Cameroon)':'minnaCameroon',
    'Minna (Nigeria)':'minnaNigeria',
    'Modified BJZ54 (China)':'modifiedBJZ54China',
    'Montjong Lowe':'montjongLowe',
    'Montserrat Island Astro 1958':'montserratIslandAstro1958',
    'Mount Dillon, Tobago':'mountDillonTobago',
    'MPoraloko (Gabon)':'mPoralokoGabon',
    'Nahrwan (Masirah Island, Oman)':'nahrwanMasirahIslandOman',
    'Nahrwan (Saudi Arabia)':'nahrwanSaudiArabia',
    'Nahrwan (United Arab Emirates)':'nahrwanUnitedArabEmirates',
    'Naparima (BWI, Trinidad and Tobago)':'naparimaBwiTrinidadTobago',
    'New French or Nouvelle Triangulation Francaise (NTF) with Zero Meridian Paris':'newFrenchZeroMeridianParis',
    'North American 1927 (Alaska)':'northAm1927Alaska',
    'North American 1927 (Alberta and British Columbia)':'northAm1927Alberta',
    'North American 1927 (Aleutian Islands East of 180 degrees West)':'northAm1927AleutianE',
    'North American 1927 (Aleutian Islands West of 180 degrees West)':'northAm1927AleutianW',
    'North American 1927 (Bahamas, except San Salvador Island)':'northAm1927Bahamas',
    'North American 1927 (Canada mean)':'northAm1927CanadMean',
    'North American 1927 (Canal Zone)':'northAm1927CanalZone',
    'North American 1927 (Caribbean)':'northAm1927Caribbean',
    'North American 1927 (Central America)':'northAm1927CentAmer',
    'North American 1927 (CONUS mean)':'northAm1927ConusMean',
    'North American 1927 (Cuba)':'northAm1927Cuba',
    'North American 1927 (Eastern US)':'northAm1927EasternUs',
    'North American 1927 (Hayes Peninsula, Greenland)':'northAm1927HayesPen',
    'North American 1927 (Manitoba and Ontario)':'northAm1927Manitoba',
    'North American 1927 (Mexico)':'northAm1927Mexico',
    'North American 1927 (Newfoundland, New Brunswick, Nova Scotia and Quebec)':'northAm1927Newfound',
    'North American 1927 (Northwest Territories and Saskatchewan)':'northAm1927NwTerSaskat',
    'North American 1927 (San Salvador Island)':'northAm1927Salvador',
    'North American 1927 (Western US)':'northAm1927WesternUs',
    'North American 1927 (Yukon)':'northAm1927Yukon',
    'North American 1983 (Alaska, excluding Aleutian Islands)':'northAm1983AlaskaExAleut',
    'North American 1983 (Aleutian Islands)':'northAm1983Aleutian',
    'North American 1983 (Canada)':'northAm1983Canada',
    'North American 1983 (CONUS)':'northAm1983Conus',
    'North American 1983 (Hawaii)':'northAm1983Hawaii',
    'North American 1983 (Mexico and Central America))':'northAm1983Mexico',
    'North Sahara 1959':'northSahara1959',
    'Observatorio Meteorologico 1939 (Corvo and Flores Islands, Azores)':'observMeteorologico1939',
    'Ocotopeque, Guatemala':'ocotopequeGuatemala',
    'Old Egyptian (Egypt)':'oldEgyptian',
    'Old Hawaiian (Hawaii)':'oldHawaiianHawaiiIsland',
    'Old Hawaiian (Kauai)':'oldHawaiianKauaiIsland',
    'Old Hawaiian (Maui)':'oldHawaiianMauiIsland',
    'Old Hawaiian (mean value)':'oldHawaiianMeanValue',
    'Old Hawaiian (Oahu)':'oldHawaiianOahuIsland',
    'Oman (Oman)':'oman',
    'Ordnance Survey G.B. 1936 (England)':'ordnanceSurvGB1936England',
    'Ordnance Survey G.B. 1936 (England, Isle of Man, and Wales)':'ordnanceSurvGB1936ScotWale',
    'Ordnance Survey G.B. 1936 (mean value)':'ordnanceSurvGB1936MeanVal',
    'Ordnance Survey G.B. 1936 (Scotland and Shetland Islands)':'ordnanceSurvGB1936ScotShet',
    'Ordnance Survey G.B. 1936 (Wales)':'ordnanceSurvGB1936Wales',
    'Oslo Observatory (Old), Norway':'osloObservatoryOld',
    'Padang Base West End (Sumatra, Indonesia)':'padangBaseWestEnd',
    'Padang Base West End (Sumatra, Indonesia) with Zero Meridian Djakarta':'padangBaseWestEndZeroMerid',
    'Palestine 1928 (Israel, Jordan)':'palestine1928',
    'Pico de las Nieves (Canary Islands)':'picoDeLasNievesCanaryIs',
    'Pitcairn Astro 1967 (Pitcairn Island)':'pitcairnAstro1967',
    'Point 58 Mean Solution (Burkina Faso and Niger)':'point58MeanSolution',
    'Pointe Noire 1948':'pointeNoire1948',
    'Potsdam or Helmertturm (Germany)':'potsdamHelmertturmGermany',
    'Prov. S. American 1956 (Bolivia)':'provSouthAm1956Bolivia',
    'Prov. S. American 1956 (Columbia)':'provSouthAm1956Columbia',
    'Prov. S. American 1956 (Ecuador)':'provSouthAm1956Ecuador',
    'Prov. S. American 1956 (Guyana)':'provSouthAm1956Guyana',
    'Prov. S. American 1956 (mean value)':'provSouthAm1956MeanValue',
    'Prov. S. American 1956 (Northern Chile near 19 degrees South)':'provSouthAm1956NChile',
    'Prov. S. American 1956 (Peru)':'provSouthAm1956Peru',
    'Prov. S. American 1956 (Southern Chile near 43 degrees South)':'provSouthAm1956SChile',
    'Prov. S. American 1956 (Venezuela)':'provSouthAm1956Venezuela',
    'Provisional South Chilean 1963 (or Hito XVIII 1963) (S. Chile, 53 degrees South)':'provSouthChilean1963',
    'Puerto Rico (Puerto Rico and Virgin Islands)':'puertoRicoVirginIslands',
    'Pulkovo 1942 (Albania)':'pulkovo1942Albania',
    'Pulkovo 1942 (Czechoslovakia)':'pulkovo1942Czechoslovakia',
    'Pulkovo 1942 (Hungary)':'pulkovo1942Hungary',
    'Pulkovo 1942 (Kazakhstan)':'pulkovo1942Kazakhstan',
    'Pulkovo 1942 (Latvia)':'pulkovo1942Latvia',
    'Pulkovo 1942 (Poland)':'pulkovo1942Poland',
    'Pulkovo 1942 (Romania)':'pulkovo1942Romania',
    'Pulkovo 1942 (Russia)':'pulkovo1942Russia',
    'Qatar National (Qatar)':'qatarNationalQatar',
    'Qornoq (South Greenland)':'qornoqSouthGreenland',
    'Rauenberg (Berlin, Germany)':'rauenbergBerlinGermany',
    'Reconnaissance Triangulation, Morocco':'reconTriangulationMorocco',
    'Reunion 1947':'reunion1947',
    'Revised Nahrwan':'revisedNahrwan',
    'Rome 1940 (or Monte Mario 1940), Italy':'rome1940',
    'Rome 1940 (or Monte Mario 1940), Italy, with Zero Meridian Rome':'rome1940ZeroMeridianRome',
    'RT90, Stockholm, Sweden':'rt90StockholmSweden',
    'Sainte Anne I 1984 (Guadeloupe)':'sainteAnneI1984Guadeloupe',
    'Santo (DOS) 1965 (Espirito Santo Island)':'santoDos1965EspiritoSanto',
    'Sao Braz (Sao Miguel, Santa Maria Islands, Azores)':'saoBrazSaoMiguelAzores',
    'Sapper Hill 1943 (East Falkland Islands)':'sapperHill1943EastFalkland',
    'Schwarzeck (Namibia)':'schwarzeckNamibia',
    'SE Base (Porto Santo) (Porto Santo and Madeira Islands)':'seBasePortoSanto',
    'Selvagem Grande 1938 (Salvage Islands)':'selvagemGrande1938Salvage',
    'Sierra Leone 1960':'sierraLeone1960',
    'S-JTSK':'sJtsk',
    'South African (South Africa)':'southAfricanSouthAfrica',
    'South American 1969 (Argentina)':'southAmerican1969Argentina',
    'South American 1969 (Baltra, Galapagos Islands)':'southAmerican1969BaltraIs',
    'South American 1969 (Bolivia)':'southAmerican1969Bolivia',
    'South American 1969 (Brazil)':'southAmerican1969Brazil',
    'South American 1969 (Chile)':'southAmerican1969Chile',
    'South American 1969 (Columbia)':'southAmerican1969Columbia',
    'South American 1969 (Ecuador)':'southAmerican1969Ecuador',
    'South American 1969 (Guyana)':'southAmerican1969Guyana',
    'South American 1969 (mean value)':'southAmerican1969MeanValue',
    'South American 1969 (Paraguay)':'southAmerican1969Paraguay',
    'South American 1969 (Peru)':'southAmerican1969Peru',
    'South American 1969 (Trinidad and Tobago)':'southAmerican1969Trinidad',
    'South American 1969 (Venezuela)':'southAmerican1969Venezuela',
    'South American Geocentric Reference System (SIRGAS)':'sirgas',
    'South Asia (Southeast Asia, Singapore)':'southAsiaSingapore',
    'Soviet Geodetic System 1985':'sovietGeodeticSystem1985',
    'Soviet Geodetic System 1990':'sovietGeodeticSystem1990',
    'St. Pierre et Miquelon 1950':'stPierreetMiquelon1950',
    'Stockholm 1938 (Sweden)':'stockholm1938Sweden',
    'Sydney Observatory, New South Wales, Australia':'sydneyObservatoryNewSouth',
    'Tananarive Observatory 1925':'tananariveObservatory1925',
    'Tananarive Observatory 1925, with Zero Meridian Paris':'tananariveObs1925ZerMerPar',
    'Timbalai 1948 (Brunei and East Malaysia - Sarawak and Sabah)':'timbalai1948BruneiMalaysia',
    'Timbalai 1968':'timbalai1968',
    'Tokyo (Japan)':'tokyoJapan',
    'Tokyo (Korea)':'tokyoKorea',
    'Tokyo (Korea) - Cycle 1':'tokyoKoreaCycle1',
    'Tokyo (mean value)':'tokyoMeanValue',
    'Tokyo (Okinawa)':'tokyoOkinawa',
    'Trinidad 1903':'trinidad1903',
    'Tristan Astro 1968 (Tristan da Cunha)':'tristanAstro1968Cunha',
    'Viti Levu 1916 (Viti Levu Island, Fiji Islands)':'vitiLevu1916FijiIslands',
    'Voirol 1875':'voirol1875',
    'Voirol 1875 with Zero Meridian Paris':'voirol1875ZeroMeridParis',
    'Voirol 1960, Algeria':'voirol1960Algeria',
    'Voirol 1960, Algeria, with Zero Meridian Paris':'voirol1960ZeroMeridParis',
    'Wake Island Astro 1952':'wakeIslandAstro1952',
    'Wake-Eniwetok 1960 (Marshall Islands)':'wakeEniwetok1960MarshallIs',
    'World Geodetic System 1960':'worldGeodeticSystem1960',
    'World Geodetic System 1966':'worldGeodeticSystem1966',
    'World Geodetic System 1972':'worldGeodeticSystem1972',
    'Yacare (Uruguay)':'yacareUruguay',
    'Zanderij (Surinam)':'zanderijSurinam'
}

# ISO Country Code
text_IC2 = {
    'UNITED STATES':'USA',
    'ARUBA':'ABW',
    'AFGHANISTAN':'AFG',
    'ANGOLA':'AGO',
    'ANGUILLA':'AIA',
    'ALAND ISLANDS':'ALA',
    'ALBANIA':'ALB',
    'ANDORRA':'AND',
    'NETHERLANDS ANTILLES':'ANT',
    'UNITED ARAB EMIRATES':'ARE',
    'ARGENTINA':'ARG',
    'ARMENIA':'ARM',
    'AMERICAN SAMOA':'ASM',
    'ANTARCTICA':'ATA',
    'FRENCH SOUTHERN TERRITORIES':'ATF',
    'ANTIGUA AND BARBUDA':'ATG',
    'AUSTRALIA':'AUS',
    'AUSTRIA':'AUT',
    'AZERBAIJAN':'AZE',
    'BURUNDI':'BDI',
    'BELGIUM':'BEL',
    'BENIN':'BEN',
    'BURKINA FASO':'BFA',
    'BANGLADESH':'BGD',
    'BULGARIA':'BGR',
    'BAHRAIN':'BHR',
    'BAHAMAS':'BHS',
    'BOSNIA AND HERZEGOVINA':'BIH',
    'SAINT BARTHELEMY':'BLM',
    'BELARUS':'BLR',
    'BELIZE':'BLZ',
    'BERMUDA':'BMU',
    'BOLIVIA':'BOL',
    'BRAZIL':'BRA',
    'BARBADOS':'BRB',
    'BRUNEI DARUSSALAM':'BRN',
    'BHUTAN':'BTN',
    'BOUVET ISLAND':'BVT',
    'BOTSWANA':'BWA',
    'CENTRAL AFRICAN REPUBLIC':'CAF',
    'CANADA':'CAN',
    'COCOS (KEELING) ISLANDS':'CCK',
    'SWITZERLAND':'CHE',
    'CHILE':'CHL',
    'CHINA':'CHN',
    'COTE DIVOIRE':'CIV',
    'CAMEROON':'CMR',
    'CONGO, THE DEMOCRATIC REPUBLIC OF THE':'COD',
    'CONGO':'COG',
    'COOK ISLANDS':'COK',
    'COLOMBIA':'COL',
    'COMOROS':'COM',
    'CAPE VERDE':'CPV',
    'COSTA RICA':'CRI',
    'CUBA':'CUB',
    'CHRISTMAS ISLAND':'CXR',
    'CAYMAN ISLANDS':'CYM',
    'CYPRUS':'CYP',
    'CZECH REPUBLIC':'CZE',
    'GERMANY':'DEU',
    'DJIBOUTI':'DJI',
    'DOMINICA':'DMA',
    'DENMARK':'DNK',
    'DOMINICAN REPUBLIC':'DOM',
    'ALGERIA':'DZA',
    'ECUADOR':'ECU',
    'EGYPT':'EGY',
    'ERITREA':'ERI',
    'WESTERN SAHARA':'ESH',
    'SPAIN':'ESP',
    'ESTONIA':'EST',
    'ETHIOPIA':'ETH',
    'FINLAND':'FIN',
    'FIJI':'FJI',
    'FALKLAND ISLANDS (MALVINAS)':'FLK',
    'FRANCE':'FRA',
    'FAROE ISLANDS':'FRO',
    'MICRONESIA, FEDERATED STATES OF':'FSM',
    'GABON':'GAB',
    'UNITED KINGDOM':'GBR',
    'GEORGIA':'GEO',
    'GUERNSEY':'GGY',
    'GHANA':'GHA',
    'GIBRALTAR':'GIB',
    'GUINEA':'GIN',
    'GUADELOUPE':'GLP',
    'GAMBIA':'GMB',
    'GUINEA-BISSAU':'GNB',
    'EQUATORIAL GUINEA':'GNQ',
    'GREECE':'GRC',
    'GRENADA':'GRD',
    'GREENLAND':'GRL',
    'GUATEMALA':'GTM',
    'FRENCH GUIANA':'GUF',
    'GUAM':'GUM',
    'GUYANA':'GUY',
    'HONG KONG':'HKG',
    'HEARD ISLAND AND MCDONALD ISLANDS':'HMD',
    'HONDURAS':'HND',
    'CROATIA':'HRV',
    'HAITI':'HTI',
    'HUNGARY':'HUN',
    'INDONESIA':'IDN',
    'ISLE OF MAN':'IMN',
    'INDIA':'IND',
    'BRITISH INDIAN OCEAN TERRITORY':'IOT',
    'IRELAND':'IRL',
    'IRAN, ISLAMIC REPUBLIC OF':'IRN',
    'IRAQ':'IRQ',
    'ICELAND':'ISL',
    'ISRAEL':'ISR',
    'ITALY':'ITA',
    'JAMAICA':'JAM',
    'JERSEY':'JEY',
    'JORDAN':'JOR',
    'JAPAN':'JPN',
    'KAZAKHSTAN':'KAZ',
    'KENYA':'KEN',
    'KYRGYZSTAN':'KGZ',
    'CAMBODIA':'KHM',
    'KIRIBATI':'KIR',
    'SAINT KITTS AND NEVIS':'KNA',
    'KOREA, REPUBLIC OF':'KOR',
    'KUWAIT':'KWT',
    'LAO PEOPLES DEMOCRATIC REPUBLIC':'LAO',
    'LEBANON':'LBN',
    'LIBERIA':'LBR',
    'LIBYAN ARAB JAMAHIRIYA':'LBY',
    'SAINT LUCIA':'LCA',
    'LIECHTENSTEIN':'LIE',
    'SRI LANKA':'LKA',
    'LESOTHO':'LSO',
    'LITHUANIA':'LTU',
    'LUXEMBOURG':'LUX',
    'LATVIA':'LVA',
    'MACAO':'MAC',
    'SAINT MARTIN (French part)':'MAF',
    'MOROCCO':'MAR',
    'MONACO':'MCO',
    'MOLDOVA, REPUBLIC OF':'MDA',
    'MADAGASCAR':'MDG',
    'MALDIVES':'MDV',
    'MEXICO':'MEX',
    'MARSHALL ISLANDS':'MHL',
    'MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF':'MKD',
    'MALI':'MLI',
    'MALTA':'MLT',
    'MYANMAR':'MMR',
    'MONTENEGRO':'MNE',
    'MONGOLIA':'MNG',
    'NORTHERN MARIANA ISLANDS':'MNP',
    'MOZAMBIQUE':'MOZ',
    'MAURITANIA':'MRT',
    'MONTSERRAT':'MSR',
    'MARTINIQUE':'MTQ',
    'MAURITIUS':'MUS',
    'MALAWI':'MWI',
    'MALAYSIA':'MYS',
    'MAYOTTE':'MYT',
    'NAMIBIA':'NAM',
    'NEW CALEDONIA':'NCL',
    'NIGER':'NER',
    'NORFOLK ISLAND':'NFK',
    'NIGERIA':'NGA',
    'NICARAGUA':'NIC',
    'NIUE':'NIU',
    'NETHERLANDS':'NLD',
    'NORWAY':'NOR',
    'NEPAL':'NPL',
    'NAURU':'NRU',
    'NEW ZEALAND':'NZL',
    'OMAN':'OMN',
    'PAKISTAN':'PAK',
    'PANAMA':'PAN',
    'PITCAIRN':'PCN',
    'PERU':'PER',
    'PHILIPPINES':'PHL',
    'PALAU':'PLW',
    'PAPUA NEW GUINEA':'PNG',
    'POLAND':'POL',
    'PUERTO RICO':'PRI',
    'KOREA, DEMOCRATIC PEOPLES REPUBLIC OF':'PRK',
    'PORTUGAL':'PRT',
    'PARAGUAY':'PRY',
    'PALESTINIAN TERRITORY, OCCUPIED':'PSE',
    'FRENCH POLYNESIA':'PYF',
    'QATAR':'QAT',
    'REUNION':'REU',
    'ROMANIA':'ROU',
    'RUSSIAN FEDERATION':'RUS',
    'RWANDA':'RWA',
    'SAUDI ARABIA':'SAU',
    'SUDAN':'SDN',
    'SENEGAL':'SEN',
    'SINGAPORE':'SGP',
    'SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS':'SGS',
    'SAINT HELENA':'SHN',
    'SVALBARD AND JAN MAYEN':'SJM',
    'SOLOMON ISLANDS':'SLB',
    'SIERRA LEONE':'SLE',
    'EL SALVADOR':'SLV',
    'SAN MARINO':'SMR',
    'SOMALIA':'SOM',
    'SAINT PIERRE AND MIQUELON':'SPM',
    'SERBIA':'SRB',
    'SAO TOME AND PRINCIPE':'STP',
    'SURINAME':'SUR',
    'SLOVAKIA':'SVK',
    'SLOVENIA':'SVN',
    'SWEDEN':'SWE',
    'SWAZILAND':'SWZ',
    'SEYCHELLES':'SYC',
    'SYRIAN ARAB REPUBLIC':'SYR',
    'TURKS AND CAICOS ISLANDS':'TCA',
    'CHAD':'TCD',
    'TOGO':'TGO',
    'THAILAND':'THA',
    'TAJIKISTAN':'TJK',
    'TOKELAU':'TKL',
    'TURKMENISTAN':'TKM',
    'TIMOR-LESTE':'TLS',
    'TONGA':'TON',
    'TRINIDAD AND TOBAGO':'TTO',
    'TUNISIA':'TUN',
    'TURKEY':'TUR',
    'TUVALU':'TUV',
    'TAIWAN, PROVINCE OF CHINA':'TWN',
    'TANZANIA, UNITED REPUBLIC OF':'TZA',
    'UGANDA':'UGA',
    'UKRAINE':'UKR',
    'UNITED STATES MINOR OUTLYING ISLANDS':'UMI',
    'URUGUAY':'URY',
    'UZBEKISTAN':'UZB',
    'HOLY SEE (VATICAN CITY STATE)':'VAT',
    'SAINT VINCENT AND THE GRENADINES':'VCT',
    'VENEZUELA':'VEN',
    'VIRGIN ISLANDS, BRITISH':'VGB',
    'VIRGIN ISLANDS, U.S.':'VIR',
    'VIET NAM':'VNM',
    'VANUATU':'VUT',
    'WALLIS AND FUTUNA':'WLF',
    'SAMOA':'WSM',
    'YEMEN':'YEM',
    'SOUTH AFRICA':'ZAF',
    'ZAMBIA':'ZMB',
    'ZIMBABWE':'ZWE'
} # End text_IC2

# Resource Content Originator
text_RCG = {
    'Army Geographic Agency (Netherlands)':'armyGeoAgencyNetherlands',
    'Army Geographic Centre (Spain)':'armyGeographicCentreSpain',
    'Army Geographic Institute (Portugal)':'armyGeoInstitutePortugal',
    'Bundeswehr Geoinformation Office (Germany)':'bundeswehrGeoinfoOffice',
    'Defence Acquisition and Logistics Organization (Denmark)':'defenceAcqLogOrgDenmark',
    'Defence Geographic Centre Intelligence Collection Group (United Kingdom)':'defenceGeoCentreIntColGrp',
    'Defence Imagery and Geospatial Organisation (Australia)':'defenceImageryGeoOrg',
    'Defense Information Security (Italy)':'defenseInfoSecurityItaly',
    'General Command of Mapping (Turkey)':'generalCommandMapping',
    'Geographic Service (Belgium)':'geoServiceBelgium',
    'Geographic Service of the Czech Armed Forces (Czech Republic)':'geoServCzechArmedForces',
    'Geospatial Information Agency (Latvia)':'geoInfoAgencyLatvia',
    'Geospatial Intelligence Organisation (New Zealand)':'geoIntelOrgNewZealand',
    'Hellenic Military Geographic Service (Greece)':'hellenicMilitaryGeoServ',
    'Joint Geography Bureau (France)':'jointGeoBureauFrance',
    'Mapping and Charting Establishment (Canada)':'mapChartEstablishment',
    'Mapping Service (Hungary)':'mappingServiceHungary',
    'Military Geographic Group (Estonia)':'milGeoGroupEstonia',
    'Military Geographic Service (Norway)':'milGeoServiceNorway',
    'Military Geography Division (Poland)':'milGeogDivisionPoland',
    'Military Mapping Centre (Lithuania)':'milMapCentreLithuania',
    'Military Topographic Directorate (Romania)':'milTopoDirectRomania',
    'Military Topographic Service (Bulgaria)':'milTopoServiceBulgaria',
    'National Army Topographic Service (Moldova)':'natArmyTopoServiceMoldova',
    'Swedish Armed Forces (Sweden)':'swedishArmedForces',
    'Topographic Institute (Slovakia)':'topoInstituteSlovakia',
    'Topographic Service (Finland)':'topoServiceFinland',
    'U.S. Africa Command (USAFRICOM)':'usAfricaCommand',
    'U.S. Air Force':'usAirForce',
    'U.S. Army':'usArmy',
    'U.S. Army Geospatial Center (AGC)':'usArmyGeospatialCenter',
    'U.S. Central Command (USCENTCOM)':'usCentralCommand',
    'U.S. Central Intelligence Agency (CIA)':'usCentralIntelAgency',
    'U.S. Coast Guard':'usCoastGuard',
    'U.S. Defense Intelligence Agency (DIA)':'usDefenseIntelAgency',
    'U.S. Department of Energy (DOE)':'usDeptOfEnergy',
    'U.S. Department of Homeland Security (DHS)':'usDeptOfHomelandSecurity',
    'U.S. Department of State':'usDeptOfState',
    'U.S. European Command (USEUCOM)':'usEuropeanCommand',
    'U.S. Federal Bureau of Investigation (FBI)':'usFedBurOfInvestigation',
    'U.S. Geological Survey (USGS)':'usGeologicalSurvey',
    'U.S. Joint Forces Command (USJFCOM)':'usJointForcesCommand',
    'U.S. Marine Corps':'usMarineCorps',
    'U.S. National Civil Applications Program (NCAP)':'usNatCivilAppsProgram',
    'U.S. National Geospatial-Intelligence Agency (NGA)':'usNationalGeoIntelAgency',
    'U.S. National Oceanic and Atmospheric Administration':'usNatOceanAtmosAdmin',
    'U.S. National Reconnaissance Office (NRO)':'usNationalReconnOffice',
    'U.S. National Security Agency (NSA)':'usNationalSecurityAgency',
    'U.S. Navy':'usNavy',
    'U.S. Northern Command (USNORTHCOM)':'usNorthernCommand',
    'U.S. Pacific Command (PACOM)':'usPacificCommand',
    'U.S. Southern Command (USSOUTHCOM)':'usSouthernCommand',
    'U.S. Special Operations Command (USSOCOM)':'usSpecialOperCommand',
    'U.S. Strategic Command (USSTRATCOM)':'usStrategicCommand',
    'U.S. Transportation Command (USTRANSCOM)':'usTransportationCommand'
}

# Vertical Datum
text_VDT = {
    'Ground Level':'groundLevel',
    'Mean Sea Level (MSL)':'meanSeaLevel',
    'National Geodetic Vertical Datum (NGVD) 1929':'ngvd29',
    'North American Vertical Datum (NAVD) 1988':'navd88',
    'WGS 84 EGM08 Geoid':'wgs84Egm08',
    'WGS 84 EGM96 Geoid':'wgs84Egm96',
    'WGS 84 Ellipsoid':'wgs84'
}

# Vertical Source
text_VSC = {
    'DTED 1':'interpolatedDted1',
    'DTED 2':'interpolatedDted2',
    'No Elevations':'noElevations',
    'Reflective Surface':'reflectiveSurface',
    'Stereoscopic Imagery':'stereoscopicImagery',
    'TIN Data':'tinData'
}

# Non-spatial Source Type
text_NSP = {
    'AAFIF':'ngaAutoAirFacInfoFile',
    'CIB 1':'ngaControlledImageBase1',
    'Commercial Data':'commercialData',
    'DAFIF':'ngaDigitalAirFltInfoFile',
    'DigitalGlobe Imagery':'digitalGlobeImagery',
    'DNC':'ngaDigitalNauticalChart',
    'DVOF':'ngaDigitalVertObstruction',
    'FFD':'ngaFoundationFeatureData',
    'GeoNames':'ngaGeoNames',
    'GPS':'gpsBasedFieldCollect',
    'Ikonos Imagery':'ikonosImagery',
    'Imagery':'imageryUnspecified',
    'ITD':'ngaInterimTerrainData',
    'IVD':'ngaInterimVectorData',
    'Landsat Imagery':'landsatImagery',
    'MIDB':'usModernizedIntegratedDB',
    'Military Map Data':'militaryMapData',
    'Non-military Map':'nonMilitaryMap',
    'NTM Imagery':'usNtmImagery',
    'Open Source Information':'openSourceInfo',
    'Operations Data':'operationsData',
    'ORBIMAGE Imagery':'orbimageImagery',
    'QuickBird Imagery':'quickBirdImagery',
    'SAC':'ngaStereoAirfieldCollect',
    'UVMap':'ngaUrbanVectorMap',
    'VITD':'ngaVectorInterimTerrain',
    'VMap 2':'ngaVectorMap2'
} # End text_NSP

# Spatial Source Type
text_SSY = {
    'CIB 1':'ngaControlledImageBase1',
    'DigitalGlobe Imagery':'digitalGlobeImagery',
    'DNC':'ngaDigitalNauticalChart',
    'FFD':'ngaFoundationFeatureData',
    'GeoEye-1 Imagery':'geoEye1',
    'GPS':'gpsBasedFieldCollect',
    'Ikonos Imagery':'ikonosImagery',
    'Imagery':'imageryUnspecified',
    'ITD':'ngaInterimTerrainData',
    'MIDB':'usModernizedIntegratedDB',
    'NTM Imagery':'usNtmImagery',
    'Operations Data':'operationsData',
    'ORBIMAGE Imagery':'orbimageImagery',
    'QuickBird Imagery':'quickBirdImagery'
} # End text_SSY

# Classification
text_ZSAX_RS0 = {
    'TS':'TS',
    'S':'S',
    'R':'R',
    'C':'C',
    'U':'U'
} # End text_ZSAX_RS0


# The main loop to process a file
def processFile(fileName):
    csvfile = openFile(fileName, 'rb')
    reader = csv.reader(csvfile, delimiter=',')

    header = reader.next()
    # print"Main header: %s\n" % (header)

    tschema = {}
    for (tableName,fieldName,fieldValue,dataType,dataLength,dataMeasure,dataRange) in reader:

        if fieldName == '': # Empty feature/line
            continue

        # Cleanup so we don't get confused
        aDefault = ''
        aDesc = ''
        aLength = ''
        aName = ''
        aType = ''
        tmpValue = ''

        # Find the Feature Name
        (fName,tName2) = tableName.split(':')
        fGeometry = geo_list[fName[-1]]

        # Drop all of the 'Table' features
        #if fGeometry == 'Table':
            #continue

        tName2 = tName2.lstrip()
        (fDesc,fCode) = tName2.split('  ')
        fDesc = fDesc.strip()
        fCode = fCode[2:-1]

        # Sort out a feature vs a codelist, enumeration etc
        if fieldName.find(':') == -1:
            aName = fieldName
            aDesc = fieldValue
        else:
            (aName,tmpValue) = fieldName.split(':')

        # Set the attribute type
        if dataType != '':
            if dataType in dataType_list:
                aType = dataType_list[dataType]
                aDefault = default_list[aType]

            if (dataType.find('StrucText') > -1):
                aType = 'String'
                aDefault = ''

            if fieldValue.find('interval closure') > -1:
                aDefault = '5'
        #else:
            #print 'dataType not found:%s:' % (dataType)

        # Set the length
        if dataLength != '' and dataLength != 'unlimited':
            aLength = dataLength

        if fName not in tschema:  # New feature
            tschema[fName] = {}
            tschema[fName]['name'] = fName
            tschema[fName]['fcode'] = fCode
            tschema[fName]['desc'] = fDesc
            tschema[fName]['geom'] = fGeometry
            tschema[fName]['columns'] = {}
            tschema[fName]['columns']['F_CODE'] = { 'name':'F_CODE','desc':"Feature Code",'type':'String','optional':'R','defValue':'','length':'5'}

        if aName != '':
            if aName not in tschema[fName]['columns']:
                tschema[fName]['columns'][aName] = {}
                tschema[fName]['columns'][aName] = { 'name':aName,
                                                        'desc':aDesc,
                                                        'type':aType,
                                                        'optional':'R',
                                                        'defValue':aDefault,
                                                        }
                                                        # 'length':aLength
                if aLength != '':
                    tschema[fName]['columns'][aName]['length'] = aLength

                if aType.find('numeration') > -1:
                    tschema[fName]['columns'][aName]['enum'] = []
                    continue

            #if tschema[fName]['columns'][aName]['type'] == 'enumeration'  or tschema[fName]['columns'][aName]['type'] == 'textEnumeration':
            if tschema[fName]['columns'][aName]['type'].find('numeration') > -1:
                if fieldValue == 'Hierarchical Enumerants':
                    if fCode == 'AL013': # Building
                        for i in building_FFN:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':building_FFN[i]})
                        tschema[fName]['columns'][aName]['func'] = 'building_FFN'

                    if fCode == 'AL010': # Facility
                        for i in facility_FFN:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':facility_FFN[i]})
                        tschema[fName]['columns'][aName]['func'] = 'facility_FFN'

                    if fCode == 'AH055': # Fortified Building
                        for i in fortified_FFN:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':fortified_FFN[i]})
                        tschema[fName]['columns'][aName]['func'] = 'fortified_FFN'
                    continue

                if dataType == 'Local specification': # Text Enumeration
                    if aName in textFuncList:
                        tschema[fName]['columns'][aName]['func'] = textFuncList[aName]

                    if aName == 'CPS': # Cell Partitioning Scheme
                        for i in text_CPS:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_CPS[i]})
                        continue

                    if aName == 'EQC': # Equivalent Scale Category
                        for i in text_EQC:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_EQC[i]})
                        continue

                    if aName == 'ETS': # Extraction Specification
                        for i in text_ETS:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_ETS[i]})
                        continue

                    if aName == 'HZD': # Geodetic Datum
                        for i in text_HZD:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_HZD[i]})
                        continue

                    if aName.find('IC2') > -1: # Country Location
                        for i in text_IC2:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_IC2[i]})
                        continue

                    if aName == 'RCG' or aName == 'ZI004_RCG': # Resource Content Originator
                        for i in text_RCG:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_RCG[i]})
                        continue

                    if aName == 'VDT': # Vertical Datum
                        for i in text_VDT:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_VDT[i]})
                        continue

                    if aName == 'ZI001_NSP': # Non-spatial Source type
                        for i in text_NSP:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_NSP[i]})
                        continue

                    if aName == 'ZI001_SSY': # Spatial Source type
                        for i in text_SSY:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_SSY[i]})
                        continue

                    if aName == 'ZI001_VSC': # Vertical Source
                        for i in text_VSC:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_VSC[i]})
                        continue

                    if aName == 'ZSAX_RS0': # Classification
                        for i in text_ZSAX_RS0:
                            tschema[fName]['columns'][aName]['enum'].append({'name':i,'value':text_ZSAX_RS0[i]})
                        continue


                # Default: Split the value and store it
                (eValue,eName) = fieldValue.split("=")
                tValue = eValue.replace("'","").strip()
                tschema[fName]['columns'][aName]['enum'].append({'name':eName.strip(),'value':tValue})
                continue

            continue # aName != ''

        # Debug: Sanity checking
        print '##### Missed:',tableName,fieldName,fieldValue,dataType,dataLength,dataMeasure,dataRange

    return tschema
# End of processFile


###########
# Main Starts Here
#
parser = argparse.ArgumentParser(description='Process TDSv40 file and build a schema')
parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
parser.add_argument('--rules', help='Dump out one2one rules',action='store_true')
parser.add_argument('--txtrules', help='Dump out text rules',action='store_true')
parser.add_argument('--numrules', help='Dump out number rules',action='store_true')
parser.add_argument('--attrlist', help='Dump out a list of attributes',action='store_true')
parser.add_argument('--fcodelist', help='Dump out a list of FCODEs',action='store_true')
parser.add_argument('--fcodeattrlist', help='Dump out a list of FCODE attributes',action='store_true')
parser.add_argument('--toenglish', help='Dump out To English translation rules',action='store_true')
parser.add_argument('--fromenglish', help='Dump out From English translation rules',action='store_true')
parser.add_argument('--attributecsv', help='Dump out attributes as a CSV file',action='store_true')
parser.add_argument('--fullschema', help='Dump out a schema with text enumerations',action='store_true')
parser.add_argument('mainfile', help='The TDSv40 spec csv file', action='store')

args = parser.parse_args()

main_csv_file = args.mainfile

schema = {}
schema = processFile(main_csv_file)

# Now dump the schema out
if args.rules:
    printRules(schema)
elif args.txtrules:
    printTxtRules(schema)
elif args.numrules:
    printNumRules(schema)
elif args.attrlist:
    printAttrList(schema)
elif args.fcodelist:
    printFcodeList(schema)
elif args.fcodeattrlist:
    printFcodeAttrList(schema)
elif args.toenglish:
    printToEnglish(schema)
elif args.fromenglish:
    printFromEnglish(schema)
elif args.attributecsv:
    printAttributeCsv(schema)
elif args.fullschema:
    printJSHeader()
    printVariableBody('building_FFN',building_FFN)
    printVariableBody('facility_FFN',facility_FFN)
    printVariableBody('fortified_FFN',fortified_FFN)
    printVariableBody('text_CPS',text_CPS)
    printVariableBody('text_EQC',text_EQC)
    printVariableBody('text_ETS',text_ETS)
    printVariableBody('text_HZD',text_HZD)
    printVariableBody('text_IC2',text_IC2)
    printVariableBody('text_RCG',text_RCG)
    printVariableBody('text_VDT',text_VDT)
    printVariableBody('text_NSP',text_NSP)
    printVariableBody('text_SSY',text_SSY)
    printVariableBody('text_VSC',text_VSC)
    printVariableBody('text_ZSAX_RS0',text_ZSAX_RS0)
    printJavascript(schema)
    printJSFooter()
else:
    dropTextEnumerations(schema)
    printJSHeader()
    printVariableBody('building_FFN',building_FFN)
    printVariableBody('facility_FFN',facility_FFN)
    printVariableBody('fortified_FFN',fortified_FFN)
    printJavascript(schema)
    printJSFooter()
# End

