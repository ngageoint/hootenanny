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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENT_DATA_H
#define ELEMENT_DATA_H

// Hoot
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

/**
 * Base class for OSM element data -- node, way and relation.
 *
 * The ElementData can be shared between multiple copies of an Element. The ElementData will be
 * copied on write. This doesn't save a lot of memory/time, but every little bit helps.
 */
class ElementData
{

public:

  static long CHANGESET_EMPTY;
  static long VERSION_EMPTY;
  static quint64 TIMESTAMP_EMPTY;
  static QString USER_EMPTY;
  static long UID_EMPTY;
  static bool VISIBLE_EMPTY;
  static double CIRCULAR_ERROR_EMPTY;

  virtual ~ElementData() = default;

  virtual void clear() { _tags.clear(); }

  bool hasCircularError() const { return _circularError >= 0; }

  Meters getCircularError() const
  { return _circularError >= 0 ? _circularError : _defaultCircularError; }
  long getId() const { return _id; }
  long getChangeset() const { return _changeset; }
  Meters getRawCircularError() const { return _circularError; }
  long getVersion() const { return _version; }
  quint64 getTimestamp() const { return _timestamp; }
  QString getUser() const { return _user; }
  long getUid() const { return _uid; }
  const Tags& getTags() const { return _tags; }
  Tags& getTags() { return _tags; }
  bool getVisible() const { return _visible; }


  void setId(long id) { _id = id; }
  void setChangeset(long changeset) { _changeset = changeset; }
  void setVersion(long version) { _version = version; }
  void setTimestamp(quint64 timestamp) { _timestamp = timestamp; }
  void setUser(QString user) { _user = user; }
  void setUid(long uid) { _uid = uid; }
  void setCircularError(Meters circularError) { _circularError = circularError; }
  void setTags(const Tags& tags) { _tags = tags; }
  void setTag(QString k, QString v) { _tags.insert(k, v); }
  void addTags(const Tags& tags) { _tags.add(tags); }
  void setVisible(bool visible) { _visible = visible; }

protected:

  // Please don't add any additional constructors. Multiple constructors has lead to a large number
  // of errors in the past. If you need more parameters please just add them to the end with a
  // sensible default value.
  ElementData(long id = LLONG_MIN, const Tags& tags = Tags(), Meters circularError = CIRCULAR_ERROR_EMPTY,
              long changeset = ElementData::CHANGESET_EMPTY, long version = ElementData::VERSION_EMPTY,
              quint64 timestamp = ElementData::TIMESTAMP_EMPTY, QString user = ElementData::USER_EMPTY,
              long uid = ElementData::UID_EMPTY, bool visible = ElementData::VISIBLE_EMPTY);

  long _id;
  Tags _tags;
  Meters _circularError;
  long _changeset;
  long _version;
  quint64 _timestamp;
  QString _user;
  long _uid;
  bool _visible;

private:

  // This needs to be a mem var vs being static or it can't be read from a config that's passed in
  // from the command line.
  Meters _defaultCircularError;
};

inline ElementData::ElementData(long id, const Tags& tags, Meters circularError, long changeset, long version, quint64 timestamp,
                                QString user, long uid, bool visible)
  : _id(id),
    _tags(tags),
    _circularError(circularError),
    _changeset(changeset),
    _version(version),
    _timestamp(timestamp),
    _user(user),
    _uid(uid),
    _visible(visible),
    _defaultCircularError(ConfigOptions().getCircularErrorDefaultValue())
{
}

}

#endif // ELEMENT_H
