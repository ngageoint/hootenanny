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
    #print "\tfileName: " + fileName  # debug output
    for l in dates:
        #print "\tl: " + l  # debug output
        years.add(l[0:4])
    # If it isn't in git then just give it the current year as the copyright.
    if len(years) == 0:
        years = [str(date.today().year)]
    return ", ".join(sorted(years))

# Copyright years are pulled from existing copyright headers AND they are determined separately from
# the git logs. Sometimes the copyright years in existing copyright headers are not reflected in the
# git logs. The goal here is not lose/replace those existing years but to combine them with the years from
# the git logs. In order to combine them they need to be sorted and have duplicates removed/ignored.
def sortDedupeYears(copyrightYears):
    # Declare return var.
    rtnCopyrightYears = ""
    
    # Remove all whitespace.
    copyrightYears = copyrightYears.replace(" ", "")
    
    # Split copyrightYears into a list.
    allCopyrightYears = copyrightYears.split(",")
    
    # Sort the list into another list.
    allCopyrightYearsSorted = sorted(allCopyrightYears, key=int)
    
    # Help for determining initial loop iteration.
    firstIteration = True
       
    # Loop through the sorted list while not allowing dupes to be concatenated to the return string.
    for copyrightYear in allCopyrightYearsSorted:
        if copyrightYear != "" and copyrightYear not in rtnCopyrightYears:
            if firstIteration:
                rtnCopyrightYears = copyrightYear
                firstIteration = False
            else:
                rtnCopyrightYears = rtnCopyrightYears + ", " + copyrightYear
        
    return rtnCopyrightYears

def run(args):
    p = Popen(args, stdout=PIPE)
    output = p.stdout.read()
    return output

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
    
#print "updateMode  = "+ str(options.updateMode)

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
    existingCopyrightYears = "" # New variable as of 2-2-2017
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
            elif "DigitalGlobe" in line: # New condition. Purpose: extract existing copyright years. (By Dave Emmith, 2-2-2017)
                # Set the pattern to get the years from ...  * @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com)
                p = re.compile("\(C\)\s*(.*?)\s*DigitalGlobe")
                # Search 'line' for the pattern.
                m = p.search(line)
                # If there is a match then assign it to existingCopyrightYears
                if m:
                    existingCopyrightYears = m.group(1)
                    #print "existingCopyrightYears: " + existingCopyrightYears
        lineCount += 1
        oldHeaderBlock += line
      else:
        #print "Augmenting line!"
        lowerNewFileStr += line
        lineCount += 1
        
    # Determine copyright years by examining git log.
    # Set the condition for whether or not the copyright header needs updating.
    copyrightYears = findCopyrightYears(options.fileName)
    updateNeeded = False
    #print "\tcopyrightYears: " + copyrightYears
    if (copyrightYears == existingCopyrightYears):
        #print "\tcopyrightYears match existingCopyrightYears"
        copyrights.append(" * @copyright Copyright (C) " + copyrightYears + " DigitalGlobe (http://www.digitalglobe.com/)")
    else:
        #print "\tcopyrightYears do NOT match existingCopyrightYears"
        if existingCopyrightYears != "":
            # There are existing copyright years, we need to find out if the years identified in the git logs
            # are already in the existing copyright years.
            if copyrightYears in existingCopyrightYears: 
                #print "\tcopyrightYears are already included in existingCopyrightYears" # No update needed
                a=1 # Meaningless statement - needed to keep this construct 'legal' (commenting out the above print statement necessitated this)
            else:
                # The years from the git logs are NOT in the existing copyright years, so combine the two strings of
                # years and run them through the sorting and deduping process (ignores any overlap and sorts the years).
                combinedCopyrightYears = sortDedupeYears(existingCopyrightYears + ", " + copyrightYears)
                #print "\tcombinedCopyrightYears: " + combinedCopyrightYears
                copyrights.append(" * @copyright Copyright (C) " + combinedCopyrightYears + " DigitalGlobe (http://www.digitalglobe.com/)")
                updateNeeded = True
        else:
            copyrights.append(" * @copyright Copyright (C) " + copyrightYears + " DigitalGlobe (http://www.digitalglobe.com/)")
            updateNeeded = True
    
    # Assemble the header lines.
    lines = copyrightHeaderStr.splitlines()
    endLine = lines.pop()
    lines = lines + copyrights + [endLine]
    
    # Create a new header that includes all necessary copyrights.
    copyrightHeaderStr = "\n".join(lines) + "\n"
    
    #print "\n\nfirstLineStr =", firstLineStr
    #print "\n\nlowerNewFileStr:\n----------\n"+lowerNewFileStr+"-----------\n"
    
    # C) If an existing header was never found, then add one at the top of the file.
    newFileStr = firstLineStr + \
      copyrightHeaderStr + \
      lowerNewFileStr
    
    #print "Checking copyright header: " + options.fileName
    
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
            #print "\tUpdating copyright header --> " + options.fileName  # Debug version (with tab)
            #print "\tcopyrightHeaderStr:\n" + copyrightHeaderStr + "\n"
            #print "\n\nnewFileStr:\n----------\n"+newFileStr+"-----------\n"
            open(options.fileName, 'w').write(newFileStr)
            exitCode = 0
        elif not options.updateMode: # update-mode = 0 (False) - note files that need a new copyright header (needs updating)
            print "Needs copyright header updated --> <" + options.fileName + ">"  # Production version (tab-less)
            #print "\tNeeds copyright header updated --> " + options.fileName  # Debug version (with tab)
            # Debug output - see what the old header looks like vs. what the new header would look like.
            #print "\toldHeaderBlock:\n" + oldHeaderBlock + "\n"
            #print "\tcopyrightHeaderStr:\n" + copyrightHeaderStr + "\n"
            exitCode = 1
except Exception as e:
    exMsg = str(e)
    print "Error occurred in UpdateCopyrightHeader.py."
    print "\tError message --> <" + exMsg + ">"
    exitCode = 2
finally:
    #print "Exit code from Python: " + str(exitCode)
    exit(exitCode)
