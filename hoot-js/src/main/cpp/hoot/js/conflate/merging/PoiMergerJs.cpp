/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */
#include "PoiMergerJs.h"

// Hoot
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/util/ConfPath.h>

#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/conflate/merging/ScriptMerger.h>

// std
#include <utility>

using namespace v8;

namespace hoot
{

ElementId PoiMergerJs::merge(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current)
{
  LOG_INFO("Merging POIs...");

  // instantiate script merger
  std::shared_ptr<PluginContext> script = std::make_shared<PluginContext>();
  HandleScope handleScope(current);
  Context::Scope context_scope(script->getContext(current));
  Local<Context> context = current->GetCurrentContext();

  script->loadScript(ConfPath::search("Poi.js", "rules"), "plugin");
  Local<Object> global = script->getContext(current)->Global();
  if (global->Has(context, toV8("plugin")).ToChecked() == false)
    throw IllegalArgumentException("Expected the script to have exports.");

  Local<Value> pluginValue = global->Get(context, toV8("plugin")).ToLocalChecked();
  Persistent<Object> plugin(current, Local<Object>::Cast(pluginValue));
  if (plugin.IsEmpty() || ToLocal(&plugin)->IsObject() == false)
    throw IllegalArgumentException("Expected plugin to be a valid object.");

  // Got in Map with POIs A, B, C, D, E
  //
  // Make a set of pairs to indicate all are same object:
  //   A->B, A->C, A->D, A->E
  //
  // ...then pass those pairs one at a time through the merger, since it only merges pairs
  int poisMerged = 0;
  std::vector<std::pair<ElementId, ElementId>> replacedNodes;
  //  Make a copy of the node map so that the iterators work while merging
  const NodeMap nodes = map->getNodes();
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
  {
    const ConstNodePtr& node = it->second;
    if (node->getId() != mergeTargetId.getId() && PoiCriterion().isSatisfied(node))
    {
      LOG_VART(node);

      std::set<std::pair<ElementId, ElementId>> matches;
      matches.emplace(mergeTargetId, node->getElementId());
      // apply script merging
      ScriptMerger merger(script, plugin, matches);
      merger.apply(map, replacedNodes);
      LOG_VART(replacedNodes.size());

      poisMerged++;
    }
  }
  LOG_INFO("Merged " << poisMerged << " POIs.");

  //  Check if the merge target wasn't changed
  if (map->containsElement(mergeTargetId))
    return mergeTargetId;
  //  Find the correct object that was merged
  for (const auto& p : replacedNodes)
  {
    if (p.first == mergeTargetId && map->containsElement(p.second))
      return p.second;
  }
  return mergeTargetId;
}

}
