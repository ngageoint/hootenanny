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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef ROADS_CROSSING_POLYS_REVIEW_MARKER_H
#define ROADS_CROSSING_POLYS_REVIEW_MARKER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/criterion/ElementCriterion.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 * TODO
 */
class RoadsCrossingPolysReviewMarker : public ConstOsmMapOperation, public Configurable
{
public:

  static std::string className() { return "hoot::RoadsCrossingPolysReviewMarker"; }

  RoadsCrossingPolysReviewMarker();

  /**
   * @see ConstOsmMapOperation
   */
  void apply(const OsmMapPtr& map);

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  virtual QString getInitStatusMessage() const
  { return "TODO..."; }

  virtual QString getCompletedStatusMessage() const
  { return "TODO"; }

  virtual QString getDescription() const
  { return "TODO"; }

  virtual std::string getClassName() const { return className(); }

 private:

  OsmMapPtr _map;
  QStringList _polyFilterList;
  ElementCriterionPtr _polyFilter;
  QSet<ElementId> _markedRoads;

  std::shared_ptr<Tgs::HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  ElementCriterionPtr _kvpStringToTagCrit(const QString& kvpStr);
  void _createPolyFilter(const QStringList& polyFilterList);

  std::shared_ptr<Tgs::HilbertRTree>& _getIndex();
  Meters _getSearchRadius(const ConstElementPtr& e) const;
  bool _isMatchCandidate(ConstElementPtr element);
};

}

#endif // ROADS_CROSSING_POLYS_REVIEW_MARKER_H
