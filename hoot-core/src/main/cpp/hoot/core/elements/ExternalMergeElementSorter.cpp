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
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "ExternalMergeElementSorter.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlReader.h>

namespace hoot
{

const QString ExternalMergeElementSorter::SORT_TEMP_FILE_BASE_NAME = "element-sorter-temp-XXXXXX";

ExternalMergeElementSorter::ExternalMergeElementSorter() :
_maxElementsPerFile(ConfigOptions().getElementSorterElementBufferSize()),
_retainTempFiles(false),
_logUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
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
    std::shared_ptr<PartialOsmMapReader> sortedElementsReader =
      std::dynamic_pointer_cast<PartialOsmMapReader>(_sortedElements);
    sortedElementsReader->finalizePartial();
    _sortedElements->close();
  }
}

std::shared_ptr<OGRSpatialReference> ExternalMergeElementSorter::getProjection() const
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
  LOG_INFO("Sorting elements on external disk...");

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

  std::shared_ptr<PartialOsmMapReader> sortedElementsReader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(_sortFinalOutput->fileName()));
  sortedElementsReader->setUseDataSourceIds(true);
  sortedElementsReader->open(_sortFinalOutput->fileName());
  sortedElementsReader->initializePartial();
  _sortedElements = std::dynamic_pointer_cast<ElementInputStream>(sortedElementsReader);
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
  std::vector<ConstElementPtr> elements;
  elements.reserve(_maxElementsPerFile);

  long elementsWritten = 0;
  while (input->hasMoreElements())
  {
    elements.push_back(input->readNextElement());
    elementCtr++;

    if ((elementCtr % _maxElementsPerFile == 0 && elementCtr != 0) || !input->hasMoreElements())
    {
      LOG_DEBUG("Sorting " << elements.size() << " elements...");
      std::sort(elements.begin(), elements.end(), _elementCompare);

      LOG_DEBUG("Writing elements to temp file...");

      std::shared_ptr<QTemporaryFile> tempOutputFile(
        new QTemporaryFile(
          ConfigOptions().getApidbBulkInserterTempFileDir() + "/" + SORT_TEMP_FILE_BASE_NAME +
          ".osm"));
      tempOutputFile->setAutoRemove(!_retainTempFiles);
      if (!tempOutputFile->open())
      {
        throw HootException("Unable to open sort temp file: " + tempOutputFile->fileName() + ".");
      }
      else
      {
        LOG_DEBUG("Opened temp file: " << tempOutputFile->fileName() << " for sorted output.");
      }
      _tempOutputFiles.append(tempOutputFile);

      std::shared_ptr<PartialOsmMapWriter> writer =
        std::dynamic_pointer_cast<PartialOsmMapWriter>(
          OsmMapWriterFactory::createWriter(tempOutputFile->fileName()));
      writer->open(tempOutputFile->fileName());
      writer->initializePartial();
      for (std::vector<ConstElementPtr>::const_iterator itr = elements.begin();
           itr != elements.end(); ++itr)
      {
        writer->writePartial(*itr);
        elementsWritten++;

        if (elementsWritten % _logUpdateInterval == 0)
        {
          PROGRESS_INFO("Wrote " << elementsWritten << " temporary sorted elements.");
        }
      }
      elements.clear();

      writer->finalizePartial();
      writer->close();
      tempOutputFile->close();
    }
  }
  //caller should close the input

  LOG_DEBUG("Finished writing sorted file outputs.");
  LOG_VART(elementCtr);
  LOG_VART(elementsWritten);
  LOG_VART(_tempOutputFiles.size());
}

void ExternalMergeElementSorter::_mergeSortedFiles()
{
  LOG_DEBUG("Merging " << _tempOutputFiles.size() << " temporary files...");

  QList<std::shared_ptr<PartialOsmMapReader>> readers;
  ElementPriorityQueue priorityQueue = _getInitializedPriorityQueue(readers);
  std::shared_ptr<PartialOsmMapWriter> writer = _getFinalOutputWriter();

  _mergeSortedElements(priorityQueue, writer, readers);

  writer->finalizePartial();
  writer->close();

  if (_retainTempFiles)
  {
    //for debugging only
    LOG_DEBUG("Sorted temp files: ");
    for (int i = 0; i < _tempOutputFiles.size(); i++)
    {
      LOG_VARD(_tempOutputFiles.at(i)->fileName());
    }
  }
}

void ExternalMergeElementSorter::_mergeSortedElements(ElementPriorityQueue& priorityQueue,
                                              std::shared_ptr<PartialOsmMapWriter> writer,
                                              QList<std::shared_ptr<PartialOsmMapReader>> readers)
{
  LOG_DEBUG("Iterating through remaining elements in sorted order...");

  int numFullyParsedFiles = 0;
  long elementsWritten = 0;
  long pushesToPriorityQueue = 0;

  //Go through the contents of each sorted files until all elements have been parsed.
  while (numFullyParsedFiles != readers.size())
  {
    LOG_VART(numFullyParsedFiles);
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

    if (elementsWritten % _logUpdateInterval == 0)
    {
      PROGRESS_INFO("Wrote " << elementsWritten << " final sorted elements.");
    }

    //The next element to be pushed to the root of the priority queue will be in the same file as
    //the previously read root element, if there are any more elements in that file.
    if (readers.at(rootPqElement.fileIndex)->hasMoreElements())
    {
      rootPqElement.element = readers.at(rootPqElement.fileIndex)->readNextElement();
      LOG_TRACE(
        "Read new element: " << rootPqElement.element->getElementId() << " from file: " <<
        rootPqElement.fileIndex);  
    }
    //Otherwise, move to the next file.
    else
    {
      LOG_TRACE("No elements left in file: " << rootPqElement.fileIndex);
      readers.at(rootPqElement.fileIndex)->close();
      //null elements don't get pushed onto the queue
      rootPqElement.element.reset();
      numFullyParsedFiles++;
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
  LOG_VART(numFullyParsedFiles);
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

std::shared_ptr<PartialOsmMapWriter> ExternalMergeElementSorter::_getFinalOutputWriter()
{
  LOG_DEBUG("Initializing final output...");

  const QString tempFile =
    ConfigOptions().getApidbBulkInserterTempFileDir() + "/" + SORT_TEMP_FILE_BASE_NAME + ".osm";
  _sortFinalOutput.reset(new QTemporaryFile(tempFile));
  _sortFinalOutput->setAutoRemove(!_retainTempFiles);
  if (!_sortFinalOutput->open())
  {
    throw HootException("Unable to open sort temp file: " + _sortFinalOutput->fileName() + ".");
  }
  else
  {
    LOG_DEBUG(
      "Opened temp final output file: " << _sortFinalOutput->fileName() << " for sorted output.");
  }
  std::shared_ptr<PartialOsmMapWriter> writer =
    std::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::createWriter(_sortFinalOutput->fileName()));
  writer->open(_sortFinalOutput->fileName());
  writer->initializePartial();

  return writer;
}

ElementPriorityQueue ExternalMergeElementSorter::_getInitializedPriorityQueue(
  QList<std::shared_ptr<PartialOsmMapReader>>& readers)
{
  LOG_DEBUG("Writing initial records from each temp file to priority queue...");

  //Push the first element from each sorted file onto the priority queue to seed it.
  ElementPriorityQueue priorityQueue;
  for (int i = 0; i < _tempOutputFiles.size(); i++)
  {
    const QString fileName = _tempOutputFiles.at(i)->fileName();
    LOG_VART(fileName);
    std::shared_ptr<PartialOsmMapReader> reader =
      std::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::createReader(fileName));

    //By default, OsmXmlReader will not add child references (node ref, elements members) to parent
    //elements if those elements are not present in the data.  For external sorting, where partial
    //chunks of elements will be present, we need to change that behavior.
    std::shared_ptr<OsmXmlReader> xmlReader =
      std::dynamic_pointer_cast<OsmXmlReader>(reader);
    if (xmlReader.get())
    {
      xmlReader->setAddChildRefsWhenMissing(true);
    }

    reader->setUseDataSourceIds(true);
    reader->open(fileName);
    reader->initializePartial();
    readers.append(reader);

    //This should always return true but is required to be called before readNextElement, so just an
    //assert isn't good enough here.
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
