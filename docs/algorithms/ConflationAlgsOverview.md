
# Conflation Algorithms

## Specific Algorithms
* **[Area Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#area-to-area-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/AreaToAreaConflation.asciidoc)) conflates non-building polygons supporting an
area geometry (e.g. park, parking lot, etc.; also includes area=yes and derivatives) using a machine learning based approach.
* **[Building Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#building-conflation) via script** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/BuildingConflation.asciidoc)) conflates building polygons using a machine learning based approach.
* **[POI Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#poi-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/PoiToPoiConflation.asciidoc)) conflates points falling into a Point of Interest (POI) schema category or points with names using a machine learning based approach via script.
* **[POI to Polygon Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#poi-to-polygon-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/PoiToPolygonConflation.asciidoc)) conflates points falling into a Point of Interest (POI) schema category or points with names with either buildings, areas, or polygons that fall into either a building or POI related schema category using a rules based approach.
* **[Power Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#power-line-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#power-line-conflation)) conflates linear power
line features using a machine learning based approach via script.
* **Railway Conflation** conflates linear railway features using a machine learning based approach that extends [Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/ScriptConflation.asciidoc).
* **[River Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#river-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#river-conflation)) conflates linear waterway features using a machine learning based approach via script.
* **[Road Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/RoadConflation.asciidoc#road-conflation)** conflates
linear road features with one of the following algorithms:
  * The **[Unifying (aka 2nd Generation) Roads Algorithm](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/UnifyingConflation.asciidoc)**, which uses a machine learning based approach.
  * The **[Network Roads Algorithm](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/NetworkConflation.asciidoc)**, which uses a graph theory based approach.

## Generic Algorithms
* **[Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#generic-geometry)** is a bare bones algorithm used to match generic linear features for which Hootenanny has no custom matching algorithm available. 
* **[Generic Point Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#generic-geometry)** is a bare bones algorithm used to match generic point features for which Hootenanny has no custom matching algorithm available.
* **[Generic Point to Polygon Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#generic-geometry)** is a bare bones algorithm used to match generic point features with generic polygon features both of which Hootenanny has no custom matching algorithm available. 
* **[Generic Polygon Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#generic-geometry)** is a bare bones algorithm used to match generic polygon features for which Hootenanny has no custom matching algorithm available. 
