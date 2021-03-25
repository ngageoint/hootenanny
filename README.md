![hoot_logo](https://github.com/ngageoint/hootenanny-ui/blob/develop/dist/img/logo/hoot_logo_dark.png)

![](https://github.com/ngageoint/hootenanny/blob/master/docs/user/images/id/hoot_conflation_new.png)

# Overview
_Hootenanny_: 

1. A gathering at which folksingers entertain often with the audience joining in

_Conflation_: 

1. Fancy word for merge

[Hootenanny](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc) is an open source map data conflation tool developed with machine learning techniques to facilitate automated and semi-automated conflation of critical Foundation GEOINT features in the topographic domain. **In short, it merges multiple maps into a single seamless map.**

## Goals
* Automatically combine geospatial features for decision making
* Allow for reviewing and manually resolving features which cannot be automatically matched with sufficient certainty
* Maintain geometry and attribute provenance for combined features
* Create up-to-date routable transportation networks from multiple sources

Hootenanny conflation occurs at the dataset level, where the user’s workflow determines the best reference dataset, source content, geometry, and attributes to transfer to the output map.  Hootenanny's internal processing leverages the key/value pair structure of OpenStreetMap (OSM) for improved utility and applicability to broader user groups.  Normalized attributes can be used to aid in feature matching, and OSM’s free tagging system allows the map to include an unlimited number of attributes describing each feature.

No automated map conflation technology is perfect. If you are conflating a relatively small number of features, you may be best served to combine them manually yourself, given you are looking for a perfect result and want to avoid any potential time spent configuring software options to get the best conflated output. For larger datasets, however, Hootenanny is a valuable tool that can be utilized standalone or as an inital step in conjunction with a crowd sourced [mapping campaign](https://tasks.hotosm.org/) to add new data into your dataset. You will find that the conflation automation provided by Hootenanny saves effort overall, and that most inaccuracies in the conflated output are a small subset of the input data which often end up being flagged for human review so that they may be manually corrected.

# Conflatable Feature Types

Hootenanny has specifically tailored conflation algorithms available for the following data types:
* Areas
* Buildings
* Points of Interest (POIs)
* Power Lines
* Railways
* Rivers
* Roads

Any feature whose type does not fit into the list of types above, or has no type at all, will be conflated with 
[Generic Geometry Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/GenericGeometryConflation.asciidoc), which uses a 
simpler rules based approach than the type specific conflation algorithms.

[Conflation Algorithms Detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/ConflationAlgsOverview.md)

You can create your own custom conflation algorithms for additional feature types via [Javascript](https://github.com/ngageoint/hootenanny/blob/master/docs/JavascriptOverview.asciidoc) or [C++](https://github.com/ngageoint/hootenanny/blob/master/hoot-core/src/main/cpp/hoot/core/conflate/matching/MatchCreator.h). 

# [Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc) Workflows

A conflation workflow defines the manner in which two maps are merged together. Hootenanny has the following workflows:
* **[Reference Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc)** **(default)** - _Keep the best of both maps while favoring the first_
  * Use this type of conflation when you want map output based on the best state of two maps while favoring the first one.
* **[Average Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc)** - _Keep an average of both maps_ 
  * Use this type of conflation when you consider both input maps equal in quality and want a result that is an average of the two.
  * Currently, geometry averaging only applies to linear features but could be extended to point and polygon geometries. Point and polygon geometries are merged the same as in Reference Conflation. 
  * Average Conflation is currently not available from iD Editor.
* **[Horizontal Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/cut.asciidoc)** (aka Cookie Cutter Conflation) - _Completely replace a section_
  * Use this type of conflation if you have a specific region of your map that you would like to completely replace with a region from another map.
* **[Differential Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/DifferentialConflation.asciidoc)** - _Add new features that do not conflict_
  * Use this type of conflation when you want to fill holes in your map with data from another source without actually modifying any of the data in your map.
  * There is an option available to additionally transfer tags to existing features in your map from matching features in another map where overlap occurs.
* **[Attribute Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/AttributeConflation.asciidoc)** - _Transfer attributes over to existing geometries_
  * Use this type of conflation when one map's geometry is superior to that of a second map, but the attributes of the second map are superior to that of the first map.
  
# Attribute Translation
Hootenanny leverages the OSM key value pair tag concept to support translation between various data schemas and supports the following schemas: 
* Topographic Data Store (TDS) 
* Multi-National Geospatial Co-Production Program (MGCP)
* Geonames
* OSM 
* [others](https://github.com/ngageoint/hootenanny/tree/master/translations)

Users can define their own [custom schema translations](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc#translations) via Javascript or Python.

# Feature Filtering

Hootenanny has the capability to let you selectively pick the features that are conflated from your data to save you from pre-conflation data wrangling. 

Some examples:
* [Conflate only buildings](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#conflate-only-buildings)
* [Conflate only restaurant buildings](https://github.com/ngageoint/hootenanny/master/blob/docs/user/CommandLineExamples.asciidoc#conflate-only-restaurant-buildings)
* [Conflate only restaurant buildings with "Subway" in the name](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#conflate-only-restaurant-buildings-with-subway-in-the-name)

Hootenanny has several [available filters](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#list-all-entities-that-can-operate-on-data) that may be used to perform feature filtering during conflation.

# Feature Validation and Cleaning

Hootenanny performs no data validation reporting of its own, but JOSM validation routines may be [invoked via Hootenanny](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureValidationAndCleaning.asciidoc) during batch processing of data.

Hootenanny has a variety of map cleaning capabilities to automatically correct erroneous data. Some of them are invoked automatically during a conflation job, however, you may also [run them separately](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/clean.asciidoc). In addition to its own built-in cleaning operations, Hootenanny is also [integrated](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureValidationAndCleaning.asciidoc) with the auto-fix capabilities of [JOSM](https://josm.openstreetmap.de/). 

**NOTE:** As of 3/19/20 the cleaning/validation integration with JOSM is only available in the Vagrant version of Hootenanny. It will be added to the RPM version of Hootenanny at a later date.

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

# Feature Summary

In addition to conflating map data, Hootenanny also provides these supporting capabilities:
* [Add missing type tags](https://github.com/ngageoint/hootenanny/blob/master/docs/user/ImplicitTypeTagging.asciidoc) to features implicitly based on their name (experimental)
* [Align two maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/align.asciidoc) more closely with each other
* [Apply data transformation operations](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/convert.asciidoc#conversion-operations) to a map
* [Calculate the geospatial extent](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/extent.asciidoc) of a map
* [Clean map data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/clean.asciidoc)
* [Compare maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/diff.asciidoc) with each other
* [Convert maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/convert.asciidoc) between [different geodata formats](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc)
* [Create a covering shape](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/alpha-shape.asciidoc) that minimally covers all features in a map
* [Crop a map](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/crop.asciidoc) to a geospatial extent
* [De-duplicate features](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#remove-intra-map-duplicates-within-a-single-map) within a single map or across two maps or simply [find duplicate features](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#mark-exact-duplicate-features-within-a-map) in a map (experimental)
* [Derive changesets between maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-derive.asciidoc) and [push](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-apply.asciidoc) through the OpenStreetMap API
* [Detect spoken languages](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LanguageTranslation.asciidoc) in a map's tag data (experimental)
* [Explore tag data](https://github.com/ngageoint/hootenanny/blob/master/docs/user/TagExploration.asciidoc)
* [Filter a subset of features](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureFiltering.asciidoc) before conflation based on tag content and schema relationships
* [Find street intersections by name](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#find-street-intersection-locations-in-a-map-by-street-name)
* [Generate a task grid](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/task-grid.asciidoc)
* Generate [pre-configured](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#display-a-set-of-statistics-for-a-map) or [custom](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#calculate-the-area-of-all-features-in-a-map) map statistics
* [Grab a random section of data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/crop-random.asciidoc) from a map for testing against
* [Identify street intersection locations](https://github.com/ngageoint/hootenanny/blob/master/docs/user/IdentifyRoadIntersections.asciidoc) in a map
* [Locate phone numbers](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LocatePhoneNumbers.asciidoc) geographically
* [Replace or remove data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-derive.asciidoc) completely within a bounding box via changeset and [push](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-apply.asciidoc) the changes through the OpenStreetMap API
* [Perturb map data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/perturb.asciidoc) for testing purposes
* [Plot node density](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/plot-node-density.asciidoc)
* [Process map postal address data](https://github.com/ngageoint/hootenanny/blob/master/docs/user/PostalAddressUtilities.asciidoc)
* [Simplify ways](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#simplify-ways-in-a-map-by-removing-unnecessary-nodes-from-them) by removing unnecessary nodes
* [Snap unconnected ways](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SnapUnconnectedWays.asciidoc) to neighboring ways
* [Sort map data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/sort.asciidoc)
* [Split](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#split-a-map-into-multiple-maps-corresponding-to-irregular-shaped-tiles-produced-above) a map into tiles
* [Translate feature tags](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc#translations) using standardized or user defined [schemas](https://github.com/ngageoint/hootenanny#tag-schemas)
* [Translate feature tags to English](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LanguageTranslation.asciidoc) (experimental)
* [Validate features](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureValidation.asciidoc)

# [Supported Data Formats](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc)

# Scalability
Hootenanny currently does not strive to conflate data at the global level. An earlier implementation of Hootenanny supported a map-reduce 
architecture that was capable of global conflation for some data types but was shelved due to general lack of interest and the maintenance 
costs to support the seldomly used capability. So, some of the conflation algorithms are actually capable of supporting distributed 
computing, if you want to try and go that route and revive the capability. However, such capabilities are likely out of date with the rest of the codebase and may have limitations as far as the feature types they can conflate. 

Hootenanny generally can scale well running on a single machine from the larger city level up to the smaller country level, depending on the 
density of the data being conflated and the RAM available on the machine. Beyond that, new conflation algorithms and/or parallelization of
existing algorithms would need to be developed to handle very large quantities of map data.

# Documentation
* [Included as a PDF with each release](https://github.com/ngageoint/hootenanny/releases)
* Online:
  * [User Interface Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyUI.asciidoc)
  * [User Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyUserGuide.asciidoc)
  * [Algorithms Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyAlgorithms.asciidoc)
  * [Algorithm Detail Slides](https://github.com/ngageoint/hootenanny/wiki/Conflation-Algorithm-Slides)
  * [Developer's Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyDeveloperGuide.asciidoc)
* On the Hootenanny VM look for the PDF documentation included in 'hoot/docs' 
* [Instructions](https://github.com/ngageoint/hootenanny/blob/master/docs/user/VAGRANT.md) to launch a virtual machine
* [Instructions](https://github.com/ngageoint/hootenanny/blob/master/docs/install/HootenannyInstall.asciidoc) for an RPM based installation
* [CLI](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/HootCommandLineReference.asciidoc) and [examples](https://github.com/ngageoint/hootenanny/tree/master/docs/user/CommandLineExamples.asciidoc)

# Support
* [FAQ](https://github.com/ngageoint/hootenanny/wiki/Frequently-Asked-Questions)

Don't hesitate to ask for help if features aren't conflating how you expect them to or if you're experiencing difficulty while installing the 
software. We officially support installing RPMs to CentOS and launching CentOS virtual machines...anything else we'll do our best to help you out, but there are no guarantees. If you have any support questions, please create an issue in this repository.

As there are lot of different data scenarios out in the wild, there is no one-size fits all conflation workflow or algorithm. Hootenanny 
attempts to provide a useful set of conflation workflows with pre-configured options that capture most conflation requirements. However, you
may require a new conflation scenario for your purposes (let us know), or you may need to custom modify configuration options for an existing scenario in order to get the best results.

Also, the availability of new software features to the user interface may lag their initial availability from the command line interface (CLI)
by one or more development cycles. If you find a conflation feature you wish to use that is mentioned in the CLI documentation but is not 
present within the UI, let us know.

# Development
* [Contributing](https://github.com/ngageoint/hootenanny/blob/master/CONTRIBUTING.md)
* [Developer's Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyDeveloperGuide.asciidoc)

# [Redistribution](https://github.com/ngageoint/hootenanny/blob/master/docs/Redistribution.md)
