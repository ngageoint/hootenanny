Attribute Conflation test for bridge merging (see option attribute.conflation.allow.ref.geometry.changes.for.bridges).  This test ensures that
any roads hoot puts into a multilinestring relation during geometry merging don't get dropped from the output.

The pieces of secondary roads running West to East on either side of the unmarked bridge (both have 
bdw-id={23973f69-7830-4ee2-be77-9592a2c17bf3}) should remain in the output.  Previously, they were being dropped due to their tags being 
removed by HighwaySnapMerger.

