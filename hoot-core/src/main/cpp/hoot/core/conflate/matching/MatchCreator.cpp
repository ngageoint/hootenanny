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
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */

#include "MatchCreator.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/OsmMap.h>

#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/criterion/ChainCriterion.h>

namespace hoot
{

MatchCreator::MatchCreator() :
_boundsAddedToFilter(false)
{
}

void MatchCreator::createMatches(
  const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& /*matches*/,
  ConstMatchThresholdPtr /*threshold*/)
{    
  if (_filter)
  {
    // We weren't able to set the map when this filter was created by MatchFactory b/c we didn't
    // have one yet.
    std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
      std::dynamic_pointer_cast<ConstOsmMapConsumer>(_filter);
    if (mapConsumer)
    {
      mapConsumer->setOsmMap(map.get());
    }
  }

  if (!_boundsAddedToFilter && ConfigUtils::boundsOptionEnabled())
  {
    // The default behavior is to match against everything in the input unless a bounds is
    // specified. Even though data data readers honor the bounds option, we still need to perform
    // a bounds check during matching. This is due to the fact that when data is read in by bounds
    // from an API database query, for example, the default behavior is to return parent relations
    // and all their relation members for any data within the bounds (fully hydrated relations).
    // Without further geo filtering of such data, conflation may undesirably modify features
    // outside of the conflate bounds and encounter increased runtime due to conflation of extra
    // features. Turning off relation member hydration or trying to crop such data after its read
    // can lead to malformed relations in the conflated output. So, we set up a filter here to
    // perform a per features bounds check beyond the one done by the reader that populated the
    // input data. Input from certain readers actually does not fully hydrate relations, which makes
    // this check unnecessary. However, the check is done automatically anyway to keep the logic
    // simple and not tie it to input source type. The check really shouldn't slow things down too
    // much, though, if at all.

    // This geo filter must be initialized here instead of in MatchFactory when this is initialized,
    // or in the constructor b/c a map is needed by it.

    ElementCriterionPtr boundsFilter = ConfigUtils::getBoundsFilter(map);
    if (!_filter)
    {
      _filter = boundsFilter;
    }
    else
    {
      ElementCriterionPtr existingFilter = _filter->clone();
      _filter = std::make_shared<ChainCriterion>(existingFilter, boundsFilter);
    }
    _boundsAddedToFilter = true;
  }
}

}

