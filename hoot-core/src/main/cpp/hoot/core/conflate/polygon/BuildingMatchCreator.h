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
#ifndef BUILDINGMATCHCREATOR_H
#define BUILDINGMATCHCREATOR_H

#include <hoot/core/conflate/MatchCreator.h>

namespace Tgs
{
class RandomForest;
}

namespace hoot
{
class BuildingRfClassifier;

using namespace Tgs;

class BuildingMatchCreator : public MatchCreator
{

public:

  static string className() { return "hoot::BuildingMatchCreator"; }

  BuildingMatchCreator();

  virtual Match* createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2);

  virtual void createMatches(const ConstOsmMapPtr& map, vector<const Match*>& matches,
    ConstMatchThresholdPtr threshold);

  virtual vector<Description> getAllCreators() const;

  /**
   * Determines whether an element is a candidate for matching for this match creator
   *
   * @param element element to determine the match candidate status of
   * @param map the map the element whose candidacy is being determined belongs to
   * @return true if the element is a match candidate; false otherwise
   */
  virtual bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map);

  virtual boost::shared_ptr<MatchThreshold> getMatchThreshold();

private:

  /// Don't use this directly. See below.
  boost::shared_ptr<BuildingRfClassifier> _rf;
  boost::shared_ptr<MatchThreshold> _matchThreshold;

  /**
   * The building model can be slow to load, this provides a lazy load.
   */
  boost::shared_ptr<BuildingRfClassifier> _getRf();
};

}

#endif // BUILDINGMATCHCREATOR_H
