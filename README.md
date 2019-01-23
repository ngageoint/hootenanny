![hoot_logo](https://github.com/ngageoint/hootenanny-ui/blob/develop/dist/img/logo/hoot_logo_dark.png)

![](https://github.com/ngageoint/hootenanny/blob/master/docs/user/images/id/hoot_conflation_new.png)

# Overview
_Hootenanny_: 

1. a gathering at which folksingers entertain often with the audience joining in

_Conflation_: 

1. Fancy word for merge

Hootenanny is an open source conflation tool developed with machine learning techniques to facilitate automated and semi-automated conflation of critical Foundation GEOINT features in the topographic domain.  In short, it merges multiple maps into a single seamless map.

Hootenanny conflation occurs at the dataset level, where the user’s workflow determines the best reference dataset, source content, geometry, 
and attributes to transfer to the output map.  Hootenanny's internal processing leverages the key value pair structure of OpenStreetMap (OSM) 
for improved utility and applicability to broader user groups.  Normalized attributes can be used to aid in feature matching, and OSM’s 
free tagging system allows the map to include an unlimited number of attributes describing each feature.

Hootenanny is developed under the open source General Public License (GPL) and maintained on the National Geospatial-Intelligence 
Agency’s (NGA) GitHub [site](https://github.com/ngageoint/hootenanny). 

# Project Goals
* Automatically combine geospatial features for decision making
* Allow for reviewing and manually resolving features which cannot be automatically matched with sufficient certainty
* Maintain geometry and attribute provenance for combined features
* Create up-to-date routable transportation networks from multiple sources

# Conflatable Feature Types
* Area polygons
* Building polygons
* Points of Interest (POIs)
* Transportation polylines (roads and railways)
* Utility polylines (power lines)
* Waterway polylines

Additional feature types can be made conflatable using Hootenanny's pluggable conflation architecture.  See the Hootenanny Developer Guide for details.

# Feature Summary
In addition to conflating maps together, Hootenanny can also:
* Add missing type tags to features
* Align two maps with each other
* Apply data transformation operations to a map
* Calculate the geospatial extent of a map
* Clean map data
* Compare maps with each other
* Compute bounding tiles based on node density
* Convert maps between different geodata formats (see Supported Data Formats section below)
* Crop a map to a geospatial extent
* Derive changesets between maps and apply the changesets to external OSM data stores
* Detect spoken languages in a map's tag data
* Explore tag data
* Filter features based on tag content and schema relationships
* Gather statistics from a map
* Identify road intersections in a map
* Locate phone numbers geographically
* Perturb map data for testing purposes
* Plot node density
* Sort map data
* Translate feature tags using user defined schemas
* Translate feature tags to English

# Documentation
User and technical documentation may be found locally after installation in 'hoot/docs' or 
[included with each release](https://github.com/ngageoint/hootenanny/releases). 

[FAQ](https://github.com/ngageoint/hootenanny/wiki/Frequently-Asked-Questions)

If you have any support questions please create an issue in the [Hootenanny GitHub repository](https://github.com/ngageoint/hootenanny).

# Installation
Hootenanny is supported on Red Hat/CentOS:

[Instructions](https://github.com/ngageoint/hootenanny/blob/master/VAGRANT.md) to launch a Hootenanny CentOS virtual machine

[Instructions](https://github.com/ngageoint/hootenanny/blob/master/docs/install/HootenannyInstall.asciidoc) for an RPM based installation 
to CentOS 7.x.

# Web User Interface
[Hootenanny's web user interface](https://github.com/ngageoint/hootenanny-ui) is built upon the open source 
[Mapbox iD Editor](https://github.com/openstreetmap/iD), which provides an intuitive and user-friendly conflation experience.  See the User Interface Guide for more details. 

# Web Services API
Access to Hootenanny core capabilities are exposed through a web services API for those wishing to develop their own conflation clients.  The API documentation can be found in the release at: docs/hootenanny-services-$(HOOT_VERSION)-javadoc.tar.gz.

# Command Line Interface
Command line access is available and exposes additional functionalities not available from the web user interface.  See the "Usage" section below for examples, as well as the Command Line Documentation section of the User Documentation.

# Programming Language Bindings
Hootenanny has [nodejs bindings](https://github.com/ngageoint/hootenanny/blob/master/docs/user/JavascriptOverview.asciidoc) available which expose core conflation capabilities.

# Supported Data Formats
See the Hootenanny User Guide [Supported Data Formats section](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc).

# Tag Schemas
Hootenanny leverages the OSM key value pair tag concept to support translation between various data schemas.  By default, Hootenanny 
supports automated schema conversion between: 
* Topographic Data Store (TDS) v6.1/v4.0 
* Multi-National Geospatial Co-Production Program (MGCP)
* Geonames
* OSM 
* others (see "translations" folder)

Users are also able to define their own custom translations.  For custom translations, a specific mapping can be defined based on an 
uploaded dataset using a semi-automated Translation Assistant.  More details on the translation capabilities of Hootenanny can be 
found in Hootenanny User Guide, as well as the Hootenanny User Interface Guide. 

# Usage
See the Hootenanny User Guide [Command Line Examples section](https://github.com/ngageoint/hootenanny/tree/master/docs/user/CommandLineExamples.asciidoc).

# Redistribution
Hootenanny was developed at the National Geospatial-Intelligence Agency (NGA) in collaboration with DigitalGlobe.  The government has 
"unlimited rights" and is releasing this software to increase the impact of government instruments by providing developers with the 
opportunity to take things in new directions. The software use, modification, and distribution rights are stipulated within the GNU 
General Public License. The GPL license is available in LICENSE.txt

All pull requests contributions to this project will be released under the GNU General Public License 3.0. Software source code previously 
released under an open source license and then modified by NGA staff is considered a "joint work" (see 17 USC 101); it is partially 
copyrighted, partially public domain, and as a whole is protected by the copyrights of the non-government authors and must be released 
according to the terms of the original open source license.

Licensed under the GNU General Public License v3.0 (the "License"); you may not use this file except in compliance with the License. You 
may obtain a copy of the License at http://www.gnu.org/copyleft/gpl.html.

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions 
and limitations under the License.

Imagery provided by permission from DigitalGlobe. Users are responsible for complying with terms of use for data and imagery they use in 
conjunction with Hootenanny. Specifically, the must properly protect and comply with all legal, copyright, and licensing terms.

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published 
by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
