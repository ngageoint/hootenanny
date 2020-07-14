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
#ifndef NETWORKMATCHCREATOR_H
#define NETWORKMATCHCREATOR_H

// hoot
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/conflate/network/NetworkDetails.h>
#include <hoot/core/conflate/network/NetworkEdgeScore.h>
#include <hoot/core/criterion/HighwayCriterion.h>

namespace hoot
{

class NetworkMatchCreator : public MatchCreator
{

public:

  static std::string className() { return "hoot::NetworkMatchCreator"; }

  NetworkMatchCreator();
  virtual ~NetworkMatchCreator() = default;

  virtual MatchPtr createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2);

  /**
   * Search the provided map for network matches and add the matches to the matches vector.
   */
  virtual void createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
                             ConstMatchThresholdPtr threshold) override;

  virtual std::vector<CreatorDescription> getAllCreators() const override;

  virtual bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map) override;

  virtual std::shared_ptr<MatchThreshold> getMatchThreshold() override;

  virtual QString getName() const { return QString::fromStdString(className()); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  virtual QStringList getCriteria() const
  { return QStringList(QString::fromStdString(HighwayCriterion::className())); }

private:

  std::shared_ptr<MatchThreshold> _matchThreshold;
  ElementCriterionPtr _userCriterion;

  double _matchScoringFunctionMax;
  double _matchScoringFunctionCurveMidpointX;
  double _matchScoringFunctionCurveSteepness;

  ConstMatchPtr _createMatch(const NetworkDetailsPtr &map, NetworkEdgeScorePtr e,
    ConstMatchThresholdPtr mt);
};

}

#endif // NETWORKMATCHCREATOR_H
