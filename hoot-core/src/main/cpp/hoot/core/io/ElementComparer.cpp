#include "ElementComparer.h"

#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

/**
 * Defaults to 5cm threshold
 */
ElementComparer::ElementComparer(Meters threshold) :
_threshold(threshold)
{

}

bool ElementComparer::isSame(ElementPtr e1, ElementPtr e2)
{
  if (e1->getElementId() != e2->getElementId() ||
      !(e1->getTags() == e2->getTags()) ||
      e1->getStatus() != e2->getStatus() ||
      e1->getVersion() != e2->getVersion() ||
      fabs(e1->getCircularError() - e2->getCircularError()) > _threshold)
  {
    return false;
  }
  switch (e1->getElementType().getEnum())
  {
    case ElementType::Node:
      return _compareNode(e1, e2);
    case ElementType::Way:
      return _compareWay(e1, e2);
    case ElementType::Relation:
      return _compareRelation(e1, e2);
    default:
      throw IllegalArgumentException("Unexpected element type.");
  }
}

bool ElementComparer::_compareNode(const shared_ptr<const Element>& re,
                                   const shared_ptr<const Element>& e)
{
  shared_ptr<const Node> rn = dynamic_pointer_cast<const Node>(re);
  shared_ptr<const Node> n = dynamic_pointer_cast<const Node>(e);

  //LOG_VARD(GeometryUtils::haversine(rn->toCoordinate(), n->toCoordinate()));
  return (GeometryUtils::haversine(rn->toCoordinate(), n->toCoordinate()) <= _threshold);
}

bool ElementComparer::_compareWay(const shared_ptr<const Element>& re,
                                  const shared_ptr<const Element>& e)
{
  shared_ptr<const Way> rw = dynamic_pointer_cast<const Way>(re);
  shared_ptr<const Way> w = dynamic_pointer_cast<const Way>(e);

  if (rw->getNodeIds().size() != w->getNodeIds().size())
  {
    return false;
  }
  for (size_t i = 0; i < rw->getNodeIds().size(); ++i)
  {
    if (rw->getNodeIds()[i] != w->getNodeIds()[i])
    {
      return false;
    }
  }

  return true;
}

bool ElementComparer::_compareRelation(const shared_ptr<const Element>& re,
                                       const shared_ptr<const Element>& e)
{
  shared_ptr<const Relation> rr = dynamic_pointer_cast<const Relation>(re);
  shared_ptr<const Relation> r = dynamic_pointer_cast<const Relation>(e);

  if (rr->getType() != r->getType() ||
      rr->getMembers().size() != r->getMembers().size())
  {
    return false;
  }

  for (size_t i = 0; i < rr->getMembers().size(); i++)
  {
    if (rr->getMembers()[i].role != r->getMembers()[i].role ||
        rr->getMembers()[i].getElementId() != r->getMembers()[i].getElementId())
    {
      return false;
    }
  }

    return true;
  }

}
