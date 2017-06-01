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
#include "ScriptMatchCreator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/filters/ArbitraryCriterion.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/WorstCircularErrorVisitor.h>
#include <hoot/core/visitors/IndexElementsVisitor.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/SettingsJs.h>

// Qt
#include <QFileInfo>
#include <qnumeric.h>

// Boost
#include <boost/bind.hpp>

// Standard
#include <deque>
#include <math.h>

// TGS
#include <tgs/RStarTree/IntersectionIterator.h>
#include <tgs/RStarTree/MemoryPageStore.h>

#include "ScriptMatch.h"

using namespace geos::geom;
using namespace std;
using namespace Tgs;
using namespace v8;

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, ScriptMatchCreator)

class ScriptMatchVisitor;

/**
 * Searches the specified map for any match potentials.
 */
class ScriptMatchVisitor : public ElementVisitor
{

public:

  ScriptMatchVisitor(const ConstOsmMapPtr& map, vector<const Match*>& result,
    ConstMatchThresholdPtr mt, boost::shared_ptr<PluginContext> script) :
    _map(map),
    _result(result),
    _mt(mt),
    _script(script),
    _customSearchRadius(-1.0)
  {
    _neighborCountMax = -1;
    _neighborCountSum = 0;
    _elementsEvaluated = 0;
    _maxGroupSize = 0;

    HandleScope handleScope;
    Context::Scope context_scope(_script->getContext());
    Handle<Object> plugin = getPlugin();
    _candidateDistanceSigma = getNumber(plugin, "candidateDistanceSigma", 0.0, 1.0);

    //this is meant to have been set externally in a js rules file
    _customSearchRadius = getNumber(plugin, "searchRadius", -1.0, 15.0);
    LOG_VART(_customSearchRadius);

    Handle<v8::Value> value = plugin->Get(toV8("getSearchRadius"));
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
      _getSearchRadius = Persistent<Function>::New(Handle<Function>::Cast(value));
    }

    boost::function<bool (ConstElementPtr e)> f =
      boost::bind(&ScriptMatchVisitor::isMatchCandidate, this, _1);
    ArbitraryCriterion crit(f);
    WorstCircularErrorVisitor worstV;
    FilteredVisitor filteredV(crit, worstV);
    map->visitRo(filteredV);
    _worstCircularError = worstV.getWorstCircularError();
  }

  ~ScriptMatchVisitor()
  {
  }

  void checkForMatch(const boost::shared_ptr<const Element>& e)
  {
    HandleScope handleScope;
    Context::Scope context_scope(_script->getContext());

    ConstOsmMapPtr map = getMap();

    // create an envlope around the e plus the search radius.
    auto_ptr<Envelope> env(e->getEnvelope(map));
    Meters searchRadius = getSearchRadius(e);
    env->expandBy(searchRadius);

    // find other nearby candidates
    set<ElementId> neighbors =
      IndexElementsVisitor::findNeighbors(*env, getIndex(), _indexToEid, getMap());
    ElementId from = e->getElementId();

    _elementsEvaluated++;
    int neighborCount = 0;

    for (set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      ConstElementPtr e2 = map->getElement(*it);
      if ((e->getStatus() != e2->getStatus() || from < *it) && isMatchCandidate(e2))
      {
        // score each candidate and push it on the result vector
        ScriptMatch* m = new ScriptMatch(_script, getPlugin(), map, from, *it, _mt);
        // if we're confident this is a miss
        if (m->getType() == MatchType::Miss)
        {
          delete m;
        }
        else
        {
          _result.push_back(m);
          neighborCount++;
        }
      }
    }

    _neighborCountSum += neighborCount;
    _neighborCountMax = std::max(_neighborCountMax, neighborCount);
  }

  ConstOsmMapPtr getMap() const { return _map.lock(); }

  static double getNumber(Handle<Object> obj, QString key, double minValue, double defaultValue)
  {
    double result = defaultValue;
    Handle<String> cdtKey = String::New(key.toUtf8());
    if (obj->Has(cdtKey))
    {
      Local<Value> v = obj->Get(cdtKey);
      if (v->IsNumber() == false || ::qIsNaN(v->NumberValue()))
      {
        throw IllegalArgumentException("Expected " + key + " to be a number.");
      }
      result = v->NumberValue();

      if (result < minValue)
      {
        throw IllegalArgumentException(QString("Expected %1 to be greater than %2.")
                                       .arg(key).arg(minValue));
      }
    }
    return result;
  }

  Persistent<Object> getPlugin()
  {
    return getPlugin(_script);
  }

  static Persistent<Object> getPlugin(boost::shared_ptr<PluginContext> script)
  {
    Context::Scope context_scope(script->getContext());
    HandleScope handleScope;
    Handle<Object> global = script->getContext()->Global();

    if (global->Has(String::New("plugin")) == false)
    {
      throw IllegalArgumentException("Expected the script to have exports.");
    }

    Handle<Value> pluginValue = global->Get(String::New("plugin"));
    Persistent<Object> plugin = Persistent<Object>::New(Handle<Object>::Cast(pluginValue));
    if (plugin.IsEmpty() || plugin->IsObject() == false)
    {
      throw IllegalArgumentException("Expected plugin to be a valid object.");
    }

    return plugin;
  }

  // See the "Calculating Search Radius" section in the user docs for more information.
  Meters getSearchRadius(const ConstElementPtr& e)
  {
    Meters result;
    if (_getSearchRadius.IsEmpty())
    {
      if (_customSearchRadius < 0)
      {
        //base the radius off of the element itself
        result = e->getCircularError() * _candidateDistanceSigma;
      }
      else
      {
        //base the radius off some predefined radius
        result = _customSearchRadius * _candidateDistanceSigma;
      }
    }
    else
    {
      Handle<Value> jsArgs[1];

      int argc = 0;
      jsArgs[argc++] = ElementJs::New(e);

      LOG_TRACE("Calling getSearchRadius...");
      Handle<Value> f = _getSearchRadius->Call(getPlugin(), argc, jsArgs);

      result = toCpp<Meters>(f) * _candidateDistanceSigma;
    }

    LOG_VART(result);
    return result;
  }

  /*
   * This is meant to run one time when the match creator is initialized.
   */
  void calculateSearchRadius()
  {
    Context::Scope context_scope(_script->getContext());
    HandleScope handleScope;

    Persistent<Object> plugin = getPlugin();
    Handle<String> initStr = String::New("calculateSearchRadius");
    //optional method, so don't throw an error
    if (plugin->Has(initStr) == false)
    {
      LOG_DEBUG("calculateSearchRadius function not present.");
      return;
    }
    Handle<v8::Value> value = plugin->Get(initStr);
    if (value->IsFunction() == false)
    {
      LOG_DEBUG("calculateSearchRadius function not present.");
      return;
    }

    Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
    Handle<Value> jsArgs[1];
    int argc = 0;
    HandleScope scope;
    assert(getMap().get());
    OsmMapPtr copiedMap(new OsmMap(getMap()));
    jsArgs[argc++] = OsmMapJs::create(copiedMap);

    func->Call(plugin, argc, jsArgs);

    //this is meant to have been set externally in a js rules file
    _customSearchRadius = getNumber(plugin, "searchRadius", -1.0, 15.0);
    LOG_VART(_customSearchRadius);

    QFileInfo scriptFileInfo(_scriptPath);
    LOG_DEBUG("Search radius calculation complete for " << scriptFileInfo.fileName());
  }

  boost::shared_ptr<HilbertRTree>& getIndex()
  {
    if (!_index)
    {
      // No tuning was done, I just copied these settings from OsmMapIndex.
      // 10 children - 368
      boost::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
      _index.reset(new HilbertRTree(mps, 2));

      // Only index elements that have Status::Unknown2 and
      boost::shared_ptr<StatusCriterion> pC1(new StatusCriterion(Status::Unknown2));
      boost::function<bool (ConstElementPtr e)> f =
        boost::bind(&ScriptMatchVisitor::isMatchCandidate, this, _1);
      boost::shared_ptr<ArbitraryCriterion> pC2(new ArbitraryCriterion(f));
      boost::shared_ptr<ChainCriterion> pCC(new ChainCriterion());
      pCC->addCriterion(pC1);
      pCC->addCriterion(pC2);

      // Instantiate our visitor
      IndexElementsVisitor v(_index,
                             _indexToEid,
                             pCC,
                             boost::bind(&ScriptMatchVisitor::getSearchRadius, this, _1),
                             getMap());

      // Do the visiting
      getMap()->visitRo(v);
      v.finalizeIndex();
    }

    return _index;
  }

  bool isMatchCandidate(ConstElementPtr e)
  {
    Context::Scope context_scope(_script->getContext());
    HandleScope handleScope;
    Persistent<Object> plugin = getPlugin();
    Handle<String> isMatchCandidateStr = String::New("isMatchCandidate");
    if (plugin->Has(isMatchCandidateStr) == false)
    {
      throw HootException("Error finding 'isMatchCandidate' function.");
    }
    Handle<v8::Value> value = plugin->Get(isMatchCandidateStr);
    if (value->IsFunction() == false)
    {
      throw HootException("isMatchCandidate is not a function.");
    }
    Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
    Handle<Value> jsArgs[2];

    int argc = 0;
    jsArgs[argc++] = OsmMapJs::create(getMap());
    jsArgs[argc++] = ElementJs::New(e);

    Handle<Value> f = func->Call(plugin, argc, jsArgs);

    return f->BooleanValue();
  }

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getStatus() == Status::Unknown1 && isMatchCandidate(e))
    {
      checkForMatch(e);
    }
  }

  void setScriptPath(QString path) { _scriptPath = path; }
  QString getScriptPath() const { return _scriptPath; }

  Meters getCustomSearchRadius() const { return _customSearchRadius; }
  void setCustomSearchRadius(Meters searchRadius) { _customSearchRadius = searchRadius; }

private:

  // don't hold on to the map.
  boost::weak_ptr<const OsmMap> _map;
  vector<const Match*>& _result;
  set<ElementId> _empty;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  ConstMatchThresholdPtr _mt;
  Meters _worstCircularError;
  boost::shared_ptr<PluginContext> _script;
  Persistent<v8::Function> _getSearchRadius;

  // Used for finding neighbors
  boost::shared_ptr<HilbertRTree> _index;
  deque<ElementId> _indexToEid;

  double _candidateDistanceSigma;
  //used for automatic search radius calculation; it is expected that this is set from the
  //Javascript rules file used for the generic conflation
  double _customSearchRadius;
  QString _scriptPath;
};

ScriptMatchCreator::ScriptMatchCreator() :
_worstCircularError(-1.0)
{
  _matchCandidateChecker.reset();
}

ScriptMatchCreator::~ScriptMatchCreator()
{
}

void ScriptMatchCreator::setArguments(QStringList args)
{
  if (args.size() != 1)
  {
    throw HootException("The ScriptMatchCreator takes exactly one argument (script path).");
  }

  _scriptPath = ConfPath::search(args[0], "rules");
  _script.reset(new PluginContext());
  HandleScope handleScope;
  Context::Scope context_scope(_script->getContext());
  _script->loadScript(_scriptPath, "plugin");
  //bit of a hack...see MatchCreator.h...need to refactor
  _description = QString::fromStdString(className()) + "," + args[0];
  _matchCandidateChecker.reset();

  QFileInfo scriptFileInfo(_scriptPath);
  LOG_DEBUG("Set arguments for: " << className() << " - rules: " << scriptFileInfo.fileName());
}

Match* ScriptMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2)
{
  if (isMatchCandidate(map->getElement(eid1), map) && isMatchCandidate(map->getElement(eid2), map))
  {
    return
      new ScriptMatch(
        _script, ScriptMatchVisitor::getPlugin(_script), map, eid1, eid2, getMatchThreshold());
  }
  else
  {
    return 0;
  }
}

void ScriptMatchCreator::createMatches(const ConstOsmMapPtr& map, vector<const Match *> &matches,
                                       ConstMatchThresholdPtr threshold)
{
  if (!_script)
  {
    throw IllegalArgumentException("The script must be set on the ScriptMatchCreator.");
  }

  ScriptMatchVisitor v(map, matches, threshold, _script);
  v.setScriptPath(_scriptPath);
  v.calculateSearchRadius();
  _cachedCustomSearchRadii[_scriptPath] = v.getCustomSearchRadius();
  LOG_VART(_scriptPath);
  LOG_VART(_cachedCustomSearchRadii[_scriptPath]);
  QFileInfo scriptFileInfo(_scriptPath);
  LOG_INFO(
    "Creating matches with: " << className() << "; rules: " << scriptFileInfo.fileName() << "...");
  LOG_VARD(*threshold);
  map->visitRo(v);
}

vector<MatchCreator::Description> ScriptMatchCreator::getAllCreators() const
{
  vector<Description> result;

  // find all the files that end with .js in [ConfPath]/rules/
  QStringList scripts = ConfPath::find(QStringList() << "*.js", "rules");

  // go through each script
  for (int i = 0; i < scripts.size(); i++)
  {
    try
    {
      // if the script is a valid generic script w/ 'description' exposed, add it to the list.
      Description d = _getScriptDescription(scripts[i]);
      if (!d.description.isEmpty())
      {
        result.push_back(d);
        LOG_TRACE(d.description);
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

MatchCreator::Description ScriptMatchCreator::_getScriptDescription(QString path) const
{
  MatchCreator::Description result;
  result.experimental = true;

  boost::shared_ptr<PluginContext> script(new PluginContext());
  HandleScope handleScope;
  Context::Scope context_scope(script->getContext());
  script->loadScript(path, "plugin");

  Persistent<Object> plugin = ScriptMatchVisitor::getPlugin(script);
  Handle<String> descriptionStr = String::New("description");
  if (plugin->Has(descriptionStr))
  {
    Handle<v8::Value> value = plugin->Get(descriptionStr);
    result.description = toCpp<QString>(value);
  }
  Handle<String> experimentalStr = String::New("experimental");
  if (plugin->Has(experimentalStr))
  {
    Handle<v8::Value> value = plugin->Get(experimentalStr);
    result.experimental = toCpp<bool>(value);
  }
  Handle<String> featureTypeStr = String::New("baseFeatureType");
  if (plugin->Has(featureTypeStr))
  {
    Handle<v8::Value> value = plugin->Get(featureTypeStr);
    result.baseFeatureType = MatchCreator::StringToBaseFeatureType(toCpp<QString>(value));
  }

  QFileInfo fi(path);
  result.className = (QString::fromStdString(className()) + "," + fi.fileName()).toStdString();

  return result;
}

bool ScriptMatchCreator::isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map)
{
  if (!_script)
  {
    throw IllegalArgumentException("The script must be set on the ScriptMatchCreator.");
  }

  if (!_matchCandidateChecker.get() || _matchCandidateChecker->getMap() != map)
  {
    LOG_VART(_matchCandidateChecker.get());
    QString scriptPath = _scriptPath;
    if (_matchCandidateChecker.get())
    {
      LOG_VART(_matchCandidateChecker->getMap() == map);
      scriptPath = _matchCandidateChecker->getScriptPath();
    }
    LOG_VART(scriptPath);

    QFileInfo scriptFileInfo(_scriptPath);
    LOG_TRACE("Resetting the match candidate checker " << scriptFileInfo.fileName() << "...");

    vector<const Match*> emptyMatches;
    _matchCandidateChecker.reset(
      new ScriptMatchVisitor(map, emptyMatches, ConstMatchThresholdPtr(), _script));
    _matchCandidateChecker->setScriptPath(scriptPath);
    //If the search radius has already been calculated for this matcher once, we don't want to do
    //it again due to the expense.
    LOG_VART(_cachedCustomSearchRadii.contains(scriptPath));
    if (!_cachedCustomSearchRadii.contains(scriptPath))
    {
      _matchCandidateChecker->calculateSearchRadius();
    }
    else
    {
      LOG_VART(_cachedCustomSearchRadii[scriptPath]);
      _matchCandidateChecker->setCustomSearchRadius(_cachedCustomSearchRadii[scriptPath]);
    }
  }

  return _matchCandidateChecker->isMatchCandidate(element);
}

boost::shared_ptr<MatchThreshold> ScriptMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    if (!_script)
    {
      throw IllegalArgumentException("The script must be set on the ScriptMatchCreator.");
    }
    HandleScope handleScope;
    Context::Scope context_scope(_script->getContext());
    Handle<Object> plugin = ScriptMatchVisitor::getPlugin(_script);

    double matchThreshold = -1.0;
    double missThreshold = -1.0;
    double reviewThreshold = -1.0;
    try
    {
      matchThreshold = ScriptMatchVisitor::getNumber(plugin, "matchThreshold", 0.0, 1.0);
      missThreshold = ScriptMatchVisitor::getNumber(plugin, "missThreshold", 0.0, 1.0);
      reviewThreshold = ScriptMatchVisitor::getNumber(plugin, "reviewThreshold", 0.0, 1.0);
    }
    catch (const IllegalArgumentException& e)
    {
    }

    if (matchThreshold != -1.0 && missThreshold != -1.0 && reviewThreshold != -1.0)
    {
      return boost::shared_ptr<MatchThreshold>(
        new MatchThreshold(matchThreshold, missThreshold, reviewThreshold));
    }
    else
    {
      return boost::shared_ptr<MatchThreshold>(new MatchThreshold());
    }
  }
  return _matchThreshold;
}

}
