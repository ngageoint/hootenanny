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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
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
 * TODO
 */
class RelationWithMostMembersOp : public OsmMapOperation, public Configurable,
  public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::RelationWithMostMembersOp"; }

  RelationWithMostMembersOp();

  virtual void apply(boost::shared_ptr<OsmMap> &map) override;

  virtual QString getDescription() const override
  { return "TODO"; }

  void setCriterion(const QString criterionClass, const QString type, const Settings& conf);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getInitStatusMessage() const
  { return "Collecting relation info by criterion..."; }

  virtual QString getCompletedStatusMessage() const;

private:

  ElementCriterionPtr _relationCriterion;
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
