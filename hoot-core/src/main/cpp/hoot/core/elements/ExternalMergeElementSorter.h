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

#ifndef EXTERNAL_MERGE_ELEMENT_SORTER_H
#define EXTERNAL_MERGE_ELEMENT_SORTER_H

// Hoot
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/PartialOsmMapWriter.h>

// Qt
#include <QTemporaryFile>

// std
#include <queue>

namespace hoot
{

struct PqElement
{
  ConstElementPtr element;
  //the index of the temporary file this element resides in needed when combining multiple sorted
  //files into a single file
  int fileIndex;
};

struct ElementComparePq
{
  //The priority queue expects the reverse element sorting priority that we use with vector sorting.
  bool operator()(const PqElement& e1, const PqElement& e2) const
  {
    const ElementType::Type type1 = e1.element->getElementType().getEnum();
    const ElementType::Type type2 = e2.element->getElementType().getEnum();
    if (type1 == type2)
    {
      return e1.element->getId() > e2.element->getId();
    }
    else
    {
      return type1 > type2;
    }
  }
};

typedef std::priority_queue<PqElement,
                            std::vector<PqElement>, ElementComparePq> ElementPriorityQueue;

/**
  This performs element sorting outside of main memory on disk and serves the sorted results up
  as an element stream.

  Very much influenced by http://www.techiedelight.com/external-merge-sort
 */
class ExternalMergeElementSorter : public ElementInputStream
{

public:

  ExternalMergeElementSorter();
  virtual ~ExternalMergeElementSorter();

  /**
   * Sorts elements first by type, then increasing by ID
   *
   * @param input a stream of elements
   */
  void sort(ElementInputStreamPtr input);

  /**
   * @see ElementInputStream
   */
  virtual std::shared_ptr<OGRSpatialReference> getProjection() const override;

  /**
   * @see ElementInputStream
   */
  virtual void close();

  /**
   * @see ElementInputStream
   */
  virtual bool hasMoreElements();

  /**
   * @see ElementInputStream
   */
  virtual ElementPtr readNextElement() override;

  void setMaxElementsPerFile(long max) { _maxElementsPerFile = max; }
  void setRetainTempFiles(bool retain) { _retainTempFiles = retain; }
  int getNumTempFiles() const { return _tempOutputFiles.size(); }

private:

  static const QString SORT_TEMP_FILE_BASE_NAME;

  //the final merged sorted element output
  std::shared_ptr<QTemporaryFile> _sortFinalOutput;

  //a stream of sorted elements to serve up
  ElementInputStreamPtr _sortedElements;

  //max elements written to any temp sorted file
  long _maxElementsPerFile;

  //set true for debugging contents of temp files only
  bool _retainTempFiles;

  //pre-merged temp files; auto-deleted on exit
  QList<std::shared_ptr<QTemporaryFile>> _tempOutputFiles;

  long _logUpdateInterval;

  void _sort(ElementInputStreamPtr input);

  /*
   * Writes a set of OSM files filled with sorted elements; the number of files written is the total
   * elements read from the source divided by the element buffer size
   */
  void _createSortedFileOutputs(ElementInputStreamPtr input);

  /*
   * Merges each sorted temp file into a final sorted file
   */
  void _mergeSortedFiles();

  /*
   * Prepares the element stream that serves up the sorted elements
   */
  void _initElementStream();

  /*
   * Order by element type (node, way, relation), then by increasing ID
   */
  static bool _elementCompare(const ConstElementPtr& e1, const ConstElementPtr& e2);

  /*
   * Adds the first member from each temp file to the priority queue
   */
  ElementPriorityQueue _getInitializedPriorityQueue(
    QList<std::shared_ptr<PartialOsmMapReader>>& readers);

  void _mergeSortedElements(ElementPriorityQueue& priorityQueue,
                            std::shared_ptr<PartialOsmMapWriter> writer,
                            QList<std::shared_ptr<PartialOsmMapReader>> readers);

  std::shared_ptr<PartialOsmMapWriter> _getFinalOutputWriter();

  void _printPriorityQueue(ElementPriorityQueue priorityQueue);
};

}

#endif // EXTERNAL_MERGE_ELEMENT_SORTER_H
