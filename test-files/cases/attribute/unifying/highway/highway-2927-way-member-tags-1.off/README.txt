TODO: fix me

This is an Attribute Conflation test for retaining way tags for ways added to a multilinestring relation during the handling of bridges.

The road Autopista: Francisco Fajardo should retain its road type tags.  This road gets added to a multilinestring relation
during conflation due to the fact a bridge exists in the middle of it.  Previously, the Eastern end of the road was losing its type tags.

Note that the Eastern end of the road is a one way street pointed in the wrong direction.  This is a bug that still needs to be fixed.
