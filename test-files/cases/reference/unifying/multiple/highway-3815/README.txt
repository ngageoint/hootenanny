This tests road in a relation of type != multilinestring. Prior to #3815 hoot didn't allow them to 
be conflated, and now it does. 

Note the poorly merged section of road in the type=route relation flagged to be worked on in #3820.

Validation reports aren't consistent currently, so disabling validation for this test.
