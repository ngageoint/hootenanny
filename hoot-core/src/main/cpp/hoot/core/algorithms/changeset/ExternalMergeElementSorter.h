
#ifndef EXTERNAL_MERGE_ELEMENT_SORTER_H
#define EXTERNAL_MERGE_ELEMENT_SORTER_H

// Hoot
#include <hoot/core/io/ElementInputStream.h>

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

  struct ElementCompare
  {
    bool operator()(const ConstElementPtr& e1, const ConstElementPtr& e2) const
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
  };

  ExternalMergeElementSorter(ElementInputStreamPtr input, const QString inputFileExtension);

  /**
   * @see ElementInputStream
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual void close();

  /**
   * @see ElementInputStream
   */
  virtual bool hasMoreElements();

  /**
   * @see ElementInputStream
   */
  virtual ElementPtr readNextElement();

private:

  boost::shared_ptr<QTemporaryFile> _sortTempFile;
  ElementInputStreamPtr _sortedElements;
  QString _inputFileExtension;
  long _maxElementsPerFile;

  void _sort(ElementInputStreamPtr input);
  QList<boost::shared_ptr<QTemporaryFile>> _createSortedFileOutputs(ElementInputStreamPtr input);
  void _mergeFiles(QList<boost::shared_ptr<QTemporaryFile>> tempOutputFiles);
};

}

#endif // EXTERNAL_MERGE_ELEMENT_SORTER_H
