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

#ifndef REPLACEMENT_SNAPPED_WAY_JOINER_H
#define REPLACEMENT_SNAPPED_WAY_JOINER_H

// Hoot
#include <hoot/core/algorithms/WayJoinerAdvanced.h>

namespace hoot
{

/**
 * @brief The ReplacementSnappedWayJoiner class is a way joiner to be used in the changeset
 * replacement generation workflow to join up ways snapped with UnconnectedWaySnapper.
 */
class ReplacementSnappedWayJoiner : public WayJoinerAdvanced
{
public:

  static QString className() { return "ReplacementSnappedWayJoiner"; }

  /**
   * @brief Constructor
   */
  ReplacementSnappedWayJoiner();

  /**
   * @brief Constructor
   * @param refIdToVersionMappings a mapping of reference element IDs to their versions
   */
  ReplacementSnappedWayJoiner(const QMap<ElementId, long>& refIdToVersionMappings);
  ~ReplacementSnappedWayJoiner() = default;

  /**
   * @see WayJoiner
   */
  void join(const OsmMapPtr& map) override;

  /**
   * @see ApiEntityInfo
   */
  QString getDescription() const override
  { return "Way joiner for use after way snapping when generating replacement changesets."; }
  /**
   * @see ApiEntityInfo
   */
  QString getClassName() const override { return className(); }
  /**
   * @see ApiEntityInfo
   */
  QString getName() const override { return className(); }

protected:

  /*
   * @see WayJoinerAdvanced
   */
  bool _hasPid(const ConstWayPtr& way) const override;
  /*
   * @see WayJoinerAdvanced
   */
  long _getPid(const ConstWayPtr& way) const override;

private:

  // keeps track of element changeset versions
  QMap<ElementId, long> _refIdToVersionMappings;
};

}

#endif  //  REPLACEMENT_SNAPPED_WAY_JOINER_H
