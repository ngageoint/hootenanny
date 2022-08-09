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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVERELATIONBYEID_H
#define REMOVERELATIONBYEID_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * Removes a relation from a map
 */
class RemoveRelationByEid : public OsmMapOperation
{
public:

  static QString className() { return "RemoveRelationByEid"; }

  /**
   * Constructor
   */
  RemoveRelationByEid();
  /**
   * Constructor
   *
   * @param rId ID of the relation to remove
   */
  RemoveRelationByEid(long rId);
  ~RemoveRelationByEid() override = default;

  /**
   * @see OsmMapOperation
   */
  void apply(OsmMapPtr& map) override;

  /**
   * Removes the specified relation from the map
   *
   * @param map Map to operate on
   * @param rId ID of relation to remove
   */
  static void removeRelation(OsmMapPtr map, long rId);

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Removes a single relation by element ID"; }

  void setRelationId(long rId) { _rIdToRemove = rId; }

private:

  long _rIdToRemove;
};

}

#endif // REMOVERELATIONBYEID_H
