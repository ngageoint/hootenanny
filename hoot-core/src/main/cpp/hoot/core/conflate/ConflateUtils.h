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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef CONFLATE_UTILS_H
#define CONFLATE_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * @brief The ConflateUtils class contains general utilities to be used with conflation.
 */
class ConflateUtils
{

public:

  /**
   * @brief writeNonConflatable writes any data that cannot be conflated.
   * @param map the map to examine
   * @param output the URL to write the data to
   * @param ignoreGenericConflators if enabled, elements conflatable only by generic conflators
   * (point, line, poly, etc.) are always considered not conflatable
   * @return the number of features written
   */
  static int writeNonConflatable(
    const ConstOsmMapPtr& map, const QString& output, const bool ignoreGenericConflators = false);

  /**
   * @brief writeDiff writes the differential between two maps.
   * @param mapUrl1 map 1 URL
   * @param mapUrl2 map 2 URL
   * @param bounds the area over which to calculate the differential
   * @param output the URL to write the data to
   */
  static void writeDiff(
    const QString& mapUrl1, const QString& mapUrl2, const geos::geom::Envelope& bounds,
    const QString& output);

  /**
   * @brief isNetworkConflate determines if the Network road conflate algorithm is activated.
   * @return true if Network road conflation is enabled; false otherwise
   */
  static bool isNetworkConflate();
};

}

#endif // CONFLATE_UTILS_H
