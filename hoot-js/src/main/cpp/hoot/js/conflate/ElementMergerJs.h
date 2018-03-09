
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

  ElementMergerJs();
  ~ElementMergerJs();

  static void _jsElementMerge(const v8::FunctionCallbackInfo<v8::Value>& args);

  static MergeType _determineMergeType(ConstOsmMapPtr map);

  static void _mergeElements(OsmMapPtr map, const MergeType& mergeType, Isolate* current);
  static void _mergePois(OsmMapPtr map, Isolate* current);
  static void _mergeAreas(OsmMapPtr map, Isolate* current);
  static ElementId _getMergeTargetFeatureId(ConstOsmMapPtr map);

  static bool _containsOnlyTwoOrMorePois(ConstOsmMapPtr map);
  static bool _containsTwoBuildings(ConstOsmMapPtr map);
  static bool _containsTwoOrMoreAreas(ConstOsmMapPtr map);
  static bool _containsOnePolygonAndOneOrMorePois(ConstOsmMapPtr map);
  static void _validateMergeTargetElement(ConstOsmMapPtr map, const MergeType& mergeType);
};

}

#endif // ELEMENTMERGERJS_H
