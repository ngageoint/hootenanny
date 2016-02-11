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
#* @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com/)
#*/

import sys, os, json, datetime, urllib2, argparse
from pprint import pprint
from time import sleep

def compareTags(t1, t2):
    if (t1['name'] < t2['name']):
        return -1
    elif (t1['name'] > t2['name']):
        return 1
    else:
        return 0


def getUrl(url):
    try:
        sleep(0.5)
        return urllib2.urlopen(url)
    except:
        print "Failed to get URL: " + url
        raise


def filterKeys(keys):
    result = []

    startsWithFilter = [
        "adr_les",
        "b5m:",
        "bag:",
        "bak:",
        "bmo:",
        "canvec:",
        "chicago:",
        "cladr:",
        "clc:",
        "CLC:",
        "dcgis:",
        "FLATE",
        "GeoBaseNHN:",
        "geobase:",
        "gnis:",
        "GNS:",
        "gst:",
        "HFCS",
        "it:fvg:ctrn:code",
        "ID",
        "kms:",
        "KSJ2:",
        "lbcs:",
        "linz",
        "LINZ",
        "lojic:",
        "maaamet:ETAK",
        "massgis:",
        "mvdgis:",
        "name:",
        "naptan:",
        "ncat",
        "nhd",
        "NHS",
        "note:",
        "nycdoitt:",
        "NHD:",
        "OBJTYPE",
        "OPPDATERIN",
        "osak:",
        "osmc:",
        "project:",
        "SK53_bulk",
        "tiger:",
        "Type",
        "WroclawGIS:",
        "yh:"
    ]

    for k in keys:
        skip = False
        for sw in startsWithFilter:
            if k.startswith(sw):
                skip = True
        if not skip:
            result.append(k)

    return result
# End filterKeys


def findKey(name):
    global schema
    if name in aliases:
        return findKey(aliases[name])

    for t in schema:
        if t['name'] == name:
            return t

    return None
# End findKey


def getAllKeys(minCount):
    page = 1
    done = False
    result = []
    while not done:
        data = getUrl("http://taginfo.openstreetmap.org/api/4/keys/all?page=%d&rp=100&sortname=count_all&filter=in_wiki&sortorder=desc" % (page))
        values = json.load(data)
        for keys in values['data']:
            if keys['count_all'] >= minCount:
                result.append(keys['key'])
            else:
                done = True
        page = page + 1

    return result
# End getAllKeys


def getDataType(s):
    if not s:
        return None

    if 'dataType' in s:
        return s['dataType']

    if 'isA' in s:
        return getDataType(findKey(s['isA']))

    return None
# End getDataType


def getWikiDetails(k, v):
    data = getUrl("http://taginfo.openstreetmap.org/api/4/tag/wiki_pages?key=%s&value=%s&format=json_pretty" % \
        (urlQuote(k), urlQuote(v)))
    values = json.load(data)
    result = None
    for v in values['data']:
        #print 'v:',v
        if v['lang'] == 'en':
            result = v

    if result == None:
        if len(values['data']) > 0:
            result = values['data'][0]
        else:
            result = {}

    return result
# End getWikiDetails


def grabValuesForKey(k):
    tags = []
    done = False
    page = 1
    while not done and page < 10:
        data = getUrl("http://taginfo.openstreetmap.org/api/4/key/values?key=%s&filter=all&lang=en&sortname=count&sortorder=desc&page=%d&rp=999&qtype=value&format=json_pretty" % (urlQuote(k), page))
        values = json.load(data)
        tags.extend(values['data'])
        if len(values['data']) == 0:
            done = True
        page = page + 1
    return tags
# End grabValuesForKey


# What file name should this tag be put into?
def getKeyCategory(tag):
    if not tag or not isinstance(tag, dict):
        return None

    key = tag['name']
    if '=' in key:
        key = key.split('=')[0]

    if ':' in key:
        key = key.split(':')[0]

    if tag['objectType'] == 'compound':
        k = getKeyCategory(findKey(tag['isA']))
        if k:
            return k

    if key.startswith('abstract_'):
        return key.replace('abstract_', '')
    elif '_name' in key:
        return 'name'
    elif key.startswith('regex?'):
        if 'isA' in tag:
            parent = findKey(tag['isA'])
            if parent:
                return getKeyCategory(parent)
            else:
                return tag['isA']
        return key
    else:
        return key
# End getKeyCategory


def getTag(kvp):
    for e in schema:
        if 'name' in e and e['name'] == kvp:
            return e
    t = {
        'objectType':'tag',
        'name':kvp
    }
    schema.append(t)
    return t
# End getTag


def getEnumeratedTag(key,value):
    haveEnum = False
    for e in schema:
        if 'name' in e and e['name'] == key:
            if 'dataType' in e:
                haveEnum = True
                break

    if not haveEnum:
        eTag = {
            'dataType':'enumeration',
            'objectType':'tag',
            'influence':1,
            'name':key
        }
        schema.append(eTag)

    tag = getTag(key + '=' + value)
    tag['isA'] = key

    return tag
# End getTag


def insertAliases(t):
    if 'aliases' in t:
        for a in t['aliases']:
            aliases[a] = t['name']
# End insertAliases


def readValuesIntoSchema(k):
    s = findKey(k)

    if s:
        dataType = getDataType(s)
        if dataType and dataType != 'enumeration':
            print "Skipping read values for %s." % (k)
            return

    tags = grabValuesForKey(k)
    print "Reading %d tags for key %s." % (len(tags),k)

    for t in tags:
        if (t['count'] > 200 or t['in_wiki'] == True):
            name = k + '=' + t['value']
            if name not in aliases:
                st = getTag(k + '=' + t['value'])
                st['tagInfoCount'] = t['count']

                # Try and add an "isA" attribute
                tCat = getKeyCategory(st)
                tTag = findKey(tCat)
                #print 'tTag:',tTag,'tCat:',tCat

                if tTag:
                    if 'isA' in tTag:
                        st['isA'] = tTag['isA']
                    else:
                        st['isA'] = tCat

                if t['description'] != '':
                    st['tagInfoDescription'] = t['description']

                wiki = getWikiDetails(k, t['value'])
                geom = []
                if len(wiki) > 0:
                    if wiki['on_node'] == True:
                        geom.append('node')
                    if wiki['on_way'] == True:
                        geom.append('linestring')
                    if wiki['on_area'] == True:
                        geom.append('area')
                    if wiki['on_relation'] == True:
                        geom.append('relation')
                    if len(geom) > 0:
                        st['geometries'] = geom
            else:
                print "Ignoring alias: " + name
# End readValuesIntoSchema


# Read all tags out of schema files
def readSchemaFile(path):
    global schema
    d = os.path.dirname(path)
    fp = open(path, 'r')
    s = json.load(fp)

    # Go through all the elements in s
    for i in s:
        # Call the imports
        if 'import' in i:
            readSchemaFile(d + '/' + i['import'])
        # add s to schema
        else:
            schema.append(i)
            insertAliases(i)
# End readSchemaFile


def toJson(v):
    return json.dumps(v, sort_keys=True, indent=4, separators=(',', ': '))
# End toJson


def urlQuote(s):
    return urllib2.quote(s.encode('utf8'))
# End urlQuote


def writeSchemaFile(path):
    global schema
    sortedKeys = {}
    if not os.path.exists(path):
        os.makedirs(path)

    if os.path.exists(path + '/schema.json'):
        raise Exception('schema.json already exists. Please delete first.')

    for s in schema:
        k = getKeyCategory(s)
        if k not in sortedKeys:
            sortedKeys[k] = []
        sortedKeys[k].append(s)

    top = []
    for (sk, sv) in sorted(sortedKeys.iteritems()):
        print 'Writeing %s JSON file' % (sk)
        top.append({'import':sk + '.json'})
        sv = sorted(sv, cmp=compareTags)
        fp = open(path + '/' + sk + '.json', 'w')
        fp.write(toJson(sv))

    fp = open(path + '/schema.json', 'w')
    fp.write(toJson(top))
# End writeSchemaFile


def addKey(key):
    global keys

    if not key in keys:
        keys.append(key)
# End addKey


def processRules(path,attributes):
    #d = os.path.dirname(path)
    fp = open(path, 'r')

    inOne2One = False
    inTxtBiased = False
    inNumBiased = False

    for line in fp:
        line = line.strip()

        # Skip empty lines
        if line == '':
            continue

        # Skip comments
        if line[0] == '/' or line[0] =='*':
            continue

        if line.find('txtBiased') == 0:
            inTxtBiased = True
            continue

        if line.find('numBiased') == 0:
            inNumBiased = True
            continue

        if line.find('one2one ') == 0 or line.find('one2oneIn') == 0:
            inOne2One = True
            continue

        # End of a section. NOTE: The sections can not overlap
        if line.find('},') == 0 or line.find('],') == 0:
            inTxtBiased = False
            inNumBiased = False
            inOne2One = False
            continue

        # Sort out Text and Number attributes
        if line[0] == "'" and (inTxtBiased or inNumBiased):
            # Line should look like this:
            # 'NAM':'name', // Name
            comment = line.split('//')

            (key,value) = comment[0].replace("',",'').split("':'")
            key = key.replace("'",'')
            value = value.strip()

            if attributes and not key in attributes:
                continue

            # Add this to the list of keys we look for in TagInfo
            addKey(value)

            st = getTag(value)

            if len(comment) > 1:
                st['tagInfoDescription'] = comment[1].strip()

            if inTxtBiased:
                st['dataType'] = 'text'
                continue

            if inNumBiased:
                st['dataType'] = 'real'
                continue

        # Sort out one2one - Geonames style
        if line[0] == "'" and inOne2One:
            # Line should look like this for one2one rules in Geonames etc
            # 'ASYL':{'amenity':'social_facility', 'social_facility:for':'mental_health'}, //  asylum: a facility
            comment = line.split('//')

            (attr,vString) = comment[0].split(":{'")
            attr = attr.replace("'",'')

            if attributes and not attr in attributes:
                continue

            values = vString.strip().replace("'},",'').split("','")

            for tKey in values:
                (key,value) = tKey.split("':'")

                # Add this to the list of keys we look for in TagInfo
                addKey(key)

                st = getTag(key.strip() + '=' + value.strip())
                st['tagInfoCount'] = 0

                # Not using the comment field at the moment
                #if len(comment) > 1:
                    #st['tagInfoDescription'] = comment[1].strip()
            continue

        # Sort out one2one - Standard type
        # NOTE: These are enumerated and make enumerated types
        if line[0] == "[" and inOne2One:
            # Skip empty values
            if line.find('undefined') > -1:
                continue

            # Line should look like this for the tandard one2one rules
            # ['AZC','1000','artificial','no'], // Natural
            comment = line.strip().split('//')

            (attr,aVal,key,value) = comment[0].replace("'],",'').replace("['",'').split("','")

            if attributes and not attr in attributes:
                continue

            # Add this to the list of keys we look for in TagInfo
            addKey(key.strip())

            st = getEnumeratedTag(key,value.strip())
            st['tagInfoCount'] = 0

            if len(comment) > 1:
                st['tagInfoDescription'] = comment[1].strip()

            continue

# End processRules


###########
# Main Starts Here
#
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Update the Hootenanny OSM schema')
    parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
    parser.add_argument('--noTaginfo', help="Don't pull tags from TagInfo",action='store_true', default=False)
    parser.add_argument('--maxKeys', help="Maximum number of keys to pull from TagInfo",action='store', default=100000)
    parser.add_argument('--rulesFile', nargs='?', help='Read a *_rules.js file to get tags', action='store')
    parser.add_argument('--attributes', nargs='*', help='Only process these attributes from a rules file: attribute, attribute, attribute,...', action='store')
    parser.add_argument('inputFile', help='The input JSON schema file', action='store')
    parser.add_argument('outputDir', help='The output directory', action='store')
    parser.add_argument('keys', nargs='*', help='Keys to process: key1,key2,key3...', action='store')

    args = parser.parse_args()

    inputFile = args.inputFile
    outputDir = args.outputDir
    rulesFile = args.rulesFile
    keys = args.keys
    attributes = args.attributes

    allTags = []
    schema = []
    aliases = {}


    readSchemaFile(inputFile)

    if rulesFile:
        processRules(rulesFile,attributes)

    # If we dont have any keys - from rules files or the command line - go get a stack them from Taginfo
    if len(keys) == 0:
        print 'About to grab %d keys from TagInfo' % (args.maxKeys)
        keys = filterKeys(getAllKeys(args.maxKeys))
        keys.sort()
    else:
        keys = filterKeys(keys)
        keys.sort()

    # Read requested tags from taginfo and update the schema
    if not args.noTaginfo:
        for k in keys:
            readValuesIntoSchema(k)

    # Export tags from schema into new schema directory
    writeSchemaFile(outputDir)


    #for tinfo in allTags:
    #    if (tinfo['count'] > 100):
    #        print '    "tag": {'
    #        print '        "name": "%s=%s",' % (key, tinfo['value'])
    #        if (tinfo['description'] != '' and tinfo['description'].strip() != '???'):
    #            print '        "description": "%s",' %(tinfo['description'])
    #        print '        "isA": "%s",' % (key)
    #        print '        "#": "Count in tag info is %d on %s"' % \
    #            (tinfo['count'], datetime.date.today())
    #        print '    },'

