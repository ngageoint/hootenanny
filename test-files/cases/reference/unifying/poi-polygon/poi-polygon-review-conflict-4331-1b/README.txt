This tests that a POI/Polygon match does not leave an orphaned node when the match involves a POI also involved in a POI/POI review. This test
changes the default behavior for POI/Polygon Conflation by forcing any POI/Polygon matches with a feature also involved in a POI/POI match to
become reviews. The output should have three reviews: one inter-dataset POI/Poly match, one intra-dataset POI/Poly match, and one POI/POI match.
