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

#ifndef SMALLHIGHWAYMERGER_H
#define SMALLHIGHWAYMERGER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/criterion/HighwayCriterion.h>

// Std
#include <set>

namespace hoot
{

class OsmMap;
class Way;
class TagDifferencer;
class NodeToWayMap;

/**
 * Merge any ludicrously small ways that have essentially the same attributes. Things like `UUID`
 * are ignored. See `small.highway.merger.threshold` for setting the threshold value. Searches for
 * itty bitty ways that connect end to end. For some reason some files provide little  way segments
 * of 2m or less in length. Silliness. If:
 *   - the attributes are essentially identical
 *   - there are only two ways meeting (not an intersection)
 *   - and one is less than the specified threshold
 *   - OsmSchema::isHighway returns true
 * The ways will be merged.
 *
 * Assumptions:
 *  - All intersections have been split. (IntersectionSplitter)
 *  - All duplicate ways have been removed. (DuplicateWayRemover)
 */
class SmallHighwayMerger : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::SmallHighwayMerger"; }

  SmallHighwayMerger(Meters threshold = -1);

  void apply(std::shared_ptr<OsmMap>& map);

  /**
   * Remove parts of ways that are duplicates.
   */
  static void mergeWays(std::shared_ptr<OsmMap> map, Meters threshold);

  virtual QString getInitStatusMessage() const { return "Merging very small roads..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Merged " + QString::number(_numAffected) + " very small roads"; }

  virtual QString getDescription() const { return "Merges very small roads"; }

  /**
   * @see FilteredByCriteria
   */
  virtual QStringList getCriteria() const
  { return QStringList(QString::fromStdString(HighwayCriterion::className())); }

  virtual std::string getClassName() const { return className(); }

protected:

  std::shared_ptr<OsmMap> _map;

  double _threshold;
  NodeToWayMap* _n2w;
  std::shared_ptr<TagDifferencer> _diff;

  void _mergeNeighbors(const std::shared_ptr<Way>& w);
  void _mergeWays(const std::set<long>& ids);
};

}

#endif // SMALLHIGHWAYMERGER_H
