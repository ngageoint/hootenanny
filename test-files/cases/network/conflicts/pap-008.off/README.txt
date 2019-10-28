Roads Lalue and Rue Fernand should conflate between the maps. They do in some parts but unfortunately not in all. Zoom in very closely when 
checking for merges, otherwise its not obvious two of the roads didn't merge.

Notes: 

1) This test requires network.edge.match.set.finder.max.iterations to be set to 20 currently. Other tests will pass with the value as low
as 12. Unfortunately, this higher number of iterations contributes to a severe bottleneck in the algorithm performance for larger datasets.

2) This test was de-activated as part of 3570 and needs to be fixed. See notes in HighwayCriterion.
