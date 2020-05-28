This is an Attribute Conflation test to ensure that building pairs where one member of the pair is completely contained by the other are always
matched and not reviewed. 

The configuration option, building.force.contained.match, makes this possible. The tags of the secondary building, "Building 1", should be 
merged into all three buildings matched with in the reference layer (one of which is a multipoly building). The building from the reference
dataset and all three buildings from the secondary dataset should become part of the same mutipolygon relation in the output.
