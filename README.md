![hoot_logo](https://github.com/ngageoint/hootenanny-ui/blob/develop/dist/img/logo/hoot_logo_dark.png)

![](https://github.com/ngageoint/hootenanny/blob/master/docs/user/images/id/hoot_conflation_new.png)

# Overview
_Hootenanny_: 

1. A gathering at which folksingers entertain often with the audience joining in

_Conflation_: 

1. Fancy word for merge

[Hootenanny](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc) is an open source map data conflation tool developed with machine learning techniques to facilitate automated and semi-automated conflation of critical Foundation GEOINT features in the topographic domain. **In short, it merges multiple maps into a single seamless map.**

Hootenanny conflation occurs at the dataset level, where the user’s workflow determines the best reference dataset, source content, geometry, and attributes to transfer to the output map.  Hootenanny's internal processing leverages the key/value pair structure of OpenStreetMap (OSM) for improved utility and applicability to broader user groups.  Normalized attributes can be used to aid in feature matching, and OSM’s free tagging system allows the map to include an unlimited number of attributes describing each feature.

# Goals
* Automatically combine geospatial features for decision making
* Allow for reviewing and manually resolving features which cannot be automatically matched with sufficient certainty
* Maintain geometry and attribute provenance for combined features
* Create up-to-date routable transportation networks from multiple sources

# When To Use
No automated map conflation technology is perfect. If you are conflating a relatively small number of features, you may be best served to combine them manually yourself, given you are looking for a perfect result and want to avoid any potential time spent configuring Hootenanny software options. 

For larger datasets, however, Hootenanny can be utilized standalone or as an inital step in conjunction with a crowd sourced [mapping campaign](https://tasks.hotosm.org/) to add new data into your dataset. You will find that the conflation automation provided by Hootenanny saves effort overall, and that most inaccuracies in the conflated output are a small subset of the input data which often end up being flagged for human review so that they may be manually corrected.

# Conflatable Feature Types

Hootenanny has conflation algorithms available for the following data types:
* Areas
* Buildings
* Points of Interest (POIs)
* Power Lines
* Railways
* Rivers
* Roads

Any feature that does not fit into the list of types above will be conflated with [Generic Geometry Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/GenericGeometryConflation.asciidoc), which uses a simpler approach.

[Conflation Algorithms Detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/ConflationAlgsOverview.md)

You can create your own custom conflation algorithms for additional feature types via [Javascript](https://github.com/ngageoint/hootenanny/blob/master/docs/JavascriptOverview.asciidoc) or [C++](https://github.com/ngageoint/hootenanny/blob/master/hoot-core/src/main/cpp/hoot/core/conflate/matching/MatchCreator.h). 

# [Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc) Workflows

A conflation workflow defines the manner in which two maps are merged together. Hootenanny has the following workflows:
* **[Attribute Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/AttributeConflation.asciidoc)** - _Transfer attributes over to existing geometries_ - Conflate map A with B where only tags are transferred from B to matching features in A and no changes are made to A's geometries. Use this type of conflation when the first dataset's geometry is superior to a second dataset, but the attributes of the second dataset are superior to that of the first dataset. This is similar to Differential Conflation With Tags but does not have the capability
of adding new non-conflicting featues.
* **[Differential Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/DifferentialConflation.asciidoc)** - _Add new features that do not conflict_ - Conflate map A with B where the only data added to the output from B is in areas that don't overlap with A. Use this type of conflation when you want to fill holes in your dataset with data from another source without modifying any of the data in the first dataset.
* **[Differential Conflation With Tags](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/DifferentialConflation.asciidoc)** - _Add new features that do not conflict and transfer attributes to existing features_ - This workflow is the same as Differential Conflation with the added step of transferring tags to existing features in map A from matching features in map B. Use this type of conflation when you want to fill holes in your dataset with data from another source without modifying geometries in your original data but possibly modifying its tags. This is similar to Attribute Conflation but with the added capability of adding new non-conflicting features.
* **[Horizontal Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/cookie-cut.asciidoc)** (aka Cookie Cutter Conflation) - _Completely replace a section_ - Either 1) Define a region in map A and replace data in that region with data in the same region from map B OR 2) Define a region in map A to preserve and replace data outside of it with data outside of the region from map B. Use this type of conflation if you have a specific region of your dataset that you would like to replace with data from another dataset or you would like to surround your dataset with new data.
* **[Reference Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc)** **(default)** - _Keep the best of both while favoring one of them_ - Conflate the best geometry and tag parts of map B into map A, favoring map A's data. Use this type of conflation when you want conflated output based on the best state of both input datasets.

# Attribute Translation
Hootenanny leverages the OSM key value pair tag concept to support translation between various data schemas and supports the following schemas: 
* Topographic Data Store (TDS) 
* Multi-National Geospatial Co-Production Program (MGCP)
* Geonames
* OSM 
* [others](https://github.com/ngageoint/hootenanny/tree/master/translations)

Users can define their own [custom schema translations](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc#translations) via Javascript or Python.

# Feature Validation and Cleaning

Hootenanny performs no data validation reporting of its own, but JOSM validation routines may be [invoked via Hootenanny](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureValidationAndCleaning.asciidoc).

Hootenanny has a variety of map cleaning capabilities to automatically correct erroneous data. Some of them are invoked by default during a conflation job, however you may also [run them separately](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/clean.asciidoc). In addition to its own built-in cleaning operations, Hootenanny is also [integrated](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureValidationAndCleaning.asciidoc) with the auto-fix capabilities of [JOSM](https://josm.openstreetmap.de/). 

**NOTE:** As of 3/19/20 the cleaning/validation integration with JOSM is only available in the Vagrant version of Hootenanny. It will be added to the RPM version of Hootenanny at a later date.

# Feature Filtering

Hootenanny has the capability to let you selectively pick the features that are conflated from your data to save you from pre-conflation data wrangling. 

Some examples:
* [Conflate only buildings](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#conflate-only-buildings)
* [Conflate only restaurant buildings](https://github.com/ngageoint/hootenanny/master/blob/docs/user/CommandLineExamples.asciidoc#conflate-only-restaurant-buildings)
* [Conflate only restaurant buildings with "Subway" in the name](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#conflate-only-restaurant-buildings-with-subway-in-the-name)

Hootenanny has several [available filters](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#list-all-entities-that-can-operate-on-data) that may be used to perform feature filtering during conflation.

# Scalability
Hootenanny currently does not strive to conflate data at the global level. An earlier implementation of Hootenanny supported a map-reduce 
architecture that was capable of global conflation for some data types but was shelved due to general lack of interest and the maintenance 
costs to support the seldom used capability (so some of the conflation algorithms are actually capable of supporting distributed 
computing, if you want to go that route...with limitations). 

Hootenanny generally can scale well running on a single machine from the larger city level up to the smaller country level, depending on the 
density of the data being conflated and the RAM available on the machine. Beyond that, new conflation algorithms and/or parallelization of
existing algorithms would need to be developed to handle very large quantities of map data.

# Configuration

Although Hootenanny is configured by default to provide the best conflation results, at times you will need to tweak its wide range of [configuration options](https://github.com/ngageoint/hootenanny/blob/master/conf/core/ConfigOptions.asciidoc) in order to get the best conflated result for your input data.

# Web User Interface
[Hootenanny's](https://github.com/ngageoint/hootenanny-ui) [web user interface](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc) is built upon the open source 
[Mapbox iD Editor](https://github.com/openstreetmap/iD), which provides an intuitive and user-friendly conflation experience.

# Web Services API
Access to Hootenanny core capabilities are exposed through a 
[web services API](https://github.com/ngageoint/hootenanny/tree/master/hoot-services/src/main/java/hoot/services/controllers) for those 
wishing to develop their own conflation clients. The web services use 
[OAuth](https://github.com/ngageoint/hootenanny/tree/master/docs/developer/OAUTH.md) authentication.

# Command Line Interface
[Command line access](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/HootCommandLineReference.asciidoc) is available to aid
in custom scripting of conflation capabilities.

Example:

```
#  conflate two datasets together
hoot conflate input1.osm input2.osm output.osm
```

[More examples](https://github.com/ngageoint/hootenanny/tree/master/docs/user/CommandLineExamples.asciidoc)

# Programming Language Bindings
Hootenanny has [nodejs bindings](https://github.com/ngageoint/hootenanny/blob/master/docs/JavascriptOverview.asciidoc) available which expose 
core conflation capabilities for creating custom workflows. 

Example:

```
//  conflate two datasets together
var hoot = require(process.env.HOOT_HOME + '/lib/HootJs');
var map = new hoot.OsmMap();
hoot.loadMap(map, "input1.osm", false, 1);
hoot.loadMap(map, "input2.osm", false, 2);
new hoot.UnifyingConflator().apply(map)
hoot.saveMap(map, "output.osm");
```

# Additional Features

In addition to running conflation jobs with map data, Hootenanny also provides finer-grained capabilities:
* [Add missing type tags](https://github.com/ngageoint/hootenanny/blob/master/docs/user/ImplicitTypeTagging.asciidoc) to features based on their name
* [Align two maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/rubber-sheet.asciidoc) more closely with each other
* [Apply data transformation operations](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/convert.asciidoc#conversion-operations) to a map
* [Calculate the geospatial extent](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/extent.asciidoc) of a map
* [Clean map data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/clean.asciidoc)
* [Compare maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/diff.asciidoc) with each other
* [Compute bounding tiles](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/node-density-tiles.asciidoc) based on node density
* [Convert maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/convert.asciidoc) between [different geodata formats](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc)
* [Crop a map](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/crop.asciidoc) to a geospatial extent
* [Derive large changesets between maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-derive.asciidoc) and [push](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-apply.asciidoc) them through the OpenStreetMap API with automatic conflict resolution
* [Detect spoken languages](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LanguageTranslation.asciidoc) in a map's tag data
* [Explore tag data](https://github.com/ngageoint/hootenanny/blob/master/docs/user/TagExploration.asciidoc)
* [Filter a subset of features](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureFiltering.asciidoc) before conflation based on tag content and schema relationships
* [Grab a random section of data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/crop-random.asciidoc) from a map for testing against
* [Identify road intersections](https://github.com/ngageoint/hootenanny/blob/master/docs/user/IdentifyRoadIntersections.asciidoc) in a map
* [Locate phone numbers](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LocatePhoneNumbers.asciidoc) geographically
* [Perturb map data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/perty.asciidoc) for testing purposes
* [Plot node density](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/node-density-plot.asciidoc)
* [Simplify ways](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#simplify-ways-in-a-map-by-removing-unnecessary-nodes-from-them) by removing unnecessary nodes
* [Snap unconnected ways](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SnapUnconnectedWays.asciidoc) to neighboring ways by type
* [Process map postal address data](https://github.com/ngageoint/hootenanny/blob/master/docs/user/PostalAddressUtilities.asciidoc)
* [Replace data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-derive-replacment.asciidoc) completely within a bounding box
* [Sort map data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/sort.asciidoc)
* [Translate feature tags](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc#translations) using standardized or user defined [schemas](https://github.com/ngageoint/hootenanny#tag-schemas)
* [Translate feature tags to English](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LanguageTranslation.asciidoc)
* [Validate features](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureValidation.asciidoc)

# [Supported Data Formats](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc)

# Documentation
* [Included as a PDF with each release](https://github.com/ngageoint/hootenanny/releases)
* Online:
  * [User Interface Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyUI.asciidoc)
  * [User Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyUserGuide.asciidoc)
  * [Algorithms Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyAlgorithms.asciidoc)
  * [Algorithm Detail Slides](https://github.com/ngageoint/hootenanny/wiki/Conflation-Algorithm-Slides)
  * [Developer's Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyDeveloperGuide.asciidoc)
* When running the Hootenanny VM look for the PDF documentation included in 'hoot/docs' 

# Installation
* [Instructions](https://github.com/ngageoint/hootenanny/blob/master/docs/user/VAGRANT.md) to launch a virtual machine
* [Instructions](https://github.com/ngageoint/hootenanny/blob/master/docs/install/HootenannyInstall.asciidoc) for an RPM based installation

# Support
* [FAQ](https://github.com/ngageoint/hootenanny/wiki/Frequently-Asked-Questions)

Don't hesitate to ask for help if features aren't conflating how you expect them to or if you're experiencing difficulty while installing the 
software. We officially support installing RPMs to CentOS and launching CentOS virtual matchines...anything else we'll do our best to help you out, but there are no guarantees. If you have any support questions, please create an issue in this repository.

As there are lot of different data scenarios out in the wild, there is no one-size fits all conflation workflow or algorithm. Hootenanny 
attempts to provide a useful set of conflation workflows with pre-configured options that capture most conflation requirements. However, you
may require a new conflation scenario for your purposes (let us know), or you may need to custom modify configuration options for an existing scenario in order to get the best results.

Also, the availability of new software features to the user interface may lag their initial availability from the CLI by multiple 
development cycles. If you find a conflation feature you wish to use that is mentioned in the CLI documentation but is not present within the UI, let us know.

# Development
* [Contributing](https://github.com/ngageoint/hootenanny/blob/master/CONTRIBUTING.md)
* [Developer's Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyDeveloperGuide.asciidoc)

# [Redistribution](https://github.com/ngageoint/hootenanny/blob/master/docs/Redistribution.md)
