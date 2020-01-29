#!/usr/bin/python

"""
Creates a JSON file of available configuration options.
"""

import re
import sys
import json
import pprint
import textwrap

if (len(sys.argv) != 3):
    print "Usage:"
    print sys.argv[0] + " (config file) (output file)"
    sys.exit(-1)

fn = sys.argv[1]
headerFn = sys.argv[2]

def escapeString(s):
    # return s.replace("\n", "\\n").replace("\"", "\\\"");
    return s.replace("\\", "\\\\").replace("\"", "\\\"");

def stripDefaultValue(s):
    m = re.search("\\* Default Value: `(.*)`", s)
    if m:
        return m.group(1)
    else:
        raise Exception("Improperly formatted default value: " + s)

def stripDouble(prefix, s):
    m = re.search("\\* " + prefix + ": `(-?\\d+\\.?\\d*)`", s)
    if m:
        return float(m.group(1))
    else:
        raise Exception("Improperly-formatted floating-point value for " + prefix + ": " + s)

def stripInt(prefix, s):
    m = re.search("\\* " + prefix + ": `(-?\\d+)`", s)
    if m:
        return int(m.group(1))
    else:
        raise Exception("Improperly-formatted integer value for " + prefix + ": " + s)

def loadAsciiDoc(fn):
    result = {}
    section = ''
    element = {}
    inBody = False
    inList = False
    lines = open(fn).readlines();
    ln = 0
    for l in lines:
        ln = ln + 1
        # remove trailing white space
        ls = l.rstrip();

        # comment
        if (ls.strip().startswith("//")):
            pass
        # New section
        elif (ls.startswith("=== ")):
            section = ls.replace("=== ", "")
            element = {}
            result[section] = element
            inBody = False
            inList = False
        elif (ls.startswith("* Data Type: ")):
            element['type'] = ls.replace("* Data Type: ", "");
            inBody = False
        elif (l.startswith("* Default Value:")):
            if (element['type'] == 'list'):
                element['default'] = []
                inList = True
            elif (element['type'] == 'bool'):
                d = stripDefaultValue(l)
                if d == 'true': element['default'] = True
                elif d == 'false': element['default'] = False
                else: raise Exception("Unexpected bool default value in " + section)
                inBody = True
                inList = False
            else:
                element['default'] = stripDefaultValue(l)
                inBody = True
                inList = False
        elif (ls.startswith("* Minimum: ")):
            if (element['type'] == 'int'):
                element['minimum'] = stripInt("Minimum", ls)
            elif (element['type'] == 'double'):
                element['minimum'] = stripDouble("Minimum", ls)
            else:
                raise Exception("Minimum value specified for unsupported type " + element['type'] +
                    " in " + section)
            if ( 'maximum' in element):
                if (element['maximum'] < element['minimum']):
                    raise Exception("Min <= Max bounds relationship not valid in " + section +
                        "; min = " + str(element['minimum']) + ", max = " + str(element['maximum']))
                elif ('default' in element):
                    default = 0;
                    if ( element['type'] == 'int' ):
                        default = int(element['default'])
                    else:
                        default = float(element['default'])
                    if ((default < element['minimum'])
                        or (default > element['maximum'])):
                        raise Exception("Invalid relationship between min/default/max in "
                            + section + ": min=" + str(element['minimum']) + ", default="
                            + str(default) + ", max=" + str(element['maximum']))
        elif (ls.startswith("* Maximum: ")):
            if (element['type'] == 'int'):
                element['maximum'] = stripInt("Maximum", ls)
            elif (element['type'] == 'double'):
                element['maximum'] = stripDouble("Maximum", ls)
            else:
                raise Exception("Maximum value specified for unsupported type " + element['type'] +
                    " in " + section)
            if ( 'minimum' in element):
                if (element['maximum'] < element['minimum']):
                    raise Exception("Min <= Max bounds relationship not valid in " + section +
                        "; min = " + str(element['minimum']) + ", max = " + str(element['maximum']))
                elif ('default' in element):
                    default = 0;
                    if ( element['type'] == 'int' ):
                        default = int(element['default'])
                    else:
                        default = float(element['default'])
                    if ((default < element['minimum']) or
                        (default > element['maximum'])):
                        raise Exception("Invalid relationship between min/default/max in " + section +
                            ": min=" + str(element['minimum']) + ", default=" + str(default) +
                            ", max=" + str(element['maximum']))
        elif inList:
            if ls.startswith('** '):
                element['default'].append(ls.split('`')[1]);
            elif ls == '':
                inList = False
                inBody = True
            else:
                raise Exception("Expected a blank line after the default list. In: " + section)
        elif inBody:
            if 'description' not in element:
                if (l != ''):
                    element['description'] = l
            else:
                element['description'] = element['description'] + l
        elif ls == '':
            pass
        else:
            raise Exception("unexpected line (%d) in: %s line: %s" % (ln, section, l))

    for k, v in result.iteritems():
        v['description'] = v['description'].strip();

    return result

if fn.endswith(".asciidoc"):
    c = loadAsciiDoc(fn)
else:
    c = json.loads("".join(open(fn).readlines()))

def toCamelCase(s):
    l = list(s)
    l[0] = l[0].upper()
    for i in range(1, len(l)):
        if l[i - 1] == '.':
            l[i] = l[i].upper()

    s = "".join(l)
    return s.replace('.', '')

def toComment(s):
    lines = s.split('\n')
    l = []
    for i in lines:
        # keep empty lines
        if i.strip() != '':
            l = l + textwrap.wrap(i, 110)
        else:
            l.append(i)

    return " ".join(l)

def createJSON(c):
    result = "{"

    keys = list(c.keys())
    keys.sort()
    remaining = len(keys)
    for k in keys:
        v = c[k]
        valueString = v['default']
        if v['type'] == 'bool':
            if valueString:
                valueString = 'true'
            else:
                valueString = 'false'
        if v['type'] == 'list':
            valueString = ';'.join(valueString)

        cc = toCamelCase(k)

        result += """ "%s":{"key":"%s","type":"%s","default":"%s" """ % (cc,k,v['type'],escapeString(valueString))

        # If there's are both max/min values for this value, expose those
        if ( ('minimum' in v) and ('maximum') in v ):
            result += ""","min":"%s","max":"%s" """ % (str(v['minimum']),str(v['maximum']))

        result += ""","description":"%s" }""" % (escapeString(toComment(v['description'])))

        remaining -= 1
        if remaining:
            result += ","

        result += "\n"

    result += "}"

    return result

finalJSON = createJSON(c)
#open(headerFn, 'w').write(createJSON(c))

open(headerFn, 'w').write(finalJSON)
