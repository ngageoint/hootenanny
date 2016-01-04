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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SCRIPTMATCHCREATOR_H
#define SCRIPTMATCHCREATOR_H

#include <hoot/core/conflate/MatchCreator.h>
#include <hoot/core/util/NotImplementedException.h>

#include <hoot/js/PluginContext.h>

namespace hoot
{

class ScriptMatchVisitor;

/**
 * @sa ScriptMatch
 */
class ScriptMatchCreator : public MatchCreator
{
public:

  static string className() { return "hoot::ScriptMatchCreator"; }

  ScriptMatchCreator();

  virtual ~ScriptMatchCreator();

  /**
   * Not implemented.
   */
  virtual Match* createMatch(const ConstOsmMapPtr&, ElementId, ElementId);

  /**
   * Search the provided map for POI matches and add the matches to the matches vector.
   */
  virtual void createMatches(const ConstOsmMapPtr& map, vector<const Match*>& matches,
    ConstMatchThresholdPtr threshold);

  virtual vector<Description> getAllCreators() const;

  virtual void setArguments(QStringList args);

  /**
   * Determines whether an element is a candidate for matching for this match creator
   *
   * @param element element to determine the match candidate status of
   * @param map the map the element whose candidacy is being determined belongs to
   * @return true if the element is a match candidate; false otherwise
   */
  virtual bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map);

  virtual shared_ptr<MatchThreshold> getMatchThreshold();

private:

  shared_ptr<PluginContext> _script;

  Description _getScriptDescription(QString path) const;

  shared_ptr<ScriptMatchVisitor> _matchCandidateChecker;
  double _worstCircularError;
  shared_ptr<MatchThreshold> _matchThreshold;

};

}

#endif // SCRIPTMATCHCREATOR_H
