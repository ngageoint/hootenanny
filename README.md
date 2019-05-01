![hoot_logo](https://github.com/ngageoint/hootenanny-ui/blob/develop/dist/img/logo/hoot_logo_dark.png)

![](https://github.com/ngageoint/hootenanny/blob/master/docs/user/images/id/hoot_conflation_new.png)

# Overview
_Hootenanny_: 

1. A gathering at which folksingers entertain often with the audience joining in

_Conflation_: 

1. Fancy word for merge

[Hootenanny](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc) is an open source conflation tool developed with machine learning techniques to facilitate automated and semi-automated conflation of critical Foundation GEOINT features in the topographic domain.  In short, it merges multiple maps into a single seamless map.

Hootenanny conflation occurs at the dataset level, where the user’s workflow determines the best reference dataset, source content, geometry, and attributes to transfer to the output map.  Hootenanny's internal processing leverages the key value pair structure of OpenStreetMap (OSM) for improved utility and applicability to broader user groups.  Normalized attributes can be used to aid in feature matching, and OSM’s free tagging system allows the map to include an unlimited number of attributes describing each feature.

Hootenanny is developed under the open source General Public License (GPL) and maintained on the National Geospatial-Intelligence Agency’s (NGA) GitHub [site](https://github.com/ngageoint/hootenanny). 

# Goals
* Automatically combine geospatial features for decision making
* Allow for reviewing and manually resolving features which cannot be automatically matched with sufficient certainty
* Maintain geometry and attribute provenance for combined features
* Create up-to-date routable transportation networks from multiple sources

# Types of [Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc)
* **[Attribute Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/AttributeConflation.asciidoc)** - Conflate map A with B where only tags are transferred from B to A and few or 
no changes are made to A's geometry.
* **[Differential Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/DifferentialConflation.asciidoc)** - Conflate map A with B where the only data added to the output from B is in areas that don't overlap with A.  Optionally, you can configure to overwrite tags in A from B even when there is overlap or to output the unmodified map A data as well.
* **[Horizontal Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/cookie-cut.asciidoc)** (aka Cookie Cutter Conflation) - Define a specific region in map A that should not be modified and stitch in data from map B around it.
* **[Reference Conflation (default)](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc)** - Conflate the best geometry and tag parts of map B into map A, favoring map A's data.

There are a wide range of [configuration options](https://github.com/ngageoint/hootenanny/blob/master/conf/core/ConfigOptions.asciidoc) 
available to customize the conflation workflows.

# Conflatable Feature Types
* Area polygons
* Building polygons
* Points of Interest (POIs)
* Transportation polylines (roads and railways)
* Utility polylines (power lines)
* Waterway polylines

Additional feature types can be made conflatable via custom script by using Hootenanny's [Generic Conflation capability](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).

# Conflation Algorithms
* [Area](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#area-to-area-conflation) ([Detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/AreaToAreaConflation.asciidoc)) - Area Conflation uses a machine learning based approach implemented with [Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).
* [Building](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#building-conflation) ([Detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/BuildingConflation.asciidoc)) - Building Conflation uses a machine learning based approach.
* [POI](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#poi-conflation) ([Detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/PoiToPoiConflation.asciidoc)) - POI Conflation uses a machine learning based approach implemented with [Generic Point Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).
* [POI to Polygon](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#poi-to-polygon-conflation) ([Detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/PoiToPolygonConflation.asciidoc)) - POI to Polygon Conflation uses a rules based approach. 
* [Power Line](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/GenericLineConflation.asciidoc#power-line-conflation) ([Detail](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#power-line-conflation)) - Power Line Conflation uses a machine learning based approach implemented with [Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).
* Railway - Railway Conflation uses a machine learning based approach implemented with [Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).
* [Road](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/RoadConflation.asciidoc#road-conflation)
  * [2nd Generation (aka Unifying)](https://github.com/ngageoint/hootenanny/blob/develop/docs/algorithms/UnifyingConflation.asciidoc) - This algorithm uses machine learning techniques based on manually matched training data to match roads.
  * [Network](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/NetworkConflation.asciidoc) - This algorithm uses a graph theory based approach to match roads.
* [Waterway](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/GenericLineConflation.asciidoc#river-conflation) ([Detail](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#river-conflation)) - Waterway Conflation uses a machine learning based approach implemented with [Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).

# Tag Schemas
Hootenanny leverages the OSM key value pair tag concept to support translation between various data schemas and supports automated schema conversion between: 
* Topographic Data Store (TDS) v6.1/v4.0 
* Multi-National Geospatial Co-Production Program (MGCP)
* Geonames
* OSM 
* [others](https://github.com/ngageoint/hootenanny/tree/master/translations)

Users are also able to define their own [custom translations](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc#translations).

# Additional Feature Summary
In addition to conflating maps together, Hootenanny can also:
* Add [missing type tags](https://github.com/ngageoint/hootenanny/blob/master/docs/user/ImplicitTypeTagging.asciidoc) to features
* [Align](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/rubber-sheet.asciidoc) two maps with each other
* Apply [data transformation](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/convert.asciidoc) operations to a map
* Calculate the [geospatial extent](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/extent.asciidoc) of a map
* [Clean](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/clean.asciidoc) map data
* [Compare](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/diff.asciidoc) maps with each other
* Compute [bounding tiles](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/node-density-tiles.asciidoc) based on node density
* [Convert](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/convert.asciidoc) maps between [different geodata formats](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc)
* [Crop](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/crop.asciidoc) a map to a geospatial extent
* [Derive](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-derive.asciidoc) large changesets between maps and [apply](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/changeset-apply.asciidoc) the changesets to external OSM data stores with automatic conflict resolution
* [Detect spoken languages](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LanguageTranslation.asciidoc) in a map's tag data
* [Explore tag data](https://github.com/ngageoint/hootenanny/blob/master/docs/user/TagExploration.asciidoc)
* [Filter](https://github.com/ngageoint/hootenanny/blob/master/docs/user/FeatureFiltering.asciidoc) features based on tag content and schema relationships
* [Filter](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/convert.asciidoc) features by bounding box
* Gather [statistics](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/stats.asciidoc) from a map
* [Identify](https://github.com/ngageoint/hootenanny/blob/master/docs/user/IdentifyRoadIntersections.asciidoc) road intersections in a map
* [Locate](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LocatePhoneNumbers.asciidoc) phone numbers geographically
* [Perturb](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/perty.asciidoc) map data for testing purposes
* Plot [node density](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/node-density-plot.asciidoc)
* [Snap](https://github.com/ngageoint/hootenanny/blob/master/docs/user/SnapUnconnectedWays.asciidoc) unconnected roads to neighboring roads
* [Sort](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/sort.asciidoc) map data
* Translate feature tags using standardized or user defined [schemas](https://github.com/ngageoint/hootenanny#tag-schemas)
* [Translate](https://github.com/ngageoint/hootenanny/blob/master/docs/user/LanguageTranslation.asciidoc) feature tags to English

# [Supported Data Formats](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc)

# Web User Interface
[Hootenanny's](https://github.com/ngageoint/hootenanny-ui) [web user interface](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc) is built upon the open source 
[Mapbox iD Editor](https://github.com/openstreetmap/iD), which provides an intuitive and user-friendly conflation experience.

# Web Services API
Access to Hootenanny core capabilities are exposed through a web services API for those wishing to develop their own conflation clients. 

The web services use [OAuth](https://github.com/ngageoint/hootenanny/tree/master/docs/developer/OAUTH.md) authentication.

# Command Line Interface
[Command line access](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/HootCommandLineReference.asciidoc) is available and exposes additional functionalities not available from the web user interface.  

[Examples](https://github.com/ngageoint/hootenanny/tree/master/docs/user/CommandLineExamples.asciidoc)

# Programming Language Bindings
Hootenanny has [nodejs bindings](https://github.com/ngageoint/hootenanny/blob/master/docs/JavascriptOverview.asciidoc) available which expose core conflation capabilities for creating custom workflows. 

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
* If you have any support questions, please create an issue in this repository.

# [Contributing](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/CONTRIBUTING.md)

# Redistribution
Hootenanny was developed at the National Geospatial-Intelligence Agency (NGA) in collaboration with DigitalGlobe.  The government has "unlimited rights" and is releasing this software to increase the impact of government instruments by providing developers with the opportunity to take things in new directions. The software use, modification, and distribution rights are stipulated within the GNU General Public License. The GPL license is available in LICENSE.txt

All pull requests contributions to this project will be released under the GNU General Public License 3.0. Software source code previously released under an open source license and then modified by NGA staff is considered a "joint work" (see 17 USC 101); it is partially copyrighted, partially public domain, and as a whole is protected by the copyrights of the non-government authors and must be released according to the terms of the original open source license.

Licensed under the GNU General Public License v3.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.gnu.org/copyleft/gpl.html.

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Imagery provided by permission from DigitalGlobe. Users are responsible for complying with terms of use for data and imagery they use in conjunction with Hootenanny. Specifically, the must properly protect and comply with all legal, copyright, and licensing terms.

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
