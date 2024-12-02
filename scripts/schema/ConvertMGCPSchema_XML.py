#!/usr/bin/python

# Parse a MGCP XML file into a schema
import os,sys,argparse,gzip

# This is where the common functions live
from hootLibrary import *

from xml.dom import minidom


# printJavascript: Dump out the structure as Javascript
#
# Note: This uses double quotes ( " ) around data elements in the output.  The csv files have values with
# single quotes ( ' ) in them.  These quotes are also in the DFDD and NFDD specs.
def printJavascript(schema):
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
        if 'definition' in schema[f]:
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

            if 'definition' in schema[f]['columns'][k]:
                aString += 'definition:"%s",' % (schema[f]['columns'][k]['definition'])

            if 'func' in schema[f]['columns'][k]:
                aString += 'type:"enumeration",defValue:"%s",enumerations: %s}' % (schema[f]['columns'][k]['defValue'],schema[f]['columns'][k]['func'])
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

# Modified printing function to print the Thematic Enum Header
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
        if 'definition' in schema[f]:
            pString += 'definition:"%s",' % (schema[f]['definition'])

        print pString
        print '  columns:['

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            if schema[f]['columns'][k]['type'] == 'enumeration':
                aType = 'Integer'
            else:
                aType = schema[f]['columns'][k]['type']

            aString = '   {name:"%s",desc:"%s",optional:"%s",type:"%s",defValue:"%s"' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'],aType,schema[f]['columns'][k]['defValue'])

            if 'length' in schema[f]['columns'][k]:
                aString += ',length:"%s"' % (schema[f]['columns'][k]['length'])

            if 'definition' in schema[f]['columns'][k]:
                aString += ',definition:"%s",' % (schema[f]['columns'][k]['definition'])

            if num_attrib == 1:  # Are we at the last attribute? yes = no trailing comma
                aString += '}'
            else:
                aString += '},'
                num_attrib -= 1
            print aString
        print '  ]'

        if num_feat == 1: # Are we at the last feature? yes = no trailing comma
            print ' }'
        else:
            print ' },'
            num_feat -= 1
    print '] // End of %s.thematicSchema' % (spec)
    print
    # print 'exports.getthematicSchema = %s.schema.getThematicSchema;' % (spec)
    print
# End printThematic

# Modified printing function to print the Thematic Enum Schema
def printThematicEnum(schema, spec):
    print 'var thematicSchema = ['  # And so it begins...

    num_feat = len(schema.keys()) # How many features in the schema?
    for f in sorted(schema.keys()):
        # Skip all of the 'Table' features and features without geometry
        if schema[f]['geom'] == 'Table' or schema[f]['geom'] == 'None':
            continue

        pString = ' {name:"%s",fcode:"%s",desc:"%s",geom:"%s",' % (f,schema[f]['fcode'],schema[f]['desc'],schema[f]['geom'])
        if 'fcsubtype' in schema[f]:
            pString += 'fcsubtype:"%s",' % (schema[f]['fcsubtype'])
        if 'fdname' in schema[f]:
            # print '  fdname:"%s",' % (schema[f]['fdname'])
            pString += 'fdname:"%s",' % (schema[f]['fdname'])
        if 'thematic' in schema[f]:
            pString += 'thematic:"%s",' % (schema[f]['thematic'])
        if 'definition' in schema[f]:
            pString += 'definition:"%s",' % (schema[f]['definition'])

        print pString
        print '  columns:['

        num_attrib = len(schema[f]['columns'].keys()) # How many attributes does the feature have?
        for k in sorted(schema[f]['columns'].keys()):
            aString = '   {name:"%s",desc:"%s",optional:"%s",' % (k,schema[f]['columns'][k]['desc'],schema[f]['columns'][k]['optional'])

            if 'length' in schema[f]['columns'][k]:
                aString += 'length:"%s",' % (schema[f]['columns'][k]['length'])

            if 'definition' in schema[f]['columns'][k]:
                aString += 'definition:"%s",' % (schema[f]['columns'][k]['definition'])

            if 'func' in schema[f]['columns'][k]:
                aString += 'type:"enumeration",defValue:"%s",enumerations: %s}' % (schema[f]['columns'][k]['defValue'],schema[f]['columns'][k]['func'])
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

# Add Additional features that are not specified in the TRD
def addExtra(schema):
    schema['LCA010'] = {}
    schema['LCA010'] = {'name':'LCA010','fcode':'CA010','desc':'Elevation Contour','geom':'Line','fcsubtype':'CA010_Elevation_Contour_Line','fdname':'MGCP_Delta','thematic':'ContourL'}
    schema['LCA010']['columns'] = {}
    schema['LCA010']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':'CA010_Elevation_Contour_Line'}
    schema['LCA010']['columns']['ZVH'] = {'name':'ZVH','desc':'Highest Elevation','type':'Real','optional':'R','defValue':'-999999.0'}
    schema['LCA010']['columns']['ESC'] = {'name':'ESC','desc':'Elevation Surface Category','optional':'R','type':'enumeration','defValue':'1'}
    schema['LCA010']['columns']['ESC']['enum'] = [{'name':'Unknown','value':'0'},{'name':'Land','value':'1'},{'name':'Snow Field and/or Ice-field','value':'2'}]
    schema['LCA010']['columns']['HQC'] = {'name':'HQC','desc':'Hypsography Portrayal Type','optional':'R','type':'enumeration','defValue':'0'}
    schema['LCA010']['columns']['HQC']['enum'] = [
     {'name':'Unknown','value':'0'}, {'name':'Index Contour','value':'1'}, {'name':'Intermediate Contour','value':'2'},
     {'name':'Half Auxiliary Contour','value':'3'}, {'name':'Depression Index Contour','value':'5'}, {'name':'Depression Intermediate Contour','value':'6'},
     {'name':'Mound Index Contour','value':'8'}, {'name':'Mound Intermediate Contour','value':'9'}, {'name':'Quarter Auxiliary Contour','value':'14'},
     {'name':'Intermediate Carrying Contour','value':'19'}, {'name':'Auxiliary Carrying Contour','value':'20'}, {'name':'Index Carrying Contour','value':'21'},
     {'name':'Depression Auxiliary Contour','value':'22'}, {'name':'Other','value':'999'}
    ]

    schema['PCA030'] = {}
    schema['PCA030'] = {'name':'PCA030','fcode':'CA030','desc':'Spot Elevation','geom':'Point','fcsubtype':'CA030_Spot_Elevation_Point','fdname':'MGCP_Delta','thematic':'ElevP'}
    schema['PCA030']['columns'] = {}
    schema['PCA030']['columns']['BEL'] = {'name':'BEL','desc':'Base Elevation','optional':'R','type':'Real','defValue':'-999999.0'}
    schema['PCA030']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':'CA030_Spot_Elevation_Point'}
    schema['PCA030']['columns']['NAM'] = {'name':'NAM','desc':'Name','optional':'R','type':'String','defValue':'UNK'}
    schema['PCA030']['columns']['NFI'] = {'name':'NFI','desc':'Named Feature Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['PCA030']['columns']['NFN'] = {'name':'NFN','desc':'Name Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['PCA030']['columns']['ELA'] = {'name':'ELA','desc':'Elevation Accuracy Category','optional':'R','type':'enumeration','defValue':'0'}
    schema['PCA030']['columns']['ELA']['enum'] = [{'name':'Unknown','value':'0'}, {'name':'Accurate','value':'1'}, {'name':'Approximate','value':'2'}]
    schema['PCA030']['columns']['ESC'] = {'name':'ESC','desc':'Elevation Surface Category','optional':'R','type':'enumeration','defValue':'1'}
    schema['PCA030']['columns']['ESC']['enum'] = [{'name':'Unknown','value':'0'}, {'name':'Land','value':'1'}, {'name':'Snow Field and/or Ice-field','value':'2'},
     {'name':'Vegetation','value':'4'}, {'name':'Inland Water','value':'5'}, {'name':'Tidal Water','value':'6'}
     ]

    schema['PCA035'] = {}
    schema['PCA035'] = {'name':'PCA035','fcode':'CA035','desc':'Inland Water Elevation','geom':'Point','fcsubtype':'CA035_Inland_Water_Elevation_Point','fdname':'MGCP_Delta','thematic':'ElevP'}
    schema['PCA035']['columns'] = {}
    schema['PCA035']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':'CA035_Inland_Water_Elevation_Point'}
    schema['PCA035']['columns']['ZVH'] = {'name':'ZVH','desc':'Highest Elevation','optional':'R','type':'Real','defValue':'-999999.0'}

    schema['LFA000'] = {}
    schema['LFA000'] = {'name':'LFA000','fcode':'FA000','desc':'Administrative Boundary','geom':'Line','fcsubtype':'FA000_Administrative_Boundary_Line','fdname':'MGCP_Delta','thematic':'PolbndL'}
    schema['LFA000']['columns'] = {}
    schema['LFA000']['columns']['NM3'] = {'name':'NM3','desc':'Boundary First Name','optional':'R','type':'String','defValue':'UNK'}
    schema['LFA000']['columns']['NM4'] = {'name':'NM4','desc':'Boundary Second Name','optional':'R','type':'String','defValue':'UNK'}
    schema['LFA000']['columns']['BST'] = {'name':'BST','desc':'Boundary Status','optional':'R','type':'enumeration','defValue':'0'}
    schema['LFA000']['columns']['BST']['enum'] =[{'name':'Unknown','value':'0'}, {'name':'Definite','value':'1'},
     {'name':'Indefinite','value':'2'}, {'name':'In Dispute','value':'3'}, {'name':'No Defined Boundary','value':'4'},{'name':'Other','value':'999'}
     ]
    schema['LFA000']['columns']['CFT'] = {'name':"CFT",'desc':"Boundary Determination Method",'optional':"R",'type':"enumeration",'defValue':"0"}
    schema['LFA000']['columns']['CFT']['enum'] = [
     {'name':"Unknown",'value':"0"}, {'name':"Elevation contour",'value':"7"}, {'name':"Drainage Limit",'value':"8"},
     {'name':"Ridgeline",'value':"10"}, {'name':"Drainage Line",'value':"11"}, {'name':"Left Bank",'value':"12"},
     {'name':"Right Bank",'value':"13"}, {'name':"Shoreline",'value':"14"}, {'name':"Lake Transec",'value':"15"},
     {'name':"Meridian",'value':"16"}, {'name':"Median",'value':"17"}, {'name':"Parallel",'value':"18"},
     {'name':"Straight Segmented",'value':"19"}, {'name':"Straight",'value':"20"}, {'name':"Anthropogeographic",'value':"33"}
    ]
    schema['LFA000']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':'FA000_Administrative_Boundary_Line'}
    schema['LFA000']['columns']['LSP'] = {'name':"LSP",'desc':"Geopolitical Line Type",'optional':"R",'type':"enumeration",'defValue':"1"}
    schema['LFA000']['columns']['LSP']['enum'] = [
     {'name':"Generic Administrative Boundary",'value':"1"}, {'name':"Provisional Administrative Line",'value':"2"},
     {'name':"Armistice Line",'value':"3"}, {'name':"Line of Control",'value':"4"}, {'name':"Demarcation Line",'value':"5"},
     {'name':"Line of Convenience",'value':"6"}, {'name':"Cease Fire Line",'value':"7"}, {'name':"Convention Line",'value':"8"},
     {'name':"Claim Line",'value':"9"}, {'name':"Intercolonial Line",'value':"10"}, {'name':"Interentity Line",'value':"11"},
     {'name':"Line of Adjacency",'value':"12"}, {'name':"Line of Withdrawal",'value':"13"}, {'name':"Military Disengagement Line",'value':"14"},
     {'name':"Treaty Line",'value':"15"}, {'name':"UNCLOS Claim Boundary",'value':"16"}, {'name':"Generic International Boundary",'value':"17"},
     {'name':"Other",'value':"999"}
    ]
    schema['LFA000']['columns']['USE_'] = {'name':"USE_",'desc':"Usage",'optional':"R",'type':"enumeration",'defValue':"0"}
    schema['LFA000']['columns']['USE_']['enum'] = [
     {'name':"Unknown",'value':"0"}, {'name':"Tribal",'value':"7"}, {'name':"City",'value':"16"},
     {'name':"International",'value':"23"}, {'name':"Primary/1st Order",'value':"26"}, {'name':"Secondary/2nd Order",'value':"30"},
     {'name':"Tertiary/3rd Order",'value':"31"}, {'name':"Insular",'value':"32"}, {'name':"Reserve/Reservation",'value':"70"},
     {'name':"Forest Preserve",'value':"141"}, {'name':"Prohibited Area",'value':"155"}, {'name':"Other",'value':"999"}
    ]

    schema['AFA002'] = {}
    schema['AFA002'] = {'name':'AFA002','fcode':'FA002','desc':'Geopolitical Entity','geom':'Area','fcsubtype':'FA002_Geopolitical_Entity_Area','fdname':'MGCP_Delta','thematic':'PolbndA'}
    schema['AFA002']['columns'] = {}
    schema['AFA002']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':'FA002_Geopolitical_Entity_Area'}
    schema['AFA002']['columns']['NAM'] = {'name':'NAM','desc':'Name','optional':'R','type':'String','defValue':'UNK'}
    schema['AFA002']['columns']['NFI'] = {'name':'NFI','desc':'Named Feature Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['AFA002']['columns']['NFN'] = {'name':'NFN','desc':'Name Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['AFA002']['columns']['COD'] = {'name':'COD','desc':'Delineation Known','optional':'R','type':'enumeration','defValue':'1'}
    schema['AFA002']['columns']['COD']['enum'] = [{'name':'Limits and Information Known','value':'1'}, {'name':'Limits and Information Unknown','value':'2'} ]
    schema['AFA002']['columns']['GEC'] = {'name':"GEC",'desc':"Geopolitical Entity Type",'optional':"R",'type':"enumeration",'defValue':"7"}
    schema['AFA002']['columns']['GEC']['enum'] = [{'name':"Area of No Sovereignty",'value':"5"}, {'name':"Demilitarized Zone (DMZ)",'value':"7"},
     {'name':"Zone of Occupation",'value':"8"}, {'name':"Leased Area",'value':"9"}, {'name':"Political Entity",'value':"10"},
     {'name':"Dependent Political Entity",'value':"11"}, {'name':"Freely Associated State",'value':"12"}, {'name':"Independent Political Entity",'value':"13"},
     {'name':"Semi-independent Political Entity",'value':"14"}, {'name':"Economic Region",'value':"15"}, {'name':"Territory",'value':"16"},
     {'name':"Buffer Zone",'value':"17"}
    ]

    schema['AFA003'] = {}
    schema['AFA003'] = {'name':'AFA003','fcode':'FA003','desc':'Administrative Division','geom':'Area','fcsubtype':'FA003_Administrative_Division_Area','fdname':'MGCP_Delta','thematic':'PolbndA'}
    schema['AFA003']['columns'] = {}
    schema['AFA003']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':'FA003_Administrative_Division_Area'}
    schema['AFA003']['columns']['NAM'] = {'name':'NAM','desc':'Name','optional':'R','type':'String','defValue':'UNK'}
    schema['AFA003']['columns']['NFI'] = {'name':'NFI','desc':'Named Feature Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['AFA003']['columns']['NFN'] = {'name':'NFN','desc':'Name Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['AFA003']['columns']['BAL'] = {'name':'BAL','desc':'BGN Administrative Level','optional':'R','type':'enumeration','defValue':'1'}
    schema['AFA003']['columns']['BAL']['enum'] = [{'name':'First-order','value':'1'}, {'name':'Second-order','value':'2'},
     {'name':'Third-order','value':'3'}, {'name':'Fourth-order','value':'4'},
     {'name':'Undifferentiated','value':'5'}, {'name':'Other','value':'999'}
     ]
    schema['AFA003']['columns']['COD'] = {'name':'COD','desc':'Delineation Known','optional':'R','type':'enumeration','defValue':'1'}
    schema['AFA003']['columns']['COD']['enum'] = [{'name':'Limits and Information Known','value':'1'}, {'name':'Limits and Information Unknown','value':'2'} ]

    schema['LFA110'] = {}
    schema['LFA110'] = {'name':'LFA110','fcode':'FA110','desc':'International Date Line','geom':'Line','fcsubtype':'FA110_International_Date_Line_Line','fdname':'MGCP_Delta','thematic':'PolbndL'}
    schema['LFA110']['columns'] = {}
    schema['LFA110']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':'FA110_International_Date_Line_Line'}

    schema['LFC021'] = {}
    schema['LFC021'] = {'name':'LFC021','fcode':'FC021','desc':'Maritime Limit Boundary','geom':'Line','fcsubtype':'FC021_Maritime_Limit_Boundary_Line','fdname':'MGCP_Delta','thematic':'PolbndL'}
    schema['LFC021']['columns'] = {}
    schema['LFC021']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':'FC021_Maritime_Limit_Boundary_Line'}
    schema['LFC021']['columns']['NM3'] = {'name':'NM3','desc':'Boundary First Name','optional':'R','type':'String','defValue':'UNK'}
    schema['LFC021']['columns']['NM4'] = {'name':'NM4','desc':'Boundary Second Name','optional':'R','type':'String','defValue':'UNK'}

    schema['PZB050'] = {}
    schema['PZB050'] = {'name':'PZB050','fcode':'ZB050','desc':'Survey','geom':'Point','fcsubtype':'ZB050_Survey_Point','fdname':'MGCP_Delta','thematic':'ElevP'}
    schema['PZB050']['columns'] = {}
    schema['PZB050']['columns']['BEL'] = {'name':'BEL','desc':'Base Elevation','optional':'R','type':'Real','defValue':'-999999.0'}
    schema['PZB050']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':'ZB050_Survey_Point'}
    schema['PZB050']['columns']['NAM'] = {'name':'NAM','desc':'Name','optional':'R','type':'String','defValue':'UNK'}
    schema['PZB050']['columns']['NFI'] = {'name':'NFI','desc':'Named Feature Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['PZB050']['columns']['NFN'] = {'name':'NFN','desc':'Name Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['PZB050']['columns']['SUY'] = {'name':'SUY','desc':'Survey Point Type','optional':'R','type':'enumeration','defValue':'0'}
    schema['PZB050']['columns']['SUY']['enum'] = [{'name':'Unknown','value':'0'}, {'name':'Astronomic Position','value':'1'},
     {'name':'Benchmark','value':'2'}, {'name':'Cadastral Control Point','value':'3'}, {'name':'Camera Station','value':'4'},
     {'name':'Geodetic Point','value':'5'}, {'name':'Gravity Point','value':'6'}, {'name':'Magnetic Station','value':'7'},
     {'name':'Theodolite Station','value':'8'}, {'name':'Tidal Benchmark','value':'9'}, {'name':'Other','value':'999'}
     ]

    schema['PZD045'] = {}
    schema['PZD045'] = {'name':'PZD045','fcode':'ZD045','desc':'Annotated Location','geom':'Point','fcsubtype':'ZD045_Annotated_Location_Point','fdname':'MGCP_Delta','thematic':'AnnoP'}
    schema['PZD045']['columns'] = {}
    schema['PZD045']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':'ZD045_Annotated_Location_Point'}
    schema['PZD045']['columns']['NAM'] = {'name':'NAM','desc':'Name','optional':'R','type':'String','defValue':'UNK'}
    schema['PZD045']['columns']['NFI'] = {'name':'NFI','desc':'Named Feature Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['PZD045']['columns']['NFN'] = {'name':'NFN','desc':'Name Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['PZD045']['columns']['ORD'] = {'name':'ORD','desc':'Relative Importance','optional':'R','type':'enumeration','defValue':'1'}
    schema['PZD045']['columns']['ORD']['enum'] = [{'name':'First','value':'1'}, {'name':'Second','value':'2'},
     {'name':'Third','value':'3'}, {'name':'Fourth','value':'4'}, {'name':'Fifth','value':'5'}
     ]
    schema['PZD045']['columns']['THC'] = {'name':'THC','desc':'Thematic Classification','optional':'R','type':'enumeration','defValue':'1'}
    schema['PZD045']['columns']['THC']['enum'] = [{'name':'Aeronautical','value':'1'}, {'name':'Vegetation','value':'2'},
     {'name':'Utilities','value':'3'}, {'name':'Population','value':'4'}, {'name':'Physiography','value':'5'},
     {'name':'Industry','value':'6'}, {'name':'Ground Transportation','value':'7'}, {'name':'Elevation','value':'8'},
     {'name':'Boundaries','value':'9'}, {'name':'Waterbodies','value':'10'}, {'name':'Maritime','value':'11'}
     ]

    schema['PZD040'] = {}
    schema['PZD040'] = {'name':'PZD040','fcode':'ZD040','desc':'Text Point','geom':'Point','fcsubtype':'','fdname':'MGCP_Delta','thematic':'TextP'}
    schema['PZD040']['columns'] = {}
    schema['PZD040']['columns']['FCSubtype'] = {'name':'FCSubtype','desc':'Feature Class Subtype','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','type':'String','defValue':''}
    schema['PZD040']['columns']['NAM'] = {'name':'NAM','desc':'Name','optional':'R','type':'String','defValue':'UNK'}
    schema['PZD040']['columns']['NFI'] = {'name':'NFI','desc':'Named Feature Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['PZD040']['columns']['NFN'] = {'name':'NFN','desc':'Name Identifier','optional':'R','type':'String','defValue':'N_A'}
    schema['PZD040']['columns']['ORD'] = {'name':'ORD','desc':'Relative Importance','optional':'R','type':'enumeration','defValue':'1'}
    schema['PZD040']['columns']['ORD']['enum'] = [{'name':'First','value':'1'}, {'name':'Second','value':'2'},
     {'name':'Third','value':'3'}, {'name':'Fourth','value':'4'}, {'name':'Fifth','value':'5'}
     ]
    schema['PZD040']['columns']['THC'] = {'name':'THC','desc':'Thematic Classification','optional':'R','type':'enumeration','defValue':'1'}
    schema['PZD040']['columns']['THC']['enum'] = [{'name':'Aeronautical','value':'1'}, {'name':'Vegetation','value':'2'},
     {'name':'Utilities','value':'3'}, {'name':'Population','value':'4'}, {'name':'Physiography','value':'5'},
     {'name':'Industry','value':'6'}, {'name':'Ground Transportation','value':'7'}, {'name':'Elevation','value':'8'},
     {'name':'Boundaries','value':'9'}, {'name':'Waterbodies','value':'10'}, {'name':'Maritime','value':'11'}
     ]

    # Add the missing attributes of features that had the default values being overwritten to null 
    schema['PAL130']['columns']['AOO'] = {'name':"AOO",'desc':"Angle of Orientation",'optional':"R",'units':"Degree",'definition':"The angular distance in the horizontal plane measured from true north (0 degrees) clockwise to the major axis of the feature. ( If the feature is square, the axis 0 up to 90 degrees is recorded. If the feature is circular, 360 degrees is recorded. )",'type':"Real",'defValue':"-32767.0"}



    # Add the common attributes to each feature
    for i in ['LCA010','PCA030','PCA035','LFA000','LFA110','LFC021','PZB050','PZD040','PZD045','AFA002','AFA003']:
        schema[i]['columns']['FCODE'] = {'name':'FCODE','desc':'Feature Code','type':'String','optional':'R','defValue':''}
        schema[i]['columns']['ACC'] = {'name':'ACC','desc':'Horizontal Accuracy Category','type':'enumeration','optional':'R','defValue':'1','func':'full_ACC'}
        schema[i]['columns']['ACC']['enum'] = [{'name':"Accurate",'value':"1"}, {'name':"Approximate",'value':"2"}]
        schema[i]['columns']['GFID'] = {'name':'GFID','desc':'Global Feature Identifier','optional':'R','definition':'A global feature identifier placeholder to avoid non-nullable field.','type':'String','defValue':'UNK'}
        schema[i]['columns']['GlobalID'] = {'name':'GlobalID','desc':'GlobalID','optional':'R','definition':'A global ID placeholder to avoid non-nullable field.','type':'String','defValue':'UNK'}
        schema[i]['columns']['CCN'] = {'name':'CCN','desc':'Commercial Copyright','type':'String','optional':'R','defValue':'No copyright or restriction of rights of use is asserted by originator of this information.'}
        schema[i]['columns']['SDP'] = {'name':'SDP','desc':'Source Description','type':'String','optional':'R','defValue':'N_A'}
        schema[i]['columns']['SDV'] = {'name':'SDV','desc':'Source Date and Time','type':'String','optional':'R','defValue':'N_A'}
        schema[i]['columns']['SRT'] = {'name':'SRT','desc':'Source Type','type':'enumeration','optional':'R','defValue':'0','enum':[],'func':'full_SRT'}
        schema[i]['columns']['SRT']['enum'] = [
         {'name':'Unknown','value':'0'}, {'name':'ADRG','value':'1'}, {'name':'AAFIF','value':'2'},
         {'name':'CHUM','value':'3'}, {'name':'City Graphic','value':'4'}, {'name':'Combat Chart','value':'5'},
         {'name':'CADRG','value':'6'}, {'name':'CIB1','value':'7'}, {'name':'CIB5','value':'8'},
         {'name':'DNC','value':'10'}, {'name':'DPF','value':'11'}, {'name':'DTOP 1','value':'16'},
         {'name':'DTOP 2','value':'17'}, {'name':'DTOP 3','value':'18'}, {'name':'DTOP 4','value':'19'},
         {'name':'DTOP 5','value':'20'}, {'name':'DVOF','value':'21'}, {'name':'FFD','value':'22'},
         {'name':'Land Cover','value':'24'}, {'name':'GeoNames','value':'25'}, {'name':'GPS','value':'26'},
         {'name':'ICM','value':'27'}, {'name':'NTM Imagery','value':'29'}, {'name':'Imagery','value':'30'},
         {'name':'ITD','value':'31'}, {'name':'IVD','value':'32'}, {'name':'International Boundaries','value':'33'},
         {'name':'JOG','value':'34'}, {'name':'LWD','value':'36'}, {'name':'MC&G','value':'37'},
         {'name':'MSD 1','value':'38'}, {'name':'MSD 2','value':'39'}, {'name':'MSD 3','value':'40'},
         {'name':'MSD 4','value':'41'}, {'name':'MSD 5','value':'42'}, {'name':'MIDB','value':'43'},
         {'name':'Native Data','value':'44'}, {'name':'Native Map','value':'45'}, {'name':'Medical Facilities','value':'46'},
         {'name':'NATE','value':'47'}, {'name':'Planning Graphic','value':'48'}, {'name':'SRTM','value':'50'},
         {'name':'Site Map','value':'51'}, {'name':'TOD 0','value':'52'}, {'name':'TOD 1','value':'53'},
         {'name':'TOD 2','value':'54'}, {'name':'TOD 3','value':'55'}, {'name':'TOD 4','value':'56'},
         {'name':'TOD 5','value':'57'}, {'name':'UN Data','value':'59'}, {'name':'UVMap','value':'60'},
         {'name':'VITD','value':'61'}, {'name':'VMap 0','value':'62'}, {'name':'VMap 1','value':'63'},
         {'name':'VMap 2','value':'64'}, {'name':'WVS Plus','value':'65'}, {'name':'SPOT HRG Digital Terrain Model','value':'85'},
         {'name':'Vector Data','value':'92'}, {'name':'1:25k Vector Data','value':'93'}, {'name':'1:50k Vector Data','value':'94'},
         {'name':'1:100k Vector Data','value':'95'}, {'name':'Very High Resolution Commercial Monoscopic Imagery','value':'110'}, {'name':'Very High Resolution Commercial Stereoscopic Imagery','value':'111'},
         {'name':'High Resolution Commercial Monoscopic Imagery','value':'112'}, {'name':'High Resolution Commercial Stereoscopic Imagery','value':'113'}, {'name':'Medium Resolution Commercial Monoscopic Imagery','value':'114'},
         {'name':'Medium Resolution Commercial Stereoscopic Imagery','value':'115'}, {'name':'Low Resolution Commercial Monoscopic Imagery','value':'116'}, {'name':'Low Resolution Commercial Stereoscopic Imagery','value':'117'},
         {'name':'Map 1:25k','value':'118'}, {'name':'Map 1:50k','value':'119'}, {'name':'Map 1:100k','value':'120'},
         {'name':'Routing Data','value':'121'}, {'name':'Multiple','value':'996'}, {'name':'Unpopulated','value':'997'},
         {'name':'Not Applicable','value':'998'}, {'name':'Other','value':'999'}
        ]

        schema[i]['columns']['TXT'] = {'name':'TXT','desc':'Associated Text','type':'String','optional':'R','defValue':'N_A'}
        schema[i]['columns']['UID'] = {'name':'UID','desc':'MGCP Feature universally unique identifier','type':'String','optional':'R','length':'36','defValue':'UNK'}

    return schema
# End of addExtra


# Read all of the features in an XML document
def readFeatures(xmlDoc,funcList):
    itemList = xmlDoc.getElementsByTagName('gfc:FC_FeatureType')


    # Debug:
    #print('Features: Items: ', len(itemList))

    # Setup handy lists
    geoList = {'L':'Line', 'A':'Area', 'P':'Point','_':'None' }
    typeList = {'CodeList':'enumeration','CharacterString':'String','Real':'Real','Integer':'Integer',
                'GM_Surface':'none', 'GM_Curve':'none','GM_Point':'none' }

    # geoList = {'C':'Line','Crv':'Line','S':'Area','Srf':'Area','P':'Point','Pnt':'Point','_':'None'}
    # typList = {'esriFieldTypeDouble':'Real','xs:double':'Real','esriFieldTypeString':'String','xs:string':'String',
              # 'esriFieldTypeInteger':'Integer','xs:int':'Integer','esriFieldTypeGlobalID':'String'}


    thematicLookup = {'AGB005':'AerofacA','AGB035':'AerofacA','AGB065':'AerofacA','AGB230':'AerofacA','PGB030':'AerofacP',
        'PGB065':'AerofacP','PGB230':'AerofacP','AAM020':'AgristrA','AAM030':'AgristrA','PAM020':'AgristrP','PAM030':'AgristrP',
        'ABH010':'AquedctA','LBH010':'AquedctL','PBH010':'AquedctP','LAL070':'BarrierL','LAL260':'BarrierL','LDB010':'BluffL',
        'AAQ040':'BridgeA','LAQ040':'BridgeL','AAL015':'BuildA','PAL015':'BuildP','AAL020':'BuiltupA','AAL208':'BuiltupA',
        'PAL020':'BuiltupP','PBI010':'CisternP','ABA030':'CoastA','ABA040':'CoastA','ABA050':'CoastA','LBA010':'CoastL',
        'PBA050':'CoastP','AAT050':'CommA','PAT010':'CommP','PAT045':'CommP','ABH135':'CropA','AEA010':'CropA','AEA040':'CropA',
        'AEA050':'CropA','AEA055':'CropA','ABI020':'DamA','ABH165':'DamA','LBI020':'DamL','LBH165':'DamL','PBI020':'DamP',
        'ABD100':'DangerA','ABD120':'DangerA','LBD120':'DangerL','PBD100':'DangerP','PBD110':'DangerP','PBD130':'DangerP',
        'PBD180':'DangerP','AAB000':'DisposeA','AAB010':'DisposeA','ADB090':'EmbankA','LAQ063':'EmbankL','LDB070':'EmbankL',
        'LDB090':'EmbankL','LDB071':'EmbankL','AAA010':'ExtractA','AAA012':'ExtractA','ABH155':'ExtractA','ABH150':'ExtractA',
        'PAA010':'ExtractP','PAA012':'ExtractP','LAQ070':'FerryL','PAQ070':'FerryP','AEC040':'FirebrkA','AEC060':'FirebrkA',
        'LBH070':'FordL','PBH070':'FordP','AAH050':'FortA','PAH050':'FortP','AEB010':'GrassA','AEB020':'GrassA','AEC010':'GrassA',
        'ADA010':'GroundA','ABB005':'HarborA','ABB090':'HarborA','PBB155':'HarborP','LAF020':'IndL','LBH060':'IndL',
        'LFA090':'IndL','ABH090':'InundA','ABH080':'LakeresA','ABH130':'LakeresA','ABH160':'Landfrm1A','ADB170':'Landfrm1A',
        'ABJ020':'Landfrm2A','ADB160':'Landfrm2A','ADB200':'LandfrmA','ABJ031':'LandfrmA','ADB061':'LandfrmA','LBJ040':'LandfrmL',
        'LDB110':'LandfrmL','LDB200':'LandfrmL','LBJ031':'LandfrmL','LDB061':'LandfrmL','LDB100':'LandfrmL','LDB160':'LandfrmL',
        'PBJ060':'LandfrmP','PDB160':'LandfrmP','ABJ030':'LandIceA','ABJ100':'LandIceA','AAJ010':'LandmrkA','AAK030':'LandmrkA',
        'AAK060':'LandmrkA','AAK120':'LandmrkA','AAK160':'LandmrkA','AAK180':'LandmrkA','AAL030':'LandmrkA','AAK040':'LandmrkA',
        'AFA015':'LandmrkA','AAI030':'LandmrkA','AAK170':'LandmrkA','LAK130':'LandmrkL','LAQ075':'LandmrkL','LAK150':'LandmrkL',
        'PAH070':'LandmrkP','PAK160':'LandmrkP','PAL030':'LandmrkP','PAL130':'LandmrkP','PAK040':'LandmrkP','PAK150':'LandmrkP',
        'PAK170':'LandmrkP','ABI030':'LockA','LBI030':'LockL','LBI040':'LockL','PBI030':'LockP','PBI040':'LockP',
        'PAL025':'MarkersP','ASU001':'MilA','LAH025':'MilL','PSU001':'MilP','PAQ110':'MiscaeroP','PGA034':'MiscaeroP',
        'PGB485':'MiscaeroP','PGB040':'MiscaeroP','PGB220':'MiscaeroP','LBH110':'MiscL','LBI041':'MiscL','PBI041':'MiscP',
        'PBI050':'MiscP','AAL012':'MiscpopA','AAL105':'MiscpopA','AAL010':'MiscpopA','AAL019':'MiscpopA','AAJ110':'MiscpopA',
        'PAL012':'MiscpopP','PAL105':'MiscpopP','PAL099':'MiscpopP','PAJ110':'MiscpopP','PAL019':'MiscpopP','PDB029':'MtnP',
        'PDB150':'MtnP','AAL140':'NuclearA','AEC020':'OasisA','PAF010':'ObstrP','PAF040':'ObstrP','PAF070':'ObstrP',
        'PAJ050':'ObstrP','ADB180':'PhysA','AAK190':'PierA','ABB190':'PierA','LAK190':'PierL','LBB190':'PierL','LAQ113':'PipeL',
        'AAL170':'PlazaA','AAD010':'PowerA','AAD050':'PowerA','LAT030':'PowerL','LAT041':'PowerL','PAD010':'PowerP',
        'PAD020':'PowerP','PAD050':'PowerP','AAC000':'ProcessA','PAC000':'ProcessP','AAA052':'PumpingA','AAQ116':'PumpingA',
        'PAQ116':'PumpingP','LAN010':'RailrdL','LAN050':'RailrdL','ABB240':'RampA','ABH120':'RapidsA','LBH120':'RapidsL',
        'LBH180':'RapidsL','PBH120':'RapidsP','PBH145':'RapidsP','PBH180':'RapidsP','PAA040':'RigwellP','PAC020':'RigwellP',
        'LAP030':'RoadL','PAN075':'RrturnP','AAN060':'RryardA','AAL200':'RuinsA','AGB015':'RunwayA','AGB045':'RunwayA',
        'AGB055':'RunwayA','AGB075':'RunwayA','LGB050':'RunwayL','PGB050':'RunwayP','ABB041':'SeastrtA','ABB043':'SeastrtA',
        'ABB140':'SeastrtA','LBB041':'SeastrtL','LBB043':'SeastrtL','LBB140':'SeastrtL','LBB230':'SeastrtL','LAL210':'ShedL',
        'PAL210':'ShedP','AAK090':'SportA','AAK100':'SportA','AAM010':'StorageA','AAM040':'StorageA','AAM060':'StorageA',
        'AAM070':'StorageA','PAM040':'StorageP','PAM060':'StorageP','PAM070':'StorageP','AAD030':'SubstatA','PAD030':'SubstatP',
        'ABH015':'SwampA','AED010':'SwampA','AED020':'SwampA','AED030':'SwampA','LAT060':'TeleL','AFA100':'TestA',
        'PZD040':'TextP','ADB115':'ThermalA','PDB115':'ThermalP','PDB180':'ThermalP','PAL241':'TowerP','LAP010':'TrackL',
        'LAP050':'TrailL','AAQ125':'TransA','AAQ135':'TransA','AAQ140':'TransA','AAL060':'TransA','AAN076':'TransA',
        'LAL060':'TransL','PAQ090':'TransP','PAQ125':'TransP','PAQ065':'TransP','PAN076':'TransP','AAC030':'TreatA',
        'AAJ030':'TreatA','ABH040':'TreatA','ABH050':'TreatA','ABH051':'TreatA','PAC030':'TreatP','PAJ030':'TreatP',
        'AEC030':'TreesA','LEA020':'TreesL','LEC030':'TreesL','PEC030':'TreesP','ABJ110':'TundraA','AAQ130':'TunnelA',
        'LAQ130':'TunnelL','PAJ051':'UtilP','PAT042':'UtilP','AZD020':'VoidA','ABH020':'WatrcrsA','ABH030':'WatrcrsA',
        'ABH140':'WatrcrsA','LBH020':'WatrcrsL','LBH030':'WatrcrsL','LBH140':'WatrcrsL','PAA050':'WellsprP',
        'PBH170':'WellsprP','LZD040':'AnnoL','LZD045':'AnnoL','PZD045':'AnnoP','LCA010':'ContourL',
        'PCA030':'ElevP','PCA035':'ElevP','PZB050':'ElevP','AFA002':'PolbndA','AFA003':'PolbndA','LFA000':'PolbndL',
        'LFA110':'PolbndL','LFC021':'PolbndL'}

    nonMGCPfd = {'LZD040','LZD045','PZD045','PZD040','LCA010','PCA030','PCA035','PZB050','AFA002','AFA003','LFA000','LFA110','LFC021'}

    # These attributes have non-standard defaults
    customDefVal = {'ACC':'1','TXT':'N_A','SDP':'N_A','CON':'998','FUN':'6','COD':'1000','NFI':'N_A','NFN':'N_A'}

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
        # print 'Geometry: ', fGeometry
        # print 'FCODE: ', fCode

        # Build a feature
        if rawfCode not in tSchema:
            tSchema[rawfCode] = {}
            tSchema[rawfCode]['name'] = rawfCode
            tSchema[rawfCode]['fcode'] = fCode
            tSchema[rawfCode]['geom'] = fGeometry
            tSchema[rawfCode]['columns'] = {}
            tSchema[rawfCode]['columns']['FCODE'] = { 'name':'FCODE','desc':'Feature Code','type':'String','optional':'R','defValue':'','length':'5'}
            tSchema[rawfCode]['columns']['GlobalID'] = { 'name':'GlobalID','desc':'GlobalID','type':'String','optional':'R','definition':'A global ID placeholder to avoid non-nullable field.','defValue':'UNK'}
            tSchema[rawfCode]['columns']['GFID'] = { 'name':'GFID','desc':'Global Feature Identifier','type':'String','optional':'R','definition':'A global feature identifier placeholder to avoid non-nullable field.','defValue':'UNK'}
            

            if rawfCode in thematicLookup:
                tSchema[rawfCode]['thematic'] = thematicLookup[rawfCode]
            # else:
            #     print '### Missing',rawfCode,'from thematic lookup'

            if rawfCode in nonMGCPfd:
                tSchema[rawfCode]['fdname'] = 'MGCP_Delta'
            else:
                tSchema[rawfCode]['fdname'] = 'MGCP'

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
            if node.localName == 'typeName' and processSingleNode(node,'gco:LocalName') == 'MGCP Feature':
                tSchema[rawfCode]['desc'] = processSingleNode(node,'gco:LocalName').replace(' Area Feature','').replace(' Point Feature','').replace(' Line Feature','')
                continue
            elif node.localName == 'typeName':
                #print 'Feature Type: ', processSingleNode(node,'gco:LocalName')
                tSchema[rawfCode]['desc'] = processSingleNode(node,'gco:LocalName').replace(' Area Feature','').replace(' Point Feature','').replace(' Line Feature','')

                # Build the subtype now that the description is populated
                tSchema[rawfCode]['fcsubtype'] = tSchema[rawfCode]['fcode'] + '_' + tSchema[rawfCode]['desc'].replace(' ','_') + '_' + tSchema[rawfCode]['geom']
                tSchema[rawfCode]['columns']['FCSubtype'] = { 'name':'FCSubtype','desc':'Feature Class Subtype','type':'String','optional':'R','definition':'A feature class subtype placeholder to avoid non-nullable field.','defValue':tSchema[rawfCode]['fcsubtype']}
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
                    if aName == 'FCSubtype':
                        tSchema[rawfCode]['columns'][aName]['defValue'] = tSchema[rawfCode]['fcsubtype']

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


# Convert a schema to Thematic schema
def makeThematic(schema):
    tSchema = {}
    for feature in schema:
        # Skip the parent feature
        if feature == '_GMGCP':
            continue

        thematicName = schema[feature]['thematic']

        # Build a feature
        if thematicName not in tSchema:
            tSchema[thematicName] = {}
            tSchema[thematicName]['name'] = thematicName
            tSchema[thematicName]['fcode'] = ''
            tSchema[thematicName]['geom'] = schema[feature]['geom']
            tSchema[thematicName]['desc'] = thematicName
            tSchema[thematicName]['fdname'] = schema[feature]['fdname']
            tSchema[thematicName]['columns'] = {}
            # tSchema[thematicName]['columns']['FCODE'] = { 'name':'FCODE','desc':"Feature Code",'type':'String','optional':'R','defValue':'','length':'5'}

        for attr in schema[feature]['columns']:
            if attr not in tSchema[thematicName]['columns']:
                tSchema[thematicName]['columns'][attr] = {}
                tSchema[thematicName]['columns'][attr] = schema[feature]['columns'][attr]

    return tSchema
# End of makeThematic


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
    parser.add_argument('--fromenglish', help='Dump out From English translation rules',action='store_true')
    parser.add_argument('--fcodeschema', help='Dump out a list of fcodes in the internal OSM schema format',action='store_true')
    parser.add_argument('--nocodedvalues', help='Generate a Thematic Schema with no coded values',action='store_true')
    parser.add_argument('--numrules', help='Dump out number rules',action='store_true')
    parser.add_argument('--rules', help='Dump out one2one rules',action='store_true')
    parser.add_argument('--thematic', help='Generate a Thematic Schame',action='store_true')
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
    thematicSchema = {}
    thematicEnumSchema = {}

    # The list of attributes to make into functions in the schema. Used for enumerated lists that get
    # repeated a lot.
    funcList = ['ACC','FUN','SRT']

    schema = readFeatures(xmlDoc,funcList)

    # There are attributes that get "inherited" by features.
    inheritance = {}
    inheritance = readInheritance(xmlDoc)

    schema = processInheritance(schema,inheritance)

    # Now add in "extra" features that are not specified in the TRD
    schema = addExtra(schema)

    # Now build a thematic schema
    thematicSchema = makeThematic(schema)
    thematicEnumSchema = makeThematic(schema)

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
        printToEnglish(schema,'MGCP TRD4','emgcp','FCODE')
    elif args.fromenglish:
        printCopyright()
        printFromEnglish(schema,'MGCP TRD4','emgcp_osm_rules')
    elif args.attributecsv:
        printAttributeCsv(schema)
    elif args.dumpenum:
        dumpEnumerations(schema,'enumMGCP')
    elif args.fieldvalues:
        printFieldValues(schema)
    else:
        printCopyright()
        if args.nocodedvalues and args.thematic:
            printThematicEnumHeader('mgcp')
            convertTextEnumerations(thematicEnumSchema)
            printFuncList(thematicEnumSchema)
            printThematicEnum(thematicEnumSchema,'mgcp')
            printThematicEnumFooter('mgcp')
        elif args.thematic:
            printThematic(thematicSchema,'mgcp')
        else:
            printJSHeader('mgcp')
            convertTextEnumerations(schema)
            printFuncList(schema)
            printJavascript(schema)
            printJSFooter('mgcp')
# End
