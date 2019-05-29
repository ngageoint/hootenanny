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
#include "ScriptMerger.h"

// hoot
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/core/util/Factory.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

using namespace node;
using namespace std;
using namespace v8;

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, ScriptMerger)

int ScriptMerger::logWarnCount = 0;

ScriptMerger::ScriptMerger() :
MergerBase()
{
}

ScriptMerger::ScriptMerger(const std::shared_ptr<PluginContext>& script, Persistent<Object>& plugin,
                           const set<pair<ElementId, ElementId>>& pairs) :
  _pairs(pairs),
  _script(script)
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope scope(current);
  _plugin.Reset(current, plugin);
  _eid1 = _pairs.begin()->first;
  _eid2 = _pairs.begin()->second;
}

void ScriptMerger::apply(const OsmMapPtr& map, vector<pair<ElementId, ElementId>>& replaced)
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
  vector<pair<ElementId, ElementId>>& replaced) const
{
  LOG_VART(_eid1);
  LOG_VART(_eid2);
  LOG_VART(_pairs.size());

  const bool mapContainsElement1 = map->containsElement(_eid1);
  const bool mapContainsElement2 = map->containsElement(_eid2);
  //This contains check was put in place as the result of changing MergerFactory not throw an
  //exception when a merger cannot be found for a set of match pairs.
  if (!mapContainsElement1 || !mapContainsElement2)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Attempting to merge one or more elements that do not exist: ");
      if (!mapContainsElement1)
      {
        LOG_WARN(_eid1);
      }
      if (!mapContainsElement2)
      {
        LOG_WARN(_eid2);
      }
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    return;
  }
  //This pair size check was put in place as the result of changing MergerFactory not throw an
  //exception when a merger cannot be found for a set of match pairs.
  else if (_pairs.size() == 0)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Attempting to merge empty element pairs.");
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    return;
  }
  else if (_pairs.size() > 1)
  {
    throw HootException("A set of elements was specified, but only mergePairs is implemented. "
                        "See the _Supplemental User Documentation_, _Conflating Sets_ for "
                        "details.");
  }

  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_script->getContext(current));
  Handle<Value> v = _callMergePair(map);

  Handle<Object> o = Handle<Object>::Cast(v);
  ElementJs* newElementJs = ObjectWrap::Unwrap<ElementJs>(o);
  ConstElementPtr newElement = newElementJs->getConstElement();

  if (map->containsElement(newElement) == false)
  {
    throw InternalErrorException("The merging script must add new elements to the map.");
  }

  LOG_VART(map->containsElement(_eid1));
  if (map->containsElement(_eid1) == false)
  {
    replaced.push_back(pair<ElementId, ElementId>(_eid1, newElement->getElementId()));
  }
  LOG_VART(map->containsElement(_eid2));
  if (map->containsElement(_eid2) == false)
  {
    replaced.push_back(pair<ElementId, ElementId>(_eid2, newElement->getElementId()));
  }
}

void ScriptMerger::_applyMergeSets(const OsmMapPtr& map,
  vector<pair<ElementId, ElementId>>& replaced) const
{
  _callMergeSets(map, replaced);

  // verify that all the elements are where they need to be (e.g. were removed from the map or
  // added to the map as needed).
}

Handle<Value> ScriptMerger::_callMergePair(const OsmMapPtr& map) const
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope handleScope(current);
  Handle<Object> plugin =
    Handle<Object>::Cast(
      _script->getContext(current)->Global()->Get(String::NewFromUtf8(current, "plugin")));
  Handle<Value> value = plugin->Get(String::NewFromUtf8(current, "mergePair"));

  if (value.IsEmpty() || value->IsFunction() == false)
  {
    throw IllegalArgumentException("The merge function 'mergePair' was not found.");
  }

  Handle<Function> func = Handle<Function>::Cast(value);
  Handle<Value> jsArgs[3];

  LOG_VART(map->getElement(_eid1));
  LOG_VART(map->getElement(_eid2));
  int argc = 0;
  jsArgs[argc++] = OsmMapJs::create(map);
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid1));
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid2));

  TryCatch trycatch;
  Handle<Value> result = func->Call(ToLocal(&_plugin), argc, jsArgs);
  HootExceptionJs::checkV8Exception(result, trycatch);

  if (result.IsEmpty() || result == Undefined(current))
  {
    throw IllegalArgumentException("The merge function must return a valid element as a result.");
  }

  return handleScope.Escape(result);
}

void ScriptMerger::_callMergeSets(const OsmMapPtr& map,
  vector<pair<ElementId, ElementId>>& replaced) const
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_script->getContext(current));
  Handle<Object> plugin =
    Handle<Object>::Cast(
      _script->getContext(current)->Global()->Get(String::NewFromUtf8(current, "plugin")));
  Handle<Value> value = plugin->Get(String::NewFromUtf8(current, "mergeSets"));

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
  Handle<Value> result = func->Call(ToLocal(&_plugin), argc, jsArgs);
  HootExceptionJs::checkV8Exception(result, trycatch);

  // read the replaced values back out
  toCpp(jsArgs[2], replaced);
}

bool ScriptMerger::hasFunction(QString name) const
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_script->getContext(current));
  Handle<Object> plugin =
    Handle<Object>::Cast(_script->getContext(current)->Global()->Get(String::NewFromUtf8(current, "plugin")));
  Handle<Value> value = plugin->Get(String::NewFromUtf8(current, name.toUtf8().data()));

  bool result = true;
  if (value.IsEmpty() || value->IsFunction() == false)
  {
    result = false;
  }
  return result;
}

}
