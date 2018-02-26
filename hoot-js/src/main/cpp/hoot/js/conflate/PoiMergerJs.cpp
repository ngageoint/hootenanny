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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include <set>
#include <utility>    // For std::pair

#include "PoiMergerJs.h"

#include <boost/shared_ptr.hpp>
#include <qstring.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/PluginContext.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/conflate/js/ScriptMerger.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(PoiMergerJs)


PoiMergerJs::PoiMergerJs()
{
}

PoiMergerJs::~PoiMergerJs()
{
}

void PoiMergerJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  exports->Set(String::NewFromUtf8(current, "poiMerge"),
               FunctionTemplate::New(current, jsPoiMerge)->GetFunction());
}

void PoiMergerJs::jsPoiMerge(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  try
  {
    HandleScope scope(current);
    if (args.Length() > 3)
    {
      args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(IllegalArgumentException(
        "Expected two or three arguments to 'poiMerge'."))));
      return;
    }

    // Argument 1: script -- note second param is directory to search under (~/hoot/rules)
    const QString scriptPath = ConfPath::search(toCpp<QString>(args[0]), "rules");
    int elementId = -1;
    if (args.Length() == 3)
       elementId = toCpp<int>(args[2]);

    // Argument 2: Map with POIs
    OsmMapJs* mapJs = node::ObjectWrap::Unwrap<OsmMapJs>(args[1]->ToObject());

    // Pull out internal POI map
    OsmMapPtr map( mapJs->getMap() );

    // Instantiate script merger
    boost::shared_ptr<PluginContext> script(new PluginContext());
    v8::HandleScope handleScope(current);
    v8::Context::Scope context_scope(script->getContext(current));
    script->loadScript(scriptPath, "plugin");

    v8::Handle<v8::Object> global = script->getContext(current)->Global();

    if (global->Has(String::NewFromUtf8(current, "plugin")) == false)
    {
      args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(IllegalArgumentException(
        "Expected the script to have exports."))));
      return;
    }

    Handle<Value> pluginValue = global->Get(String::NewFromUtf8(current, "plugin"));
    Persistent<Object> plugin(current, Handle<Object>::Cast(pluginValue));
    if (plugin.IsEmpty() || ToLocal(&plugin)->IsObject() == false)
    {
      args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(IllegalArgumentException(
        "Expected plugin to be a valid object."))));
      return;
    }

    // Got in Map with POIs A, B, C, D, E
    //
    // Make a set of pairs to indicate all are same object:
    //   A->B, A->C, A->D, A->E
    //
    // ...then pass those pairs one at a time through the merger, since it only merges pairs
    NodeMap nodes = map->getNodes();
    OsmMapPtr mergedMap(map);

    const ElementId firstId = ElementId::node(elementId);
    LOG_TRACE("First ID: " << firstId.getId());
    for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
      if (it->second->getId() != elementId) {
        const ConstNodePtr& n = it->second;

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
    Handle<Object> returnMap = OsmMapJs::create(mergedMap);
    args.GetReturnValue().Set(returnMap);
  }
  catch ( const HootException& e )
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

}
