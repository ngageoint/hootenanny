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
 * @copyright Copyright (C) 2016-2023 Maxar (http://www.maxar.com/)
 */
#include "RailwayMergerJs.h"

// Hoot
#include <hoot/core/conflate/railway/RailwayOneToManySecondaryMatchElementRemover.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/util/ConfPath.h>

#include <hoot/js/conflate/merging/ScriptMerger.h>
#include <hoot/js/elements/OsmMapJs.h>

// std
#include <utility>

using namespace v8;

namespace hoot
{

ElementId RailwayMergerJs::merge(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current)
{
  LOG_INFO("Merging railways...");

  // instantiate the script merger
  std::shared_ptr<PluginContext> script = std::make_shared<PluginContext>();
  HandleScope handleScope(current);
  Context::Scope context_scope(script->getContext(current));
  Local<Context> context = current->GetCurrentContext();
  script->loadScript(ConfPath::search("Railway.js", "rules"), "plugin");
  Local<Object> global = script->getContext(current)->Global();
  if (global->Has(context, toV8("plugin")).ToChecked() == false)
    throw IllegalArgumentException("Expected the script to have exports.");

  Local<Value> pluginValue = global->Get(context, toV8("plugin")).ToLocalChecked();
  Persistent<Object> plugin(current, Local<Object>::Cast(pluginValue));
  if (plugin.IsEmpty() || ToLocal(&plugin)->IsObject() == false)
    throw IllegalArgumentException("Expected plugin to be a valid object.");

  int numMerged = 0;
  RailwayCriterion crit;
  std::vector<std::pair<ElementId, ElementId>> replacedElements;
  //  Make a copy of the way map so that the mergers can modify the original
  const WayMap ways = map->getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    const ConstWayPtr& way = it->second;
    LOG_VART(way->getId());
    LOG_VART(crit.isSatisfied(way));
    if (way->getElementId() != mergeTargetId && crit.isSatisfied(way))
    {
      LOG_TRACE("Merging railway: " << way << " into " << mergeTargetId);

      std::set<std::pair<ElementId, ElementId>> matches;
      matches.emplace(mergeTargetId, ElementId::way(way->getId()));
      // apply script merging
      ScriptMerger merger(script, plugin, matches);
      merger.apply(map, replacedElements);
      LOG_VART(replacedElements.size());

      numMerged++;
    }
  }

  // We'll go ahead and remove the secondary feature. As noted in the ElementMergerServer
  // documentation, the client shouldn't actually delete this feature from the data store if its
  // involved in any other reviews.
  RailwayOneToManySecondaryMatchElementRemover remover;
  map->visitWaysRw(remover);

  LOG_INFO("Merged " << numMerged << " railways.");

  //  Check if the merge target wasn't changed
  if (map->containsElement(mergeTargetId))
    return mergeTargetId;
  //  Find the correct object that was merged
  for (const auto& p : replacedElements)
  {
    if (p.first == mergeTargetId && map->containsElement(p.second))
      return p.second;
  }
  return mergeTargetId;
}

}
