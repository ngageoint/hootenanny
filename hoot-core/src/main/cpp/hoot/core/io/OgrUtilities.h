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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OGRUTILITIES_H
#define OGRUTILITIES_H

// GDAL
class OGRDataSource;

// Qt
#include <QString>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

class OgrUtilities
{
public:
  OgrUtilities();

  shared_ptr<OGRDataSource> createDataSource(QString url);

  static OgrUtilities& getInstance();

  /**
   * Returns true if this is likely a data source OGR can open. This will just do a quick check
   * and doesn't verify that the source exists or is a proper format.
   */
  bool isReasonableUrl(QString url);

  shared_ptr<OGRDataSource> openDataSource(QString url);

private:
  static shared_ptr<OgrUtilities> _theInstance;
};

}

#endif // OGRUTILITIES_H
