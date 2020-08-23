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
 #* @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 #*/


# ConvertTDSv70Schema.py
#
# Build a TDSv70 schema. The script also generates rules and lookup tables from the schema
#
# Note: This script does not do any error or sanity checking of the input files.
#
# Mattj Jun 19
#

import sys,csv,argparse

# This is where the common functions live
from hootLibrary import *


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
        print '          columns:['

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            print '                     { name:"%s",' % (k)
            print '                       desc:"%s",' % (schema[f]['columns'][k]['desc'])
            print '                       optional:"%s",' % (schema[f]['columns'][k]['optional'])

            if withDefs and 'definition' in schema[f]['columns'][k]:
                print '                       definition:"%s",' % (schema[f]['columns'][k]['definition'])


            if 'length' in schema[f]['columns'][k]:
                print '                       length:"%s",' % (schema[f]['columns'][k]['length'])

            if 'units' in schema[f]['columns'][k]:
                print '                       units:"%s",' % (schema[f]['columns'][k]['units'])

            if 'range' in schema[f]['columns'][k]:
                print '                       range:"%s",' % (schema[f]['columns'][k]['range'])

            if 'func' in schema[f]['columns'][k]:
                print '                       type:"enumeration",'
                print '                       defValue:"%s",' % (schema[f]['columns'][k]['defValue'])
                print '                       enumerations: %s' % (schema[f]['columns'][k]['func'])

            elif schema[f]['columns'][k]['type'] == 'Enumeration':
                #print '                       type:"%s",' % (schema[f]['columns'][k]['type'])
                print '                       type:"enumeration",'
                print '                       defValue:"%s",' % (schema[f]['columns'][k]['defValue'])
                print '                       enumerations:['
                for l in schema[f]['columns'][k]['enum']:
                    #print '                           { name:"%s", value:"%s" }, ' % (l['name'],l['value'])
                    print '                           {value:"%s",name:"%s"},' % (l['value'],l['name'])
                # print '                        ] // End of Enumerations '
                print '                        ]'

            else:
                print '                       type:"%s",' % (schema[f]['columns'][k]['type'])
                print '                       defValue:"%s" ' % (schema[f]['columns'][k]['defValue'])

            if num_attrib == 1:  # Are we at the last attribute? yes = no trailing comma
                # print '                     } // End of %s' % (k)
                print '                     }'
            else:
                # print '                     }, // End of %s' % (k)
                print '                     },'
                num_attrib -= 1

        # print '                    ] // End of Columns'
        print '                    ]'

        if num_feat == 1: # Are we at the last feature? yes = no trailing comma
            # print '          } // End of feature %s\n' % (schema[f]['fcode'])
            print '          }'
        else:
            # print '          }, // End of feature %s\n' % (schema[f]['fcode'])
            print '          },'
            num_feat -= 1

    # print '    ]; // End of schema\n'
    print '    ];'
# End printJavascript


# Print out a codelist as a JS variable
def printFunctions(eList):
    for i in eList:
        print '    var %s = [' % (i)

        num_vals = len(eList[i]['values'].keys()) # How many values does the thing have?
        for j in sorted(eList[i]['values'].keys(), key=asint):
            if num_vals == 1: # Are we at the last feature? yes = no trailing comma
                #print '              { name:"%s", value:"%s" } ' % (eList[i]['values'][j],j)
                print '              {value:"%s",name:"%s"}' % (j,eList[i]['values'][j])
            else:
                #print '              { name:"%s", value:"%s" }, ' % (eList[i]['values'][j],j)
                print '              {value:"%s",name:"%s"},' % (j,eList[i]['values'][j])
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


# Data & Lists
geo_list = {'T':'Table', 'C':'Line', 'S':'Area', 'P':'Point' }

# Convert from the Spec into what Hoot understands
dataType_list = {
    'BasicEncyclopediaStrucText':'String',
    'CodeList':'Enumeration',
    'DateTimeStrucText':'String',
    'Double':'Real',
    'Enumeration':'Enumeration',
    'IcaoLocIndStrucText':'String',
    'LexicalText':'String',
    # 'Local specification':'String',
    'LongInteger':'Integer',
    'NonlexicalText':'String',
    'PropertyDfddAccessorStrucText':'String',
    'ResClassificationStrucText':'String',
    'ResDissemControlsStrucText':'String',
    'ResNonIntelComMarkingsStrucText':'String',
    'ResOwnerProducerStrucText':'String',
    'RunwayDesigStrucText':'String',
    'SpecifiedDomainValuesStrucText':'String',
    'StructuredText':'String',
    'textEnumeration':'textEnumeration',
    'UniqueEntityIdentStrucText':'String',
    'UniqueResourceIdentStrucText':'String',
    'VerticalObstIdentifierStrucText':'String',
    }

# The set of default values for datatypes
default_list = {
    'BasicEncyclopediaStrucText':'noInformation',
    'CodeList':'noInformation',
    'DateTimeStrucText':'noInformation',
    'Double':'-999999.0',
    'Enumeration':'-999999',
    'IcaoLocIndStrucText':'noInformation',
    'LexicalText':'No Information',
    # 'Local specification':'No Information',
    'LongInteger':'-999999',
    'NonlexicalText':'No Information',
    'PropertyDfddAccessorStrucText':'noInformation',
    'ResClassificationStrucText':'noInformation',
    'ResDissemControlsStrucText':'noInformation',
    'ResNonIntelComMarkingsStrucText':'noInformation',
    'ResOwnerProducerStrucText':'noInformation',
    'RunwayDesigStrucText':'noInformation',
    'SpecifiedDomainValuesStrucText':'noInformation',
    'UniqueEntityIdentStrucText':'noInformation',
    'UniqueResourceIdentStrucText':'noInformation',
    'VerticalObstIdentifierStrucText':'noInformation',
    'textEnumeration':'noInformation'
}

srtList = {
"commercial":"A non-military map or chart, in a digital (often proprietary) format, sold commercially and usually accompanied by distribution and/or use restrictions.",
"deLormeDigitalAtlasEarth":"A DeLorme proprietary global feature data set consisting of a dense seamless topographical map of the world.",
"gpsBasedFieldCollect":"GPS-based field-collected open source data.",
"ikonosImagery":"Ikonos imagery of unspecified type and resolution.",
"imageryUnspecified":"Imagery of unspecified type and resolution.",
"militaryMapData":"A military (for example: NGA or defense service) map or chart, whether in hardcopy format or digitally scanned.",
"navteqData":"A NAVTEQ proprietary road and feature information data set used for routing and identification of points of interest.",
"ngaAutoAirFacInfoFile":"NGA Automated Aeronautical Facilities Information File.",
"ngaControlledImageBase1":"NGA Controlled Imagery Base 1 metre data.",
"ngaDigitalAirFltInfoFile":"NGA Digital Aeronautical Flight Information File.",
"ngaDigitalNauticalChart":"NGA Digital Nautical Chart.",
"ngaDigitalVertObstruction":"NGA Digital Vertical Obstruction File.",
"ngaFoundationFeatureData":"NGA Foundation Feature Data.",
"ngaGeoNames":"NGA GeoNames.",
"ngaInterimTerrainData":"NGA Interim Terrain Data.",
"ngaInterimVectorData":"NGA Interim Vector Data.",
"ngaStereoAirfieldCollect":"NGA Stereo Airfield Collection.",
"ngaUrbanVectorMap":"NGA Urban Vector Map.",
"ngaVectorInterimTerrain":"NGA Vector Interim Terrain Data.",
"ngaVectorMap2":"NGA Vector Map Level 2.",
"nonMilitaryMap":"A non-military (for example: commercial or personally prepared) map or chart in hardcopy format.",
"openSource":"Unclassified published information, whether obtained from digital or hardcopy sources.",
"operationsData":"Mission-specific and/or operational data set prepared by a US Combatant Command.",
"quickBirdImagery":"QuickBird imagery of unspecified type and resolution.",
"tomTomData":"A TomTom proprietary road and feature data set generally used for routing and identification of points of interest.",
"usModernizedIntegratedDB":"(US) Modernized Integrated Data Base.",
"usNtmImagery":"Imagery from (US) National Technical Means (NTM)."
}

# Go through the schema and add attributes for imagery tags
def addImgAttrs(schema):
    for featureName in schema:
        # schema[featureName]['columns']['fName'] = {'name':'AEI'.encode('utf8'),
        #                                         'desc':'Acquisition Event Identifier'.encode('utf8'),
        #                                         'type':'String',
        #                                         'defValue':'No Information'.encode('utf8'),
        #                                         'length':'256'.encode('utf8'),
        #                                         'optional':'R'
        #                                         }
        schema[featureName]['columns']['AEI'] = {}
        schema[featureName]['columns']['AEI'] = {'name':'AEI','desc':'Acquisition Event Identifier','type':'String',
                                                'defValue':'No Information','length':'254','optional':'R'}
        schema[featureName]['columns']['early_date'] = {}
        schema[featureName]['columns']['early_date'] = {'name':'early_date','desc':'Earliest image date in mosaic','type':'String',
                                                'defValue':'No Information','length':'20','optional':'R'}
        schema[featureName]['columns']['late_date'] = {}
        schema[featureName]['columns']['late_date'] = {'name':'late_date','desc':'Latest image date in mosaic','type':'String',
                                                'defValue':'No Information','length':'20','optional':'R'}
        schema[featureName]['columns']['img_layer'] = {}
        schema[featureName]['columns']['img_layer'] = {'name':'img_layer','desc':'Imagery Layer Name','type':'String',
                                                'defValue':'No Information','length':'254','optional':'R'}
        schema[featureName]['columns']['img_mosaic'] = {}
        schema[featureName]['columns']['img_mosaic'] = {'name':'img_mosaic','desc':'Image Mosaic','type':'String',
                                                'defValue':'no','length':'3','optional':'R'}
        schema[featureName]['columns']['PFI'] = {}
        schema[featureName]['columns']['PFI'] = {'name':'PFI','desc':'Acquisition Platform Identifier','type':'String',
                                                'defValue':'No Information','length':'15','optional':'R'}

        if 'ZI001_SRT' not in schema[featureName]['columns']:
            schema[featureName]['columns']['ZI001_SRT'] = {}
            schema[featureName]['columns']['ZI001_SRT'] = {'name':'ZI001_SRT','desc':'Source Information : Source Type',
                                                'type':'textEnumeration',
                                                'func':'ZI001_SRT',
                                                'enum':[],
                                                'defValue':'noInformation','length':'30','optional':'R'}
            # schema[featureName]['columns']['ZI001_SRT']['enum'] = []
            for k in srtList:
                schema[featureName]['columns']['ZI001_SRT']['enum'].append({'name':k,'value':srtList[k]})


        if 'ZI001_SDV' not in schema[featureName]['columns']:
            schema[featureName]['columns']['ZI001_SDV'] = {}
            schema[featureName]['columns']['ZI001_SDV'] = {'name':'ZI001_SDV','desc':'Source Information : Source Date and Time',
                                                'type':'String',
                                                'defValue':'noInformation','length':'20','optional':'R'}

        if 'ZI001_SDP' not in schema[featureName]['columns']:
            schema[featureName]['columns']['ZI001_SDP'] = {}
            schema[featureName]['columns']['ZI001_SDP'] = {'name':'ZI001_SDP','desc':'Source Information : Source Description',
                                                'type':'String',
                                                'defValue':'No Information','optional':'R'}

    return schema
# End addImgAttrs


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
        # print fieldName,fieldValue

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

    # Debug
    # for k in tValues:
    #     print k
    #     print tValues[k]

    return tValues
# End processValues


# The main loop to process a file
def processFile(fileName,enValues):
    csvfile = openFile(fileName, 'rb')
    reader = csv.reader(csvfile, delimiter=',')

    header = reader.next()
    # print"Main header: %s\n" % (header) # Debug

    tschema = {}

    for (rawFeatureName,fieldName,fieldValue,dataType,dataLength,dataMeasure,dataRange,fDefinition,fMoreText,fNote) in reader:

        if rawFeatureName == '': # Empty feature/line
            continue

        # Cleanup so we don't get confused later
        aDesc = ''
        aName = ''
        aType = ''
        tmpValue = ''

        # Find the Feature Name
        (fName,tmpFcode) = rawFeatureName.split(':')
        tmpFcode = tmpFcode.strip()
        (fDescription,rawFcode) = tmpFcode.split('   ')
        fCode = rawFcode[1:-1] # or 2:-1
        fGeometry = geo_list[fName[-1]]

        # Drop all of the 'Table' features
        if fGeometry == 'Table':
            #print "Skipping: " + rawFeatureName # Debug
            continue

        # Drop the "_<geometry>" from the end of the name, add spaces and Title Case
        # fDescription = fName[:-2].replace("_"," ").title()

        if fName not in tschema:  # New feature
            # print "Name:%s  fCode:%s  Geom:%s  Desc:%s" % (fName,fCode,fGeometry,fDescription)  # Debug
            tschema[fName] = {}
            tschema[fName]['name'] = fName
            tschema[fName]['fcode'] = fCode
            tschema[fName]['desc'] = fDescription.strip()
            tschema[fName]['definition'] = fDefinition + ' ' + fMoreText
            tschema[fName]['geom'] = fGeometry
            tschema[fName]['columns'] = {}
            tschema[fName]['columns']['F_CODE'] = { 'name':'F_CODE','desc':"Feature Code",'type':'String','optional':'R','defValue':'','length':'5'}

        # If no field name, no need to continue
        if fieldName == '':
            continue

        # Swap "< >" for "[ ]" so that they don't get interpreted as HTML
        fieldValue = fieldValue.replace("<","[").replace(">","]")

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
                                                     'definition':fDefinition + ' ' + fMoreText,
                                                     'optional':'R'
                                                   }

            if dataType == 'Local specification':
                continue

            tschema[fName]['columns'][aName]['type'] = dataType_list[dataType]
            tschema[fName]['columns'][aName]['defValue'] = default_list[dataType]

            # AAARRRGGGHHH!!!!
            if aName == 'ZI026_CTUC':
                tschema[fName]['columns'][aName]['defValue'] = '5'
                dataType = 'CodeList'

            if dataType == 'Enumeration':
                tschema[fName]['columns'][aName]['enum'] = []
                continue

            if dataType == 'CodeList':
                # These are generally the codelists etc that get made into functions in the schema
                funcName = aName
                # ZI020_GE4, ZI020_GE42, ZI020_GE43,ZI020_GE44, all have the same lookup table.
                if aName.find('ZI020_GE4') > -1:
                    funcName = 'ZI020_GE4'

                # Why?????
                if aName == 'RCG':
                    funcName = 'ZI004_RCG'

                # print "  funcName: %s" % (funcName)
                if funcName in enValues:
                    tschema[fName]['columns'][aName]['func'] = funcName

                    if enValues[funcName]['type'] == 'text':
                        tschema[fName]['columns'][aName]['type'] = 'textEnumeration'
                    # else:
                    #     dataType = 'Enumeration'

                    # Now fill in the values
                    tschema[fName]['columns'][aName]['enum'] = []
                    for i in enValues[funcName]['values']:
                        tschema[fName]['columns'][aName]['enum'].append({'name':enValues[funcName]['values'][i],'value':i})

                else:
                    print "#### Missing dataType = " + dataType

            if aName == 'F_CODE':
                tschema[fName]['columns'][aName]['defValue'] = ''

            # Set the length
            if dataLength != '' and dataLength != 'unlimited':
                tschema[fName]['columns'][aName]['length'] = dataLength

            # Check if we have any units for the attribute
            if dataMeasure != '':
                tschema[fName]['columns'][aName]['units'] = dataMeasure

            # Check if we have a range for the attribute
            if dataRange != '':
                tschema[fName]['columns'][aName]['range'] = dataRange


            continue
        # End dataType

        # Debug
        # print 'aName',aName,'fieldValue:',fieldValue

        # Ugly repleated code for building FFN's
        if fieldValue == 'Hierarchical Enumerants':
            # Fortified Buildings have the same lookup as Buildings
            if fCode == 'AH055':
                fCode = 'AL013'
            # The lookup table is in the Values.csv
            tschema[fName]['columns'][aName]['func'] = fCode
            tschema[fName]['columns'][aName]['type'] = 'Enumeration'
            tschema[fName]['columns'][aName]['defValue'] = default_list['Enumeration']
            for i in enValues[fCode]['values']:
                tschema[fName]['columns'][aName]['enum'].append({'name':enValues[fCode]['values'][i],'value':i})

            continue

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
parser = argparse.ArgumentParser(description='Process TDS7 files and build a schema')
parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
parser.add_argument('--attributecsv', help='Dump out attributes as a CSV file',action='store_true')
parser.add_argument('--attrlist', help='Dump out a list of attributes',action='store_true')
parser.add_argument('--dumpenum', help='Dump out the enumerated attributes, one file per FCODE into a directory called enum',action='store_true')
parser.add_argument('--fcodeattrlist', help='Dump out a list of FCODE attributes',action='store_true')
parser.add_argument('--fcodelist', help='Dump out a list of fcodes',action='store_true')
parser.add_argument('--fcodeschema', help='Dump out a list of fcodes in the internal OSM schema format',action='store_true')
parser.add_argument('--fieldvalues', help='Dump out the schema as a JSON object for the Translation Assistant',action='store_true')
parser.add_argument('--fromenglish', help='Dump out From English translation rules',action='store_true')
parser.add_argument('--fullschema', help='Dump out a schema with text enumerations',action='store_true')
parser.add_argument('--intattr', help='Dump out all attributes that are integers',action='store_true')
# parser.add_argument('--layerlist', help='Dump out a list of fcodes and layer names',action='store_true')
parser.add_argument('--numrules', help='Dump out number rules',action='store_true')
parser.add_argument('--rules', help='Dump out one2one rules',action='store_true')
parser.add_argument('--toenglish', help='Dump out To English translation rules',action='store_true')
parser.add_argument('--txtlen', help='Dump out the lengths of all of the text attributes',action='store_true')
parser.add_argument('--txtrules', help='Dump out text rules',action='store_true')
parser.add_argument('--withdefs', help='Add feature and attribute definitions to the schema',action='store_true')
parser.add_argument('mainfile', help='The main TDS7 spec csv file', action='store')
# parser.add_argument('layerfile', help='A csv file with layer information', action='store')
parser.add_argument('valuesfile', help='A csv file with enumerated values', action='store')

args = parser.parse_args()

main_csv_file = args.mainfile
# layer_csv_file = args.layerfile
values_csv_file = args.valuesfile


# if args.layerlist:  # Dump out the F_CODES and layer names
#     layerList = {}
#     layerList = processLayers(layer_csv_file)
#     printLayerList(layerList)
#     sys.exit()


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

# NOTE: In the TDSv6.1 spec we made this optional
schema = addImgAttrs(schema)

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
    printCopyright()
    printToEnglish(schema,'TDSv70','etds70','F_CODE')

elif args.fromenglish:
    printCopyright()
    printFromEnglish(schema,'TDSv70','etds70_osm_rules')

elif args.attributecsv:  # Dump the schema as a CSV
    printAttributeCsv(schema)

elif args.fullschema:
    printCopyright()
    printJSHeader('tds70')
    printFunctions(enumValues)
    printJavascript(schema,withDefs)
    printJSFooter('tds70')

elif args.dumpenum:
    dumpEnumerations(schema,'enumTDS70')

elif args.fieldvalues:
    printFieldValues(schema,'tdsv70FieldValues')

else: # The default is to dump out a basic schema with no text enumerations
    schema = convertTextEnumerations(schema)
    printCopyright()
    printJSHeader('tds70')
    enumValues = dropTextEnumerations(enumValues)
    printFunctions(enumValues)
    printJavascript(schema,withDefs)
    printJSFooter('tds70')
# End

