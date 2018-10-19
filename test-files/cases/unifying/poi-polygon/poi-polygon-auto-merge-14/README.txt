A bug in the PoiPolygonAddressScoreExtractor originally caused one of the POI way nodes to match the polygon on address and merge 
into it because of the match.  Both of the POI way nodes should merge into the polygon based on auto-merge being enabled.


