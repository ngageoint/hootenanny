
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

 */
class ElementExternalMergeSorter : public ElementSorter
{

public:

  ElementExternalMergeSorter(const QString input);

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
  boost::shared_ptr<ElementInputStream> _inputStream;

  void _sort(QString input);
};

}

#endif // ELEMENT_EXTERNAL_MERGE_SORTER_H
