
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
 * elements (way nodes, relation members) or an invalid map passed in without the constituent
 * elements.  It can do this because only the parent elements are needed for actual merging.  The
 * invalid map input case is necessary b/c the UI sends the server only the features that need
 * merging and then handles removing any constituent features itself after the merge with a call to
 * the OSM services.  Technically, we have no client code sending in valid maps with constituent
 * features for merging yet, but since that could change, and its easy to support, we'll supporting
 * both types of inputs.
 *
 * This class has a mix of functionality where the merging is done by hoot-js calls into generic
 * scripts and functionality where the merging is done strictly by hoot-core code.  Arguably, you
 * could do all the merging via hoot-core code which would make the workflow simpler, make the code
 * easier to read, and avoid unnecessary calls out to Javascript.  However, since the generic
 * scripts have their own merge functions already defined that users may want to customize, for c
 * onsistency's sake it makes more sense to use this hybrid approach.  The downside to this approach
 * is that if we ever want to expose element merging outside of a js script (command line), we
 * won't be able to do it for any merging that uses the script logic.  If having a hoot merge
 * command ever becomes a priority, then we can think about converting the script merge logic in
 * this class to hoot-core merge logic, moving all of this class's merge logic to hoot-core, and
 * then wrapping calls to the hoot-core class with this class.
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
  //an unlimited number of POIs may be merged
  static void _mergePois(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current);
  //an unlimited number of areas may be merged
  static void _mergeAreas(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current);
  //an unlimited number of buildings may be merged
  static void _mergeBuildings(OsmMapPtr map, const ElementId& mergeTargetId);
  //only a single POI and polygon may be merged (support for multiple POIs into a single or
  //multiple polygons could be possible, if necessary); returns the merge target id
  static ElementId _mergePoiAndPolygon(OsmMapPtr map);

  static ElementId _getMergeTargetFeatureId(ConstOsmMapPtr map);
  static void _validateMergeTargetElement(ConstOsmMapPtr map, const MergeType& mergeType);
  static QString _mergeTypeToString(const MergeType& mergeType);
};

}

#endif // ELEMENTMERGERJS_H
