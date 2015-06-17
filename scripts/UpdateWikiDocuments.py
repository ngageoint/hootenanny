#!/usr/bin/python

import fpformat
import re
import sys
import requests
from requests.auth import HTTPBasicAuth
import time
import math
import string
import codecs

baseUrl = None
notReferences = {}
references = {}

def urlOpenWithRetry(url, tries, delay=5, backoff=2, username='', password=''):
    """Opens URL using retry with exponential backoff on retry times.

    delay sets the initial delay in seconds, and backoff sets the factor by which
    the delay should lengthen after each failure. backoff must be greater than 1,
    or else it isn't really a backoff. tries must be at least 1, and delay
    greater than 0."""

    if backoff <= 1:
        raise ValueError("backoff must be greater than 1")

    tries = math.floor(tries)
    if tries < 1:
        raise ValueError("tries must be 1 or greater")

    if delay <= 0:
        raise ValueError("delay must be greater than 0")

    mtries, mdelay = tries, delay # make mutable

    while mtries > 0:
        try:
            #print "Attempting to open the following URL (using username/password): ",
            print url
            response = requests.get(url, auth=HTTPBasicAuth(username, password), verify=False)
            if response.ok:
                #print "Successfully opened the URL."
                return response
            else:
                mtries -= 1
                if not mtries:
                    print "Fatal error.  Out of retries.  No exceptions in final try."
                    raise
                print "Unable to open URL:",
                print url
                print "Status Code and Reason: ", response.status_code, " ", response.reason
                print "Tries remaining = ",
                print mtries
                time.sleep(mdelay) # wait...
                mdelay *= backoff  # make future wait longer
        except (requests.exceptions.RequestException):
            mtries -= 1
            if not mtries:
                print "Fatal error.  Out of retries.  Exception follows:"
                raise
            print "Unable to open this URL due to exception:",
            print url
            print "Tries remaining = ",
            print mtries
            time.sleep(mdelay) # wait...
            mdelay *= backoff  # make future wait longer

    return False # Ran out of tries :-(

def safe_str(obj):
    """ return the byte string representation of obj """
    try:
        return str(obj)
    except UnicodeEncodeError:
        # obj is unicode
        return unicode(obj).encode('utf-8')

def writeHtml(url, file, level, username, password):
    # Write out a HTML help document
    output = 0
    divs = 0
    response = urlOpenWithRetry(url, 30, 3, 1.1, username, password)
    for line in response.text.splitlines(True):
        # Remove all the annoying paragraph symbols.
        line = line.replace("&para;", "");
        if '<body>' in line:
            output = 1
            divs = 1
            continue
        if output == 1:
            for i in range(5, 0, -1):
                tmp = '<h' + fpformat.fix(i, 0) + ' '
                replace = '<h' + fpformat.fix(i + level, 0) + ' '
                if tmp in line:
                    line = re.sub(tmp, replace, line)
                tmp = '</h' + fpformat.fix(i, 0) + '>'
                replace = '</h' + fpformat.fix(i + level, 0) + '>'
                if tmp in line:
                    line = re.sub(tmp, replace, line)
            refs = re.findall('<a class="wiki" href="/trac/sa/wiki/([A-Za-z0-9]+)">', line);
            for ref in refs:
                references[ref] = 1
            if '</body>' in line:
                output = 0
            else:
                file.write(line)
    file.write("")

class Section:

    def __init__(self, name, link):
        self.name = name
        self.link = link.replace(' ', '_')
        self.link = link.replace('/', '')
        self.outputFile = link.replace('/', '_')
        self.children = []

    def addChild(self, child):
        self.children.append(child)

    def write(self, file, level, username, password):
        writeHtml(baseUrl + 'wiki/' + self.link + '?format=html', file, level, username, password)
        notReferences[self.link] = 1
        
        if self.name == "Command Descriptions":
            self.writeCommands(file, level, username, password)
        for child in self.children:
            child.write(file, level + 1, username, password);

    def writeCommands(self, file, level, username, password):
        commands = []

        response = urlOpenWithRetry(baseUrl + 'wiki/' + self.link + '?format=txt', 30, 3, 1.1, username, password)
        
        for line in response.text.splitlines(True):
            if ' * ' in line: # Look only for the line items
                words = re.findall(r'[A-Za-z0-9]+', line)
                #check to see if the line has more than one match, if it does, it's probably because it was prepened w/ '[wiki:';words[1] will skip this
                if len(words) > 0:
                    if words[0] == 'wiki':
                        if len(words) > 1:
                            commands.append(words[1])
                        else:
                            commands.append(words[0])
               
##                                if len(words) == 1:
##                                        commands.append(words[0])
##                                else:
##                                        if len(words) > 1:
##                                                commands.append(words[1])
                
#                if len(words) > 0:
#                    commands.append(words[0])

        for command in commands:
            #print command
            notReferences[command] = 1
            writeHtml(baseUrl + 'wiki/' + command, file, level + 1, username, password)

    
                
def updateWiki(username, password, bUrl, pageName, outputFilename):
  global baseUrl, notReferences, references
  baseUrl = bUrl

  urlOpenWithRetry(baseUrl, 30, 3, 1.1, username, password)
  
  saDocs = codecs.open(outputFilename, 'w', 'utf-8')
  references = {}
  notReferences = {}
  
  saDocs.write("<!-- This file is automatically generated. Please don't modify. -->\n");
  
  # Set it to true when we get to the contents
  contents = 0
  
  
  #
  # Get the names of all the sections and put them in a tree
  #
  #print "Requesting contents"
  topLevelSections = []
  ancestry = [None] * 100
  response = urlOpenWithRetry(baseUrl + 'wiki/' + pageName + '?format=txt', 30, 3, 1.1, username, password)
  for line in response.text.splitlines(True):
      if re.match('h.\\. Contents', line) != None:
          contents = 1
          continue
      if contents == 1:
          indent = 0
          spaces = re.findall('\*+', line)
          if len(spaces) > 0:
              indent = len(spaces[0]) - 1
          if indent == 0:
              parent = 0
          match = re.match('\*+ \[\[([\-A-Z\& a-z0-9/]+)(\|(.*))?\]\].*', line)
          #print line
          section = ""
          if match != None:
              #print match
              # We have a name and a wiki link
              link = match.group(1)
              name = match.group(3)
              if name == None:
                  name = link
              if link != None:
                  link = link.replace(" ", "_").replace('/', '')
                  #print link + ":" + name
                  section = Section(name, link)
              if section != "":
                  if indent == 0:
                      parent = section
                      topLevelSections.append(section)
                  elif ancestry[indent - 1] != None:
                      ancestry[indent -1].addChild(section)
                  ancestry[indent] = section
  
  for section in topLevelSections:
      section.write(saDocs, 0, username, password)
  
  for nr in notReferences:
      references[nr] = 0
  
  dum = []
  sortedReferences = []
  for k, v in references.iteritems():
      dum.append(k)
      if v == 1:
          sortedReferences.append(k)
  
  sortedReferences.sort()
  dum.sort()
  ##print "begin dump dum"
  ##print dum
  ##print "end dump dum"
  ##print sortedReferences
  
  if len(sortedReferences) != 0:
      saDocs.write("<h1>References</h1>")
      print
      print "Missing References:"
      for name in sortedReferences:
          writeHtml(baseUrl + 'wiki/' + name, saDocs, 1, username, password)
          print "  " + name
  
  
  saDocs.write("")
