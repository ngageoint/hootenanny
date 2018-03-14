#include "AreaMergerJs.h"

// Hoot
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/conflate/js/ScriptMerger.h>
#include <hoot/core/schema/OsmSchema.h>

// Qt
#include <QString>

// std
#include <set>
#include <utility>

using namespace v8;

namespace hoot
{

void AreaMergerJs::mergeAreas(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current)
{
  LOG_INFO("Merging areas...");

  // Instantiate script merger
  boost::shared_ptr<PluginContext> script(new PluginContext());
  v8::HandleScope handleScope(current);
  v8::Context::Scope context_scope(script->getContext(current));
  script->loadScript(ConfPath::search("Area.js", "rules"), "plugin");
  v8::Handle<v8::Object> global = script->getContext(current)->Global();
  if (global->Has(String::NewFromUtf8(current, "plugin")) == false)
  {
    throw IllegalArgumentException("Expected the script to have exports.");
  }
  Handle<Value> pluginValue = global->Get(String::NewFromUtf8(current, "plugin"));
  Persistent<Object> plugin(current, Handle<Object>::Cast(pluginValue));
  if (plugin.IsEmpty() || ToLocal(&plugin)->IsObject() == false)
  {
    throw IllegalArgumentException("Expected plugin to be a valid object.");
  }

  int areasMerged = 0;

  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const ConstWayPtr& way = it->second;
    LOG_VART(way->getId());
    LOG_VART(OsmSchema::getInstance().isNonBuildingArea(way));
    if (way->getElementId() != mergeTargetId && OsmSchema::getInstance().isNonBuildingArea(way))
    {
      LOG_TRACE("Merging way area: " << way << " into " << mergeTargetId);

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(std::pair<ElementId,ElementId>(mergeTargetId, ElementId::way(way->getId())));
      // Now create scriptmerger, and invoke apply method which will apply merge
      // transformation, reducing the areas down to one.
      ScriptMerger merger(script, plugin, matches);
      std::vector< std::pair< ElementId, ElementId > > replacedWays;
      merger.apply(map, replacedWays);
      LOG_VART(replacedWays.size());

      areasMerged++;
    }
  }

  const RelationMap relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const ConstRelationPtr& relation = it->second;
    LOG_VART(relation->getId());
    LOG_VART(OsmSchema::getInstance().isNonBuildingArea(relation));
    if (relation->getElementId() != mergeTargetId &&
        OsmSchema::getInstance().isNonBuildingArea(relation))
    {
      LOG_TRACE("Merging relation area: " << relation << " into " << mergeTargetId);

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(
        std::pair<ElementId,ElementId>(mergeTargetId, ElementId::relation(relation->getId())));
      // Now create scriptmerger, and invoke apply method which will apply merge
      // transformation, reducing the areas down to one.
      ScriptMerger merger(script, plugin, matches);
      std::vector< std::pair< ElementId, ElementId > > replacedRelations;
      merger.apply(map, replacedRelations);
      LOG_VART(replacedRelations.size());

      areasMerged++;
    }
  }

  LOG_INFO("Merged " << areasMerged << " areas.");
}

}
