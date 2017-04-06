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
#ifndef CALCULATESTATSOP_H
#define CALCULATESTATSOP_H

// hoot
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/conflate/MatchCreator.h>
#include "ConstOsmMapOperation.h"
#include "stats/SingleStat.h"

// Qt
#include <QList>
#include <QString>

namespace hoot
{
class ElementVisitor;
class FilteredVisitor;

class CalculateStatsOp : public ConstOsmMapOperation
{
public:

  CalculateStatsOp(QString mapName = "", bool inputIsConflatedMapOutput = false);
  CalculateStatsOp(ElementCriterionPtr criterion, QString mapName = "",
                   bool inputIsConflatedMapOutput = false);

  virtual void apply(const boost::shared_ptr<OsmMap>& map);

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
   * @return ptr to match creator, if found, otherwise boost::shared_ptr to null
   */
  boost::shared_ptr<MatchCreator> getMatchCreator(const vector< boost::shared_ptr<MatchCreator> > &matchCreators,
                                           const QString &matchCreatorName,
                                           MatchCreator::BaseFeatureType &featureType);

  double _applyVisitor(boost::shared_ptr<const OsmMap>& map, const hoot::FilteredVisitor &v);

  double _applyVisitor(boost::shared_ptr<const OsmMap>& map, const hoot::FilteredVisitor &v,
                       any& visitorData);

  void _applyVisitor(boost::shared_ptr<const OsmMap>& map, ElementVisitor *v);

  static bool _matchDescriptorCompare(const MatchCreator::Description& m1,
                                      const MatchCreator::Description& m2);

  void _generateFeatureStats(boost::shared_ptr<const OsmMap>& map, QString description,
                             float conflatableCount, MatchCreator::FeatureCalcType type,
                             ElementCriterion* criterion);
};

}

#endif // CALCULATESTATSOP_H
