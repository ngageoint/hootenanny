#!/usr/bin/python

# Parse a MGCP XML file into a schema
import sys,argparse,gzip
from xml.dom import minidom


# Start printJavascript
def printJSHeader(fileName):
    print notice
    print
    print '//  Schema built from %s' % (fileName)
    print
    print "var _global = (0, eval)('this');"
    print 'if (!_global.mgcp)'
    print '{'
    print '  _global.mgcp = {};'
    print '}'
    print
    print 'mgcp.schema = {'
    print 'getDbSchema: function()'
    print '{'
# End printJSHeader


# printJavascript: Dump out the structure as Javascript
#
# Note: This uses double quotes ( " ) around data elements in the output.  The csv files have values with
# single quotes ( ' ) in them.  These quotes are also in the DFDD and NFDD specs.
def printJavascript(schema):
    print '    var schema = [' # And so it begins...

    num_feat = len(schema.keys()) # How many features in the schema?
    for f in sorted(schema.keys()):
        # Skip all of the 'Table' features and features without geometry
        if schema[f]['geom'] == 'Table' or schema[f]['geom'] == 'None':
            continue

        print '        { name:"%s",' % (f); # name = geom + FCODE
        print '          fcode:"%s",' % (schema[f]['fcode'])
        print '          desc:"%s",' % (schema[f]['desc'])
        if 'definition' in schema[f]:
            print '          definition:"%s",' % (schema[f]['definition'])
        print '          geom:"%s",' % (schema[f]['geom'])
        print '          columns:[ '

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            print '                     { name:"%s",' % (k)
            print '                       desc:"%s" ,' % (schema[f]['columns'][k]['desc'])
            print '                       optional:"%s" ,' % (schema[f]['columns'][k]['optional'])

            #if schema[f]['columns'][k]['length'] != '':
            if 'definition' in schema[f]['columns'][k]:
                print '                       definition:"%s", ' % (schema[f]['columns'][k]['definition'])

            if 'length' in schema[f]['columns'][k]:
                print '                       length:"%s", ' % (schema[f]['columns'][k]['length'])

            if 'units' in schema[f]['columns'][k]:
                print '                       units:"%s", ' % (schema[f]['columns'][k]['units'])

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
# End printJavascript


# Print the footer for the javascript file
def printJSFooter():
    print '    return schema; \n'
    print '} // End of getDbSchema\n'
    print '} // End of mgcp.schema\n'
    print
    print 'exports.getDbSchema = mgcp.schema.getDbSchema;'
    print
# End printJSFooter


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

    #print '"FCODE","Name"'
    for i in sorted(tList.keys()):
        print '%s : %s' % (i,tList[i])
# End printFcodeList


# Print ToEnglish
# Dump out the set of MGCP English rules
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
            if eName == 'FCODE':
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
    print '    MGCP TRD4 "To English" Lookup Tables\n'
    print '    Huge piles of Ugly JSON!'
    print '*/\n'
    print 'emgcp.rules = {'
    print '    // Tables built from %s' % args.xmlFile

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

    print '} // End of emgcp.rules\n'
# End printToEnglish


# Print FromEnglish
# Dump out the set of MGCP English rules
def printFromEnglish(schema):
    eList = {}
    sList = {}
    for i in schema:
        for j in schema[i]['columns']:
            #if schema[i]['columns'][j]['name'] == 'FCODE':
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
    print '    MGCP TRD4 "From English" Lookup Tables\n'
    print '    Huge piles of Ugly JSON!'
    print '*/\n'
    print 'emgcp_osm_rules = {'
    print '    // Tables built from %s' % args.xmlFile

    # Dump out the Enumerated Values
    print '    // Enumerated Values'
    print '    enumValues : %s , // End of enumValues\n' % (eList)

    # Dump out the Single Values
    print '    // Single Values'
    print '    singleValues : %s , // End of singleValues\n' % (sList)

    print '} // End of emgcp_osm_rules\n'
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


# Go through the schema and pull out all of the attributes that are to be replaced with
# functions
def printFuncList(schema):
    printList = {}

    for i in schema:
        for j in schema[i]['columns']:
            if 'func' in schema[i]['columns'][j]:
                tName = 'full_' + schema[i]['columns'][j]['name']
                if tName not in printList:
                    printList[tName] = schema[i]['columns'][j]['enum']

    for i in printList:
        print '    var %s = [' % (i)
        num_vals = len(printList[i]) # How many values does the thing have?
        for k in printList[i]:
            if num_vals == 1: # Are we at the last feature? yes = no trailing comma
                print '              { name:"%s", value:"%s" } ' % (k['name'],k['value'])
            else:
                print '              { name:"%s", value:"%s" }, ' % (k['name'],k['value'])
                num_vals -= 1

        print '             ];'
        print
# End printFuncList


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


# XML Functions
# Text Node
def processText(node):
    tlist = node.getElementsByTagName('gco:CharacterString')
    tmp = u' ' + tlist[0].firstChild.data
    #print 'Definition:', tmp.encode('utf8').strip()
    return tmp.encode('utf8').strip()
# End Text Node


# Generic single node
def processSingleNode(node,text):
    tlist = node.getElementsByTagName(text)
    tmp = u' ' + tlist[0].firstChild.data
    #print 'Feature Type:', tmp.encode('utf8').strip()
    return tmp.encode('utf8').strip()
# End typeName


# Read the list of inheritance relations
def readInheritance(xmlDoc):
    #itemList = xmlDoc.getElementsByTagName('gfc:featureType')
    itemList = xmlDoc.getElementsByTagName('gfc:FC_InheritanceRelation')

    inheritList = {}

    for item in itemList:
        key = ''
        value = ''

        for node in item.childNodes:
            if not node.localName:
                continue
            #else:
                #print 'localName: ', node.localName

            if node.localName == 'subtype':
                key = node.getAttribute('xlink:href').replace('#','')
                continue

            if node.localName == 'supertype':
                value = node.getAttribute('xlink:href').replace('#','')
                continue

        inheritList[key] = value

    return inheritList
# End of readInheritance


# Add "inherited" attributes to a feature
def processInheritance(schema,inheritance):
    # This is not very good. There is probably a better way
    for i in inheritance:
        for item in schema[inheritance[i]]['columns']:
            tName = schema[inheritance[i]]['columns'][item]['name']
            schema[i]['columns'][tName]  = schema[inheritance[i]]['columns'][item]

    return schema
# End processInheritance


# Read all of the features in an XML document
def readFeatures(xmlDoc,funcList):

    itemList = xmlDoc.getElementsByTagName('gfc:FC_FeatureType')

    # Debug:
    #print 'Features: Items: ', len(itemList)

    # Setup handy lists
    geoList = {'L':'Line', 'A':'Area', 'P':'Point','_':'None' }
    typeList = {'CodeList':'enumeration','CharacterString':'String','Real':'Real','Integer':'Integer',
                'GM_Surface':'none', 'GM_Curve':'none','GM_Point':'none' }

    # These attributes have non-standard defaults
    customDefVal = {'ACC':'1','TXT':'N_A','SDP':'N_A','CON':'998','FUN':'6'}

    tSchema = {}

    for feature in itemList:
        tmp = u' ' + feature.getAttribute('id')
        rawfCode = tmp.encode('utf8').strip()

        # Debug: Just process one feature - AA010 Extraction Mine Area Feature
        #if rawfCode != 'AAA010':
            #continue

        fGeometry = geoList[rawfCode[0]]
        fCode = rawfCode[1:]
        # Debug:
        #print 'Geometry: ', fGeometry
        #print 'FCODE: ', fCode

        # Build a feature
        if rawfCode not in tSchema:
            tSchema[rawfCode] = {}
            tSchema[rawfCode]['name'] = rawfCode
            tSchema[rawfCode]['fcode'] = fCode
            tSchema[rawfCode]['geom'] = fGeometry
            tSchema[rawfCode]['columns'] = {}
            tSchema[rawfCode]['columns']['FCODE'] = { 'name':'FCODE','desc':"Feature Code",'type':'String','optional':'R','defValue':'','length':'5'}


        for node in feature.childNodes:
            if not node.localName:
                continue
            #else:
                #print 'localName: ', node.localName

            if node.localName == 'isAbstract':
                #print 'Abstract: ', processSingleNode(node,'gco:Boolean')
                continue

            if node.localName == 'featureCatalogue':
                continue

            # The short version of the feature definition
            if node.localName == 'typeName':
                #print 'Feature Type: ', processSingleNode(node,'gco:LocalName')
                tSchema[rawfCode]['desc'] = processSingleNode(node,'gco:LocalName').replace(' Area Feature','').replace(' Point Feature','').replace(' Line Feature','')
                continue

            # The long version of the feature definition
            if node.localName == 'definition':
                #print 'Definition:%s:' % processText(node)
                tSchema[rawfCode]['definition'] = processText(node)
                continue

            if node.localName == 'definitionReference':
                #print 'Node: DefinitionReference: ', processText(node)
                continue

            # Loop through the feature attributes
            if node.localName == 'carrierOfCharacteristics':
                aName = ''
                aDesc = ''
                aDef = ''
                aType = ''
                aDefVal = ''
                aLength = ''
                aUnits = ''
                aEnum = []

                for attribute in node.getElementsByTagName('gfc:FC_FeatureAttribute')[0].childNodes:
                    if not attribute.localName:
                        continue
                    #else:
                        #print 'Attr: ', attribute.localName

                    if attribute.localName == 'cardinality':
                        continue

                    if attribute.localName == 'featureType':
                        continue

                    if attribute.localName == 'memberName':
                        #print 'attr Name Def: ', processSingleNode(attribute,'gco:LocalName')
                        aDesc = processSingleNode(attribute,'gco:LocalName')
                        continue

                    if attribute.localName == 'definition':
                        #print 'aDef: ', processText(attribute)
                        aDef = processText(attribute)
                        continue

                    if attribute.localName == 'definitionReference':
                        #print 'attr Name: ', processText(attribute)
                        aName = processText(attribute)
                        continue

                    if attribute.localName == 'valueType':
                        #print 'attr Type: ', processText(attribute)
                        aType = typeList[processText(attribute)]

                        if aType == 'String':
                            aDefVal = 'UNK'
                        if aType == 'Real':
                            aDefVal = '-32767.0'
                        if aType == 'Integer':
                            aDefVal = '-32767'
                        if aType == 'enumeration':
                            aDefVal = '0'

                        continue

                    if attribute.localName == 'valueMeasurementUnit':
                        aUnits = attribute.getAttribute('xlink:href').replace('#','')
                        #print 'attr Type: Units = ', aUnits
                        continue

                    if attribute.localName == 'constrainedBy':
                        #print 'attr Constraint: ', processText(attribute)
                        # Constraint is something like: "0 to 100 Characters"
                        aLength = processText(attribute).split()[2]

                        # AAARRRGGGHHH!!!! Shapefile limitation
                        if aLength == '255':
                            aLength = '254'

                        continue

                    if attribute.localName == 'listedValue':
                        lName = ''
                        lValue = ''
                        for listed in attribute.getElementsByTagName('gfc:FC_ListedValue')[0].childNodes:
                            if not listed.localName:
                                continue
                            #else:
                                #print 'Listed Attr: ', listed.localName

                            if listed.localName == 'label':
                                #print 'Listed Label: ', processText(listed)
                                lName = processText(listed)
                                continue

                            if listed.localName == 'code':
                                #print 'Listed Code: ', processText(listed)
                                lValue = processText(listed)
                                continue

                            if listed.localName == 'definition':
                                #print 'Listed Def: ', processText(listed)
                                continue

                            if listed.localName == 'definitionReference':
                                #print 'Listed Name: ', processText(listed)
                                continue

                            # Debug: If we missed a listedValue print it here
                            print '##### Listed Missed ', listed.localName

                        aEnum.append({'name':lName,'value':lValue})
                        continue

                    # Debug: If we didn't process a value for one of the carrierOfCharacteristics
                    print '##### Carrier Missed ', attribute.localName

                # Now build a feature
                if aType != 'none':
                    # Sort out custom default values
                    if aName in customDefVal:
                        aDefVal = customDefVal[aName]

                    tSchema[rawfCode]['columns'][aName] = {}
                    tSchema[rawfCode]['columns'][aName] = { 'name':aName,
                                                            'desc':aDesc,
                                                            'type':aType,
                                                            'defValue':aDefVal,
                                                            'optional':'R'
                                                          }
                    if aName in funcList:
                        tSchema[rawfCode]['columns'][aName]['func'] = 'full_' + aName

                    if aDef != '':
                        tSchema[rawfCode]['columns'][aName]['definition'] = aDef

                    if aLength != '':
                        tSchema[rawfCode]['columns'][aName]['length'] = aLength

                    if aUnits != '':
                        tSchema[rawfCode]['columns'][aName]['units'] = aUnits

                    if aType == 'enumeration':
                        tSchema[rawfCode]['columns'][aName]['enum'] = []
                        tSchema[rawfCode]['columns'][aName]['enum'] = aEnum

                # Debug:
                #print '------------------------------------------------'
                continue

            # Debug: If we didn't process a value for a Node, print what we missed
            print '##### Node Missed ', node.localName

    return tSchema
# End of readFeatures




###########
# Main Starts Here
#
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process XML Schema file and build a schema')
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
    parser.add_argument('xmlFile', help='The XML Schema file', action='store')

    args = parser.parse_args()

    xmlDoc = minidom.parse(args.xmlFile)

    schema = {}

    # The list of attributes to make into functions in the schema. Used for enumerated lists that get
    # repeated a lot.
    funcList = ['SRT','FUN']

    schema = readFeatures(xmlDoc,funcList)

    # There are attributes that get "inherited" by features.
    inheritance = {}
    inheritance = readInheritance(xmlDoc)

    schema = processInheritance(schema,inheritance)


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
    else:
        printJSHeader(args.xmlFile)
        printFuncList(schema)
        printJavascript(schema)
        printJSFooter()
# End



