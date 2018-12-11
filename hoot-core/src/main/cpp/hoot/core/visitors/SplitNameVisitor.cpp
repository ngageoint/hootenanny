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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SplitNameVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, SplitNameVisitor)

SplitNameVisitor::SplitNameVisitor() : _maxSize(255)
{
}

void SplitNameVisitor::_addExtraNames(Tags& t, const QStringList& extraNames)
{
  int lastNameId = -1;
  int size = 0;
  QStringList names;

  for (int i = 0; i < extraNames.size(); i++)
  {
    int thisSize = extraNames[i].size();
    if (size + thisSize > _maxSize)
    {
      lastNameId = _getNextNameId(t, lastNameId);
      QString k = QString("name:%1").arg(lastNameId);
      t.setList(k, names);
      names.clear();
      size = 0;
    }

    names.append(extraNames[i]);
    size += thisSize + 1;
  }

  if (names.size() > 0)
  {
    lastNameId = _getNextNameId(t, lastNameId);
    QString k = QString("name:%1").arg(lastNameId);
    t.setList(k, names);
  }
}

int SplitNameVisitor::_getNextNameId(const Tags& t, int lastId)
{
  for (int i = lastId + 1; i < lastId + 100; i++)
  {
    QString k = QString("name:%1").arg(i);

    if (t.contains(k) == false)
    {
      return i;
    }
  }

  throw InternalErrorException("Unable to find a valid key for a new extra name.");
}

QStringList SplitNameVisitor::_splitNames(const QString& v, QStringList& extras)
{
  QStringList result;
  QStringList allNames = Tags::split(v);

  int size = 0;

  int i = 0;
  for (; i < allNames.size(); i++)
  {
    if (allNames[i].size() > _maxSize)
    {
      allNames[i] = allNames[i].left(_maxSize - 3) + "...";
    }

    int thisSize = allNames[i].size();
    if (size + thisSize <= _maxSize)
    {
      result.append(allNames[i]);
      size += thisSize + 1;
    }
    else
    {
      break;
    }
  }

  for (; i < allNames.size(); i++)
  {
    if (allNames[i].size() > _maxSize)
    {
      allNames[i] = allNames[i].left(_maxSize - 3) + "...";
    }
    extras.append(allNames[i]);
  }

  return result;
}


void SplitNameVisitor::visit(const boost::shared_ptr<Element>& e)
{
  Tags& t = e->getTags();

  QStringList extraNames;

  Tags copy = t;
  for (Tags::const_iterator it = copy.begin(); it != copy.end(); ++it)
  {
    const QString& k = it.key();
    const QString& v = it.value();
    if (v.size() > _maxSize &&
        OsmSchema::getInstance().getCategories(it.key()).intersects(OsmSchemaCategory::name()))
    {
      QStringList l = _splitNames(v, extraNames);
      t.setList(k, l);
    }
  }

  _addExtraNames(t, extraNames);
}

}
