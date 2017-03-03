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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include <set>
#include <utility>    // For std::pair
#include <hoot/js/SystemNodeJs.h>

#include "PoiMergerJs.h"

#include <hoot/js/OsmMapJs.h>
#include <boost/shared_ptr.hpp>
#include <v8.h>
#include <qstring.h>
#include <hoot/core/OsmMap.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/conflate/js/ScriptMerger.h>
#include <hoot/js/PluginContext.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/js/util/HootExceptionJs.h>

namespace hoot
{

HOOT_JS_REGISTER(PoiMergerJs)


PoiMergerJs::PoiMergerJs()
{
}

PoiMergerJs::~PoiMergerJs()
{
}

void PoiMergerJs::Init(v8::Handle<v8::Object> exports)
{
  exports->Set(v8::String::NewSymbol("poiMerge"), v8::FunctionTemplate::New(jsPoiMerge)->GetFunction());
}

v8::Handle<v8::Value> PoiMergerJs::jsPoiMerge(const v8::Arguments& args)
{
  HandleScope scope;

  try{
    if (args.Length() > 3)
    {
      return v8::ThrowException(HootExceptionJs::create(IllegalArgumentException(
        "Expected two or three arguments to 'poiMerge'.")));
    }

    // Argument 1: script -- note second param is directory to search under (~/hoot/rules)
    const QString scriptPath = ConfPath::search(toCpp<QString>(args[0]), "rules");
    int elementId = -1;
    if (args.Length() == 3) {
       elementId = toCpp<int>(args[2]);
    }

    // Argument 2: Map with POIs
    OsmMapJs* mapJs = node::ObjectWrap::Unwrap<OsmMapJs>(args[1]->ToObject());

    // Pull out internal POI map
    OsmMapPtr map( mapJs->getMap() );

    // Instantiate script merger
    shared_ptr<PluginContext> script(new PluginContext());
    v8::HandleScope handleScope;
    v8::Context::Scope context_scope(script->getContext());
    script->loadScript(scriptPath, "plugin");

    v8::Handle<v8::Object> global = script->getContext()->Global();

    if (global->Has(String::New("plugin")) == false)
    {
      return v8::ThrowException(HootExceptionJs::create(IllegalArgumentException(
        "Expected the script to have exports.")));
    }

    v8::Handle<v8::Value> pluginValue = global->Get(String::New("plugin"));
    v8::Persistent<v8::Object> plugin = v8::Persistent<v8::Object>::New(v8::Handle<v8::Object>::Cast(pluginValue));
    if (plugin.IsEmpty() || plugin->IsObject() == false)
    {
      return v8::ThrowException(HootExceptionJs::create(IllegalArgumentException(
        "Expected plugin to be a valid object.")));
    }

    // Got in Map with POIs A, B, C, D, E
    //
    // Make a set of pairs to indicate all are same object:
    //   A->B, A->C, A->D, A->E
    //
    // ...then pass those pairs one at a time through the merger, since it only merges pairs
    NodeMap nodes = map->getNodeMap();
    OsmMapPtr mergedMap(map);

    const ElementId firstId = ElementId::node(elementId);
    LOG_TRACE("First ID: " << firstId.getId());
    for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
      if (it->second->getId() != elementId) {
        const boost::shared_ptr<const Node>& n = it->second;

        std::set< std::pair< ElementId, ElementId> > matches;
        matches.insert(std::pair<ElementId,ElementId>(firstId, ElementId::node(n->getId())));

        // Now create scriptmerger, and invoke apply method which will apply apply merge transformation, reducing the POIs down to one
        ScriptMerger merger(script, plugin, matches);
        OsmMapPtr mergedMap(map);
        std::vector< std::pair< ElementId, ElementId > > replacedNodes;
        merger.apply(mergedMap, replacedNodes );

        if ( replacedNodes.size() == 1 )
        {
          LOG_TRACE("POI merge: replacing node #" << replacedNodes[0].first.getId() <<
                    " with updated version of node #" << replacedNodes[0].second.getId() );
          mergedMap->replaceNode(replacedNodes[0].first.getId(), replacedNodes[0].second.getId());
        }
      }
    }

    // Hand merged POIs back to caller in OsmMap
    v8::Handle<v8::Object> returnMap = OsmMapJs::create(mergedMap);
    return scope.Close(returnMap);
  }
  catch ( const HootException& e )
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }
}

}
