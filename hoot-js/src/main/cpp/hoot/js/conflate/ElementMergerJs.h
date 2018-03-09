
#ifndef ELEMENTMERGERJS_H
#define ELEMENTMERGERJS_H

// Hoot
#include <hoot/js/HootJsStable.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/PluginContext.h>
#include <hoot/core/OsmMap.h>

using namespace v8;

namespace hoot
{

/**
 * Merges two or more elements based on their feature type
 */
class ElementMergerJs : public node::ObjectWrap
{

public:

 typedef enum MergeType
 {
   PoiToPoi = 0,
   PoiToPolygon,
   AreaToArea,
   BuildingToBuilding
  } MergeType;

 static void Init(v8::Handle<v8::Object> target);

private:

 friend class ElementMergerJsTest;

  ElementMergerJs();
  ~ElementMergerJs();

  /**
   *
   *
   * @param args
   */
  static void _jsElementMerge(const v8::FunctionCallbackInfo<v8::Value>& args);

  static MergeType _determineMergeType(ConstOsmMapPtr map);

  static void _mergeElements(OsmMapPtr map, Isolate* current);
  static void _mergePois(OsmMapPtr map, Isolate* current);
  static void _mergeAreas(OsmMapPtr map, Isolate* current);
  static void _mergeBuildings(OsmMapPtr map, const ElementId& mergeTargetId);
  /*
   * Merges a single POI with a single polygon, both as defined by PoiPolygonMerger
   *
   * Way polys do not have to have their way nodes passed in, and relations do not have to have
   * their relation members passed in.  The resulting invalid map does not affect the merger.  If
   * constituents of the input features are not passed in, the caller is responsible for deleting
   * the constituent way nodes/relation members.
   *
   * @param map an OSM map containing a single node POI and a single polygon that fits
   * PoiPolygonMatch's definition of a polygon
   */
  static void _mergePoiAndPolygon(OsmMapPtr map);
  static ElementId _getMergeTargetFeatureId(ConstOsmMapPtr map);

  static bool _containsOnlyTwoOrMorePois(ConstOsmMapPtr map);
  static bool _containsTwoBuildings(ConstOsmMapPtr map);
  static bool _containsTwoOrMoreAreas(ConstOsmMapPtr map);
  static bool _containsOnePolygonAndOneOrMorePois(ConstOsmMapPtr map);
  static void _validateMergeTargetElement(ConstOsmMapPtr map, const MergeType& mergeType);

  static QString _mergeTypeToString(const MergeType& mergeType);
};

}

#endif // ELEMENTMERGERJS_H
