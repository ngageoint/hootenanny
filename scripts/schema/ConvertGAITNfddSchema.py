#!/usr/bin/python

# ConvertNfddSchema.py
#
# Convert the GAIT NFDD.csv and NFDD_domains.csv to a Javascript data structure or just dump the schema
# as text.
# 
#i Note: This script does not do any error or sanity checking of the input files.
#
# Usage: convert_NFDD.py  <-t> NFDD.csv NFDD_domains.csv
#
# -t option dumps the schema as plain text.
#
# Mattj Oct 13
#

import sys,os,csv,argparse,gzip

# printText: Dump out the structure as plain text
def printText(schema):
    for f in schema:
        print '\n%s: %s  Geometry: %s ' % (schema[f]['fcode'],schema[f]['name'],schema[f]['geom'])
        for k in schema[f]['attrib']:
            if 'enum' not in schema[f]['attrib'][k]['domain']:
                print'\t%s: %s  Optional: %s  Default: %s  Type: %s  Min: %s  Max: %s  Length: %s' % (k,\
                    schema[f]['attrib'][k]['desc'], \
                    schema[f]['attrib'][k]['optional'], \
                    schema[f]['attrib'][k]['defval'], \
                    schema[f]['attrib'][k]['domain']['dtype'], \
                    schema[f]['attrib'][k]['domain']['minv'], \
                    schema[f]['attrib'][k]['domain']['maxv'], \
                    schema[f]['attrib'][k]['domain']['length'])
            else:
                print'\t%s: %s  Optional: %s  Default: %s Type: %s' % (k, \
                    schema[f]['attrib'][k]['desc'], \
                    schema[f]['attrib'][k]['optional'], \
                    schema[f]['attrib'][k]['defval'], \
                    schema[f]['attrib'][k]['domain']['dtype'])
                for l in schema[f]['attrib'][k]['domain']['enum']:
                    print'\t\t%s = %s' % (l[0],l[1])
# End printText

# printJavascript: Dump out the structure as Javascript
#
# Note: This uses double quotes ( " ) around data elements in the output.  The GAIT NFDD.csv and GAIT_domains.csv
#       files have values with single quotes ( ' ) in them.  These quotes are also in the DFDD and NFDD specs.
#
def printJavascript(schema):
    print notice
    print ''
    #print '__setupPackage__(__extension__);'
    print 'tds.schema = {'
    print '// Build a schema'
    print 'getDbSchema: function()'
    print '{'
    print '    schema = [' # And so it begins...

    num_feat = len(schema.keys()); # How many features in the schema?
    for f in schema:
        print '        { name:"%s",' % (f); # name = geom + FCODE
        print '          fcode:"%s",' % (schema[f]['fcode']);
        print '          desc:"%s",' % (schema[f]['name']);
        print '          geom:"%s",' % (schema[f]['geom']);
        print '          columns:[ ';

        num_attrib = len(schema[f]['attrib'].keys()); # How many attributes does the feature have?
        for k in schema[f]['attrib']:
            if 'enum' not in schema[f]['attrib'][k]['domain']:
                print '                     { name:"%s",' % (k);
                print '                       desc:"%s" ,' % (schema[f]['attrib'][k]['desc']);
                print '                       optional:"%s" ,' % (schema[f]['attrib'][k]['optional']);
                print '                       defValue:"%s" ,' % (schema[f]['attrib'][k]['defval']);
                print '                       type:"%s",' % (schema[f]['attrib'][k]['domain']['dtype']);
                print '                       minimum:"%s",' % (schema[f]['attrib'][k]['domain']['minv']);
                print '                       maximum:"%s",' % (schema[f]['attrib'][k]['domain']['maxv']);

                # Note: if not printing "Length", remove the trailing comma from "maximum"
                print '                       length:"%s" ' % (schema[f]['attrib'][k]['domain']['length']);
            else:
                print '                     { name:"%s",' % (k);
                print '                       desc:"%s" ,' % (schema[f]['attrib'][k]['desc']);
                print '                       optional:"%s" ,' % (schema[f]['attrib'][k]['optional']);
                print '                       defValue:"%s" ,' % (schema[f]['attrib'][k]['defval']);
                print '                       type:"enumeration",';
                print '                       enumerations:[';
                for l in schema[f]['attrib'][k]['domain']['enum']:
                    print '                           { name:"%s", value:"%s" }, ' % (l[1],l[0]);
                print '                        ] // End of Enumerations ';

            if num_attrib == 1:  # Are we at the last attribute? yes = no trailing comma
                print '                     } // End of %s' % (k)
            else:
                print '                     }, // End of %s' % (k)
                num_attrib -= 1;

        print '                    ] // End of Columns'; 

        if num_feat == 1: # Are we at the last feature? yes = no trailing comma
            print '          } // End of feature %s\n' % (schema[f]['fcode']);
        else:
            print '          }, // End of feature %s\n' % (schema[f]['fcode']); 
            num_feat -= 1;

    print '    ]; // End of schema\n'; # End of schema
    print '    return schema; \n'
    print '} // End of GetDBSchema\n'
    print '} // End of tds.schema\n'

# End printJavascript

def openFile(path, mode):
    if path.endswith(".gz"):
        return gzip.GzipFile(path, mode)
    else:
        return open(path, mode)

######
# Main Starts Here
# 
parser = argparse.ArgumentParser(description='Process GAIT NFDD files and build a schema')
parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
parser.add_argument('-t','--text', help='Dump the schema as text. Default is Javascript',action='store_true')
parser.add_argument('mainfile', help='The main NFDD csv file', action='store')
parser.add_argument('domainfile', help='The NFDD domain csv file', action='store')

args = parser.parse_args()

main_csv_file = args.mainfile
domain_csv_file = args.domainfile
# print 'Main: %s  Domain: %s\n' % (main_csv_file, domain_csv_file)

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


# Data structures
features = {}
domain = {}
geom_type = {'N/A':'NA', 'L':'Line', 'A':'Area', 'P':'Point' }

# feature_list is a list of FCODE's to dump out. Empty list = dump all features
# The next step is to make this a command line arg...
#feature_list = ['DB150' \
#                'AL200', \
#                'AN010','AN050','AN060', \
#                'AP010','AP020','AP030', \
#                'AQ040','AQ113','AQ130' \
#                'AT050', \
#                'BH010','BH020','BH030','BH070','BH140', \
#                'ZD040'
#                ]; # End feature_list

feature_list = []; # The default is to dump all


# Start with the domain csv file
with openFile(domain_csv_file, 'rb') as csvfile:

    reader = csv.reader(csvfile, delimiter=',')
    # Read the header
    header = reader.next()
    # print"Domain header: %s\n" % (header)

    # For the Domain file, we are expecting:
    # 0 = Domain
    # 1 = Type
    # 2 = Value
    # 3 = label

    for row in reader:
        if row[0] in domain: # Have we seen this domain?
            if [row[2],row[3]] in domain[row[0]]['enum']:
                if args.quiet == False:
                    sys.stderr.write('Duplicate enumerated value: ' + row[0] + ', ' + row[2] + ' = ' + row[3] + '\n' )
            else:
                domain[row[0]]['enum'].append([row[2],row[3]])

        else: # No, new domain
            domain[row[0]] = {}
            domain[row[0]].update({'domain':row[0], 'dtype':row[1], 'enum':[]})
            domain[row[0]]['enum'].append([row[2],row[3]])

# Now do the the main csv file
with openFile(main_csv_file, 'rb') as csvfile:

    reader = csv.reader(csvfile, delimiter=',')
    # Read the header
    header = reader.next()
    # print"Main header: %s\n" % (header)

    # For the main file, we are expecting:
    # 0 = FCODE Name
    # 1 = FCODE
    # 2 = Geometry
    # 3 = Attribute Code
    # 4 = Attribute Name
    # 5 = Optional 0/1
    # 6 = Min Value
    # 7 = Max Value
    # 8 = Domain <- Enumerations
    # 9 = Data Type
    # 10 = Length

    for row in reader:
        if feature_list and row[1] not in feature_list: # Skip everything that we don't want.
            continue;

        if row[2] == 'N/A':  # Dump anything that doesn't have geometry
            # tname = 'N' + row[1] # No geometry, makes NAP030
            continue 
        else:
            tname = row[2] + row[1] # Have geometry, makes [LAP]AP030

        # Now set a default value based on the attribute type
        if row[9] == 'String':
            if row[7] > 5:
                defaultVal = 'Unk'
            else:        
                defaultVal = ''
        elif row[9] == 'Real':
            # defaultVal = '0.0'
            defaultVal = '-999999' # -999999.0 ???
        elif row[9] == 'Integer': # Use minimum value: 0 or 1
            defaultVal = row[6]

        if tname in features: # Have we seen this feature before?
            if row[8] == 'N/A': # No domain
                features[tname]['attrib'].update({row[3]:{'desc':row[4], 'defval':defaultVal, 'optional':row[5], 'domain':{'dtype':row[9], 'minv':row[6], 'maxv':row[7], 'length':row[10]}}})
            else: # We have a domain
                features[tname]['attrib'].update({row[3]:{'desc':row[4], 'defval':'-999999', 'optional':row[5], 'domain':domain[row[8]]}})

        else: # No, its a new feature
            features[tname] = {}
            features[tname].update({'fcode':row[1], 'name':row[0], 'geom':geom_type[row[2]], 'attrib':{} })
            if row[8] == 'N/A': # No domain
                features[tname]['attrib'].update({row[3]:{'desc':row[4], 'defval':defaultVal, 'optional':row[5], 'domain':{'dtype':row[9], 'minv':row[6], 'maxv':row[7], 'length':row[10]}}})
            else: # We have a domain
                # Looking at the domains, -999999 -> No Information is a good default
                features[tname]['attrib'].update({row[3]:{'desc':row[4], 'defval':'-999999', 'optional':row[5], 'domain':domain[row[8]]}})

# Now dump the schema out
if args.text:
    printText(features)
else:
    printJavascript(features)

# End
