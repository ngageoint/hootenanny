
#include "RubberSheetDeriver.h"

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

RubberSheetDeriver::RubberSheetDeriver()
{
}

void RubberSheetDeriver::derive(const QString input1, const QString input2,
                                const QString transform2To1, const QString transform1To2,
                                const bool ref)
{
  OsmMapPtr map(new OsmMap());
  IoUtils::loadMap(map, input1, false, Status::Unknown1);
  IoUtils::loadMap(map, input2, false, Status::Unknown2);

  // make sure rubber sheeting isn't applied during cleaning.
  QStringList l = ConfigOptions().getMapCleanerTransforms();
  l.removeAll(QString::fromStdString(RubberSheet::className()));
  conf().set(MapCleaner::opsKey(), l);
  MapCleaner().apply(map);

  RubberSheet rs;
  rs.setReference(ref);
  rs.calculateTransform(map);

  QFile fp2to1(transform2To1);
  if (fp2to1.open(QIODevice::WriteOnly) == false)
  {
    throw HootException(QString("Error opening %1 for writing").arg(transform2To1));
  }
  rs.writeTransform2to1(fp2to1);
  // if --ref is specified then this will just write an identity transform
  if (transform1To2 != "")
  {
    QFile fp1to2(transform1To2);
    if (fp1to2.open(QIODevice::WriteOnly) == false)
    {
      throw HootException(QString("Error opening %1 for writing").arg(transform1To2));
    }
    rs.writeTransform1to2(fp1to2);
  }
}

}
