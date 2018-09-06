
#include "ElementExternalMergeSorter.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmFileSorter.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/ElementStreamer.h>

// Qt
#include <QFileInfo>

namespace hoot
{

ElementExternalMergeSorter::ElementExternalMergeSorter(const QString input) :
ElementSorter(ConstOsmMapPtr())
{
  _sort(input);

  LOG_DEBUG("Opening reader for " << _sortTempFile->fileName());
  boost::shared_ptr<PartialOsmMapReader> reader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::getInstance().createReader(_sortTempFile->fileName()));
  reader->setUseDataSourceIds(true);
  reader->open(input);
  _inputStream = boost::dynamic_pointer_cast<ElementInputStream>(reader);
}

void NonMemoryBoundElementSorter::close()
{
  _inputStream->close();
}

boost::shared_ptr<OGRSpatialReference> ElementExternalMergeSorter::getProjection() const
{
  return _inputStream->getProjection();
}

bool ElementExternalMergeSorter::hasMoreElements()
{
  return _inputStream->hasMoreElements();
}

ElementPtr ElementExternalMergeSorter::readNextElement()
{
  return _inputStream->readNextElement();
}

bool ElementExternalMergeSorter::isSupportedInputFormat(const QString input)
{
  return OsmFileSorter::isSupportedInputFormat(input);
}

void ElementExternalMergeSorter::_sort(QString input)
{
  LOG_INFO("Sorting " << input << " by element ID...");

  const QString sortTempFileBaseName = "element-sorter-temp-XXXXXX";
  if (OgrReader().isSupported(input))
  {
    //Unfortunately for now, sorting an OGR input is going to require an extra pass over the data
    //to first write it to a sortable format.
    LOG_WARN("OGR inputs must be converted to the OSM format before sorting by element ID.");
    LOG_WARN("Converting input to OSM format...");

    boost::shared_ptr<QTemporaryFile> pbfTemp(
      new QTemporaryFile(
        ConfigOptions().getApidbBulkInserterTempFileDir() +
        "/element-sorter-temp-XXXXXX.osm.pbf"));
    //for debugging only
    //pbfTemp->setAutoRemove(false);
    if (!pbfTemp->open())
    {
      throw HootException("Unable to open sort temp file: " + pbfTemp->fileName() + ".");
    }

    ElementStreamer::stream(input, pbfTemp->fileName());
    input = pbfTemp->fileName();
  }

  QFileInfo newInputFileInfo(input);
  _sortTempFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() + "/" + sortTempFileBaseName + "." +
      newInputFileInfo.completeSuffix()));
  //for debugging only
  //sortTempFile->setAutoRemove(false);
  if (!_sortTempFile->open())
  {
    throw HootException("Unable to open sort temp file: " + _sortTempFile->fileName() + ".");
  }

  OsmFileSorter::sort(input, _sortTempFile->fileName());

  LOG_DEBUG(input << " sorted by element ID to output: " << _sortTempFile->fileName() << ".");
}

}
