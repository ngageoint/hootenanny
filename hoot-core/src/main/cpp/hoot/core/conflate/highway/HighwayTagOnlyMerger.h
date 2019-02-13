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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HIGHWAY_TAG_ONLY_MERGER_H
#define HIGHWAY_TAG_ONLY_MERGER_H

// Hoot
#include <hoot/core/conflate/highway/HighwayMergerAbstract.h>

namespace hoot
{

/**
 * Merges road tags only, keeping ref1 tags
 */
class HighwayTagOnlyMerger : public HighwayMergerAbstract
{

public:

  static std::string className() { return "hoot::HighwayTagOnlyMerger"; }

  HighwayTagOnlyMerger(const std::set<std::pair<ElementId, ElementId>>& pairs);

  virtual void apply(const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced) override;

protected:

  virtual bool _mergePair(
    const OsmMapPtr& map, ElementId eid1, ElementId eid2,
    std::vector<std::pair<ElementId, ElementId>>& replaced) override;
};

typedef boost::shared_ptr<HighwayTagOnlyMerger> HighwayTagOnlyMergerPtr;

}

#endif // HIGHWAY_TAG_ONLY_MERGER_H
