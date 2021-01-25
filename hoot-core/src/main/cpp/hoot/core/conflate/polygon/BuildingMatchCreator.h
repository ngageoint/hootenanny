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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BUILDINGMATCHCREATOR_H
#define BUILDINGMATCHCREATOR_H

#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/criterion/BuildingCriterion.h>

namespace Tgs
{
class RandomForest;
}

namespace hoot
{

class BuildingRfClassifier;

class BuildingMatchCreator : public MatchCreator
{

public:

  static std::string className() { return "hoot::BuildingMatchCreator"; }

  BuildingMatchCreator();
  virtual ~BuildingMatchCreator() = default;

  virtual MatchPtr createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) override;

  virtual void createMatches(
    const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
    ConstMatchThresholdPtr threshold) override;

  virtual std::vector<CreatorDescription> getAllCreators() const override;

  /**
   * Determines whether an element is a candidate for matching for this match creator
   *
   * @param element element to determine the match candidate status of
   * @param map the map the element whose candidacy is being determined belongs to
   * @return true if the element is a match candidate; false otherwise
   */
  virtual bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map) override;

  virtual std::shared_ptr<MatchThreshold> getMatchThreshold() override;

  virtual QString getName() const { return QString::fromStdString(className()); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  virtual QStringList getCriteria() const;

private:

  // Don't use this directly. See below.
  std::shared_ptr<BuildingRfClassifier> _rf;
  std::shared_ptr<MatchThreshold> _matchThreshold;

  QString _conflateMatchBuildingModel;

  BuildingCriterion _buildingCrit;

  /*
   * The building model can be slow to load, this provides a lazy load.
   */
  std::shared_ptr<BuildingRfClassifier> _getRf();
};

}

#endif // BUILDINGMATCHCREATOR_H
