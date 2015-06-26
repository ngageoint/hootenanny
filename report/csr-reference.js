/**
 * This is a script used by the Plugin type StatsComposer.
 * It specifically is a template for the composition of stats associated with the
 * StatsCmd object. 
 * 
 * Rules:
 * - exports.textBody -- main body of document
 *    + accepts 4 args:
 *        input1Stats - input 1 stats
 *        input2Stats - input 2 stats
 *        outputStats - output stats
 *        histTables  - a mapping of tables for the histograms denoted by the token group key
 *           (e.g. histTables["countTokens1"] = "<table text goes here for the histogram identified>"
 * - Histogram tables
 *    + These are complex messy tables that should not be entered by the JS user because of the high
 *      risk of messing something up, the high repetitiveness, etc.  The C++ code will also validate
 *      that the statistic(s) referred to by the tokens are actually available.
 *    + To create: (1) setup the list of tokens for the stats that will be included in the table
 *        e.g.
 *        exports.tagCountTokens2 = [ 
 *          "Translated Populated Tag Count",
 *          "Translated Default Tag Count",
 *          "Translated Null Tag Count"
 *        ];
 *      (2) Tell the textBody method where the table should be inserted using the histTables map
 *        e.g.--snippet from below--
 *        plot '-' using 2:xtic(1) ti col, '' u 2 ti col, '' u 2 ti col\n"+
 * --->   histTables["tagCountTokens2"]+"\
 *        ------\n\
 *****************************************************************************************************
 * - Testing commands:
 *   + TO ASCIIDOC
 *     hoot conflate -D writer.include.debug=true -D stats.class="hoot::ScriptStatsComposer" -D stats.script=./report/csr-default.js -D stats.format=pdf -D stats.output=/tmp/test2 ./test-files/ToyTestA.osm ./test-files/ToyTestB.osm /tmp/output1.osm --stats
 *   + TO PDF
 *     stats.format=pdf
 *   + TO HTML
 *     stats.format=html
 *****************************************************************************************************
 */

///////////////////////////////////////////////////////////////////
// GLOBAL SETTINGS AND FUNCTIONS
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// MAIN TEXT BODY 
///////////////////////////////////////////////////////////////////

exports.textBody = function(input1Stats, input2Stats, outputStats, histTables, stats4, dataTables, extra)
{

var intro = "\n\
- Schema: " + extra["Translation Schema"] + "\n\n\
.*Conflation Type*\n\n\
- Type: " + extra["Conflation Type"] + "\n\n\
.*Conflation Match Thresholds*\n\n\
- Building: " + extra["Building Match Threshold"] + "\n\n\
- Highway: " + extra["Highway Match Threshold"] + "\n\n\
- POI: " + extra["POI Match Threshold"] + "\n\n\
.*Conflation Miss Thresholds*\n\n\
- Building: " + extra["Building Miss Threshold"] + "\n\n\
- Highway: " + extra["Highway Miss Threshold"] + "\n\n\
- POI: " + extra["POI Miss Threshold"] + "\n\n\
.*Conflation Review Thresholds*\n\n\
- Building: " + extra["Building Review Threshold"] + "\n\n\
- Highway: " + extra["Highway Review Threshold"] + "\n\n\
- POI: " + extra["POI Review Threshold"] + "\n\n\
.*Conflation Search Radii*\n\n\
- Highway: " + extra["Highway Search Radius"] + "\n\n\
- All Other Features: " + extra["All Other Search Radius"] + "\n\n\
.*Notes*\n\n\
- In the Hootenanny command replace any occurrences of '!semi!' with ';' before testing.\n\n\
<<<\n"

//////////////////////////////////////////////////
// input 1 stats
//////////////////////////////////////////////////

var input1 = "\n\
== Statistics for Input 1\n\
\n\
These are all of the statistics associated with the source Input 1.\n\
\n";

for (var key in input1Stats) {
  if (input1Stats.hasOwnProperty(key)) {
    input1 += key + " -> " + input1Stats[key] + "\n\n";
  }
}

input1 += "<<<\n";

//////////////////////////////////////////////////
// input 2 stats
//////////////////////////////////////////////////

var input2 = "\n\
== Statistics for Input 2\n\
\n\
These are all of the statistics associated with the source Input 2.\n\
\n";

for (var key in input2Stats) {
  if (input2Stats.hasOwnProperty(key)) {
    input2 += key + " -> " + input2Stats[key] + "\n\n";
  }
}

input2 += "<<<\n";

//////////////////////////////////////////////////
// output stats
//////////////////////////////////////////////////

var output = "\n\
== Statistics for the Conflated Output\n\
\n\
These are all of the statistics associated with the Conflated Output.\n\
\n";

for (var key in outputStats) {
  if (outputStats.hasOwnProperty(key)) {
    output += key + " -> " + outputStats[key] + "\n\n";
  }
}

output += "<<<\n";

//////////////////////////////////////////////////
// general stats
//////////////////////////////////////////////////

var general = "\n\
== General Statistics for the Conflation Job\n\
\n\
These are all of the general statistics associated with the conflation job.\n\
\n";

for (var key in stats4) {
  if (stats4.hasOwnProperty(key)) {
    general += key + " -> " + stats4[key] + "\n\n";
  }
}

general += "<<<\n";


////////////////////////////////////////////////
// build the string using \n separator
////////////////////////////////////////////////

  var doc = intro+"\n\n"+
    input1+"\n\n"+ 
    input2+"\n\n"+ 
    output+"\n\n"+ 
    general+"\n\n"+ 
    "\n";

  return doc;
}


///////////////////////////////////////////////
// input filter processing data
///////////////////////////////////////////////

// COUNT FEATURES 

exports.countTokens1 = [
  "Node Count;Nodes",
  "Way Count;Ways",
  "Relation Count;Relations",
  "Total Feature Tags;Tags",
];

exports.countTokens2 = [
  "Building Count;Buildings",
  "Highway Count;Highways",
  "POI Count;POIs"
];

exports.uniqueTokens = [
  "Unique Names;All Names",
  "Building Unique Name Count;Building Names",
  "Highway Unique Name Count;Highway Names"
];

exports.poiTokens = [
  "POI Count;POIs",
  "Conflated POIs;Conflated POIs"
];

exports.countTokens3 = [
  "Total Conflated Feature;Conflated Features",
  "Total Unmatched Features;Unmatched Features",
  "Conflated POIs;Conflated POIs",
  "Conflated Highways;Conflated Highways",
  "Conflated Buildings;Conflated Buildings"
];

// PERCENTAGE FEATURES 

exports.percentageTokens1 = [
  "Percentage of Total Features Conflated;Conflated Features",
  "Percentage of Total Features Unmatched;Unmatched Features"
];

// DISTANCE FEATURES 

exports.distanceTokens1 = [
  "Meters of Linear Features;Linear Features",
  "Meters of Highway;Highways",
  "Meters of Highway Processed by Conflation;Conflated Highways"
];

// AREA FEATURES 

exports.areaTokens1 = [
  "Meters Squared of Area Features;Area Features",
  "Meters Squared of Buildings;Buildings",
  "Meters Squared of Buildings Processed by Conflation;Conflated Buildings"
];

// TAGS COUNT FEATURES

exports.tagCountTokens1 = [
  "Total Feature Tags;All Tags",
  "Total Feature Information Tags;Information Tags",
  "Total Feature Metadata Tags;Metadata Tags"
];

exports.tagCountTokens2 = [ 
  "Translated Populated Tag Count;Translated Populated Tags",
  "Translated Default Tag Count;Translated Default Tags",
  "Translated Null Tag Count;Translated Null Tags"
];

// TAG PERCENTAGE FEATURES

exports.tagPercentageTokens1 = [
  "Translated Populated Tag Percent;Translated Populated Tags",
  "Building Translated Populated Tag Percent;Translated Populated Building Tags",
  "Highway Translated Populated Tag Percent;Translated Populated Highway Tags",
  "POI Translated Populated Tag Percent;Translated Populated POI Tags"
];


///////////////////////////////////////////////////////////////////
//  BAR CHARTS (single bar chart representing group of features)
///////////////////////////////////////////////////////////////////

//  BYTES READ AND WRITTEN 

exports.bytesTokens1 = [
  "Bytes Read",
  "Bytes Written"
];

///////////////////////////////////////////////////////////////////
//  MATCH PIE CHARTS
///////////////////////////////////////////////////////////////////

// MATCHES 

exports.matchTokens1 = [
  "Number of Matches Found",
  "Number of Matches Before Whole Groups",
  "Number of Optimized Matches"
];

// MATCH RATES

exports.matchRateTokens1 = [
  "Number of Matches Found per Second",
  "Number of Matches Optimized per Second"
];


///////////////////////////////////////////////////////////////////
//  MERGE PIE CHARTS
///////////////////////////////////////////////////////////////////

exports.mergeRateTokens1 = [
  "Mergers Applied per Second"
];


///////////////////////////////////////////////////////////////////
//  TIMING PIE CHARTS
///////////////////////////////////////////////////////////////////

exports.timingTokens1 = [
  "Read Inputs Time (sec)",
  "Apply Named Ops Time (sec)",
  "Old Road Conflation Time (sec)",
  "Apply Pre Ops Time (sec)",
  "Project to Planar Time (sec)",
  "Find Matches Time (sec)",
  "Optimize Matches Time (sec)",
  "Create Mergers Time (sec)",
  "Apply Mergers Time (sec)",
  "Apply Post Ops Time (sec)",
  "Conflation Time (sec)",
  "Project to WGS84 Time (sec)",
  "Write Output Time (sec)"
];

