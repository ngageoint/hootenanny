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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ScriptMergerCreator.h"

// hoot
#include <hoot/core/conflate/merging/MarkForReviewMerger.h>
#include <hoot/core/util/Factory.h>
#include <hoot/js/conflate/matching/ScriptMatch.h>
#include <hoot/js/conflate/merging/ScriptMerger.h>
#include <hoot/core/util/StringUtils.h>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, ScriptMergerCreator)

bool ScriptMergerCreator::createMergers(const MatchSet& matches, vector<MergerPtr>& mergers) const
{
  LOG_TRACE(
    "Creating merger group with " << className() << " for " <<
    StringUtils::formatLargeNumber(matches.size()) << " match(es)...");

  bool result = false;
  assert(matches.size() > 0);

  set<pair<ElementId, ElementId>> eids;

  std::shared_ptr<PluginContext> script;
  Persistent<Object> plugin;
  QStringList matchType;

  // go through all the matches
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    ConstMatchPtr m = *it;
    //LOG_VART(m->toString());
    std::shared_ptr<const ScriptMatch> sm = dynamic_pointer_cast<const ScriptMatch>(m);
    // check to make sure all the input matches are script matches
    if (sm == 0)
    {
      // return an empty result
      LOG_TRACE("Match invalid; skipping merge: " << m->toString());
      return false;
    }
    // add all the element to element pairs to a set
    else
    {
      script = sm->getScript();

      Isolate* current = v8::Isolate::GetCurrent();
      HandleScope handleScope(current);
      Context::Scope context_scope(script->getContext(current));

      plugin.Reset(current, sm->getPlugin());
      set<pair<ElementId, ElementId>> s = sm->getMatchPairs();
      eids.insert(s.begin(), s.end());
      if (matchType.contains(sm->getName()) == false)
      {
        matchType.append(sm->getName());
      }
    }
  }
  LOG_VART(eids);

  std::shared_ptr<ScriptMerger> sm(new ScriptMerger(script, plugin, eids));
  sm->setMatchType(matchType.join(";"));
  // only add the merger if there are elements to merge
  if (sm->hasFunction("mergeSets"))
  {
    if (eids.size() >= 1)
    {
      mergers.push_back(sm);
      result = true;
    }
  }
  else
  {
    if (eids.size() == 1)
    {
      mergers.push_back(sm);
      result = true;
    }
    else if (eids.size() > 1)
    { 
      LOG_TRACE("Overlapping matches:\n" << eids << "\nmatch types: " << matchType.join(";"));
      mergers.push_back(
        MergerPtr(new MarkForReviewMerger(eids, "Overlapping matches", matchType.join(";"), 1.0)));
      result = true;
    }
  }

  LOG_TRACE(
    "Created " << StringUtils::formatLargeNumber(mergers.size()) <<  " merger(s) for group.");
  return result;
}

vector<CreatorDescription> ScriptMergerCreator::getAllCreators() const
{
  CreatorDescription d;
  d.className = QString::fromStdString(className());
  d.description = "Generates mergers used in Generic Conflation";
  d.experimental = false;
  vector<CreatorDescription> result;
  result.push_back(d);

  return result;
}

bool ScriptMergerCreator::isConflicting(
  const ConstOsmMapPtr& map, ConstMatchPtr m1, ConstMatchPtr m2,
  const QHash<QString, ConstMatchPtr>& matches) const
{
  const ScriptMatch* sm1 = dynamic_cast<const ScriptMatch*>(m1.get());
  const ScriptMatch* sm2 = dynamic_cast<const ScriptMatch*>(m2.get());

  bool result = false;
  if (sm1 && sm2)
  {
    result = m1->isConflicting(m2, map, matches);
  }

  return result;
}

}
