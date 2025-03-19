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
 #* This will properly maintain the copyright information. Maxar
 #* copyrights will be updated automatically.
 #*
 #* @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 #*/

# Parse a TDS7 XML file into a schema
import sys,csv,argparse,gzip

# This is where the common functions live
from hootLibrary import *

from xml.dom import minidom


# printJavascript: Dump out the structure as Javascript
#
# Note: This uses double quotes ( " ) around data elements in the output.  The XML files have values with
# single quotes ( ' ) in them.  These quotes are also in the DFDD and NFDD specs.
def printJavascript(schema,withDefs):
    print 'var schema = [' # And so it begins...

    num_feat = len(schema.keys()) # How many features in the schema?
    for f in sorted(schema.keys()):
        # Skip all of the 'Table' features and features without geometry
        if schema[f]['geom'] == 'Table' or schema[f]['geom'] == 'None':
            continue

        pString = ' {name:"%s",fcode:"%s",desc:"%s",geom:"%s",' % (f,schema[f]['fcode'],schema[f]['desc'],schema[f]['geom']);
        if 'fcsubtype' in schema[f]:
            pString += 'fcsubtype:"%s",' % (schema[f]['fcsubtype'])
        if 'fdname' in schema[f]:
            # print '  fdname:"%s",' % (schema[f]['fdname'])
            pString += 'fdname:"%s",' % (schema[f]['fdname'])
        if 'thematic' in schema[f]:
            pString += 'thematic:"%s",' % (schema[f]['thematic'])
        if withDefs and 'definition' in schema[f]:
            pString += 'definition:"%s",' % (schema[f]['definition'])

        print pString
        print '  columns:['

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            aString = '   {name:"%s",desc:"%s",optional:"%s",' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'])

            if 'length' in schema[f]['columns'][k]:
                aString += 'length:"%s",' % (schema[f]['columns'][k]['length'])

            if 'units' in schema[f]['columns'][k]:
                aString += 'units:"%s",' % (schema[f]['columns'][k]['units'])

            if withDefs and 'definition' in schema[f]['columns'][k]:
                aString += 'definition:"%s",' % (schema[f]['columns'][k]['definition'])

            if 'func' in schema[f]['columns'][k]:
                aString += 'type:"%s",defValue:"%s",enumerations: %s}' % (schema[f]['columns'][k]['type'], schema[f]['columns'][k]['defValue'],schema[f]['columns'][k]['func'])
                if num_attrib > 1:  # Are we at the last attribute? yes = no trailing comma
                    aString += ','
                    num_attrib -= 1
                print aString

            elif schema[f]['columns'][k]['type'] == 'enumeration':
                aString += 'type:"enumeration",defValue:"%s",' % (schema[f]['columns'][k]['defValue'])
                print aString
                print '    enumerations:['
                num_enum = len(schema[f]['columns'][k]['enum']) # How many attributes does the feature have?
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

# Modified printing function to print the Thematic Enum header
def printThematicEnumHeader(spec):
    print
    print "var _global = (0, eval)('this');"
    print 'if (!_global.%s)' % (spec)
    print '{'
    print ' _global.%s = {};' % (spec)
    print '}'
    print
    print '%s.thematicSchema = {' % (spec)
    print 'getDbSchema: function()'
    print '{'

def printThematicEnumFooter(spec):
    print '  return thematicSchema;'
    print '} // End of getDbSchema'
    print '} // End of %s.thematicSchema' % (spec)
    print
    print 'exports.getDbSchema = %s.thematicSchema.getDbSchema;' % (spec)
    print

# Modified printing function to print the Thematic Schema
def printThematic(schema,spec):
    print
    print "var _global = (0, eval)('this');"
    print 'if (!_global.%s)' % (spec)
    print '{'
    print ' _global.%s = {};' % (spec)
    print '}'
    print
    print '%s.thematicSchema = [' % (spec)
    num_feat = len(schema.keys()) # How many features in the schema?
    for f in sorted(schema.keys()):
        # Skip all of the 'Table' features and features without geometry
        if schema[f]['geom'] == 'Table' or schema[f]['geom'] == 'None':
            continue

        pString = ' {name:"%s",fcode:"%s",desc:"%s",geom:"%s",' % (f,schema[f]['fcode'],schema[f]['desc'],schema[f]['geom']); # name = geom + FCODE
        if 'fdname' in schema[f]:
            pString += 'fdname:"%s",' % (schema[f]['fdname'])
        if 'thematic' in schema[f]:
            pString += 'thematic:"%s",' % (schema[f]['thematic'])

        print pString
        print '  columns:['

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            if num_attrib == 1:  # Are we at the last attribute? yes = no trailing comma
                print '   {name:"%s",desc:"%s",optional:"%s",type:"%s",length:"%s",defValue:"%s"}' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'],schema[f]['columns'][k]['type'],schema[f]['columns'][k]['length'],schema[f]['columns'][k]['defValue'])
            else:
                print '   {name:"%s",desc:"%s",optional:"%s",type:"%s",length:"%s",defValue:"%s"},' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'],schema[f]['columns'][k]['type'],schema[f]['columns'][k]['length'],schema[f]['columns'][k]['defValue'])
                num_attrib -= 1

        print '  ]'

        if num_feat == 1: # Are we at the last feature? yes = no trailing comma
            print ' }'
        else:
            print ' },'
            num_feat -= 1
    print '] // End of %s.thematicSchema' % (spec)
    print
    # print 'exports.getthematicSchema = %s.schema.getThematicSchema;' % (spec)
    print# End printThematic

# Modified printing function to print the Thematic Enum Schema
def printThematicEnum(schema, spec):
    print 'var thematicSchema = [' # And so it begins...

    num_feat = len(schema.keys()) # How many features in the schema?
    for f in sorted(schema.keys()):
        # Skip all of the 'Table' features and features without geometry
        if schema[f]['geom'] == 'Table' or schema[f]['geom'] == 'None':
            continue
        pString = ' {name:"%s",fcode:"%s",desc:"%s",geom:"%s",' % (f,schema[f]['fcode'],schema[f]['desc'],schema[f]['geom']); # name = geom + FCODE
        if 'fdname' in schema[f]:
            pString += 'fdname:"%s",' % (schema[f]['fdname'])
        if 'thematic' in schema[f]:
            pString += 'thematic:"%s",' % (schema[f]['thematic'])

        print pString
        print '  columns:['

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            if 'func' in schema[f]['columns'][k]:
                if num_attrib > 1:  # Are we at the last attribute? yes = no trailing comma
                    print '   {name:"%s",desc:"%s",optional:"%s",type:"%s",defValue:"%s",enumerations: %s},' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'],schema[f]['columns'][k]['type'], schema[f]['columns'][k]['defValue'],schema[f]['columns'][k]['func'])
                    num_attrib -= 1
                else:
                    print '   {name:"%s",desc:"%s",optional:"%s",type:"%s",defValue:"%s",enumerations: %s}' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'],schema[f]['columns'][k]['type'], schema[f]['columns'][k]['defValue'],schema[f]['columns'][k]['func'])

            elif schema[f]['columns'][k]['type'] == 'enumeration':
                print '   {name:"%s",desc:"%s",optional:"%s",type:"enumeration",defValue:"%s",' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'],schema[f]['columns'][k]['defValue'])
                print '    enumerations:['
                num_enum = len(schema[f]['columns'][k]['enum']) # How many attributes does the feature have?
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
            elif num_attrib == 1: # Are we at the last attribute? yes = no trailing comma
                print '   {name:"%s",desc:"%s",optional:"%s",type:"%s",length:"%s",defValue:"%s"}' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'],schema[f]['columns'][k]['type'],schema[f]['columns'][k]['length'],schema[f]['columns'][k]['defValue'])
            else:
                print '   {name:"%s",desc:"%s",optional:"%s",type:"%s",length:"%s",defValue:"%s"},' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'],schema[f]['columns'][k]['type'],schema[f]['columns'][k]['length'],schema[f]['columns'][k]['defValue'])
                num_attrib -= 1
        print '  ]'

        if num_feat == 1:
            print ' }'
        else:
            print ' },'
            num_feat -= 1
    print '] // End of %s.thematicSchema' % (spec)
    print


# Go through the schema and pull out all of the attributes that are to be replaced with
# functions
def printFuncList(schema):
    printList = {}

    for i in schema:
        for j in schema[i]['columns']:
            if 'func' in schema[i]['columns'][j]:
                tName = schema[i]['columns'][j]['func']
                if tName not in printList:
                    # printList[tName] = schema[i]['columns'][j]['enum']
                    printList[tName] = []
                    printList[tName].extend(schema[i]['columns'][j]['enum'])

    for i in printList:
        print 'var %s = [' % (i)
        num_vals = len(printList[i]) # How many values does the thing have?
        for k in printList[i]:
            if num_vals == 1: # Are we at the last feature? yes = no trailing comma
                print ' {name:"%s",value:"%s"}' % (k['name'],k['value'])
            else:
                print ' {name:"%s",value:"%s"},' % (k['name'],k['value'])
                num_vals -= 1

        print '];'
        print
# End printFuncList


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


# Go through the schema and add attributes for imagery tags
def addExtraAttrs(tSchema):
    for featureName in tSchema:
        tSchema[featureName]['columns']['image_id'] = {}
        tSchema[featureName]['columns']['image_id'] = {'name':'image_id','desc':'Image Id','type':'String',
                                                'defValue':'noInformation','length':'254','optional':'R'}
        tSchema[featureName]['columns']['legacy_id'] = {}
        tSchema[featureName]['columns']['legacy_id'] = {'name':'legacy_id','desc':'Legacy Image Id','type':'String',
                                                'defValue':'noInformation','length':'254','optional':'R'}
        tSchema[featureName]['columns']['early_date'] = {}
        tSchema[featureName]['columns']['early_date'] = {'name':'early_date','desc':'Earliest image date in mosaic','type':'String',
                                                'defValue':'noInformation','length':'254','optional':'R'}
        tSchema[featureName]['columns']['late_date'] = {}
        tSchema[featureName]['columns']['late_date'] = {'name':'late_date','desc':'Latest image date in mosaic','type':'String',
                                                'defValue':'noInformation','length':'254','optional':'R'}
        tSchema[featureName]['columns']['img_layer'] = {}
        tSchema[featureName]['columns']['img_layer'] = {'name':'img_layer','desc':'Imagery Layer Name','type':'String',
                                                'defValue':'noInformation','length':'254','optional':'R'}
        tSchema[featureName]['columns']['img_mosaic'] = {}
        tSchema[featureName]['columns']['img_mosaic'] = {'name':'img_mosaic','desc':'Image Mosaic','type':'String',
                                                'defValue':'no','length':'3','optional':'R'}
        tSchema[featureName]['columns']['PFI'] = {}
        tSchema[featureName]['columns']['PFI'] = {'name':'PFI','desc':'Acquisition Platform Identifier','type':'String',
                                                'defValue':'noInformation','length':'15','optional':'R'}

    return tSchema
# End addExtraAttrs


def readNames(fileName):
    csvfile = openFile(fileName, 'rb')
    reader = csv.reader(csvfile, delimiter='#')

    tValues = {}
    for (name,desc,definition) in reader:
        tValues[name] = {'desc':desc, 'definition':definition}

    return tValues
# End readnames


# Add names and descriptions to the schema
def addNames(namList,tSchema):
    for feature in tSchema:
        fCode = tSchema[feature]['fcode']
        if fCode != '' and fCode in namList:
            tSchema[feature]['desc'] = namList[fCode]['desc']
            tSchema[feature]['definition'] = namList[fCode]['definition']
        # else:
        #     print 'Missing fcode ',fCode,' ',feature,'  ',tSchema[feature]['desc']

        for field in tSchema[feature]['columns']:
            aName = tSchema[feature]['columns'][field]['name']

            if aName in namList:
                tSchema[feature]['columns'][field]['desc'] = namList[field]['desc']
                if namList[field]['definition'] != '':
                    tSchema[feature]['columns'][field]['definition'] = namList[field]['definition']
            else:
                print 'Missing field ',field,' ',tSchema[feature]['columns'][field]['desc']

    return tSchema
# End addNames


# Fixup the default values in the thematic schema
def fixDefaults():
    defList = {}

    for feature in schema:
        for field in schema[feature]['columns']:
            aName = schema[feature]['columns'][field]['name']

            if aName == 'F_CODE' or aName == 'FCSUBTYPE' or aName == 'GLOBALID':
                continue

            if aName in defList:  # Sanity check
                # One different value left that we are going to ignore
                # if defList[aName] != schema[feature]['columns'][field]['defValue']:
                #     print '## ',schema[feature]['name'],'different',aName,defList[aName],'vs',schema[feature]['columns'][field]['defValue']
                continue

            defList[aName] = schema[feature]['columns'][field]['defValue']

    for feature in thematicSchema:
        for field in thematicSchema[feature]['columns']:
            aName = thematicSchema[feature]['columns'][field]['name']

            if aName == 'F_CODE' or aName == 'FCSUBTYPE' or aName == 'GLOBALID':
                continue

            if aName not in defList: # Sanity check
                # print '## ',thematicSchema[feature]['name'],'Missing default value for',aName
                continue

            thematicSchema[feature]['columns'][field]['defValue'] = defList[aName]
            thematicEnumSchema[feature]['columns'][field]['defValue'] = defList[aName]

        # Remove the default FCSUBTYPE from the thematic layers.  This gets populated from the individual F_CODES
        thematicSchema[feature]['columns']['FCSUBTYPE']['defValue'] = ''
        thematicEnumSchema[feature]['columns']['FCSUBTYPE']['defValue'] = ''

    thematicSchema['MaximumElevationSrf']['columns']['MAX_TERRAIN']['defValue'] = '-999999'
    thematicSchema['MaximumElevationSrf']['columns']['MAX_ELEVATION']['defValue'] = '-999999'
    thematicSchema['MaximumElevationSrf']['columns']['MEF_VALUE']['defValue'] = '-999999'

    thematicEnumSchema['MaximumElevationSrf']['columns']['MAX_TERRAIN']['defValue'] = '-999999'
    thematicEnumSchema['MaximumElevationSrf']['columns']['MAX_ELEVATION']['defValue'] = '-999999'
    thematicEnumSchema['MaximumElevationSrf']['columns']['MEF_VALUE']['defValue'] = '-999999'


# End fixDefaults


# Fixup the enumerations for enum types in the thematic enum schema
def fixEnums():
    enumList = {}

    for feature in schema:
        for field in schema[feature]['columns']:
            aName = schema[feature]['columns'][field]['name']

            if aName in enumList:
                continue
            
            if schema[feature]['columns'][field]['type'] == 'enumeration':
                enumList[aName] = schema[feature]['columns'][field]['enum']

    for feature in thematicEnumSchema:
        for field in thematicEnumSchema[feature]['columns']:
            aName = thematicEnumSchema[feature]['columns'][field]['name']

            if aName not in enumList:
                continue

            thematicEnumSchema[feature]['columns'][field]['type'] = 'enumeration'
            thematicEnumSchema[feature]['columns'][field]['enum'] = []
            thematicEnumSchema[feature]['columns'][field]['enum'] = enumList[aName]

# End fixEnums

    # Setup handy lists
geoList = {'C':'Line','Crv':'Line','S':'Area','Srf':'Area','P':'Point','Pnt':'Point','_':'None'}
typList = {'esriFieldTypeDouble':'Real','esriFieldTypeString':'String','esriFieldTypeInteger':'Integer'}

# Pull all of the Domain values from XML document
def readDomains(xmlDoc):
    tDomain = {}
    itemList = xmlDoc.getElementsByTagName('Domain')

    for domain in itemList:
        domainName = domain.getElementsByTagName('DomainName')[0].firstChild.data.encode('utf8')
        domainType = typList[domain.getElementsByTagName('FieldType')[0].firstChild.data.encode('utf8')]

        # Debug
        # print "domainName:",domainName

        if domainName in tDomain:
            # print('domain: clash ' + domainName)
            continue

        tDomain[domainName] = {}
        tDomain[domainName]['type'] = typList[domain.getElementsByTagName('FieldType')[0].firstChild.data.encode('utf8')]
        tDomain[domainName]['enum'] = []

        if domain.getElementsByTagName('CodedValues'):
            tDomain[domainName]['enum'] = []
            for value in domain.getElementsByTagName('CodedValue'):
                vName = value.getElementsByTagName('Name')[0].firstChild.data.encode('utf8').replace('"','')
                vCode = value.getElementsByTagName('Code')[0].firstChild.data.encode('utf8')
                tDomain[domainName]['enum'].append({'name':vName,'value':vCode})

        # else:
            # print '# No Enum:',domainName

    return tDomain
# End readDomains


def testFeatures(xmlDoc,funcList,domList,namList,fSchema,tSchema):
    itemList = xmlDoc.getElementsByTagName('DataElement')

    for feature in itemList:
        featureName = feature.getElementsByTagName('Name')[0].firstChild.data.encode('utf8')

        print ''
        print "featureName:",featureName
        print 'typNS', repr(feature.getAttributeNS('http://www.w3.org/2001/XMLSchema-instance','type'))

        # for atr in feature.attributes.values():
        #     print 'aNameS', repr(atr.namespaceURI)
        #     print 'aLname', repr(atr.localName)
        #     print 'apFix', repr(atr.prefix)

        # Drop this one. It seems to be the catch all
        if feature.getAttributeNS('http://www.w3.org/2001/XMLSchema-instance','type') == 'esri:DEFeatureDataset':
        # if featureName == 'TDS' or featureName == 'TDS_CARTO':
            print 'Got DEFeatureDataset'
            continue

        subTypeList = feature.getElementsByTagName('Subtype')
        if subTypeList != 'None':
            for subType in subTypeList:
                featureName = subType.getElementsByTagName('SubtypeName')[0].firstChild.data.encode('utf8')
                print ''
                print 'fName:',featureName

                # Build the basic feature
                print 'name',featureName.replace('_',' ').title().encode('utf8')
                print 'geom',geoList[featureName[-1]]

                # Now add fields to the feature
                fieldList = feature.getElementsByTagName('SubtypeFieldInfo')
                if fieldList != 'None':
                    for field in fieldList:
                        fieldName = field.getElementsByTagName('FieldName')[0].firstChild.data.encode('utf8')
                        defaultValueNode = field.getElementsByTagName('DefaultValue')[0]
                        # defaultValue = field.getElementsByTagName('DefaultValue')[0].firstChild.data
                        # defaultValue = defaultValueNode.firstChild.data

                        # print 'dvType',defaultValueNode.getAttributeNS('http://www.w3.org/2001/XMLSchema-instance','type')

                        if field.getElementsByTagName('DomainName'):
                            domainName = field.getElementsByTagName('DomainName')[0].firstChild.data.encode('utf8')
                        else:
                            domainName = ''

                        # Now start building the attribute
                        print '  name',fieldName.encode('utf8')
                        # print '  desc',''
                        print '  Xtype',defaultValueNode.getAttributeNS('http://www.w3.org/2001/XMLSchema-instance','type')
                        print '  defValue',defaultValueNode.firstChild.data.encode('utf8')
                        print '  domain',domainName


# Read all of the features in an XML document
def readFeatures(xmlDoc,funcList,domList,namList,tfList,fSchema,tSchema,tESchema):
    # Setup handy lists
    geoList = {'C':'Line','Crv':'Line','S':'Area','Srf':'Area','P':'Point','Pnt':'Point','_':'None'}
    typList = {'esriFieldTypeDouble':'Real','xs:double':'Real','esriFieldTypeString':'String','xs:string':'String',
               'esriFieldTypeInteger':'Integer','xs:int':'Integer','esriFieldTypeGlobalID':'String'}
    fixDefList = {'ZI013_CSP':'-999999','DOF':'-999999.0','ZI001_SPS':'-999999','CIW':'-999999','ZSAX_RX3':'noInformation'}

    itemList = xmlDoc.getElementsByTagName('DataElement')

    for feature in itemList:
        featureName = feature.getElementsByTagName('Name')[0].firstChild.data.encode('utf8')

        # Debug
        # print "featureName:",featureName

        featureType = feature.getAttributeNS('http://www.w3.org/2001/XMLSchema-instance','type')

        # Skip Tables
        if featureType == 'esri:DETable':
            continue

        if featureType == 'esri:DEFeatureDataset':
        # if featureName == 'TDS' or featureName == 'TDS_CARTO':
            fdName = featureName
            # print 'Got DEFeatureDataset:',fdName
            continue

        # Build the basic feature
        tSchema[featureName] = {}
        tSchema[featureName]['name'] = featureName
        tSchema[featureName]['geom'] = geoList[featureName[-3]]
        tSchema[featureName]['fcode'] = ''
        tSchema[featureName]['fdname'] = fdName
        tSchema[featureName]['fcList'] = []
        tSchema[featureName]['columns'] = {}

        tESchema[featureName] = {}
        tESchema[featureName]['name'] = featureName
        tESchema[featureName]['geom'] = geoList[featureName[-3]]
        tESchema[featureName]['fcode'] = ''
        tESchema[featureName]['fdname'] = fdName
        tESchema[featureName]['fcList'] = []
        tESchema[featureName]['columns'] = {}

        # There has got to be a better way to do this
        for node in feature.childNodes:
            # print 'cn:',node.nodeName
            if node.nodeName == 'AliasName':
                tSchema[featureName]['desc'] = node.firstChild.data.encode('utf8')
                tESchema[featureName]['desc'] = node.firstChild.data.encode('utf8')
                # print '  Alias:',tSchema[featureName]['desc']
                break

        # Debug
        # print '#######################################'
        # print 'DE Name: ',featureName
        fieldValueList = feature.getElementsByTagName('Field')
        if fieldValueList != 'None':
            for fieldValue in fieldValueList:
                # fName = fieldValue.getElementsByTagName('Name')[0].firstChild.data
                fName = fieldValue.getElementsByTagName('ModelName')[0].firstChild.data.encode('utf8').upper()

                # Skip features we don't want
                if fName in ['OBJECTID','SHAPE','SHAPE_LENGTH','SHAPE_AREA']:
                    # Debug
                    # print 'Skipped: ', fName
                    continue

                # print 'Field:', fName

                fType = fieldValue.getElementsByTagName('Type')[0].firstChild.data.encode('utf8')
                length = fieldValue.getElementsByTagName('Length')[0].firstChild.data.encode('utf8')
                if fieldValue.getElementsByTagName('AliasName'):
                    desc = fieldValue.getElementsByTagName('AliasName')[0].firstChild.data.encode('utf8')
                else:
                    desc = ''

                if fName not in namList:
                    # print '# Adding nam:',fName,' = ',desc
                    namList[fName] = {}
                    namList[fName]['desc'] = desc
                    namList[fName]['definition'] = ''
                # else:
                #     if namList[fName]['desc'] != desc:
                #         print 'field:',fName
                #         print' nam:',namList[fName]['desc']
                #         print' des:',desc

                # These are OK. We have filtered out the features that don't have these
                if fieldValue.getElementsByTagName('DefaultValue'):
                    defaultValue = fieldValue.getElementsByTagName('DefaultValue')[0].firstChild.data.encode('utf8')
                else:
                    defaultValue = ''

                # Now start building the attribute
                tSchema[featureName]['columns'][fName] = {}
                tSchema[featureName]['columns'][fName] = { 'name':fName,
                                                        'desc':desc,
                                                        'type':typList[fType],
                                                        'defValue':defaultValue,
                                                        'length':length,
                                                        'optional':'R'
                                                      }
                
                if fieldValue.getElementsByTagName('DefaultValue'):
                    defaultValueNode = fieldValue.getElementsByTagName('DefaultValue')[0]
                    fType = defaultValueNode.getAttributeNS('http://www.w3.org/2001/XMLSchema-instance','type')

                tESchema[featureName]['columns'][fName] = {}
                tESchema[featureName]['columns'][fName] = { 'name':fName,
                                                        'desc':desc,
                                                        'type':typList[fType],
                                                        'defValue':defaultValue,
                                                        'length':length,
                                                        'optional':'R'
                                                      }

                # Defensive. Addint an OTH
                if 'OTH' not in tSchema[featureName]['columns']:
                    # print '## Missing OTH: ', subName
                    tSchema[featureName]['columns']['OTH'] = {'name':'OTH','desc':'Specified Domain Value(s)','type':'String',
                                                          'defValue':'noInformation','length':'255','optional':'R'}
                    tESchema[featureName]['columns']['OTH'] = {'name':'OTH','desc':'Specified Domain Value(s)','type':'String',
                                                          'defValue':'noInformation','length':'255','optional':'R'}
                    
                if fName in funcList:
                    tESchema[featureName]['columns'][fName]['func'] = 'full_' + fName

                if fName in tfList:
                    tESchema[featureName]['columns'][fName]['func'] = 'truefalse'

                if 'ZI020_GE' in fName:
                    tESchema[featureName]['columns'][fName]['func'] = 'full_ZI020_GE4'

                if 'ZSAX_RS0' in fName or 'ZI020_GE4' in fName:
                    tESchema[featureName]['columns'][fName]['type'] = 'textEnumeration'

                # Debug
                # print 'Name:',fName.encode('utf8'),'  Type:',fType.encode('utf8')
                # print 'Default:',defaultValue.encode('utf8')
                # print 'Length:',length.encode('utf8')

        subTypeList = feature.getElementsByTagName('Subtype')
        if subTypeList != 'None':
            for subType in subTypeList:
                subName = subType.getElementsByTagName('SubtypeName')[0].firstChild.data.encode('utf8')
                # print 'subName:',subName

                # Build the basic feature
                fSchema[subName] = {}
                fSchema[subName]['name'] = subName
                fSchema[subName]['geom'] = geoList[subName[-1]]
                fSchema[subName]['desc'] = subName[:-2].replace('_',' ').title()
                fSchema[subName]['thematic'] = featureName # What this layer belongs to
                fSchema[subName]['fcsubtype'] = ''
                fSchema[subName]['fdname'] = fdName
                fSchema[subName]['fcode'] = ''
                fSchema[subName]['columns'] = {}

                # Now add fields to the feature
                fieldList = subType.getElementsByTagName('SubtypeFieldInfo')
                if fieldList != 'None':
                    for field in fieldList:
                        fieldName = field.getElementsByTagName('FieldName')[0].firstChild.data.encode('utf8').upper()

                        defaultValueNode = field.getElementsByTagName('DefaultValue')[0]

                        if fieldName == 'F_CODE':
                            fSchema[subName]['fcode'] = defaultValueNode.firstChild.data.encode('utf8')

                        # print 'fieldName',fieldName
                        fType = defaultValueNode.getAttributeNS('http://www.w3.org/2001/XMLSchema-instance','type')
                        defValue = defaultValueNode.firstChild.data.encode('utf8')

                        if fType == 'xs:double':  # For some reason, this is an int value in the XML
                            defValue = '-999999.0'

                        # Mass changes to some values
                        if fieldName in fixDefList:
                            defValue = fixDefList[fieldName]

                        # Now start building the attribute
                        fSchema[subName]['columns'][fieldName] = {}
                        fSchema[subName]['columns'][fieldName] = { 'name':fieldName,
                                                                'desc':'',
                                                                'type':typList[fType],
                                                                'defValue':defValue,
                                                                'optional':'R'
                                                              }
                        
                        if fieldName == 'FCSUBTYPE':
                            #fSchema[subName]['fcsubtype'] = defaultValueNode.firstChild.data.encode('utf8')
                            fSchema[subName]['fcsubtype'] = fSchema[subName]['fcode'] + '_' + fSchema[subName]['desc'].replace(' ','_') + '_' + fSchema[subName]['geom']
                            fSchema[subName]['columns']['FCSUBTYPE'] = { 'name':'FCSUBTYPE','desc':'Feature Class Subtype','type':'String','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','defValue':fSchema[subName]['fcsubtype']}
                            #tSchema[fSchema[subName]['thematic']]['columns']['FCSUBTYPE'] = { 'name':'FCSUBTYPE','desc':'Feature Class Subtype','type':'String','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','defValue':fSchema[subName]['fcsubtype']}
                            #tESchema[fSchema[subName]['thematic']]['columns']['FCSUBTYPE'] = { 'name':'FCSUBTYPE','desc':'Feature Class Subtype','type':'String','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','defValue':fSchema[subName]['fcsubtype']}

                        # Start filling in the domains
                        if field.getElementsByTagName('DomainName') and fieldName != 'F_CODE':
                            domainName = field.getElementsByTagName('DomainName')[0].firstChild.data.encode('utf8')

                            if domainName == 'Mst_Null_Integer' or domainName == 'Mst_Null_Double' or domainName == 'Mst_Null_String':
                                # print ' ',subName,'Dropping: ',fieldName
                                del fSchema[subName]['columns'][fieldName]
                                continue

                            if fieldName in funcList:
                                fSchema[subName]['columns'][fieldName]['func'] = 'full_' + fieldName

                            if fieldName in tfList:
                                fSchema[subName]['columns'][fieldName]['func'] = 'truefalse'

                            if 'ZI020_GE' in fieldName:
                                fSchema[subName]['columns'][fieldName]['func'] = 'full_ZI020_GE4'

                            fSchema[subName]['columns'][fieldName]['domain'] = domainName

                            if domainName == 'BG010_DOF':
                                fSchema[subName]['columns'][fieldName]['type'] = 'Integer'
                                fSchema[subName]['columns'][fieldName]['defValue'] = '-999999'
                                continue

                            # print 'dName:',domainName
                            if domainName not in domList:
                                print '## Missing Domain',domainName,'from',fieldName,'in',subName
                                continue

                            if fSchema[subName]['columns'][fieldName]['type'] == 'String':
                                fSchema[subName]['columns'][fieldName]['type'] = 'textEnumeration'
                            else:
                                fSchema[subName]['columns'][fieldName]['type'] = 'enumeration'

                            fSchema[subName]['columns'][fieldName]['enum'] = []
                            fSchema[subName]['columns'][fieldName]['enum'].extend(domList[domainName]['enum'])
                            # print 'dd:',subName,fieldName,domainName,repr(fSchema[subName]['columns'][fieldName]['enum'])

                if fSchema[subName]['fcode'] == '':
                    nameList = {'FLOW_ARROW_P':'FLWAR','VERTICAL_OBSTRUCTION_P':'VOBST','MAXIMUM_ELEVATION_S':'MXELE',
                                'UNESCOW_P':'UNSCW','ROAD_MARKER_P':'RDMKR','LEADER_LINE_C':'LDRLN'}
                    # print 'Empty FCODE: ',subName
                    fSchema[subName]['fcode'] = nameList[subName]

                # Defensive. Addint an OTH
                if 'OTH' not in fSchema[subName]['columns']:
                    # print '## Missing OTH: ', subName
                    fSchema[subName]['columns']['OTH'] = {'name':'OTH','desc':'Specified Domain Value(s)','type':'String',
                                                          'defValue':'noInformation','length':'255','optional':'R'}

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
    parser.add_argument('--intattr', help='Dump out all attributes that are integers',action='store_true')
    parser.add_argument('--nocodedvalues', help='Generate a Thematic Schema with no coded values',action='store_true')
    parser.add_argument('--numrules', help='Dump out number rules',action='store_true')
    parser.add_argument('--rules', help='Dump out one2one rules',action='store_true')
    parser.add_argument('--thematic', help='Generate a Thematic Schame',action='store_true')
    parser.add_argument('--toenglish', help='Dump out To English translation rules',action='store_true')
    parser.add_argument('--txtlen', help='Dump out the lengths of all of the text attributes',action='store_true')
    parser.add_argument('--txtrules', help='Dump out text rules',action='store_true')
    parser.add_argument('--withdefs', help='Add feature ad attribute definitions to the schema',action='store_true')
    parser.add_argument('xmlFile', help='The XML Schema file', action='store')
    # parser.add_argument('namesFile', help='CSV file of names and descriptions', action='store')

    args = parser.parse_args()

    # Decide if we are going to print the definition for each feature and attribute
    withDefs = False
    if args.withdefs:
        withDefs = True


    # Apparently, reding to a string and then parseing is quicker
    if args.xmlFile.endswith(".gz"):
        infile =  gzip.open(args.xmlFile, 'rb')
    else:
        infile = open(args.xmlFile, 'rb')

    content = infile.read()

    # xmlDoc = minidom.parse(args.xmlFile)
    xmlDoc = minidom.parseString(content)
    # print(xmlDoc.toprettyxml(indent = '  ',encoding = 'UTF-8'))

    # namesCsvFile = args.namesFile
    namesList = {}

    # namesList = readNames(namesCsvFile)
    # testFeatures(xmlDoc,funcList,domainList,schema,thematicSchema)

    domainList = {}
    domainList = readDomains(xmlDoc)
    # print '#####'
    # for i in sorted(domainList):
    #     print 'nam:',i,'  ',repr(domainList[i])
    # print '#####'

    # sys.exit()

    # The list of attributes to make into functions in the schema. Used for enumerated lists that get
    # repeated a lot.
    funcList = ['ZI001_SRT','ZI004_RCG','ZI026_CTUC','SGCC','ZSAX_RS0','CPS','ETS','HZD','VDT']

    # The list of attributes that are True/False/NoInfo to be made into a function
    tfList = ['AWP', 'AZC', 'BSM', 'CBP', 'CDA', 'CIW', 'CST', 'CWT', 'DMD', 'FAC', 'FLO', 'FSC', 'GFT', 'GUG', 'LMC',
    'LUN', 'MAN', 'MCX', 'MES', 'MNS', 'ONE', 'OWO', 'PEC', 'PEC', 'PRM', 'RFD', 'RIP', 'ROR', 'SAD', 'SBB', 'SBC', 'SEP',
    'SPT', 'THR', 'TID', 'TRA', 'ZI001_SPS', 'ZI013_PIG', 'ZI017_RIR', 'ZI032_GUG']


    thematicSchema = {}
    thematicEnumSchema = {}
    schema = {}

    # Populate the two schema
    readFeatures(xmlDoc,funcList,domainList,namesList,tfList,schema,thematicSchema,thematicEnumSchema)

    # Add names and descriptions to elements
    schema = addNames(namesList,schema)
    thematicSchema = addNames(namesList,thematicSchema)
    thematicEnumSchema = addNames(namesList,thematicEnumSchema)

    # Fix default values in the schema
    fixDefaults()

    # Fix enumerations in the schema
    fixEnums()

    # Add extra attributes to each feature
    schema = addExtraAttrs(schema)
    thematicSchema = addExtraAttrs(thematicSchema)
    thematicEnumSchema = addExtraAttrs(thematicEnumSchema)

    # Now dump the schema out
    if args.rules:
        printRules(schema,True)
    elif args.txtrules:
        printTxtRules(schema)
    elif args.txtlen:  # Print all of the lengths of the text attributes
        printTxtLen(schema)
    elif args.numrules:
        printNumRules(schema)
    elif args.intattr:  # Integer attributes
        printIntAttr(schema)
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
        printToEnglish(schema,'TDSv71','etds71','FCODE')
    elif args.fromenglish:
        printCopyright()
        printFromEnglish(schema,'TDSv71','etds71_osm_rules')
    elif args.attributecsv:
        printAttributeCsv(schema)
    elif args.dumpenum:
        dumpEnumerations(schema,'enumTDS7')
    elif args.fullschema:
        printCopyright()
        if args.thematic:
            printThematic(thematicSchema,'tds71')
        else:
            printJSHeader('tds71')
            printFuncList(schema)
            printJavascript(schema,withDefs)
            printJSFooter('tds71')
    else:
        printCopyright()
        if args.nocodedvalues and args.thematic:
            printThematicEnumHeader('tds71')
            convertTextEnumerations(thematicEnumSchema)
            printFuncList(thematicEnumSchema)
            printThematicEnum(thematicEnumSchema,'tds71')
            printThematicEnumFooter('tds71')
        elif args.thematic:
            # convertTextEnumerations(thematicSchema)
            # printFuncList(thematicSchema)
            printThematic(thematicSchema,'tds71')
        else:
            printJSHeader('tds71')
            convertTextEnumerations(schema)
            printFuncList(schema)
            printJavascript(schema,withDefs)
            printJSFooter('tds71')
# End



