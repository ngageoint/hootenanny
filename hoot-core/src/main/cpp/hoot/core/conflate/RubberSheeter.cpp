
#include "RubberSheeter.h"

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

namespace hoot
{

RubberSheeter::RubberSheeter()
{
}

void RubberSheeter::rubberSheet(const QString input1, const QString input2, const QString output)
{
  OsmMapPtr map(new OsmMap());
  IoUtils::loadMap(map, input1, false, Status::Unknown1);
  IoUtils::loadMap(map, input2, false, Status::Unknown2);

  QStringList l = ConfigOptions().getMapCleanerTransforms();
  l.removeAll(QString::fromStdString(RubberSheet::className()));
  conf().set(MapCleaner::opsKey(), l);
  MapCleaner().apply(map);
  RubberSheet().apply(map);

  MapProjector::projectToWgs84(map);

  IoUtils::saveMap(map, output);
}

}
