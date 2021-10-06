This is an Attribute Conflation test with the Network Roads algorithm for retaining way tags for 
ways added to a multilinestring relation during the handling of bridges.

The road Autopista: Francisco Fajardo should retain its road type tags.  This road gets added to a 
multilinestring relation during conflation due to the fact a bridge exists in the middle of it. 
Previously, the Eastern end of the road was losing its type tags.

TODO: Note that the Eastern end of the road is a one way street pointed in the wrong direction. That 
is a bug that still needs to be fixed. There are also several missed matches in this version that 
aren't in the Unifying version. Fix as part of #3137.
