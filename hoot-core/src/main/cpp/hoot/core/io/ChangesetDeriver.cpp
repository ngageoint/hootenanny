#include "ChangesetDeriver.h"

#include <hoot/core/elements/Node.h>
#include <hoot/core/io/ElementComparer.h>
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

ChangesetDeriver::ChangesetDeriver(ElementInputStreamPtr from, ElementInputStreamPtr to) :
  _from(from),
  _to(to)
{
  LOG_INFO("Changeset deriver initialization...");

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

  // if we've run out of "from" elements, create all the remaining elements in "to"
  if (!_fromE.get() && _toE.get())
  {
    result.type = Change::Create;
    result.e = _toE;

    LOG_DEBUG("run out of 'from'' elements:");
    LOG_VARD(result.toString());

    _toE = _to->readNextElement();
  }
  // if we've run out of "to" elements, delete all the remaining elements in "from"
  else if (_fromE.get() && !_toE.get())
  {
    result.type = Change::Delete;
    result.e = _fromE;

    LOG_DEBUG("run out of 'to' elements:");
    LOG_VARD(result.toString());

    _fromE = _from->readNextElement();
  }
  else
  {
    // while the elements are exactly the same there is nothing to do.
    while (_fromE.get() && _toE.get() &&
        _fromE->getElementId() == _toE->getElementId() &&
        ElementComparer().isSame(_fromE, _toE))
    {
      LOG_DEBUG("skipping identical elements - 'from' element: " << _fromE->getElementId() <<
                " 'to' element: " << _toE->getElementId());

      _fromE = _from->readNextElement();
      _toE = _to->readNextElement();
    }

    if (!_fromE.get() && !_toE.get())
    {
      // pass
      LOG_DEBUG("both null elements");
    }
    else if (!_fromE.get() && _toE.get())
    {
      result.type = Change::Create;
      result.e = _toE;

      LOG_DEBUG("'from' element null; 'to' element not null: " << _toE->getElementId());
      LOG_VARD(result.toString());

      _toE = _to->readNextElement();
    }
    // if we've run out of "to" elements, delete all the remaining elements in "from"
    else if (_fromE.get() && !_toE.get())
    {
      result.type = Change::Delete;
      result.e = _fromE;

      LOG_DEBUG("'to' element null; 'from' element not null: " << _fromE->getElementId());
      LOG_VARD(result.toString());

      _fromE = _from->readNextElement();
    }
    else if (_fromE->getElementId() == _toE->getElementId())
    {
      result.type = Change::Modify;
      result.e = _toE;

      LOG_DEBUG(
        "'from' element id: " << _fromE->getElementId() << " equals 'to' element id: " <<
        _toE->getElementId());
      LOG_VARD(result.toString());

      _toE = _to->readNextElement();
    }
    else if (_fromE->getElementId() < _toE->getElementId())
    {
      result.type = Change::Delete;
      result.e = _fromE;

      LOG_DEBUG(
        "'from' element id: " << _fromE->getElementId() << " less than 'to' element id: " <<
        _toE->getElementId());
      LOG_VARD(result.toString());

      _fromE = _from->readNextElement();
    }
    else
    {
      result.type = Change::Create;
      result.e = _toE;

      LOG_DEBUG(
        "'from' element id: " << _fromE->getElementId() << " greater than 'to' element id: " <<
        _toE->getElementId());
      LOG_VARD(result.toString());

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
