
#include "ExternalMergeElementSorter.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlReader.h>

namespace hoot
{

const QString ExternalMergeElementSorter::SORT_TEMP_FILE_BASE_NAME = "element-sorter-temp-XXXXXX";

ExternalMergeElementSorter::ExternalMergeElementSorter() :
_tempFormat(ConfigOptions().getElementSorterExternalTempFormat()),
_maxElementsPerFile(ConfigOptions().getElementSorterElementBufferSize()),
_retainTempFiles(false)
{
}

ExternalMergeElementSorter::~ExternalMergeElementSorter()
{
  close();
}

void ExternalMergeElementSorter::close()
{
  if (_sortedElements)
  {
    boost::shared_ptr<PartialOsmMapReader> sortedElementsReader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(_sortedElements);
    sortedElementsReader->finalizePartial();
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

  if (_maxElementsPerFile < 1)
  {
    throw HootException("Invalid buffer size value: " + QString::number(_maxElementsPerFile));
  }

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

  boost::shared_ptr<PartialOsmMapReader> sortedElementsReader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::getInstance().createReader(_sortFinalOutput->fileName()));
  sortedElementsReader->setUseDataSourceIds(true);
  sortedElementsReader->open(_sortFinalOutput->fileName());
  _sortedElements = boost::dynamic_pointer_cast<ElementInputStream>(sortedElementsReader);
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
    ConstElementPtr element = input->readNextElement();
    LOG_TRACE("Read element: " << element->getElementId());
    elements.push_back(element);
    elementCtr++;

    if ((elementCtr % _maxElementsPerFile == 0 && elementCtr != 0) || !input->hasMoreElements())
    {
      LOG_DEBUG("Sorting " << elements.size() << " elements...");
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
        LOG_TRACE("Wrote element: " << element->getElementId());
        writer->writePartial(element);
      }
      elements.clear();
      LOG_VART(elements.size());

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

  int fullyParsedFiles = 0;
  long elementsWritten = 0;
  long pushesToPriorityQueue = 0;

  //Go through the contents of each sorted files until all elements have been parsed.
  while (fullyParsedFiles != readers.size())
  {
    LOG_VART(fullyParsedFiles);
    //_printPriorityQueue(priorityQueue);

    //pop the next element to be written off of the queue
    PqElement rootPqElement = priorityQueue.top();
    priorityQueue.pop();
    LOG_TRACE(
      "Read root element from priority queue and removed it: " <<
      rootPqElement.element->getElementId());

    //write the element to the final output file
    LOG_TRACE(
      "Writing root element from priority queue to final output: " <<
      rootPqElement.element->getElementId());
    writer->writePartial(rootPqElement.element);
    elementsWritten++;
    LOG_VART(elementsWritten);

    //The next file to be pushed to the root of the priority queue will be in the same file as the
    //previously read root, if there are any more elements in the file.
    if (readers.at(rootPqElement.fileIndex)->hasMoreElements())
    {
      ConstElementPtr element = readers.at(rootPqElement.fileIndex)->readNextElement();
      LOG_TRACE(
        "Read new element: " << element->getElementId() << " from file: " <<
      rootPqElement.fileIndex);
      rootPqElement.element = element;
    }
    //Otherwise, move to the next file.
    else
    {
      LOG_TRACE("No elements left in file: " << rootPqElement.fileIndex);
      readers.at(rootPqElement.fileIndex)->close();
      //null elements don't get pushed onto the queue
      rootPqElement.element.reset();
      fullyParsedFiles++;
    }

    if (rootPqElement.element)
    {
      LOG_TRACE(
        "Pushing element to priority queue: " << rootPqElement.element->getElementId() <<
        " for file: " << rootPqElement.fileIndex);
      priorityQueue.push(rootPqElement);
      pushesToPriorityQueue++;
    }
  }

  LOG_VART(elementsWritten);
  LOG_VART(fullyParsedFiles);
  LOG_VART(priorityQueue.size());
  LOG_VART(pushesToPriorityQueue);
}

void ExternalMergeElementSorter::_printPriorityQueue(ElementPriorityQueue priorityQueue)
{
  QString str;
  while (!priorityQueue.empty())
  {
    PqElement pqElement = priorityQueue.top();
    str += pqElement.fileIndex + "," + pqElement.element->getElementId().toString() + ";";
    priorityQueue.pop();
  }
  LOG_TRACE("Priority queue: " << str);
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

  //Push the first element from each sorted file onto the priority queue to seed it.
  ElementPriorityQueue priorityQueue;
  for (int i = 0; i < _tempOutputFiles.size(); i++)
  {
    const QString fileName = _tempOutputFiles.at(i)->fileName();
    LOG_VART(fileName);
    boost::shared_ptr<PartialOsmMapReader> reader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(fileName));

    boost::shared_ptr<OsmXmlReader> xmlReader =
      boost::dynamic_pointer_cast<OsmXmlReader>(reader);
    if (xmlReader.get())
    {
      xmlReader->setAddChildRefsWhenMissing(true);
    }

    reader->setUseDataSourceIds(true);
    reader->open(fileName);
    readers.append(reader);

    //This should always return true but is required to be called before readNextElement, so just an
    //assert isn't good enough here
    if (reader->hasMoreElements())
    {
      ConstElementPtr element = reader->readNextElement();
      PqElement pqElement;
      pqElement.element = element;
      pqElement.fileIndex = i;
      LOG_TRACE(
        "Pushing element to priority queue: " << pqElement.element->getElementId() <<
        " from file: " << pqElement.fileIndex);
      priorityQueue.push(pqElement);
    }
  }
  LOG_VART(priorityQueue.size());
  LOG_VART(readers.size());

  return priorityQueue;
}

}
