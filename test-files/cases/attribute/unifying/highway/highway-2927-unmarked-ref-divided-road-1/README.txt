This is an Attribute Conflation test with the Unifying Roads Algorithm for ensuring small section of road at the start of unmarked 
reference divided highways are properly joined into connecting roads

The road Av. Baralt splits into two divided roads in one section with a sidewalk in between them.  Since the reference geometry on one side
of the divided road pair is smaller than the other, the smaller section is unable to match with any secondary feature.  Previously, this smaller
section of road would not pick up any road type.  By enabling aggressive way joining after conflation 
(attribute.conflation.aggressive.highway.joining) we are able add type tags to it and other roads like it.  However, this may be done at the 
expensive of some false positive road type tag additions.
