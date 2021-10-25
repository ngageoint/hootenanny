#!/usr/bin/python

"""
Creates a JSON file of configuration options used by UI
"""

import sys
import json
import os
import re

from collections import OrderedDict

if (len(sys.argv) != 5):
    print("Usage:")
    print(sys.argv[0] + " (UI Groups file) (Config Options file) (Group Defaults file) (output file)")
    sys.exit(-1)

fn = sys.argv[1]
parent = sys.argv[2]
defaults = sys.argv[3]
headerFn = sys.argv[4]

uiGroups = json.loads("".join(open(fn).readlines()), object_pairs_hook=OrderedDict)
configOptions = json.loads("".join(open(parent).readlines()))
groupDefaults = json.loads("".join(open(defaults).readlines()))

optionTypesMap = {
    'text': ['double', 'int', 'string', 'long', 'list'],
    'checkbox': ['bool']
}

def toInput(optionType):
    matchingInput = ''
    for inputType, options in optionTypesMap.items():
        if optionType in options:
            matchingInput = inputType
            break;
    return matchingInput

def toLabel(conflateType):
    return 'Poi to Polygon' if conflateType == 'PoiPolygon' else conflateType;

def getDefaultValue(templateKey):
    defaultValueKey =  ''.join( c for c in templateKey if c not in '${}').replace('.', ' ').title().replace(' ', '')
    defaultValue = configOptions[defaultValueKey]['default']
    if '${' in defaultValue:
        getDefaultValue(defaultValue)
    else:
        return defaultValue;

 # merges matches between ConfigOptions.json and conflationTypesMap.json
 # on command key back into conflationTypesMap. Outputs as ConflationTypesGroups.json
def createUiJSON(groups, options):
    try:
        uiGroupsArray = []
        configMembers = [{'key': key, 'member': member} for key, member in options.items()]
        cleaningOptions = options['MapCleanerTransforms']['default'].split(';')
        for conflateType, conflateConfig in groups.items():
            if conflateType == 'RoadsNetwork': continue;
            members = []
            typeMembers = conflateConfig['members']
            isCleaning = conflateType == 'Cleaning'
            for flag in typeMembers.keys():
                if isCleaning:
                    members.append({
                        'label': typeMembers[flag],
                        'id': flag.replace('hoot::', ''),
                        'input': 'checkbox',
                        'default': 'true' if flag in cleaningOptions else 'false'
                    })
                else:
                    match = next(
                        (m['key'] for m in configMembers if m['member']['key'] == flag),
                        None
                    )

                    if match != None:
                        memberConfig = { 'label': typeMembers[flag], 'id': match }
                        memberConfig.update(configOptions[match])
                        del memberConfig['key']
                        templateDefault = re.findall(r'(?<=\${).*(?=})', memberConfig['default'])
                        # If default points to file of values
                        if 'HOOT_HOME' in memberConfig['default'] and memberConfig['default'].endswith('.json'):
                            filePath = os.path.expandvars(memberConfig['default'])
                            if os.path.isfile(filePath):
                                with open(filePath, 'r') as jsonFile:
                                    data = json.load(jsonFile)
                                    for (key, value) in data.items():
                                        #Use key as label for what the objects inside represent
                                        category = key.replace('_', ' ').capitalize()

                                        # ignore comments in the json
                                        if key != '#' and isinstance(value, list):
                                            for categoryObj in value:
                                                memberObject = {
                                                    'id': match,
                                                    'input': 'text',
                                                    'type': 'json',
                                                    'parentKey': key
                                                }
                                                objKeys = []

                                                if 'key' in categoryObj:
                                                    categoryKey = categoryObj['key']
                                                    objKeys.append('key')
                                                    memberObject['key'] = categoryKey
                                                else:
                                                    categoryKey = ''

                                                if 'value' in categoryObj:
                                                    categoryVal = categoryObj['value']
                                                    objKeys.append('value')
                                                    memberObject['value'] = categoryVal
                                                else:
                                                    categoryVal = '*'

                                                if 'distance' in categoryObj:
                                                    categoryDefault = categoryObj['distance']
                                                    objKeys.append('distance')
                                                    memberObject['distance'] = categoryDefault
                                                else:
                                                    categoryDefault = ''

                                                label = category + ' [' + categoryKey + '=' + categoryVal + ']'
                                                description = category + ' for ' + categoryKey

                                                memberObject['default'] = categoryDefault
                                                memberObject['description'] = description
                                                memberObject['label'] = label
                                                memberObject['keysList'] = objKeys

                                                members.append(memberObject)

                                    continue # Need this so that the member config that describes all the json components doesn't get added
                        elif len(templateDefault) == 1:
                            defaultKey = templateDefault[0]
                            memberConfig['default'] = next(
                            (m['member']['default'] for m in configMembers if m['member']['key'] == defaultKey),
                            ''
                            )
                            if '${' in memberConfig['default']:
                                memberConfig['default'] = getDefaultValue(memberConfig['default'])

                        memberConfig['input'] = toInput(memberConfig['type'])
                        members.append(memberConfig)

            if (conflateType == 'Roads'):
                members = groupDefaults[conflateType] + members

            uiGroupsArray.append({ 'name': conflateType, 'label': toLabel(conflateType), 'members': members  })
    except:
        print 'failed to load %s' % fn2
    finally:
        return json.dumps({ 'hoot2': uiGroupsArray })

#open(headerFn, 'w').write(createUiJSON(uiGroups, configOptions))
finalJSON = createUiJSON(uiGroups, configOptions)
open(headerFn, 'w').write(finalJSON)
