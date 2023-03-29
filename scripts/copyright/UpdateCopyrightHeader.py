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

def printDebug(output, debugMode):
    if debugMode:
        print(output)

# Given a file determine first and last years it was changed in git.
def findCopyrightYears(fileName, debugMode):
    dates = run(["git", "log", "--date=short", "--format=format:%ad", fileName]).split()
    years = []
    printDebug("\tfileName: " + fileName, debugMode)
    for l in dates:
        printDebug("\tl: " + l, debugMode)
        years.append(eval(l[0:4]))
    years.sort()
    length = len(years)
    # If it isn't in git then just give it the current year as the copyright.
    if length == 0:
        return [date.today().year]
    elif length == 1 or years[0] == years[length - 1]:
        return [years[0]]
    else:
        return [years[0], years[length - 1]]

def compareCopyrightYears(gitCopyrightYears, existingCopyrightYears):
    gitLen = len(gitCopyrightYears)
    fileLen = len(existingCopyrightYears)
    if gitLen < 1 or fileLen < 1:
        return False
    elif gitLen > 2 or fileLen > 2:
        return False
    elif gitLen != fileLen:
        # Check the year values
        return gitLen == 1 and existingCopyrightYears[0] <= gitCopyrightYears[0] and existingCopyrightYears[1] >= gitCopyrightYears[0]
    else:
        # Arrays are the same size, compare the values
        if gitLen == 1:
            return gitCopyrightYears[0] == existingCopyrightYears[0]
        else:
            return gitCopyrightYears[0] >= existingCopyrightYears[0] and gitCopyrightYears[1] <= existingCopyrightYears[1]

def combineYears(years1, years2):
    combined = years1 + years2
    combined.sort()
    length = len(combined)
    if length == 0:
        return []
    elif length == 1:
        return combined
    elif length == 2 and combined[0] == combined[1]:
        return [combined[0]]
    elif combined[0] == combined[length - 1]:
        return [combined[0]]
    else:
        return [combined[0], combined[length - 1]]

def getYearArray(copyrightYearString):
    # split on spaces, commas, and dashes
    allYears = re.split(", *| ?- ?| +", copyrightYearString)
    intYears = [eval(i) for i in allYears]
    intYears.sort()
    length = len(intYears)

    if length == 0:
        return []
    elif length == 1:
        return intYears
    elif length == 2:
        # Don't put duplicates in an array i.e. [2023, 2023]
        if intYears[0] == intYears[1]:
            return [intYears[0]]
        else:
            return intYears
    else:
        # Don't put duplicates in an array i.e. [2023, 2023]
        if intYears[0] == intYears[length - 1]:
            return [intYears[0]]
        else:
            return [intYears[0], intYears[length - 1]]

def yearsToString(copyrightYears):
    length = len(copyrightYears)
    if length == 0:
        return ""
    elif length == 1 or copyrightYears[0] == copyrightYears[length - 1]:
        return str(copyrightYears[0])
    else:
        return "" + str(copyrightYears[0]) + "-" + str(copyrightYears[length - 1])


def run(args):
    p = Popen(args, stdout=PIPE)
    output = p.stdout.read()
    return output

def runcode(args):
    p = Popen(args, stderr=PIPE)
    p.communicate()
    code = p.returncode
    return code

#
# Read in the command-line arguments
#

usageHelp = r"""UpdateCopyrightHeader.py [OPTIONS]

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
        --copyright-header=$PWD/Copyright.txt --file={} --update-mode=0 \;

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
  help="File containing the copyright header to be used." )

clp.add_option(
  "--file", dest="fileName", type="string", default="",
  help="File to have the copyright header replaced in." )

clp.add_option(
  "--script-mode", dest="scriptMode", action="store_true", default=False,
  help="Enable script mode. This prevents the addition of c style comment " +
       "markers at the begining and end of the copyright notice" )

clp.add_option("--update-mode", dest="updateMode", type="int", default=0, 
    help="1 = Enable update mode which means the copyrights will be updated " +
        "otherwise, 0 = the copyrights will only be checked, not updated")

clp.add_option("--debug", dest="debugMode", action="store_true", default=False,
    help="Enable debug mode. Print extra output for debugging copyright")

(options, args) = clp.parse_args()

exitCode = 0

# =========================
# Check/adjust the input arguments
# =========================

if not options.copyrightHeader:
  raise Exception("Error, must set --copyright-header")
  exitCode = 2

if not options.fileName:
  raise Exception("Error, must set --file")
  exitCode = 2
  
#if not options.updateMode:
if (options.updateMode!=0 and options.updateMode!=1):
    raise Exception("Error, must set --update-mode (0 or 1)")
    exitCode = 2
    
printDebug("updateMode  = "+ str(options.updateMode), options.debugMode)

if options.scriptMode:
  commentBegin = ""
  commentEnd   = ""
else:
  commentBegin = "/*" + os.linesep
  commentEnd   = " */" + os.linesep


# =========================
# Executable code
# =========================

try:
    # Check if the file is even tracked in git
    retcode = runcode(["git", "ls-files", "--error-unmatch", options.fileName])
    if retcode != 0:
        exit(0)
    # A) Read in the standard copyright header
    copyrightHeaderStr = open(options.copyrightHeader, 'r').read()

    # B) Read in the given file line by line looking to replace the copyright header
    reFirstHeaderLine = re.compile(r"/\\*.*")
    reLastHeaderLine = re.compile(r".*\\*/.*")

    fileLines = open(options.fileName, 'r').readlines()

    copyrights = []

    # See if the first line is #!
    printDebug("fileLines[0] = '" + fileLines[0] + "'", options.debugMode)
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
    existingCopyrightYears = [] # New variable as of 2-2-2017
    for line in fileLines:
      printDebug("line: '" + line + "'", options.debugMode)
      if firstLineIsSheBang and not foundSheBang:
        printDebug("Found #!", options.debugMode)
        firstLineStr = line
        foundSheBang = True
        lineCount = 0
      elif lineCount == 0 and not doneWithHeader and line.startswith("/*") and not inHeaderBlock:
        printDebug("Found first @HEADER!", options.debugMode)
        inHeaderBlock = True
        oldHeaderBlock += line
      elif "*/" in line and inHeaderBlock:
        printDebug("Found last @HEADER!", options.debugMode)
        inHeaderBlock = False
        doneWithHeader = True
        oldHeaderBlock += line
      elif inHeaderBlock:
        printDebug("Skipping line in existing header block!", options.debugMode)
        if line.startswith(" * @copyright"):
            if "Maxar" not in line:
                copyrights.append(line.rstrip())
            elif "Maxar" in line: # New condition. Purpose: extract existing copyright years. (By Dave Emmith, 2-2-2017)
                # Set the pattern to get the years from ...  * @copyright Copyright (C) 2012, 2013 Maxar (http://www.maxar.com)
                p = re.compile("\(C\)\s*(.*?)\s*Maxar")
                # Search 'line' for the pattern.
                m = p.search(line)
                # If there is a match then assign it to existingCopyrightYears
                if m:
                    existingCopyrightYears = getYearArray(m.group(1))
                    printDebug("existingCopyrightYears: " + yearsToString(existingCopyrightYears), options.debugMode)
        lineCount += 1
        oldHeaderBlock += line
      else:
        printDebug("Augmenting line!", options.debugMode)
        lowerNewFileStr += line
        lineCount += 1


    # Determine copyright years by examining git log.
    # Set the condition for whether or not the copyright header needs updating.
    copyrightYears = findCopyrightYears(options.fileName, options.debugMode)
    updateNeeded = False
    printDebug("\t        copyrightYears: " + yearsToString(copyrightYears), options.debugMode)
    printDebug("\texistingCopyrightYears: " + yearsToString(existingCopyrightYears), options.debugMode)
    if compareCopyrightYears(copyrightYears, existingCopyrightYears):
        printDebug("\tcopyrightYears match or cover existingCopyrightYears", options.debugMode)
        copyrights.append(" * @copyright Copyright (C) " + yearsToString(copyrightYears) + " Maxar (http://www.maxar.com/)")
    else:
        printDebug("\tcopyrightYears do NOT match or cover existingCopyrightYears", options.debugMode)
        # The years from the git logs are NOT in the existing copyright years, so combine the two strings of
        # years and run them through the sorting and deduping process (ignores any overlap and sorts the years).
        combinedCopyrightYears = combineYears(existingCopyrightYears, copyrightYears)
        printDebug("\tcombinedCopyrightYears: " + yearsToString(combinedCopyrightYears), options.debugMode)
        copyrights.append(" * @copyright Copyright (C) " + yearsToString(combinedCopyrightYears) + " Maxar (http://www.maxar.com/)")
        updateNeeded = True

    # Assemble the header lines.
    lines = copyrightHeaderStr.splitlines()
    endLine = lines.pop()
    lines = lines + copyrights + [endLine]

    # Create a new header that includes all necessary copyrights.
    copyrightHeaderStr = "\n".join(lines) + "\n"

    printDebug("\n\nfirstLineStr =" + firstLineStr, options.debugMode)
    printDebug("\n\nlowerNewFileStr:\n----------\n" + lowerNewFileStr + "-----------\n", options.debugMode)

    # C) If an existing header was never found, then add one at the top of the file.
    newFileStr = firstLineStr + \
      copyrightHeaderStr + \
      lowerNewFileStr

    printDebug("Checking copyright header: " + options.fileName, options.debugMode)

    # D) Write the new file.
    # The *elif* portion of the following condition is being changed because the LicenseTemplate.txt file has a space character
    # at the end of line 8 in that file. Some of our files containing copyright headers DO NOT have a space at that position in
    # their copyright header. That causes a false positive condition to occur when comparing oldHeaderBlock to copyrightHeaderStr.
    # This change will fix that issue.
    if "DO NOT ALTER OR REMOVE COPYRIGHT NOTICES" in oldHeaderBlock:
        print "Skipping header: " + options.fileName
    elif updateNeeded: # oldHeaderBlock != copyrightHeaderStr
        if options.updateMode: # update-mode = 1 (True) - write a new copyright header
            print "Updating copyright header --> <" + options.fileName + ">"  # Production version (tab-less)
            printDebug("\tUpdating copyright header --> " + options.fileName, options.debugMode)
            printDebug("\tcopyrightHeaderStr:\n" + copyrightHeaderStr + "\n", options.debugMode)
            printDebug("\n\nnewFileStr:\n----------\n" + newFileStr + "-----------\n", options.debugMode)
            open(options.fileName, 'w').write(newFileStr)
            exitCode = 0
        elif not options.updateMode: # update-mode = 0 (False) - note files that need a new copyright header (needs updating)
            print "Needs copyright header updated --> <" + options.fileName + ">"  # Production version (tab-less)
            printDebug("\tNeeds copyright header updated --> " + options.fileName, options.debugMode)
            # Debug output - see what the old header looks like vs. what the new header would look like.
            printDebug("\toldHeaderBlock:\n" + oldHeaderBlock + "\n", options.debugMode)
            printDebug("\tcopyrightHeaderStr:\n" + copyrightHeaderStr + "\n", options.debugMode)
            exitCode = 1
except Exception as e:
    exMsg = str(e)
    print "Error occurred in UpdateCopyrightHeader.py."
    print "\tError message --> <" + exMsg + ">"
    exitCode = 2
finally:
    printDebug("Exit code from Python: " + str(exitCode), options.debugMode)
    exit(exitCode)
