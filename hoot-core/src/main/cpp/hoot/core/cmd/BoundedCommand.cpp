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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "BoundedCommand.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/geometry/GeometryUtils.h>

namespace hoot
{

int BoundedCommand::runSimple(QStringList& args)
{
  if (args.contains("--write-bounds"))
  {
    args.removeAll("--write-bounds");
    _writeBoundsFile();
  }

  return 0;
}

void BoundedCommand::_writeBoundsFile()
{
  ConfigOptions opts;
  // TODO: We aren't dealing with the osm/hoot api db reader version of bounding box, which could
  // eventually cause a problem.
  QString boundsStr = opts.getConvertBounds().trimmed();
  if (!boundsStr.isEmpty())
  {
    OsmMapWriterFactory::write(
      GeometryUtils::createMapFromBounds(GeometryUtils::boundsFromConfigString(boundsStr)),
      opts.getBoundsOutputFile());
  }
}

}
