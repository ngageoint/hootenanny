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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#ifndef PARENT_MEMBERSHIP_REMOVER_H
#define PARENT_MEMBERSHIP_REMOVER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Removes parent element references for an element
 */
class ParentMembershipRemover
{

public:

  ParentMembershipRemover() = default;
  ~ParentMembershipRemover() = default;

  /**
   * Removes all parent memberships in either relations or ways for the specified element
   *
   * @param elementId ID of the element to remove memberships for
   * @return the number of memberships removed
   */
  static int removeMemberships(const ElementId& elementId, const ConstOsmMapPtr& map);
};

}

#endif // PARENT_MEMBERSHIP_REMOVER_H
