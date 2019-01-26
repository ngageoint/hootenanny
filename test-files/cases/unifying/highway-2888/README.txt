The test uses Attribute Conflation where attributes are copied from the secondary dataset to the reference dataset, but the geometries of 
features in the reference dataset are not modified.  

One specific feature it looks at is the road named Boduthakurufaanu magu.  That road should have all tags transferred from the secondary 
feature to the output feature.  The "length" tag in particular caused problems with this output feature initially, but logic was added to
sum the length tags during post conflation way joining.  In general, all roads in the reference dataset should get tag only updates if they
can be matched to a corresponding feature in the secondary dataset.  

NOTE: Would like to make the input datasets smaller for test runtime performance purposes but had difficulty reproducing the desired 
result after cropping them down.
