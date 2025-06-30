#!/usr/bin/python

# Parse a MUVD XML file into a schema
import os,sys,argparse,gzip

# This is where the common functions live
from hootLibrary import *

from xml.dom import minidom

# printJavascript: Dump out the structure as Javascript
def printJavascript(schema):
    print 'var schema = [' # And so it begins...

    num_feat = len(schema.keys()) # How many features in the schema?
    for f in sorted(schema.keys()):
        # Skip all of the 'Table' features and features without geometry
        if schema[f]['geom'] == 'Table' or schema[f]['geom'] == 'None':
            continue

        pString = ' {name:"%s",fcode:"%s",desc:"%s",geom:"%s",' % (f,schema[f]['fcode'],schema[f]['name'],schema[f]['geom'])
        if 'fcsubtype' in schema[f]:
            pString += 'fcsubtype:"%s",' % (schema[f]['fcsubtype'])
        if 'fdname' in schema[f]:
            pString += 'fdname:"%s",' % (schema[f]['fdname'])
        if 'thematic' in schema[f]:
            pString += 'thematic:"%s",' % (schema[f]['thematic'])
        if 'description' in schema[f]:
            pString += 'definition:"%s",' % (schema[f]['description'])

        print pString
        print '  columns:['

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            aString = '   {name:"%s",desc:"%s",optional:"%s",' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'])

            if 'length' in schema[f]['columns'][k]:
                aString += 'length:"%s",' % (schema[f]['columns'][k]['length'])

            if 'units' in schema[f]['columns'][k]:
                aString += 'units:"%s",' % (schema[f]['columns'][k]['units'])

            if 'definition' in schema[f]['columns'][k]:
                aString += 'definition:"%s",' % (schema[f]['columns'][k]['definition'])

            if schema[f]['columns'][k]['type'] == 'enumeration':
                aString += 'type:"enumeration",defValue:"%s",' % (schema[f]['columns'][k]['defValue'])
                print aString
                print '    enumerations:['
                num_enum = len(schema[f]['columns'][k]['enum']) # How many enumerated values does the attribute have?
                for l in schema[f]['columns'][k]['enum']:
                    if num_enum == 1:
                        print '     {name:"%s",value:"%s"}' % (l['name'],l['value'])
                    else:
                        print '     {name:"%s",value:"%s"},' % (l['name'],l['value'])
                        num_enum -= 1
                print '    ]'
                if num_attrib == 1:  # Are we at the last attribute? yes = no trailing comma
                    print '   }'
                else:
                    print '   },'
                    num_attrib -= 1
            else:
                aString += 'type:"%s",defValue:"%s"}' % (schema[f]['columns'][k]['type'],schema[f]['columns'][k]['defValue'])
                if num_attrib > 1:  # Are we at the last attribute? yes = no trailing comma
                    aString += ','
                    num_attrib -= 1
                print aString
        
        print '  ]' # End of the attributes

        if num_feat == 1: # Are we at the last feature? yes = no trailing comma
            print ' }'
        else:
            print ' },'
            num_feat -= 1

    print '];' # End of schema
# End printJavascript

# XML Functions
def processFeatureGeom(node):
    return node.firstChild.nodeValue[-2] == '_'

def processTypeText(node,typeList):
    typeElements = node.getElementsByTagName('ValueDataType')
    if typeElements:
        dataType = typeElements[0]
        value = dataType.firstChild.nodeValue.strip() if dataType.firstChild else None
        if value in typeList:
            return typeList[value]
        elif value:
            return 'enumeration'
    return None

# Generic single node
def processSingleNode(node,text):
    tlist = node.getElementsByTagName(text)
    if tlist:
        tmp = u' ' + tlist[0].firstChild.data
        return tmp.encode('utf8').strip()
    return None
    
# End typeName

# Read all of the features in an XML document
def readFeatures(xmlDoc,funcList):
    itemList = xmlDoc.getElementsByTagName('FeatureType')

    # Setup handy lists
    geoList = {'C':'Curve', 'P':'Point', 'S':'Surface', '_':'None' }
    typeList = {'enumeration':'enumeration','CharacterString':'String','Integer':'Integer','Real':'Real'}

    tSchema = {}

    # Parse and index the enumerated values
    enumValue_lookup = {}
    for val in xmlDoc.getElementsByTagName('Value'):
        eValue = ''
        eCodedValue = ''

        enumValue_id = u' ' + val.getAttribute('id').encode('utf8').strip()
        eValue = processSingleNode(val,'label')
        eCodedValue = processSingleNode(val,'code')

        enumValue_lookup[enumValue_id] = { 'name':eCodedValue,
                                           'value':eValue
                                         }

    # Parse and index the FeatureAttributes
    attribute_lookup = {}
    for attr in xmlDoc.getElementsByTagName('FeatureAttribute'):
        aName = ''
        aDesc = ''
        aDef = ''
        aType = ''
        aDefVal = ''
        aLength = ''
        aUnits = ''
        aEnum = []

        attr_id = u' ' + attr.getAttribute('id').encode('utf8').strip()

        for node in attr.childNodes:

            if not node.localName:
                continue

            if node.localName == 'definition':
                aDef = processSingleNode(attr,'definition')
                continue

            if node.localName == 'taggedValues':
                aNode = attr.getElementsByTagName('taggedValues')[0]
                aDesc = processSingleNode(aNode,'name')
                continue

            if node.localName == 'title':
                aName = processSingleNode(attr,'title')
                continue

            if node.localName == 'ValueDataType':
                aType = processTypeText(attr,typeList)

                if aType == 'String':
                    aDefVal = 'UNK'
                if aType == 'Real':
                    aDefVal = '-32767.0'
                if aType == 'Integer':
                    aDefVal = '-32767'
                if aType == 'enumeration':
                    aDefVal = '0'

                continue

            if node.localName == 'enumeratedBy':
                enumRef_id = u' ' + node.getAttribute('idref').encode('utf8').strip()
                if enumRef_id in enumValue_lookup:
                    aEnum.append(enumValue_lookup[enumRef_id])
                continue

        attribute_lookup[attr_id] = { 'name':aName,
                                      'desc':aDesc,
                                      'definition':aDef,
                                      'optional':'R',
                                      'type':aType,
                                      'defValue':aDefVal
                                    }
        
        if aType == 'enumeration':
            attribute_lookup[attr_id]['enum'] = []
            attribute_lookup[attr_id]['enum'] = aEnum

    # Parse through the features
    for feature in itemList:
        fCode = ''
        geomChar = ''

        if feature.getElementsByTagName('name') and feature.getElementsByTagName('title'):
                rawName = processSingleNode(feature,'name')
                fGeom = geoList[rawName[-1]]
                geomChar = rawName[-1]
                fCode = geomChar + processSingleNode(feature,'title')

        if fCode:
            
            if feature.getElementsByTagName('taggedValues'):
                tNode = feature.getElementsByTagName('taggedValues')[0]
                fName = processSingleNode(tNode,'name')

            if feature.getElementsByTagName('definition'):
                fDesc = processSingleNode(feature,'definition')
            

            # Build a feature
            if fCode not in tSchema:
                tSchema[fCode] = {}
                tSchema[fCode]['fcode'] = fCode
                tSchema[fCode]['columns'] = {}
                tSchema[fCode]['columns']['FCODE'] = { 'name':'FCODE','desc':'Feature Code','type':'String','optional':'R','defValue':'','length':'5'}

            # Process each node of the feature
            for node in feature.childNodes:

                if not node.localName:
                    continue

                # Cross reference the attributes in the feature types with the attribute lookup
                if node.localName == 'characterizedBy':
                    ref_id = u' ' + node.getAttribute('idref').encode('utf8').strip()
                    if ref_id in attribute_lookup:
                        tSchema[fCode]['columns'][attribute_lookup[ref_id]['name']] = attribute_lookup[ref_id]
                    continue

                if node.localName == 'code':
                    continue

                if node.localName == 'definition':
                    fDesc = processSingleNode(feature,'definition')
                    tSchema[fCode]['description'] = fDesc
                    continue

                if node.localName == 'description':
                    continue

                if node.localName == 'name':
                    rawName = processSingleNode(feature,'name')
                    fGeom = geoList[rawName[-1]]
                    geomChar = rawName[-1]
                    tSchema[fCode]['geom'] = fGeom
                    continue
                
                if node.localName == 'title':
                    continue

                if node.localName == 'taggedValues':
                    tNode = feature.getElementsByTagName('taggedValues')[0]
                    fName = processSingleNode(tNode,'name')
                    tSchema[fCode]['name'] = fName.replace('_S','').replace('_P','').replace('_C','')
                    continue
                
                if node.localName == 'package' or node.localName == 'type':
                    continue
                
                # Debug: If we didn't process a value for a node, print what we missed
                print('#### Node Missed ', node.localName)

    return tSchema


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
    parser.add_argument('--fieldvalues', help='Dump out the schema as a JSON object for the Translation Assistant',action='store_true')
    parser.add_argument('--fromenglish', help='Dump out From Englist translation rules',action='store_true')
    parser.add_argument('--fcodeschema', help='Dump out a list of fcodes in the internal OSM schema format',action='store_true')
    parser.add_argument('--nocodedvalues', help='Generate a Thematic Schema with no coded values',action='store_true')
    parser.add_argument('--numrules', help='Dump out number rules',action='store_true')
    parser.add_argument('--rules', help='Dump out one2one rules',action='store_true')
    parser.add_argument('--thematic', help='Generate a Thematic Schema',action='store_true')
    parser.add_argument('--toenglish', help='Dump out To English translation rules',action='store_true')
    parser.add_argument('--txtrules', help='Dump out text rules',action='store_true')
    parser.add_argument('xmlFile', help='The XML Schema file',action='store')

    args = parser.parse_args()

    # Apparently, reading to a string and then parsing is quicker
    if args.xmlFile.endswith(".gz"):
        infile = gzip.open(args.xmlFile, 'rb')
    else:
        infile = open(args.xmlFile, 'rb')

    content = infile.read()

    xmlDoc = minidom.parseString(content)

    schema = {}
    thematicSchema = {}
    thematicEnumSchema = {}

    # The list of attributes to make into functions in the schema. Used for enumerated lists that get
    # repeated a lot.
    funcList = []

    schema = readFeatures(xmlDoc,funcList)

    # Now dump the schema out
    if args.rules:
        printRules(schema)
    else:
        printCopyright()
        printJSHeader('muvd')
        printJavascript(schema)
        printJSFooter('muvd')

# End