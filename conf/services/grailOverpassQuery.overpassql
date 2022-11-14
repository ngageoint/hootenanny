[out:json];
(node({{bbox}});<;>;)->.all;
(
   relation({{bbox}})[natural=coastline];>;
   way({{bbox}})[natural=coastline];<;>;
   relation({{bbox}})[boundary=administrative];>;
   way({{bbox}})[boundary=administrative];<;>;
   node({{bbox}})[boundary=administrative];
)->.exclude;
(.all; - .exclude;);
out meta;