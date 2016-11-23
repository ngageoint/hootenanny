# Translation Server API
* Translates OSM xml to/from supported schemas.
* Provides full field information for selected feature types.
* Provides available key/value information for translated schemas by feature type. (a la taginfo).
* Provides a search service for translated feature types.

___
## GET /version

### URL
http://localhost:8094/version

### Response
```
{version: '0.0.2'}
```
___
## GET /capabilities

### URL
http://localhost:8094/capabilities

### Response
```
{"TDSv40":{"isavailable":true},"TDSv61":{"isavailable":true},"MGCP":{"isavailable":true}}
```
___
## GET /osmtotds

### URL
http://localhost:8094/osmtotds?idval=AL015&geom=Point&translation=MGCP&idelem=fcode

### Parameters
___
idelem | The tag key for the feature to translate.
--- | --- 
_required_ | **Example: fcode**
___

idval | The tag value for the feature to translate.
--- | --- 
_required_ | **Example: AL015**
___

geom | The geometry type for the feature to translate.
--- | --- 
_required_ | **Example: Point**
___

translation | The attribute schema to translate OSM tags to.
--- | --- 
_required_ | **Example: TDSv61**
___

### Response
```
{
  "name": "PAL015",
  "fcode": "AL015",
  "desc": "General Building",
  "definition": "A relatively permanent structure, roofed and usually walled and designed for some particular use.",
  "geom": "Point",
  "columns": [
    {
      "name": "ACC",
      "desc": "Horizontal Accuracy Category",
      "optional": "R",
      "definition": "A general evaluation of the horizontal accuracy of the geographic position of a feature, as a category.",
      "type": "enumeration",
      "defValue": "1",
      "enumerations": [
        {
          "name": "Accurate",
          "value": "1"
        },
        {
          "name": "Approximate",
          "value": "2"
        }
      ]
    },
    {
      "name": "CCN",
      "desc": "Commercial Copyright Notice",
      "optional": "R",
      "definition": "A description of any commercial (or similar) copyright notice applicable to information regarding the feature or data set. ( For example, '©2000 Space Imaging, Inc.', in which case the copyright information for use of such imagery needs to be stated; this generally includes restrictions on use and distribution. For non copyright data, for example 'Copyright 2004 by the National Geospatial-Intelligence Agency, U.S. Government. No domestic copyright claimed under Title 17 U.S.C. All rights reserved.' )",
      "length": "254",
      "type": "String",
      "defValue": "UNK"
    },
    {
      "name": "FCODE",
      "desc": "Feature Code",
      "optional": "R",
      "length": "5",
      "type": "String",
      "defValue": ""
    },
    .
    .
    .
  ]
}    
```
___
## POST /osmtotds

### URL
http://localhost:8094/osmtotds?translation=TDSv61

### Parameters
___
translation | The attribute schema to translate OSM tags to.
--- | --- 
_required_ | **Example: TDSv61**
___

### Body

```
<osm version="0.6" upload="true" generator="JOSM">
    <node id="-1" lon="-105.21811763904256" lat="39.35643172777992" version="0">
        <tag k="building" v="yes"/>
        <tag k="uuid" v="{bfd3f222-8e04-4ddc-b201-476099761302}"/>
    </node>
</osm>
```

### Response
```
<?xml version="1.0" encoding="UTF-8"?>
<osm version="0.6" generator="hootenanny" srs="+epsg:4326" schema="TDSv61">
    <node visible="true" id="-1" timestamp="1970-01-01T00:00:00Z" version="1" lat="39.3564317277799205" lon="-105.2181176390425605">
        <tag k="Feature Code" v="AL013:Building"/>
        <tag k="Unique Entity Identifier" v="bfd3f222-8e04-4ddc-b201-476099761302"/>
    </node>
</osm>
```
___
## GET /tdstoosm

### URL
http://localhost:8094/tdstoosm?fcode=AL013&translation=TDSv61

### Parameters
___
fcode | The feature code to translate to OSM.
--- | --- 
_required_ | **Example: AL013**
___

translation | The attribute schema to translate to OSM tags.
--- | --- 
_required_ | **Example: TDSv61**
___

### Response
```
{
  "attrs": {
    "building": "yes",
    "uuid": "{d5d6502a-60d3-4f85-a872-e939d57bcb4f}"
  },
  "tableName": ""
}
```
___
## POST /tdstoosm

### URL
http://localhost:8094/tdstoosm?translation=MGCP

### Parameters
___
translation | The attribute schema to translate OSM tags from.
--- | --- 
_required_ | **Example: TDSv61**
___

### Body

```
<osm version="0.6" upload="true" generator="JOSM">
    <node id="-9" lon="-104.907037158172" lat="38.8571566428667" version="0">
        <tag k="Horizontal Accuracy Category" v="Accurate"/>
        <tag k="Built-up Area Density Category" ve="Unknown"/>
        <tag k="Commercial Copyright Notice" v="UNK"/>
        <tag k="Feature Code" v="AL020:Built-Up Area"/>
        <tag k="Functional Use" v="Other"/>
        <tag k="Condition of Facility" v="Unknown"/>
        <tag k="Name" v="Manitou Springs"/>
        <tag k="Named Feature Identifier" v="UNK"/>
        <tag k="Name Identifier" v="UNK"/>
        <tag k="Relative Importance" v="Unknown"/>
        <tag k="Source Description" v="N_A"/>
        <tag k="Source Date and Time" v="UNK"/>
        <tag k="Source Type" v="Unknown"/>
        <tag k="Associated Text" v="&lt;OSM&gt;{&quot;poi&quot;:&quot;yes&quot;}&lt;/OSM&gt;"/>
        <tag k="MGCP Feature universally unique identifier" v="c6df0618-ce96-483c-8d6a-afa33541646c"/>
    </node>
</osm>
```

### Response
```
<?xml version="1.0" encoding="UTF-8"?>
<osm version="0.6" generator="hootenanny" srs="+epsg:4326">
    <node visible="true" id="-1" timestamp="1970-01-01T00:00:00Z" version="1" lat="38.8571566428666983" lon="-104.9070371581720025">
        <tag k="source:copyright" v="UNK"/>
        <tag k="uuid" v="{b0fc3f23-3c16-4cb5-a26a-5b919c9f4911}"/>
        <tag k="landuse" v="built_up_area"/>
    </node>
</osm>
```
___
## GET /schema

### URL
http://localhost:8094/schema?geometry=point&translation=MGCP&searchstr=Buil&maxlevdst=20&limit=12

### Parameters
___
geometry | The geometry of the feature.
--- | --- 
_required_ | **Example: line**

___
translation | The attribute schema name.
--- | --- 
_required_ | **Example: TDSv61**

___
searchStr | The string used to search feature types and feature codes in this schema.
--- | --- 
_required_ | **Example: Building**

___
maxlevdst | The maximum levenshtein distance to determine a fuzzy match.
--- | --- 
_optional_ | **Example: 20**

___
limit | The maximum number of matches to return.
--- | --- 
_optional_ | **Example: 12**
___

### Response
```
[
  {
    "name": "PAL015",
    "fcode": "AL015",
    "desc": "General Building",
    "geom": "Point",
    "idx": -1
  },
  {
    "name": "PAL020",
    "fcode": "AL020",
    "desc": "Built-Up Area",
    "geom": "Point",
    "idx": -1
  },
  .
  .
  .
  {
    "name": "PBH070",
    "fcode": "BH070",
    "desc": "Ford",
    "geom": "Point",
    "idx": 4
  },
  {
    "name": "PBH120",
    "fcode": "BH120",
    "desc": "Rapids",
    "geom": "Point",
    "idx": 4
  }
]
```
___

## GET /taginfo/key/values

### URL
http://localhost:8094/taginfo/key/values?fcode=AA040&filter=nodes&key=FUN&page=1&query=Damaged&rp=25&sortname=count_nodes&sortorder=desc&translation=MGCP

### Parameters
___
fcode | The feature code.
--- | --- 
_required_ | **Example: AA040**

___
filter | The geometry type to filter options by.
--- | --- 
_required_ | **Example: nodes**

___
key | The tag key name for specified translation schema.
--- | --- 
_required_ | **Example: FUN**

___
page | The result set page number. (NOT SURE THIS IS IMPLEMENTED)
--- | --- 
_required_ | **Example: 1**

___
query | The query for tag values. (NOT SURE THIS IS IMPLEMENTED).
--- | --- 
_required_ | **Example: Damaged**

___
rp | The results per page. (NOT SURE THIS IS IMPLEMENTED)
--- | --- 
_optional_ | **Example: 25**

___
sortname | The property to sort. (NOT SURE THIS IS IMPLEMENTED)
--- | --- 
_optional_ | **Example: count_nodes**

___
sortorder | The sort order [asc or desc]. (NOT SURE THIS IS IMPLEMENTED).
--- | --- 
_optional_ | **Example: desc**

___
translation | The attribute schema name.
--- | --- 
_required_ | **Example: TDSv61**

___

### Response
```
{
  "page": 1,
  "rp": 7,
  "total": 7,
  "url": "",
  "data": [
    {
      "value": "Unknown",
      "count": 1,
      "fraction": 0.19,
      "in_wiki": false,
      "description": "Unknown",
      "internal_val": "0"
    },
    {
      "value": "Under Construction",
      "count": 1,
      "fraction": 0.19,
      "in_wiki": false,
      "description": "Under Construction",
      "internal_val": "1"
    },
    {
      "value": "Abandoned",
      "count": 1,
      "fraction": 0.19,
      "in_wiki": false,
      "description": "Abandoned",
      "internal_val": "2"
    },
    {
      "value": "Destroyed",
      "count": 1,
      "fraction": 0.19,
      "in_wiki": false,
      "description": "Destroyed",
      "internal_val": "3"
    },
    {
      "value": "Dismantled",
      "count": 1,
      "fraction": 0.19,
      "in_wiki": false,
      "description": "Dismantled",
      "internal_val": "4"
    },
    {
      "value": "Fully Functional",
      "count": 1,
      "fraction": 0.19,
      "in_wiki": false,
      "description": "Fully Functional",
      "internal_val": "6"
    },
    {
      "value": "Damaged",
      "count": 1,
      "fraction": 0.19,
      "in_wiki": false,
      "description": "Damaged",
      "internal_val": "13"
    }
  ]
}
```

___
## GET /taginfo/keys/all

### URL
http://localhost:8094/taginfo/keys/all?fcode=AA040&page=1&query=&rawgeom=Point&rp=10&sortname=count_nodes&sortorder=desc&translation=MGCP

### Parameters
___

fcode | The feature code.
--- | --- 
_required_ | **Example: AA040**

___
page | The result set page number. (NOT SURE THIS IS IMPLEMENTED)
--- | --- 
_required_ | **Example: 1**

___
query | The query for tag values. (NOT SURE THIS IS IMPLEMENTED).
--- | --- 
_required_ | **Example: Damaged**

___
rawgeom | The geometry of the feature.
--- | --- 
_required_ | **Example: Point**

___
rp | The results per page. (NOT SURE THIS IS IMPLEMENTED)
--- | --- 
_optional_ | **Example: 25**

___
sortname | The property to sort. (NOT SURE THIS IS IMPLEMENTED)
--- | --- 
_optional_ | **Example: count_nodes**

___
sortorder | The sort order [asc or desc]. (NOT SURE THIS IS IMPLEMENTED).
--- | --- 
_optional_ | **Example: desc**

___
translation | The attribute schema name.
--- | --- 
_required_ | **Example: TDSv61**

___

### Response
```
{
  "page": 1,
  "rp": 15,
  "total": 15,
  "url": "",
  "data": [
    {
      "key": "ACC",
      "count_all": 100001,
      "count_all_fraction": 0.1,
      "count_nodes": 100001,
      "count_nodes_fraction": 0.1,
      "count_ways": 100001,
      "count_ways_fraction": 0.1,
      "count_relations": 100001,
      "count_relations_fraction": 0.1,
      "values_all": 100,
      "users_all": 100,
      "in_wiki": false,
      "in_josm": false
    },
    {
      "key": "CCN",
      "count_all": 100001,
      "count_all_fraction": 0.1,
      "count_nodes": 100001,
      "count_nodes_fraction": 0.1,
      "count_ways": 100001,
      "count_ways_fraction": 0.1,
      "count_relations": 100001,
      "count_relations_fraction": 0.1,
      "values_all": 100,
      "users_all": 100,
      "in_wiki": false,
      "in_josm": false
    },
    {
      "key": "FCODE",
      "count_all": 100001,
      "count_all_fraction": 0.1,
      "count_nodes": 100001,
      "count_nodes_fraction": 0.1,
      "count_ways": 100001,
      "count_ways_fraction": 0.1,
      "count_relations": 100001,
      "count_relations_fraction": 0.1,
      "values_all": 100,
      "users_all": 100,
      "in_wiki": false,
      "in_josm": false
    },
    .
    .
    .
  ]
}
```
___

