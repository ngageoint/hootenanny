#!/usr/bin/python
import datetime
import json
import os
import sys
import urllib2
from pprint import pprint
from time import sleep

if len(sys.argv) < 3:
    print "%s (input file) (output dir) [key1, key2, ...]" % sys.argv[0]
    sys.exit(-1)

inputFile = sys.argv[1]
outputDir = sys.argv[2]
keys = sys.argv[3:]

allTags = []

schema = []
aliases = {}

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

def findKey(name):
    global schema
    if name in aliases:
        return findKey(aliases[name])
    for t in schema:
        if t['name'] == name:
            return t
    return None

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

def getDataType(s):
    if 'dataType' in s:
        return s['dataType']
    if 'isA' in s:
        return getDataType(findKey(s['isA']))
    return None

def getWikiDetails(k, v):
    data = getUrl("http://taginfo.openstreetmap.org/api/4/tag/wiki_pages?key=%s&value=%s&format=json_pretty" % \
        (urlQuote(k), urlQuote(v)))
    values = json.load(data)
    result = None

    for v in values['data']:
        if v['lang'] == 'en':
            result = v

    if result == None:
        if len(values["data"]) > 0:
            result = values["data"][0]
        else:
            result = {}

    return result

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

def insertAliases(t):
    if 'aliases' in t:
        for a in t['aliases']:
            aliases[a] = t['name']

def readValuesIntoSchema(k):
    s = findKey(k)
    if s:
        dataType = getDataType(s)
        if dataType and dataType != 'enumeration':
            print "Skipping read values for %s." % k
            return

    tags = grabValuesForKey(k)
    print "Reading %d tags." % (len(tags))

    for t in tags:
        if (t['count'] > 200 or t['in_wiki'] == True):
            name = k + '=' + t['value']
            if name not in aliases:
                st = getTag(k + '=' + t['value'])
                st['tagInfoCount'] = t['count']
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

def toJson(v):
    return json.dumps(v, sort_keys=True, indent=4, separators=(',', ': '))

def urlQuote(s):
    return urllib2.quote(s.encode('utf8'))

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
        print sk
        top.append({'import':sk + '.json'})
        sv = sorted(sv, cmp=compareTags)
        fp = open(path + '/' + sk + '.json', 'w')
        fp.write(toJson(sv))

    fp = open(path + '/schema.json', 'w')
    fp.write(toJson(top))

# Find all the keys that need to be updated
if len(keys) == 0:
    keys = filterKeys(getAllKeys(100000))
    keys.sort()

readSchemaFile(inputFile)

# Read requested tags from taginfo and update schema
for k in keys:
    print k
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

