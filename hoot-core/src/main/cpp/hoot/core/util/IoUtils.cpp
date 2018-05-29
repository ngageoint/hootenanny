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

#include "IoUtils.h"

// Hoot
#include <hoot/core/io/OgrUtilities.h>
#include "FileUtils.h"
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/util/Progress.h>

// Qt
#include <QFileInfo>

namespace hoot
{

bool IoUtils::isSupportedOsmFormat(const QString input)
{
  const QString inputLower = input.toLower();
  return inputLower.endsWith(".osm") || inputLower.endsWith(".osm.pbf") ||
         inputLower.startsWith("hootapidb://") || inputLower.startsWith("osmapidb://");
}

bool IoUtils::isSupportedOgrFormat(const QString input, const bool allowDir)
{
  LOG_VART(input);
  LOG_VART(allowDir);

  if (!allowDir && QFileInfo(input).isDir())
  {
    return false;
  }

  LOG_VART(QFileInfo(input).isDir());
  //input is a dir; only accepting a dir as input if it contains a shape file or is a file geodb
  if (QFileInfo(input).isDir())
  {
    return input.toLower().endsWith(".gdb") ||
           FileUtils::dirContainsFileWithExtension(QFileInfo(input).dir(), "shp");
  }
  //single input
  else
  {
    //The only zip file format we support are ones containing OGR inputs.
    if (input.toLower().endsWith(".zip") ||
        //We only support this type of postgres URL for OGR inputs.
        input.toLower().startsWith("pg:"))
    {
      return true;
    }
    LOG_VART(OgrUtilities::getInstance().getSupportedFormats(false));
    LOG_VART(QFileInfo(input).suffix());
    return OgrUtilities::getInstance().getSupportedFormats(false)
             .contains("." + QFileInfo(input).suffix());
  }
}

bool IoUtils::areSupportedOgrFormats(const QStringList inputs, const bool allowDir)
{
  if (inputs.size() == 0)
  {
    return false;
  }

  for (int i = 0; i < inputs.size(); i++)
  {
    const QString input = inputs.at(i);
    LOG_VART(input);
    const QString file = input.split(";")[0];
    LOG_VART(file);
    if (!isSupportedOgrFormat(file, allowDir))
    {
      return false;
    }
  }
  return true;
}

void IoUtils::loadMap(OsmMapPtr map, QString path, bool useFileId, Status defaultStatus)
{
  QStringList pathLayer = path.split(";");
  QString justPath = pathLayer[0];
  if (OgrReader::isReasonablePath(justPath))
  {
    OgrReader reader;
    Progress progress("OsmUtils");
    reader.setDefaultStatus(defaultStatus);
    reader.read(justPath, pathLayer.size() > 1 ? pathLayer[1] : "", map, progress);
  }
  else
  {
    OsmMapReaderFactory::read(map, path, useFileId, defaultStatus);
  }
}

void IoUtils::saveMap(boost::shared_ptr<const OsmMap> map, QString path)
{
  OsmMapWriterFactory::write(map, path);
}

}
