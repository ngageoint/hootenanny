#!/usr/bin/python

# Parse a MUVD XML file into a schema
import os,sys,argparse,gzip

# This is where the common functions live
from hootLibrary import *

from xml.dom import minidom

###########
# Main Starts Here
#
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process XML Schema file and build a schema')
    parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
    parser.add_argument('--attributecsv', help='Dump out attributes as a CSV file',action='store_true')
    parser.add_argument('--attrlist', help='Dump out a list of attributes',action='store_true')
    parser.add_argument('--dumpenum', help='Dump out the enumerated attributes, one file per FCODE into a directory called enum',action='store_true')
    parser.add_argument('--fcodeattrlist', help='Dump out a list of FCODE attributes',action='store_true')
    parser.add_argument('--fcodelist', help='Dump out a list of FCODEs',action='store_true')
    parser.add_argument('--fieldvalues', help='Dump out the schema as a JSON object for the Translation Assistant',action='store_true')
    parser.add_argument('--fromenglish', help='Dump out From Englist translation rules',action='store_true')
    parser.add_argument('--fcodeschema', help='Dump out a list of fcodes in the internal OSM schema format',action='store_true')
    parser.add_argument('--nocodedvalues', help='Generate a Thematic Schema with no coded values',action='store_true')
    parser.add_argument('--numrules', help='Dump out number rules',action='store_true')
    parser.add_argument('--rules', help='Dump out one2one rules',action='store_true')
    parser.add_argument('--thematic', help='Generate a Thematic Schema',action='store_true')
    parser.add_argument('--toenglish', help='Dump out To English translation rules',action='store_true')
    parser.add_argument('--txtrules', help='Dump out text rules',action='store_true')
    parser.add_argument('xmlFile', help='The XML Schema file',action='store')

    args = parser.parse_args()

    # Apparently, reading to a string and then parsing is quicker
    if args.xmlFile.endswith(".gz"):
        infile = gzip.open(args.xmlFile, 'rb')
    else:
        infile = open(args.xmlFile, 'rb')

    content = infile.read()

    xmlDoc = minidom.parseString(content)

    schema = {}
    thematicSchema = {}
    thematicEnumSchema = {}

    print(xmlDoc)

    #schema = readFeatures(xmlDoc,funcList)