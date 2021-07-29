See note in PoiPolygonConflateAddressTest to why this has temporarily been disabled.

This tests that we favor a POI/Polygon match over a POI/POI review and that the POI/POI review 
doesn't keep the POI/Poly match from happening. All the POI/Poly storage tanks matches should merge 
and any POI/POI reviews involving the merge storage tank POIs that existed before those merges 
should be removed. See notes in poi-polygon-review-conflict-4331-1 as to other ways we could handle 
this.
