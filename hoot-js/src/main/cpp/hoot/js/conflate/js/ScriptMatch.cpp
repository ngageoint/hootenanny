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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ScriptMatch.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>
#include <hoot/core/algorithms/aggregator/RmseAggregator.h>
#include <hoot/core/algorithms/aggregator/QuantileAggregator.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/MaxWordSetDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/Soundex.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/MergerFactory.h>
#include <hoot/core/conflate/Merger.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/conflate/js/ScriptMergerCreator.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/StreamUtilsJs.h>

// Qt
#include <qnumeric.h>

// Standard
#include <sstream>

// tgs
#include <tgs/RandomForest/RandomForest.h>

namespace hoot
{
using namespace Tgs;

unsigned int ScriptMatch::logWarnCount = 0;

ScriptMatch::ScriptMatch(boost::shared_ptr<PluginContext> script, Persistent<Object> plugin,
  const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
  ConstMatchThresholdPtr mt) :
  Match(mt),
  _eid1(eid1),
  _eid2(eid2),
  _isWholeGroup(false),
  _plugin(plugin),
  _script(script)
{
  _calculateClassification(map, plugin);
}

void ScriptMatch::_calculateClassification(const ConstOsmMapPtr& map, Handle<Object> plugin)
{
  Context::Scope context_scope(_script->getContext());
  HandleScope handleScope;

  // removing these two lines causes a crash when checking for conflicts. WTF?
  Handle<Object> global = _script->getContext()->Global();
  global->Get(String::NewSymbol("plugin"));

  if (_plugin->Has(String::NewSymbol("isWholeGroup")))
  {
    Handle<Value> v = _script->call(_plugin, "isWholeGroup");
    _isWholeGroup = v->BooleanValue();
  }

  try
  {
    Handle<Value> v = _call(map, plugin);

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
        throw IllegalArgumentException("If the match is a review an appropriate explanation must "
                                       "be provided (E.g. { 'review': 1, "
                                       "'explain': 'some reason' }.");
      }
    }
  }
  catch (NeedsReviewException& ex)
  {
    LOG_VART(ex.getClassName());
    _p.setReview();
    _explainText = ex.getWhat();
  }

  _p.normalize();
}

set< pair<ElementId, ElementId> > ScriptMatch::getMatchPairs() const
{
  set< pair<ElementId, ElementId> > result;
  result.insert(pair<ElementId, ElementId>(_eid1, _eid2));
  return result;
}

double ScriptMatch::getProbability() const
{
  return _p.getMatchP();
}

bool ScriptMatch::isConflicting(const Match& other, const ConstOsmMapPtr& map) const
{
  bool conflicting = true;

  const ScriptMatch* hm = dynamic_cast<const ScriptMatch*>(&other);
  if (hm == 0)
  {
    return true;
  }
  if (hm == this)
  {
    return false;
  }

  // See ticket #5272
  if (getClassification().getReviewP() == 1.0 || other.getClassification().getReviewP() == 1.0)
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
      // we need to check for a conflict in two directions. Is it conflicting if we merge the shared
      // EID with this class first, then is it a conflict if we merge with the other EID first.
      if (_isOrderedConflicting(map, sharedEid, o1, o2) ||
          hm->_isOrderedConflicting(map, sharedEid, o2, o1))
      {
        conflicting = true;
      }
      else
      {
        conflicting = false;
      }
    }
    catch (NeedsReviewException& e)
    {
      conflicting = true;
    }
    _conflicts[hm->_getConflictKey()] = conflicting;
  }

  return conflicting;
}

bool ScriptMatch::_isOrderedConflicting(const ConstOsmMapPtr& map, ElementId sharedEid,
  ElementId other1, ElementId other2) const
{
  set<ElementId> eids;
  eids.insert(sharedEid);
  eids.insert(other1);
  eids.insert(other2);

  OsmMapPtr copiedMap(new OsmMap(map->getProjection()));
  CopySubsetOp(map, eids).apply(copiedMap);

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

  auto_ptr<ScriptMatch> m1(new ScriptMatch(_script, _plugin, copiedMap, eid11, eid12, _threshold));
  MatchSet ms;
  ms.insert(m1.get());
  vector<Merger*> mergers;
  ScriptMergerCreator creator;
  creator.createMergers(ms, mergers);
  m1.reset();

  bool conflicting = true;
  // if we got a merger, then check to see if it conflicts
  if (mergers.size() == 1)
  {
    // apply the merger to our map copy
    vector< pair<ElementId, ElementId> > replaced;
    mergers[0]->apply(copiedMap, replaced);

    // replace the element id in the second merger.
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

    // if we can still find the second match after the merge was applied then it isn't a conflict
    if (copiedMap->containsElement(eid21) &&
        copiedMap->containsElement(eid22))
    {
      ScriptMatch m2(_script, _plugin, copiedMap, eid21, eid22, _threshold);
      if (m2.getType() == MatchType::Match)
      {
        conflicting = false;
      }
    }
  }

  return conflicting;
}

Handle<Value> ScriptMatch::_call(const ConstOsmMapPtr& map, Handle<Object> plugin)
{
  HandleScope handleScope;
  Context::Scope context_scope(_script->getContext());

  plugin =
    Handle<Object>::Cast(_script->getContext()->Global()->Get(String::New("plugin")));
  Handle<v8::Value> value = plugin->Get(String::New("matchScore"));
  Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
  Handle<Value> jsArgs[3];

  if (func.IsEmpty() || func->IsFunction() == false)
  {
    throw IllegalArgumentException("matchScore must be a valid function.");
  }

  Handle<Object> mapObj = OsmMapJs::create(map);

  int argc = 0;
  jsArgs[argc++] = mapObj;
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid1));
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid2));

  TryCatch trycatch;
  Handle<Value> result = func->Call(plugin, argc, jsArgs);
  HootExceptionJs::checkV8Exception(result, trycatch);

  return handleScope.Close(result);
}

Handle<Value> ScriptMatch::_callGetMatchFeatureDetails(const ConstOsmMapPtr& map) const
{
  HandleScope handleScope;
  Context::Scope context_scope(_script->getContext());

  Handle<Object> plugin =
    Handle<Object>::Cast(_script->getContext()->Global()->Get(String::New("plugin")));
  Handle<v8::Value> value = plugin->Get(String::New("getMatchFeatureDetails"));
  Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
  Handle<Value> jsArgs[3];

  if (func.IsEmpty() || func->IsFunction() == false)
  {
    throw IllegalArgumentException("getMatchFeatureDetails must be a valid function.");
  }

  Handle<Object> mapObj = OsmMapJs::create(map);

  int argc = 0;
  jsArgs[argc++] = mapObj;
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid1));
  jsArgs[argc++] = ElementJs::New(map->getElement(_eid2));

  TryCatch trycatch;
  Handle<Value> result = func->Call(plugin, argc, jsArgs);
  HootExceptionJs::checkV8Exception(result, trycatch);

  return handleScope.Close(result);
}

std::map<QString, double> ScriptMatch::getFeatures(const ConstOsmMapPtr& map) const
{
  Context::Scope context_scope(_script->getContext());
  HandleScope handleScope;

  // removing these two lines causes a crash when checking for conflicts. WTF?
  Handle<Object> global = _script->getContext()->Global();
  global->Get(String::NewSymbol("plugin"));

  std::map<QString, double> result;
  Handle<Value> v = _callGetMatchFeatureDetails(map);

  if (v.IsEmpty() || v->IsObject() == false)
  {
    throw IllegalArgumentException(
      "Expected getMatchFeatureDetails to return an associative array.");
  }

  QVariantMap vm = toCpp<QVariantMap>(v);
  for (QVariantMap::const_iterator it = vm.begin(); it != vm.end(); ++it)
  {
    if (it.value().isNull() == false)
    {
      double d = it.value().toDouble();
      result[it.key()] = d;
      if (::qIsNaN(result[it.key()]))
      {
        if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN("found NaN feature value for: " << it.key());
        }
        else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
  }

  return result;
}

QString ScriptMatch::toString() const
{
  stringstream ss;
  ss << "ScriptMatch: " << _eid1 << ", " << _eid2 << " p: " << _p.toString();
  return QString::fromStdString(ss.str());
}

}
