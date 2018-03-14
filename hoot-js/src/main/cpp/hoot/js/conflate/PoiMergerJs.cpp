#include "PoiMergerJs.h"

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

void PoiMergerJs::mergePois(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current)
{
  LOG_INFO("Merging POIs...");

  // Instantiate script merger
  boost::shared_ptr<PluginContext> script(new PluginContext());
  v8::HandleScope handleScope(current);
  v8::Context::Scope context_scope(script->getContext(current));
  script->loadScript(ConfPath::search("PoiGeneric.js", "rules"), "plugin");
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

  // Got in Map with POIs A, B, C, D, E
  //
  // Make a set of pairs to indicate all are same object:
  //   A->B, A->C, A->D, A->E
  //
  // ...then pass those pairs one at a time through the merger, since it only merges pairs
  int poisMerged = 0;
  const NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    const ConstNodePtr& node = it->second;
    if (node->getId() != mergeTargetId.getId() && OsmSchema::getInstance().isPoi(*node))
    {
      LOG_VART(node);

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(std::pair<ElementId,ElementId>(mergeTargetId, node->getElementId()));
      // Now create scriptmerger, and invoke apply method which will apply merge
      // transformation, reducing the POIs down to one.
      ScriptMerger merger(script, plugin, matches);
      std::vector< std::pair< ElementId, ElementId > > replacedNodes;
      merger.apply(map, replacedNodes);
      LOG_VART(replacedNodes.size());

      poisMerged++;
    }
  }
  LOG_INFO("Merged " << poisMerged << " POIs.");
}

}
