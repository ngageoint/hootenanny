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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RubberSheeter.h"

// Hoot
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheet.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

RubberSheeter::RubberSheeter()
{
}

void RubberSheeter::rubberSheet(const QString& input1, const QString& input2, const QString& output)
{
  OsmMapPtr map(new OsmMap());
  IoUtils::loadMap(map, input1, false, Status::Unknown1);
  IoUtils::loadMap(map, input2, false, Status::Unknown2);

  QStringList l = ConfigOptions().getMapCleanerTransforms();
  l.removeAll(RubberSheet::className());
  conf().set(MapCleaner::opsKey(), l);
  MapCleaner().apply(map);
  RubberSheet().apply(map);

  MapProjector::projectToWgs84(map);

  IoUtils::saveMap(map, output);
}

}
