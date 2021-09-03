![hoot_logo](https://github.com/ngageoint/hootenanny-ui/blob/develop/dist/img/logo/hoot_logo_dark.png)

![](https://github.com/ngageoint/hootenanny/blob/master/docs/user/images/id/hoot_conflation_new.png)

_Hootenanny_: 

1. A gathering at which folksingers entertain often with the audience joining in

_Conflation_: 

1. Fancy word for merge

# Overview

[Hootenanny](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc) is an open source map data conflation tool developed with machine learning techniques to facilitate automated and semi-automated conflation of critical Foundation GEOINT features in the topographic domain. **In short, it merges multiple maps into a single seamless map.**

Goals:

* Automatically combine geospatial features for decision making
* Allow for reviewing and manually resolving features which cannot be automatically matched with sufficient certainty
* Maintain geometry and attribute provenance for combined features
* Create up-to-date routable transportation networks from multiple sources

# [Detail Summary](https://github.com/ngageoint/hootenanny/blob/master/docs/user/DetailSummary.asciidoc)

# [Releases](https://github.com/ngageoint/hootenanny/releases)

For installation from RPM, please follow the [guide in the rpms repo](https://github.com/ngageoint/hootenanny-rpms/blob/master/docs/install.md). If you want to install from source without downloading the release, please follow [VAGRANT.md](https://github.com/ngageoint/hootenanny/blob/master/docs/user/VAGRANT.md).

Documentation is included as [PDF files](https://github.com/ngageoint/hootenanny/releases) with each release. Download 
the Hootenanny `.tar.gz` installation file and locate the documentation within the "docs" directory.

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
[Generic Geometry Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#generic-geometry), which uses a 
simpler rules based approach than the type specific conflation algorithms.

[Conflation Algorithms Detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/ConflationAlgsOverview.md)

You can create your own custom conflation algorithms for additional feature types via [Javascript](https://github.com/ngageoint/hootenanny/blob/master/docs/JavascriptOverview.asciidoc) or [C++](https://github.com/ngageoint/hootenanny/blob/master/hoot-core/src/main/cpp/hoot/core/conflate/matching/MatchCreator.h). 

# [Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc) Workflows

A conflation workflow defines the manner in which two maps are merged together. Hootenanny has the following workflows:
* **[Reference Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc#conflation-strategies)** (**default**; aka Vertical Conflation) - _Keep the best of both maps while favoring the first_
  * Use this type of conflation when you want map output based on the best state of two maps while favoring the first one.
* **[Average Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc#average-conflation)** - _Keep an average of both maps_ 
  * Use this type of conflation when you consider both input maps equal in quality and want a result that is an average of the two.
  * Currently, geometry averaging only applies to linear features but could be extended to point and polygon geometries. Point and polygon geometries are merged the same as in Reference Conflation. 
  * Average Conflation is currently not available from iD Editor.
* **[Horizontal Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc#horizontal-conflation)** (aka Cookie Cutter Conflation) - _Completely replace a section_
  * Use this type of conflation if you have a specific region of your map that you would like to completely replace with a region from another map.
* **[Differential Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/DifferentialConflation.asciidoc)** - _Add new features that do not conflict_
  * Use this type of conflation when you want to fill holes in your map with data from another source without actually modifying any of the data in your map.
  * There is an option available to additionally transfer tags to existing features in your map from matching features in another map where overlap occurs.
* **[Attribute Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/AttributeConflation.asciidoc)** - _Transfer attributes over to existing geometries_
  * Use this type of conflation when one map's geometry is superior to that of a second map, but the attributes of the second map are superior to that of the first map.
  
# [Attribute Translation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#translation)

Hootenanny leverages the OSM key value pair tag concept to support translation between various data schemas and supports the following schemas: 
* Topographic Data Store (TDS) 
* Multi-National Geospatial Co-Production Program (MGCP)
* Geonames
* OSM 
* [others](https://github.com/ngageoint/hootenanny/tree/master/translations)

Users can define their own [custom schema translations](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc#translations) via Javascript or Python.

# [Feature Filtering](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureFiltering.asciidoc)

Hootenanny has the capability to let you selectively pick the features that are conflated from your data to save you from pre-conflation data wrangling. 

Some examples:
* [Conflate only buildings](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#conflate-only-buildings)
* [Conflate only restaurant buildings](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#conflate-only-restaurant-buildings)
* [Conflate only restaurant buildings with "Subway" in the name](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#conflate-only-restaurant-buildings-with-subway-in-the-name)

Hootenanny has several [available filters](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#list-all-entities-that-can-operate-on-data) that may be used to perform feature filtering during conflation.

# [Feature Validation and Cleaning](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureValidationAndCleaning.asciidoc)

Hootenanny has limited data validation reporting of its own, and JOSM validation routines may also be invoked via Hootenanny during batch processing of data.

Hootenanny has a variety of map cleaning capabilities to automatically correct erroneous data. Some of them are invoked automatically during a conflation job, however, you may also [run them separately](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/clean.asciidoc). In addition to its own built-in cleaning operations, Hootenanny is also integrated with the auto-fix capabilities of [JOSM](https://josm.openstreetmap.de/). 

**NOTE:** As of 3/19/20 the cleaning/validation integration with JOSM is only available in the Vagrant version of Hootenanny. It may be added to the RPM version of Hootenanny at a later date.

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

# [Command Line Interface](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/HootCommandLineReference.asciidoc)

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

# Feature Summary

In addition to conflating map data, Hootenanny also provides these supporting capabilities that may aid conflation:
* [Add missing type tags](https://github.com/ngageoint/hootenanny/blob/master/docs/user/ImplicitTypeTagging.asciidoc) to features implicitly based on their name (experimental)
* [Align](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/align.asciidoc) the geometries of two maps more closely with each other
* [Apply data transformation operations](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/convert.asciidoc#conversion-operations) to a map
* [Calculate the geospatial extent](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/extent.asciidoc) of a map
* [Clean map data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/clean.asciidoc)
* [Compare maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/diff.asciidoc) with each other
* [Convert maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/convert.asciidoc) between [different geodata formats](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc)
* [Create a covering shape](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/alpha-shape.asciidoc) that minimally covers all features in a map
* [Crop a map](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/crop.asciidoc) to a geospatial extent
* [De-duplicate features](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/de-duplicate.asciidoc) within a single map or across two maps or simply [find duplicate features](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/de-duplicate.asciidoc) in a map (experimental)
* [Derive changesets between maps](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-derive.asciidoc) and [push](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-apply.asciidoc) through the OpenStreetMap API
* [Detect spoken languages](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LanguageTranslation.asciidoc) in a map's tag data (experimental)
* [Explore tag data](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#tag-info)
* [Filter a subset of features](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureFiltering.asciidoc) before conflation based on tag content and schema relationships
* [Find street intersections by name](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#find-street-intersection-locations-in-a-map-by-street-name)
* [Generate a task grid](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/task-grid.asciidoc)
* Generate [pre-configured](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/stats.asciidoc) or [custom](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/stat.asciidoc) map statistics
* [Grab a random section of data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/crop-random.asciidoc) from a map for testing against (experimental)
* [Identify street intersection locations](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#detect-road-intersections) in a map
* [Locate phone numbers](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#add-admin-boundary-level-location-tags-associated-with-element-phone-numbers) geographically
* [Replace or remove data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-derive.asciidoc) completely within a bounding box via changeset and [push](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-apply.asciidoc) the changes through the OpenStreetMap API
* [Perturb map data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/perturb.asciidoc) for testing purposes
* [Plot node density](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/plot-node-density.asciidoc)
* [Process map postal address data](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#count-the-number-of-elements-with-valid-address-tags-in-a-map)
* [Simplify ways](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#simplify-ways-in-a-map-by-removing-unnecessary-nodes-from-them) by removing unnecessary nodes
* [Snap unconnected ways](https://github.com/ngageoint/hootenanny/blob/master/docs/user/CommandLineExamples.asciidoc#snap-unconnected-roads-in-one-dataset-back-to-neighboring-roads-in-another-dataset) to neighboring ways
* [Sort map data](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/sort.asciidoc)
* [Split](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/split.asciidoc) a map into tiles
* [Translate feature tags](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#translation) using standardized or user defined [schemas](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc#TranslationAssistant)
* [Translate feature tags to English](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LanguageTranslation.asciidoc) (experimental)
* [Validate features](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureValidationAndCleaning.asciidoc)

# [Supported Data Formats](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc)

# [Scalability](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Scalability.asciidoc)

# [Support](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Support.asciidoc)

* [FAQ](https://github.com/ngageoint/hootenanny/wiki/Frequently-Asked-Questions)

# [Contributing](https://github.com/ngageoint/hootenanny/blob/master/CONTRIBUTING.md)

# [Redistribution](https://github.com/ngageoint/hootenanny/blob/master/docs/Redistribution.md)
