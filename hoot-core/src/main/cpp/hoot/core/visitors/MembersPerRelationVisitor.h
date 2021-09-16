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

#ifndef MEMBERS_PER_RELATION_VISITOR_H
#define MEMBERS_PER_RELATION_VISITOR_H

// hoot
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/criterion/RelationCriterion.h>

namespace hoot
{

class MembersPerRelationVisitor : public ConstElementVisitor, public NumericStatistic
{
public:

  static QString className() { return "MembersPerRelationVisitor"; }

  MembersPerRelationVisitor();
  ~MembersPerRelationVisitor() override = default;

  void visit(const ConstElementPtr& e) override;

  QString getInitStatusMessage() const override
  { return "Calculating relation member statistics..."; }
  QString getCompletedStatusMessage() const override
  { return "Calculated member statistics for " + QString::number(_numAffected) + " relations"; }

  long numWithStat() const override { return _numAffected; }
  double getStat() const override { return _totalMembers; }
  double getMin() const override { return _minMembersPerRelation; }
  double getMax() const override { return _maxMembersPerRelation; }
  double getAverage() const override
  {
    const double average = _numAffected == 0 ? 0.0 : _totalMembers / _numAffected;
    return average;
  }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Calculates relation member statistics"; }

private:

  RelationCriterion _crit;

  int _totalMembers;
  int _minMembersPerRelation;
  int _maxMembersPerRelation;
};

}

#endif // MEMBERS_PER_RELATION_VISITOR_H
