
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

struct ElementComparePQ
{
  bool operator()(const ConstElementPtr& e1, const ConstElementPtr& e2) const
  {
    const ElementType::Type type1 = e1->getElementType().getEnum();
    const ElementType::Type type2 = e2->getElementType().getEnum();
    if (type1 == type2)
    {
      return e1->getId() > e2->getId();
    }
    else
    {
      return type1 > type2;
    }
  }
};

typedef std::priority_queue<ConstElementPtr,
                            std::vector<ConstElementPtr>, ElementComparePQ> ElementPriorityQueue;

/**
  Very much influenced by http://www.techiedelight.com/external-merge-sort
 */
class ExternalMergeElementSorter : public ElementInputStream
{

public:

  ExternalMergeElementSorter();
  ~ExternalMergeElementSorter();

  /**
   *
   *
   * @param input
   * @param inputFileExtension
   */
  void sort(ElementInputStreamPtr input, QString inputFileExtension);

  /**
   * @see ElementInputStream
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

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
  virtual ElementPtr readNextElement();

  void setMaxElementsPerFile(long max) { _maxElementsPerFile = max; }

  int getNumTempFiles() const { return _tempOutputFiles.size(); }

private:

  static const QString SORT_TEMP_FILE_BASE_NAME;

  //
  boost::shared_ptr<QTemporaryFile> _sortTempFile;
  //
  ElementInputStreamPtr _sortedElements;
  boost::shared_ptr<PartialOsmMapReader> _sortedElementsReader;
  //
  QString _inputFileExtension;
  //
  long _maxElementsPerFile;
  QList<boost::shared_ptr<QTemporaryFile>> _tempOutputFiles;

  void _sort(ElementInputStreamPtr input);

  /*
   */
  void _createSortedFileOutputs(ElementInputStreamPtr input);

  /*
   */
  void _mergeSortedFiles();

  /*
   */
  void _initElementStream();

  /*
   */
  static bool _elementCompare(const ConstElementPtr& e1, const ConstElementPtr& e2);

  ElementPriorityQueue _getInitializedPriorityQueue(
    QList<boost::shared_ptr<PartialOsmMapReader>>& readers);

  void _mergeSortedElements(ElementPriorityQueue& priorityQueue,
                            boost::shared_ptr<PartialOsmMapWriter> writer,
                            QList<boost::shared_ptr<PartialOsmMapReader>> readers);

  boost::shared_ptr<PartialOsmMapWriter> _getFinalOutputWriter();
};

}

#endif // EXTERNAL_MERGE_ELEMENT_SORTER_H
