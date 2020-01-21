
# Conflation Algorithms

* **[Area Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#area-to-area-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/AreaToAreaConflation.asciidoc)) conflates non-building polygons supporting an
area geometry (e.g. park, parking lot, etc.; also includes area=yes and derivatives) and uses a machine learning based approach that extends 
[Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).
* **[Building Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#building-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/BuildingConflation.asciidoc)) uses a machine learning based approach to conflate building polygons.
* **[Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc)** is a bare bones algorithm extending used to match linear features for which Hootenanny has no custom matching algorithm available.
* **[Generic Point Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc)** is a bare bones algorithm used to match point features for which Hootenanny has no custom matching algorithm available.
* **[Generic Polygon Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc)** is a bare bones algorithm used to match polygon features for which Hootenanny has no custom matching algorithm 
available. 
* **[POI Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#poi-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/PoiToPoiConflation.asciidoc)) conflates Points of Interest (named points or those falling
into a POI schema category) and uses a machine learning based approach that extends [Generic Point Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).
* **[POI to Polygon Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#poi-to-polygon-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/PoiToPolygonConflation.asciidoc)) conflates POIs with either buildings 
or area related polygons and uses a rules based approach. 
* **[Power Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/GenericLineConflation.asciidoc#power-line-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#power-line-conflation)) conflates linear power
line features and uses a machine learning based approach that extends [Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).
* **Railway Conflation** conflates linear railway features and uses a machine learning based approach that extends [Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).
* **[River Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/GenericLineConflation.asciidoc#river-conflation)** ([detail](https://github.com/ngageoint/hootenanny/blob/master/docs/user/OldDocs.asciidoc#river-conflation)) conflates linear waterway features and 
uses a machine learning based approach that extends [Generic Line Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyConflatingANewFeatureTypeWithGenericConflation.asciidoc).
* **[Road Conflation](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/RoadConflation.asciidoc#road-conflation)** conflates
linear road features with one of the following algorithms:
  * The **[2nd Generation (aka Unifying) Roads Algorithm](https://github.com/ngageoint/hootenanny/blob/develop/docs/algorithms/UnifyingConflation.asciidoc)** uses machine learning techniques based on manually matched training data to match linear roads.
  * The **[Network Roads Algorithm](https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/NetworkConflation.asciidoc)** (default) uses a graph theory derived approach to match linear roads.
