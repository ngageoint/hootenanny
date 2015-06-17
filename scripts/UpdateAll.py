#!/usr/bin/python

import fpformat
import os
import re
import sys
import getpass
from UpdateWikiDocuments import *

noPassword = 0;

#pwfile = os.environ["HOME"] + "/.automaton-passwd"
#if os.path.exists(pwfile):
#    fp = open(pwfile)
#    password = fp.read() + " silly salt"
#    username = "automaton"
#else:
username = raw_input("Please enter your redmine username: ")
password = getpass.getpass()

updateWiki(username, password, "https://insightcloud.digitalglobe.com/redmine/projects/hootenany/", "User_Documentation", "tmp/UserDocumentation.html")
