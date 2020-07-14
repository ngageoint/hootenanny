This is an Differential Conflation test to ensure that building multipolygon members do not have shared relation roles and that no invalid roles
are present. 

These errors were found with JOSM validation. A Differential Conflation job was used to discover the test but it most likely could
be caused by any of the conflation workflows.

The first input map is empty and the second contains a single multipolygon relation for a building. The output should contain a single 
multipolygon relation made up of inner and outer roles only. None of the feature members should be repeated with different roles and the outline
role should not be present.
