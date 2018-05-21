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
  if (!allowDir && QFileInfo(input).isDir())
  {
    return false;
  }

  //input is a dir; only accepting a dir as input if it contains a shape file for now
  if (QFileInfo(input).isDir())
  {
    return FileUtils::dirContainsFileWithExtension(QFileInfo(input).dir(), "shp");
  }
  //single input
  else
  {
    if (input.toLower().endsWith(".zip"))
    {
      return true;
    }
    return OgrUtilities::getInstance().getSupportedFormats(true)
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
    const QString file = input.split(";")[0];
    if (!isSupportedOgrFormat(file, allowDir))
    {
      return false;
    }
  }
  return true;
}

}
