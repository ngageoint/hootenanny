This is a Differential Conflation test to ensure that non-conflatable relations with conflatable child relation members are not thrown out when
the `differential.remove.unconflatable.data` option is enabled. 

The large building in the reference layer is a member of a multipolygon relation that is not tagged as a building or anything hoot can
conflate. Prior to this test, the NonConflatableElementRemover run by Differential Conflation would find the relation to be unconflatable and
remove it, as well as all of its children when `differential.remove.unconflatable.data=true`. So, we'd lose the large building from the 
differential output. With the added option `non.conflatable.criterion.ignore.relation.members=false` (default) NonConflatableCriterion won't
allow the relation to be deemed non-conflatable since it has a conflatable building way child.
