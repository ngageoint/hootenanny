
#ifndef ELEMENTMERGERJS_H
#define ELEMENTMERGERJS_H

#include <hoot/js/HootJsStable.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/core/OsmMap.h>

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

  static MergeType _determineMergeType(ConstOsmMapPtr map, QString& scriptPath);

  static OsmMapPtr _mergeElements(OsmMapPtr map, Persistent<Object> plugin);
  static OsmMapPtr _mergePois(OsmMapPtr map, Persistent<Object> plugin);
  static OsmMapPtr _mergeAreas(OsmMapPtr map, Persistent<Object> plugin);

  static bool _containsOnlyPois(ConstOsmMapPtr map);
  static bool _containsTwoBuildings(ConstOsmMapPtr map);
  static bool _containsTwoOrMoreAreas(ConstOsmMapPtr map);
  static bool _containsOnePolygonAndOneOrMorePois(ConstOsmMapPtr map);
};

}

#endif // ELEMENTMERGERJS_H
