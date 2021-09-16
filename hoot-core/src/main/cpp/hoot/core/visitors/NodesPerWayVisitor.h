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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef NODES_PER_WAY_VISITOR_H
#define NODES_PER_WAY_VISITOR_H

// hoot
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/criterion/WayCriterion.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/criterion/ElementCriterionConsumer.h>

namespace hoot
{

/*
 * Making this inherit from MultipleCriterionConsumerVisitor worked for some situations but caused
 * incorrect output when this was used with CalculateStats op, so implementing single crierion
 * support without inheritance. It appears the incorrect behavior has something to do with
 * MultipleCriterionConsumerVisitor inheriting from ElementVisitor and not ConstElementVisitor but
 * not exactly sure why yet. See related note in MultipleCriterionConsumerVisitor.
 */
class NodesPerWayVisitor : public ConstElementVisitor, public NumericStatistic,
  public Configurable, public ElementCriterionConsumer
{
public:

  static QString className() { return "NodesPerWayVisitor"; }

  NodesPerWayVisitor();
  ~NodesPerWayVisitor() override = default;

  void addCriterion(const ElementCriterionPtr& e) override;

  void visit(const ConstElementPtr& e) override;

  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override
  { return "Calculating way node statistics..."; }
  QString getCompletedStatusMessage() const override
  { return "Calculated node statistics for " + QString::number(_numAffected) + " ways"; }

  long numWithStat() const override { return _numAffected; }
  double getStat() const override { return _totalWayNodes; }
  double getMin() const override { return _minNodesPerWay; }
  double getMax() const override { return _maxNodesPerWay; }
  double getAverage() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Calculates way node statistics"; }

private:

  WayCriterion _wayCrit;
  std::shared_ptr<ElementCriterion> _customCrit;

  int _totalWayNodes;
  int _minNodesPerWay;
  int _maxNodesPerWay;

  bool _negateCriterion;

  void _setCriterion(const QString& criterionName);
};

}

#endif // NODES_PER_WAY_VISITOR_H
