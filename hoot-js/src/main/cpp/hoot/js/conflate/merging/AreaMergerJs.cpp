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
 * @copyright Copyright (C) 2016, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "AreaMergerJs.h"

// Hoot
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/conflate/merging/ScriptMerger.h>
#include <hoot/core/criterion/NonBuildingAreaCriterion.h>

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

  // instantiate the script merger
  std::shared_ptr<PluginContext> script(new PluginContext());
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

  NonBuildingAreaCriterion nonBuildingAreaCrit;

  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const ConstWayPtr& way = it->second;
    LOG_VART(way->getId());
    LOG_VART(nonBuildingAreaCrit.isSatisfied(way));
    if (way->getElementId() != mergeTargetId && nonBuildingAreaCrit.isSatisfied(way))
    {
      LOG_TRACE("Merging way area: " << way << " into " << mergeTargetId);

      std::set<std::pair<ElementId, ElementId>> matches;
      matches.emplace(mergeTargetId, ElementId::way(way->getId()));
      // apply script merging
      ScriptMerger merger(script, plugin, matches);
      std::vector<std::pair<ElementId, ElementId>> replacedWays;
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
    LOG_VART(nonBuildingAreaCrit.isSatisfied(relation));
    if (relation->getElementId() != mergeTargetId && nonBuildingAreaCrit.isSatisfied(relation))
    {
      LOG_TRACE("Merging relation area: " << relation << " into " << mergeTargetId);

      std::set<std::pair<ElementId, ElementId>> matches;
      matches.emplace(mergeTargetId, ElementId::relation(relation->getId()));
      // apply script merging
      ScriptMerger merger(script, plugin, matches);
      std::vector<std::pair<ElementId, ElementId>> replacedRelations;
      merger.apply(map, replacedRelations);
      LOG_VART(replacedRelations.size());

      areasMerged++;
    }
  }

  LOG_INFO("Merged " << areasMerged << " areas.");
}

}
