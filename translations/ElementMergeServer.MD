# Element Merge Server API

#### This feature merging service is primarily used to merge features during the conflation manual 
#### review process.
####
#### It supports merging the following feature types:
#### * one or more areas into one area
#### * one or more buildings into one building
#### * one or more POIs into one POI
#### * one POI into one polygon
#### * one or more railways into one railway
####
#### The merger will select the appropriate feature merge type based on the elements contained in 
#### the map passed into it. The 'hoot:status' tag must be present on the features being merged.
#### The element being merged into must also have the "hoot:merge:target=yes" tag with the following 
#### exceptions where the tag is not needed and ignored: 
#### * POI/Polygon merging always merges the POI into the polygon. 
#### * Railway merging where features are involved in a One To Many match always merges secondary 
####   features into reference or previously conflated features (status=2 into status=1 or 3).
####
#### Constituent elements (way nodes or relation members) are optional in the request for polygon 
#### features. They are, however, required for linear features. Whether they are passed in with the 
#### request for polygon features or not, the caller is responsible for deleting those constituent 
#### features that belong to any parent features that were merged into other features.
####
#### Railway One To Many merging additional notes:
#### * The metadata tag: 'hoot:railway:one:to:many:match:secondary=yes' must remain present on 
####   secondary features involved in a one to many match in order for this type of merging to work 
####   properly.
#### * The merging process deletes the secondary way after merging. If all features to be merged are 
####   passed in at once, this should not cause any problems. However, if one to many reviews are 
####   being merged one at a time, then the secondary way being merged should not be deleted on the 
####   client end until it is no longer involved in reviews.
___
## POST /elementmerge

### Example Body #1

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

### Example Response #1
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

### Example Body #2

```
TODO: no constituents
```

### Example Response #2
```
TODO: no constituents
```

### Example Body #3

```
TODO: one to many rail
```

### Example Response #3
```
TODO: one to many rail
```
___
