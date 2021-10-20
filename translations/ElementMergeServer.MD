# Element Merge Server API

#### This feature merging service is primarily used to merge features during the conflation manual 
#### review process. Merges two or more OSM features.
####
#### Supports merging the following feature types:
#### * areas with areas
#### * buildings with buildings
#### * POIs with POIs
#### * one POI with one polygon
#### * railways with railways
####
#### The merger will select the appropriate merge type based on the elements contained in the map 
#### passed to it.
#### The element being merged into must have the "hoot:merge:target=yes" tag with the following 
#### exceptions where the tag is not needed and is ignored: 
#### * POI/Polygon merging always merges the POI into the polygon 
#### * Railway merging where features are involved in a One To Many match always merges secondary 
####   features in to reference features (status=2 into status=1). The metadata tag:
####   'hoot:railway:one:to:many:match:secondary=yes' must not be removed from secondary features in 
####   order for this type of merging to work.
####
#### Constituent elements (way nodes or relation members) are optional in the request. Whether they
#### are passed in the request or not, the caller is responsible for deleting those that belong to 
#### any parent features that were merged into other features.
___
## POST /elementmerge?mergeMode=

### Body

```
<?xml version='1.0' encoding='UTF-8'?>
<osm version='0.6' upload='true' generator='hootenanny'>
    <node id='-3200079' visible='true' lat='48.0479399' lon='11.7012814'>
        <tag k='amenity' v='post_office' />
        <tag k='error:circular' v='15' />
        <tag k='hoot:status' v='1' />
    </node>
    <node id='-3200083' visible='true' lat='48.04854' lon='11.70194'>
        <tag k='amenity' v='post_office' />
        <tag k='error:circular' v='150' />
        <tag k='name' v='POST, JÄGER-VON-FALL-STRASSE' />
        <tag k='hoot:status' v='2' />
        <tag k="hoot:merge:target" v="yes"/>
    </node>
</osm>
```

### Response
```
<?xml version="1.0" encoding="UTF-8"?>
<osm version="0.6" generator="hootenanny" srs="+epsg:4326">
    <node visible="true" id="-1" timestamp="1970-01-01T00:00:00Z" version="1" lat="48.0479399000000029" lon="11.7012813999999992">
        <tag k="hoot:status" v="3"/>
        <tag k="name" v="POST, JÄGER-VON-FALL-STRASSE"/>
        <tag k="amenity" v="post_office"/>
        <tag k="error:circular" v="15"/>
    </node>
</osm>
```
___
