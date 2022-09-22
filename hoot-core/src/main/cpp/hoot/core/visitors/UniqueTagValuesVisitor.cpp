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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "UniqueTagValuesVisitor.h"

namespace hoot
{

// This isn't being factory registered, since there's no standard way to retrieve a set of strings
// from a visitor

UniqueTagValuesVisitor::UniqueTagValuesVisitor(QString key, std::set<QString>& bag, bool split)
  : _key(key),
    _bag(bag),
    _split(split)
{
}

void UniqueTagValuesVisitor::visit(const ConstElementPtr& e)
{
  auto it = e->getTags().find(_key);
  if (it != e->getTags().end())
  {
    if (_split)
    {
      QStringList l;
      e->getTags().readValues(_key, l);
      for (const auto& value : qAsConst(l))
        _bag.insert(value);
    }
    else
      _bag.insert(it.value());
  }
}

}
