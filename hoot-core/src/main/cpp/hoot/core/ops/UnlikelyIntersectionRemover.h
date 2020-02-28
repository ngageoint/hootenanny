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

#ifndef UNLIKELYINTERSECTIONREMOVER_H
#define UNLIKELYINTERSECTIONREMOVER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/criterion/LinearCriterion.h>

// Standard
#include <set>
#include <vector>

namespace hoot
{

class OsmMap;
class Way;

/**
 * Locates intersections that are likely mistakes and separates them. This is typically a problem
 * with data ingested into OSM (e.g. government data such as TIGER).
 *
 * For example, a motorway overpass intersecting a residential street at a 90° is considered
 * unlikely and "unsnapped". The geometry location is not modified.
 */
class UnlikelyIntersectionRemover : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::UnlikelyIntersectionRemover"; }

  UnlikelyIntersectionRemover();

  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * Splits all the ways in the input map and returns the resulting map.
   */
  static void removeIntersections(std::shared_ptr<OsmMap> map);

  virtual QString getInitStatusMessage() const override
  { return "Removing unlikely intersections..."; }

  virtual QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " unlikely intersections"; }

  virtual QString getDescription() const override
  { return "Removes road intersections that are likely mistakes"; }

  /**
   * @see FilteredByCriteria
   */
  virtual QStringList getCriteria() const
  { return QStringList(QString::fromStdString(LinearCriterion::className())); }

  virtual std::string getClassName() const { return className(); }

protected:

  std::shared_ptr<OsmMap> _result;

  void _evaluateAndSplit(long intersectingNode, const std::set<long>& wayIds);
  double _pIntersection(long intersectingNode, const std::shared_ptr<Way>& w1,
                        const std::shared_ptr<Way>& w2);
  void _splitIntersection(long intersectingNode, const std::vector<std::shared_ptr<Way>>& g2);
};

}


#endif // UNLIKELYINTERSECTIONREMOVER_H
