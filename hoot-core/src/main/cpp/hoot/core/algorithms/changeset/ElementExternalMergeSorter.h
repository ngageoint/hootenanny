
#ifndef ELEMENT_EXTERNAL_MERGE_SORTER_H
#define ELEMENT_EXTERNAL_MERGE_SORTER_H

// Hoot
#include "ElementSorter.h"
#include <hoot/core/io/ElementInputStream.h>

// Qt
#include <QTemporaryFile>

namespace hoot
{

/**
  Very much influenced by http://www.techiedelight.com/external-merge-sort
 */
class ElementExternalMergeSorter : public ElementSorter
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

  ElementExternalMergeSorter(ElementInputStreamPtr input, const QString inputFileExtension);

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

  static bool isSupportedInputFormat(const QString input);

private:

  boost::shared_ptr<QTemporaryFile> _sortTempFile;
  ElementInputStreamPtr _sortedElements;
  QString _inputFileExtension;
  int _maxElementsPerFile;

  void _sort(ElementInputStreamPtr input);
  QList<boost::shared_ptr<QTemporaryFile>> _createSortedFileOutputs(ElementInputStreamPtr input);
  void _mergeFiles(QList<boost::shared_ptr<QTemporaryFile>> tempOutputFiles);
};

}

#endif // ELEMENT_EXTERNAL_MERGE_SORTER_H
