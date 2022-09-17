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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "DuplicateNameRemover.h"

// Hoot
#include <hoot/core/conflate/ConflateUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

// TGS
#include <tgs/StreamUtils.h>

using namespace Tgs;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, DuplicateNameRemover)

DuplicateNameRemover::DuplicateNameRemover()
  : _caseSensitive(true),
    _preserveOriginalName(false)
{
  setConfiguration(conf());
}

void DuplicateNameRemover::setConfiguration(const Settings& conf)
{
  ConfigOptions opts = ConfigOptions(conf);
  setCaseSensitive(opts.getDuplicateNameCaseSensitive());
  _preserveOriginalName = opts.getDuplicateNamePreserveOriginalName();
}

void DuplicateNameRemover::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;
  _map = map;

  const WayMap& wm = _map->getWays();
  for (auto it = wm.begin(); it != wm.end(); ++it)
  {
    const WayPtr& w = it->second;
    _numProcessed++;
    if (!w)
      continue;
    // Since this class operates on elements with generic types, an additional check must be
    // performed here during conflation to enure we don't modify any element not associated with
    // an active conflate matcher in the current conflation configuration.
    else if (_conflateInfoCache && !_conflateInfoCache->elementCanBeConflatedByActiveMatcher(w, className()))
    {
      LOG_TRACE(
        "Skipping processing of " << w->getElementId() << ", as it cannot be conflated by any " <<
        "actively configured conflate matcher.");
      continue;
    }
    LOG_VART(w->getElementId());

    // If we have a name that is not an alt name, let's record it here so we can preserve it
    // as the main name later on.
    const QStringList tagNameKeys = Tags::getNameKeys(w->getTags());
    LOG_VART(tagNameKeys);
    QString name = w->getTags().getName().trimmed();
    LOG_VART(name);
    if (w->getTags().get("alt_name").toLower().contains(name.toLower()))
      name = "";

    QStringList list = w->getTags().getNames();
    // add in alt names
    list.append(w->getTags().getList("alt_name"));
    LOG_VART(list);

    // remove empty names
    QStringList list2 = list;
    list.clear();
    for (const auto& name : qAsConst(list2))
    {
      if (!name.isEmpty())
        list.append(name);
    }
    LOG_VART(list);

    const Qt::CaseSensitivity caseSensitivity = _caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    // filter on case sensitivity and "best name"
    QStringList filtered;
    for (const auto& name : qAsConst(list))
    {
      bool done = false;
      for (int j = 0; j < filtered.size(); j++)
      {
        if (filtered[j].compare(name, caseSensitivity) == 0)
        {
          LOG_VART(name);
          filtered[j] = _getBestName(filtered[j], name);
          LOG_VART(filtered[j]);
          done = true;
          break;
        }
      }

      if (done == false)
        filtered.append(name);
    }
    LOG_VART(filtered);

    _numAffected = list.size() - filtered.size();

    if (!filtered.empty())
    {
      if (filtered.size() != list.size())
      {
        LOG_VART(w->getTags());
        // preserve the original "name"
        if (_preserveOriginalName && !name.isEmpty() && filtered[0].compare(name, caseSensitivity) != 0)
          filtered.removeAll(name);
        else
        {
          w->getTags().insert("name", filtered[0]);
          filtered.pop_front();
        }
        LOG_VART(filtered);
        // If there are additional names, put them in alt_name.
        if (!filtered.empty())
          w->getTags().insert("alt_name", QStringList(filtered).join(";"));
      }
    }
    else
      w->getTags().remove("alt_name");

    LOG_VART(w);
  }
}

QString DuplicateNameRemover::_getBestName(const QString& n1, const QString& n2) const
{
  // Here we assume that a name that has mixed case is better than all upper or all lower. E.g.
  // Walnut Street (better)
  // WALNUT STREET (worse)

  int s1 = 0;
  int s2 = 0;

  // if n1 contains upper case letters.
  if (n1.toLower() != n1)
    s1++;
  // if n1 contains lower case letters.
  if (n1.toUpper() != n1)
    s1++;

  // if n2 contains upper case letters.
  if (n2.toLower() != n2)
    s2++;
  // if n2 contains lower case letters.
  if (n2.toUpper() != n2)
    s2++;

  if (s2 > s1)
    return n2;
  else
    return n1;
}

void DuplicateNameRemover::removeDuplicates(std::shared_ptr<OsmMap> map)
{
  DuplicateNameRemover a;
  a.apply(map);
}

}
