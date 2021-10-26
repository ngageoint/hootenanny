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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ScriptMatchCreator.h"

// hoot
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/info/CreatorDescription.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/visitors/SpatialIndexer.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/NonConflatableCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/util/MemoryUsageChecker.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>

#include <hoot/js/conflate/matching/ScriptMatch.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>

// Qt
#include <qnumeric.h>
#include <QStringBuilder>
#include <QElapsedTimer>

// Standard
#include <functional>
#include <cmath>

// TGS
#include <tgs/RStarTree/IntersectionIterator.h>
#include <tgs/RStarTree/MemoryPageStore.h>

using namespace geos::geom;
using namespace std;
using namespace Tgs;
using namespace v8;

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, ScriptMatchCreator)

const QString ScriptMatchCreator::POINT_POLYGON_SCRIPT_NAME = "PointPolygon.js";

/**
 * Searches the specified map for any match potentials.
 */
class ScriptMatchVisitor : public ConstElementVisitor
{

public:

  ScriptMatchVisitor(const ConstOsmMapPtr& map, vector<ConstMatchPtr>& result,
    ConstMatchThresholdPtr mt, std::shared_ptr<PluginContext> script,
    ElementCriterionPtr filter = ElementCriterionPtr()) :
    _map(map),
    _result(result),
    _mt(mt),
    _script(script),
    _filter(filter),
    _customSearchRadius(-1.0),
    _neighborCountMax(-1),
    _neighborCountSum(0),
    _elementsEvaluated(0),
    _numElementsVisited(0),
    _numMatchCandidatesVisited(0),
    _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
    _memoryCheckUpdateInterval(ConfigOptions().getMemoryUsageCheckerInterval()),
    _totalElementsToProcess(0)
  {
    // Calls to script functions/var are expensive, both memory-wise and processing-wise. Since this
    // constructor gets called repeatedly by createMatch, keep those calls out of this constructor.

    // Point/Polygon is not meant to conflate any polygons that are conflatable by other *specific*
    // alg conflation routines (generic geometry algs should be ignored here), hence the use of
    // NonConflatableCriterion.
    std::shared_ptr<NonConflatableCriterion> nonConflatableCrit =
      std::make_shared<NonConflatableCriterion>(map);
    nonConflatableCrit->setIgnoreGenericConflators(true);
    _pointPolyCrit =
      std::make_shared<ChainCriterion>(std::make_shared<PolygonCriterion>(map), nonConflatableCrit);

    _timer.start();
  }

  ~ScriptMatchVisitor() override
  {
    //  Free the persistent object
    if (_mapJs.IsEmpty())
      return;
    Local<Object> mapJs(ToLocal(&_mapJs));
    OsmMapJs* object = node::ObjectWrap::Unwrap<OsmMapJs>(mapJs);
    _mapJs.ClearWeak();
    _mapJs.Reset();
    delete object;
  }

  void initSearchRadiusInfo()
  {
    LOG_DEBUG("Initializing search radius info...");

    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    Context::Scope context_scope(_script->getContext(current));
    Local<Context> context = current->GetCurrentContext();
    Local<Object> plugin = getPlugin();

    _candidateDistanceSigma = getNumber(plugin, "candidateDistanceSigma", 0.0, 1.0);

    //this is meant to have been set externally in a js rules file
    _customSearchRadius =
      getNumber(plugin, "searchRadius", -1.0, ConfigOptions().getCircularErrorDefaultValue());
    LOG_VARD(_customSearchRadius);

    Local<Value> value = plugin->Get(context, toV8("getSearchRadius")).ToLocalChecked();
    if (value->IsUndefined())
    {
      // pass
    }
    else if (value->IsFunction() == false)
    {
      throw HootException("getSearchRadius is not a function.");
    }
    else
    {
      _getSearchRadius.Reset(current, Local<Function>::Cast(value));
    }
  }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  void checkForMatch(const std::shared_ptr<const Element>& e)
  {
    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    Context::Scope context_scope(_script->getContext(current));
    Persistent<Object> plugin(current, getPlugin(_script));
    Local<Object> mapJs(ToLocal(&_mapJs));

    LOG_VART(e->getElementId());

    ConstOsmMapPtr map = getMap();

    // create an envelope around the e plus the search radius.
    std::shared_ptr<Envelope> env(e->getEnvelope(map));
    Meters searchRadius = getSearchRadius(e);
    LOG_VART(searchRadius);
    env->expandBy(searchRadius);
    LOG_VART(env);

    // find other nearby candidates
    LOG_TRACE(
      "Finding neighbors for: " << e->getElementId() << " during conflation: " << _scriptPath <<
      "...");
    const set<ElementId> neighbors =
      SpatialIndexer::findNeighbors(*env, getIndex(), _indexToEid, map);
    LOG_VART(neighbors);
    ElementId from = e->getElementId();

    _elementsEvaluated++;

    const bool isPointPolyConflation =
      _scriptPath.contains(ScriptMatchCreator::POINT_POLYGON_SCRIPT_NAME);
    for (set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      ConstElementPtr e2 = map->getElement(*it);
      LOG_VART(e2->getElementId());

      // isCorrectOrder and isMatchCandidate don't apply to Point/Polygon, so we add a different
      // workflow for it here. All other generic scripts use isMatchCandidate to identify both
      // what to index and what to match and that doesn't work for Point/Polygon matching two
      // different geometries. See related note in getIndex about Point/Polygon.

      bool attemptToMatch = false;
      if (!isPointPolyConflation)
      {
        LOG_VART(isCorrectOrder(e, e2));
        LOG_VART(isMatchCandidate(e2));
        attemptToMatch = isCorrectOrder(e, e2) && isMatchCandidate(e2);
      }
      else
      {
        attemptToMatch = _pointPolyCrit->isSatisfied(e2);
      }
      LOG_VART(attemptToMatch);

      if (attemptToMatch)
      {
        // Score each candidate and push it on the result vector.
        std::shared_ptr<ScriptMatch> m =
          std::make_shared<ScriptMatch>(_script, plugin, map, mapJs, from, *it, _mt);
        m->setMatchMembers(
          ScriptMatch::geometryTypeToMatchMembers(
            GeometryTypeCriterion::typeToString(_scriptInfo.getGeometryType())));
        // if we're confident this is not a miss
        if (m->getType() != MatchType::Miss)
        {
          _result.push_back(m);
        }
      }
    }

    _neighborCountSum += neighbors.size();
    _neighborCountMax = std::max(_neighborCountMax, (int)neighbors.size());
  }

  ConstOsmMapPtr getMap() const { return _map.lock(); }

  static double getNumber(Local<Object> obj, QString key, double minValue, double defaultValue)
  {
    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    Local<Context> context = current->GetCurrentContext();

    double result = defaultValue;
    Local<String> cdtKey = String::NewFromUtf8(current, key.toUtf8()).ToLocalChecked();
    if (obj->Has(context, cdtKey).ToChecked())
    {
      Local<Value> v = obj->Get(context, cdtKey).ToLocalChecked();
      if (v->IsNumber() == false || ::qIsNaN(v->NumberValue(context).ToChecked()))
      {
        throw IllegalArgumentException("Expected " + key + " to be a number.");
      }
      result = v->NumberValue(context).ToChecked();

      if (result < minValue)
      {
        throw IllegalArgumentException(
          QString("Expected %1 to be greater than %2.").arg(key).arg(minValue));
      }
    }
    return result;
  }

  Local<Object> getPlugin()
  {
    return getPlugin(_script);
  }

  static Local<Object> getPlugin(const std::shared_ptr<PluginContext>& script)
  {
    Isolate* current = v8::Isolate::GetCurrent();
    EscapableHandleScope handleScope(current);
    Context::Scope context_scope(script->getContext(current));
    Local<Context> context = current->GetCurrentContext();
    Local<Object> global = script->getContext(current)->Global();

    if (global->Has(context, toV8("plugin")).ToChecked() == false)
    {
      throw IllegalArgumentException("Expected the script to have exports.");
    }

    Local<Value> pluginValue = global->Get(context, toV8("plugin")).ToLocalChecked();
    Local<Object> plugin(Local<Object>::Cast(pluginValue));
    if (plugin.IsEmpty() || plugin->IsObject() == false)
    {
      throw IllegalArgumentException("Expected plugin to be a valid object.");
    }

    return handleScope.Escape(plugin);
  }

  Meters getSearchRadius(const ConstElementPtr& e)
  {
    // See the "Calculating Search Radius" section in the user docs for more information.

    Meters result;
    if (_getSearchRadius.IsEmpty())
    {
      if (_customSearchRadius < 0)
      {
        // base the radius off of the element itself
        LOG_TRACE("Calculating search radius based off of element...");
        result = e->getCircularError() * _candidateDistanceSigma;
      }
      else
      {
        // base the radius off some predefined radius
        LOG_TRACE("Calculating search radius based off of custom defined script value...");
        result = _customSearchRadius * _candidateDistanceSigma;
      }
    }
    else
    {
      if (_searchRadiusCache.contains(e->getElementId()))
      {
        LOG_TRACE("Retrieving search radius from cache...");
        result = _searchRadiusCache[e->getElementId()];
      }
      else
      {
        LOG_TRACE("Calling getSearchRadius function for: " << _scriptPath << "...");

        Isolate* current = v8::Isolate::GetCurrent();
        HandleScope handleScope(current);
        Context::Scope context_scope(_script->getContext(current));
        Local<Context> context = current->GetCurrentContext();

        Local<Value> jsArgs[1];

        int argc = 0;
        jsArgs[argc++] = ElementJs::New(e);

        Local<Value> f =
          ToLocal(&_getSearchRadius)->Call(context, getPlugin(), argc, jsArgs).ToLocalChecked();

        result = toCpp<Meters>(f) * _candidateDistanceSigma;

        _searchRadiusCache[e->getElementId()] = result;
      }
    }

    LOG_VART(result);
    return result;
  }

  void calculateSearchRadius()
  {
    // This is meant to run one time when the match creator is initialized. We're either getting
    // the search radius from a predefined property linked to a config option in the conflate rules
    // file or we're calculating it with a function call from the rules file. Either way, then we're
    // caching it after the first retrieval.

    LOG_DEBUG("Checking for existence of search radius export for: " << _scriptPath << "...");

    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    Context::Scope context_scope(_script->getContext(current));
    Local<Context> context = current->GetCurrentContext();

    Persistent<Object> plugin(current, getPlugin(_script));
    Local<String> initStr = String::NewFromUtf8(current, "calculateSearchRadius").ToLocalChecked();
    // optional method, so don't throw an error
    if (ToLocal(&plugin)->Has(context, initStr).ToChecked() == false)
    {
      LOG_TRACE("calculateSearchRadius function not present.");
      return;
    }
    Local<Value> value = ToLocal(&plugin)->Get(context, initStr).ToLocalChecked();
    if (value->IsFunction() == false)
    {
      LOG_TRACE("calculateSearchRadius function not present.");
      return;
    }

    LOG_DEBUG("Getting search radius for: " << _scriptPath << "...");

    Local<Function> func = Local<Function>::Cast(value);
    Local<Value> jsArgs[1];
    int argc = 0;
    assert(getMap().get());
    OsmMapPtr copiedMap = std::make_shared<OsmMap>(getMap());
    jsArgs[argc++] = OsmMapJs::create(copiedMap);

    Local<Value> c = func->Call(context, ToLocal(&plugin), argc, jsArgs).ToLocalChecked();
    LOG_DEBUG("Return value: " << c);

    // This could be an expensive call.
    _customSearchRadius =
      getNumber(
        ToLocal(&plugin), "searchRadius", -1.0, ConfigOptions().getCircularErrorDefaultValue());

    QFileInfo scriptFileInfo(_scriptPath);
    LOG_DEBUG(
      "Search radius of: " << _customSearchRadius << " to be used for: " <<
      scriptFileInfo.fileName());
  }

  void cleanMapCache()
  {
    _mapJs.Empty();
  }

  std::shared_ptr<HilbertRTree>& getIndex()
  {
    if (!_index)
    {
      LOG_INFO("Creating script feature index for: " << _scriptPath << "...");

      // No tuning was done, just copied these settings from OsmMapIndex. 10 children - 368 - see
      // #3054
      _index = std::make_shared<Tgs::HilbertRTree>(std::make_shared<Tgs::MemoryPageStore>(728), 2);

      // Only index elements that satisfy the isMatchCandidate. Previously we only indexed Unknown2,
      // but that causes issues when wanting to conflate from n datasets and support intra-dataset
      // conflation. This approach over-indexes a bit and will likely slow things down, but should
      // give the same results. An option in the future would be to support an "isIndexedFeature" or
      // similar function to speed the operation back up again.

      // Point/Polygon conflation behaves diferently than all other generic scripts in that it
      // conflates geometries of different types. This class wasn't really originally designed to
      // handle that, so we add a logic path here to accommodate it.
      long numElementsIndexed = 0;
      if (!_scriptPath.contains(ScriptMatchCreator::POINT_POLYGON_SCRIPT_NAME))
      {
        std::function<bool (ConstElementPtr)> f =
          std::bind(&ScriptMatchVisitor::isMatchCandidate, this, placeholders::_1);
        std::shared_ptr<ArbitraryCriterion> pC = std::make_shared<ArbitraryCriterion>(f);

        SpatialIndexer v(_index,
                         _indexToEid,
                         pC,
                         std::bind(&ScriptMatchVisitor::getSearchRadius, this, placeholders::_1),
                         getMap());
        switch (_scriptInfo.getGeometryType())
        {
          case GeometryTypeCriterion::GeometryType::Point:
            getMap()->visitNodesRo(v);
            break;
          case GeometryTypeCriterion::GeometryType::Line:
            getMap()->visitWaysRo(v);
            getMap()->visitRelationsRo(v);
            break;
          case GeometryTypeCriterion::GeometryType::Polygon:
            getMap()->visitWaysRo(v);
            getMap()->visitRelationsRo(v);
            break;
          default:
            // visit all geometry types if the script didn't identify its geometry
            getMap()->visitRo(v);
            break;
        }
        v.finalizeIndex();
        numElementsIndexed = v.getSize();
      }
      else
      {
        // Point/Polygon identifies points as match candidates, so we just index all polygons here.
        SpatialIndexer v(_index,
                         _indexToEid,
                         _pointPolyCrit,
                         std::bind(&ScriptMatchVisitor::getSearchRadius, this, placeholders::_1),
                         getMap());
        getMap()->visitWaysRo(v);
        getMap()->visitRelationsRo(v);
        v.finalizeIndex();
        numElementsIndexed = v.getSize();
      }

      LOG_INFO(
        "Script feature index created for: " << _scriptPath << " with " <<
        StringUtils::formatLargeNumber(numElementsIndexed) << " elements.");
    }
    return _index;
  }

  Local<Object> getOsmMapJs()
  {
    Isolate* current = v8::Isolate::GetCurrent();
    EscapableHandleScope handleScope(current);
    if (_mapJs.IsEmpty())
    {
      _mapJs.Reset(current, OsmMapJs::create(getMap()));
    }
    return handleScope.Escape(ToLocal(&_mapJs));
  }

  /**
   * Returns true if e1, e2 is in the correct ordering for matching. This does a few things:
   *
   *  - Avoid comparing e1 to e2 and e2 to e1
   *  - The Unknown1/Input1 is always e1. This is a requirement for some of the older code.
   *  - Gives a consistent ordering to allow backwards compatibility with system tests.
   */
  bool isCorrectOrder(const ConstElementPtr& e1, const ConstElementPtr& e2) const
  {
    LOG_VART(e1->getStatus().getEnum());
    LOG_VART(e2->getStatus().getEnum());
    LOG_VART(e1->getElementId());
    LOG_VART(e2->getElementId());

    if (e1->getStatus().getEnum() == e2->getStatus().getEnum())
    {
      return e1->getElementId() < e2->getElementId();
    }
    else
    {
      return e1->getStatus().getEnum() < e2->getStatus().getEnum();
    }
  }

  bool isMatchCandidate(ConstElementPtr e)
  {
    if (_matchCandidateCache.contains(e->getElementId()))
    {
      return _matchCandidateCache[e->getElementId()];
    }

    if (_filter && !_filter->isSatisfied(e))
    {
      return false;
    }

    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    Context::Scope context_scope(_script->getContext(current));
    Local<Context> context = current->GetCurrentContext();
    Persistent<Object> plugin(current, getPlugin(_script));

    bool result = false;

    // TODO: Prioritize exports.matchCandidateCriterion over the isMatchCandidate function and use
    // the crit instead of the function; doing so causes this to crash; see #3047 and the history
    // of this file for the failing code that needs to be re-enabled

    Local<String> isMatchCandidateStr =
      String::NewFromUtf8(current, "isMatchCandidate").ToLocalChecked();
    if (ToLocal(&plugin)->Has(context, isMatchCandidateStr).ToChecked() == false)
    {
      throw HootException("Error finding 'isMatchCandidate' function.");
    }
    Local<Value> value = ToLocal(&plugin)->Get(context, isMatchCandidateStr).ToLocalChecked();
    if (value->IsFunction() == false)
    {
      throw HootException("isMatchCandidate is not a function.");
    }
    Local<Function> func = Local<Function>::Cast(value);
    Local<Value> jsArgs[2];

    int argc = 0;
    jsArgs[argc++] = getOsmMapJs();
    jsArgs[argc++] = ElementJs::New(e);

    Local<Value> f = func->Call(context, ToLocal(&plugin), argc, jsArgs).ToLocalChecked();

    result = f->BooleanValue(current);

    _matchCandidateCache[e->getElementId()] = result;

    return result;
  }

  void visit(const ConstElementPtr& e) override
  {
    if (isMatchCandidate(e))
    {
      checkForMatch(e);

      _numMatchCandidatesVisited++;
      if (_numMatchCandidatesVisited % (_taskStatusUpdateInterval * 100) == 0)
      {
        PROGRESS_DEBUG(
          "\tProcessed " << StringUtils::formatLargeNumber(_numMatchCandidatesVisited) <<
          " match candidates / " << StringUtils::formatLargeNumber(_totalElementsToProcess) <<
          " total elements.");
      }
    }

    // if matching gets slow, throttle the log update interval accordingly.
    if (_timer.elapsed() > 3000 && _taskStatusUpdateInterval >= 10)
    {
      _taskStatusUpdateInterval /= 10;
    }
    else if (_timer.elapsed() < 250 && _taskStatusUpdateInterval < 10000)
    {
      _taskStatusUpdateInterval *= 10;
    }

    _numElementsVisited++;
    if (_numElementsVisited % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_STATUS(
        "\tProcessed " << StringUtils::formatLargeNumber(_numElementsVisited) << " of " <<
        StringUtils::formatLargeNumber(_totalElementsToProcess) << " elements.");
       _timer.restart();
    }
    if (_numElementsVisited % _memoryCheckUpdateInterval == 0)
    {
      MemoryUsageChecker::getInstance().check();
    }
  }

  void setScriptPath(QString path) { _scriptPath = path; }
  QString getScriptPath() const { return _scriptPath; }

  Meters getCustomSearchRadius() const { return _customSearchRadius; }
  void setCustomSearchRadius(Meters searchRadius) { _customSearchRadius = searchRadius; }

  double getCandidateDistanceSigma() const { return _candidateDistanceSigma; }
  void setCandidateDistanceSigma(double sigma) { _candidateDistanceSigma = sigma; }

  CreatorDescription getCreatorDescription() const { return _scriptInfo; }
  void setCreatorDescription(const CreatorDescription& description)
  {
    _scriptInfo = description;

    if (_scriptPath.toLower().contains("relation")) // hack
    {
      _totalElementsToProcess = getMap()->getRelationCount();
    }
    else
    {
      switch (_scriptInfo.getGeometryType())
      {
        case GeometryTypeCriterion::GeometryType::Point:
          _totalElementsToProcess = getMap()->getNodeCount();
          break;
        case GeometryTypeCriterion::GeometryType::Line:
          _totalElementsToProcess = getMap()->getWayCount() + getMap()->getRelationCount();
          break;
        case GeometryTypeCriterion::GeometryType::Polygon:
          _totalElementsToProcess = getMap()->getWayCount() + getMap()->getRelationCount();
          break;
        default:
          // visit all geometry types if the script didn't identify its geometry
          _totalElementsToProcess = getMap()->size();
          break;
      }
    }
  }

  long getNumMatchCandidatesFound() const { return _numMatchCandidatesVisited; }

  bool hasCustomSearchRadiusFunction() const { return !_getSearchRadius.IsEmpty(); }

private:

  // Don't hold on to the map.
  std::weak_ptr<const OsmMap> _map;
  Persistent<Object> _mapJs;

  vector<ConstMatchPtr>& _result;
  ConstMatchThresholdPtr _mt;

  std::shared_ptr<PluginContext> _script;

  ElementCriterionPtr _filter;

  // used for automatic search radius calculation; it is expected that this is set from the
  // Javascript rules file used for the generic conflation
  double _customSearchRadius;

  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  long _numElementsVisited;
  long _numMatchCandidatesVisited;

  int _taskStatusUpdateInterval;
  int _memoryCheckUpdateInterval;

  long _totalElementsToProcess;

  std::shared_ptr<ChainCriterion> _pointPolyCrit;

  QElapsedTimer _timer;

  CreatorDescription _scriptInfo;

  Persistent<Function> _getSearchRadius;

  QHash<ElementId, bool> _matchCandidateCache;
  QHash<ElementId, double> _searchRadiusCache;

  // Used for finding neighbors
  std::shared_ptr<HilbertRTree> _index;
  deque<ElementId> _indexToEid;

  double _candidateDistanceSigma;

  QString _scriptPath;

  set<ElementId> _empty;
};

ScriptMatchCreator::ScriptMatchCreator() :
_railwayOneToManyMatchEnabled(false),
_railwayOneToManyTransferAllKeys(false)
{
}

void ScriptMatchCreator::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _railwayOneToManyMatchEnabled = opts.getRailwayOneToManyMatch();
  _railwayOneToManyIdentifyingKeys = opts.getRailwayOneToManyIdentifyingKeys();
  _railwayOneToManyTransferKeys = opts.getRailwayOneToManyTransferKeys();
  _railwayOneToManyTransferAllKeys = opts.getRailwayOneToManyTransferAllTags();
}

void ScriptMatchCreator::init(const ConstOsmMapPtr& map)
{
  _getCachedVisitor(map)->initSearchRadiusInfo();
}

Meters ScriptMatchCreator::calculateSearchRadius(
  const ConstOsmMapPtr& map, const ConstElementPtr& e)
{
  return _getCachedVisitor(map)->getSearchRadius(e);
}

void ScriptMatchCreator::setArguments(const QStringList& args)
{
  if (args.size() != 1)
  {
    throw HootException("The ScriptMatchCreator takes exactly one argument (script path).");
  }

  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  _scriptPath = ConfPath::search(args[0], "rules");
  _script = std::make_shared<PluginContext>();
  Context::Scope context_scope(_script->getContext(current));
  _script->loadScript(_scriptPath, "plugin");
  // bit of a hack...see MatchCreator.h...need to refactor
  _description = className() + "," + args[0];
  _cachedScriptVisitor.reset();
  _scriptInfo = _getScriptDescription(_scriptPath);

  setConfiguration(conf());
  // Validate the configuration outside of setConfiguration, since it needs to be done each time a
  // different conflate script is specified.
  _validateConfig(_scriptInfo.getBaseFeatureType());

  LOG_DEBUG(
    "Set arguments for: " << className() << " - rules: " << QFileInfo(_scriptPath).fileName());
}

void ScriptMatchCreator::_validateConfig(
  const CreatorDescription::BaseFeatureType& baseFeatureType)
{
  ConfigOptions opts;

  // Other feature types may also need settings validation.
  if (baseFeatureType == CreatorDescription::BaseFeatureType::Railway &&
      _railwayOneToManyMatchEnabled)
  {
    StringUtils::removeEmptyStrings(_railwayOneToManyIdentifyingKeys);
    if (_railwayOneToManyIdentifyingKeys.empty())
    {
      throw IllegalArgumentException(
        "No railway one to many identifying keys specified in " +
        ConfigOptions::getRailwayOneToManyIdentifyingKeysKey() + ".");
    }

    if (!_railwayOneToManyTransferAllKeys)
    {
      StringUtils::removeEmptyStrings(_railwayOneToManyTransferKeys);
      if (_railwayOneToManyTransferKeys.empty())
      {
        throw IllegalArgumentException(
          "No railway one to many transfer tag keys specified in " +
          ConfigOptions::getRailwayOneToManyTransferKeysKey() + ".");
      }
    }

    LOG_STATUS("Running railway one to many custom conflation workflow...");
  }
}

void ScriptMatchCreator::_validatePluginConfig(
  const CreatorDescription::BaseFeatureType& baseFeatureType, Local<Object> plugin) const
{
  if (baseFeatureType == CreatorDescription::BaseFeatureType::Railway)
  {
    const double railwayTypeMatchThreshold =
      ScriptMatchVisitor::getNumber(plugin, "typeThreshold", 0.0, 1.0);
     if (railwayTypeMatchThreshold < 0.0 ||  railwayTypeMatchThreshold > 1.0)
     {
       throw IllegalArgumentException(
         "Railway type match threshold out of range: " +
         QString::number(railwayTypeMatchThreshold) + ".");
     }
  }
}

MatchPtr ScriptMatchCreator::createMatch(
  const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2)
{
  LOG_VART(eid1);
  LOG_VART(eid2);

  // There may be some benefit at some point in caching matches calculated in ScriptMatchVisitor and
  // accessing that cached information here to avoid extra calls into the JS match script. So far,
  // haven't seen any performance improvement after adding match caching.

  const bool isPointPolyConflation = _scriptPath.contains(POINT_POLYGON_SCRIPT_NAME);
  LOG_VART(isPointPolyConflation);
  bool attemptToMatch = false;
  ConstElementPtr e1 = map->getElement(eid1);
  ConstElementPtr e2 = map->getElement(eid2);
  if (e1 && e2)
  {
    if (!isPointPolyConflation)
    {
      attemptToMatch = isMatchCandidate(e1, map) && isMatchCandidate(e2, map);
    }
    else
    {
      if (!_pointPolyPolyCrit)
      {
          _pointPolyPolyCrit =
            std::make_shared<ChainCriterion>(
              std::make_shared<PolygonCriterion>(map),
              std::make_shared<NonConflatableCriterion>(map));
      }
      if (!_pointPolyPointCrit)
      {
          _pointPolyPointCrit =
            std::make_shared<ChainCriterion>(
              std::make_shared<PointCriterion>(map),
              std::make_shared<NonConflatableCriterion>(map));
      }

      // see related note in ScriptMatchVisitor::checkForMatch
      attemptToMatch =
        (_pointPolyPointCrit->isSatisfied(e1) && _pointPolyPolyCrit->isSatisfied(e2)) ||
        (_pointPolyPolyCrit->isSatisfied(e1) && _pointPolyPointCrit->isSatisfied(e2));
    }
  }
  LOG_VART(attemptToMatch);

  if (attemptToMatch)
  {
    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    Context::Scope context_scope(_script->getContext(current));

    Local<Object> mapJs = OsmMapJs::create(map);
    Persistent<Object> plugin(current, ScriptMatchVisitor::getPlugin(_script));

    std::shared_ptr<ScriptMatch> match =
      std::make_shared<ScriptMatch>(_script, plugin, map, mapJs, eid1, eid2, getMatchThreshold());
    match->setMatchMembers(
      ScriptMatch::geometryTypeToMatchMembers(
        GeometryTypeCriterion::typeToString(_scriptInfo.getGeometryType())));
    return match;
  }

  return MatchPtr();
}

void ScriptMatchCreator::createMatches(
  const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches, ConstMatchThresholdPtr threshold)
{
  if (!_script)
  {
    throw IllegalArgumentException("The script must be set on the ScriptMatchCreator.");
  }

  // The parent does some initialization we need.
  MatchCreator::createMatches(map, matches, threshold);

  QElapsedTimer timer;
  timer.start();

  ScriptMatchVisitor v(map, matches, threshold, _script, _filter);
  v.setScriptPath(_scriptPath);
  _descriptionCache[_scriptPath] = _scriptInfo;
  v.setCreatorDescription(_scriptInfo);
  v.initSearchRadiusInfo();
  v.calculateSearchRadius();

  QFileInfo scriptFileInfo(_scriptPath);

  // This doesn't work with _candidateDistanceSigma, but right now its set to 1.0 in every script
  // and has no effect on the search radius.
  QString searchRadiusStr;
  const double searchRadius = v.getCustomSearchRadius();
  if (v.hasCustomSearchRadiusFunction())
  {
    searchRadiusStr = "within a function calculated search radius";
  }
  else if (searchRadius < 0)
  {
    searchRadiusStr = "within a feature dependent search radius";
  }
  else
  {
    searchRadiusStr =
      "within a search radius of " + QString::number(searchRadius, 'g', 2) + " meters";
  }

  LOG_INFO(
    "Looking for matches with: " << scriptFileInfo.fileName() << " " << searchRadiusStr << "...");
  LOG_VARD(*threshold);
  const int matchesSizeBefore = matches.size();

  _cachedCustomSearchRadii[_scriptPath] = searchRadius;
  _candidateDistanceSigmaCache[_scriptPath] = v.getCandidateDistanceSigma();

  LOG_VARD(GeometryTypeCriterion::typeToString(_scriptInfo.getGeometryType()));
  // kind of hack; not sure of a better way to do determine if we're doing collection relation
  // conflation
  if (scriptFileInfo.fileName().toLower().contains("relation"))
  {
    map->visitRelationsRo(v);
  }
  else
  {
    switch (_scriptInfo.getGeometryType())
    {
      case GeometryTypeCriterion::GeometryType::Point:
        map->visitNodesRo(v);
        break;
      case GeometryTypeCriterion::GeometryType::Line:
        map->visitWaysRo(v);
        map->visitRelationsRo(v);
        break;
      case GeometryTypeCriterion::GeometryType::Polygon:
        map->visitWaysRo(v);
        map->visitRelationsRo(v);
        break;
      default:
        // visit all geometry types if the script didn't identify its geometry
        map->visitRo(v);
        break;
    }
  }

  const int matchesSizeAfter = matches.size();

  QString matchType = CreatorDescription::baseFeatureTypeToString(_scriptInfo.getBaseFeatureType());
  // Workaround for the Point/Polygon script since it doesn't identify a base feature type. See
  // note in ScriptMatchVisitor::getIndex and rules/PointPolygon.js.
  if (_scriptPath.contains(POINT_POLYGON_SCRIPT_NAME))
  {
    matchType = "PointPolygon";
  }

  LOG_STATUS(
    "\tFound " << StringUtils::formatLargeNumber(v.getNumMatchCandidatesFound()) << " " <<
    matchType << " match candidates and " <<
    StringUtils::formatLargeNumber(matchesSizeAfter - matchesSizeBefore) <<
    " total matches in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

vector<CreatorDescription> ScriptMatchCreator::getAllCreators() const
{
  vector<CreatorDescription> result;

  // find all the files that end with .js in [ConfPath]/rules/
  const QStringList scripts = ConfPath::find(QStringList() << "*.js", "rules");
  // go through each script
  for (int i = 0; i < scripts.size(); i++)
  {
    try
    {
      // if the script is a valid generic script w/ 'description' exposed, add it to the list.
      CreatorDescription d = _getScriptDescription(scripts[i]);
      if (!d.getDescription().isEmpty())
      {
        result.push_back(d);
        LOG_TRACE(d.getDescription());
      }
      else
      {
        LOG_TRACE(QString("Skipping reporting script %1 because it has no "
          "description.").arg(scripts[i]));
      }
    }
    catch (const HootException& e)
    {
      LOG_WARN("Error loading script: " + scripts[i] + " exception: " + e.getWhat());
    }
  }

  return result;
}

std::shared_ptr<ScriptMatchVisitor> ScriptMatchCreator::_getCachedVisitor(
  const ConstOsmMapPtr& map)
{
  if (!_cachedScriptVisitor.get() || _cachedScriptVisitor->getMap() != map)
  {
    LOG_VART(_cachedScriptVisitor.get());
    QString scriptPath = _scriptPath;
    if (_cachedScriptVisitor.get())
    {
      LOG_VART(_cachedScriptVisitor->getMap() == map);
      scriptPath = _cachedScriptVisitor->getScriptPath();
    }
    LOG_VART(scriptPath);

    QFileInfo scriptFileInfo(_scriptPath);
    LOG_TRACE("Resetting the match candidate checker: " << scriptFileInfo.fileName() << "...");

    vector<ConstMatchPtr> emptyMatches;
    _cachedScriptVisitor =
      std::make_shared<ScriptMatchVisitor>(
        map, emptyMatches, ConstMatchThresholdPtr(), _script, _filter);

    _cachedScriptVisitor->setScriptPath(scriptPath);

    // setting these cached values on the visitor here for performance reasons; this could all be
    // consolidated and cleaned up
    LOG_VART(_descriptionCache.contains(scriptPath));
    if (_descriptionCache.contains(scriptPath))
    {
      _cachedScriptVisitor->setCreatorDescription(_descriptionCache[scriptPath]);
    }

    LOG_VART(_candidateDistanceSigmaCache.contains(scriptPath));
    if (_candidateDistanceSigmaCache.contains(scriptPath))
    {
      _cachedScriptVisitor->setCandidateDistanceSigma(_candidateDistanceSigmaCache[scriptPath]);
    }

    //If the search radius has already been calculated for this matcher once, we don't want to do
    //it again due to the expense.
    LOG_VART(_cachedCustomSearchRadii.contains(scriptPath));
    if (!_cachedCustomSearchRadii.contains(scriptPath))
    {
      _cachedScriptVisitor->calculateSearchRadius();
    }
    else
    {
      LOG_VART(_cachedCustomSearchRadii[scriptPath]);
      _cachedScriptVisitor->setCustomSearchRadius(_cachedCustomSearchRadii[scriptPath]);
    }
  }

  return _cachedScriptVisitor;
}

CreatorDescription ScriptMatchCreator::_getScriptDescription(QString path) const
{
  LOG_DEBUG("Getting script description...");

  CreatorDescription result;
  result.setExperimental(true);

  std::shared_ptr<PluginContext> script = std::make_shared<PluginContext>();
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(script->getContext(current));
  Local<Context> context = current->GetCurrentContext();
  script->loadScript(path, "plugin");
  Persistent<Object> plugin(current, ScriptMatchVisitor::getPlugin(script));

  Local<String> descriptionStr = String::NewFromUtf8(current, "description").ToLocalChecked();
  if (ToLocal(&plugin)->Has(context, descriptionStr).ToChecked())
  {
    Local<Value> value = ToLocal(&plugin)->Get(context, descriptionStr).ToLocalChecked();
    result.setDescription(toCpp<QString>(value));
  }
  else
  {
    throw IllegalArgumentException("No script description provided for: " + path);
  }

  Local<String> experimentalStr = String::NewFromUtf8(current, "experimental").ToLocalChecked();
  if (ToLocal(&plugin)->Has(context, experimentalStr).ToChecked())
  {
    Local<Value> value = ToLocal(&plugin)->Get(context, experimentalStr).ToLocalChecked();
    result.setExperimental(toCpp<bool>(value));
  }

  CreatorDescription::BaseFeatureType baseFeatureType =
    CreatorDescription::BaseFeatureType::Unknown;
  Local<String> featureTypeStr = String::NewFromUtf8(current, "baseFeatureType").ToLocalChecked();
  if (ToLocal(&plugin)->Has(context, featureTypeStr).ToChecked())
  {
    Local<Value> value = ToLocal(&plugin)->Get(context, featureTypeStr).ToLocalChecked();
    baseFeatureType = CreatorDescription::stringToBaseFeatureType(toCpp<QString>(value));
    result.setBaseFeatureType(baseFeatureType);
  }
  // A little kludgy, but we'll identify generic geometry Point/Polygon conflation by its script
  // name.
  else if (!path.contains("PointPolygon.js"))
  {
    throw IllegalArgumentException("No base feature type provided for: " + path);
  }

  Local<String> geometryTypeStr = String::NewFromUtf8(current, "geometryType").ToLocalChecked();
  if (ToLocal(&plugin)->Has(context, geometryTypeStr).ToChecked())
  {
    Local<Value> value = ToLocal(&plugin)->Get(context, geometryTypeStr).ToLocalChecked();
    result.setGeometryType(GeometryTypeCriterion::typeFromString(toCpp<QString>(value)));
  }
  else if (!path.contains("PointPolygon.js"))
  {
    throw IllegalArgumentException("No geometry type provided for: " + path);
  }

  // The criteria parsed here describe which feature types a script conflates. Its used only for
  // determining which conflate ops to disable with SuperfluousConflateOpRemover and for some
  // scripts, also to determine how to cull features when performing rubbersheeting during search
  // radius auto-calc. Is does *not* actually cull features during matching.
  // exports.isMatchCandidate does that. So, there is a bit of a disconnect there. However, it
  // hasn't caused any problems so far.
  Local<String> matchCandidateCriterionStr =
    String::NewFromUtf8(current, "matchCandidateCriterion").ToLocalChecked();
  if (ToLocal(&plugin)->Has(context, matchCandidateCriterionStr).ToChecked())
  {
    Local<Value> value = ToLocal(&plugin)->Get(context, matchCandidateCriterionStr).ToLocalChecked();
    const QString valueStr = toCpp<QString>(value);
    if (valueStr.contains(";"))
    {
      result.setMatchCandidateCriteria(valueStr.split(";"));
    }
    else
    {
      result.setMatchCandidateCriteria(QStringList(valueStr));
    }
  }
  else
  {
    throw IllegalArgumentException("No match candidate criteria provided for: " + path);
  }

  QFileInfo fi(path);
  result.setClassName(className() + "," + fi.fileName());

  // config error handling that required plugin
  _validatePluginConfig(baseFeatureType, ToLocal(&plugin));

  return result;
}

bool ScriptMatchCreator::isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map)
{
  if (!_script)
  {
    throw IllegalArgumentException("The script must be set on the ScriptMatchCreator.");
  }
  return _getCachedVisitor(map)->isMatchCandidate(element);
}

std::shared_ptr<MatchThreshold> ScriptMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    if (!_script)
    {
      throw IllegalArgumentException("The script must be set on the ScriptMatchCreator.");
    }
    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    Context::Scope context_scope(_script->getContext(current));
    Local<Object> plugin = ScriptMatchVisitor::getPlugin(_script);

    double matchThreshold = -1.0;
    double missThreshold = -1.0;
    double reviewThreshold = -1.0;
    try
    {
      matchThreshold = ScriptMatchVisitor::getNumber(plugin, "matchThreshold", 0.0, 1.0);
      missThreshold = ScriptMatchVisitor::getNumber(plugin, "missThreshold", 0.0, 1.0);
      reviewThreshold = ScriptMatchVisitor::getNumber(plugin, "reviewThreshold", 0.0, 1.0);
    }
    catch (const IllegalArgumentException& /*e*/)
    {
    }

    if (matchThreshold != -1.0 && missThreshold != -1.0 && reviewThreshold != -1.0)
    {
      return std::make_shared<MatchThreshold>(matchThreshold, missThreshold, reviewThreshold);
    }
    else
    {
      return std::make_shared<MatchThreshold>();
    }
  }
  return _matchThreshold;
}

QString ScriptMatchCreator::getName() const
{
  QFileInfo scriptFileInfo(_scriptPath);
  return className() + ";" + scriptFileInfo.fileName();
}

QStringList ScriptMatchCreator::getCriteria() const
{
  return _scriptInfo.getMatchCandidateCriteria();
}

}
