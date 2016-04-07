#ifndef ELEMENTCOMPARER_H
#define ELEMENTCOMPARER_H

// Hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 *
 */
class ElementComparer
{

public:

  /**
   * Defaults to 5cm threshold
   */
  ElementComparer(Meters threshold = 0.05);

  bool isSame(ElementPtr e1, ElementPtr e2);

private:

  Meters _threshold;

  bool _compareNode(const shared_ptr<const Element>& re,
                    const shared_ptr<const Element>& e);

  bool _compareWay(const shared_ptr<const Element>& re,
                   const shared_ptr<const Element>& e);

  bool _compareRelation(const shared_ptr<const Element>& re,
                        const shared_ptr<const Element>& e);

};

}

#endif // ELEMENTCOMPARER_H
