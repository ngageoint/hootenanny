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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ScriptMatch.h"

// hoot
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/merging/Merger.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/Factory.h>

#include <hoot/js/conflate/merging/ScriptMergerCreator.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// Qt
#include <qnumeric.h>
#include <QElapsedTimer>

// Standard
#include <sstream>

using namespace std;
using namespace Tgs;
using namespace v8;

namespace hoot
{

HOOT_FACTORY_REGISTER(Match, ScriptMatch)

int ScriptMatch::logWarnCount = 0;

ScriptMatch::ScriptMatch() :
Match()
{
}

ScriptMatch::ScriptMatch(const std::shared_ptr<PluginContext>& script,
  const Persistent<Object>& plugin, const ConstOsmMapPtr& map, const v8::Handle<Object>& mapObj,
  const ElementId& eid1, const ElementId& eid2, const ConstMatchThresholdPtr& mt) :
  Match(mt),
  _eid1(eid1),
  _eid2(eid2),
  _isWholeGroup(false),
  _neverCausesConflict(false),
  _script(script)
{
  _plugin.Reset(v8::Isolate::GetCurrent(), plugin);
  _calculateClassification(map, mapObj, ToLocal(&plugin));
}

void ScriptMatch::_calculateClassification(
  const ConstOsmMapPtr& map, Handle<Object> mapObj, Handle<Object> plugin)
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_script->getContext(current));

  // removing these two lines causes a crash when checking for conflicts. WTF?
  Handle<Object> global = _script->getContext(current)->Global();
  global->Get(String::NewFromUtf8(current, "plugin"));

  if (ToLocal(&_plugin)->Has(String::NewFromUtf8(current, "isWholeGroup")))
  {
    Handle<Value> v = _script->call(ToLocal(&_plugin), "isWholeGroup");
    _isWholeGroup = v->BooleanValue();
  }

  if (ToLocal(&_plugin)->Has(String::NewFromUtf8(current, "neverCausesConflict")))
  {
    Handle<Value> v = _script->call(ToLocal(&_plugin), "neverCausesConflict");
    _neverCausesConflict = v->BooleanValue();
  }

  Handle<String> featureTypeStr = String::NewFromUtf8(current, "baseFeatureType");
  if (ToLocal(&_plugin)->Has(featureTypeStr))
  {
    Handle<Value> value = ToLocal(&_plugin)->Get(featureTypeStr);
    _matchName = toCpp<QString>(value);
  }

  try
  {
    Handle<Value> v = _call(map, mapObj, plugin);

    if (v.IsEmpty() || v->IsObject() == false)
    {
      throw IllegalArgumentException("Expected matchScore to return an associative array.");
    }

    QVariantMap vm = toCpp<QVariantMap>(v);
    // grab the match, miss, review results. If they aren't populated they get a value of 0.
    _p.setMatchP(_script->toNumber(v, "match", 0));
    _p.setMissP(_script->toNumber(v, "miss", 0));
    _p.setReviewP(_script->toNumber(v, "review", 0));

    _explainText = vm["explain"].toString();
    if (_explainText.isEmpty())
    {
      _explainText = _threshold->getTypeDetail(_p);
    }
    if (_threshold->getType(_p) == MatchType::Review)
    {
      if (_explainText.isEmpty())
      {
        throw IllegalArgumentException(
          "If the match is a review an appropriate explanation must be provided (E.g. "
          "{ 'review': 1, 'explain': 'some reason' }.");
      }
    }
  }
  catch (const NeedsReviewException& ex)
  {
    LOG_VART(ex.getClassName());
    _p.setReview();
    _explainText = ex.getWhat();
  }

  _p.normalize();
}

set<pair<ElementId, ElementId>> ScriptMatch::getMatchPairs() const
{
  set<pair<ElementId, ElementId>> result;
  result.insert(pair<ElementId, ElementId>(_eid1, _eid2));
  return result;
}

double ScriptMatch::getProbability() const
{
  return _p.getMatchP();
}

bool ScriptMatch::isConflicting(
  const ConstMatchPtr& other, const ConstOsmMapPtr& map,
  const QHash<QString, ConstMatchPtr>& matches) const
{
  LOG_TRACE("Checking for match conflict...");

  if (_neverCausesConflict)
  {
    return false;
  }

  bool conflicting = true;

  const ScriptMatch* hm = dynamic_cast<const ScriptMatch*>(other.get());
  if (hm == 0)
  {
    return true;
  }
  if (hm == this)
  {
    return false;
  }

  // See ticket #5272
  if (getClassification().getReviewP() == 1.0 || other->getClassification().getReviewP() == 1.0)
  {
    return true;
  }

  ElementId sharedEid;
  if (_eid1 == hm->_eid1 || _eid1 == hm->_eid2)
  {
    sharedEid = _eid1;
  }

  if (_eid2 == hm->_eid1 || _eid2 == hm->_eid2)
  {
    // both eids should never be the same.
    assert(sharedEid.isNull());
    sharedEid = _eid2;
  }

  // if the matches don't share at least one eid then it isn't a conflict.
  if (sharedEid.isNull())
  {
    return false;
  }

  // assign o1 and o2 to the non-shared eids
  ElementId o1 = _eid1 == sharedEid ? _eid2 : _eid1;
  ElementId o2 = hm->_eid1 == sharedEid ? hm->_eid2 : hm->_eid1;

  bool foundCache = false;
  bool cacheConflict = false;
  QHash<ConflictKey, bool>::const_iterator cit1 = _conflicts.find(hm->_getConflictKey());
  if (cit1 != _conflicts.end())
  {
    foundCache = true;
    cacheConflict = cit1.value();
  }
  QHash<ConflictKey, bool>::const_iterator cit2 = hm->_conflicts.find(_getConflictKey());
  if (cit2 != hm->_conflicts.end())
  {
    foundCache = true;
    cacheConflict = cit2.value();
  }

  if (foundCache)
  {
    conflicting = cacheConflict;
  }
  else
  {
    try
    {
      // We need to check for a conflict in two directions. If its conflicting when we merge the
      // shared EID with this class first, then is it a conflict if we merge with the other EID
      // first.
      if (_isOrderedConflicting(map, sharedEid, o1, o2, matches) ||
          hm->_isOrderedConflicting(map, sharedEid, o2, o1, matches))
      {
        conflicting = true;
      }
      else
      {
        conflicting = false;
      }
    }
    catch (const NeedsReviewException& /*e*/)
    {
      conflicting = true;
    }
    _conflicts[hm->_getConflictKey()] = conflicting;
  }

  return conflicting;
}

bool ScriptMatch::_isOrderedConflicting(
  const ConstOsmMapPtr& map, ElementId sharedEid, ElementId other1, ElementId other2,
  const QHash<QString, ConstMatchPtr>& matches) const
{
  LOG_TRACE("Checking " << other1 << " and " << other2 << " for order conflict...");

  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_script->getContext(current));

  set<ElementId> eids;
  eids.insert(sharedEid);
  eids.insert(other1);
  eids.insert(other2);

  OsmMapPtr copiedMap(new OsmMap(map->getProjection()));
  CopyMapSubsetOp(map, eids).apply(copiedMap);
  Handle<Object> copiedMapJs = OsmMapJs::create(copiedMap);

  // make sure unknown1 is always first
  ElementId eid11, eid12, eid21, eid22;
  if (map->getElement(sharedEid)->getStatus() == Status::Unknown1)
  {
    eid11 = sharedEid;
    eid21 = sharedEid;
    eid12 = other1;
    eid22 = other2;
  }
  else
  {
    eid11 = other1;
    eid21 = other2;
    eid12 = sharedEid;
    eid22 = sharedEid;
  }

  LOG_VART(eid11);
  LOG_VART(eid12);
  // This and the other commented block of code below is an attempt to prevent script matching
  // being executed on non-candidate matches, during match conflict resolution. These changes cause
  // regression test failures, and it isn't clear why at this point.
//  if (!_isMatchCandidate(copiedMap->getElement(eid11), copiedMap) ||
//      !_isMatchCandidate(copiedMap->getElement(eid12), copiedMap))
//  {
//    LOG_TRACE("Skipping non-match candidate from pair 1: " << eid11 << ", " << eid12 << "...");
//    return true;
//  }

  std::shared_ptr<const ScriptMatch> m1 = _getMatch(copiedMap, copiedMapJs, eid11, eid12, matches);
  MatchSet ms;
  ms.insert(m1);
  vector<MergerPtr> mergers;
  ScriptMergerCreator creator;
  creator.createMergers(ms, mergers);

  // If we got a merger, then check to see if it conflicts.
  if (mergers.size() == 1)
  {
    // apply the merger to our map copy
    vector<pair<ElementId, ElementId>> replaced;
    mergers[0]->apply(copiedMap, replaced);

    // replace the element id in the second merger
    for (size_t i = 0; i < replaced.size(); ++i)
    {
      if (replaced[i].first == eid21)
      {
        eid21 = replaced[i].second;
      }
      if (replaced[i].first == eid22)
      {
        eid22 = replaced[i].second;
      }
    }

    // If we can still find the second match after the merge was applied, then it isn't a conflict.
    if (copiedMap->containsElement(eid21) && copiedMap->containsElement(eid22))
    { 
      LOG_VART(eid21);
      LOG_VART(eid22);
//      if (!_isMatchCandidate(copiedMap->getElement(eid21), copiedMap) ||
//          !_isMatchCandidate(copiedMap->getElement(eid22), copiedMap))
//      {
//        LOG_TRACE("Skipping non-match candidate from pair 2: " << eid21 << ", " << eid22 << "...");
//        //return true;
//        return false;
//      }

      std::shared_ptr<const ScriptMatch> m2 =
        _getMatch(copiedMap, copiedMapJs, eid21, eid22, matches);
      if (m2->getType() == MatchType::Match)
      {
        return false;
      }
    }
  }

  return true;
}

std::shared_ptr<const ScriptMatch> ScriptMatch::_getMatch(
  OsmMapPtr map, Handle<Object> mapJs, const ElementId& eid1, const ElementId& eid2,
  const QHash<QString, ConstMatchPtr>& matches) const
{
  std::shared_ptr<const ScriptMatch> match;

  QString matchKey;
  if (eid1 < eid2)
  {
    matchKey = eid1.toString() + "," + eid2.toString();
  }
  else
  {
    matchKey = eid2.toString() + "," + eid1.toString();
  }
  QHash<QString, ConstMatchPtr>::const_iterator itr = matches.find(matchKey);
  if (itr != matches.end())
  {
    std::shared_ptr<const ScriptMatch> scriptMatch =
      std::dynamic_pointer_cast<const ScriptMatch>(itr.value());
    if (scriptMatch)
    {
      match = scriptMatch;
      LOG_TRACE("Match cache hit for: " << matchKey);
    }
  }

  if (!match)
  {
    match.reset(new ScriptMatch(_script, _plugin, map, mapJs, eid1, eid2, _threshold));
  }

  return match;
}

bool ScriptMatch::_isMatchCandidate(ConstElementPtr e, const ConstOsmMapPtr& map) const
{
  // This code is a little redundant with that in ScriptMatchVisitor::isMatchCandidate. See related
  // notes in that method.

  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_script->getContext(current));
  Handle<Object> plugin =
    Handle<Object>::Cast(
      _script->getContext(current)->Global()->Get(String::NewFromUtf8(current, "plugin")));

  Handle<Value> value = plugin->Get(String::NewFromUtf8(current, "isMatchCandidate"));
  Handle<Function> func = Handle<Function>::Cast(value);
  Handle<Value> jsArgs[2];

  int argc = 0;
  Handle<Object> mapObj = OsmMapJs::create(map);
  jsArgs[argc++] = mapObj;
  jsArgs[argc++] = ElementJs::New(e);

  TryCatch trycatch;
  Handle<Value> result = func->Call(plugin, argc, jsArgs);
  HootExceptionJs::checkV8Exception(result, trycatch);

  return result->BooleanValue();
}

Handle<Value> ScriptMatch::_call(
  const ConstOsmMapPtr& map, Handle<Object> mapObj, Handle<Object> plugin)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope handleScope(current);
  Context::Scope context_scope(_script->getContext(current));

  plugin =
    Handle<Object>::Cast(
      _script->getContext(current)->Global()->Get(String::NewFromUtf8(current, "plugin")));
  Handle<Value> value = plugin->Get(String::NewFromUtf8(current, "matchScore"));
  Handle<Function> func = Handle<Function>::Cast(value);
  Handle<Value> jsArgs[3];

  if (func.IsEmpty() || func->IsFunction() == false)
  {
    throw IllegalArgumentException("matchScore must be a valid function.");
  }

  int argc = 0;
  jsArgs[argc++] = mapObj;
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid1));
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid2));

  LOG_VART(map->getElement(_eid1).get());
  LOG_VART(map->getElement(_eid2).get());
  LOG_TRACE("Calling script matcher...");

  TryCatch trycatch;
  Handle<Value> result = func->Call(plugin, argc, jsArgs);
  HootExceptionJs::checkV8Exception(result, trycatch);

  return handleScope.Escape(result);
}

Handle<Value> ScriptMatch::_callGetMatchFeatureDetails(const ConstOsmMapPtr& map) const
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope handleScope(current);
  Context::Scope context_scope(_script->getContext(current));

  Handle<Object> plugin =
    Handle<Object>::Cast(
      _script->getContext(current)->Global()->Get(String::NewFromUtf8(current, "plugin")));
  Handle<Value> value = plugin->Get(String::NewFromUtf8(current, "getMatchFeatureDetails"));
  Handle<Function> func = Handle<Function>::Cast(value);
  Handle<Value> jsArgs[3];

  if (func.IsEmpty() || func->IsFunction() == false)
  {
    throw IllegalArgumentException(
      "getMatchFeatureDetails must be a valid function for match from: " + _matchName);
  }

  Handle<Object> mapObj = OsmMapJs::create(map);

  int argc = 0;
  jsArgs[argc++] = mapObj;
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid1));
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid2));

  TryCatch trycatch;
  Handle<Value> result = func->Call(plugin, argc, jsArgs);
  HootExceptionJs::checkV8Exception(result, trycatch);

  return handleScope.Escape(result);
}

std::map<QString, double> ScriptMatch::getFeatures(const ConstOsmMapPtr& map) const
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_script->getContext(current));

  // removing these two lines causes a crash when checking for conflicts. WTF?
  Handle<Object> global = _script->getContext(current)->Global();
  global->Get(String::NewFromUtf8(current, "plugin"));

  std::map<QString, double> result;
  LOG_TRACE("Calling getMatchFeatureDetails...");
  Handle<Value> v = _callGetMatchFeatureDetails(map);

  if (v.IsEmpty() || v->IsObject() == false)
  {
    throw IllegalArgumentException(
      "Expected getMatchFeatureDetails to return an associative array.");
  }

  QVariantMap vm = toCpp<QVariantMap>(v);
  long valCtr = 0;
  LOG_VART(vm.size());
  for (QVariantMap::const_iterator it = vm.begin(); it != vm.end(); ++it)
  {
    if (it.value().isNull() == false)
    {
      double d = it.value().toDouble();
      result[it.key()] = d;
      if (::qIsNaN(result[it.key()]))
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("found NaN feature value for: " << it.key());
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
    valCtr++;
  }

  if (vm.size() > 0)
  {
    LOG_DEBUG("Processed " << vm.size() << " sample values.");
  }

  return result;
}

QString ScriptMatch::toString() const
{
  stringstream ss;
  ss << _matchName << "Match: " << _eid1 << ", " << _eid2 << " p: " << _p.toString();
  return QString::fromStdString(ss.str());
}

}
