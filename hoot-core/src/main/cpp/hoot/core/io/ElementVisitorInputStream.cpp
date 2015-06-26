#include <boost/shared_ptr.hpp>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementVisitor.h>
#include "ElementVisitorInputStream.h"

namespace hoot
{

ElementVisitorInputStream::ElementVisitorInputStream(const shared_ptr<ElementInputStream>& elementSource, const shared_ptr<ElementVisitor>& visitor) :
  _elementSource(elementSource),
  _visitor(visitor)
{
  ;
}

ElementPtr ElementVisitorInputStream::readNextElement()
{
  ElementPtr element = _elementSource->readNextElement();
  _visitor->visit(element);

  return element;
}


}
