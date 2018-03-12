
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
 *
 * This class will handle either the case where a valid map is passed in with all constituent
 * elements (way nodes, relation members) or an invalid map is passed without the constituent
 * elements and only the parent elements to be merged.  The invalid map input case is necessary b/c
 * the UI sends the server only the features that need merging and then handles removing any
 * constituent features itself afterward with a the merge with a call to the OSM services.
 *
 * This class has a mix of functionality where the merging is done strictly by hoot-js code or
 * the merging is done by hoot-core code.  Arguably, you could do all the merging via hoot-core code
 * and it might be more easily read, but in the case of generic conflation having this class
 * merge using the hoot-js code keeps a single code path for auto-merging at conflate time and
 * manual merge at review time.
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
  static bool _containsOnePolygonAndOnePoi(ConstOsmMapPtr map);
  static bool _containsPolys(ConstOsmMapPtr map);
  static bool _containsAreas(ConstOsmMapPtr map);
  static bool _containsBuildings(ConstOsmMapPtr map);
  static bool _containsPois(ConstOsmMapPtr map);

  static QString _mergeTypeToString(const MergeType& mergeType);
};

}

#endif // ELEMENTMERGERJS_H
