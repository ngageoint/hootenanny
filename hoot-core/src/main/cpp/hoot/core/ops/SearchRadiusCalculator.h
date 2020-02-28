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

#ifndef SEARCH_RADIUS_CALCULATOR_H
#define SEARCH_RADIUS_CALCULATOR_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>

// Std
#include <vector>

namespace hoot
{

class OsmMap;

/**
 * Calculates a conflation search radius using tie points from rubbersheeting.  If there
 * are not enough tie points to perform the calculation, the default search radius will be used.
 */
class SearchRadiusCalculator : public OsmMapOperation, public Configurable
{
public:

  static std::string className() { return "hoot::SearchRadiusCalculator"; }

  SearchRadiusCalculator();

  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  virtual boost::any getResult() override { boost::any result = _result; return result; }

  virtual void setConfiguration(const Settings& conf);

  void setCircularError(double val) { _circularError = val; }

  void setRubberSheetRef(bool val) { _rubberSheetRef = val; }

  void setRubberSheetMinTies(int minTies) { _minTies = minTies; }

  void setPrecision(int precision) { _precision = precision; }

  virtual QString getDescription() const override
  { return "Calculates a search radius using tie points found by rubbersheeting"; }

  virtual std::string getClassName() const { return className(); }

private:

  void _calculateSearchRadius(const std::vector<double>& tiePointDistances);
  double _calculateStandardDeviation(const std::vector<double>& samples);

  double _result;

  double _circularError;

  bool _rubberSheetRef;
  int _minTies;

  int _precision;

  QString _elementCriterion;

  friend class SearchRadiusCalculatorTest;
};

}

#endif // SEARCH_RADIUS_CALCULATOR_H
