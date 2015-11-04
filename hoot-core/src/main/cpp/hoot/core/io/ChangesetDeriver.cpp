#include "ChangesetDeriver.h"

#include <hoot/core/elements/Node.h>
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

class ElementCompare
{
public:

  /**
   * Defaults to 5cm threshold
   */
  ElementCompare(Meters threshold = 0.05)
  {
    _threshold = threshold;
  }

  bool isSame(ElementPtr e1, ElementPtr e2)
  {
    if (e1->getElementId() != e2->getElementId() ||
        !(e1->getTags() == e2->getTags()) ||
        e1->getStatus() != e2->getStatus() ||
        fabs(e1->getCircularError() - e2->getCircularError()) > _threshold)
    {
      return false;
    }
    switch (e1->getElementType().getEnum())
    {
    case ElementType::Node:
      return compareNode(e1, e2);
    case ElementType::Way:
      return compareWay(e1, e2);
    case ElementType::Relation:
      return compareRelation(e1, e2);
    default:
      throw IllegalArgumentException("Unexpected element type.");
    }
  }

  bool compareNode(const shared_ptr<const Element>& re, const shared_ptr<const Element>& e)
  {
    shared_ptr<const Node> rn = dynamic_pointer_cast<const Node>(re);
    shared_ptr<const Node> n = dynamic_pointer_cast<const Node>(e);

    return (GeometryUtils::haversine(rn->toCoordinate(), n->toCoordinate()) <= _threshold);
  }

  bool compareWay(const shared_ptr<const Element>& re, const shared_ptr<const Element>& e)
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

  bool compareRelation(const shared_ptr<const Element>& re, const shared_ptr<const Element>& e)
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

private:
  Meters _threshold;
};

ChangesetDeriver::ChangesetDeriver(ElementInputStreamPtr from, ElementInputStreamPtr to) :
  _from(from),
  _to(to)
{
  if (_from->getProjection()->IsGeographic() == false ||
      _to->getProjection()->IsGeographic() == false)
  {
    throw IllegalArgumentException("The projections must both be geographic.");
  }
}

ChangesetDeriver::~ChangesetDeriver()
{
}

boost::shared_ptr<OGRSpatialReference> ChangesetDeriver::getProjection() const
{
  return _from->getProjection();
}


void ChangesetDeriver::close()
{
  _from->close();
  _to->close();
}

bool ChangesetDeriver::hasMoreChanges()
{
  if (_next.e.get() == 0)
  {
    _next = _nextChange();
  }

  return _next.e.get() != 0;
}

Change ChangesetDeriver::_nextChange()
{
  Change result;

  if (!_fromE.get() && _from->hasMoreElements())
  {
    _fromE = _from->readNextElement();
  }

  if (!_toE.get() && _to->hasMoreElements())
  {
    _toE = _to->readNextElement();
  }

  // if we've run out of from elements, create all the remaining elements in to
  if (!_fromE.get() && _toE.get())
  {
    result.type = Change::Create;
    result.e = _toE;
    _toE = _to->readNextElement();
  }
  // if we've run out of to elements, delete all the remaining elements in from
  else if (_fromE.get() && !_toE.get())
  {
    result.type = Change::Delete;
    result.e = _fromE;
    _fromE = _from->readNextElement();
  }
  else
  {
    // while the elements are exactly the same there is nothing to do.
    while (_fromE.get() && _toE.get() &&
        _fromE->getElementId() == _toE->getElementId() &&
        ElementCompare().isSame(_fromE, _toE))
    {
      _fromE = _from->readNextElement();
      _toE = _to->readNextElement();
    }

    if (!_fromE.get() && !_toE.get())
    {
      // pass
    }
    else if (!_fromE.get() && _toE.get())
    {
      result.type = Change::Create;
      result.e = _toE;
      _toE = _to->readNextElement();
    }
    // if we've run out of to elements, delete all the remaining elements in from
    else if (_fromE.get() && !_toE.get())
    {
      result.type = Change::Delete;
      result.e = _fromE;
      _fromE = _from->readNextElement();
    }
    else if (_fromE->getElementId() == _toE->getElementId())
    {
      result.type = Change::Modify;
      result.e = _toE;
      _toE = _to->readNextElement();
    }
    else if (_fromE->getElementId() < _toE->getElementId())
    {
      result.type = Change::Delete;
      result.e = _fromE;
      _fromE = _from->readNextElement();
    }
    else
    {
      result.type = Change::Create;
      result.e = _toE;
      _toE = _to->readNextElement();
    }
  }

  return result;
}

Change ChangesetDeriver::readNextChange()
{
  if (!_next.e)
  {
    _next = _nextChange();
  }

  Change result = _next;

  _next.e.reset();

  return result;
}

}
