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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef CHANGESET_REPLACEMENT_ELEMENT_ID_SYNCHRONIZER_H
#define CHANGESET_REPLACEMENT_ELEMENT_ID_SYNCHRONIZER_H

// Hoot
#include <hoot/core/elements/ElementIdSynchronizer.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

namespace hoot
{

/**
 * This performs some additional element ID synchronization specific to cut and replace.
 */
class ChangesetReplacementElementIdSynchronizer : public ElementIdSynchronizer
{

public:

  ChangesetReplacementElementIdSynchronizer() = default;
  virtual ~ChangesetReplacementElementIdSynchronizer() = default;

  /**
   * see ElementIdSynchronizer
   */
  virtual void synchronize(const OsmMapPtr& map1, const OsmMapPtr& map2,
                           const ElementType& elementType = ElementType::Unknown) override;

private:

  void _syncElementIds(const QSet<QString>& identicalHashes);
};

}

#endif // CHANGESET_REPLACEMENT_ELEMENT_ID_SYNCHRONIZER_H
