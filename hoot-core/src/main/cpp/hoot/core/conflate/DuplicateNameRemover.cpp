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

#include "DuplicateNameRemover.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QDebug>
#include <QSet>
#include <QStringList>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{
  using namespace std;

HOOT_FACTORY_REGISTER(OsmMapOperation, DuplicateNameRemover)

DuplicateNameRemover::DuplicateNameRemover() :
_caseSensitive(true)
{
  setCaseSensitive(ConfigOptions().getDuplicateNameCaseSensitive());
}

void DuplicateNameRemover::apply(boost::shared_ptr<OsmMap>& map)
{
  _map = map;

  WayMap wm = _map->getWays();
  // go through each way
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); it++)
  {
    const boost::shared_ptr<Way>& w = it->second;

    QStringList list = w->getTags().getNames();
    // put all the alt_name values in a set, this will remove duplicates.
    list.append(w->getTags().getList("alt_name"));

    QStringList list2 = list;
    list.clear();
    for (int i = 0; i < list2.size(); i++)
    {
      if (list2[i].isEmpty() == false)
      {
        list.append(list2[i]);
      }
    }

    QStringList filtered;
    for (int i = 0; i < list.size(); i++)
    {
      bool done = false;
      for (int j = 0; j < filtered.size() && done == false; j++)
      {
        const Qt::CaseSensitivity caseSensitivity =
          _caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
        if (filtered[j].compare(list[i], caseSensitivity) == 0)
        {
          filtered[j] = _getBestName(filtered[j], list[i]);
          done = true;
        }
      }

      if (done == false)
      {
        filtered.append(list[i]);
      }
    }

    if (filtered.size() > 0)
    {
      if (filtered.size() != list.size())
      {
        w->getTags().insert("name", filtered[0]);
        filtered.pop_front();
        // If there are additional names, put them in alt_name
        if (filtered.size() > 0)
        {
          w->getTags().insert("alt_name", QStringList(filtered).join(";"));
        }
      }
    }
    else
    {
      w->getTags().remove("alt_name");
    }
  }
}

QString DuplicateNameRemover::_getBestName(QString n1, QString n2)
{
  // Here we assume that a name that has mixed case is better than all upper or all lower. E.g.
  // Walnut Street (better)
  // WALNUT STREET (worse)

  int s1 = 0;
  int s2 = 0;

  // if n1 contains upper case letters.
  if (n1.toLower() != n1)
  {
    s1++;
  }
  // if n1 contains lower case letters.
  if (n1.toUpper() != n1)
  {
    s1++;
  }

  // if n2 contains upper case letters.
  if (n2.toLower() != n2)
  {
    s2++;
  }
  // if n2 contains lower case letters.
  if (n2.toUpper() != n2)
  {
    s2++;
  }

  if (s2 > s1)
  {
    return n2;
  }
  else
  {
    return n1;
  }
}

void DuplicateNameRemover::removeDuplicates(boost::shared_ptr<OsmMap> map)
{
  DuplicateNameRemover a;
  a.apply(map);
}

}
