#!/usr/bin/env python

# Taken from https://github.com/nschloe/trilinos/blob/master/commonTools/refactoring/update-copyright-header.py
# and modified for our purposes.
# Ironically this file doesn't have a copyright header.

from datetime import date
import os
import re
from subprocess import Popen, PIPE
import sys
import traceback

# Given a file determine which years it was changed in git.
def findCopyrightYears(fileName):
    dates = run(["git", "log", "--date=short", "--format=format:%ad", fileName]).split()
    years = set()
    for l in dates:
        years.add(l[0:4])
    # If it isn't in git then just give it the current year as the copyright.
    if len(years) == 0:
        years = [str(date.today().year)]
    return ", ".join(sorted(years))

def run(args):
    p = Popen(args, stdout=PIPE)
    output = p.stdout.read()
    return output


#
# Read in the command-line arguments
#

usageHelp = r"""update-copyright-header.py [OPTIONS]

This script updates the copyright header in a given file.  

It looks for a section that starts with a line "/*" on the first line. Then
looks for a "*/" somewhere after that. The following header formats should be fine:

/*****
 * My copyright
 * more stuff...
 *****/

/*
 * Stuff
 */

/*
stuff
*/

This won't work:

/* copyright */

* Note: Untested. * Probably won't work anymore.
Passing in --script-mode, the copyright 
header will not be inserted into the first line if --script-mode
is given and the first line is '#!'.  In this case, the copyright header will
be added the the second line instead of the first line.

To replace the copyright header for all of the source files for a complete
package do:

  $ cd packages/SOME_PACKAGE
  $ find . -name "*pp" -exec \
      ../../commonTools/refactoring/update-copyright-header.py  \
        --copyright-header=$PWD/Copyright.txt --file={} \;

For example, the file packages/SOME_PACKAGE/Copyright.txt should look like:

  $ cat packages/teuchos/Copyright.txt
    // @HEADER
    // ***********************************************************************
    // 
    //                    Teuchos: Common Tools Package
    //                 Copyright (2004) Sandia Corporation
    //
    // ...
    // 
    // ***********************************************************************
    // @HEADER

See packages/teuchos/Copyright.txt for an example.
"""

from optparse import OptionParser

clp = OptionParser(usage=usageHelp)

clp.add_option(
  "--copyright-header", dest="copyrightHeader", type="string", default="",
  help="File containing the copyright header the be used." )

clp.add_option(
  "--file", dest="fileName", type="string", default="",
  help="File to have the copyright header replaced in." )

clp.add_option(
  "--script-mode", dest="scriptMode", action="store_true", default=False,
  help="Enable script mode. This prevents the addition of c style comment " +
       "markers at the begining and end of the copyright notice" )

(options, args) = clp.parse_args()


#
# Check/adjust the input arguments
#

if not options.copyrightHeader:
  raise Exception("Error, must set --copyrightHeader")

if not options.fileName:
  raise Exception("Error, must set --fileName")

if options.scriptMode:
  commentBegin = ""
  commentEnd   = ""
else:
  commentBegin = "/*" + os.linesep
  commentEnd   = " */" + os.linesep


#
# Executble code
#

# A) Read in the standard copyright header

copyrightHeaderStr = open(options.copyrightHeader, 'r').read()

# B) Read in the given file line by line looking to replace the copyright header

reFirstHeaderLine = re.compile(r"/\\*.*")
reLastHeaderLine = re.compile(r".*\\*/.*")

fileLines = open(options.fileName, 'r').readlines()

copyrights = []

# See if the first line is #!
#print "fileLines[0] = '"+fileLines[0]+"'"
if options.scriptMode and fileLines[0][0:2] == "#!":
  firstLineIsSheBang = True
else:
  firstLineIsSheBang = False

# Look for the header
oldHeaderBlock = ""
foundSheBang = False
inHeaderBlock = False
firstLineStr = ""
lowerNewFileStr = ""
doneWithHeader = False
lineCount = 0
for line in fileLines:
  #print "line: '"+line+"'"
  if firstLineIsSheBang and not foundSheBang:
    #print "Found #!"
    firstLineStr = line
    foundSheBang = True
    lineCount = 0
  elif lineCount == 0 and not doneWithHeader and line.startswith("/*") and not inHeaderBlock:
    #print "Found first @HEADER!"
    inHeaderBlock = True
    oldHeaderBlock += line
  elif "*/" in line and inHeaderBlock:
    #print "Found last @HEADER!"
    inHeaderBlock = False
    doneWithHeader = True
    oldHeaderBlock += line
  elif inHeaderBlock:
    #print "Skipping line in existing header block!"
    if line.startswith(" * @copyright"):
        if "DigitalGlobe" not in line:
            copyrights.append(line.rstrip())
    lineCount += 1
    oldHeaderBlock += line
  else:
    #print "Augmenting line!"
    lowerNewFileStr += line
    lineCount += 1

copyrightYears = findCopyrightYears(options.fileName)
copyrights.append(" * @copyright Copyright (C) " + copyrightYears + " DigitalGlobe (http://www.digitalglobe.com/)")

lines = copyrightHeaderStr.splitlines()
endLine = lines.pop()
lines = lines + copyrights + [endLine]

# Create a new header that includes all necessary copyrights.
copyrightHeaderStr = "\n".join(lines) + "\n"

#print "\n\nfirstLineStr =", firstLineStr
#print "\n\nlowerNewFileStr:\n----------\n"+lowerNewFileStr+"-----------\n"


# C) If an existing header was never found, then add one at the top of the
# file.

newFileStr = firstLineStr + \
  copyrightHeaderStr + \
  lowerNewFileStr

print "Checking copyright header: " + options.fileName

# D) Write the new file
if "DO NOT ALTER OR REMOVE COPYRIGHT NOTICES" in oldHeaderBlock:
    print "Skipping header: " + options.fileName
elif oldHeaderBlock != copyrightHeaderStr:
    print "Rewriting header: " + options.fileName
    #print "\n\nnewFileStr:\n----------\n"+newFileStr+"-----------\n"
    open(options.fileName, 'w').write(newFileStr)

