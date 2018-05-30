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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef IOUTILS_H
#define IOUTILS_H

// Hoot
#include <hoot/core/OsmMap.h>

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
  static bool isSupportedOsmFormat(const QString input);

  /**
   * Returns true if the input format is a Hootenanny supported OGR format
   *
   * @param input input path
   * @param allowDir if true; then a directory can be passed as an input
   * @return true if all inputs passed in are OGR and supported; false otherwise
   */
  static bool isSupportedOgrFormat(const QString input, const bool allowDir = false);

  /**
   *
   *
   * @param inputs
   * @param allowDir
   * @return
   */
  static bool areSupportedOgrFormats(const QStringList inputs, const bool allowDir = false);

  /**
    Loads an OSM map into an OsmMap object

    @param map the object to load the map into
    @param path the file path to load the map from
    @param useFileId if true, uses the element ID's in the map file; otherwise, generates new
    element ID's
    @param defaultStatus the hoot status to assign to all elements
    @todo This was copied from BaseCommand.  Point all references to BaseCommand::loadMap to this
    instead.
    */
  static void loadMap(boost::shared_ptr<OsmMap> map, QString path, bool useFileId,
                      Status defaultStatus = Status::Invalid);

  /**
    Saves an OSM map to an OsmMap object

    @param map the map object to save
    @param path the file path to save the map to
    @todo This was copied from BaseCommand.  Point all references to BaseCommand::saveMap to this
    instead.
    */
  static void saveMap(boost::shared_ptr<const OsmMap> map, QString path);
};

}

#endif // IOUTILS_H
