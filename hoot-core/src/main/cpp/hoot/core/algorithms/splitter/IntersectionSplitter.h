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

#ifndef INTERSECTIONSPLITTER_H
#define INTERSECTIONSPLITTER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/criterion/ElementCriterion.h>

// Qt
#include <QMultiHash>
#include <QSet>
#include <QMap>
#include <QList>

namespace hoot
{

class OsmMap;
class Way;

/**
 * Given an OsmMap intersection splitter makes all intersections contain only way end nodes. For
 * example, if two ways make a four way intesection the intersection splitter will convert that
 * into four ways that meet at a four way intersection. No nodes are modified in this process.
 */
class IntersectionSplitter : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::IntersectionSplitter"; }

  IntersectionSplitter();

  IntersectionSplitter(const std::shared_ptr<OsmMap>& map);

  void apply(std::shared_ptr<OsmMap>& map) override;

  static void splitIntersections(const std::shared_ptr<OsmMap>& map);

  void splitIntersections();

  virtual QString getInitStatusMessage() const { return "Splitting road intersections..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Split " + QString::number(_numAffected) + " road intersections"; }

  virtual QString getDescription() const override
  { return "Makes all road intersections contain only way end nodes"; }

  /**
   * @see FilteredByCriteria
   */
  virtual QStringList getCriteria() const;

  virtual std::string getClassName() const { return className(); }

private:

  std::shared_ptr<OsmMap> _map;
  QMultiHash<long, long> _nodeToWays;
  QSet<long> _todoNodes;

  void _mapNodesToWays();
  void _mapNodesToWay(const std::shared_ptr<Way>& w);
  void _removeWayFromMap(const std::shared_ptr<Way>& way);

  /**
   * Given a way and a node, split the way at that node.
   */
  void _splitWay(long wayId, long nodeId);
};

}

#endif // INTERSECTIONSPLITTER_H
