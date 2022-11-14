//assign query to ref
._->.ref;

//get ref nodes outside of bbox
(node(w.ref); - node(w.ref)({{bbox}});)->.oobnd;

//get ways referencing out of bounds nodes
way(bn.oobnd);

//recurse to get nodes of connected ways
(._;>;)->.connectedWays;

//union ref and oob connected ways
(.ref; .connectedWays;);

out meta;
