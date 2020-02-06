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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef IOUTILS_H
#define IOUTILS_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/io/ElementVisitorInputStream.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * General utilities related to I/O
 */
class IoUtils
{

public:

  /**
   * Returns true if the input format is a Hootenanny supported OSM format
   *
   * @param input input path
   * @return true if the input is OSM and supported; false otherwise
   */
  static bool isSupportedOsmFormat(const QString& input);

  /**
   * Returns true if the input format is a Hootenanny supported OGR format
   *
   * @param input input path
   * @param allowDir if true; then a directory can be passed as an input
   * @return true if the input is supported by OGR; false otherwise
   */
  static bool isSupportedOgrFormat(const QString& input, const bool allowDir = false);

  /**
   * Determines if a set of inputs paths are all OGR supported formats
   *
   * @param inputs input paths
   * @param allowDir if true; then a directory can be passed as an input
   * @return true if all inputs passed in are supported by OGR; false otherwise
   */
  static bool areSupportedOgrFormats(const QStringList& inputs, const bool allowDir = false);

  /**
   * Determines if any in a set of inputs paths are OGR supported formats
   *
   * @param inputs input paths
   * @param allowDir if true; then a directory can be passed as an input
   * @return true if any of the inputs passed in are supported by OGR; false otherwise
   */
  static bool anyAreSupportedOgrFormats(const QStringList& inputs, const bool allowDir = false);

  /**
    Loads an OSM map into an OsmMap object

    @param map the object to load the map into
    @param path the file path to load the map from
    @param useFileId if true, uses the element ID's in the map file; otherwise, generates new
    element ID's
    @param defaultStatus the hoot status to assign to all elements
    */
  static void loadMap(const OsmMapPtr& map, const QString& path, bool useFileId,
                      Status defaultStatus = Status::Invalid);

  /**
    Loads multiple OSM maps into an OsmMap object

    @param map the object to load the map into
    @param paths the file paths to load the maps from
    @param useFileId if true, uses the element ID's in the map file; otherwise, generates new
    element ID's
    @param defaultStatus the hoot status to assign to all elements
    */
  static void loadMaps(const OsmMapPtr& map, const QStringList& paths, bool useFileId,
                       Status defaultStatus = Status::Invalid);

  /**
    Saves an OSM map to an OsmMap object

    @param map the map object to save
    @param path the file path to save the map to
    */
  static void saveMap(const OsmMapPtr& map, const QString& path);

  /**
   * Crops a map to a given bounds
   *
   * @param map the map to crop
   * @param bounds the bounds to crop to
   * @param keepConnectedOobWays if true any way falling outside of the bounds but directly
   * connected to a way within the bounds will be kept
   */
  static void cropToBounds(OsmMapPtr& map, const geos::geom::Envelope& bounds,
                           const bool keepConnectedOobWays = false);

  /**
   * Creates an input stream with a visitor in the loop
   *
   * @param input the data soruce to read
   * @param visitorClassName the class name of the visitor to use
   * @return an input stream
   */
  static std::shared_ptr<ElementVisitorInputStream> getVisitorInputStream(
    const QString& input, const QString& visitorClassName, const bool useDataSourceIds = false);
};

}

#endif // IOUTILS_H
