See note in PoiPolygonConflateAddressTest to why this has temporarily been disabled.

There is a specific use case where you have POIs with only an address but missing type information, generically typed buildings,
the features are overalapping, and there is roughly a 1:1 correlation between the two.  In this case, even with the weak
association, we want these to conflate.  So, we tweak the allow any points with an address to automatically be considered
POIs, and we lower the match evidence threshold.


