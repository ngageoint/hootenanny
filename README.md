![hoot_logo](https://github.com/ngageoint/hootenanny-ui/blob/develop/dist/img/logo/hoot_logo_dark.png)

![](https://github.com/ngageoint/hootenanny/blob/master/docs/user/images/id/hoot_conflation_new.png)

_Hootenanny_: 

1. A gathering at which folksingers entertain often with the audience joining in.

_Conflation_: 

1. Fancy word for merge

# [Overview](https://github.com/ngageoint/hootenanny/blob/master/docs/overview/Introduction.asciidoc)

Hootenanny is an open source map data conflation tool developed with machine learning techniques to facilitate automated and semi-automated conflation of critical Foundation GEOINT features in the topographic domain. **In short, it merges multiple maps into a single seamless map.**

Goals:

* Automatically combine geospatial features for decision making
* Allow for reviewing and manually resolving features which cannot be automatically matched with sufficient certainty
* Maintain geometry and attribute provenance for combined features
* Create up-to-date routable transportation networks from multiple sources

# Documentation

[pdf](https://github.com/ngageoint/hootenanny/releases/download/v0.2.78/HootenannyUserGuide.pdf)

This documentation is also available as part of the release package: `docs/HootenannyUserGuide.pdf` (located in `hootenanny-*.tar.gz`).

There is also an introduction [video](https://www.youtube.com/watch?v=LeaTLxVCFmc) of a talk from State of the Map US 2019.

# [Releases](https://github.com/ngageoint/hootenanny/releases)

For installation from RPM, follow the [guide in the rpms repo](https://github.com/ngageoint/hootenanny-rpms/blob/master/docs/install.md). If you want to install from source without downloading the release, follow [this](https://github.com/ngageoint/hootenanny/blob/master/docs/install/Vagrant.asciidoc).

For installation using Docker, [follow these instructions](https://github.com/ngageoint/hootenanny/blob/master/Docker.md).

# Conflatable Feature Types

Hootenanny has specifically tailored conflation algorithms available for the following data types:
* [Areas](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SupportedFeatureTypes.asciidoc#areas)
* [Buildings](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SupportedFeatureTypes.asciidoc#buildings)
* [Points of Interest (POIs)](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SupportedFeatureTypes.asciidoc#pois)
* [Power Lines](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SupportedFeatureTypes.asciidoc#power-lines)
* [Railways](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SupportedFeatureTypes.asciidoc#railways)
* [Rivers](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SupportedFeatureTypes.asciidoc#rivers)
* [Roads](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SupportedFeatureTypes.asciidoc#roads)

Any feature whose type does not fit into the list above or has no type at all, will be conflated with 
[Generic Geometry Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SupportedFeatureTypes.asciidoc#generic-geometry-features), which uses a simpler approach than the type specific conflation algorithms.

You can create your own custom conflation algorithms for additional feature types via [Javascript](https://github.com/ngageoint/hootenanny/blob/master/docs/JavascriptOverview.asciidoc) or [C++](https://github.com/ngageoint/hootenanny/blob/master/hoot-core/src/main/cpp/hoot/core/conflate/matching/MatchCreator.h). 

# Conflation Workflows

A conflation workflow defines the manner in which two maps are merged together. Hootenanny has [these workflows](https://github.com/ngageoint/hootenanny/blob/master/docs/user/ConflationSteps.asciidoc#select-a-conflation-workflow) available.
  
# [Attribute Translation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Translation.asciidoc)

Hootenanny leverages the OSM key value pair tag concept to support translation between various data schemas and supports the following schemas: 
* Topographic Data Store (TDS) 
* Multi-National Geospatial Co-Production Program (MGCP)
* Geonames
* OSM 
* [others](https://github.com/ngageoint/hootenanny/blob/master/translations)

Users can define their own [custom schema translations](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc#translations) via Javascript or Python.

# [Feature Filtering](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureFiltering.asciidoc)

Hootenanny has the capability to let you selectively pick the features that are conflated from your data to save you from pre-conflation data wrangling. 

Some examples:
* [Conflate only buildings](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#conflate-only-buildings)
* [Conflate only restaurant buildings](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#conflate-only-restaurant-buildings)
* [Conflate only restaurant buildings with "Subway" in the name](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#conflate-only-restaurant-buildings-with-subway-in-the-name)

Hootenanny has several [available filters](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#list-all-entities-that-can-operate-on-data) that may be used to perform additional types of feature filtering during conflation.

# [Feature Validation and Cleaning](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureValidationAndCleaning.asciidoc)

Hootenanny has limited data validation reporting of its own, but JOSM validation routines may be invoked via Hootenanny during batch processing of data.

Hootenanny has a variety of map cleaning capabilities to automatically correct erroneous data. Some of them are invoked automatically during a conflation job, however, you may also [run them separately](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/clean.asciidoc). In addition to its own built-in cleaning operations, Hootenanny is also integrated with the auto-fix capabilities of [JOSM](https://josm.openstreetmap.de/). 

# Configuration

Although Hootenanny is configured by default to provide the best conflation results, at times you will need to tweak its wide range of [configuration options](https://github.com/ngageoint/hootenanny/blob/master/conf/core/ConfigOptions.asciidoc) in order to get the best conflated result for your input data.

# Web User Interface

[Hootenanny's web user interface](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc) is built upon the open source 
[Mapbox iD Editor](https://github.com/openstreetmap/iD), which provides an intuitive and user-friendly conflation experience.

# Web Services API

Access to Hootenanny capabilities are exposed through a web services API for those wishing to develop their own conflation clients. The web services use [OAuth](https://github.com/ngageoint/hootenanny/blob/master/developer/HootenannySecurity.asciidoc) authentication.

# Command Line Interface

Basic conflation example:

```
#  conflate two datasets together
hoot conflate input1.osm input2.osm output.osm
```

[More examples](https://github.com/ngageoint/hootenanny/tree/master/docs/user/CommandLineExamples.asciidoc)

# Programming Language Bindings

Hootenanny has [nodejs bindings](https://github.com/ngageoint/hootenanny/blob/master/docs/JavascriptOverview.asciidoc) available which expose 
core conflation capabilities for creating custom workflows. 

Basic conflation example:

```
//  conflate two datasets together
var hoot = require(process.env.HOOT_HOME + '/lib/HootJs');
var map = new hoot.OsmMap();
hoot.loadMap(map, "input1.osm", false, 1);
hoot.loadMap(map, "input2.osm", false, 2);
new hoot.UnifyingConflator().apply(map)
hoot.saveMap(map, "output.osm");
```

# [Additional Feature Summary](https://github.com/ngageoint/hootenanny/tree/master/docs/user/AdditionalFeatureSummary.asciidoc)

# [Supported Data Formats](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc)

# [Scalability](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Scalability.asciidoc)

# [Support](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Support.asciidoc)

* [FAQ](https://github.com/ngageoint/hootenanny/wiki/Frequently-Asked-Questions)

# [Contributing](https://github.com/ngageoint/hootenanny/blob/master/CONTRIBUTING.md)

# [Redistribution](https://github.com/ngageoint/hootenanny/blob/master/docs/Redistribution.md)
