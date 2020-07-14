Attribute Conflation test with the Unifying Roads Algorithm for retaining road types when a single named road has different highway 
classification as you travel along it.

The road named Av. Caracas, as you travel from West to East, should retain its highway=secondary tag on the Western portion and its 
highway=residential tags on the Eastern portions.  Previously, the way joiner was extending the highway=secondary portion onto part of one 
of the highway=residential portions.

