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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "KeepTagsVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, KeepTagsVisitor)

KeepTagsVisitor::KeepTagsVisitor(const QStringList& keys) :
RemoveTagsVisitor(keys)
{
}

void KeepTagsVisitor::visit(const std::shared_ptr<Element>& e)
{
  // see if the element passes the filter (if there is one)
  if (_criterion.get() && !_criterion->isSatisfied(e))
  {
    return;
  }
  _numAffected++;

  // get a copy of the tags for modifying
  Tags tags;
  tags.add(e->getTags());
  for (Tags::const_iterator it = e->getTags().begin(); it != e->getTags().end(); ++it)
  {
    const QString key = it.key();
    bool removeTag = true;

    for (int i = 0; i < _keyRegexs.size(); i++)
    {
      const QRegExp regex = _keyRegexs.at(i);
      if (regex.exactMatch(key))
      {
        removeTag = false;
        break;
      }
    }

    if (removeTag)
    {
      _numTagsRemoved += tags.remove(key);
    }
  }
  e->setTags(tags);
}

}
