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

exports.textBody = function(input1Stats, input2Stats, outputStats, histTables, stats4, dataTables)
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

var summary = "== Summary\n" +
  "A summary of the most important statistics for the conflation job just executed are presented in the bar graph and table below.  Shown are the total number of features and the number that are conflatable for both inputs.  Next is the total number of features in the output after splitting and merging conflation steps have taken place.  The number should have no correlation with the input numbers due to the splitting of features and other processing steps.  Last is the number of the output features that were conflated and marked for manual review.\n\
\n\
Note for now we do not show percentage values of how many features were conflated from the inputs to the output because there is no easy way to calculate and record these values while considering the feature splits and the impact on the performance of Hootenanny's conflation. \n"+
  "\n" + 
  "[gnuplot, summaryChart.png]\n" +
  "------\n" +
  histogramSetup+
  "set yrange [0:100]\n"+ 
  "set autoscale ymax\n"+
  "set title \"Summary of Important Statistics\"\n"+
  "set ylabel \"Number of Features\"\n"+
  "unset xtics\n"+
  "set xtic rotate by -70 center offset 0.5,-3 scale 0 font \"arial,10\"\n"+
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
  "\"Input 1 Conflatables\"\t" + input1Stats["Total Conflatable Features"] + "\n" +
  "\"Input 2 Conflatables\"\t" + input2Stats["Total Conflatable Features"] + "\n" +
  "\"Output Total Features\"\t" + outputStats["Total Feature Count"] + "\n" +
  "\"Conflated Features\"\t" + outputStats["Total Conflated Features"] + "\n" +
  "\"Reviewable Features\"\t" + outputStats["Total Features Marked for Review"] + "\n" +
//  "\"Unmatched Features\"\t" + outputStats["Total Unmatched Features"] + "\n" +
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
|Conflated Features |"+outputStats["Total Conflated Features"]+" |NA\n\
|Reviewable Features |"+outputStats["Total Features Marked for Review"]+" |NA\n\
|===============================\n\
<<<\n";


var features = "== Statistical Aspects of the Input and Output Datasets\n" +
  "Statistics for a variety of aspects of the input and output datasets are presented in this section.\n" +
  "\n" +
  "=== Summary of Basic Feature Elements\n" +
  "There are three basic elements of OpenStreetMap's conceptual data model of the physical world. These are nodes (points in space), ways (linear and area features), and relations (a high-level construct that helps coordinate how ways and nodes work together).  To go along with these are tags which can associate with any of the three basic elements.  In this section, a summary of the number of each element and tags are shown in the bar graph.  For each,  a comparison of the number of features for the two inputs and output are shown.\n" +
  "\n" +
  "Note: More information about the OSM Elements is provided at: wiki.openstreetmap.org/wiki/Elements\n" +
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
  "=== Summary of Key Hootenanny Features\n" +
  "Some of the key feature types that Hootenanny operates upon are 'buildings', 'highways', and 'points of interest (POIs)'.  These types are defined using the naming conventions that OSM assigns for these attributes.  The buildings type are the area footprints for what the title indicates--buildings.  The highways type is a bit more generic and refers to a collection of linear segments that include road types (highways, streets, dirt roads), cart tracks, and trails.  The POIs are defined for Hootenanny as nodes, but in general are either linear or area features.  The POIs are typically derived from the OSM attribute \"amenity\".  For example, POIs may be a \"place of worship\", \"cafe\", etc.  A summary of the number of buildings, highways, and POIs within the input and output datasets is shown in the bar graph.\n" +
  "\n" +
  "[gnuplot, featureCountHistogram2.png]\n"+
  "------\n"+
  "reset\n"+
  setLabels("Summary of Key Hootenanny Features","Number of Features")+
  histogramSetup+
  histTables["countTokens2"]+
  "------\n"+
  histPost("featureCountHistogram2.png", "featureCountHistogram2r.png")+
  dataTables["countTokens2"]+
  "<<<\n" +
  "=== Summary of Conflated Features\n" +
  "The statistics key to the conflation process are described in this section.  The bar graph shows first the number of total features and then the number of features that are conflatable.  Those that are conflatable are the ones not marked for review.  The table shows the conflatable count and its percentage of the total features.  The statistics shown to their right show the total number of conflated features and a breakdown of the number of conflated features for each of the basic feature types supported: buildings, highways, and POIs.  For each type, the table shows the count and the percentage relative to the conflatables count.  The bar heights show the respective counts for all values. \n" +
  "\n" +
  "[gnuplot, conflatedFeature.png]\n" +
  "------\n" +
  histogramSetup+
  "set autoscale y\n"+
  "set title \"Summary of Conflated Features\"\n"+
  "set ylabel \"Number of Features\"\n"+
  "unset xtics\n"+
  "set xtic rotate by -70 center offset 0.5,-3 scale 0 font \"arial,10\"\n"+
  "set nokey\n"+
  "set size 1,1\n"+
  "set terminal png size 600,450\n"+
  "set bmargin at screen 0.3\n"+
//  "set label 1 'Percentage ="+roundPercent(outputStats["Percentage of Total Features Marked for Review"])+"' at graph 0.3,0.97 front font \"Times,9\"\n"+
//  "set label 2 'Percentage ="+roundPercent(outputStats["Percentage of Total Features Conflated"])+"' at graph 0.5,0.97 front font \"Times,9\"\n"+
//  "set label 3 'Percentage ="+roundPercent(outputStats["Percentage of Total Features Unmatched"])+"' at graph 0.7,0.97 front font \"Times,9\"\n"+
  "plot '-' using 2:xtic(1) ti col, '' u 2 ti col\n"+
  "unknownKey\n"+
  "\"Total Features\"\t" + outputStats["Total Feature Count"] + "\n" +
  //this stat has no meaning in the output data now
  //"\"Conflatable Features\"\t" + outputStats["Total Conflatable Features"] + "\n" +
  "\"Conflated Features\"\t" + outputStats["Total Conflated Features"] + "\n" +
  "\"Conflated Buildings\"\t" + outputStats["Conflated Buildings"] + "\n" +
  "\"Conflated Highways\"\t" + outputStats["Conflated Highways"] + "\n" +
  "\"Conflated POIs\"\t" + outputStats["Conflated POIs"] + "\n" +
  "e\n" +
  "------\n\
[width=\"90%\",options=\"header\"]\n\
|===============================\n\
|  |Count |Percentage\n\
|Total Features |"+outputStats["Total Feature Count"] + " |NA\n\
|Conflated Features |"+outputStats["Total Conflated Features"]+" |"+calcPercent(outputStats["Total Conflated Features"],outputStats["Total Conflatable Features"],false)+"\n\
|Conflated Buildings |"+outputStats["Conflated Buildings"]+" |"+calcPercent(outputStats["Conflated Buildings"],outputStats["Total Conflatable Features"],false)+"\n\
|Conflated Highways |"+outputStats["Conflated Highways"]+" |"+calcPercent(outputStats["Conflated Highways"],outputStats["Total Conflatable Features"],false)+"\n\
|Conflated POIs |"+outputStats["Conflated POIs"]+" |"+calcPercent(outputStats["Conflated POIs"],outputStats["Total Conflatable Features"],false)+"\n\
|===============================\n\
<<<\n";

var areaFeatures = "=== Summary of Area Features\n" +
  "\n" +
  "Area features are the collection of OSM attributes that encompass a geographic area like a building footprint.  Hootenanny supports conflation of building type area features to date.  The statistics presented in this section show the area features, the portion of those that are buildings, and the portion of buildings that are conflated.  The bar graph shows the area that each of these categories encompass across each set of items.  The area provides an estimate metric for how much effort was performed by the tool.  Measurements are recorded in square kilometers.  The table shows the area measurements for the two inputs and output for each category (where appropriate).\n" +
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
  "Linear features are the collection of OSM attributes that have a path-like structure and do not make up an area.  Hootenanny supports the highways type (described in an earlier section) to date.  The statistics presented in this section show the linear features, the portion of those that are highways, and portion of highways that are conflated.  The bar graph shows the aggregate length of all the features within each category.  The length provides an estimate metric for how much effort was performed by the tool.  The measurements are recorded in kilometers.  The table shows the length measurements for the two inputs and output for each category (where appropriate).\n" +
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
  "Points of Interest (POIs) are defined as nodes in Hootenanny, and are typically derived from the OSM arribute \"amenity\", which may contain features like: \"place of worship\", \"cafe\", etc.  The statistics presented in this section show the total number of POIs for the two inputs and output, and then the number of conflated POIs in the output.  The bar graph shows the number for each category.  The table shows the counts for the inputs and output (where appropriate).\n" +
  "\n" +
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
  "This section shows the number of unique names that are in the two inputs and output across three categories of features.  The three categories shown are: all features, highways, and buildings.  The bar graph shows the number of names for each category. The table shows the specific counts for the categories (where appropriate).\n" +
  "\n" +
  "[gnuplot, uniqueHistogram.png]\n\
------\n\
reset\n"+
setLabels("Summary of Unique Names","Number of Unique Names")+
histogramSetup+
histTables["uniqueTokens"]+"\
------\n"+
histPost("uniqueHistogram.png", "uniqueHistogramr.png")+
dataTables["uniqueTokens"]+
"<<<\n";

var tagFeatures = "=== Summary of Tags\n" +
  "Tags describe specific features of the OSM data elements 'nodes', 'ways', and 'relations' (defined in an earlier section).  Tags are classified in this section as information and metadata.  The metadata tags are the ones containing data about the feature's provenance.  For example, who created it, where it came from, UUID, etc. The information tags contain the general information associated with the features (basically all the rest of the attributes).  The bar graph shows the total number of tags and the breakdown of the number of information and metadata tags for the two inputs and output datasets.  The table shows the specific counts for each category.\n" +
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
  "Translated tags are defined as the tags produced from a conversion from a different data source type (e.g. Shapefile).  In the conversion, the attributes of the input data source are converted to OSM+ tags.  The translation to the OSM+ feature may only minimally need a portion of the input attributes to fill the requirements it needs.  For example, a shapefile may have ten different attributes, but the same type of feature in OSM+ might only require or define five of those attributes.  This section shows three types of translated tags: populated, default, and null tags.  The default tags are those described by the definition above.  The populated tags are those where a tag is generated and populated for an unassigned attribute.  Null tags are those generated for unassigned attributes that are not populated.  A summary of the number of tags for the three tag types across the two inputs and output are shown in the bar graph.  The specific values for each of the tag types is shown in the table.\n" +
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
