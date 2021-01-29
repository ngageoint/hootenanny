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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVERELATIONBYEID_H
#define REMOVERELATIONBYEID_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * @brief The RemoveRelationEid class removes a relation from an OsmMap
 */
class RemoveRelationByEid : public OsmMapOperation
{
public:

  /**
   * @brief className gets the class name as a string
   * @return class name
   */
  static QString className() { return "hoot::RemoveRelationByEid"; }
  virtual QString getName() const { return className(); }
  virtual QString getClassName() const override { return className(); }

  /**
   * @brief RemoveRelationByEid Default constructor
   */
  RemoveRelationByEid();
  virtual ~RemoveRelationByEid() = default;

  /**
   * @brief RemoveRelationByEid
   * @param rId ID of relation to remove
   */
  RemoveRelationByEid(long rId);

  /**
   * @brief apply Peform the remove operation
   * @param map Map to operate on
   */
  void apply(OsmMapPtr& map);

  /**
   * @brief setRelationId Set the ID of the relation to remove
   * @param rId ID of relation to remove
   */
  void setRelationId(long rId) { _rIdToRemove = rId; }

  /**
   * @brief removeRelation Remove the specified relation from the given map
   * @param map Map to operate on
   * @param rId ID of relation to remove
   */
  static void removeRelation(OsmMapPtr map, long rId);

  virtual QString getDescription() const { return "Removes a single relation by element ID"; }

private:

  long _rIdToRemove;
};

}

#endif // REMOVERELATIONBYEID_H
