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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MERGE_H
#define MERGE_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/info/ApiEntityInfo.h>

// Standard
#include <vector>

namespace hoot
{

/**
 * Merges features together after matching. Mergers are created by the MergerFactory.
 */
class Merger : public ApiEntityInfo
{
public:

  static QString className() { return "hoot::Merger"; }

  Merger() = default;
  virtual ~Merger() = default;

  /**
   * Applies the merge to the input map. Any way that is significantly modified should get a new ID.
   *
   * In some cases one element will be replaced by multiple smaller elements. If this happens then
   * create a relation element to contain the smaller pieces. (e.g. multipolygon or multilinestring)
   *
   * @param replaced A deque of all element Ids that were replaced as part of this Merger operation.
   */
  virtual void apply(const OsmMapPtr& map,
                     std::vector<std::pair<ElementId, ElementId>>& replaced) = 0;

  /**
   * Returns all the element ids that are impacted by this merger operation.
   */
  virtual std::set<ElementId> getImpactedElementIds() const = 0;

  /**
   * Returns true if this merge can be applied to the specified map.
   */
  virtual bool isValid(const ConstOsmMapPtr& map) const = 0;

  /**
   * Replaces an old element id with a new element id. This is sometimes required as other mergers
   * may be manipulating elements that this merger relies on.
   */
  virtual void replace(ElementId oldEid, ElementId newEid) = 0;

  virtual QString toString() const = 0;
};

typedef std::shared_ptr<Merger> MergerPtr;
typedef std::shared_ptr<const Merger> ConstMergerPtr;

}

#endif // MERGE_H
