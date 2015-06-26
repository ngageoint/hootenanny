#ifndef ELEMENTVISITORINPUTSTREAM_H
#define ELEMENTVISITORINPUTSTREAM_H

#include <boost/shared_ptr.hpp>

#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

class ElementVisitorInputStream : public ElementInputStream
{
public:
  /**
   * @brief ElementVisitorInputStream
   * @param elementSource The stream used to read elements from
   * @param visitor The operation that should be performed on elements that are read from the input source before being returned
   */
  ElementVisitorInputStream(const shared_ptr<ElementInputStream>& elementSource, const shared_ptr<ElementVisitor>& visitor);

  /**
   * @brief close
   * Invokes the close function on the source element input stream
   */
  virtual void close() { _elementSource->close(); }

  /**
   * @brief hasMoreElements
   * @return return value from call to source ElementInputStream's hasMoreElements() method
   */
  virtual bool hasMoreElements() { return _elementSource->hasMoreElements(); }

  /**
   * @brief readNextElement
   * @return Pointer to an elemement which will have been read from the source elementinputstream
   *    AND had the visitor's visit method applied to it before it is returned
   */
  virtual ElementPtr readNextElement();


private:
  shared_ptr<ElementInputStream> _elementSource;
  shared_ptr<ElementVisitor> _visitor;
};

}

#endif // ELEMENTVISITORINPUTSTREAM_H
