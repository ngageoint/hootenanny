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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef BUILDING_LEVELS_VISITOR_H
#define BUILDING_LEVELS_VISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/criterion/BuildingCriterion.h>

namespace hoot
{

/**
 *
 */
class BuildingLevelsVisitor : public ConstElementVisitor, public NumericStatistic
{
public:

  static std::string className() { return "hoot::BuildingLevelsVisitor"; }

  BuildingLevelsVisitor();

  virtual void visit(const ConstElementPtr& e) override;

  virtual QString getDescription() const { return "Calculates building level statistics"; }

  virtual QString getInitStatusMessage() const
  { return "Calculating building level statistics..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Calculated level statistics for " + QString::number(_numAffected) + " buildings"; }

  virtual long numWithStat() const { return _numAffected; }
  virtual double getStat() const { return _totalLevels; }
  virtual double getMin() const { return _minLevels; }
  virtual double getMax() const { return _maxLevels; }
  virtual double getAverage() const
  {
    const double average = _numAffected == 0 ? 0.0 : _totalLevels / _numAffected;
    return average;
  }

  virtual std::string getClassName() const { return className(); }

private:

  BuildingCriterion _crit;

  double _totalLevels;
  double _minLevels;
  double _maxLevels;

  int _getNumLevels(const ConstElementPtr& e) const;
};

}

#endif // BUILDING_LEVELS_VISITOR_H
