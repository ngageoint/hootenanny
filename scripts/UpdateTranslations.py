#!/bin/python

"""
Removes every translation script from the passed in list via the web services and then re-adds the 
latest version of those same scripts via the services
"""

import sys
import json
import requests
import string
import os
from requests.auth import HTTPBasicAuth

baseUrl = sys.argv[1]
#print "baseUrl: " + baseUrl
translationScriptNamesToUpdate = sys.argv[2].split(",")
#print "translationScriptNamesToUpdate: "
#print translationScriptNamesToUpdate
#print "translationScriptNamesToUpdate length: " + str(len(translationScriptNamesToUpdate))
translationsHomeDir = sys.argv[3]
#print "translationsHomeDir: " + translationsHomeDir

warnings = ""

# get a list of all the translations scripts
getScriptsResponse = requests.get(baseUrl + "/hoot-services/ingest/customscript/getlist", auth=HTTPBasicAuth("", ""), verify=False)
getScriptsResponseData = json.loads(getScriptsResponse.text)
#print "getScriptsResponseData: " + str(getScriptsResponseData)

# for each one of the exisiting scripts in the list of the translations to be updated, add it to a 
# delete request; then delete the batch of them
scriptsToDelete = []
for existingScript in getScriptsResponseData:
	# if translationsToUpdate contains the script name, add it to the delete list
	if existingScript['NAME'] in translationScriptNamesToUpdate:
		scriptToDelete = {}
		scriptToDelete['name'] = existingScript['NAME']
		scriptsToDelete.append(scriptToDelete)

deleteScriptsRequest = {}
headers = {'Content-type': 'application/json'}
deleteScriptsRequest = json.dumps({ "scripts": scriptsToDelete })
#print "deleteScriptsRequest: " + str(deleteScriptsRequest)
deleteScriptsResponse = requests.delete(baseUrl + "/hoot-services/ingest/customscript/deletescripts", auth=HTTPBasicAuth("", ""), verify=False, headers=headers, data=deleteScriptsRequest)
deleteScriptsResponseData = json.loads(deleteScriptsResponse.text)
#print "deleteScriptsResponseData: " + str(deleteScriptsResponseData)
if len(deleteScriptsResponseData["scriptsModified"]) != len(scriptsToDelete):
	warnings += "Number of translation scripts requested for delete not equal to the number of translation scripts deleted.\n"

# for each script in translationsToUpdate upload the new script (verify the response contains a 
# list of all that were supposed to be saved; warn otherwise)
scriptsToUpload = []
for scriptName in translationScriptNamesToUpdate:
	#  assumes the names of the scripts being updated always equals the script's file name.
	scriptFileName = translationsHomeDir + "/" + scriptName + ".js"
	if os.path.isfile(scriptFileName):
		scriptToUpload = {}
		with open (translationsHomeDir + "/" + scriptName + ".js") as scriptFile:
			scriptToUpload['content'] = scriptFile.read()
			#print "content: " + scriptToUpload['content']
		scriptToUpload['name'] = scriptName
		scriptToUpload['description'] = scriptName
		scriptsToUpload.append(scriptToUpload)
	else:
		warnings += "File not found: " + scriptFileName + "\n"

saveScriptsRequest = {}
headers = {'Content-type': 'application/json'}
saveScriptsRequest = json.dumps({ "scripts": scriptsToUpload })
#print "saveScriptsRequest: " + str(saveScriptsRequest)
saveScriptsResponse = requests.post(baseUrl + "/hoot-services/ingest/customscript/saveMultiple", auth=HTTPBasicAuth("", ""), verify=False, headers=headers, data=saveScriptsRequest)
saveScriptsResponseData = json.loads(saveScriptsResponse.text)
#print "saveScriptsResponseData: " + str(saveScriptsResponseData)
if len(saveScriptsResponseData["scriptsModified"]) != len(scriptsToUpload):
	warnings += "Number of translation scripts requested for save not equal to the number of translation scripts saved.\n"

if warnings == "":
	print "SUCCESS"
else:
	print warnings
