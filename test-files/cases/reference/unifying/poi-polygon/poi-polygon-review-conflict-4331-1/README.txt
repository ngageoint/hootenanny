This tests that a POI/Polygon match does not leave an orphaned node when the match involves a POI also involved in a POI/POI review. The output 
should contain a single review between the Abu Bakr Siddiq Mosque POI and the only poly. PoiPolygonInvalidReviewNodeRemover does the cleanup on
the matched unamed POI. 

Its notable that the argument could be made here for three reviews instead: one inter-dataset POI/Poly match, one intra-dataset POI/Poly match, 
and one POI/POI match. That type of change is possible but may increase POI/POI and POI/Polygon reviews when those matchers are run together. 
See notes in PoiPolygonMergerCreator.
