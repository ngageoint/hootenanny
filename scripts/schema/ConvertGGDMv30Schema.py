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
 #* @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 #*/


# ConvertGGDMv30Schema.py
#
# Build a GGDBv30 schema. The script also generates rules and lookup tables from the schema
#
# Note: This script does not do any error or sanity checking of the input files.
#
# Mattj Jul 16
#

import sys,os,csv,argparse,gzip


def printJSHeader():
    print notice
    print
    print "var _global = (0, eval)('this');"
    print 'if (!_global.ggdm30)'
    print '{'
    print '  _global.ggdm30 = {};'
    print '}'
    print
    print 'ggdm30.schema = {'
    print '//  Schema built from %s, %s and %s' % (main_csv_file, layer_csv_file, values_csv_file)
    print 'getDbSchema: function()'
    print '{'

# printJavascript: Dump out the structure as Javascript
#
# Note: This uses double quotes ( " ) around data elements in the output.  The csv files have values with
# single quotes ( ' ) in them.  These quotes are also in the DFDD and NFDD specs.
def printJavascript(schema,withDefs):
    print '    var schema = [' # And so it begins...

    num_feat = len(schema.keys()) # How many features in the schema?
    for f in sorted(schema.keys()):
        # Skip all of the 'Table' features
        if schema[f]['geom'] == 'Table':
            continue

        print '        { name:"%s",' % (f); # name = geom + FCODE
        print '          fcode:"%s",' % (schema[f]['fcode'])
        print '          desc:"%s",' % (schema[f]['desc'])
        if withDefs and 'definition' in schema[f]:
            print '          definition:"%s",' % (schema[f]['definition'])
        print '          geom:"%s",' % (schema[f]['geom'])
        print '          columns:[ '

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            print '                     { name:"%s",' % (k)
            print '                       desc:"%s" ,' % (schema[f]['columns'][k]['desc'])
            print '                       optional:"%s" ,' % (schema[f]['columns'][k]['optional'])

            if withDefs and 'definition' in schema[f]['columns'][k]:
                print '                       definition:"%s", ' % (schema[f]['columns'][k]['definition'])


            if 'length' in schema[f]['columns'][k]:
                print '                       length:"%s", ' % (schema[f]['columns'][k]['length'])

            if 'units' in schema[f]['columns'][k]:
                print '                       units:"%s", ' % (schema[f]['columns'][k]['units'])

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
                    #print '                           { name:"%s", value:"%s" }, ' % (l['name'],l['value'])
                    print '                           { value:"%s", name:"%s" }, ' % (l['value'],l['name'])
                print '                        ] // End of Enumerations '

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
    print '} // End of ggdm30.schema\n'
    print
    print 'exports.getDbSchema = ggdm30.schema.getDbSchema;'
    print

# End printJavascript


# Print out a codelist as a JS variable
def printFunctions(eList):
    for i in eList:
        print '    var %s = [' % (i)

        num_vals = len(eList[i]['values'].keys()) # How many values does the thing have?
        for j in sorted(eList[i]['values'].keys(), key=asint):
            if num_vals == 1: # Are we at the last feature? yes = no trailing comma
                #print '              { name:"%s", value:"%s" } ' % (eList[i]['values'][j],j)
                print '              { value:"%s", name:"%s" } ' % (j,eList[i]['values'][j])
            else:
                #print '              { name:"%s", value:"%s" }, ' % (eList[i]['values'][j],j)
                print '              { value:"%s", name:"%s" }, ' % (j,eList[i]['values'][j])
                num_vals -= 1

        print '             ];'
        print
# End printFunctions


# Drop all of the text enumerations from the Enumerated Values list
def dropTextEnumerations(tValues):

    nValues = {}

    for i in tValues:
        if tValues[i]['type'] != 'text':
            nValues[i] = tValues[i]

    return nValues
# End dropTextEnumerations


# Drop all of the text enumerations and replace them with strings
def convertTextEnumerations(tschema):
    for i in tschema:
        for j in tschema[i]['columns']:
            if tschema[i]['columns'][j]['type'] == 'textEnumeration':
                del tschema[i]['columns'][j]['func']
                tschema[i]['columns'][j]['type'] = 'String'

    return tschema
# End convertTextEnumerations


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


# Print FCODES in the internal OSM schema format
def printFcodeSchema(schema):
    tList = {}
    gList = {'Line':'linestring', 'Area':'area', 'Point':'node' }

    for i in schema:
        # Skip geometry we don't care about
        if schema[i]['geom'] not in gList:
            continue

        if schema[i]['fcode'] not in tList:
            tList[schema[i]['fcode']] = {}
            tList[schema[i]['fcode']]['description'] = schema[i]['desc']
            tList[schema[i]['fcode']]['isA'] = 'FCODE'
            tList[schema[i]['fcode']]['name'] = 'FCODE=' + schema[i]['fcode']
            tList[schema[i]['fcode']]['objectType'] = 'tag'
            tList[schema[i]['fcode']]['geometries'] = []

        tList[schema[i]['fcode']]['geometries'].append(gList[schema[i]['geom']])

    #print '"FCODE","Name"'
    for i in sorted(tList.keys()):
        #print tList[i]
        # Manual printing to get the order we want for sorting later
        print '{"name": "%s", "geometries": %s, "description": "%s", "isA": "FCODE", "objectType": "tag"}' % (tList[i]['name'],tList[i]['geometries'],tList[i]['description'])
# End printFcodeSchema


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
# End printFcodeAttrList


# printLayerList
def printLayerList(layerList):
    for i in sorted(layerList.keys()):
        print "'%s':'%s',  // %s" % (i,layerList[i]['layer'],layerList[i]['name'])

# End printLayerList


# Print ToEnglish
# Dump out the set of GGDM English rules
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
    print '    GGDMv30 "To English" Lookup Tables\n'
    print '    Huge piles of Ugly JSON!'
    print '*/\n'
    print 'eggdm30.rules = {'
    print '    // Tables built from %s, %s and %s' % (main_csv_file, layer_csv_file, values_csv_file)

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

    print '} // End of eggdm30.rules\n'
# End printToEnglish


# Print FromEnglish
# Dump out the set of TDS English rules
def printFromEnglish(schema):
    eList = {}
    sList = {}
    for i in schema:
        for j in schema[i]['columns']:

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
    print '    GGDMv30 "From English" Lookup Tables\n'
    print '    Huge piles of Ugly JSON!'
    print '*/\n'
    print 'eggdm30_osm_rules = {'
    print '    // Tables built from %s, %s and %s' % (main_csv_file, layer_csv_file, values_csv_file)

    # Dump out the Enumerated Values
    print '    // Enumerated Values'
    print '    enumValues : %s , // End of enumValues\n' % (eList)

    # Dump out the Single Values
    print '    // Single Values'
    print '    singleValues : %s , // End of singleValues\n' % (sList)

    print '} // End of eggdm30_osm_rules\n'
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


# Print Text Attribute Lengths
# Output format:      'BA000_VDR':80,
def printTxtLen(schema):
    tList = {}

    for i in schema:
        for j in schema[i]['columns']:
            if schema[i]['columns'][j]['type'] != 'String':
                continue

            if 'length' in schema[i]['columns'][j]:
                tList[schema[i]['columns'][j]['name']] = schema[i]['columns'][j]['length']

    for i in sorted(tList.keys()):
        print "     '%s':%s," % (i,tList[i])
# End printTxtLen


# Print Integer Attributes
# Output format:      'XXX',
def printIntAttr(schema):
    tList = []

    for i in schema:
        for j in schema[i]['columns']:
            if schema[i]['columns'][j]['type'] != 'Integer':
                continue

            if schema[i]['columns'][j]['name'] not in tList:
                tList.append(schema[i]['columns'][j]['name'])

    for i in sorted(tList):
        print "     '%s'," % (i)
# End printIntAttr


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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

 ////
 // This file is automatically generated. Please do not modify the file
 // directly.
 ////
 """

geo_list = {'T':'Table', 'C':'Line', 'S':'Area', 'P':'Point' }

# Convert from the Spec into what Hoot understands
dataType_list = {
    'Boolean':'enumeration',
    #'CodeList':'textEnumeration',
    #'Codelist':'textEnumeration',
    'CodeList':'enumeration',
    'Codelist':'enumeration',
    'CONSTRAINED_STRING':'String',
    'Enumeration':'enumeration',
    'Integer':'Integer',
    'integer':'Integer',
    'Real':'Real',
    'real':'Real',
    'String':'String',
    'StructuredText':'String'
    }

# The set of default values for datatypes
default_list = {
    'enumeration':'-999999',
    'Integer':'-999999',
    'Real':'-999999.0',
    'String':'No Information',
    'StructuredText':'noInformation',
    'textEnumeration':'noInformation'
}


# Process the Layers file
#
# NOTE: This just supports the "compsite" version at the moment
#
# Output format:         'PAA010':'IndustryPnt', // Industry
#
def processLayers(fileName):
    csvfile = openFile(fileName, 'rb')
    reader = csv.reader(csvfile, delimiter=',')

    header = reader.next()
    # print"Main header: %s\n" % (header)

    tValues = {}

    #FeatureName,FeatureCode,Global,Regional,Local,Specialized,Composite,NumericID,AliasName
    for (FeatureName,FeatureCode,Global,Regional,Local,Specialized,Composite,NumericID,AliasName) in reader:
        if FeatureName == '': # Empty feature/line
            continue

        # Build LFA000 from ADMINISTRATIVE_BOUNDARY_C and FA000
        nFcode = geo_list[FeatureName[-1]][0] + FeatureCode

        if Composite != '':
            tValues[nFcode] = {'layer':Composite, 'name':FeatureName}

    return tValues
# End processLayers


# Process the enumerated values file
def processValues(fileName):
    csvfile = openFile(fileName, 'rb')
    reader = csv.reader(csvfile, delimiter=',')

    header = reader.next()
    # print"Main header: %s\n" % (header)

    tValues = {}

    for (fieldName,fieldValue,fieldDefinition) in reader:

        if fieldName == '': # Empty feature/line
            continue

        if fieldDefinition == '':
            if fieldName not in tValues:  # New feature
                tValues[fieldName] = {}
                tValues[fieldName]['name'] = fieldValue
                tValues[fieldName]['values'] = {}
            continue

        # Default: Split the value and store it
        if (fieldValue.find('=') > -1):
            (aValue,aName) = fieldValue.split("=")
            tName = aName.replace("'","").replace("\"","").strip()
            tValue = aValue.replace("'","").strip()
            tValues[fieldName]['type'] = 'integer'
            tValues[fieldName]['values'][tValue] = tName
        else:
            tValues[fieldName]['type'] = 'text'
            tDef = fieldDefinition.replace("'","").replace("\"","").strip()
            tValues[fieldName]['values'][fieldValue] = tDef



    return tValues
# End processValues


# The main loop to process a file
def processFile(fileName,enValues):
    csvfile = openFile(fileName, 'rb')
    reader = csv.reader(csvfile, delimiter=',')

    header = reader.next()
    # print"Main header: %s\n" % (header) # Debug

    tschema = {}

    for (rawFeatureName,fieldName,fieldValue,dataType,dataLength,dataMeasure,fDefinition,dataLinage) in reader:

        if rawFeatureName == '': # Empty feature/line
            continue

        # Cleanup so we don't get confused later
        aDesc = ''
        aName = ''
        aType = ''
        tmpValue = ''

        # Find the Feature Name
        (fName,rawFcode) = rawFeatureName.split('  ')

        fCode = rawFcode[1:-1] # or 2:-1

        fGeometry = geo_list[fName[-1]]

        # Drop all of the 'Table' features
        if fGeometry == 'Table':
            #print "Skipping: " + rawFeatureName # Debug
            continue

        # Drop the "_<geometry>" from the end of the name, add spaces and Title Case
        fDescription = fName[:-2].replace("_"," ").title()

        if fName not in tschema:  # New feature
            #print "Name:%s  fCode:%s  Geom:%s  Desc:%s" % (fName,fCode,fGeometry,fDescription)  # Debug
            tschema[fName] = {}
            tschema[fName]['name'] = fName
            tschema[fName]['fcode'] = fCode
            tschema[fName]['desc'] = fDescription
            tschema[fName]['definition'] = fDefinition
            tschema[fName]['geom'] = fGeometry
            tschema[fName]['columns'] = {}

        # If no field name, no need to continue
        if fieldName == '':
            continue

        # Process the attributes
        # Sort out a feature vs a codelist, enumeration etc
        if fieldName.find(':') == -1:
            aName = fieldName
            aDesc = fieldValue
        else:
            (aName,tmpValue) = fieldName.split(':')

        if dataType != '':
            if aName not in tschema[fName]['columns']:
                tschema[fName]['columns'][aName] = {}
                tschema[fName]['columns'][aName] = { 'name':aName,
                                                     'desc':aDesc,
                                                     'definition':fDefinition,
                                                     'optional':'R'
                                                   }

            if dataType in dataType_list:
                aType = dataType_list[dataType]
                if aType.find('numeration') > -1:
                    tschema[fName]['columns'][aName]['enum'] = []
            else:
                # These are generally the codelists etc that gat made into functions in the schema
                if dataType in enValues:
                    #print "  Got: %s" % (dataType)
                    tschema[fName]['columns'][aName]['func'] = dataType

                    if enValues[dataType]['type'] == 'text':
                        aType = 'textEnumeration'
                    else:
                        aType = 'enumeration'

                    # Now fill in the values
                    tschema[fName]['columns'][aName]['enum'] = []
                    for i in enValues[dataType]['values']:
                        tschema[fName]['columns'][aName]['enum'].append({'name':enValues[dataType]['values'][i],'value':i})

                else:
                    print "#### Missing dataType = " + dataType

            tschema[fName]['columns'][aName]['type'] = aType
            tschema[fName]['columns'][aName]['defValue'] = default_list[aType]

            # AAARRRGGGHHH!!!!
            if dataType == 'GGDM_Interval_Domain':
                tschema[fName]['columns'][aName]['defValue'] = '5'

            if aName == 'F_CODE':
                tschema[fName]['columns'][aName]['defValue'] = ''

            # Set the length
            if dataLength != '' and dataLength != 'unlimited':
                tschema[fName]['columns'][aName]['length'] = dataLength

            # Check if we have any units for the attribute
            if dataMeasure != '':
                tschema[fName]['columns'][aName]['units'] = dataMeasure


            continue
        # End dataType

        # Default: Split the value and store it
        (eValue,eName) = fieldValue.split("=")
        tValue = eValue.replace("'","").strip()
        tName = eName.replace('"',"'").strip()

        if 'enum' in tschema[fName]['columns'][aName]:
            tschema[fName]['columns'][aName]['enum'].append({'name':tName,'value':tValue})

    return tschema
# End of processFile


###########
# Main Starts Here
#
parser = argparse.ArgumentParser(description='Process GGDM files and build a schema')
parser.add_argument('--attributecsv', help='Dump out attributes as a CSV file',action='store_true')
parser.add_argument('--attrlist', help='Dump out a list of attributes',action='store_true')
parser.add_argument('--fcodeattrlist', help='Dump out a list of FCODE attributes',action='store_true')
parser.add_argument('--fcodelist', help='Dump out a list of fcodes',action='store_true')
parser.add_argument('--fcodeschema', help='Dump out a list of fcodes in the internal OSM schema format',action='store_true')
parser.add_argument('--fromenglish', help='Dump out From English translation rules',action='store_true')
parser.add_argument('--fullschema', help='Dump out a schema with text enumerations',action='store_true')
parser.add_argument('--intattr', help='Dump out all attributes that are integers',action='store_true')
parser.add_argument('--layerlist', help='Dump out a list of fcodes and layer names',action='store_true')
parser.add_argument('--numrules', help='Dump out number rules',action='store_true')
parser.add_argument('--rules', help='Dump out one2one rules',action='store_true')
parser.add_argument('--toenglish', help='Dump out To English translation rules',action='store_true')
parser.add_argument('--txtlen', help='Dump out the lengths of all of the text attributes',action='store_true')
parser.add_argument('--txtrules', help='Dump out text rules',action='store_true')
parser.add_argument('--withdefs', help='Add feature ad attribute definitions to the schema',action='store_true')
parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
parser.add_argument('mainfile', help='The main GGDM spec csv file', action='store')
parser.add_argument('layerfile', help='A csv file with layer information', action='store')
parser.add_argument('valuesfile', help='A csv file with enumerated values', action='store')

args = parser.parse_args()

main_csv_file = args.mainfile
layer_csv_file = args.layerfile
values_csv_file = args.valuesfile


if args.layerlist:  # Dump out the F_CODES and layer names
    layerList = {}
    layerList = processLayers(layer_csv_file)
    printLayerList(layerList)
    sys.exit()


# Decide if we are going to print the definition for each feature and attribute
withDefs = False
if args.withdefs:
    withDefs = True

enumValues = {}
enumValues = processValues(values_csv_file)

# Debug
#for i in enumValues:
    #print "Name: %s  Type: %s  Def: %s" % (i,enumValues[i]['type'],enumValues[i]['name'])
    #for j in enumValues[i]['values']:
        #print "    Value: %s  Def: %s" % (j,enumValues[i]['values'][j])
    #print ""

# <Place holder>
# The next thing to do is to process the layers file and map the features into TDS like layers


schema = {}
schema = processFile(main_csv_file,enumValues)


# Now dump the schema, rules etc out
if args.rules:  # One2one Rules
    printRules(schema)

elif args.txtrules:  # Text attribute rules
    printTxtRules(schema)

elif args.txtlen:  # Print all of the lengths of the text attributes
    printTxtLen(schema)

elif args.numrules:  # Numerical rules
    printNumRules(schema)

elif args.intattr:  # Integer attributes
    printIntAttr(schema)

elif args.attrlist:  # Dump out all of the attribute names and definitions
    printAttrList(schema)

elif args.fcodelist:  # List the FCODES
    printFcodeList(schema)

elif args.fcodeschema:  # List the FCODES in the internal OSM schema format
    printFcodeSchema(schema)

elif args.fcodeattrlist:
    printFcodeAttrList(schema)

elif args.toenglish:
    printToEnglish(schema)

elif args.fromenglish:
    printFromEnglish(schema)

elif args.attributecsv:  # Dump the schema as a CSV
    printAttributeCsv(schema)

elif args.fullschema:
    printJSHeader()
    printFunctions(enumValues)
    printJavascript(schema,withDefs)
    printJSFooter()

else: # The default is to dump out a basic schema with no text enumerations
    schema = convertTextEnumerations(schema)
    printJSHeader()
    enumValues = dropTextEnumerations(enumValues)
    printFunctions(enumValues)
    printJavascript(schema,withDefs)
    printJSFooter()
# End

