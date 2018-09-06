
#include "ElementExternalMergeSorter.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

// Qt
#include <QFileInfo>

// std
#include <queue>

namespace hoot
{

ElementExternalMergeSorter::ElementExternalMergeSorter(ElementInputStreamPtr input,
                                                       const QString inputFileExtension) :
ElementSorter(ConstOsmMapPtr()),
_inputFileExtension(inputFileExtension),
//TODO: put vals in config
_maxElementsPerFile(1000)
{
  _sort(input);

  LOG_DEBUG("Opening reader for " << _sortTempFile->fileName());
  boost::shared_ptr<PartialOsmMapReader> reader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::getInstance().createReader(_sortTempFile->fileName()));
  reader->setUseDataSourceIds(true);
  reader->open(_sortTempFile->fileName());
  _sortedElements = boost::dynamic_pointer_cast<ElementInputStream>(reader);
}

void ElementExternalMergeSorter::close()
{
  _sortedElements->close();
}

boost::shared_ptr<OGRSpatialReference> ElementExternalMergeSorter::getProjection() const
{
  return _sortedElements->getProjection();
}

bool ElementExternalMergeSorter::hasMoreElements()
{
  return _sortedElements->hasMoreElements();
}

ElementPtr ElementExternalMergeSorter::readNextElement()
{
  return _sortedElements->readNextElement();
}

bool ElementExternalMergeSorter::isSupportedInputFormat(const QString /*input*/)
{
  //return OsmFileSorter::isSupportedInputFormat(input);
  //??
  return true;
}

void ElementExternalMergeSorter::_sort(ElementInputStreamPtr input)
{
  LOG_INFO("Sorting input by element ID...");

  _mergeFiles(_createSortedFileOutputs(input));

  LOG_DEBUG("input sorted by element ID to output: " << _sortTempFile->fileName() << ".");
}

bool elementCompare(const ConstElementPtr& e1, const ConstElementPtr& e2)
{
  if (e1->getElementType().getEnum() < e2->getElementType().getEnum())
  {
    return true;
  }
  else if (e1->getElementType().getEnum() < e2->getElementType().getEnum())
  {
    return false;
  }
  return e1->getId() < e2->getId();
}

QList<boost::shared_ptr<QTemporaryFile>> ElementExternalMergeSorter::_createSortedFileOutputs(
  ElementInputStreamPtr input)
{
  long elementCtr = 0;
  QList<boost::shared_ptr<QTemporaryFile>> tempOutputFiles;
  boost::shared_ptr<QTemporaryFile> tempOutputFile;
  boost::shared_ptr<PartialOsmMapWriter> writer;
  std::vector<ConstElementPtr> elements;
  elements.reserve(_maxElementsPerFile);

  while (input->hasMoreElements())
  {
    elements.push_back(input->readNextElement());
    elementCtr++;

    if ((elementCtr % _maxElementsPerFile == 0 && elementCtr != 0) || !input->hasMoreElements())
    {
      const QString sortTempFileBaseName = "element-sorter-temp-XXXXXX";
      tempOutputFile.reset(
        new QTemporaryFile(
          ConfigOptions().getApidbBulkInserterTempFileDir() + "/" + sortTempFileBaseName + "." +
          _inputFileExtension));
      //for debugging only
      //tempOutputFile->setAutoRemove(false);
      if (!tempOutputFile->open())
      {
        throw HootException("Unable to open sort temp file: " + tempOutputFile->fileName() + ".");
      }
      tempOutputFiles.append(tempOutputFile);

      writer =
        boost::dynamic_pointer_cast<PartialOsmMapWriter>(
          OsmMapWriterFactory::getInstance().createWriter(tempOutputFile->fileName()));
      writer->open(tempOutputFile->fileName());

      std::sort(elements.begin(), elements.end(), elementCompare);

      for (std::vector<ConstElementPtr>::const_iterator itr = elements.begin();
           itr != elements.end(); ++itr)
      {
        ConstElementPtr element = *itr;
        writer->writePartial(element);
      }
      elements.clear();

      if (writer)
      {
        writer->close();
      }
      if (tempOutputFile)
      {
        tempOutputFile->close();
      }
    }
  }
  input->close();

  return tempOutputFiles;
}

void ElementExternalMergeSorter::_mergeFiles(
                                     QList<boost::shared_ptr<QTemporaryFile>> tempOutputFiles)
{
  const QString sortTempFileBaseName = "element-sorter-temp-XXXXXX";
  _sortTempFile.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() + "/" + sortTempFileBaseName + "." +
      _inputFileExtension));
  //for debugging only
  //tempOutputFile->setAutoRemove(false);
  if (!_sortTempFile->open())
  {
    throw HootException("Unable to open sort temp file: " + _sortTempFile->fileName() + ".");
  }
  boost::shared_ptr<PartialOsmMapWriter> writer =
    boost::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::getInstance().createWriter(_sortTempFile->fileName()));
  writer->open(_sortTempFile->fileName());

  ConstElementPtr elementArr[tempOutputFiles.size()];
  std::priority_queue<ConstElementPtr, std::vector<ConstElementPtr>, ElementCompare> priorityQueue;

  QList<boost::shared_ptr<PartialOsmMapReader>> readers;
  for (int i = 0; i < tempOutputFiles.size(); i++)
  {
    const QString fileName = tempOutputFiles.at(i)->fileName();
    boost::shared_ptr<PartialOsmMapReader> reader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(fileName));
    reader->setUseDataSourceIds(true);
    reader->open(fileName);
    readers.append(reader);

    elementArr[i] = reader->readNextElement();
    priorityQueue.push(elementArr[i]);
  }

  int i = 0;
  while (i != readers.size())
  {
    ConstElementPtr root = priorityQueue.top();
    priorityQueue.pop();
    writer->writePartial(root);
    if (readers.at(i)->hasMoreElements())
    {
      root = readers.at(i)->readNextElement();
    }
    else
    {
      root.reset(new Relation(Status::Unknown1, LONG_MAX, 15.0)); //??
      i++;
    }

    priorityQueue.push(root);
  }

  writer->close();
}

}
