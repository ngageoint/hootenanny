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

#include <QString>

namespace hoot
{

/**
 * Base class for OSM element data -- node, way and releation.
 */
class ElementData
{

public:

  static long CHANGESET_EMPTY;
  static long VERSION_EMPTY;
  static unsigned int TIMESTAMP_EMPTY;
  static QString USER_EMPTY;
  static long UID_EMPTY;

  virtual ~ElementData() {}

  virtual void clear() { _tags.clear(); }

  Meters getCircularError() const { return _circularError >= 0 ? _circularError : 15.0; }

  bool hasCircularError() const { return _circularError >= 0; }

  long getId() const { return _id; }

  long getChangeset() const { return _changeset; }

  Meters getRawCircularError() const { return _circularError; }

  long getVersion() const { return _version; }

  unsigned int getTimestamp() const { return _timestamp; }

  QString getUser() const { return _user; }

  long getUid() const { return _uid; }

  const Tags& getTags() const { return _tags; }

  Tags& getTags() { return _tags; }

  void setCircularError(Meters circularError) { _circularError = circularError; }

  void setTags(const Tags& tags) { _tags = tags; }

  void setTag(QString k, QString v) { _tags.insert(k, v); }

protected:

  ElementData();

  ElementData(long id, long changeset = ElementData::CHANGESET_EMPTY,
              long version = ElementData::VERSION_EMPTY,
              unsigned int timestamp = ElementData::TIMESTAMP_EMPTY,
              QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY);

  long _id;
  Tags _tags;
  Meters _circularError;
  long _changeset;
  long _version;
  unsigned int _timestamp;
  QString _user;
  long _uid;

};

}

#endif // ELEMENT_H
