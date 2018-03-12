
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
  static void _mergePois(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current);
  static void _mergeAreas(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current);
  static void _mergeBuildings(OsmMapPtr map, const ElementId& mergeTargetId);
  static void _mergePoiAndPolygon(OsmMapPtr map, const ElementId& mergeTargetId);

  static ElementId _getMergeTargetFeatureId(ConstOsmMapPtr map);
  static void _validateMergeTargetElement(ConstOsmMapPtr map, const MergeType& mergeType);

  static bool _containsTwoOrMorePois(ConstOsmMapPtr map);
  static bool _containsTwoOrMoreBuildings(ConstOsmMapPtr map);
  static bool _containsTwoOrMoreAreas(ConstOsmMapPtr map);
  //static bool _containsOnePolygonAndOneOrMorePois(ConstOsmMapPtr map);
  static bool _containsOnePolygonAndOnePoi(ConstOsmMapPtr map);
  static bool _containsPolys(ConstOsmMapPtr map);
  static bool _containsAreas(ConstOsmMapPtr map);
  static bool _containsBuildings(ConstOsmMapPtr map);
  static bool _containsPois(ConstOsmMapPtr map);

  static QString _mergeTypeToString(const MergeType& mergeType);
};

}

#endif // ELEMENTMERGERJS_H
