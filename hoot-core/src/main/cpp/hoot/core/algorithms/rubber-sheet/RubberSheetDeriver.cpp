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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "RubberSheetDeriver.h"

// Hoot
#include <hoot/core/algorithms/rubber-sheet/RubberSheet.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

void RubberSheetDeriver::derive(const QString& input1, const QString& input2, const QString& transform2To1,
                                const QString& transform1To2, const bool ref) const
{
  LOG_STATUS(
    "Deriving alignment transform for inputs ..." << FileUtils::toLogFormat(input1, 25) <<
    " and " << FileUtils::toLogFormat(input2, 25) << "...");

  OsmMapPtr map = std::make_shared<OsmMap>();
  IoUtils::loadMap(map, input1, false, Status::Unknown1);
  IoUtils::loadMap(map, input2, false, Status::Unknown2);

  // make sure rubber sheeting isn't applied during cleaning.
  QStringList l = ConfigOptions().getMapCleanerTransforms();
  l.removeAll(RubberSheet::className());
  conf().set(MapCleaner::opsKey(), l);
  MapCleaner().apply(map);

  RubberSheet rs;
  rs.setReference(ref);
  rs.calculateTransform(map);

  QFile fp2to1(transform2To1);
  if (fp2to1.open(QIODevice::WriteOnly) == false)
    throw HootException(QString("Error opening %1 for writing").arg(transform2To1));
  rs.writeTransform2to1(fp2to1);
  // if --ref is specified then this will just write an identity transform
  if (transform1To2 != "")
  {
    QFile fp1to2(transform1To2);
    if (fp1to2.open(QIODevice::WriteOnly) == false)
      throw HootException(QString("Error opening %1 for writing").arg(transform1To2));
    rs.writeTransform1to2(fp1to2);
  }
}

}
