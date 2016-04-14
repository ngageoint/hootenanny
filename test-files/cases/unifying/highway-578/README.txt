Tests road and footpath conflate.

* If two highways have significantly different enumerated types then 
  they will not be considered for match. For example: 
  "highway=primary" vs "highway=secondary" has a diff of 0.2.
  "highway=track" vs "highway=footway" has a diff of 0.2.
  "highway=road" vs "highway=footway" has a diff of 0.2.
  "highway=primary" vs "highway=footway" has a diff of 0.67.