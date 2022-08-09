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
#include "SelectiveOverwriteTagMerger.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(TagMerger, SelectiveOverwriteTagMerger)
HOOT_FACTORY_REGISTER(TagMerger, SelectiveOverwriteTag1Merger)
HOOT_FACTORY_REGISTER(TagMerger, SelectiveOverwriteTag2Merger)

SelectiveOverwriteTagMerger::SelectiveOverwriteTagMerger(bool swap)
  : _swap(swap)
{
}

void SelectiveOverwriteTagMerger::setConfiguration(const Settings& conf)
{
  TagMerger::setConfiguration(conf);
  ConfigOptions config = ConfigOptions(conf);
  setTagKeys(config.getSelectiveOverwriteTagMergerKeys());
  setTagExcludeKeys(config.getSelectiveOverwriteTagMergerKeysExclude());
}

Tags SelectiveOverwriteTagMerger::mergeTags(const Tags& t1, const Tags& t2, ElementType /*et*/) const
{
  LOG_VART(_tagKeys);

  Tags tagsToOverwriteWith;
  Tags tagsToBeOverwritten;
  if (!_swap)
  {
    tagsToOverwriteWith = t1;
    tagsToBeOverwritten = t2;
  }
  else
  {
    tagsToOverwriteWith = t2;
    tagsToBeOverwritten = t1;
  }
  LOG_VART(tagsToOverwriteWith);
  LOG_VART(tagsToBeOverwritten);

  // Assume all keys to be transferred if the include list is empty. In that case, this ends up
  // having the same behavior as OverwriteTagMerger.
  if (_tagKeys.isEmpty())
    _tagKeys = tagsToOverwriteWith.toKeys();

  for (const auto& tagKey : qAsConst(_tagKeys))
  {
    LOG_VART(tagKey);
    if (tagsToOverwriteWith.contains(tagKey) && !_tagKeysExclude.contains(tagKey))
      tagsToBeOverwritten[tagKey] = tagsToOverwriteWith[tagKey];
  }

  LOG_TRACE("merged tags: " << tagsToBeOverwritten);
  return tagsToBeOverwritten;
}

}
