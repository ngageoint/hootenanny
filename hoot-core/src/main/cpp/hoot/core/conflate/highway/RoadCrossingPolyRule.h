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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef ROAD_CROSSING_POLY_RULE_H
#define ROAD_CROSSING_POLY_RULE_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/OsmMap.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * Defines a rule indicating which types of polygons should be searched for roads crossing over them
 * and which types of crossing roads are allowed over certain polygons. See the "Crossing Roads"
 * section in the "Feature Validation" documentation for detail on the rule file format.
 */
class RoadCrossingPolyRule
{
public:

  RoadCrossingPolyRule(ConstOsmMapPtr map);
  RoadCrossingPolyRule(RoadCrossingPolyRule && rule) noexcept;

  RoadCrossingPolyRule(const RoadCrossingPolyRule& rule) = default;
  RoadCrossingPolyRule& operator=(const RoadCrossingPolyRule& s) = default;

  /**
   * Reads a road crossing poly rules file and creates a collection of rules
   *
   * @param rulesFile path to the rules file
   * @param map map containing the roads and polygons for which rules are being created
   * @return a collection of rules
   */
  static QList<RoadCrossingPolyRule> readRules(const QString& rulesFile, ConstOsmMapPtr map);

  /**
   * Converts a polygon rule filter string to a criterion filter. At least one of the criteria
   * filter or tag filter must be specified. The two criteria are logically AND'd together.
   *
   * @param polyCriteriaFilterStr polygon criteria filter string used to determine which types of
   * polygons are checked for crossing roads using Hootenanny criteria of the form:
   * <criterion 1>;<criterion 2>...; e.g. "BuildingCriterion;AreaCriterion"; the
   * individual type criteria are logically OR'd together
   * @param polyTagFilterStr polygon tag filter string used to determine which types of polygons
   * are checked for crossing roads using tag checks of the form: <key1=value1>;<key2=value2>...;
   * e.g. "amenity=parking;leisure=park"; the individual tag criteria are logically OR'd together
   * @return a polygon criterion filter
   */
  static ElementCriterionPtr polyRuleFilterStringsToFilter(const QString& polyCriteriaFilterStr, const QString& polyTagFilterStr);

  /**
   * Converts a tag rule to a criterion filter
   *
   * @param kvpStr tag string of the form: <key1=value1>;<key2=value2>...; the individual tag
   * criteria are logically OR'd together; wildcards ('*') are allowed for values but not keys
   * @param allowedKeys optional list of allowed keys to restrict the input allowed to the tag
   * filter
   * @return a tag criterion filter or an empty criterion if the input tags are empty
   */
  static ElementCriterionPtr tagRuleStringToFilter(const QString& kvpStr, const QStringList& allowedKeys = QStringList());

  /**
   * Creates the spatial index used to search for crossing roads
   */
  void createIndex();

  bool operator<(const RoadCrossingPolyRule& other) const;

  QString getName() const { return _name; }
  void setName(const QString& name) { _name = name; }

  QString getPolyFilterString() const { return _polyFilterStr; }
  ElementCriterionPtr getPolyFilter() const { return _polyFilter; }
  QString getAllowedRoadTagFilterString() const { return _allowedRoadTagFilterStr; }
  ElementCriterionPtr getAllowedRoadTagFilter() const { return _allowedRoadTagFilter; }
  std::shared_ptr<Tgs::HilbertRTree> getIndex() const { return _index; }
  std::deque<ElementId> getIndexToEid() const { return _indexToEid; }

  void setPolyFilterString(const QString& filterStr) { _polyFilterStr = filterStr; }
  void setPolyFilter(const ElementCriterionPtr& filter) { _polyFilter = filter; }
  void setAllowedRoadTagFilterString(const QString& filterStr)
  { _allowedRoadTagFilterStr = filterStr; }
  void setAllowedRoadTagFilter(const ElementCriterionPtr& filter)
  { _allowedRoadTagFilter = filter; }
  void setIndex(const std::shared_ptr<Tgs::HilbertRTree>& index) { _index = index; }
  void setIndexToEid(const std::deque<ElementId>& indexToEid) { _indexToEid = indexToEid; }

private:

  ConstOsmMapPtr _map;

  // name of the rule; shown in reviews
  QString _name;

  // raw poly combined filter string used for display purposes only
  QString _polyFilterStr;
  // the filter used to search for polys with roads crossing over them
  ElementCriterionPtr _polyFilter;

  // raw allowed road filter string used for display purposes only
  QString _allowedRoadTagFilterStr;
  // the filter that defines which roads are allowed to cross over the polys passing the _polyFilter
  // filter
  ElementCriterionPtr _allowedRoadTagFilter;

  // spatial indexes
  std::shared_ptr<Tgs::HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  Meters _getSearchRadius(const ConstElementPtr& e) const;
  bool _isMatchCandidate(ConstElementPtr element) const;
};

}

#endif // ROAD_CROSSING_POLY_RULE_H
