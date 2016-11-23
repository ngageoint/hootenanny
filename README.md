# Introduction

Hootenanny is an open source conflation tool developed to facilitate automated and semi-automated conflation 
of critical Foundation GEOINT features in the topographic domain, namely roads (polylines), buildings (polygons), 
and points-of-interest (POI’s) (points). Conflation occurs at the dataset level, where the user’s workflow 
determines the best reference dataset and source content, geometry and attributes, to transfer to the output map. 
Hootenanny internal processing leverages the key value pair structure of OpenStreetMap (OSM) for improved utility 
and applicability to broader user groups, e.g. normalized attributes can be used to aid in feature matching and OSM’s 
free tagging system allows the map to include an unlimited number of attributes describing each feature.  Hootenanny 
is developed under the open source General Public License (GPL) and maintained on the National Geospatial-Intelligence 
Agency’s (NGA) GitHub [site](https://github.com/ngageoint/hootenanny).

Hootenanny is built upon the open source [Mapbox iD Editor](https://github.com/openstreetmap/iD) which provides an intuitive 
and user-friendly conflation experience without the extra overhead of thick desktop GIS clients. This offers several key 
benefits which include its open license allowing users to easily customize and add functionality, a robust editing capability 
originally targeted for interactive editing of OSM features, and a web service based architecture for integrating into SOA environments.  
A REST API is in place to connect the web browser based User Interface (UI) with the core conflation algorithms and database. 
The translation and conflation operations are also exposed through node.js service endpoints and the resulting vector data is accessible via an Open Geospatial Consortium (OGC) Web Feature Service (WFS) for additional open 
interoperability. The application is primarily supported in either the Chrome or Chromium browser with limited supported in Firefox.  

If you have any support questions please create an Issue in this repo or contact us via email: Hootenanny.Help@DigitalGlobe.com.

# Specifications

We actively maintain and update our [Frequently Asked Questions page](https://github.com/ngageoint/hootenanny/wiki/Frequently-Asked-Questions) 
so please refer to that page for any questions about general Hootenanny capabilities.

## Conflation
Hootenanny currently supports the conflation of: 
* POIs
* Building polygons
* Transportation polylines (highways, cart tracks, trails, bridges, and tunnels)
* Waterway polylines (hydrologic features such as rivers, streams, drainage, and ditches).
* POIs and building/area polygons

When conflating these features, it is important to note that, with the exception of the POI to polygon conflation, the conflation occurs between similar feature types (e.g. POI to POI, Building to Building, Transportation polyline to Transportation polyline, etc.).

## Supported Data Formats
_Import:_ Hootenanny can ingest from:
* Shapefile (.shp)
* OpenStreetMap (.osm)
* ESRI File Geodatabase (.gdb)
* .zip files containing shapefiles and/or .gdb files
* geonames.org (.geonames)
* OSM API database

_Export:_ Hootenanny can export to: 
* Shapefile (.shp)
* OpenStreetMap (.osm)
* ESRI File Geodatabase (.gdb)
* Web Feature Service (WFS)
* OSM API database via SQL changeset
* OSM XML changeset

Note:

* .gdb, Shapefile, and OSM formats are exported as a zip file containing all of the relevant 
associated files
* WFS is exported as WFS Get Capabilities service URL that can be added into an OGC-enabled third party application
* OSM API database export is done with an OSM changeset representing the before/after difference of conflated data.
* imports that may specify a geospatial bounds are supported with OSM API database format imports only, currently

## Tag Schemas
Hootenanny leverages the OSM key value pair tag concept and PostgreSQL database structure to support translation between various 
data schemas.  By default, Hootenanny supports automated schema conversion between Topographic Data Store (TDS) v6.1/v4.0, 
Multi-National Geospatial Co-Production Program (MGCP), Geonames, OSM, as well as user defined "custom" translations where a 
specific mapping can be defined based on an uploaded dataset using a semi-automated Translation Assistant.  More detailed 
background on the translation capabilities of Hootenanny can be found in Hootenanny User Guide as well as specific examples 
from the UI in the Hootenanny User Interface Guide. These documents can accessed in your local Hootenanny instance in hoot/docs 
or on the [releases](https://github.com/ngageoint/hootenanny/releases) under each version e.g. hootenanny-0.2.22.tar.gz/docs. 

# Installation
Instructions to launch a Hootenanny virtual machine using [Vagrant](https://www.vagrantup.com/) may
be found [here](https://github.com/ngageoint/hootenanny/blob/master/VAGRANT.md).

Optional file download:
* https://s3.amazonaws.com/hoot-rpms/support-files/words1.sqlite.bz2

If you're compiling manually on CentOS, you'll need:
** https://github.com/ngageoint/hootenanny/releases/download/v0.2.16/geos-3.3.8-patched.tgz

Additional instructions for setting up a Hootenanny development environment may be found in the Hootenanny
Developer's Guide.

# Getting Started
Once installed and configured, Hootenanny can be accessed from a browser by opening the application URL.  http://localhost:port/hootenanny-id

![](https://cloud.githubusercontent.com/assets/7560096/11984226/6ed7b6ae-a96e-11e5-9470-dc1f987f0b7a.png)

* Note that the specific port designation can be configured from either the Vagrantfile (if using Vagrant installation approach) or from the Tomcat/conf/server.xml file.  

## Hootenanny User Interface Guide
The Hootenanny User Interface Guide contains all of the background required to walk through each functional 
component of the software.  Sample datasets can be acquired from the provided URLs in the guide or within the 
%HOOT_HOME/test-files directory.

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
