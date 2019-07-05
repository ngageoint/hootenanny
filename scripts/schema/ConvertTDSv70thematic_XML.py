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
        # print '          fcode:"%s",' % (schema[f]['fcode'])
        print '          desc:"%s",' % (schema[f]['desc'])
        if 'definition' in schema[f]:
            print '          definition:"%s",' % (schema[f]['definition'])
        print '          geom:"%s",' % (schema[f]['geom'])
        print '          columns:['

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            print '                     { name:"%s",' % (k)
            print '                       desc:"%s",' % (schema[f]['columns'][k]['desc'])
            if 'length' in schema[f]['columns'][k]:
                print '                       length:"%s",' % (schema[f]['columns'][k]['length'])

                print '                       type:"%s",' % (schema[f]['columns'][k]['type'])
                # print '                       defValue:"%s"' % (schema[f]['columns'][k]['defValue'])

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


# def printJSHeader(spec):
#     print
#     print "var _global = (0, eval)('this');"
#     print 'if (!_global.%s)' % (spec)
#     print '{'
#     print '  _global.%s = {};' % (spec)
#     print '}'
#     print
#     print '%s.schema = {' % (spec)
#     print 'getDbSchema: function()'
#     print '{'


# def printJSFooter(spec):
#     print '    return schema; \n'
#     print '} // End of getDbSchema\n'
#     print '} // End of %s.schema\n' % (spec)
#     print
#     print 'exports.getDbSchema = %s.schema.getDbSchema;' % (spec)
#     print


# Dump out the fcode to subtype list
def printSubTypes(schemaList):
    printList = {}

    for layerName in schemaList:
        for feature in schemaList[layerName]:

            # Some features don't have F_CODES.
            if feature['fcode'] == '':
                continue

            printList[feature['fcode']] = feature['subtype']

    for i in sorted(printList.keys()):
        print "'%s':'%s'" % (i,printList[i])
# End printSubTypes


# Dump out the fcode to thematic layer list
# NOTE: This is not sorted
def printLayerList(schemaList):
    geoList = {'Crv':'L', 'Srf':'A', 'Pnt':'P','_':'None' }

    for layerName in schemaList:
        geom = geoList[layerName[-3:]]

        for feature in schemaList[layerName]:

            # Some features don't have F_CODES.
            if feature['fcode'] == '':
                continue

            gFcode = geom + feature['fcode']
            print "'%s':'%s' // %s" % (gFcode,layerName,feature['name'])
# End printLayerList


# Read all of the features in an XML document
def readFeatures(xmlDoc):
    tSchema = {}

    # Setup handy lists
    geoList = {'Crv':'Line', 'Srf':'Area', 'Pnt':'Point','_':'None' }
    typeList = {'esriFieldTypeDouble':'Real', 'esriFieldTypeInteger':'Integer', 'esriFieldTypeString':'String'}

    itemList = xmlDoc.getElementsByTagName('DataElement')

    for feature in itemList:
        featureName = feature.getElementsByTagName('Name')[0].firstChild.data

        # Skip the high level categories 
        if featureName == 'TDS' or featureName == 'TDS_CARTO':
            continue

        # Debug
        # print '#######################################'
        # print 'DE Name: ',featureName

        # print 'nodename:',str(feature.nodeName) 
        # print 'nodevalue:',str(feature.nodeValue) 
        # print 'childNodes:',str(feature.childNodes) 

        tDesc = ''
        for tNode in feature.childNodes:
            # print '   nodename:',str(tNode.nodeName) 
            # print '   nodevalue:',str(tNode.nodeValue) 
            # print '   childNodes:',str(tNode.childNodes) 
            if tNode.nodeName == 'AliasName':
                tDesc = tNode.firstChild.data
                # break

        # print 'tDesc: ', tDesc.encode('utf8')

        # Build the basic feature
        tSchema[featureName] = {}
        tSchema[featureName]['name'] = featureName.encode('utf8')
        tSchema[featureName]['geom'] = geoList[featureName[-3:]]
        tSchema[featureName]['desc'] = tDesc.encode('utf8')
        tSchema[featureName]['description'] = ''
        tSchema[featureName]['columns'] = {}

        fieldValueList = feature.getElementsByTagName('Field')
        if fieldValueList != 'None':
            for fieldValue in fieldValueList:
                fName = fieldValue.getElementsByTagName('Name')[0].firstChild.data
                # Skip features we don't want
                if fName == 'ObjectID' or fName == 'SHAPE' or fName == 'SHAPE_Length' or fName == 'SHAPE_Area':
                    # Debug
                    # print 'Skipped: ', fName
                    continue

                fType = typeList[fieldValue.getElementsByTagName('Type')[0].firstChild.data]
                fLength = fieldValue.getElementsByTagName('Length')[0].firstChild.data
                fDesc = fieldValue.getElementsByTagName('AliasName')[0].firstChild.data

                # Now start building the attribute
                tSchema[featureName]['columns'][fName] = {}                
                tSchema[featureName]['columns'][fName] = { 'name':fName.encode('utf8'),
                                                        'desc':fDesc.encode('utf8'),
                                                        'type':fType.encode('utf8'),
                                                        'length':fLength.encode('utf8'),
                                                      }

                # Debug
                # print 'Name:',fName.encode('utf8'),'  Type:',fType.encode('utf8')
                # print 'Desc:',description.encode('utf8')
                # print 'Default:',defaultValue.encode('utf8')
                # print 'Length:',length.encode('utf8')


    return tSchema
# End of readFeatures


# Read all of the layers and SubTypes in an XML document
def readLayers(xmlDoc):
    tLayers = {}

    itemList = xmlDoc.getElementsByTagName('DataElement')

    for feature in itemList:
        featureName = feature.getElementsByTagName('Name')[0].firstChild.data

        if featureName == 'TDS' or featureName == 'TDS_CARTO':
            continue

        # Debug
        # print '#######################################'
        # print 'DE Name: ',featureName

        tLayers[featureName] = []

        subTypeList = feature.getElementsByTagName('Subtype')
        if subTypeList != 'None':
            for subTypeValue in subTypeList:
                sName = subTypeValue.getElementsByTagName('SubtypeName')[0].firstChild.data
                sCode = subTypeValue.getElementsByTagName('SubtypeCode')[0].firstChild.data

                fldCode = ''
                fieldList = subTypeValue.getElementsByTagName('SubtypeFieldInfo')
                for fld in fieldList:
                    fldName = fld.getElementsByTagName('FieldName')[0].firstChild.data
                    if fldName == 'F_CODE':
                        fldCode = fld.getElementsByTagName('DefaultValue')[0].firstChild.data
                        break

                # print '  ',sName,sCode,fldCode
                tLayers[featureName].append({'name':sName.encode('utf8'),'subtype':sCode.encode('utf8'),'fcode':fldCode.encode('utf8')})

    return tLayers
# End of readLayers


###########
# Main Starts Here
#
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process XML Schema file and build a schema')
    parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
    parser.add_argument('--subtypes', help='Dump out a list of FCODEs and subtypes',action='store_true')
    parser.add_argument('--layerlist', help='Dump out a list of FCODEs and thematic layers',action='store_true')
    parser.add_argument('--fcodeschema', help='Dump out a list of fcodes in the internal OSM schema format',action='store_true')
    parser.add_argument('xmlFile', help='The XML Schema file', action='store')

    args = parser.parse_args()

    # Apparently, reding to a string and then parseing is quicker
    if args.xmlFile.endswith(".gz"):
        infile =  gzip.open(args.xmlFile, 'rb')
    else:
        infile = open(args.xmlFile, 'rb')

    content = infile.read()

    xmlDoc = minidom.parseString(content)
    # print(xmlDoc.toprettyxml(indent = '  ',encoding = 'UTF-8'))

    schema = {}
    schema = readFeatures(xmlDoc)

    layerList = {}
    layerList = readLayers(xmlDoc)

    # Now dump the schema out
    if args.subtypes:
        printSubTypes(layerList)
    elif args.layerlist:
        printLayerList(layerList)
    elif args.fcodeschema:  # List the FCODES in the internal OSM schema format
        printFcodeSchema(schema)
    else:
        printCopyright()
        printJSHeader('tds70')
        printJavascript(schema)
        printJSFooter('tds70')
# End



