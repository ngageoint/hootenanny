A bug in the PoiPolygonAddressScoreExtractor originally caused one of the POI way nodes to match the polygon on address and merge into it.
Neither of the POI way nodes should merge into the polygon since there are more than one of them.


