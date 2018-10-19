![hoot_logo](https://github.com/ngageoint/hootenanny-ui/blob/develop/dist/img/logo/hoot_logo_dark.png)

![](https://github.com/ngageoint/hootenanny/blob/master/docs/user/images/id/hoot_conflation_new.png)

# Introduction
_Hootenanny_: 

1. a gathering at which folksingers entertain often with the audience joining in

_Conflation_: 

1. Fancy word for merge

Hootenanny is an open source conflation tool developed with machine learning techniques to facilitate automated and semi-automated conflation of critical Foundation GEOINT features in the topographic domain.  In short, it merges multiple maps into a single seamless map.

# Project Goals
* Automatically combine geospatial features for decision making
* Allow for reviewing and manually resolving features which cannot be automatically matched with sufficient certainty
* Maintain geometry and attribute provenance for combined features
* Create up-to-date routable transportation networks from multiple sources

# Conflatable Data Types
* Area polygons
* Building polygons
* Points of Interest (POIs)
* Transportation polylines (roads and railways)
* Utility polylines (power lines)
* Waterway polylines

# Feature Summary
In addition to conflating maps, Hootenanny can also:
* Add missing type tags to feature data
* Align two maps together
* Calculate the extent of map data
* Clean map data
* Compare maps
* Compute bounding tiles based on node density
* Concatenate maps together
* Convert maps between different geodata formats (see Supported Data Formats section below)
* Derive changesets between maps and apply them to external OSM data stores
* Explore tag data
* Gather statistics on map features
* Identify road intersections
* Perturb map data for testing purposes
* Plot node feature density
* Sort map data
* Translate feature tags using user defined schemas
* Translate feature tags to English

# Overview
Hootenanny conflation occurs at the dataset level, where the user’s workflow determines the best reference dataset, source content, geometry, 
and attributes to transfer to the output map.  Hootenanny's internal processing leverages the key value pair structure of OpenStreetMap (OSM) 
for improved utility and applicability to broader user groups.  Normalized attributes can be used to aid in feature matching, and OSM’s 
free tagging system allows the map to include an unlimited number of attributes describing each feature.

Hootenanny is developed under the open source General Public License (GPL) and maintained on the National Geospatial-Intelligence 
Agency’s (NGA) GitHub [site](https://github.com/ngageoint/hootenanny). 

# Installation
Hootenanny is supported on Red Hat/CentOS:

[Instructions](https://github.com/ngageoint/hootenanny/blob/master/VAGRANT.md) to launch a Hootenanny CentOS virtual machine

[Instructions](https://github.com/ngageoint/hootenanny/blob/master/docs/install/HootenannyInstall.asciidoc) for an RPM based installation 
to CentOS 7.x.

# Documentation
User and technical documentation may be found locally after installation in 'hoot/docs' or 
[included with each release](https://github.com/ngageoint/hootenanny/releases). 

[FAQ](https://github.com/ngageoint/hootenanny/wiki/Frequently-Asked-Questions)

If you have any support questions please create an issue in the [Hootenanny GitHub repository](https://github.com/ngageoint/hootenanny).

# Web User Interface
[Hootenanny's web user interface](https://github.com/ngageoint/hootenanny-ui) is built upon the open source 
[Mapbox iD Editor](https://github.com/openstreetmap/iD), which provides an intuitive and user-friendly conflation experience. 

# Web Services API
Access to Hootenanny core capabilities are exposed through a web services API for those wishing to develop their own conflation clients.  See the User Documentation for more details.

# Command Line
Command line access is available and exposes additional functionalities not available from the web user interface.  See the "Usage" section below for examples, as well as the User Documentation.

# Bindings
Hootenanny has nodejs bindings available which expose core conflation capabilities.

# Supported Data Formats
**Hootenanny can import from:**
* ESRI File Geodatabase (.gdb)
* GeoJSON (.geojson) **(M)**
* geonames.org (.geonames)
* Hootenanny API Database (hootapidb://)
* JSON file (.json; similar to Overpass JSON) **(M)**
* OpenStreetMap XML (.osm)
* OpenStreetMap Protocol Buffers (.osm.pbf)
* OpenStreetMap API Database (osmapidb://)
* Shapefile (.shp)
* Zip files containing shapefiles and/or ESRI File Geodatabase files (.zip)
* Additional OGR supported formats

**Hootenanny can export to:** 
* ESRI File Geodatabase (.gdb)
* GeoJSON (.geojson) **(M)**
* Hootenanny API Database (hootapidb://)
* JSON file (.json; similar to Overpass JSON) **(M)**
* OpenStreetMap XML file (.osm) **(*)**
* OpenStreetMap Protocol Buffers file (.osm.pbf)
* OpenStreetMap API Database (osmapidb://)
* Shapefile (.shp) **(M)**
* Additional OGR supported formats

**Hootenanny can export changesets to:** 
* OpenStreetMap API Web Service
* OpenStreetMap SQL changeset file (.osc.sql)
* OpenStreetMap XML changeset file (.osc)

Notes:
* **(M)** = format requires reading entire dataset into memory during processing
* **(*)** = format requires reading entire dataset into memory during processing only if element ID output needs to remain sorted
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

# Usage

See the Hootenanny User Guide for more usage examples and details on command input parameters.

## Basic

    # Display available commands
    hoot
    
    # Display help for a command
    hoot help conflate
    
    # CONFLATION

    # Conflate two maps into a single map
    hoot conflate input1.osm input2.osm output.osm
    
    # Conflate and only add data to output from the second input that does not conflict with data in the first input
    hoot conflate --differential input1.osm input2.osm output.osm
    
    # APPLYING CHANGES

    # Derive a changeset between two maps and write the result back to a Rails Port instance
    hoot changeset-derive inputData1.osm inputData2.osm changeset.osc
    hoot changeset-apply changeset.osc http://railsPortUrl --stats --progress
    
    # DATA TRANSFORMATION
    
    # Convert an OSM file to a file geodatabase and apply a schema translation
    hoot convert input.osm output.gdb --trans MyTranslation.js
    
    # Convert an OSM database to a file geodatabase and apply a schema translation
    hoot convert PG:"dbname='mydb' host='myhost' port='5432' user='myuser' password='mypass'" output.gdb \
      --trans MyTranslation.js
    
    # Convert an OSM file to a shape file specifying export columns
    hoot convert input.osm output.shp --cols "highway,surface,name,alt_name,oneway,bridge"
    
    # Convert multiple shape files to an OSM file with schema translation
    hoot convert input1.shp input2.shp output.osm --trans translation.js
    
    # Convert roads, bridges, overpasses and tunnels from a File Geodatabase into a single .osm file with schema translation
    hoot convert input.gdb;ROAD_L input.gdb;BRIDGE_OVERPASS_L input.gdb;TUNNEL_L output.osm --trans translation.js
    
    # Convert a shapefile that is stored inside the a Zip file:
    hoot convert /vsizip//gis-data/input.zip/tds/LAP030.shp output.osm --trans translation.js
    
    # UTILITIES
    
    # Clean erroneous data from two maps
    hoot clean input.osm output.osm
    
    # Crop a map
    hoot crop input.osm output.osm "-77.0551,38.8845,-77.0281,38.9031"
    
    # Bring two datasets closer in alignment
    hoot rubber-sheet input1.osm input2.osm output.osm
    
    # Display the extent of map data
    hoot extent input.osm
    
    Map extent (minx,miny,maxx,maxy): -104.902,38.8532,-104.896,38.855
    
    # Sort data to the OSM standard in memory
    hoot sort input.osm output.osm
    
    # Concatenate two maps
    hoot cat input1.osm input2.osm output.osm
    
    # COMPARISON
    
    # Calculate the difference between two maps
    hoot diff input1.osm input2.osm
    
    # Compare two maps
    hoot compare input1.osm input2.osm
    
    Attribute Score 1: 981 +/-5
    Attribute Score 2: 993 +/-3
    Attribute Score: 987 +/-4 (983 to 991)
    Raster Score 1: 982
    Raster Score 2: 989
    Raster Score: 986
    Graph Score 1: 944 +/-19 (925 to 963)
    Graph Score 2: 996 +/-0 (996 to 996)
    Graph Score: 970 +/-10 (960 to 980)
    Overall: 981 +/-4 (977 to 985)
    
    # Compare tags between maps
    hoot tag-compare input1.osm input2.osm
    
    |                    | amenity=restaurant | building=yes | name=<NULL> | name=<SIMILAR> |
    | amenity=restaurant |                  4 |              |             |                |
    |       building=yes |                    |           28 |             |                |
    |        name=<NULL> |                    |              |           4 |                |
    |     name=<SIMILAR> |                    |              |             |             24 |
    
    # STATISTICS
    
    # Display a set of statistics for a map
    hoot stats input.osm
    
    # count all elements
    hoot count input.osm --all-elements

    # count all POIs
    hoot count "input1.osm;input2.osm" hoot::PoiCriterion

## Advanced

    # CONFLATION
    
    # Conflate only the buildings from the input maps
    hoot conflate -D match.creators="hoot::BuildingMatchCreator" -D merger.creators="hoot::BuildingMergerCreator" \
      input1.osm input2.osm output.osm
      
    # Filter input datasets down to POIs only before conflating them
    hoot conflate -D conflate.pre.ops="hoot::RemoveElementsVisitor" \ 
      -D remove.elements.visitor.element.criterion="hoot::PoiCriterion" input1.osm input2.osm output.osm
    
    # Translate features to a schema before conflating them
    hoot conflate -D conflate.pre.ops="hoot::TranslationOp" -D translation.script=myTranslation.js \  
      input1.osm input2.osm output.osm
      
    # Align a second input map towards the first input map before conflating them
    hoot conflate -D conflate.pre.ops="hoot::RubberSheet" -D rubber.sheet.ref=true input1.osm input2.osm output.osm
    
    # Assuming the first input map is superior to the second, cut out the shape of the first input map from the data \
      being conflated so that only data from the second input map is stitched in
    hoot conflate -D unify.pre.ops=hoot::CookieCutterOp -D cookie.cutter.alpha=2500 -D cookie.cutter.alpha.shape.buffer=0 \
      -D cookie.cutter.output.crop=false
    
    # APPLYING CHANGES
    
    # Derive a changeset between two maps and write the result directly to an OSM API database
    hoot changeset-derive inputData1.osm inputData2.osm changeset.osc.sql
    hoot changeset-apply changeset.osc.sql osmapidb://username:password@localhost:5432/databaseName
    
    # DATA TRANSFORMATION
    
    # Convert an OSM file to a shape file, allowing the export columns to be automatically selected based on frequency
    hoot convert input.osm output.shp --cols
    
    # Bulk write to an offline OSM API database
    hoot convert -D changeset.user.id=1 -D osmapidb.bulk.inserter.disable.database.constraints.during.write=true \
      -D osmapidb.bulk.inserter.disable.database.indexes.during.write=true -D apidb.bulk.inserter.starting.node.id=10 \
      -D apidb.bulk.inserter.starting.way.id=10 -D apidb.bulk.inserter.starting.relation.id=10 \
      input.osm.pbf osmapidb://username:password@localhost:5432/database
      
    # Bulk write to an online OSM API database
    hoot convert -D changeset.user.id=1 \
      -D osmapidb.bulk.inserter.reserve.record.ids.before.writing.data=true \
      input.osm.pbf osmapidb://username:password@localhost:5432/database
    
    # Write only nodes from an input file to output
    hoot convert -D convert.ops="hoot::RemoveElementsVisitor" \ 
      -D remove.elements.visitor.element.criterion="hoot::NodeCriterion" input.osm output.osm
      
    # Remove all duplicate ways from a map
    hoot convert -D convert.ops="hoot::DuplicateWayRemover" input.osm output.osm
    
    # Remove duplicate name tags from features
    hoot convert -D convert.ops="hoot::DuplicateNameRemover" input.osm output.osm
    
    # Merge nodes that are near each other
    hoot convert -D convert.ops="hoot::MergeNearbyNodes" input.osm output.osm
    
    # Remove elements that contain no useful information
    hoot convert -D convert.ops="hoot::NoInformationElementRemover" input.osm output.osm
    
    # Add the tag "error:circular=5.0" to all elements
    hoot convert -D convert.ops=hoot::SetTagVisitor -D set.tag.visitor.key=error:circular \
      -D set.tag.visitor.value=5.0 input.osm output.osm
    
    # Remove all "source" and "error:circular" tags from ways
    hoot convert -D convert.ops="hoot::RemoveTagsVisitor" -D remove.tags.visitor.element.criterion="hoot::WayCriterion" \
      -D remove.tags.visitor.keys="source;error:circular" input.osm output.osm
      
    # Remove all elements that have the tag "status=proposed"
    hoot convert -D convert.ops=hoot::RemoveElementsVisitor -D remove.elements.visitor.filter=hoot::TagCriterion \ 
      -D tag.criterion.kvps="status=proposed"
      
    # Remove all tags with keys "REF1" and "REF2" from elements containing the tag "power=line"
    hoot convert -D convert.ops=hoot::RemoveTagsVisitor -D remove.tags.visitor.keys="REF1;REF2" \ 
      -D remove.tags.visitor.element.criterion=hoot::TagCriterion -D tag.criterion.kvps="power=line" \ 
      -D element.criterion.negate=true input.osm output.osm
      
    # For all features with a "voltage" tag between 1 and 45k, set the tag "power=minor_line"
    hoot convert -D convert.ops=hoot::SetTagValueVisitor -D set.tag.value.visitor.key=power \ 
      -D set.tag.value.visitor.value=minor_line \
      -D set.tag.value.visitor.element.criterion=hoot::TagValueNumericRangeCriterion \
      -D tag.value.numeric.range.criterion.keys=voltage -D tag.value.numeric.range.criterion.min=1 \
      -D tag.value.numeric.range.criterion.max=45000 input.osm output.osm
      
    # Add missing attributes to corrupted elements
    hoot convert -D convert.ops="hoot::AddAttributesVisitor" -D add.attributes.visitor.kvps="changeset=1" \
      input.osm output.osm
      
    # UTILITIES
    
    # Sort data to the OSM standard on disk
    hoot sort -D element.sorter.element.buffer.size=10000 input.osm output.osm 
    
    # Combine sets of polygons together
    hoot union-polygons input1.osm input2.osm output.osm
    
    # Detect road intersections and write them to output
    hoot find-intersections input.osm output.osm
    
    # Create a node density plot
    hoot node-density-plot input.osm output.png 100
    
    # Make a perturbed copy of data for testing purposes
    hoot perty -D perty.search.distance=20 -D perty.way.generalize.probability=0.7 input.osm output.osm
    
    # Display the internal tag schema Hootenanny uses
    hoot tag-schema
    
    # STATISTICS
    
    # count all features
    hoot count input.osm

    # count all elements that are not POIs
    hoot count -D element.criterion.negate=true "input1.osm;input2.osm" hoot::PoiCriterion --all-elements

    # count all features which have a tag whose key contains the text "phone"
    hoot count -D tag.key.contains.criterion.text="phone" input1.osm hoot::TagKeyContainsCriterion
    
    # Calculate the numerical average of all "accuracy" tags
    hoot stat -D tags.visitor.keys="accuracy" input.osm hoot::AverageNumericTagsVisitor
    
    # Find the largest ID in a set of elements
    hoot stat input.osm hoot::MaxIdVisitor
    
    # Display the accuracy distribution for a map; output shows that 14 ways were found with an accuracy 
    # of 15 meters and the value of 15 meters represents 100% of the ways examined
    hoot tag-accuracy-distribution input.osm
    
    15 : 14 (1)
    
    # Display tag schema information for a map
    hoot tag-info input.osm
    
    .{
    "ca-Transmission_Line-state-gov.shp":{
    "ca-Transmission_Line-state-gov":{
      "Circuit":[
        "Double",
        "Duble",
        "Liberty Energy",
        "Many",
        "Quad",
        "Single"
        ],
      "Comments":[
        "Attached to 115kv poles",
        "Caldwell-victor 220kv",
        "Changed kv from 115 to 60kv",
        "Distribution line",
        ...
        ],
      "Legend":[
        "IID_161kV",
        "IID_230kV",
        "IID_34.5_92kV",
        "LADWP_115_138kV",
        ...
        ],
        ...
    }}
    
    # Display frequences of feature names
    hoot tag-name-frequencies input.osm
    
    Total word count: 1163
    320 (0.28) : nw
    246 (0.21) : st
    80 (0.069) : ave
    45 (0.039) : sw
    18 (0.015) : h
    18 (0.015) : pennsylvania
    ...
    
    # ADD MISSING TYPE TAGS
    
    # Attempt to add type tags to POI and building features that are missing them
    hoot convert -D convert.ops=hoot::ImplicitPoiPolygonTypeTagger input.osm output.osm
    
    # Attempt to add type tags to POI and building features that are missing them before conflating them
    hoot convert -D conflate.pre.ops=hoot::ImplicitPoiPolygonTypeTagger input1.osm input2.osm output.osm
    
    # LANGUAGE TRANSLATION
    
    # Translate "name" and "alt_name" tags from German or Spanish to English
    hoot convert -D convert.ops="hoot::ToEnglishTranslationVisitor" -D language.translation.source.languages="de;es" \
      -D language.translation.to.translate.tag.keys="name;alt_name" input.osm output.osm
      
    # Translate "name" tags to English and let the source language be detected
    hoot convert -D convert.ops="hoot::ToEnglishTranslationVisitor" -D language.translation.source.languages="detect" \ 
      -D language.translation.to.translate.tag.keys="name" input.osm output.osm
      
    # APPLICATION METAINFO
    
    # Lists all configuration option names
    hoot info --config-options

    # List all configuration option names and their descriptions
    hoot info --config-options --option-details

    # List all configuration option names containing "poi.polygon"
    hoot info --config-options poi.polygon --option-names

    # List all available feature extractors
    hoot info --feature-extractors
    
    # List all supported input data formats
    hoot info --formats --input
    
    # List all supported output data formats
    hoot info --formats --output
    
    # List all available feature matchers
    hoot info --matchers
    
    # List all available feature mergers
    hoot info --mergers
    
    # List all available data operators
    hoot info --operators
      
    # List all available tag mergers
    hoot info --tag-mergers
    
    # List all available language detectors
    hoot languages --detectors
    
    # List all availabe language translators
    hoot languages --translators
    
    # List all detectable langauges
    hoot languages --detectable
    
    # List all translatable languages
    hoot languages --translatable

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
