![hoot_logo](https://github.com/ngageoint/hootenanny-ui/blob/develop/dist/img/logo/hoot_logo_dark.png)

![](https://github.com/ngageoint/hootenanny/blob/master/docs/user/images/id/hoot_conflation_new.png)

# Introduction
_Hootenanny_: 

1. a gathering at which folksingers entertain often with the audience joining in

_Conflation_: 

1. Fancy word for merge

Hootenanny is an open source conflation tool developed to facilitate automated and semi-automated conflation 
of critical Foundation GEOINT features in the topographic domain.  In short, it merges multiple maps of geodata into a 
single seamless map.

Conflation occurs at the dataset level, where the user’s workflow determines the best reference dataset, source content, geometry, and attributes to transfer to the output map.  Hootenanny's internal processing leverages the key value pair structure of OpenStreetMap (OSM) for improved utility and applicability to broader user groups.  Normalized attributes can be used to aid in feature matching, and OSM’s free tagging system allows the map to include an unlimited number of attributes describing each feature. 

# Project Goals
* Automatically combine geospatial features for decision making
* Allow for reviewing and manually resolving features which cannot be automatically matched with sufficient certainty
* Maintain geometry and attribute provenance for combined features
* Create up-to-date routable transportation networks from multiple sources

# Supported Geodata Types
Hootenanny can conflate:
* POIs
* Building polygons
* Transportation polylines
* Waterway polylines

# Supported Data Formats
Hootenanny can import from:
* OpenStreetMap XML (.osm)
* Shapefile (.shp)
* ESRI File Geodatabase (.gdb)
* Zip files containing shapefiles and/or ESRI File Geodatabase files (.zip)
* OpenStreetMap Protocol Buffers (.osm.pbf)
* OpenStreetMap JSON file (.json; similar to Overpass JSON)
* geonames.org (.geonames)
* Hootenanny API Database (hootapidb://)
* OpenStreetMap API Database (osmapidb://)

Hootenanny can export to: 
* OpenStreetMap XML file (.osm)
* Shapefile (.shp)
* ESRI File Geodatabase (.gdb)
* OpenStreetMap Protocol Buffers file (.osm.pbf)
* OpenStreetMap JSON file (.json; similar to Overpass JSON)
* Hootenanny API Database (hootapidb://)
* OpenStreetMap API Database (osmapidb://)
* OpenStreetMap XML changeset file (.osc)
* OpenStreetMap SQL changeset file (.osc.sql)

# Tag Schemas
Hootenanny leverages the OSM key value pair tag concept and PostgreSQL database structure to support translation between various data schemas.  By default, Hootenanny supports automated schema conversion between: 
* Topographic Data Store (TDS) v6.1/v4.0 
* Multi-National Geospatial Co-Production Program (MGCP)
* Geonames
* OSM
* user defined "custom" translations 

For custom translations, a specific mapping can be defined based on an uploaded dataset using a semi-automated Translation Assistant.  More detailed background on the translation capabilities of Hootenanny can be found in Hootenanny User Guide as well as specific examples from the UI in the Hootenanny User Interface Guide. These documents can accessed in your local Hootenanny instance in hoot/docs or on the [releases](https://github.com/ngageoint/hootenanny/releases) under each version e.g. hootenanny-0.2.22.tar.gz/docs. 

# Installation
Instructions to launch an Ubuntu Hootenanny virtual machine using [Vagrant](https://www.vagrantup.com/) may
be found [here](https://github.com/ngageoint/hootenanny/blob/master/VAGRANT.md).

An RPM based installation is also available for CentOS 6.7. 

# Documentation
Please read the documentation included with the latest release for details on installing and using Hootenanny, as well as the algorithmic concepts behind the software.

# Contributing
Please read the Hootenanny Developer's Guide for details on setting up an environment, coding standards, and development process.  Hootenanny developers use the [Gitflow workflow](https://www.atlassian.com/git/tutorials/comparing-workflows#gitflow-workflow).  
## Workflow Summary
* Open a repository issue for the new feature to be worked on.
* Perform work for the feature on a new git feature branch named with the number of the issue opened.
* Open a pull request and assign at least one reviewer to merge the feature branch into the "develop" branch mainline when the feature is complete.

# Specifications
Hootenanny is developed under the open source General Public License (GPL) and maintained on the National Geospatial-Intelligence Agency’s (NGA) GitHub [site](https://github.com/ngageoint/hootenanny).

Hootenanny is built upon the open source [Mapbox iD Editor](https://github.com/openstreetmap/iD), which provides an intuitive and user-friendly conflation experience without the extra overhead of thick desktop GIS clients.  The application is primarily supported in either the Chrome or Chromium browser with more limited supported in Firefox.   

A REST API is in place to connect the web browser based User Interface (UI) with the core conflation algorithms and database.  The translation and conflation operations are also exposed through web service endpoints. 

We actively maintain and update our [Frequently Asked Questions page](https://github.com/ngageoint/hootenanny/wiki/Frequently-Asked-Questions), so please refer to that page for any questions about general Hootenanny capabilities.  If you have any support questions please create an issue in this repo or contact us via email: Hootenanny.Help@DigitalGlobe.com.

# Redistribution
Hootenanny was developed at the National Geospatial-Intelligence Agency (NGA) in collaboration with DigitalGlobe.  The government has "unlimited rights" and is releasing this software to increase the impact of government instruments by providing developers with the opportunity to take things in new directions. The software use, modification, and distribution rights are stipulated within the GNU General Public License. The GPL license is available in LICENSE.txt

All pull requests contributions to this project will be released under the GNU General Public License 3.0. Software source code previously released under an open source license and then modified by NGA staff is considered a "joint work" (see 17 USC 101); it is partially copyrighted, partially public domain, and as a whole is protected by the copyrights of the non-government authors and must be released according to the terms of the original open source license.

Licensed under the GNU General Public License v3.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.gnu.org/copyleft/gpl.html.

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Imagery provided by permission from DigitalGlobe. Users are responsible for complying with terms of use for data and imagery they use in conjunction with Hootenanny. Specifically, the must properly protect and comply with all legal, copyright, and licensing terms.

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

# Developers
In alphabetical order:
* Trey Caldwell (Trey.Caldwell@digitalglobe.com)
* Jong Choi (Jong.Choi@digitalglobe.com)
* Jason Goffeney (jason.goffeney@digitalglobe.com)
* Brian Hatchl (Brian.Hatchl@digitalglobe.com)
* Matt Jackson (Matthew.Jackson@digitalglobe.com)
* Ben Marchant (Benjamin.Marchant@digitalglobe.com)
* Dmitriy Mylov (Dmitriy.Mylov@digitalglobe.com)
* Seth Putnam (Seth.Putnam@digitalglobe.com)
* Micah Schicker (Micah.Schicker@digitalglobe.com)
* Greg Schmidt (Greg.Schmidt@digitalglobe.com)
* Josh Sisskind (Josh.Sisskind@digitalglobe.com)
* Ming Su (Ming.Su@digitalglobe.com)
* Jason R. Surratt (Jason.Surratt@digitalglobe.com)
* Brandon Witham (Brandon.Witham@digitalglobe.com)
