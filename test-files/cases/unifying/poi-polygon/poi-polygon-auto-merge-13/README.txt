A bug in the address parsing originally caused one of the POI way nodes to match the polygon on address and merge into it. Neither of the POI 
way nodes should merge into the polygon since there are more than one of them and auto merge is turned of.  The nodes should both generate a 
review with the polygon.


