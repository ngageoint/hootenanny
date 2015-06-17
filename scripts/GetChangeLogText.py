#!/bin/python

"""
Uses the input query to retrieve a list of tickets and constructs Hoot change log text from them.
"""

import sys
import json
import requests
import string
from requests.auth import HTTPBasicAuth

ticketIds = sys.argv[1].split(",")
#print "ticketIds: "
#print ticketIds
#print "ticketIds length: " + str(len(ticketIds))
queryUrl = sys.argv[2]
changeLogText=""
limit = 100
offset = 0
totalCount = 999999

while offset < totalCount:
	limitedUrl = queryUrl + "&offset=" + str(offset) + "&limit=" + str(limit)
	#print "limitedUrl: " + limitedUrl
	response = requests.get(limitedUrl, auth=HTTPBasicAuth("", ""), verify=False)
	responseData = json.loads(response.text)
	totalCount = responseData['total_count']
	#print "totalCount: " + str(totalCount)
	for issue in responseData['issues']:
		#print "examining: " + str(issue['id'])
		if str(issue['id']) in ticketIds:
			#print "found: " + str(issue['id'])
			changeLogText += "#" + str(issue['id']) + " - " + issue['subject'].replace("-", "") + " - " + str(issue['status']['name']) + "\\n"

	offset += limit

print changeLogText
