
#ifndef RELATION_WITH_MOST_MEMBERS_OP_H
#define RELATION_WITH_MOST_MEMBERS_OP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * Finds a single relation satisfying a specified criterion that has the most relation members
 * satisfying some other criterion. This is useful when you have a large map file from which you
 * want to find a single large relation meeting some specified criteria.
 */
class RelationWithMostMembersOp : public OsmMapOperation, public Configurable,
  public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::RelationWithMostMembersOp"; }

  RelationWithMostMembersOp();

  virtual void apply(boost::shared_ptr<OsmMap> &map) override;

  virtual QString getDescription() const override
  { return "Finds relation with most members satisfying a criterion"; }

  void setCriterion(const QString criterionClass, const QString type, const Settings& conf);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getInitStatusMessage() const
  { return "Collecting relation info by criterion..."; }

  virtual QString getCompletedStatusMessage() const;

private:

  // determines which relations are selected
  ElementCriterionPtr _relationCriterion;
  // determines which relation members are counted
  ElementCriterionPtr _memberCriterion;

  int _maxNumCritSatisifed;
  int _totalRelations;
  int _totalRelationMembers;
  int _numRelationsSatisfyingCriterion;
  int _numRelationMembersSatisfyingCriterion;

  long getIdOfRelationWithMaxCritSatisfactions(const ConstOsmMapPtr& map, RelationPtr& outRelation);
  void _setOutput(const long relationId, OsmMapPtr& outputMap);
};

}

#endif // RELATION_WITH_MOST_MEMBERS_OP_H
