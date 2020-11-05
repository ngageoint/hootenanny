This tests that a POI/Polygon match does not leave an orphaned node when the match involves a POI also involved in a POI/POI review. The default
behavior for POI/Polygon conflation is to keep matches even when they have a feature involved in a POI/POI review. The output should contain a 
single review between the Abu Bakr Siddiq Mosque POI and the only poly. PoiPolygonInvalidReviewNodeRemover does the cleanup on the matched 
unamed POI. 
