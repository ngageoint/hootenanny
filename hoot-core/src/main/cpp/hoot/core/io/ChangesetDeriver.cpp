/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
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

    /*if (_toE->getTags().contains("note"))
    {
      result.note = _toE->getTags().get("note");
    }*/
    /*LOG_DEBUG(
      "run out of from elements; 'from' element null; 'to' element not null: " <<
        _toE->getElementId() << "; creating 'to' element...");
    LOG_VARD(result.toString());*/

    _toE = _to->readNextElement();
  }
  // if we've run out of "to" elements, delete all the remaining elements in "from"
  else if (_fromE.get() && !_toE.get())
  {
    result.type = Change::Delete;
    result.e = _fromE;

    /*if (_fromE->getTags().contains("note"))
    {
      result.note = _fromE->getTags().get("note");
    }*/
    /*LOG_DEBUG(
      "run out of 'to' elements; to' element null; 'from' element not null: " <<
      _fromE->getElementId() << "; deleting 'from' element...");
    LOG_VARD(result.toString());*/

    _fromE = _from->readNextElement();
  }
  else
  {
    // while the elements are exactly the same there is nothing to do.
    while (_fromE.get() && _toE.get() && _fromE->getElementId() == _toE->getElementId() &&
           ElementComparer().isSame(_fromE, _toE))
    {
      /*LOG_DEBUG("skipping identical elements - 'from' element: " << _fromE->getElementId() <<
                " 'to' element: " << _toE->getElementId());
      LOG_VARD(_fromE->toString());
      LOG_VARD(_toE->toString());*/

      _toE = _to->readNextElement();
      _fromE = _from->readNextElement();
    }

    if (!_fromE.get() && !_toE.get())
    {
      // pass
      //LOG_DEBUG("both are null elements; skipping");
    }
    // if we've run out of "from" elements, create all the remaining elements in "to"
    else if (!_fromE.get() && _toE.get())
    {
      result.type = Change::Create;
      result.e = _toE;

      /*if (_toE->getTags().contains("note"))
      {
        result.note = _toE->getTags().get("note");
      }*/
      /*LOG_DEBUG(
        "run out of from elements; 'from' element null; 'to' element not null: " <<
          _toE->getElementId() << "; creating 'to' element...");
      LOG_VARD(_toE->toString());
      LOG_VARD(result.toString());*/

      _toE = _to->readNextElement();
    }
    // if we've run out of "to" elements, delete all the remaining elements in "from"
    else if (_fromE.get() && !_toE.get())
    {
      result.type = Change::Delete;
      result.e = _fromE;

      /*if (_fromE->getTags().contains("note"))
      {
        result.note = _fromE->getTags().get("note");
      }*/
      /*LOG_DEBUG(
        "run out of 'to' elements; to' element null; 'from' element not null: " <<
        _fromE->getElementId() << "; deleting 'from' element...");
      LOG_VARD(_fromE->toString());
      LOG_VARD(result.toString());*/

      _fromE = _from->readNextElement();
    }
    else if (_fromE->getElementId() == _toE->getElementId())
    {
      result.type = Change::Modify;
      result.e = _toE;

      /*if (_toE->getTags().contains("note"))
      {
        result.note = _toE->getTags().get("note");
      }*/
      /*LOG_DEBUG(
        "'from' element id: " << _fromE->getElementId() << " equals 'to' element id: " <<
        _toE->getElementId() << " modifying 'to' element...");
      LOG_VARD(_fromE->toString());
      LOG_VARD(_toE->toString());
      LOG_VARD(result.toString());*/

      _toE = _to->readNextElement();
    }
    else if (_fromE->getElementId() < _toE->getElementId())
    {
      result.type = Change::Delete;
      result.e = _fromE;

      /*if (_fromE->getTags().contains("note"))
      {
        result.note = _fromE->getTags().get("note");
      }*/
      /*LOG_DEBUG(
        "'from' element id: " << _fromE->getElementId() << " less than 'to' element id: " <<
        _toE->getElementId() << " deleting 'from' element...");
      LOG_VARD(_fromE->toString());
      LOG_VARD(_toE->toString());
      LOG_VARD(result.toString());*/

      _fromE = _from->readNextElement();
    }
    else
    {
      result.type = Change::Create;
      result.e = _toE;

      /*if (_toE->getTags().contains("note"))
      {
        result.note = _toE->getTags().get("note");
      }*/
      /*LOG_DEBUG(
        "'from' element id: " << _fromE->getElementId() << " greater than 'to' element id: " <<
        _toE->getElementId() << " creating 'to' element...");
      LOG_VARD(_fromE->toString());
      LOG_VARD(_toE->toString());
      LOG_VARD(result.toString());*/

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
