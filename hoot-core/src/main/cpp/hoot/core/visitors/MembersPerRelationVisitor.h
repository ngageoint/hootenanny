
#ifndef MEMBERS_PER_RELATION_VISITOR_H
#define MEMBERS_PER_RELATION_VISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/NumericStatistic.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/criterion/RelationCriterion.h>

namespace hoot
{

class MembersPerRelationVisitor : public ConstElementVisitor, public NumericStatistic,
  public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::MembersPerRelationVisitor"; }

  MembersPerRelationVisitor();

  virtual void visit(const ConstElementPtr& e) override;

  virtual QString getDescription() const { return "Calculates relation member statistics"; }

  virtual QString getInitStatusMessage() const
  { return "Calculating relation member statistics..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Calculated member statistics for " + QString::number(_numAffected) + " relations"; }

  virtual double getStat() const { return _totalMembers; }
  virtual double getMin() const { return _minMembersPerRelation; }
  virtual double getMax() const { return _maxMembersPerRelation; }
  virtual double getAverage() const { return _totalMembers / _numRelations; }

private:

  RelationCriterion _crit;

  int _totalMembers;
  int _numRelations;
  int _minMembersPerRelation;
  int _maxMembersPerRelation;
};

}

#endif // MEMBERS_PER_RELATION_VISITOR_H
