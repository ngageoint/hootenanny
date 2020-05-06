This tests to see that duplicate overlapping roads with similar, but not exact tags, are removed by the DuplicateWayRemover post conflate op 
when strict tag matching is turned off. In the output, look at the section of road named "Unyama Pageya Road" that has the tag 
"note=check_me". If you delete that road, you should not see a road of identical size hidden underneath it.
