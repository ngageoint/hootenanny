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
#ifndef MATCHCREATOR_H
#define MATCHCREATOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchThreshold.h>

// Standard
#include <string>
#include <vector>

namespace hoot
{

using namespace std;
class Match;

class MatchCreator
{
public:
  class Description
  {
  public:
    Description() : experimental() {}
    Description(string className, QString description, bool experimental)
    {
      this->className = className;
      this->experimental = experimental;
      this->description = description;
    }

    bool experimental;
    string className;
    QString description;
  };

  static string className() { return "hoot::MatchCreator"; }

  virtual ~MatchCreator() {}

  /**
   * Given two elements, create a match if it is appropriate. If it is not appropriate then return
   * null.
   */
  virtual Match* createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) = 0;

  /**
   * Search the provided map for building matches and add the matches to the matches vector.
   */
  virtual void createMatches(const ConstOsmMapPtr& map, vector<const Match*>& matches,
    ConstMatchThresholdPtr threshold) = 0;

  /**
   * Generally this just returns the class name of this creator. However, creators that take
   * arguments to specify scripts such as the ScriptMatchCreator may return multiple results.
   */
  virtual vector<Description> getAllCreators() const = 0;

  /**
   * Determines whether an element is a candidate for matching for this match creator
   *
   * @param element element to determine the match candidate status of
   * @param map the map the element whose candidacy is being determined belongs to
   * @return true if the element is a match candidate; false otherwise
   */
  virtual bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map) = 0;

  virtual shared_ptr<MatchThreshold> getMatchThreshold() = 0;

  /**
   * Arguments are passed in by the MatchFactory.
   */
  virtual void setArguments(QStringList /*args*/)
  {
    throw HootException("This match creator takes no argument.");
  }

  /*
   * This is actually being done in order to track the script name in ScriptMatchCreator, so we
   * need to do some refactoring to get rid of this.  Redundant with Description class above.
   */
  QString getDescription() { return _description; }
  void setDescription(QString description) { _description = description; }

protected:

  QString _description;

};

}

#endif // MATCHCREATOR_H
