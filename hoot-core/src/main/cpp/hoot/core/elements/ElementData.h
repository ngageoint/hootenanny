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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENT_DATA_H
#define ELEMENT_DATA_H

#include "Tags.h"

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

// Hoot
#include <hoot/core/Units.h>

// Standard
#include <set>

namespace hoot {

/**
 * Base class for OSM element data -- node, way and releation.
 */
class ElementData
{
public:

  virtual ~ElementData() {}

  virtual void clear() { _tags.clear(); }

  Meters getCircularError() const { return _circularError; }

  long getId() const { return _id; }

  long getChangeset() const { return _changeset; }

  long getVersion() const { return _version; }

  long getTimestamp() const { return _timestamp; }

  const Tags& getTags() const { return _tags; }

  Tags& getTags() { return _tags; }

  void setCircularError(Meters circularError) { _circularError = circularError; }

  void setTags(const Tags& tags) { _tags = tags; }

  void setTag(QString k, QString v) { _tags.insert(k, v); }

protected:

  ElementData() { }

  ElementData(long id) : _id(id), _changeset(0), _version(1), _timestamp(0) { }

  ElementData(long id, const Tags& tags, Meters circularError);

  ElementData(long id, long changeset, long version, unsigned int timestamp) :
    _id(id), _changeset(changeset), _version(version), _timestamp(timestamp) { }

  long _id;
  long _changeset;
  long _version;
  unsigned int _timestamp;
  Tags _tags;
  Meters _circularError;
};

}

#endif // ELEMENT_H
