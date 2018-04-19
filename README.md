![hoot_logo](https://github.com/ngageoint/hootenanny-ui/blob/develop/dist/img/logo/hoot_logo_dark.png)

![](https://github.com/ngageoint/hootenanny/blob/master/docs/user/images/id/hoot_conflation_new.png)

# Introduction
_Hootenanny_: 

1. a gathering at which folksingers entertain often with the audience joining in

_Conflation_: 

1. Fancy word for merge

Hootenanny is an open source conflation tool developed to facilitate automated and semi-automated conflation of critical Foundation 
GEOINT features in the topographic domain.  In short, it merges multiple maps of geodata into a single seamless map.

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

# Installation
Hootenanny is supported on Red Hat/CentOS:

[Instructions](https://github.com/ngageoint/hootenanny/blob/master/VAGRANT.md) to launch a Hootenanny CentOS virtual machine

[Instructions](https://github.com/ngageoint/hootenanny/blob/master/docs/install/HootenannyInstall.asciidoc) for an RPM based installation to 
CentOS 7.x.

# Documentation
User and technical documentation may be found locally after installation in 'hoot/docs' or 
[included with each release](https://github.com/ngageoint/hootenanny/releases). 

[FAQ](https://github.com/ngageoint/hootenanny/wiki/Frequently-Asked-Questions)

If you have any support questions please create an issue in the [Hootenanny GitHub repository](https://github.com/ngageoint/hootenanny).

# Web User Interface
[Hootenanny's web user interface](https://github.com/ngageoint/hootenanny-ui) is built upon the open source 
[Mapbox iD Editor](https://github.com/openstreetmap/iD), which provides an intuitive and user-friendly conflation experience. 

# Command Line User Interface
A command line interface to conflation capabilities is available for users not requiring the web user interface.  It can be accessed by 
typing 'hoot' at the command line in the Hootenanny environment.  More detail is provided 
[here](https://github.com/ngageoint/hootenanny/blob/master/VAGRANT.md).

# Supported Geodata Types
**Hootenanny can conflate:**
* Area polygons
* Building polygons
* Points of Interest (POIs)
* Transportation polylines
* Waterway polylines

# Supported Data Formats
**Hootenanny can import from:**
* ESRI File Geodatabase (.gdb)
* GeoJSON (.geojson) (M)
* geonames.org (.geonames)
* Hootenanny API Database (hootapidb://)
* JSON file (.json; similar to Overpass JSON) (M)
* OpenStreetMap XML (.osm)
* OpenStreetMap Protocol Buffers (.osm.pbf)
* OpenStreetMap API Database (osmapidb://)
* Shapefile (.shp)
* Zip files containing shapefiles and/or ESRI File Geodatabase files (.zip)
* OGR supported formats

**Hootenanny can export to:** 
* ESRI File Geodatabase (.gdb)
* GeoJSON (.geojson) (M)
* Hootenanny API Database (hootapidb://)
* JSON file (.json; similar to Overpass JSON) (M)
* OpenStreetMap XML file (.osm) (*)
* OpenStreetMap Protocol Buffers file (.osm.pbf)
* OpenStreetMap API Database (osmapidb://)
* Shapefile (.shp) (M)
* OGR supported formats

**Hootenanny can export changesets to:** 
* OpenStreetMap XML changeset file (.osc) (M)
* OpenStreetMap SQL changeset file (.osc.sql) (M)

Notes:
* (M) = format requires reading entire dataset into memory during processing
* (*) = format requires reading entire dataset into memory during processing only if element ID output is sorted
* All data read with a specified bounding box filter requires reading the entire dataset into memory during processing.

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

# Data Cleaning
Hootenanny supports a variety of geometric and tag based data cleaning operations.  Hootenanny can also add missing tags to POI data 
implicitly based on a feature's name.  See the documentation for more details.

# Contributing
Please read the Hootenanny Developer's Guide for details on setting up an environment, coding standards, and development process.  Hootenanny 
developers use a customization of the [Gitflow workflow](https://www.atlassian.com/git/tutorials/comparing-workflows#gitflow-workflow).
## Workflow Summary
* Open a repository issue for the new feature to be worked on.
* Perform work for the feature on a new git feature branch named as the number of the issue opened.
* Open a pull request and assign at least one reviewer to merge the feature branch into the "develop" branch mainline when the 
feature is complete.

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

# Developers
In alphabetical order (past and present):
* Justin Bronn (justin.bronn@radiantsolutions.com)
* Trey Caldwell (trey.caldwell@radiantsolutions.com)
* Jong Choi
* Jason Goffeney (jason.goffeney@radiantsolutions.com)
* Brian Hatchl (brian.hatchl@radiantsolutions.com)
* Matt Jackson (matthew.jackson@radiantsolutions.com)
* Ben Marchant (benjamin.marchant@radiantsolutions.com)
* Dmitriy Mylov
* Seth Putnam (seth.putnam@radiantsolutions.com)
* Micah Schicker (micah.schicker@radiantsolutions.com)
* Greg Schmidt
* Josh Sisskind (josh.sisskind@radiantsolutions.com)
* Ming Su (ming.su@radiantsolutions.com)
* Jason R. Surratt
* Brandon Witham (brandon.witham@radiantsolutions.com)
