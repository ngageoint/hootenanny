#!/usr/bin/python

"""
Creates a JSON file of configuration options used by UI
"""

import sys
import json
import os

if (len(sys.argv) != 4):
    print "Usage:"
    print sys.argv[0] + " (config file) (output file)"
    sys.exit(-1)

fn = sys.argv[1]
parent = sys.argv[2]
headerFn = sys.argv[3]

uiGroups = json.loads("".join(open(fn).readlines()))
configOptions = json.loads("".join(open(parent).readlines()))

 # merges matches between ConfigOptions.json and conflationTypesMap.json
 # on command key back into conflationTypesMap. Outputs as ConflationTypesGroups.json
def createUiJSON(groups, options):
    try:
        configMembers = [{'key': key, 'member': member} for key, member in options.items()]
        for conflateType, conflateConfig in groups.items():
            members = []
            for index, member in enumerate(conflateConfig['members']):
                match = next((m['key'] for m in configMembers if m['member']['key'] == member['key']), None)
                if match != None:
                    member.update(configOptions[match])
                    member['id'] = match
                    del member['key']
                    # add logic to get default for token if like ${}
                    # handle edge case for road engine
                    # add some logic to get input from type...
                    members.append(member)

            groups[conflateType] = members
    except:
        print('failed to load %s' % fn2)
    finally:
        return json.dumps(groups)

open(headerFn, 'w').write(createUiJSON(uiGroups, configOptions))