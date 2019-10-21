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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef NODES_PER_WAY_VISITOR_H
#define NODES_PER_WAY_VISITOR_H

// hoot
#include <hoot/core/visitors/MultipleCriterionConsumerVisitor.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class NodesPerWayVisitor : public MultipleCriterionConsumerVisitor, public NumericStatistic,
  public OperationStatusInfo, public Configurable
{
public:

  static std::string className() { return "hoot::NodesPerWayVisitor"; }

  NodesPerWayVisitor();

  virtual void visit(const ElementPtr& e) override;

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const { return "Calculates way node statistics"; }

  virtual QString getInitStatusMessage() const
  { return "Calculating way node statistics..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Calculated node statistics for " + QString::number(_numAffected) + " ways"; }

  virtual long numWithStat() const { return _numAffected; }
  virtual double getStat() const { return _totalWayNodes; }
  virtual double getMin() const { return _minNodesPerWay; }
  virtual double getMax() const { return _maxNodesPerWay; }
  virtual double getAverage() const
  {
    const double average = _numAffected == 0 ? 0.0 : _totalWayNodes / _numAffected;
    return average;
  }

private:

  WayCriterion _wayCrit;

  int _totalWayNodes;
  int _minNodesPerWay;
  int _maxNodesPerWay;
};

}

#endif // NODES_PER_WAY_VISITOR_H
