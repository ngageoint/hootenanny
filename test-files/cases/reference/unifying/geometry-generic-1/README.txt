This tests generic geometry conflation, which is conflation meant to occur against features that are not conflatable by any specific conflation
routine (e.g. Road, Building, etc.). You can check the hoot:MatchedBy tag to ensure generic geometry scripts are not conflating features that
specific routines are meant to.

Here is a description for each feature pair in the test input (see the note tag on each):

Points

A/B - These are two POIs that are similar enough to generate a review with POI to POI conflation.
C/D - These are two points with with types and geometries similar enough to conflate with Generic Point conflation and are not covered by any 
      specific conflation routine.
E/F - These are two points with with types similar enough but geometries not similar enough to conflate with Generic Point conflation and 
      are not covered by any specific conflation routine.
G/H - These are two points with with geometries similar enough but types not similar enough to conflate with Generic Point conflation and 
      are not covered by any specific conflation routine.

Lines

A/B - These are two identical roads that should conflate with Road conflation.
C/D - These are two lines with types and geometries similar enough to conflate with Generic Line conflation and are not covered by any specific
      conflation routine.
E/F - These are two lines with types similar enough but geometries not similar enough to conflate with Generic Line conflation and are not
      covered by any specific conflation routine.
G/H - These are two lines with geometries similar enough but types not similar enough to conflate with Generic Line conflation and are not
      covered by any specific conflation routine.

Polygons

A/B - These are two buildings that are similar enough to match with Building conflation.
C/D - These are two polygons with types and geometries similar enough to conflate with Generic Polygon conflation and are not covered by any 
      specific conflation routine.
E/F - These are two polygons with types similar enough but geometries not similar enough to conflate with Generic Polygon conflation and are not
      covered by any specific conflation routine.
G/H - These are two polygons with geometries similar enough but types not similar enough to conflate with Generic Polygon conflation and are not
      covered by any specific conflation routine.

Points and Polygons

I/J - This is a point/polygon pair with types and geometries similar enough to conflate with Generic Point/Polygon conflation and are not 
      covered by any specific conflation routine.
K/L - This is a point/polygon pair with types and geometries similar enough to conflate with Generic Point/Polygon conflation and are not 
      covered by any specific conflation routine.
M/N - This is a point/polygon pair with types and geometries similar enough to conflate with Generic Point/Polygon conflation and are not 
      covered by any specific conflation routine.
O/P - This is a point/polygon pair with types and geometries similar enough to conflate with Generic Point/Polygon conflation and are not 
      covered by any specific conflation routine.
Q/R - This is a point/polygon pair with geometries similar enough but types not similar enough to conflate with POI/Polygon conflation.
S/T - This is a point/polygon pair with types similar enough but geometries not similar enough to conflate with Generic Point/Polygon conflation.
U/V - This is a point/polygon pair with geometries similar enough but types not similar enough to conflate with POI/Polygon conflation.


