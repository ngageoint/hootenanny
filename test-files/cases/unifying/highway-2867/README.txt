This is a multi-part test case that tests the following:

1) NOTE: This case is broken now after recent changes and needs to be fixed.

The road named Naseemee Hingun in the secondary dataset should have its tags transferred over to the corresponding reference feature that doesn't have any tags, and the geometry of the reference feature should be preserved with no splits.  The nearby roads named Miriyas Magu and Dhakandhaa Magu should contain splits because that reference road data had its geometry extended by features in the secondary dataset.

2) The road named Majeedhee Magu should extend across the entire width of the island (from node -930 to node -274) and have some tags 
transferred over to it from the secondary dataset.

3) The road named Boduthakurufaanu magu should extend from node -5812 to node -347 and have some tags transferred over to it from the 
secondary dataset.

4) The road named Falhumathee Magu should extend from node -1745 to node -962 and have some tags transferred over to it from the 
secondary dataset.

Note that for none of these will the road type from the secondary dataset override that in the reference dataset since we are using regular
Unifying Conflation here and not Attribute Conflation (see highway-2888).


