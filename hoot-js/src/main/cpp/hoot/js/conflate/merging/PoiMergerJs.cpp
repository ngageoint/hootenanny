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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiMergerJs.h"

// Hoot
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/conflate/merging/ScriptMerger.h>
#include <hoot/core/criterion/PoiCriterion.h>

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

  // instantiate script merger
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
    if (node->getId() != mergeTargetId.getId() && PoiCriterion().isSatisfied(node))
    {
      LOG_VART(node);

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(std::pair<ElementId,ElementId>(mergeTargetId, node->getElementId()));
      // apply script merging
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
