#!/usr/bin/python

# Usage: ConvertMGCPSchema.py <text file>
#
# The text file is the PDF MGCP spec from GAIT converted to text.
#
# Mattj Mar 14
#

import sys,os,csv,argparse,gzip,re

def dumpMeta():
    print '                     { name:"ACC",'
    print '                       desc:"Horizontal Accuracy Category",'
    print '                       type:"enumeration",'
    print '                       optional:"R",'
    print '                       defValue:"2",'
    print '                       enumerations:['
    print '                           { name:"Accurate", value:"1" },'
    print '                           { name:"Approximate", value:"2" },'
    print '                        ] // End of Enumerations '
    print '                     },'
    print '                     { name:"TXT",'
    print '                       desc:"Associated Text",'
    print '                       type:"String",'
    print '                       optional:"R",'
    print '                       defValue:"UNK",'
    print '                       length:"254"'
    print '                     },'
    print '                     { name:"UID",'
    print '                       desc:"MGCP Feature universally unique identifier",'
    print '                       type:"String",'
    print '                       optional:"R",'
    print '                       defValue:"",'
    print '                       length:"36"'
    print '                     },'
    print '                     { name:"SDV",'
    print '                       desc:"Source Date and Time",'
    print '                       type:"String",'
    print '                       optional:"R",'
    print '                       defValue:"UNK",'
    print '                       length:"20"'
    print '                     },'
    print '                     { name:"SDP",'
    print '                       desc:"Source Description",'
    print '                       type:"String",'
    print '                       optional:"R",'
    print '                       defValue:"UNK",'
    print '                       length:"254"'
    print '                     },'
    print '                     { name:"SRT",'
    print '                       desc:"Source Type",'
    print '                       type:"enumeration",'
    print '                       optional:"R",'
    print '                       defValue:"0",'
    print '                       enumerations:['
    print '                           { name:"Unknown", value:"0" },'
    print '                           { name:"ADRG", value:"1" },'
    print '                           { name:"AAFIF", value:"2" },'
    print '                           { name:"CHUM", value:"3" },'
    print '                           { name:"City Graphic", value:"4" },'
    print '                           { name:"Combat Chart", value:"5" },'
    print '                           { name:"CADRG", value:"6" },'
    print '                           { name:"CIB1", value:"7" },'
    print '                           { name:"CIB5", value:"8" },'
    print '                           { name:"DNC", value:"10" },'
    print '                           { name:"DPF", value:"11" },'
    print '                           { name:"DTOP 1", value:"16" },'
    print '                           { name:"DTOP 2", value:"17" },'
    print '                           { name:"DTOP 3", value:"18" },'
    print '                           { name:"DTOP 4", value:"19" },'
    print '                           { name:"DTOP 5", value:"20" },'
    print '                           { name:"DVOF", value:"21" },'
    print '                           { name:"FFD", value:"22" },'
    print '                           { name:"Land Cover", value:"24" },'
    print '                           { name:"GeoNames", value:"25" },'
    print '                           { name:"GPS", value:"26" },'
    print '                           { name:"ICM", value:"27" },'
    print '                           { name:"NTM Imagery", value:"29" },'
    print '                           { name:"Imagery", value:"30" },'
    print '                           { name:"ITD", value:"31" },'
    print '                           { name:"IVD", value:"32" },'
    print '                           { name:"International Boundaries", value:"33" },'
    print '                           { name:"JOG", value:"34" },'
    print '                           { name:"LWD", value:"36" },'
    print '                           { name:"MC&G", value:"37" },'
    print '                           { name:"MSD 1", value:"38" },'
    print '                           { name:"MSD 2", value:"39" },'
    print '                           { name:"MSD 3", value:"40" },'
    print '                           { name:"MSD 4", value:"41" },'
    print '                           { name:"MSD 5", value:"42" },'
    print '                           { name:"MIDB", value:"43" },'
    print '                           { name:"Native Data", value:"44" },'
    print '                           { name:"Native Map", value:"45" },'
    print '                           { name:"Medical Facilities", value:"46" },'
    print '                           { name:"NATE", value:"47" },'
    print '                           { name:"Planning Graphic", value:"48" },'
    print '                           { name:"SRTM", value:"50" },'
    print '                           { name:"Site Map", value:"51" },'
    print '                           { name:"TOD 0", value:"52" },'
    print '                           { name:"TOD 1", value:"53" },'
    print '                           { name:"TOD 2", value:"54" },'
    print '                           { name:"TOD 3", value:"55" },'
    print '                           { name:"TOD 4", value:"56" },'
    print '                           { name:"TOD 5", value:"57" },'
    print '                           { name:"UN Data", value:"59" },'
    print '                           { name:"UVMap", value:"60" },'
    print '                           { name:"VITD", value:"61" },'
    print '                           { name:"VMap 0", value:"62" },'
    print '                           { name:"VMap 1", value:"63" },'
    print '                           { name:"VMap 2", value:"64" },'
    print '                           { name:"WVS Plus", value:"65" },'
    print '                           { name:"SPOT HRG Digital Terrain Model", value:"85" },'
    print '                           { name:"Vector Data", value:"92" },'
    print '                           { name:"1:25k Vector Data", value:"93" },'
    print '                           { name:"1:50k Vector Data", value:"94" },'
    print '                           { name:"1:100k Vector Data", value:"95" },'
    print '                           { name:"Very High Resolution Commercial Monoscopic Imagery", value:"110" },'
    print '                           { name:"Very High Resolution Commercial Stereoscopic Imagery", value:"111" },'
    print '                           { name:"High Resolution Commercial Monoscopic Imagery", value:"112" },'
    print '                           { name:"High Resolution Commercial Stereoscopic Imagery", value:"113" },'
    print '                           { name:"Medium Resolution Commercial Monoscopic Imagery", value:"114" },'
    print '                           { name:"Medium Resolution Commercial Stereoscopic Imagery", value:"115" },'
    print '                           { name:"Low Resolution Commercial Monoscopic Imagery", value:"116" },'
    print '                           { name:"Low Resolution Commercial Stereoscopic Imagery", value:"117" },'
    print '                           { name:"Map 1:25k", value:"118" },'
    print '                           { name:"Map 1:50k", value:"119" },'
    print '                           { name:"Map 1:100k", value:"120" },'
    print '                           { name:"Routing Data", value:"121" },'
    print '                           { name:"Multiple", value:"996" },'
    print '                           { name:"Unpopulated", value:"997" },'
    print '                           { name:"Not Applicable", value:"998" },'
    print '                           { name:"Other", value:"999" },'
    print '                        ] // End of Enumerations '
    print '                     },'
    print '                     { name:"CCN",'
    print '                       desc:"Commercial Copyright Notice",'
    print '                       type:"String",'
    print '                       optional:"R",'
    print '                       defValue:"UNK",'
    print '                       length:"254"'
    print '                     },'


def openFile(path, mode):
    if path.endswith(".gz"):
        return gzip.GzipFile(path, mode)
    else:
        return open(path, mode)

######
# Main Starts Here
# 
parser = argparse.ArgumentParser(description='Process a text file and build a schema')
parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
parser.add_argument('textfile', help='The MGCP spec as a text file', action='store')

args = parser.parse_args()

main_text_file = args.textfile

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

inHeader = True
inFeature = False
startFeature = False
inAttrib = False
inEnum = False
inString = False
eNameExtra = ''

# Now start printing
print notice
print
print "var _global = (0, eval)('this');"
print 'if (!_global.mgcp)'
print '{'
print '  _global.mgcp = {};'
print '}'
print
#print '__setupPackage__(__extension__);'
print 'mgcp.schema = {'
print '// Schema built from %s' % (main_text_file)
print 'getDbSchema: function()'
print '{'
print '    var schema = [' # And so it begins...


infile = openFile(main_text_file, 'rb')
try:
    for line in infile:

        line = line.strip()

        if (len(line) < 4) or (line.find('Page') > -1) or (line.find('Schema') > -1) or (line.find('dated') > -1):
            continue

        if line.find('____') > -1:
            inHeader = False
            startFeature = True
            continue

        if inHeader:
            continue

        if line.find('Metadata Attribute') > -1: 
            break

        if startFeature:
            if inFeature: 
                if inEnum: 
                    eName = eName + ' ' + eNameExtra 
                    print '                           { name:"%s", value:"%s" }' % (eName,eVal) 
                    print '                        ] // End of Enumerations ';
                    inEnum = False
                print '                     }'
                print '                    ] // End of Columns'
                print '          }, // End of Feature\n'
            inFeature = True
            inAttrib = False
            startFeature = False 

            (fCode,tGeom,fName) = re.split(r'\s{2,}',line) 
            fName = fName.strip() 
            tGeom = tGeom.strip() 
            fGeom = tGeom.split()[0]

            # Try out the funky new names
            #print '        { name:"%s",' % (fName.replace(' ','_').upper() + '_' + fGeom[0]) # XXXXX_Y
            print '        { name:"%s",' % (fGeom[0] + fCode) # name = geom + FCODE
            print '          fcode:"%s",' % (fCode)
            print '          desc:"%s",' % (fName)
            # backward compatibility for a while
            #geoList = {'P':'Point', 'S':'Surface', 'C':'Curve'}
            #geoList = {'P':'Point', 'S':'Area', 'C':'Line'}
            #geomType = geoList[geomType]

            print '          geom:"%s",' % (fGeom)
            print '          columns:[ '
            print '                     { name:"FCODE",'
            print '                       desc:"Feature Code",'
            print '                       type:"String",'
            print '                       optional:"R",'
            #print '                       defValue:"%s",' % (fCode)
            print '                       defValue:"",'
            # print '                       minimum:"0",'
            # print '                       maximum:"5",'
            print '                       length:"5"'
            print '                     },'
            dumpMeta()
            continue

        #if (line.find('Real') > -1) or (line.find('Float') > -1):
        if (line.find('Real') > -1): 
            (junk,aName,aDesc) = re.split(r'\s{2,}',line) 
            aName = aName.strip()
            aDesc = aDesc.strip()
            if inEnum: 
                eName = eName + ' ' + eNameExtra 
                print '                           { name:"%s", value:"%s" }' % (eName,eVal) 
                print '                        ] // End of Enumerations '; 
            if inAttrib: 
                print '                     },'

            inAttrib = True
            inEnum = False
            print '                     { name:"%s",' % (aName)
            print '                       desc:"%s",' % (aDesc)
            print '                       type:"Real",'
            if line.find('Required') > -1: 
                opt = 'R' 
            else: 
                opt = 'O'
            print '                       optional:"%s",' % (opt)
            print '                       defValue:"-32767.0"'
            continue

        if line.find('Integer') > -1:
            (junk,aName,aDesc) = re.split(r'\s{2,}',line) 
            aName = aName.strip() 
            aDesc = aDesc.strip()
            if inEnum: 
                eName = eName + ' ' + eNameExtra 
                print '                           { name:"%s", value:"%s" }' % (eName,eVal) 
                print '                        ] // End of Enumerations '; 
            if inAttrib: 
                print '                     },'

            inEnum = False
            inAttrib = True
            print '                     { name:"%s",' % (aName)
            print '                       desc:"%s",' % (aDesc)
            print '                       type:"Integer",'
            if line.find('Required') > -1: 
                opt = 'R' 
            else: 
                opt = 'O'
            print '                       optional:"%s",' % (opt)
            print '                       defValue:"-32767"'
            continue

        if (line.find('Required String') > -1) or (line.find('Optional String') > -1):
            (junk,aName,aDesc) = re.split(r'\s{2,}',line) 
            aName = aName.strip() 
            aDesc = aDesc.strip()
            if inEnum: 
                eName = eName + ' ' + eNameExtra 
                print '                           { name:"%s", value:"%s" }' % (eName,eVal) 
                print '                        ] // End of Enumerations ';

            if inAttrib: 
                print '                     },'

            inAttrib = True
            inEnum = False 
            print '                     { name:"%s",' % (aName)
            print '                       desc:"%s",' % (aDesc)
            print '                       type:"String",'
            if line.find('Required') > -1: 
                opt = 'R' 
            else: 
                opt = 'O'
            print '                       optional:"%s",' % (opt)
            print '                       defValue:"UNK",'
            continue

        if line.find('Length') > -1:
            tmp = line.split()
            print '                       length:"%s"' % (tmp[3])
            continue

        if line.find('Enumerated') > -1:
            (junk,aName,aDesc) = re.split(r'\s{2,}',line) 
            aDesc = aDesc.strip() 
            aName = aName.strip()
            if inEnum: 
                eName = eName + ' ' + eNameExtra 
                print '                           { name:"%s", value:"%s" }' % (eName,eVal) 
                print '                        ] // End of Enumerations ';
            if inAttrib: 
                print '                     },'

            inAttrib = True
            inEnum = True
            print '                     { name:"%s",' % (aName);
            print '                       desc:"%s",' % (aDesc);
            print '                       type:"enumeration",';
            if line.find('Required') > -1: 
                opt = 'R' 
            else: 
                opt = 'O'
            print '                       optional:"%s",' % (opt)
            print '                       defValue:"0",'
            print '                       enumerations:[';
            continue

        if line.find('enumerants') > -1:
            inEnum = True
            eName = ''
            continue

        if inEnum: 
            if line.find(',') > -1: 
                if eName != '': 
                    eName = eName + ' ' + eNameExtra 
                    print '                           { name:"%s", value:"%s" },' % (eName,eVal) 
                (eVal,eName) = line.split(',')
                eNameExtra = ''
            else:
                eNameExtra = ' ' + line
            continue

        if line == 'No prescribed attribute associations': 
            print '                     { name:"TEXT",'
            print '                       desc:"Free text Field",'
            print '                       type:"String",'
            print '                       optional:"R",'
            print '                       defValue:"UNK",'
            print '                       length:"254"'

finally:
    infile.close()

if inEnum:
    print '                        ] // End of Enumerations ';
print '                     }'
print '                    ] // End of Columns'
print '          } // End of Feature\n'
print '    ]; // End of schema\n' # End of schema
print '    return schema; \n'
print '} // End of getDbSchema\n'
print '} // End of mgcp.schema\n'
print
print 'exports.getDbSchema = mgcp.schema.getDbSchema;'
print

# End
