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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ChangesetDeriver.h"

#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/MapProjector.h>

namespace hoot
{

ChangesetDeriver::ChangesetDeriver(ElementInputStreamPtr from, ElementInputStreamPtr to) :
_from(from),
_to(to),
_numFromElementsParsed(0),
_numToElementsParsed(0),
_allowDeletingReferenceFeatures(ConfigOptions().getChangesetAllowDeletingReferenceFeatures()),
_metadataAllowKeys(ConfigOptions().getChangesetMetadataAllowedTagKeys())
{
  LOG_VART(_from.get());
  LOG_VART(_to.get());
  if (_from->getProjection()->IsGeographic() == false ||
      _to->getProjection()->IsGeographic() == false)
  {
    throw IllegalArgumentException(
      "The projections must both be geographic. Input 1: " +
      MapProjector::toWkt(_from->getProjection()) + ", Input 2: " +
      MapProjector::toWkt(_to->getProjection()));
  }

  _changesByType.clear();
  _changesByType[Change::ChangeType::Create] = 0;
  _changesByType[Change::ChangeType::Modify] = 0;
  _changesByType[Change::ChangeType::Delete] = 0;

  LOG_VART(_allowDeletingReferenceFeatures);
}

ChangesetDeriver::~ChangesetDeriver()
{
  close();
}

std::shared_ptr<OGRSpatialReference> ChangesetDeriver::getProjection() const
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
  if (!_next.getElement().get())
  {
    _next = _nextChange();
  }
  return _next.getElement().get();
}

Change ChangesetDeriver::_nextChange()
{
  // TODO: this method is a bit of a mess now...refactor into smaller chunks

  LOG_TRACE("Reading next change...");

  Change result;

  LOG_VART(_fromE.get());
  LOG_VART(_from->hasMoreElements());
  LOG_VART(_toE.get());
  LOG_VART(_to->hasMoreElements());

  if (!_fromE && _from->hasMoreElements())
  {
    LOG_TRACE("'from' element null and 'from' has more elements; reading next 'from' element...");
    if (Log::getInstance().getLevel() <= Log::Trace && !_from->hasMoreElements())
    {
      LOG_TRACE("Last from element: " << _fromE->getElementId());
    }
    _fromE = _from->readNextElement();
    _numFromElementsParsed++;
    LOG_TRACE("Read next 'from' element: " << _fromE->getElementId());
  }
  if (!_toE && _to->hasMoreElements())
  {
    LOG_TRACE("'to' element null and 'to' has more elements; reading next 'to' element...");
    if (Log::getInstance().getLevel() <= Log::Trace && !_to->hasMoreElements())
    {
      LOG_TRACE("Last to element: " << _toE->getElementId());
    }
    _toE = _to->readNextElement();
    _numToElementsParsed++;
    LOG_TRACE("Read next 'to' element: " << _toE->getElementId());
  }

  // If we've run out of "from" elements, create all the remaining elements in "to".
  if (!_fromE && _toE)
  {
    LOG_TRACE(
      "run out of from elements; 'from' element null; 'to' element not null: " <<
      _toE->getElementId() << "; creating 'to' element...");

    if (Log::getInstance().getLevel() <= Log::Trace && !_to->hasMoreElements())
    {
      LOG_TRACE("Last to element: " << _toE->getElementId());
    }

    result = Change(Change::Create, _toE);

    if (_to->hasMoreElements())
    {
      _toE = _to->readNextElement();
    }
    else
    {
      _toE.reset();
    }
    if (_toE)
    {
      _numToElementsParsed++;
      LOG_TRACE("Next 'to' element: " << _toE->getElementId());
    }
  }
  // If we've run out of "to" elements, delete all the remaining elements in "from".
  else if (_fromE && !_toE)
  { 
    if (Log::getInstance().getLevel() <= Log::Trace && !_from->hasMoreElements())
    {
      LOG_TRACE("Last from element: " << _fromE->getElementId());
    }

    if (_allowDeletingReferenceFeatures &&
        !_fromE->getTags().contains(MetadataTags::HootChangeExcludeDelete()))
    {
      LOG_TRACE(
        "run out of 'to' elements; to' element null; 'from' element not null: " <<
        _fromE->getElementId() << "; deleting 'from' element...");
      result = Change(Change::Delete, _fromE);
    }
    else
    {
      // See similar note below where the 'from' element ID is less than the 'to' element ID.
      LOG_TRACE(
        "Skipping delete on unknown1 'from' element " << _fromE->getElementId() <<
        " due to " << ConfigOptions::getChangesetAllowDeletingReferenceFeaturesKey() <<
        "=false or inclusion of " << MetadataTags::HootChangeExcludeDelete() << " tag...");
      result = Change(Change::Unknown, _fromE);
    }

    if (_from->hasMoreElements())
    {
      _fromE = _from->readNextElement();
    }
    else
    {
      _fromE.reset();
    }
    if (_fromE)
    {
      _numFromElementsParsed++;
      LOG_TRACE("Next 'from' element: " << _fromE->getElementId());
    }
  }
  else
  {
    // While the elements are exactly the same, there is nothing to do.
    while (_fromE && _toE && _fromE->getElementId() == _toE->getElementId() &&
           _elementComparer.isSame(_fromE, _toE) &&
           // ElementComparer always ignores metadata tags during comparison. So, if there is a
           // specific metadata tag in the target element that isn't in the original element and we
           // want to allow in the changeset output, this allows that to happen.
           !(!_fromE->getTags().hasAnyKey(_metadataAllowKeys) &&
             _toE->getTags().hasAnyKey(_metadataAllowKeys)))
    {
      LOG_TRACE(
        "skipping identical elements - 'from' element: " << _fromE->getElementId() <<
        " and 'to' element: " << _toE->getElementId() << "...");

      if (Log::getInstance().getLevel() <= Log::Trace && !_from->hasMoreElements())
      {
        LOG_TRACE("Last from element: " << _fromE->getElementId());
      }
      if (Log::getInstance().getLevel() <= Log::Trace && !_to->hasMoreElements())
      {
        LOG_TRACE("Last to element: " << _toE->getElementId());
      }

      if (_to->hasMoreElements())
      {
        _toE = _to->readNextElement();
      }
      else
      {
        _toE.reset();
      }
      if (_toE)
      { 
        _numToElementsParsed++;
        LOG_TRACE("Next 'to' element: " << _toE->getElementId());
      }

      if (_from->hasMoreElements())
      {
        _fromE = _from->readNextElement();
      }
      else
      {
        _fromE.reset();
      }
      if (_fromE)
      {
        _numFromElementsParsed++;
        LOG_TRACE("Next 'from' element: " << _fromE->getElementId());
      }
    }

    if (!_fromE && !_toE)
    {
      // pass
      LOG_TRACE("both are null elements; skipping...");
    }
    // If we've run out of "from" elements, create all the remaining elements in "to".
    else if (!_fromE && _toE)
    {
      LOG_TRACE(
        "run out of from elements; 'from' element null; 'to' element not null: " <<
        _toE->getElementId() << "; creating 'to' element...");

      if (Log::getInstance().getLevel() <= Log::Trace && !_to->hasMoreElements())
      {
        LOG_TRACE("Last to element: " << _toE->getElementId());
      }

      result = Change(Change::Create, _toE);

      if (_to->hasMoreElements())
      {
        _toE = _to->readNextElement();
      }
      else
      {
        _toE.reset();
      }
      if (_toE)
      {
        _numToElementsParsed++;
        LOG_TRACE("Next 'to' element: " << _toE->getElementId());
      }
    }
    // If we've run out of "to" elements, delete all the remaining elements in "from".
    else if (_fromE && !_toE)
    {
      if (Log::getInstance().getLevel() <= Log::Trace && !_from->hasMoreElements())
      {
        LOG_TRACE("Last from element: " << _fromE->getElementId());
      }

      if (_allowDeletingReferenceFeatures &&
          !_fromE->getTags().contains(MetadataTags::HootChangeExcludeDelete()))
      {
        LOG_TRACE(
          "run out of 'to' elements; to' element null; 'from' element not null: " <<
          _fromE->getElementId() << "; deleting 'from' element...");
        result = Change(Change::Delete, _fromE);
      }
      else
      {
        // See similar note below where the 'from' element ID is less than the 'to' element ID.
        LOG_TRACE(
          "Skipping delete on unknown1 'from' element " << _fromE->getElementId() <<
          " due to " << ConfigOptions::getChangesetAllowDeletingReferenceFeaturesKey() <<
          "=false or inclusion of " << MetadataTags::HootChangeExcludeDelete() << " tag...");
        result = Change(Change::Unknown, _fromE);
      }

      if (_from->hasMoreElements())
      {
        _fromE = _from->readNextElement();
      }
      else
      {
        _fromE.reset();
      }
      if (_fromE)
      {
        _numFromElementsParsed++;
        LOG_TRACE("Next 'from' element: " << _fromE->getElementId());
      }
    }
    else if (_fromE->getElementId() == _toE->getElementId())
    {
      LOG_TRACE(
        "'from' element id: " << _fromE->getElementId() << " equals 'to' element id: " <<
        _toE->getElementId() << " modifying 'to' element: ");

      if (Log::getInstance().getLevel() <= Log::Trace && !_from->hasMoreElements())
      {
        LOG_TRACE("Last from element: " << _fromE->getElementId());
      }
      if (Log::getInstance().getLevel() <= Log::Trace && !_to->hasMoreElements())
      {
        LOG_TRACE("Last to element: " << _toE->getElementId());
      }

      result = Change(Change::Modify, _toE, _fromE);

      if (_to->hasMoreElements())
      {
        _toE = _to->readNextElement();
      }
      else
      {
        _toE.reset();
      }
      if (_toE)
      {
        _numToElementsParsed++;
        LOG_TRACE("Next 'to' element: " << _toE->getElementId());
      }

      if (_from->hasMoreElements())
      {
        _fromE = _from->readNextElement();
      }
      else
      {
        _fromE.reset();
      }
      if (_fromE)
      {
        _numFromElementsParsed++;
        LOG_TRACE("Next 'from' element: " << _fromE->getElementId());
      }
    }
    else if (_fromE->getElementId() < _toE->getElementId())
    {
      if ((_allowDeletingReferenceFeatures ||
          // this assumes the 'from' dataset was loaded as unknown1
          // TODO: Don't understand the use case for this and its been around for awhile now...need
          // to define and add a test
          (!_allowDeletingReferenceFeatures && _fromE->getStatus() != Status::Unknown1)) &&
          !_fromE->getTags().contains(MetadataTags::HootChangeExcludeDelete()))
      {
        LOG_TRACE(
          "'from' element id: " << _fromE->getElementId() << " less than 'to' element id: " <<
          _toE->getElementId() << "; deleting 'from' element...");
        result = Change(Change::Delete, _fromE);
      }
      else
      {
        // The changeset provider will return no more changes if a null change is returned here.  We
        // want to force no changes for this particular element, so we're going to use the unknown
        // change type, which ends up being a no-op.  Skipping an element delete in this situation
        // minimizes the changeset impact on reference datasets in certain situations.
        LOG_TRACE(
          "Skipping delete on unknown1 'from' element " << _fromE->getElementId() <<
          " due to " << ConfigOptions::getChangesetAllowDeletingReferenceFeaturesKey() <<
          "=false or inclusion of " << MetadataTags::HootChangeExcludeDelete() << " tag...");
        result = Change(Change::Unknown, _fromE);
      }

      if (_from->hasMoreElements())
      {
        _fromE = _from->readNextElement();
      }
      else
      {
        _fromE.reset();
      }
      if (_fromE)
      {
        _numFromElementsParsed++;
        LOG_TRACE("Next 'from' element: " << _fromE->getElementId());
      }
    }
    else
    {
      LOG_TRACE(
        "'from' element id: " << _fromE->getElementId() << " greater than 'to' element id: " <<
        _toE->getElementId() << "; creating 'to' element...");

      if (Log::getInstance().getLevel() <= Log::Trace && !_to->hasMoreElements())
      {
        LOG_TRACE("Last to element: " << _toE->getElementId());
      }

      result = Change(Change::Create, _toE);

      if (_to->hasMoreElements())
      {
        _toE = _to->readNextElement();
      }
      else
      {
        _toE.reset();
      }
      if (_toE)
      {
        _numToElementsParsed++;
        LOG_TRACE("Next 'to' element: " << _toE->getElementId());
      }
    }
  }

  LOG_VART(result);
  return result;
}

Change ChangesetDeriver::readNextChange()
{
  if (!_next.getElement())
  {
    _next = _nextChange();
  }

  Change result = _next;
  _changesByType[result.getType()]++;
  _next.clearElement();
  return result;
}

}
