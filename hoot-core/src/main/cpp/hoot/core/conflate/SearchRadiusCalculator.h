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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
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

using namespace std;

/**
 * Calculates a conflation search radius using tie points from rubbersheeting.  If there
 * are not enough tie points to perform the calculation
 */
class SearchRadiusCalculator : public OsmMapOperation, public Configurable
{
public:

  static string className() { return "hoot::SearchRadiusCalculator"; }

  SearchRadiusCalculator();

  virtual void apply(boost::shared_ptr<OsmMap>& map);

  virtual any getResult() { any result = _result; return result; }

  virtual void setConfiguration(const Settings& conf);

  void setCircularError(double val) { _circularError = val; }

  void setRubberSheetRef(bool val) { _rubberSheetRef = val; }

  void setRubberSheetMinTies(int minTies) { _minTies = minTies; }

private:

  void _calculateSearchRadius(const vector<double>& tiePointDistances);
  double _calculateStandardDeviation(const vector<double>& samples);

  double _result;

  double _circularError;

  bool _rubberSheetRef;
  int _minTies;

  friend class SearchRadiusCalculatorTest;
};

}

#endif // SEARCH_RADIUS_CALCULATOR_H
