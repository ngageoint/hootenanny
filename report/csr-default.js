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

var horizontalHistograms = false;

var histogramSetup = "unset key\n" +
  "set style data histogram\n" +
  "set style histogram cluster gap 1\n" +
  "set style fill solid 0.5 border -1\n" +
  "set grid ytics lt 0 lw 1 lc rgb \"#bbbbbb\"\n" +
  "set grid xtics lt 0 lw 1 lc rgb \"#bbbbbb\"\n" +
  "set boxwidth 0.8\n";
  
if (horizontalHistograms) {
  histogramSetup += "set size 0.7,1.0\n" +
    "set xtic rotate by 90 scale 0 right font \"arial,10\"\n" +
    "unset ytics\n" +
    "set y2tics rotate by 20 offset -1,0,0\n";
} else {
  histogramSetup += "set size 1.0,1.0\n" +
    "set xtic rotate by -70 scale 0 font \"arial,10\"\n" +
    "set ytics rotate by 0 offset -1,0,0\n"+
    "set key vertical outside\n";
}

setLabels = function(title, label) {
  var cmds = "";
  if (horizontalHistograms) {
    cmds += "" +
    "unset title\n"+
    "set ylabel \""+title+"\"\n"+
    "set y2label \""+label+"\"\n";
  } else {
    cmds += "" +
    "unset y2label\n"+
    "set title \""+title+"\"\n"+
    "set ylabel \""+label+"\"\n";
  }
  return cmds;
}

maxFeatures = function(inp) {
  return 1.2*Math.max(inp["Total Feature Count"],inp["Total Conflatable Features"],
    inp["Building Count"], inp["Highway Count"], inp["POI Count"]);
}

maxFeatures2 = function(inp1,inp2) {
  return 1.2*Math.max(inp1["Total Feature Count"],inp1["Total Conflatable Features"],
    inp1["Building Count"], inp1["Highway Count"], inp1["POI Count"],
    inp2["Total Feature Count"],inp2["Total Conflatable Features"],
    inp2["Building Count"], inp2["Highway Count"], inp2["POI Count"]);
}

histPost = function(input1, input2) {
  var cmds = "";
  if (horizontalHistograms) {
    cmds = "{sys3:convert -rotate 90 "+input1+" "+input2+"}\n"+
           "{sys3:/bin/cp "+input1+" "+input2+"}\n";
  }
  return cmds;
}

formatTime = function(valueInSecs) {
  var format = "";
  if(isNaN(valueInSecs)) {
    format = "Not applicable";
  } else {
    var integerPart = Math.floor( valueInSecs );
    var fractionalPart = valueInSecs - integerPart;
    var mins = Math.floor(integerPart / 60);
    var secs = integerPart - 60*mins;
    var millisecs = Math.round( fractionalPart * 1000.0 );
    format = mins+"m "+secs+"s "+millisecs+"ms"; 
  }
  return format;
}

/**
 * Decimal adjustment of a number.
 *
 * @param {String}  type  The type of adjustment.
 * @param {Number}  value The number.
 * @param {Integer} exp   The exponent (the 10 logarithm of the adjustment base).
 * @returns {Number} The adjusted value.
 */
function decimalAdjust(type, value, exp) {
  // If the exp is undefined or zero...
  if (typeof exp === 'undefined' || +exp === 0) {
    return Math[type](value);
  }
  value = +value;
  exp = +exp;
  // If the value is not a number or the exp is not an integer...
  if (isNaN(value) || !(typeof exp === 'number' && exp % 1 === 0)) {
    return NaN;
  }
  // Shift
  value = value.toString().split('e');
  value = Math[type](+(value[0] + 'e' + (value[1] ? (+value[1] - exp) : -exp)));
  // Shift back
  value = value.toString().split('e');
  return +(value[0] + 'e' + (value[1] ? (+value[1] + exp) : exp));
}

// Decimal round
if (!Math.round10) {
  Math.round10 = function(value, exp) {
    return decimalAdjust('round', value, exp);
  };
}

roundPercent = function(value) {
  var format = "";
  if(!isNaN(value)) {
    var percent = Math.round(value * 10.0)/10.0;
    format = " "+percent+"% ";
  }
  return format;
}

calcPercent1 = function(numerator, denominator) {
  return calcPercent(numerator, denominator, true);
}

calcPercent = function(numerator, denominator, parenthesisFlag) {
  var format = "";
  if(!isNaN(numerator)) {
    var percent = Math.round(numerator / denominator * 1000.0)/10.0;
    if(parenthesisFlag) {
      format = " ( "+percent+"% )";
    } else {
      format = " "+percent+"% ";
    }
  }
  return format; 
}

cleanUpNans = function(dataTable) {
  var newDataTable = dataTable.replace(/nan/g, "NA");
  return newDataTable;
}

addNAs = function(dataTable, pattern) {
  var n = dataTable.search( pattern );
  var searchPattern = pattern.concat('|0|0|'); 
  var replacePattern = pattern.concat('|NA|NA|'); 
  var newDataTable = dataTable.replace(searchPattern, replacePattern);
  return newDataTable;
}

///////////////////////////////////////////////////////////////////
// MAIN TEXT BODY 
///////////////////////////////////////////////////////////////////

exports.textBody = function(input1Stats, input2Stats, outputStats, histTables, stats4, dataTables, extra)
{
//hoot.log("outputStats = ");
//hoot.log(outputStats);

/*
[mpl, exampleSin.png]\n\
------\n\
figure(figsize=(4,2))\n\
X=arange(0,6,0.1)\n\
Y=sin(X)\n\
plot(X,Y)\n\
------\n";
*/

var summary = "\n\
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
<<<\n\
== Summary\n" +
  "A summary of the most important statistics for the conflation job just executed are presented in the bar graph and table below.  Shown are the total and conflatable number of features for both inputs, the total number of features for the output, and a breakdown of the output features by those that are conflated, marked for review, and unmatched.\n\
\n\
Conflatable features are defined as those that are supported by Hootenanny's matching algorithms. Generally speaking, Hootenanny support types of highways, buildings and POIs.  Other features are not supported by Hootenanny. Note also that conflatables is defined by the current settings of the matching algorithm.  The matching algorithm can be configured by setting different \"match creators\", where each supports different types of features. By default, a full set of match creators is applied, but this could be reduced to a subset of feature types depending on the user's desired product needs.\n\
\n\
The total number of features in the output have no correlation with the two inputs' numbers of features.  There are a few reasons for this: (1) features from the inputs are split in order to perform matching, (2) features are added during specific conflation algorithm steps, and (3) the merge step of the conflation may change the number of features.\n\
\n\
The output features are classified by the following breakdown: (1) conflated - those features that successfully combined from multiple sources and possibly from the same input; (2) unmatched - those features where no matches were found between any other, and possibly from the same source; and (3) reviewable - those features with rankings falling between the conflated and unmatched classifications where manual review by a user is required.\n\
\n\
The bar graph shows the number of features for each category, and the table provides the actual count numbers and percentages where appropriate.  The percentage of conflatables are relative to the total number of features for the respective input.  The percentages for the conflated, reviewable, and unmatched are relative to the total number of features in the output.  Note for now we do not show percentage values of how many features were conflated from the inputs to the output because there is no easy way to calculate and record these values while considering the feature splits and the impact on the performance of Hootenanny's conflation. \n"+
  "\n" + 
  "[gnuplot, summaryChart.png]\n" +
  "------\n" +
  histogramSetup+
//  "set yrange [0:100]\n"+ 
//  "set autoscale ymax\n"+
  "set yrange [0:"+1.2*Math.max(
    input1Stats["Total Feature Count"], input2Stats["Total Feature Count"],
    input1Stats["Total Conflatable Features"], input2Stats["Total Conflatable Features"],
    outputStats["Total Feature Count"], outputStats["Total Conflated Features"],
    outputStats["Total Features Marked for Review"], outputStats["Total Unmatched Features"])+"]\n"+
  "set title \"Summary of Important Statistics\"\n"+
  "set ylabel \"Number of Features\"\n"+
  "unset xtics\n"+
  "set xtic rotate by -70 center offset 1.3,-3.3 scale 0 font \"arial,10\"\n"+
  "set nokey\n"+
  "set size 1,1\n"+
  "set terminal png size 600,450\n"+ 
  "set bmargin at screen 0.3\n"+
//  "set label 1 'Percentage ="+roundPercent(outputStats["Percentage of Total Features Marked for Review"])+"' at graph 0.3,0.97 front font \"Times,9\"\n"+
//  "set label 2 'Percentage ="+roundPercent(outputStats["Percentage of Total Features Conflated"])+"' at graph 0.5,0.97 front font \"Times,9\"\n"+
//  "set label 3 'Percentage ="+roundPercent(outputStats["Percentage of Total Features Unmatched"])+"' at graph 0.7,0.97 front font \"Times,9\"\n"+
  "plot '-' using 2:xtic(1) ti col, '' u 2 ti col\n"+
  "unknownKey\n"+
  "\"Input 1 Total Features\"\t" + input1Stats["Total Feature Count"] + "\n" +
  "\"Input 2 Total Features\"\t" + input2Stats["Total Feature Count"] + "\n" +
  "\"Input 1 Conflatables  \"\t" + input1Stats["Total Conflatable Features"] + "\n" +
  "\"Input 2 Conflatables  \"\t" + input2Stats["Total Conflatable Features"] + "\n" +
  "\"Output Total Features \"\t" + outputStats["Total Feature Count"] + "\n" +
  "\"Conflated Features    \"\t" + outputStats["Total Conflated Features"] + "\n" +
  "\"Reviewable Features  \"\t" + outputStats["Total Features Marked for Review"] + "\n" +
  "\"Unmatched Features   \"\t" + outputStats["Total Unmatched Features"] + "\n" +
  "e\n" +
  "------\n\
[width=\"90%\",options=\"header\"]\n\
|===============================\n\
|  |Count |Percentage\n\
|Input 1 Total Features |"+input1Stats["Total Feature Count"]+" |NA\n\
|Input 2 Total Features |"+input2Stats["Total Feature Count"]+" |NA\n\
|Input 1 Conflatables |"+input1Stats["Total Conflatable Features"]+" |"+roundPercent(input1Stats["Percentage of Total Features Conflatable"])+"\n\
|Input 2 Conflatables |"+input2Stats["Total Conflatable Features"]+" |"+roundPercent(input2Stats["Percentage of Total Features Conflatable"])+"\n\
|Output Total Features |"+outputStats["Total Feature Count"]+" |NA\n\
|Conflated Features |"+outputStats["Total Conflated Features"]+" |"+roundPercent(outputStats["Percentage of Total Features Conflated"])+"\n\
|Reviewable Features |"+outputStats["Total Features Marked for Review"]+" |"+roundPercent(outputStats["Percentage of Total Features Marked for Review"])+"\n\
|Unmatched Features |"+outputStats["Total Unmatched Features"]+" |"+roundPercent(outputStats["Percentage of Total Features Unmatched"])+"\n\
|===============================\n\
<<<\n";


var features = "== Statistical Aspects of the Input and Output Datasets\n" +
  "Statistics for a variety of aspects of the input and output datasets are presented in this section.\n" +
  "\n" +
  "=== Summary of Basic Feature Elements\n" +
  "There are three basic elements of OpenStreetMap's conceptual data model of the physical world. These are nodes (points in space), ways (linear and area features), and relations (a high-level construct that helps coordinate how ways and nodes work together).  To go along with these are tags which can associate with any of the three basic elements.  In this section, a summary of the number of each element and tags are shown in the bar graph.  For each,  a comparison of the number of features for the two inputs and output are shown.\n" +
  "\n" +
  "Note: More information about the OSM Elements is provided at: http://wiki.openstreetmap.org/wiki/Elements\n" +
  "\n" +
  "[gnuplot, featureCountHistogram1.png]\n"+
  "------\n"+
  "reset\n"+
  setLabels("Summary of Basic Feature Elements","Number of Features")+
  histogramSetup+
  histTables["countTokens1"]+
  "------\n"+
  histPost("featureCountHistogram1.png", "featureCountHistogram1r.png")+
  dataTables["countTokens1"]+
  "<<<\n" +
  "=== Summary of Input Features by Type\n" +
  "Some of the key feature types that Hootenanny operates upon are 'buildings', 'highways', and 'points of interest (POIs)'.  These types are defined using the naming conventions that OSM assigns for these attributes.  The buildings type are the area footprints for what the title indicates--buildings.  The highways type is a bit more generic and refers to a collection of linear segments that include road types (highways, streets, dirt roads), cart tracks, and trails.  The POIs are defined for Hootenanny as nodes with a attribute \"poi=yes\".  The POIs are typically derived from an OSM attribute like \"amenity\".  For example, POIs may be a \"place of worship\", \"cafe\", etc.\n\
\n\
A bar graph and table are presented below showing information about the input datasets. The bar graph shows the total features, number of conflatable features, and a breakdown of the features by types for each of the two inputs.  The feature types presented are: buildings, highways, and POIs.  The number of conflatables is a count of features that Hootenanny supports (see definition described previously).  The table shows the specific count values, percentage of conflatables relative to the total features, and percentage of feature type relative to the number of conflatables.\n" +
  "\n" +
  "[gnuplot, inputFeatures.png]\n" +
  "------\n"+
  histogramSetup+
  "set yrange [0:"+maxFeatures2(input1Stats,input2Stats)+"]\n"+
  "set title \"Breakdown of Features by Type for Inputs 1 & 2\"\n"+
  "set ylabel \"Number of Features\"\n"+
  "unset xtics\n"+
  "set xtic rotate by -70 center offset 0.0,-2 scale 0 font \"arial,10\"\n"+
  "set size 1,1\n"+
  "set nokey\n"+
  "set terminal png size 600,450\n"+
  "set bmargin at screen 0.3\n"+
  "plot '-' using 2:xtic(1) ti col, '' u 2 ti col, '' u 2 ti col\n"+
  "\"Input 1\"\n"+
  "\"Total Features\"\t" + input1Stats["Total Feature Count"] + "\n" +
  "\"Conflatables  \"\t" + input1Stats["Total Conflatable Features"] + "\n" +
  "\"Buildings      \"\t" + input1Stats["Building Count"] + "\n" +
  "\"Highways      \"\t" + input1Stats["Highway Count"] + "\n" +
  "\"POIs           \"\t" + input1Stats["POI Count"] + "\n" +
  "e\n" +
  "\"Input 2\"\n"+
  "\"Total Features\"\t" + input2Stats["Total Feature Count"] + "\n" +
  "\"Conflatables  \"\t" + input2Stats["Total Conflatable Features"] + "\n" +
  "\"Buildings      \"\t" + input2Stats["Building Count"] + "\n" +
  "\"Highways      \"\t" + input2Stats["Highway Count"] + "\n" +
  "\"POIs           \"\t" + input2Stats["POI Count"] + "\n" +
  "e\n" +
  "------\n\
[width=\"90%\",options=\"header\"]\n\
|===============================\n\
|  |Input 1 |Input 2\n\
|Total Features |"+input1Stats["Total Feature Count"]+"|"+input2Stats["Total Feature Count"]+"\n\
|Conflatables |"+input1Stats["Total Conflatable Features"]+" ("+roundPercent(input1Stats["Percentage of Total Features Conflatable"])+")|"+input2Stats["Total Conflatable Features"]+" ("+roundPercent(input2Stats["Percentage of Total Features Conflatable"])+")\n\
|Buildings |"+input1Stats["Building Count"]+" ("+calcPercent(input1Stats["Building Count"],input1Stats["Total Conflatable Features"])+")|"+input2Stats["Building Count"]+" ("+calcPercent(input2Stats["Building Count"],input2Stats["Total Conflatable Features"])+")\n\
|Highways |"+input1Stats["Highway Count"]+" ("+calcPercent(input1Stats["Highway Count"],input1Stats["Total Conflatable Features"])+")|"+input2Stats["Highway Count"]+" ("+calcPercent(input2Stats["Highway Count"],input2Stats["Total Conflatable Features"])+")\n\
|POIs |"+input1Stats["POI Count"]+" ("+calcPercent(input1Stats["POI Count"],input1Stats["Total Conflatable Features"])+")|"+input2Stats["POI Count"]+" ("+calcPercent(input2Stats["POI Count"],input2Stats["Total Conflatable Features"])+")\n\
|===============================\n\
<<<\n"+
  "=== Summary of Output Features by Type\n" +
  "A breakdown of the features by type for the output are described in this section.  The bar graph shows statistics for all features, and the breakdown by types of buildings, highways, and POIs.  The statistics shown are the total number of features, number conflated, number marked for review, and number of unmatched features.  The table shows the specific count values and the percentages of the conflated, marked for review, and unmatched relative to the total counts of the respective feature type.\n" +
  "\n" +
  "[gnuplot, conflatedFeature.png]\n" +
  "------\n" +
  histogramSetup+
  //"set autoscale y\n"+
  "set yrange [0:"+1.2*outputStats["Total Feature Count"]+"]\n"+
  "set title \"Breakdown of Features by Type for the Output\"\n"+
  "set ylabel \"Number of Features\"\n"+
  "unset xtics\n"+
  "set xtic rotate by -70 center offset 0.5,-2 scale 0 font \"arial,10\"\n"+
  "set size 1,1\n"+
  "set key\n"+
  "set terminal png size 600,450\n"+
  "set bmargin at screen 0.3\n"+
  "plot '-' using 2:xtic(1) ti col, '' u 2 ti col, '' u 2 ti col, '' u 2 ti col\n"+
  "\"Total Count\"\n"+
  "\"All Features\"\t" + outputStats["Total Feature Count"] + "\n" +
  "\"Buildings   \"\t" + outputStats["Building Count"] + "\n" +
  "\"Highways    \"\t" + outputStats["Highway Count"] + "\n" +
  "\"POIs        \"\t" + outputStats["POI Count"] + "\n" +
  "e\n" +
  "\"Conflated\"\n"+
  "\"All Features\"\t" + outputStats["Total Conflated Features"] + "\n" +
  "\"Buildings   \"\t" + outputStats["Conflated Buildings"] + "\n" +
  "\"Highways    \"\t" + outputStats["Conflated Highways"] + "\n" +
  "\"POIs        \"\t" + outputStats["Conflated POIs"] + "\n" +
  "e\n" +
  "\"Marked Review\"\n"+
  "\"All Features\"\t" + outputStats["Total Features Marked for Review"] + "\n" +
  "\"Buildings   \"\t" + outputStats["Buildings Marked for Review"] + "\n" +
  "\"Highways    \"\t" + outputStats["Highways Marked for Review"] + "\n" +
  "\"POIs        \"\t" + outputStats["POIs Marked for Review"] + "\n" +
  "e\n" +
  "\"Unmatched\"\n"+
  "\"All Features\"\t" + outputStats["Total Unmatched Features"] + "\n" +
  "\"Buildings   \"\t" + outputStats["Unmatched Buildings"] + "\n" +
  "\"Highways    \"\t" + outputStats["Unmatched Highways"] + "\n" +
  "\"POIs        \"\t" + outputStats["Unmatched POIs"] + "\n" +
  "e\n" +
  "------\n\
[width=\"90%\",options=\"header\"]\n\
|===============================\n\
|  |Total Count |Conflated |Marked Review |Unmatched\n\
|All Features |"+outputStats["Total Feature Count"]+" |"+outputStats["Total Conflated Features"]+" ("+Math.round10(outputStats["Percentage of Total Features Conflated"],-1)+"%) |"+outputStats["Total Features Marked for Review"]+" ("+Math.round10(outputStats["Percentage of Total Features Marked for Review"],-1)+"%) |"+outputStats["Total Unmatched Features"]+" ("+Math.round10(outputStats["Percentage of Total Features Unmatched"],-1)+"%)\n\
|Buildings |"+outputStats["Building Count"]+" |"+outputStats["Conflated Buildings"]+" ("+Math.round10(outputStats["Percentage of Buildings Conflated"],-1)+"%) |"+outputStats["Buildings Marked for Review"]+" ("+Math.round10(outputStats["Percentage of Buildings Marked for Review"],-1)+"%) |"+outputStats["Unmatched Buildings"]+" ("+Math.round10(outputStats["Percentage of Unmatched Buildings"],-1)+"%)\n\
|Highways |"+outputStats["Highway Count"]+" |"+outputStats["Conflated Highways"]+" ("+Math.round10(outputStats["Percentage of Highways Conflated"],-1)+"%) |"+outputStats["Highways Marked for Review"]+" ("+Math.round10(outputStats["Percentage of Highways Marked for Review"],-1)+"%) |"+outputStats["Unmatched Highways"]+" ("+Math.round10(outputStats["Percentage of Unmatched Highways"],-1)+"%)\n\
|POIs |"+outputStats["POI Count"]+" |"+outputStats["Conflated POIs"]+" ("+Math.round10(outputStats["Percentage of POIs Conflated"],-1)+"%) |"+outputStats["POIs Marked for Review"]+" ("+Math.round10(outputStats["Percentage of POIs Marked for Review"],-1)+"%) |"+outputStats["Unmatched POIs"]+" ("+Math.round10(outputStats["Percentage of Unmatched POIs"],-1)+"%)\n\
|===============================\n\
<<<\n";

var areaFeatures = "=== Summary of Area Features\n" +
  "\n" +
  "Area features are the collection of OSM attributes that encompass a geographic area like a building footprint.  Hootenanny supports conflation of building type area features to date.  In this section, area measurements are presented for the total area features, the building features, and the output conflated buildings for both the inputs and output (where appropriate).  The area measurements are calculated in square kilometers and presented in the bar graph and tables.\n"+
  "\n"+
"The area measurements provide an estimate for how well Hootenanny performed conflation on the input data.  If the two input datasets do not overlap geographically, the output area should be the sum of the two input areas and the conflated output area equal to 0.  This also implies the output area should never be greater than the sum of the input areas.\n"+
  "\n" +
"Another rule to understand is it is correct to assume that the output area is always greater than or equal to the input with the largest area.  However, the results presented in the report here take into consideration that some features are marked for manual review (i.e. those not marked automatically as matches nor misses for conflation).  These cases are not included in the area measurement calculations for the output because their eventual outcome is unknown until the user makes a decision for each. This brings up a good point for future work to decide if the features marked for review should be included in the area measurements, even though it is unknown what the exact output area will be before the decision, an estimate could be made that approximates the logical rule defined here.\n" +   
  "\n" +
  "[gnuplot, areaHistogram.png]\n\
------\n\
reset\n"+
setLabels("Summary of Area Features","Kilometers Squared")+
histogramSetup+
histTables["areaTokens1"]+"\
------\n"+
histPost("areaHistogram.png", "areaHistogramr.png")+
addNAs(dataTables["areaTokens1"],"Conflated Buildings")+
"<<<\n";

var linearFeatures = "=== Summary of Linear Features\n"+
  "Linear features are the collection of OSM attributes that have a path-like structure and do not make up an area.  Hootenanny supports the highways type (described in an earlier section) to date.  In this section, length measurements are presented for the total linear features, the highway type features, and the output conflated highways. The length measurements are calculated in kilometers and presented in the bar graph and tables.\n"+
"\n"+  
"The length meaurements provide an estimate for how well Hootenanny performed conflation on the input data.  Like the area features, if the two input datasets do not overlap geographically, the output length should be the sum of the two input lengths and the conflated output length equal to 0.  This also implies the output length should never be greater than the sum of the input lengths.\n"+
"\n"+
"The next rule to understand is similar to the area feature's second rule.  It is correct to assume that the output length is always greater than or equal to the input with the longest length.  However, the results presented in the report here take into consideration that some features are marked for manual review (i.e., those not marked automatically as matches nor misses for conflation).  Like for area measurements, these cases are also not included in the length measurement calculations for the output because their eventual outcome is unknown until the user makes a decision for each.  The idea of estimating the lengths associated with the features marked for review will be discussed at a future point.\n" +
  "\n" +
  "[gnuplot, distancesHistogram.png]\n\
------\n\
reset\n"+
setLabels("Summary of Linear Features","Kilometers")+
histogramSetup+
histTables["distanceTokens1"]+"\
------\n"+
histPost("distanceHistogram.png", "distanceHistogramr.png")+
addNAs(dataTables["distanceTokens1"],"Conflated Highways")+
"<<<\n";

var poiFeatures = "=== Summary of POI Features\n"+
  "Points of Interest (POIs) are defined as nodes in Hootenanny, and are typically derived from the OSM arribute \"amenity\", which may contain features like: \"place of worship\", \"cafe\", etc.  The statistics presented in this section show the total number of POIs for the two inputs and the output, and the number of conflated POIs in the output. The data is presented in the bar graph and table provided.\n" +
  "\n" +
"The POI counts for the output should never be greater than the sum of the counts for the two inputs.  The maximum count for the output is reached when all of the POIs from both inputs are not matches for conflation.  This implies that any two POIs do not overlay geographically within a specified radius and their names are not similar.  If these conditions are completely satisifed, the output POI count reaches a maximum by summing the POIs from both inputs.\n"+
"\n"+
"It is also intuitive to think that the output count should always be greater than or equal to the maximum input count. But this is not correct if there exists any matching POIs from within the same input.  A match within an input is referred to as a \"self conflation\" when the PLACES POI conflation algorithm is being used.  Note, a match between inputs is known as a \"challenge conflation\" for the PLACES algorithm.\n"+
"\n"+
"There is also another case where the output may be lower than the expected and that occurs when POIs are combined due to their close spatial proximity (e.g., two Starbucks locations located within a half mile may get merged into one if the POI radius of influence is too large).  This may occur within and\/or between the two inputs.  This issue is documented in the Hootenanny Algorithms manual section \"The Starbucks Problem\".\n" +   
"\n"+
"[gnuplot, poiHistogram.png]\n\
------\n\
reset\n"+
setLabels("Summary of POI Features","Number of Features")+
histogramSetup+
histTables["poiTokens"]+"\
------\n"+
histPost("poiHistogram.png", "poiHistogramr.png")+
addNAs(dataTables["poiTokens"],"Conflated POIs")+
"<<<\n";

var uniqueNames = "=== Summary of Unique Names\n"+
  "A summary of the unique names used for the features are shown in the bar graph in this section.  The number of unique names for the two inputs and the output are shown in the left-most bar.  A breakdown of the unique names by the types of building and highways are shown in the right-most bars.  The table shows the specific counts for each data source and type.\n" +
  "\n" +
  "[gnuplot, uniqueHistogram.png]\n\
------\n\
reset\n"+
setLabels("Breakdown of Unique Names by Type","Number of Names")+
histogramSetup+
histTables["uniqueTokens"]+"\
------\n"+
histPost("uniqueHistogram.png", "uniqueHistogramr.png")+
dataTables["uniqueTokens"]+
"<<<\n";

var tagFeatures = "=== Summary of Tags\n" +
  "Tags describe specific attributes of the OSM data elements 'nodes', 'ways', and 'relations' (defined in an earlier section).  The tags in Hootenanny are classified in two categories: information and metadata.  Metadata tags contain data about the feature's provenance (for example, who created it, where it came from, UUID, etc.).  Information tags contain general information associated with the features (basically all the rest of the attributes).  The bar graph shows the total number of tags and a breakdown of the number of information and metadata tags for the two inputs and output datasets.  The table shows the specific counts for each category.\n" +
  "\n"+
  "[gnuplot, tagCountHistogram1.png]\n\
------\n\
reset\n"+
setLabels("Summary of Tags","Number of Tags")+
histogramSetup+
histTables["tagCountTokens1"]+"\
------\n"+
histPost("tagCountHistogram1.png", "tagCountHistogram1r.png")+
addNAs(dataTables["tagCountTokens1"],"Metadata Tags")+
  "<<<\n"+
  "=== Summary of Translated Tags\n" +
  "Translated tags are generated in Hootenanny by applying the translator to the inputs and conflated output data.  It applies the translation schema defined in Section 2 and then collects information about what was generated to build the statistics for the translated tags.  There are three types of translated tags: populated, default, and null.\n\n"+
  "- Populated tags are tags that have been assigned non-default values.  In other words, values populated from the source by the translation script.\n"+
  "- Default tags are tags that have been assigned the default value from the translation schema. For example, from the translation schema TDSv61:\n\n"+
  "--------------------------\n"+
  "\{ name:\"CAA\",\n"+
  "   desc:\"Controlling Authority\",\n"+
  "   optional:\"R\",\n"+
  "   defValue:\"-999999\",\n"+
  "   type:\"enumeration\",\n"+
  "   \<...\>\n"+
  "\}\n"+
  "--------------------------\n"+
  "If the translated output used the default value \'CAA == \"-999999\"' then the tag is a translated default tag.\n\n"+
  "- Null tags are tags that have been assigned a null value to the output where no default value (from the schema) nor any translated value was available.\n"+
  "\n"+
"The number of each translated tag type for the inputs and the conflated output are shown in the bar graph and table.  The breakdown of each tag type (default, populated, and null) are presented with the specific values provided in the table.\n" + 
  "\n" +
  "[gnuplot, tagCountHistogram2.png]\n\
------\n\
reset\n"+
setLabels("Summary of Translated Tags","Number of Tags")+
histogramSetup+
histTables["tagCountTokens2"]+"\
------\n"+
histPost("tagCountHistogram2.png", "tagCountHistogram2r.png")+
dataTables["tagCountTokens2"]+
"<<<\n";

var tagPercentages = "=== Summary of Tag Percentages\n" +
  "A summary of the percentage of tags for four specific types is shown in the bar graph.  The four types are: all, building, highway, and POI translated populated tags. For each type, a comparison of the percentage of the tags for the two inputs and output are shown.\n" +
  "\n" +
  "[gnuplot, tagPercentageHistogram.png]\n\
------\n\
reset\n"+
setLabels("Summary of Tag Percentages","Percentages")+
histogramSetup+
histTables["tagPercentageTokens1"]+"\
------\n"+
histPost("tagPercentageHistogram.png", "tagPercentageHistogramr.png")+
cleanUpNans( dataTables["tagPercentageTokens1"] )+
"<<<\n";

cleanUpNans(dataTables["tagPercentageTokens1"]);

var ioOperations = "" +
  "== File IO Operation Statistics for Input and Output Datasets\n" +
  "A summary of the bytes of data read and written is shown in the bar graph.\n" +
  "\n" +
  "[gnuplot, bytesBarChart.png]\n" +
  "------\n" +
  "set title \"Summary of Input and Output Operations\"\n" +
  histogramSetup+
  "set autoscale y\n"+
  "set ylabel \"Size (MBytes)\"\n"+
  "set xtic center offset -0.5,-2.5 rotate by -70 scale 0 font \"arial,10\"\n"+
  "set nokey\n"+
  "set label 1 'Size="+Math.round(stats4["Bytes Read"]/1048576*100.0)/100.0+"' at graph 0.22,0.95 front font \"Times,9\"\n"+
  "set label 2 'Size="+Math.round(stats4["Bytes Written"]/1048576*100.0)/100.0+"' at graph 0.6,0.95 front font \"Times,9\"\n"+
  "plot '-' using 2:xtic(1) ti col, '' u 2 ti col\n"+
  "unknownKey\n"+
  "\"Read Operations\"\t" + stats4["Bytes Read"]/1048576 + "\n" +
  "\"Write Operations\"\t" + stats4["Bytes Written"]/1048576 + "\n" +
  "e\n" +
  "------\n\
[width=\"50%\",options=\"header\"]\n\
|===============================\n\
|IO Operation|Size (MBytes)\n\
|Reads  |"+Math.round(stats4["Bytes Read"]/1048576*100.0)/100.0+"\n\
|Writes |"+Math.round(stats4["Bytes Written"]/1048576*100.0)/100.0+"\n\
|===============================\n\
<<<\n";

var oldRoadConf = stats4["Old Road Conflation Time (sec)"];
if (isNaN(oldRoadConf)) {
  oldRoadConf = 0;
}

var timingOther = stats4["Total Time Elapsed (sec)"] -  
  stats4["Read Inputs Time (sec)"] -
  stats4["Time to Calculate Stats for Input 1 (sec)"] -
  stats4["Time to Calculate Stats for Input 2 (sec)"] -
  stats4["Time to Calculate Stats for Output (sec)"] -
  stats4["Apply Named Ops Time (sec)"] -
  oldRoadConf -
  stats4["Apply Pre Ops Time (sec)"] -
  stats4["Project to Planar Time (sec)"] -
  stats4["Find Matches Time (sec)"] -
  stats4["Optimize Matches Time (sec)"] -
  stats4["Create Mergers Time (sec)"] -
  stats4["Apply Mergers Time (sec)"] -
  stats4["Apply Post Ops Time (sec)"] -
//  stats4["Conflation Time (sec)"] -
  stats4["Project to WGS84 Time (sec)"] -
  stats4["Write Output Time (sec)"];


var timingTable = "" +
  "\"Read Inputs\"\t\"Stats for Input 1\"\t\"Stats for Input 2\"\t\"Stats for Output\"\t\"Apply Named Ops\"\t\"Apply Pre Ops\"\t" +
  "\"Project to Planar\"\t\"Find Matches\"\t\"Optimize Matches\"\t\"Create Mergers\"\t" +
  "\"Apply Mergers\"\t\"Apply Post Ops\"\t\"Conflation\"\t\"Project to WGS84\"\t" +
  "\"Old Road Conflation\"\t\"Write Output\"\t" + "Other\n" +
  Math.round(stats4["Read Inputs Time (sec)"]*10.0)/10.0 + "\t" +
  Math.round(stats4["Time to Calculate Stats for Input 1 (sec)"]*10.0)/10.0 + "\t" + 
  Math.round(stats4["Time to Calculate Stats for Input 2 (sec)"]*10.0)/10.0 + "\t" + 
  Math.round(stats4["Time to Calculate Stats for Output (sec)"]*10.0)/10.0 + "\t" + 
  Math.round(stats4["Apply Named Ops Time (sec)"]*10.0)/10.0 + "\t" + 
  Math.round(stats4["Apply Pre Ops Time (sec)"]*10.0)/10.0 + "\t" +
  Math.round(stats4["Project to Planar Time (sec)"]*10.0)/10.0 + "\t" +
  Math.round(stats4["Find Matches Time (sec)"]*10.0)/10.0 + "\t" +
  Math.round(stats4["Optimize Matches Time (sec)"]*10.0)/10.0 + "\t" +
  Math.round(stats4["Create Mergers Time (sec)"]*10.0)/10.0 + "\t" +
  Math.round(stats4["Apply Mergers Time (sec)"]*10.0)/10.0 + "\t" +
  Math.round(stats4["Apply Post Ops Time (sec)"]*10.0)/10.0 + "\t" +
  Math.round(stats4["Project to WGS84 Time (sec)"]*10.0)/10.0 + "\t" +
  Math.round(oldRoadConf*10.0)/10.0 + "\t" +
  Math.round(stats4["Write Output Time (sec)"]*10.0)/10.0 + "\t" +
  Math.round(timingOther*10.0)/10.0 + "\n" +
  "e\n"; 

var ggg=outputStats["Feature Count"]; 
hoot.log("test test = ");
hoot.log(outputStats["Feature Count"]);
 
var timing = "== Hootenanny Run-time Performance\n"+ 
"A summary of the performance results by time for each of the processing steps of the conflation are shown in the bar graph.  The processing components measured are: read inputs, stats for inputs 1 and 2, stats for output, apply named operations, apply preprocessing operations, projections to planar, find matches, optimize matches, create mergers, apply mergers, apply post operations, projections to WGS84, write output, old road conflation, overall conflation, and other times.  The bar chart shows measurements in seconds and the table (on the next page) with precise timing in minutes, seconds, and milliseconds and percentage of overall time.  The table also shows some aggregate timing measurements: old road conflation (if applicable), conflation (aggregate of the core processing steps), and total (aggregate of all the steps).  The category 'other' is the residual timing information.\n\
\n\
[mpl, performanceTiming.png]\n\
------\n\
X=("+
  Math.round(timingOther*10.0)/10.0 + "," +
  Math.round(stats4["Conflation Time (sec)"]*10.0)/10.0 + "," +
  Math.round(oldRoadConf*10.0)/10.0 + "," +
  Math.round(stats4["Write Output Time (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Project to WGS84 Time (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Apply Post Ops Time (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Apply Mergers Time (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Create Mergers Time (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Optimize Matches Time (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Find Matches Time (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Project to Planar Time (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Apply Pre Ops Time (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Apply Named Ops Time (sec)"]*10.0)/10.0 + ","+ 
  Math.round(stats4["Time to Calculate Stats for Output (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Time to Calculate Stats for Input 2 (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Time to Calculate Stats for Input 1 (sec)"]*10.0)/10.0 + "," +
  Math.round(stats4["Read Inputs Time (sec)"]*10.0)/10.0 + ")\n" + 
"val=X\n\
pos=arange(17)+.5\n\
figure(1)\n\
barh(pos,val,align='center')\n\
yticks(pos, ('Other','Conflation','Old Road Conflation', 'Write Output', 'Project to WGS84', 'Apply Post Ops', 'Apply Mergers', 'Create Mergers', 'Optimize Matches', 'Find Matches', 'Project to Planar', 'Apply Pre Ops', 'Apply Named Ops', 'Stats for Output', 'Stats for Input 2', 'Stats for Input 1', 'Read Inputs'))\n\
xlabel('Time (s)')\n\
title('Timing of Processing Steps')\n\
------\n"+
"<<<\n" +
histPost("timingHistogram1.png", "timingHistogram1r.png")+"\
[width=\"75%\",options=\"header\"]\n\
|===============================\n\
|Processing Step|Time\n\
|Read Inputs|"+ formatTime(stats4["Read Inputs Time (sec)"]) + calcPercent(stats4["Read Inputs Time (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Stats for Input 1|"+ formatTime(stats4["Time to Calculate Stats for Input 1 (sec)"]) + calcPercent(stats4["Time to Calculate Stats for Input 1 (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Stats for Input 2|"+ formatTime(stats4["Time to Calculate Stats for Input 2 (sec)"]) + calcPercent(stats4["Time to Calculate Stats for Input 2 (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Stats for Output|"+ formatTime(stats4["Time to Calculate Stats for Output (sec)"]) + calcPercent(stats4["Time to Calculate Stats for Output (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Apply Named Ops|"+ formatTime(stats4["Apply Named Ops Time (sec)"]) + calcPercent(stats4["Apply Named Ops Time (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Apply Pre Ops|"+ formatTime(stats4["Apply Pre Ops Time (sec)"]) + calcPercent(stats4["Apply Pre Ops Time (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Project to Planar|"+ formatTime(stats4["Project to Planar Time (sec)"]) + calcPercent(stats4["Project to Planar Time (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Find Matches|"+ formatTime(stats4["Find Matches Time (sec)"]) + calcPercent(stats4["Find Matches Time (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Optimize Matches|"+ formatTime(stats4["Optimize Matches Time (sec)"]) + calcPercent(stats4["Optimize Matches Time (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Create Mergers|"+ formatTime(stats4["Create Mergers Time (sec)"]) + calcPercent(stats4["Create Mergers Time (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Apply Mergers|"+ formatTime(stats4["Apply Mergers Time (sec)"]) + calcPercent(stats4["Apply Mergers Time (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Apply Post Ops|"+ formatTime(stats4["Apply Post Ops Time (sec)"]) + calcPercent(stats4["Apply Post Ops Time (sec)"],stats4["Total Time Elapsed (sec)"],true)  + "\n\
|Project to WGS84|"+ formatTime(stats4["Project to WGS84 Time (sec)"]) + calcPercent(stats4["Project to WGS84 Time (sec)"],stats4["Total Time Elapsed (sec)"],true)  + "\n\
|Write Output|"+ formatTime(stats4["Write Output Time (sec)"]) + calcPercent(stats4["Write Output Time (sec)"],stats4["Total Time Elapsed (sec)"],true)  + "\n\
|Old Road Conflation|"+ formatTime(stats4["Old Road Conflation Time (sec)"]) + calcPercent(stats4["Old Road Conflation Time (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Conflation (aggregate of several steps)|"+ formatTime(stats4["Conflation Time (sec)"]) + calcPercent(stats4["Conflation Time (sec)"],stats4["Total Time Elapsed (sec)"],true) + "\n\
|Other|"+ formatTime(timingOther) + calcPercent(timingOther,stats4["Total Time Elapsed (sec)"],true) + "\n\
|Total|"+ formatTime(stats4["Total Time Elapsed (sec)"])+"\n\
|===============================\n";


// build the string using \n separator
  var doc = summary+"\n\n"+ 
    features+"\n"+
    areaFeatures+"\n\n"+
    linearFeatures+"\n\n"+
    poiFeatures+"\n\n"+
    uniqueNames+"\n\n"+
    tagFeatures+"\n\n"+
//    tagPercentages+"\n\n"+
//    ioOperations+"\n\n"+
    timing+"\n";

hoot.log("TRANSLATED POP TAG COUNT");
hoot.log(outputStats["Translated Populated Tag Count"]); 
hoot.log("TRANSLATED DEFAULT TAG COUNT");
hoot.log(outputStats["Translated Default Tag Count"]); 
hoot.log("TRANSLATED NULL TAG COUNT");
hoot.log(outputStats["Translated Null Tag Count"]); 

  return doc;
}


///////////////////////////////////////////////////////////////////
// HISTOGRAM-LIKE TABLES (3 BARS : input1, input2, output)
///////////////////////////////////////////////////////////////////

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
