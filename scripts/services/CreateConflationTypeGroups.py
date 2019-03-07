#!/usr/bin/python

"""
Creates a JSON file of configuration options used by UI
"""

import sys
import json
import os
import re

if (len(sys.argv) != 5):
    print "Usage:"
    print sys.argv[0] + " (config file) (output file)"
    sys.exit(-1)

fn = sys.argv[1]
parent = sys.argv[2]
defaults = sys.argv[3]
headerFn = sys.argv[4]

uiGroups = json.loads("".join(open(fn).readlines()))
configOptions = json.loads("".join(open(parent).readlines()))
groupDefaults = json.loads("".join(open(defaults).readlines()))

optionTypesMap = {
    'text': ['double', 'int', 'string', 'long'],
    'combo': ['list'],
    'check': ['bool']
}

def toInput(optionType):
    matchingInput = ''
    for inputType, options in optionTypesMap.items():
        if optionType in options:
            matchingInput = inputType
            break;
    return matchingInput

 # merges matches between ConfigOptions.json and conflationTypesMap.json
 # on command key back into conflationTypesMap. Outputs as ConflationTypesGroups.json
def createUiJSON(groups, options):
    try:
        uiGroupsArray = []
        configMembers = [{'key': key, 'member': member} for key, member in options.items()]
        for conflateType, conflateConfig in groups.items():
            members = []
            typeMembers = conflateConfig['members']
            for flag in typeMembers.keys():
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

            uiGroupsArray.append({ 'name': conflateType, 'members': members  })
    except:
        print 'failed to load %s' % fn2
    finally:
        return json.dumps(uiGroupsArray)

open(headerFn, 'w').write(createUiJSON(uiGroups, configOptions))