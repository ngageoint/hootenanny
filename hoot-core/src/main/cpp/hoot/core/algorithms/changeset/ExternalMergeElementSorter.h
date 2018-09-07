
#ifndef EXTERNAL_MERGE_ELEMENT_SORTER_H
#define EXTERNAL_MERGE_ELEMENT_SORTER_H

// Hoot
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/PartialOsmMapReader.h>

// Qt
#include <QTemporaryFile>

namespace hoot
{

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

  struct ElementCompare
  {
    bool operator()(const ConstElementPtr& e1, const ConstElementPtr& e2) const
    {
      return _elementCompare2(e1, e2);
    }
  };

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
  QList<boost::shared_ptr<QTemporaryFile>> _createSortedFileOutputs(ElementInputStreamPtr input);

  /*
   */
  void _mergeFiles(QList<boost::shared_ptr<QTemporaryFile>> tempOutputFiles);

  /*
   */
  void _initElementStream();

  /*
   */
  static bool _elementCompare1(const ConstElementPtr& e1, const ConstElementPtr& e2);

  static bool _elementCompare2(const ConstElementPtr& e1, const ConstElementPtr& e2);

  //static QString _getSortTempFileBaseName() { return "element-sorter-temp-XXXXXX"; }
};

}

#endif // EXTERNAL_MERGE_ELEMENT_SORTER_H
