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
 * @copyright Copyright (C) 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CONFLATE_UTILS_H
#define CONFLATE_UTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
//#include <hoot/core/criterion/ElementCriterion.h>
//#include <hoot/core/criterion/InBoundsCriterion.h>

namespace hoot
{

/**
 * General utilities to be used with conflation
 */
class ConflateUtils
{

public:

  /**
   * Writes any data that cannot be conflated
   *
   * @param map the map to examine
   * @param output the URL to write the data to
   * @return the number of features written
   */
  static int writeNonConflatable(const ConstOsmMapPtr& map, const QString& output);

  /**
   * Writes the differential between to maps
   *
   * @param mapUrl1 map 1 URL
   * @param mapUrl2 map 2 URL
   * @param bounds the area over which to calculate the differential
   * @param output the URL to write the data to
   */
  static void writeDiff(const QString& mapUrl1, const QString& mapUrl2,
                        const geos::geom::Envelope& bounds, const QString& output);

//  /**
//   * Checks to see if an element can be conflated by any of the configured matchers for conflation.
//   *
//   * @param element element to examine
//   * @param map map containing the element
//   * @return true if the conflate matchers are configured with at least one matcher that
//   * can conflate the input element; false otherwise
//   */
//  static bool elementCanBeConflatedByActiveMatcher(
//    const ConstElementPtr& element, const ConstOsmMapPtr& map);

//  /**
//   * TODO
//   *
//   * @param element
//   * @param map
//   * @return
//   */
//  static bool elementFailsConfiguredBoundsCheck(
//    const ConstElementPtr& element, const ConstOsmMapPtr& map);

//private:

  //static QMap<QString, ElementCriterionPtr> _critCache;
  //static std::shared_ptr<InBoundsCriterion> _cachedBoundsCrit;
};

}

#endif // CONFLATE_UTILS_H
