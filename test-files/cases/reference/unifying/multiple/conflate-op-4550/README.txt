This tests that conflate ops that operate on generically typed features do not modify features that aren't configured to be
conflated as part of the current conflation configuration. For this test, we turn on only POI/Polygon conflation. We 
check to see that no unrelated feature were modified by DuplicateNameRemover and DuplicateNodeRemover (ops causing 
unnecessary changes before this fix). Note that the generic matchers (point/line/poly/pointpoly) must be turned off in order 
for this test to pass. If they are turned on, the ops in question will modify unrelated features since they're configured to 
work against generically typed elements.

Specifically, check the road, "Заводская улица", with 12 nodes. It should not share a node with the landuse=residential poly
it cross over just before connecting with the road, "Железнодорожная улица" (DuplicateNodeRemover caused this to happen
previously).

Also, check the road, "Садовая улица", with 11 nodes. It should not have the tag, "atl_name" (DuplicateNameRemover caused this
to happen previously).


