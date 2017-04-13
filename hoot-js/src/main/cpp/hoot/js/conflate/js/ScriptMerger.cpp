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
#include "ScriptMerger.h"

// hoot
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/StreamUtilsJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{
using namespace node;

ScriptMerger::ScriptMerger(boost::shared_ptr<PluginContext> script, Persistent<Object> plugin,
                           const set<pair<ElementId, ElementId> > &pairs) :
  _pairs(pairs),
  _plugin(plugin),
  _script(script)
{
  _eid1 = _pairs.begin()->first;
  _eid2 = _pairs.begin()->second;
}

void ScriptMerger::apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced) const
{
  bool hasMergeSet = hasFunction("mergeSets");
  bool hasMergePair = hasFunction("mergePair");

  if (hasMergeSet == hasMergePair)
  {
    throw HootException("The merging script must implement exactly one of mergeSet or mergePair.");
  }
  else if (hasMergeSet)
  {
    _applyMergeSets(map, replaced);
  }
  else
  {
    _applyMergePair(map, replaced);
  }
}

void ScriptMerger::_applyMergePair(const OsmMapPtr& map,
  vector< pair<ElementId, ElementId> >& replaced) const
{
  if (_pairs.size() != 1)
  {
    throw HootException("A set of elements was specified, but only mergePairs is implemented. "
                        "See the _Supplemental User Documentation_, _Conflating Sets_ for "
                        "details.");
  }

  HandleScope handleScope;
  Context::Scope context_scope(_script->getContext());
  Handle<Value> v = _callMergePair(map);

  Handle<Object> o = Handle<Object>::Cast(v);
  ElementJs* newElementJs = ObjectWrap::Unwrap<ElementJs>(o);
  ConstElementPtr newElement = newElementJs->getConstElement();

  if (map->containsElement(newElement) == false)
  {
    throw InternalErrorException("The merging script must add new elements to the map.");
  }

  if (map->containsElement(_eid1) == false)
  {
    replaced.push_back(pair<ElementId, ElementId>(_eid1, newElement->getElementId()));
  }
  if (map->containsElement(_eid2) == false)
  {
    replaced.push_back(pair<ElementId, ElementId>(_eid2, newElement->getElementId()));
  }
}

void ScriptMerger::_applyMergeSets(const OsmMapPtr& map,
  vector< pair<ElementId, ElementId> >& replaced) const
{
  _callMergeSets(map, replaced);

  // verify that all the elements are where they need to be (e.g. were removed from the map or
  // added to the map as needed).
}

Handle<Value> ScriptMerger::_callMergePair(const OsmMapPtr& map) const
{
  HandleScope handleScope;
  Handle<Object> plugin =
    Handle<Object>::Cast(_script->getContext()->Global()->Get(String::New("plugin")));
  Handle<Value> value = plugin->Get(String::New("mergePair"));

  if (value.IsEmpty() || value->IsFunction() == false)
  {
    throw IllegalArgumentException("The merge function 'mergePair' was not found.");
  }

  Handle<Function> func = Handle<Function>::Cast(value);
  Handle<Value> jsArgs[3];

  int argc = 0;
  jsArgs[argc++] = OsmMapJs::create(map);
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid1));
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid2));

  TryCatch trycatch;
  Handle<Value> result = func->Call(_plugin, argc, jsArgs);
  HootExceptionJs::checkV8Exception(result, trycatch);

  if (result.IsEmpty() || result == Undefined())
  {
    throw IllegalArgumentException("The merge function must return a valid element as a result.");
  }

  return handleScope.Close(result);
}

void ScriptMerger::_callMergeSets(const OsmMapPtr& map,
  vector< pair<ElementId, ElementId> >& replaced) const
{
  HandleScope handleScope;
  Context::Scope context_scope(_script->getContext());
  Handle<Object> plugin =
    Handle<Object>::Cast(_script->getContext()->Global()->Get(String::New("plugin")));
  Handle<Value> value = plugin->Get(String::New("mergeSets"));

  if (value.IsEmpty() || value->IsFunction() == false)
  {
    throw IllegalArgumentException("The merge function 'mergeSets' was not found.");
  }

  Handle<Function> func = Handle<Function>::Cast(value);
  Handle<Value> jsArgs[3];

  int argc = 0;
  jsArgs[argc++] = OsmMapJs::create(map);
  jsArgs[argc++] = toV8(_pairs);
  jsArgs[argc++] = toV8(replaced);

  TryCatch trycatch;
  Handle<Value> result = func->Call(_plugin, argc, jsArgs);
  HootExceptionJs::checkV8Exception(result, trycatch);

  // read the replaced values back out
  toCpp(jsArgs[2], replaced);
}

bool ScriptMerger::hasFunction(QString name) const
{
  HandleScope handleScope;
  Context::Scope context_scope(_script->getContext());
  Handle<Object> plugin =
    Handle<Object>::Cast(_script->getContext()->Global()->Get(String::New("plugin")));
  Handle<Value> value = plugin->Get(String::New(name.toUtf8().data()));

  bool result = true;
  if (value.IsEmpty() || value->IsFunction() == false)
  {
    result = false;
  }
  return result;
}

}
