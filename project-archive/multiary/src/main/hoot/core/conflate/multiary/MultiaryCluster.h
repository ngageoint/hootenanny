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
 * @copyright Copyright (C) 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef MULTIARYCLUSTER_H
#define MULTIARYCLUSTER_H

// hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * Represents a cluster of elements that are candidates for conflation. The cluster may contain
 * zero or more elements and all the elements in a single cluster should be conflatable together.
 *
 * For simplicity the cluster simply inherits from QList, but now that it has grown a bit it may
 * be worth refactoring this into a more formal class. Thoughts?
 */
class MultiaryCluster : public QList<ConstElementPtr>
{
public:
  /**
   * True if the cluster is still valid. This may be set to false after a cluster has been merged
   * into a parent cluster. The valid flag is here to ease book-keeping.
   */
  bool valid;

  /**
   * A list of links from this cluster to other clusters. These links refer to some kind of
   * non-miss relationship between the clusters.
   */
  QList<std::shared_ptr<MultiaryCluster>> links;

  /**
   * A single element that represents the merged result of all the cluster's members.
   */
  ElementPtr mergedElement;

  MultiaryCluster() { valid = true; }

  QString toString() const;
};

using MultiaryClusterPtr = std::shared_ptr<MultiaryCluster>;

}

#endif // MULTIARYCLUSTER_H
