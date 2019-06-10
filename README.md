![hoot_logo](https://github.com/ngageoint/hootenanny-ui/blob/develop/dist/img/logo/hoot_logo_dark.png)

![](https://github.com/ngageoint/hootenanny/blob/master/docs/user/images/id/hoot_conflation_new.png)

# Overview
_Hootenanny_: 

1. A gathering at which folksingers entertain often with the audience joining in

_Conflation_: 

1. Fancy word for merge

[Hootenanny](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc) is an open source conflation tool developed with machine learning techniques to facilitate automated and semi-automated conflation of critical Foundation GEOINT features in the topographic domain.  In short, it merges multiple maps into a single seamless map.

Hootenanny conflation occurs at the dataset level, where the user’s workflow determines the best reference dataset, source content, geometry, and attributes to transfer to the output map.  Hootenanny's internal processing leverages the key value pair structure of OpenStreetMap (OSM) for improved utility and applicability to broader user groups.  Normalized attributes can be used to aid in feature matching, and OSM’s free tagging system allows the map to include an unlimited number of attributes describing each feature.

# Goals
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

Additional feature types can be made conflatable quickly via custom script by using Hootenanny's [Generic Conflation capability](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).

# Types of [Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Introduction.asciidoc)
## Default
* **[Reference Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc)** - Conflate the best geometry and tag parts of map B into map A, favoring map A's data.
## Advanced
* **[Attribute Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/AttributeConflation.asciidoc)** - Conflate map A with B where only tags are transferred from B to A and few or 
no changes are made to A's geometry.
* **[Differential Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/DifferentialConflation.asciidoc)** - Conflate map A with B where the only data added to the output from B is in areas that don't overlap with A.  Optionally, you can configure to overwrite tags in A from B even when there is overlap or to output the unmodified map A data as well.
* **[Horizontal Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/cookie-cut.asciidoc)** (aka Cookie Cutter Conflation) - Define a specific region in map A that should not be modified and stitch in data from map B around it.

There are a wide range of [configuration options](https://github.com/ngageoint/hootenanny/blob/master/conf/core/ConfigOptions.asciidoc) 
available to customize the conflation workflows.

# [Conflation Algorithms](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/ConflationAlgsOverview.md)

# Tag Schemas
Hootenanny leverages the OSM key value pair tag concept to support translation between various data schemas and supports automated schema conversion between: 
* Topographic Data Store (TDS) v6.1/v4.0 
* Multi-National Geospatial Co-Production Program (MGCP)
* Geonames
* OSM 
* [others](https://github.com/ngageoint/hootenanny/tree/master/translations)

Users can define their own [custom translations](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc#translations).

# [Supported Data Formats](https://github.com/ngageoint/hootenanny/tree/master/docs/user/SupportedDataFormats.asciidoc)

# Web User Interface
[Hootenanny's](https://github.com/ngageoint/hootenanny-ui) [web user interface](https://github.com/ngageoint/hootenanny/blob/master/docs/user/Hootenanny-id.asciidoc) is built upon the open source 
[Mapbox iD Editor](https://github.com/openstreetmap/iD), which provides an intuitive and user-friendly conflation experience.

# Web Services API
Access to Hootenanny core capabilities are exposed through a web services API for those wishing to develop their own conflation clients. 

The web services use [OAuth](https://github.com/ngageoint/hootenanny/tree/master/docs/developer/OAUTH.md) authentication.

# Command Line Interface
[Command line access](https://github.com/ngageoint/hootenanny/blob/master/docs/commands/HootCommandLineReference.asciidoc) is available to aid
in custom scripting of conflation capabilities.

Example:

```
#  conflate two datasets together
hoot conflate input1.osm input2.osm output.osm
```

[More examples](https://github.com/ngageoint/hootenanny/tree/master/docs/user/CommandLineExamples.asciidoc)

The command line often exposes additional functionalities not available from the web user interface.

# Programming Language Bindings
Hootenanny has [nodejs bindings](https://github.com/ngageoint/hootenanny/blob/master/docs/JavascriptOverview.asciidoc) available which expose core conflation capabilities for creating custom workflows. 

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

# [Additional Feature Summary](https://github.com/ngageoint/hootenanny/tree/master/docs/user/AdditionalFeatureSummary.md)

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

# Development
* [Contributing](https://github.com/ngageoint/hootenanny/blob/master/CONTRIBUTING.md)
* [Developer's Guide](https://github.com/ngageoint/hootenanny/blob/master/docs/HootenannyDeveloperGuide.asciidoc)

# [Redistribution](https://github.com/ngageoint/hootenanny/blob/master/docs/Redistribution.md)
