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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef OSMMAPWRITERFACTORY_H
#define OSMMAPWRITERFACTORY_H

// Hoot
#include <hoot/core/conflate/network/NetworkMatcher.h>
#include <hoot/core/io/ElementInputStream.h>

// Qt
#include <QString>

// Standard
#include <memory>

// GEOS
#include <geos/geom/Geometry.h>

namespace hoot
{

class OsmMapWriter;
class OsmMap;

/**
 * A factory for constructing writers based on the URL
 */
class OsmMapWriterFactory
{
public:

  static std::shared_ptr<OsmMapWriter> createWriter(const QString& url);

  static bool hasElementOutputStream(const QString& url);

  static void write(const std::shared_ptr<OsmMap>& map, const QString& url,
                    const bool silent = false, const bool is_debug = false);

  static QString getWriterName(const QString& url);

  /**
   * Writes a map useful for debugging
   *
   * @param map the map to write
   * @param title a descriptive title
   * @param matcher If the Network conflation algorithm was used to generate the input data, pass
   * in the matcher used to write additional debugging information.
   */
  static void writeDebugMap(const std::shared_ptr<const OsmMap>& map, const QString& title = "",
                            NetworkMatcherPtr matcher = NetworkMatcherPtr());

  /**
   * Writes a feature geometry useful for debugging
   *
   * @param geometry the geometry to write
   * @param title a descriptive title
   * @param matcher If the Network conflation algorithm was used to generate the input data, pass
   * in the matcher used to write additional debugging information.
   */
  static void writeDebugMap(const std::shared_ptr<geos::geom::Geometry>& geometry,
                            std::shared_ptr<OGRSpatialReference> spatRef, const QString& title = "",
                            NetworkMatcherPtr matcher = NetworkMatcherPtr());

  /**
   * Reset the debug map count for output file naming purposes; useful in unit tests that
   */
  static void resetDebugMapCount() { _debugMapCount = 1; }

private:

  static unsigned int _debugMapCount;
};

}

#endif // OSMMAPWRITERFACTORY_H
