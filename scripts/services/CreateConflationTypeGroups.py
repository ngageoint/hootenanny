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
    print "Usage:"
    print sys.argv[0] + " (config file) (output file)"
    sys.exit(-1)

fn = sys.argv[1]
parent = sys.argv[2]
defaults = sys.argv[3]
headerFn = sys.argv[4]

uiGroups = json.loads("".join(open(fn).readlines()), object_pairs_hook=OrderedDict)
configOptions = json.loads("".join(open(parent).readlines()))
groupDefaults = json.loads("".join(open(defaults).readlines()))

optionTypesMap = {
    'text': ['double', 'int', 'string', 'long'],
    'combobox': ['list'],
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
                        if len(templateDefault) == 1:
                            defaultKey = templateDefault[0]
                            memberConfig['default'] = next(
                                (m['member']['default'] for m in configMembers if m['member']['key'] == defaultKey),
                                ''
                            )
                        memberConfig['input'] = toInput(memberConfig['type'])
                        members.append(memberConfig)



            if (conflateType == 'Roads'):
                members = groupDefaults[conflateType] + members

            uiGroupsArray.append({ 'name': conflateType, 'label': toLabel(conflateType), 'members': members  })
    except:
        print 'failed to load %s' % fn2
    finally:
        return json.dumps({ 'hoot2': uiGroupsArray })

open(headerFn, 'w').write(createUiJSON(uiGroups, configOptions))
