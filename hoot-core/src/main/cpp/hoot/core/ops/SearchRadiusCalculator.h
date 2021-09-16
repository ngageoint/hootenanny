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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef SEARCH_RADIUS_CALCULATOR_H
#define SEARCH_RADIUS_CALCULATOR_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Calculates a conflation search radius using tie points from rubbersheeting. If there
 * are not enough tie points to perform the calculation, the default search radius will be used.
 */
class SearchRadiusCalculator : public OsmMapOperation, public Configurable
{
public:

  static QString className() { return "SearchRadiusCalculator"; }

  SearchRadiusCalculator() = default;
  ~SearchRadiusCalculator() override = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  boost::any getResult() override { boost::any result = _result; return result; }

  void setConfiguration(const Settings& conf) override;

  QString getDescription() const override
  { return "Calculates a search radius using tie points found by rubbersheeting"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setCircularError(double val) { _circularError = val; }
  void setRubberSheetRef(bool val) { _rubberSheetRef = val; }
  void setRubberSheetMinTies(int minTies) { _minTies = minTies; }
  void setPrecision(int precision) { _precision = precision; }

private:

  friend class SearchRadiusCalculatorTest;

  double _result;

  double _circularError;

  bool _rubberSheetRef;
  int _minTies;

  int _precision;

  QString _elementCriterion;

  OsmMapPtr _getFilteredMap(const ConstOsmMapPtr& map) const;

  std::vector<double> _getTiePointDistances(OsmMapPtr& map) const;

  void _calculateSearchRadius(const std::vector<double>& tiePointDistances);
  double _calculateStandardDeviation(const std::vector<double>& samples) const;
};

}

#endif // SEARCH_RADIUS_CALCULATOR_H
