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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef __REMOVE_EMPTY_RELATIONS_OP_H__
#define __REMOVE_EMPTY_RELATIONS_OP_H__

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

/**
 * Removes all relations that have no members.  This needs to be an op, rather than a visitor, so
 * that we can delete relations that are children of other relations.
 */
class RemoveEmptyRelationsOp : public OsmMapOperation, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::RemoveEmptyRelationsOp"; }

  RemoveEmptyRelationsOp();

  virtual void apply(OsmMapPtr& map);

  virtual std::string getClassName() const { return className(); }

  long getNumRemoved() const { return _numAffected; }

  virtual QString getInitStatusMessage() const { return "Removing empty relations..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " empty relations"; }

  virtual QString getDescription() const { return "Removes relations with no members"; }

private:

  void _deleteEmptyRelations(OsmMapPtr& map, const bool reverseOrder);
};

}

#endif // __REMOVE_EMPTY_RELATIONS_OP_H__
