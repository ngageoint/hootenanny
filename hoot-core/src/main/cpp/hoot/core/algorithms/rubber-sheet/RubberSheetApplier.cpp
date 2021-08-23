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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "RubberSheetApplier.h"

// Hoot
#include <hoot/core/algorithms/rubber-sheet/RubberSheet.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>


#include <hoot/core/util/Settings.h>

// Qt


namespace hoot
{

void RubberSheetApplier::apply(
  const QString& transform, const QString& input, const QString& output) const
{
  LOG_STATUS(
    "Applying alignment transform from ..." << FileUtils::toLogFormat(input, 25) << " to " <<
    FileUtils::toLogFormat(output, 25) << "...");

  OsmMapPtr map = std::make_shared<OsmMap>();
  IoUtils::loadMap(map, input, true, Status::Unknown1);

  // make sure rubber sheeting isn't applied during cleaning.
  QStringList l = ConfigOptions().getMapCleanerTransforms();
  l.removeAll(RubberSheet::className());
  conf().set(MapCleaner::opsKey(), l);
  MapCleaner().apply(map);

  RubberSheet rs;
  rs.setReference(false);

  QFile fp(transform);
  if (fp.open(QIODevice::ReadOnly) == false)
  {
    throw HootException(QString("Error opening %1 for writing").arg(transform));
  }
  rs.readTransform1to2(fp);

  rs.applyTransform(map);

  MapProjector::projectToWgs84(map);

  IoUtils::saveMap(map, output);
}

}
