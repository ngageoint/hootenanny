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

#ifndef ROAD_CROSSING_POLY_RULE_H
#define ROAD_CROSSING_POLY_RULE_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * TODO
 *
 * We may want to make this part of the validate command as well.
 */
class RoadCrossingPolyRule
{
public:

  RoadCrossingPolyRule(ConstOsmMapPtr map);

  /**
   * TODO
   *
   * @param rulesFile
   * @param map
   * @return
   */
  static QList<RoadCrossingPolyRule> readRules(const QString& rulesFile, ConstOsmMapPtr map);

  /**
   * TODO
   *
   * @param polyTypeFilterStr
   * @param polyTagFilterStr
   * @return
   */
  static ElementCriterionPtr polyRuleStringsToCrit(
    const QString& polyTypeFilterStr, const QString& polyTagFilterStr);

  /**
   * TODO
   *
   * @param kvpStr
   * @return
   */
  static ElementCriterionPtr kvpRuleStringToTagCrit(const QString& kvpStr);

  /**
   * TODO
   */
  void createIndex();

  QString getPolyFilterString() const { return _polyFilterStr; }
  void setPolyFilterString(const QString& filterStr) { _polyFilterStr = filterStr; }
  ElementCriterionPtr getPolyFilter() const { return _polyFilter; }
  void setPolyFilter(const ElementCriterionPtr& filter) { _polyFilter = filter; }

  QString getAllowedRoadTagFilterString() const { return _allowedRoadTagFilterStr; }
  void setAllowedRoadTagFilterString(const QString& filterStr)
  { _allowedRoadTagFilterStr = filterStr; }
  ElementCriterionPtr getAllowedRoadTagFilter() const { return _allowedRoadTagFilter; }
  void setAllowedRoadTagFilter(const ElementCriterionPtr& filter)
  { _allowedRoadTagFilter = filter; }

  std::shared_ptr<Tgs::HilbertRTree> getIndex() const { return _index; }
  void setIndex(const std::shared_ptr<Tgs::HilbertRTree>& index) { _index = index; }

  std::deque<ElementId> getIndexToEid() const { return _indexToEid; }
  void setIndexToEid(const std::deque<ElementId>& indexToEid) { _indexToEid = indexToEid; }

 private:

  ConstOsmMapPtr _map;

  QString _polyFilterStr;
  ElementCriterionPtr _polyFilter;
  QString _allowedRoadTagFilterStr;
  ElementCriterionPtr _allowedRoadTagFilter;

  std::shared_ptr<Tgs::HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  Meters _getSearchRadius(const ConstElementPtr& e) const;
  bool _isMatchCandidate(ConstElementPtr element);
};

}

#endif // ROAD_CROSSING_POLY_RULE_H
