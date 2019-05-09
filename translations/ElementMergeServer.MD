# Element Merge Server API

#### Merges two or more OSM features.  Currently supports merging POIs with POIs, one POI with one polygon, areas 
#### with areas, or buildings with buildings.  The element being merged into must have the "hoot:merge:target=yes" tag, 
#### except for POI/polygon merging which always merges the POI into the polygon.  
####
#### Constituent elements (way nodes or relation members) are optional in the request.  Whether they are passed in 
#### the request or not, the caller is responsible for deleting the ones that belong to any parent features that 
#### were merged into other features.
___
## POST /elementmerge

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
