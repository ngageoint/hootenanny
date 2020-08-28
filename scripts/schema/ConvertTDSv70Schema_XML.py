#!/usr/bin/python

# Parse a TDS7 XML file into a schema
import sys,argparse,gzip

# This is where the common functions live
from hootLibrary import *

from xml.dom import minidom


# printJavascript: Dump out the structure as Javascript
#
# Note: This uses double quotes ( " ) around data elements in the output.  The XML files have values with
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
        print '          columns:['

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            print '                     { name:"%s",' % (k)
            print '                       desc:"%s",' % (schema[f]['columns'][k]['desc'])
            print '                       optional:"%s",' % (schema[f]['columns'][k]['optional'])

            #if schema[f]['columns'][k]['length'] != '':
            if 'definition' in schema[f]['columns'][k]:
                print '                       definition:"%s",' % (schema[f]['columns'][k]['definition'])

            if 'length' in schema[f]['columns'][k]:
                print '                       length:"%s",' % (schema[f]['columns'][k]['length'])

            if 'units' in schema[f]['columns'][k]:
                print '                       units:"%s",' % (schema[f]['columns'][k]['units'])

            #if schema[f]['columns'][k]['type'].find('numeration') != -1:
            if 'func' in schema[f]['columns'][k]:
                print '                       type:"enumeration",'
                print '                       defValue:"%s",' % (schema[f]['columns'][k]['defValue'])
                print '                       enumerations: %s' % (schema[f]['columns'][k]['func'])

            elif schema[f]['columns'][k]['type'] == 'enumeration':
                #print '                       type:"%s",' % (schema[f]['columns'][k]['type'])
                print '                       type:"enumeration",'
                print '                       defValue:"%s",' % (schema[f]['columns'][k]['defValue'])
                print '                       enumerations:['
                for l in schema[f]['columns'][k]['enum']:
                    print '                           { name:"%s", value:"%s" },' % (l['name'],l['value'])
                # print '                        ] // End of Enumerations'
                print '                        ]'

            #elif schema[f]['columns'][k]['type'] == 'textEnumeration':
                #print '                       type:"Xenumeration",'
                #print '                       defValue:"%s", ' % (schema[f]['columns'][k]['defValue'])
                #print '                       enumerations: text_%s' % (k)

            else:
                print '                       type:"%s",' % (schema[f]['columns'][k]['type'])
                print '                       defValue:"%s"' % (schema[f]['columns'][k]['defValue'])

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

    print '    ]; // End of schema\n' # End of schema
# End printJavascript


# Go through the schema and pull out all of the attributes that are to be replaced with
# functions
def printFuncList(schema):
    printList = {}

    for i in schema:
        for j in schema[i]['columns']:
            if 'func' in schema[i]['columns'][j]:
                tName = schema[i]['columns'][j]['func']
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
                                                'defValue':'No Information','length':'256','optional':'R'}
        schema[featureName]['columns']['earlydate'] = {}
        schema[featureName]['columns']['earlydate'] = {'name':'earlydate','desc':'Earliest image date in mosaic','type':'String',
                                                'defValue':'No Information','length':'256','optional':'R'}
        schema[featureName]['columns']['latedate'] = {}
        schema[featureName]['columns']['latedate'] = {'name':'AEI','desc':'Latest image date in mosaic','type':'String',
                                                'defValue':'No Information','length':'256','optional':'R'}
        schema[featureName]['columns']['img_layer'] = {}
        schema[featureName]['columns']['img_layer'] = {'name':'img_layer','desc':'Imagery Layer Name','type':'String',
                                                'defValue':'No Information','length':'256','optional':'R'}
        schema[featureName]['columns']['img_mosaic'] = {}
        schema[featureName]['columns']['img_mosaic'] = {'name':'img_mosaic','desc':'Image Mosaic','type':'String',
                                                'defValue':'no','length':'3','optional':'R'}
        schema[featureName]['columns']['PFI'] = {}
        schema[featureName]['columns']['PFI'] = {'name':'PFI','desc':'Acquisition Platform Identifier','type':'String',
                                                'defValue':'No Information','length':'15','optional':'R'}

        if 'ZI001_SRT' not in schema[featureName]['columns']:
            schema[featureName]['columns']['ZI001_SRT'] = {}
            schema[featureName]['columns']['ZI001_SRT'] = {'name':'ZI001_SRT','desc':'Source Information : Source Type',
                                                'type':'String',
                                                'func':'ZI001_SRT',
                                                'defValue':'noInformation','length':'30','optional':'R'}

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



# Read all of the features in an XML document
def readFeatures(xmlDoc,funcList):
    tSchema = {}

    # Setup handy lists
    geoList = {'C':'Line', 'S':'Area', 'P':'Point','_':'None' }

    itemList = xmlDoc.getElementsByTagName('DataElement')

    for feature in itemList:
        featureName = feature.getElementsByTagName('Name')[0].firstChild.data

        # Drop this one. It seems to be the catch all
        if featureName == 'TDS' or featureName == 'TDS_CARTO':
            continue

        # Build the basic feature
        tSchema[featureName] = {}
        tSchema[featureName]['name'] = featureName
        tSchema[featureName]['geom'] = geoList[featureName[-1]]
        tSchema[featureName]['desc'] = ''
        # tSchema[featureName]['description'] = ''
        tSchema[featureName]['columns'] = {}

        # Debug
        # print '#######################################'
        # print 'DE Name: ',featureName
        fieldValueList = feature.getElementsByTagName('Field')
        if fieldValueList != 'None':
            for fieldValue in fieldValueList:
                fName = fieldValue.getElementsByTagName('Name')[0].firstChild.data

                # Skip features we don't want
                if fName == 'ObjectID' or fName == 'SHAPE' or fName == 'SHAPE_Length' or fName == 'SHAPE_Area':
                    # Debug
                    # print 'Skipped: ', fName
                    continue

                # In this XML, the F_CODE is in the fields, not in the feature header.
                if fName == 'F_CODE':
                    tSchema[featureName]['columns']['F_CODE'] = { 'name':'F_CODE','desc':"Feature Code",'type':'String','optional':'R','defValue':'','length':'5'}
                    f_code = fieldValue.getElementsByTagName('DefaultValue')[0].firstChild.data
                    tSchema[featureName]['fcode'] = f_code

                    tDesc = fieldValue.getElementsByTagName('CodedValue')[0].getElementsByTagName('Name')[0].firstChild.data
                    tSchema[featureName]['desc'] = tDesc

                    # Debug
                    # print 'F_CODE = ', f_code,' tDesc= ',tDesc
                    continue

                fType = fieldValue.getElementsByTagName('Type')[0].firstChild.data

                # These are OK. We have filtered out the features that don't have these
                if fieldValue.getElementsByTagName('DefaultValue'):
                    defaultValue = fieldValue.getElementsByTagName('DefaultValue')[0].firstChild.data
                else:
                    defaultValue = ''

                length = fieldValue.getElementsByTagName('Length')[0].firstChild.data
                description = fieldValue.getElementsByTagName('AliasName')[0].firstChild.data

                # Now start building the attribute
                tSchema[featureName]['columns'][fName] = {}
                tSchema[featureName]['columns'][fName] = { 'name':fName.encode('utf8'),
                                                        'desc':description.encode('utf8'),
                                                        'type':'',
                                                        'defValue':defaultValue.encode('utf8'),
                                                        'length':length.encode('utf8'),
                                                        'optional':'R'
                                                      }

                # Swap some of the enumerated lists for functions to save storge space
                # E.g. having a full list of countries on EVERY feature is dumb
                if fName in funcList:
                    # Ugly bit of hard coding.
                    # We know that ZI020_GE4, ZI020_GE42, ZI020_GE43 etc are the same list
                    # Same with SGCC & *_CTUC etc
                    if fName.find('ZI020_GE4') > -1:
                        tSchema[featureName]['columns'][fName]['func'] = 'full_ZI020_GE4'
                    elif fName == 'SGCC':
                        tSchema[featureName]['columns'][fName]['func'] = 'full_ZI026_CTUC'
                    else:
                        tSchema[featureName]['columns'][fName]['func'] = 'full_' + fName

                # Debug
                # print 'Name:',fName.encode('utf8'),'  Type:',fType.encode('utf8')
                # print 'Desc:',description.encode('utf8')
                # print 'Default:',defaultValue.encode('utf8')
                # print 'Length:',length.encode('utf8')

                if fType == 'esriFieldTypeDouble':
                    tSchema[featureName]['columns'][fName]['type'] = 'Real'
                    continue

                if fType == 'esriFieldTypeString':
                    enumValueList = fieldValue.getElementsByTagName('CodedValue')

                    # 'esriFieldTypeString' can be enumerations OR just a String attribute
                    if len(enumValueList) > 0:
                        tSchema[featureName]['columns'][fName]['type'] = 'textEnumeration'
                        tSchema[featureName]['columns'][fName]['enum'] = []

                        for enumValue in enumValueList:
                            eName = enumValue.getElementsByTagName('Name')[0].firstChild.data.encode('utf8')
                            eCode = enumValue.getElementsByTagName('Code')[0].firstChild.data.encode('utf8')
                            tSchema[featureName]['columns'][fName]['enum'].append({'name':eName,'value':eCode})

                            # Debug
                            # print '  Name:',eName,'  Code:',eCode
                    else:
                        tSchema[featureName]['columns'][fName]['type'] = 'String'
                    continue

                if fType == 'esriFieldTypeInteger':
                    enumValueList = fieldValue.getElementsByTagName('CodedValue')

                    # 'esriFieldTypeInteger' can be enumerations OR just an Integer attribute
                    if len(enumValueList) > 0:
                        tSchema[featureName]['columns'][fName]['type'] = 'enumeration'
                        tSchema[featureName]['columns'][fName]['enum'] = []

                        for enumValue in enumValueList:
                            eName = enumValue.getElementsByTagName('Name')[0].firstChild.data.encode('utf8')
                            eCode = enumValue.getElementsByTagName('Code')[0].firstChild.data.encode('utf8')
                            tSchema[featureName]['columns'][fName]['enum'].append({'name':eName,'value':eCode})

                            # Debug
                            # print '  Name:',eName,'  Code:',eCode
                    else:
                        tSchema[featureName]['columns'][fName]['type'] = 'Integer'
                    continue

    return tSchema
# End of readFeatures


###########
# Main Starts Here
#
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process XML Schema file and build a schema')
    parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
    parser.add_argument('--attributecsv', help='Dump out attributes as a CSV file',action='store_true')
    parser.add_argument('--attrlist', help='Dump out a list of attributes',action='store_true')
    parser.add_argument('--dumpenum', help='Dump out the enumerated attributes, one file per FCODE into a directory called enum',action='store_true')
    parser.add_argument('--fcodeattrlist', help='Dump out a list of FCODE attributes',action='store_true')
    parser.add_argument('--fcodelist', help='Dump out a list of FCODEs',action='store_true')
    parser.add_argument('--fcodeschema', help='Dump out a list of fcodes in the internal OSM schema format',action='store_true')
    parser.add_argument('--fromenglish', help='Dump out From English translation rules',action='store_true')
    parser.add_argument('--fullschema', help='Dump out a schema with text enumerations',action='store_true')
    parser.add_argument('--numrules', help='Dump out number rules',action='store_true')
    parser.add_argument('--rules', help='Dump out one2one rules',action='store_true')
    parser.add_argument('--toenglish', help='Dump out To English translation rules',action='store_true')
    parser.add_argument('--txtrules', help='Dump out text rules',action='store_true')
    parser.add_argument('xmlFile', help='The XML Schema file', action='store')

    args = parser.parse_args()

    # Apparently, reding to a string and then parseing is quicker
    if args.xmlFile.endswith(".gz"):
        infile =  gzip.open(args.xmlFile, 'rb')
    else:
        infile = open(args.xmlFile, 'rb')

    content = infile.read()

    # xmlDoc = minidom.parse(args.xmlFile)
    xmlDoc = minidom.parseString(content)
    # print(xmlDoc.toprettyxml(indent = '  ',encoding = 'UTF-8'))

    schema = {}

    # The list of attributes to make into functions in the schema. Used for enumerated lists that get
    # repeated a lot.
    funcList = ['ZI001_SRT','ZI004_RCG','ZI020_GE4','ZI020_GE42','ZI020_GE43','ZI020_GE44',
        'ZI026_CTUC','SGCC','ZSAX_RS0','CPS','ETS','HZD','VDT']

    # schema = readDomains(xmlDoc,funcList)
    # schema = readFeatures(xmlDoc,funcList)

    # Add additional imagery relates attributes
    # schema = addImgAttrs(schema)

    schema = addImgAttrs(readFeatures(xmlDoc,funcList))


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
    elif args.fcodeschema:  # List the FCODES in the internal OSM schema format
        printFcodeSchema(schema)
    elif args.fcodeattrlist:
        printFcodeAttrList(schema)
    elif args.toenglish:
        printCopyright()
        printToEnglish(schema,'TDSv70','etds70','FCODE')
    elif args.fromenglish:
        printCopyright()
        printFromEnglish(schema,'TDSv70','etds70_osm_rules')
    elif args.attributecsv:
        printAttributeCsv(schema)
    elif args.dumpenum:
        dumpEnumerations(schema,'enumTDS7')
    elif args.fullschema:
        printCopyright()
        printJSHeader('tds70')
        printFuncList(schema)
        printJavascript(schema)
        printJSFooter('tds70')
    else:
        convertTextEnumerations(schema)
        printCopyright()
        printJSHeader('tds70')
        printFuncList(schema)
        printJavascript(schema)
        printJSFooter('tds70')
# End



