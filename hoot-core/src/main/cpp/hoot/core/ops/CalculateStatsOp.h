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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CALCULATESTATSOP_H
#define CALCULATESTATSOP_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/info/SingleStat.h>

// Qt
#include <QList>
#include <QString>

namespace hoot
{
class FilteredVisitor;

/**
 * Calcs the set of stats that feeds the stats command.
 *
 * @todo This has grown quite large and could probably be made a little maintenance-prone with some
 * abstractions. - see #2908
 */
class CalculateStatsOp : public ConstOsmMapOperation
{
public:

  static std::string className() { return "hoot::CalculateStatsOp"; }

  CalculateStatsOp(QString mapName = "", bool inputIsConflatedMapOutput = false);
  CalculateStatsOp(ElementCriterionPtr criterion, QString mapName = "",
                   bool inputIsConflatedMapOutput = false);

  virtual void apply(const OsmMapPtr& map);

  QList<SingleStat> getStats() const { return _stats; }

  void printStats();

  /**
   * O(n)
   */
  double getSingleStat(const QString& n) const;

  /**
   * O(n)
   */
  bool hasSingleStat(const QString& n) const;

  /**
   * O(n)
   */
  long indexOfSingleStat(const QString& n) const;

  void setQuickSubset(bool quick) { _quick = quick; }

  virtual QString getDescription() const { return "Calculates map statistics"; }

private:

  ElementCriterionPtr _criterion;
  //simple map name string for logging purposes
  QString _mapName;
  bool _quick;
  //We differentiate between maps that are the input to a conflation job vs those that are the
  //output of a conflation job.  Another option would be to refactor this class for both maps
  //meant to be input to a conflation job and those that are output from a conflation job.
  bool _inputIsConflatedMapOutput;
  QList<SingleStat> _stats;

  /**
   * @brief getMatchCreator finds the match creator (in the supplied vector) by name
   * @param [in]  matchCreators vector of matchCreators to search
   * @param [in]  matchCreatorName name for which to search
   * @param [out] featureType base feature type for the found matchCreator
   * @return ptr to match creator, if found, otherwise std::shared_ptr to null
   */
  std::shared_ptr<MatchCreator> getMatchCreator(const std::vector<std::shared_ptr<MatchCreator>>& matchCreators,
                                                const QString &matchCreatorName,
                                                CreatorDescription::BaseFeatureType &featureType);

  double _applyVisitor(std::shared_ptr<const OsmMap>& map, const hoot::FilteredVisitor &v);

  double _applyVisitor(std::shared_ptr<const OsmMap>& map, const hoot::FilteredVisitor &v,
                       boost::any& visitorData);

  void _applyVisitor(const std::shared_ptr<const OsmMap>& map, ConstElementVisitor *v);

  static bool _matchDescriptorCompare(const CreatorDescription& m1,
                                      const CreatorDescription& m2);

  void _generateFeatureStats(std::shared_ptr<const OsmMap>& map,
                             const CreatorDescription::BaseFeatureType& featureType,
                             const float conflatableCount,
                             const CreatorDescription::FeatureCalcType& type,
                             ElementCriterionPtr criterion,
                             const long poisMergedIntoPolys);

  ConstElementVisitorPtr _getElementVisitorForFeatureType(
    const CreatorDescription::BaseFeatureType& featureType);
};

}

#endif // CALCULATESTATSOP_H
