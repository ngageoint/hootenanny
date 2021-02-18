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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ElementId.h"

// Hoot
#include <hoot/core/util/Log.h>

namespace hoot
{

ElementId::ElementId() :
_type(ElementType::Unknown),
_id(-std::numeric_limits<int>::max())
{
}

ElementId::ElementId(ElementType type, long id) :
_type(type), _id(id)
{
}

ElementId::ElementId(QString str)
{
  str = str.trimmed();
  const QString errorMsg = "Invalid element ID string: " + str;
  if (str.endsWith(")"))
  {
    // Way(1)

    const QStringList strParts = str.trimmed().split("(");
    if (strParts.size() != 2)
    {
      throw IllegalArgumentException(errorMsg);
    }

    _type = ElementType::fromString(strParts[0].toLower().trimmed());

    bool ok = false;
    _id = strParts[1].split(")")[0].trimmed().toLong(&ok);
    if (!ok)
    {
      throw IllegalArgumentException(errorMsg);
    }
  }
  else if (!str.isEmpty())
  {
    // way:1

    const QStringList strParts = str.trimmed().split(":");
    if (strParts.size() != 2)
    {
      throw IllegalArgumentException(errorMsg);
    }

    _type = ElementType::fromString(strParts[0].toLower().trimmed());

    bool ok = false;
    _id = strParts[1].trimmed().toLong(&ok);
    if (!ok)
    {
      throw IllegalArgumentException(errorMsg);
    }
  }
  else
  {
    throw IllegalArgumentException(errorMsg);
  }
}

bool ElementId::isNull() const
{
  return _type == ElementType::Unknown;
}

bool ElementId::operator!=(const ElementId& other) const
{
  return !(*this == other);
}

bool ElementId::operator==(const ElementId& other) const
{
  return getType() == other.getType() && getId() == other.getId();
}

bool ElementId::operator<(const ElementId& other) const
{
  if (getType().getEnum() < other.getType().getEnum())
  {
    return true;
  }
  else if (getType().getEnum() > other.getType().getEnum())
  {
    return false;
  }
  else
  {
    return getId() < other.getId();
  }
}

QString ElementId::toString() const
{
  // Now printing element ids as "(id)" rather than ":id" as they used to be. This makes it easier
  // to trace the events of a single feature when searching through log output w/o having to look
  // at features that you don't want. e.g. Searching through text for "Way:-1" in the past would
  // give you "Way:-1", "Way:-12".  Now, you can search for "Way(-1)" instead and not return
  // results for "Way(-12)".
  return getType().toString() + "(" + QString::number(getId()) + ")";
}

QString ElementId::toString()
{
  return const_cast<const ElementId*>(this)->toString();
}

}
