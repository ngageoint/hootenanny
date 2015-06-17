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
 * @copyright Copyright (C) 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ScriptMatchCreator.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/SettingsJs.h>

// Qt
#include <QFileInfo>

#include "ScriptMatch.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, ScriptMatchCreator)

class ScriptMatchVisitor;
class SmWorstCircularErrorVisitor : public ElementVisitor
{
public:
  Meters _worst;
  ScriptMatchVisitor& _smv;

  SmWorstCircularErrorVisitor(ScriptMatchVisitor& smv) : _smv(smv)
  {
    _worst = -1;
  }

  Meters getWorstCircularError() { return _worst; }

  virtual void visit(const ConstElementPtr& e);
};

/**
 * Searches the specified map for any match potentials.
 */
class ScriptMatchVisitor : public ElementVisitor
{

public:

  ScriptMatchVisitor(const ConstOsmMapPtr& map, vector<const Match*>& result,
    ConstMatchThresholdPtr mt, shared_ptr<PluginContext> script) :
    _map(map),
    _result(result),
    _mt(mt),
    _script(script),
    _searchRadius(-1.0)
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
    _searchRadius = getNumber(plugin, "searchRadius", -1.0, 15.0);

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

    SmWorstCircularErrorVisitor v(*this);
    map->visitRo(v);
    _worstCircularError = v.getWorstCircularError();
  }

  ~ScriptMatchVisitor()
  {
  }

  void checkForMatch(const shared_ptr<const Element>& e)
  {
    HandleScope handleScope;
    Context::Scope context_scope(_script->getContext());

    auto_ptr<Envelope> env(e->getEnvelope(_map));
    //if _searchRadius is unchanged, don't use it as an auto-calculated search radius value and
    //expand the search envelope based on the element's circular error and map worst circular error
    //instead
    if (_searchRadius == -1.0)
    {
      env->expandBy(_candidateDistanceSigma * _worstCircularError + getSearchRadius(e));
    }
    else
    {
      env->expandBy(_candidateDistanceSigma * _searchRadius);
    }

    // find other nearby candidates
    set<ElementId> neighbors = _map->findElements(*env);
    ElementId from(e->getElementType(), e->getId());

    _elementsEvaluated++;
    int neighborCount = 0;

    for (set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      ConstElementPtr e2 = _map->getElement(*it);
      if ((e->getStatus() != e2->getStatus() || from < *it) && isMatchCandidate(e2))
      {
        // score each candidate and push it on the result vector
        ScriptMatch* m = new ScriptMatch(_script, getPlugin(), _map, from, *it, _mt);
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

  static double getNumber(Handle<Object> obj, QString key, double minValue, double defaultValue)
  {
    double result = defaultValue;
    Handle<String> cdtKey = String::New(key.toUtf8());
    if (obj->Has(cdtKey))
    {
      Local<Value> v = obj->Get(cdtKey);
      if (v->IsNumber() == false)
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

  static Persistent<Object> getPlugin(shared_ptr<PluginContext> script)
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

  Meters getSearchRadius(const ConstElementPtr& e)
  {
    if (_getSearchRadius.IsEmpty())
    {
      return e->getCircularError() * _candidateDistanceSigma;
    }
    else
    {
      Handle<Value> jsArgs[1];

      int argc = 0;
      jsArgs[argc++] = ElementJs::New(e);

      Handle<Value> f = _getSearchRadius->Call(getPlugin(), argc, jsArgs);

      return toCpp<Meters>(f);
    }
  }

  /*
   * This is meant to run one time when the match creator is initialized.
   */
  void customScriptInit()
  {
    Context::Scope context_scope(_script->getContext());
    HandleScope handleScope;

    Persistent<Object> plugin = getPlugin();
    Handle<String> initStr = String::New("init");
    if (plugin->Has(initStr) == false)
    {
      throw HootException("Error finding 'init' function.");
    }
    Handle<v8::Value> value = plugin->Get(initStr);
    if (value->IsFunction() == false)
    {
      throw HootException("init is not a function.");
    }

    Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
    Handle<Value> jsArgs[1];
    int argc = 0;
    HandleScope scope;
    assert(_map.get());
    OsmMapPtr copiedMap(new OsmMap(_map));
    jsArgs[argc++] = OsmMapJs::create(copiedMap);

    func->Call(plugin, argc, jsArgs);

    //this is meant to have been set externally in a js rules file
    _searchRadius = getNumber(plugin, "searchRadius", -1.0, 15.0);
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
    jsArgs[argc++] = OsmMapJs::create(_map);
    jsArgs[argc++] = ElementJs::New(e);

    Handle<Value> f = func->Call(plugin, argc, jsArgs);

    return f->BooleanValue();
  }

  virtual void visit(ElementType type, long id)
  {
    shared_ptr<const Element> element(_map->getElement(type, id));
    if (element->getStatus() == Status::Unknown1 && isMatchCandidate(element))
    {
      checkForMatch(element);
    }
  }

private:

  const ConstOsmMapPtr& _map;
  vector<const Match*>& _result;
  set<ElementId> _empty;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  ConstMatchThresholdPtr _mt;
  Meters _worstCircularError;
  shared_ptr<PluginContext> _script;
  Persistent<v8::Function> _getSearchRadius;

  double _candidateDistanceSigma;
  //used for automatic search radius calculation; it is expected that this is set from the Javascript
  //rules file used for the generic conflation
  double _searchRadius;

  double _test;

};


void SmWorstCircularErrorVisitor::visit(const ConstElementPtr& e)
{
  if (_smv.isMatchCandidate(e))
  {
    _worst = max(_worst, _smv.getSearchRadius(e));
  }
}

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

  QString path = ConfPath::search(args[0], "rules");
  _script.reset(new PluginContext());
  HandleScope handleScope;
  Context::Scope context_scope(_script->getContext());
  _script->loadScript(path, "plugin");
}

void ScriptMatchCreator::createMatches(const ConstOsmMapPtr& map, vector<const Match *> &matches,
                                       ConstMatchThresholdPtr threshold)
{
  if (!_script)
  {
    throw IllegalArgumentException("The script must be set on the ScriptMatchCreator.");
  }
  ScriptMatchVisitor v(map, matches, threshold, _script);
  v.customScriptInit();
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
      }
      else
      {
        LOG_DEBUG(QString("Skipping reporting script %1 because it has no "
          "description.").arg(scripts[i]));
      }
    }
    catch (HootException& e)
    {
      LOG_INFO("Error loading script: " + scripts[i] + " exception: " + e.getWhat());
    }
  }

  return result;
}

MatchCreator::Description ScriptMatchCreator::_getScriptDescription(QString path) const
{
  MatchCreator::Description result;
  result.experimental = true;

  shared_ptr<PluginContext> script(new PluginContext());
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
  if (!_matchCandidateChecker.get())
  {
    vector<const Match *> emptyMatches;
    _matchCandidateChecker.reset(
      new ScriptMatchVisitor(map, emptyMatches, ConstMatchThresholdPtr(), _script));
    _matchCandidateChecker->customScriptInit();
  }
  return _matchCandidateChecker->isMatchCandidate(element);
}

shared_ptr<MatchThreshold> ScriptMatchCreator::getMatchThreshold()
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
      return shared_ptr<MatchThreshold>(
        new MatchThreshold(matchThreshold, missThreshold, reviewThreshold));
    }
    else
    {
      return shared_ptr<MatchThreshold>(new MatchThreshold());
    }
  }
  return _matchThreshold;
}

}
