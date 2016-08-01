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

#include "Element.h"

namespace hoot
{

long ElementData::CHANGESET_EMPTY = 0;
long ElementData::VERSION_EMPTY = 0;
unsigned int ElementData::TIMESTAMP_EMPTY = 0;
QString ElementData::USER_EMPTY = "";
long ElementData::UID_EMPTY = 0;
bool ElementData::VISIBLE_EMPTY = true;

ElementData::ElementData(long id, const Tags& tags, Meters circularError, long changeset,
                         long version, unsigned int timestamp, QString user, long uid,
                         bool visible) :
  _id(id),
  _tags(tags),
  _circularError(circularError),
  _changeset(changeset),
  _version(version),
  _timestamp(timestamp),
  _user(user),
  _uid(uid),
  _visible(visible)
{
}

}
