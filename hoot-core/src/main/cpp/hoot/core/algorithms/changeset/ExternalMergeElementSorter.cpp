
#include "ExternalMergeElementSorter.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

const QString ExternalMergeElementSorter::SORT_TEMP_FILE_BASE_NAME = "element-sorter-temp-XXXXXX";

ExternalMergeElementSorter::ExternalMergeElementSorter() :
_tempFormat(ConfigOptions().getElementSorterExternalTempFormat()),
_maxElementsPerFile(ConfigOptions().getElementSorterElementBufferSize()),
_retainTempFiles(false)
{
  assert(_maxElementsPerFile != -1);
}

ExternalMergeElementSorter::~ExternalMergeElementSorter()
{
  close();
}

void ExternalMergeElementSorter::close()
{
  if (_sortedElementsReader)
  {
    _sortedElementsReader->finalizePartial();
    _sortedElementsReader->close();
  }
  if (_sortedElements)
  {
    _sortedElements->close();
  }
}

boost::shared_ptr<OGRSpatialReference> ExternalMergeElementSorter::getProjection() const
{
  return _sortedElements->getProjection();
}

bool ExternalMergeElementSorter::hasMoreElements()
{
  if (!_sortedElements)
  {
    return false;
  }
  return _sortedElements->hasMoreElements();
}

ElementPtr ExternalMergeElementSorter::readNextElement()
{
  return _sortedElements->readNextElement();
}

void ExternalMergeElementSorter::sort(ElementInputStreamPtr input)
{
  LOG_VART(_tempFormat);
  LOG_VART(_maxElementsPerFile);

  _sort(input);
  _initElementStream();
}

void ExternalMergeElementSorter::_sort(ElementInputStreamPtr input)
{
  LOG_INFO("Sorting input by element ID and type...");

  //if only one file was written, skip merging
  _createSortedFileOutputs(input);
  if (_tempOutputFiles.size() > 1)
  {
    _mergeSortedFiles();
  }
  else
  {
    _sortFinalOutput = _tempOutputFiles.at(0);
  }
}

void ExternalMergeElementSorter::_initElementStream()
{
  LOG_DEBUG("Opening reader for element stream at " << _sortFinalOutput->fileName() << "...");

  _sortedElementsReader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::getInstance().createReader(_sortFinalOutput->fileName()));
  _sortedElementsReader->setUseDataSourceIds(true);
  _sortedElementsReader->open(_sortFinalOutput->fileName());
  _sortedElements = boost::dynamic_pointer_cast<ElementInputStream>(_sortedElementsReader);
}

bool ExternalMergeElementSorter::_elementCompare(const ConstElementPtr& e1,
                                                 const ConstElementPtr& e2)
{
  const ElementType::Type type1 = e1->getElementType().getEnum();
  const ElementType::Type type2 = e2->getElementType().getEnum();
  if (type1 == type2)
  {
    return e1->getId() < e2->getId();
  }
  else
  {
    return type1 < type2;
  }
}

void ExternalMergeElementSorter::_createSortedFileOutputs(ElementInputStreamPtr input)
{
  LOG_DEBUG("Writing sorted file outputs...");

  long elementCtr = 0;
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
      LOG_DEBUG("Sorting elements...");
      std::sort(elements.begin(), elements.end(), _elementCompare);

      LOG_DEBUG("Writing elements to temp file...");

      tempOutputFile.reset(
        new QTemporaryFile(
          ConfigOptions().getApidbBulkInserterTempFileDir() + "/" + SORT_TEMP_FILE_BASE_NAME +
          "." + _tempFormat));
      if (_retainTempFiles)
      {
        //for debugging only
        tempOutputFile->setAutoRemove(false);
      }
      if (!tempOutputFile->open())
      {
        throw HootException("Unable to open sort temp file: " + tempOutputFile->fileName() + ".");
      }
      else
      {
        LOG_DEBUG("Opened temp file: " << tempOutputFile->fileName() << " for sorted output.");
      }
      _tempOutputFiles.append(tempOutputFile);

      writer =
        boost::dynamic_pointer_cast<PartialOsmMapWriter>(
          OsmMapWriterFactory::getInstance().createWriter(tempOutputFile->fileName()));
      writer->open(tempOutputFile->fileName());
      for (std::vector<ConstElementPtr>::const_iterator itr = elements.begin();
           itr != elements.end(); ++itr)
      {
        ConstElementPtr element = *itr;
        LOG_VART(element);
        writer->writePartial(element);
      }
      elements.clear();

      if (writer)
      {
        writer->finalizePartial();
        writer->close();
      }
      if (tempOutputFile)
      {
        tempOutputFile->close();
      }
    }
  }
  input->close();
  LOG_DEBUG("Finished writing sorted file outputs.");
  LOG_VART(elementCtr);
  LOG_VART(_tempOutputFiles.size());
}

void ExternalMergeElementSorter::_mergeSortedFiles()
{
  LOG_DEBUG("Merging " << _tempOutputFiles.size() << " temporary files...");

  QList<boost::shared_ptr<PartialOsmMapReader>> readers;
  ElementPriorityQueue priorityQueue = _getInitializedPriorityQueue(readers);
  boost::shared_ptr<PartialOsmMapWriter> writer = _getFinalOutputWriter();

  _mergeSortedElements(priorityQueue, writer, readers);

  writer->finalizePartial();
  writer->close();

  if (_retainTempFiles)
  {
    //for debugging only; this will only be useful if you temporarily turn off auto removal of the
    //temp file
    LOG_DEBUG("Sorted temp files: ");
    for (int i = 0; i < _tempOutputFiles.size(); i++)
    {
      LOG_VARD(_tempOutputFiles.at(i)->fileName());
    }
  }
}

void ExternalMergeElementSorter::_mergeSortedElements(ElementPriorityQueue& priorityQueue,
                                           boost::shared_ptr<PartialOsmMapWriter> writer,
                                           QList<boost::shared_ptr<PartialOsmMapReader>> readers)
{
  LOG_DEBUG("Iterating through remaining elements in sorted order...");

  int i = 0;
  long elementPerFileCount = 0;
  long pushesToPriorityQueue = 0;
  while (i != readers.size())
  {
    ConstElementPtr rootElement = priorityQueue.top();
    priorityQueue.pop();
    if (rootElement->getId() != LONG_MAX)
    {
      LOG_TRACE("Writing element from priority queue: " << rootElement);
      writer->writePartial(rootElement);
      elementPerFileCount++;
    }
    if (readers.at(i)->hasMoreElements())
    {
      rootElement = readers.at(i)->readNextElement();
      LOG_TRACE("Read new element: " << rootElement);
    }
    else
    {
      LOG_TRACE("No elements left in file.");
      rootElement.reset(new Relation(Status::Invalid, LONG_MAX, 15.0));
      LOG_VART(elementPerFileCount);
      elementPerFileCount = 0;
      i++;
    }

    LOG_TRACE("Pushing element to priority queue: " << rootElement);
    priorityQueue.push(rootElement);
    pushesToPriorityQueue++;
  }
  LOG_VART(priorityQueue.size());
  LOG_VART(pushesToPriorityQueue);
}

boost::shared_ptr<PartialOsmMapWriter> ExternalMergeElementSorter::_getFinalOutputWriter()
{
  LOG_DEBUG("Initializing final output...");

  _sortFinalOutput.reset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() + "/" + SORT_TEMP_FILE_BASE_NAME + "." +
      _tempFormat));
  if (_retainTempFiles)
  {
    //for debugging only
    _sortFinalOutput->setAutoRemove(false);
  }
  if (!_sortFinalOutput->open())
  {
    throw HootException("Unable to open sort temp file: " + _sortFinalOutput->fileName() + ".");
  }
  else
  {
    LOG_DEBUG(
      "Opened temp final output file: " << _sortFinalOutput->fileName() << " for sorted output.");
  }
  boost::shared_ptr<PartialOsmMapWriter> writer =
    boost::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::getInstance().createWriter(_sortFinalOutput->fileName()));
  writer->open(_sortFinalOutput->fileName());

  return writer;
}

ElementPriorityQueue ExternalMergeElementSorter::_getInitializedPriorityQueue(
  QList<boost::shared_ptr<PartialOsmMapReader>>& readers)
{
  LOG_DEBUG("Writing initial records from each temp file to priority queue...");

  ElementPriorityQueue priorityQueue;
  for (int i = 0; i < _tempOutputFiles.size(); i++)
  {
    const QString fileName = _tempOutputFiles.at(i)->fileName();
    LOG_VART(fileName);
    boost::shared_ptr<PartialOsmMapReader> reader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(fileName));
    reader->setUseDataSourceIds(true);
    reader->open(fileName);
    readers.append(reader);

    //this should always return true but has to be called before readNextElement, so an assert
    //isn't good enough here
    if (reader->hasMoreElements())
    {
      ConstElementPtr element = reader->readNextElement();
      LOG_TRACE("Pushing element to priority queue: " << element);
      priorityQueue.push(element);
    }
  }
  LOG_VART(priorityQueue.size());
  LOG_VART(readers.size());

  return priorityQueue;
}

}
