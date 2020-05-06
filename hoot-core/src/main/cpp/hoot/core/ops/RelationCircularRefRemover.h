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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef RELATION_CIRCULAR_REF_REMOVER_H
#define RELATION_CIRCULAR_REF_REMOVER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

// Qt
#include <QMultiMap>

namespace hoot
{

/**
 * Removes relations members that have cross references between relations as well as relation
 * members that reference themselves
 */
class RelationCircularRefRemover : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::RelationCircularRefRemover"; }

  RelationCircularRefRemover();

  void apply(OsmMapPtr& map);

  virtual QString getInitStatusMessage() const
  { return "Removing relation members with circular references..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) +
           " relation members involved in circular references"; }

  virtual QString getDescription() const
  { return "Removes half of a relation pair that reference each other from a map"; }

  virtual std::string getClassName() const { return className(); }

private:

  QMultiMap<long, long> _relationIdsToRelationMemberIds; 
};

}

#endif // RELATION_CIRCULAR_REF_REMOVER_H
