#!/usr/bin/python
import datetime
import json
import sys
import urllib2
from pprint import pprint

tag = sys.argv[1]

print tag

allTags = []

done = False
for i in range(1, 10):
    data = urllib2.urlopen("http://taginfo.openstreetmap.org/api/4/key/values?key=%s&filter=all&lang=en&sortname=count&sortorder=desc&page=%d&rp=23&qtype=value&format=json_pretty" % (tag, i))
    values = json.load(data)
    allTags.extend(values['data'])

for tinfo in allTags:
    if (tinfo['count'] > 100):
        print '    "tag": {'
        print '        "name": "%s=%s",' % (tag, tinfo['value'])
        if (tinfo['description'] != '' and tinfo['description'].strip() != '???'):
            print '        "description": "%s",' %(tinfo['description'])
        print '        "isA": "%s",' % (tag)
        print '        "#": "Count in tag info is %d on %s"' % \
            (tinfo['count'], datetime.date.today())
        print '    },'

