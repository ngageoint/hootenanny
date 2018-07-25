
#include "RubberSheetApplier.h"

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/conflate/RubberSheet.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/util/Settings.h>

// Qt
#include <QFile>

namespace hoot
{

RubberSheetApplier::RubberSheetApplier()
{
}

void RubberSheetApplier::apply(const QString transform, const QString input, const QString output)
{
  OsmMapPtr map(new OsmMap());
  IoUtils::loadMap(map, input, true, Status::Unknown1);

  // make sure rubber sheeting isn't applied during cleaning.
  QStringList l = ConfigOptions().getMapCleanerTransforms();
  l.removeAll(QString::fromStdString(RubberSheet::className()));
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
