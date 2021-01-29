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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MULTIARYPOIMERGERCREATOR_H
#define MULTIARYPOIMERGERCREATOR_H

// hoot
#include <hoot/core/conflate/merging/MergerCreator.h>

namespace hoot
{

/**
 * Creates mergers for merging subgraphs of elements using clustering.
 *
 * WARNING: There is a good chance that this MergerCreator will not play well with others. It
 * certainly will not play well with the ScriptMergerCreator. We will need to give this significant
 * thought before we combine this with other conflation operations.
 *
 * WARNING: Do not use this with the MarkForReviewMergerCreator. By default the
 * MarkForReviewMergerCreator runs as the first MergerCreator. Careful configuration of the
 * UnifyingConflator must be done to omit the MarkForReviewMergerCreator.
 * @sa MultiaryPoiMergerCreatorTest
 *
 * This merger is very aggressive and will merge pretty much any set of matches that are passed
 * in. This should generally be the last merger. The merger will also happily consume reviews
 * between elements. These reviews are a relevant aspect of the clustering.
 *
 * @sa MultiaryPoiMerger
 * @todo Should this inherit from MergerBase instead?
 */
class MultiaryPoiMergerCreator : public MergerCreator
{
public:
  static QString className() { return "hoot::MultiaryPoiMergerCreator"; }

  MultiaryPoiMergerCreator() = default;
  virtual ~MultiaryPoiMergerCreator() = default;

  /**
   * This merger is very aggressive and will merge pretty much any set of matches that are passed
   * in. This should generally be the last merger.
   */
  virtual bool createMergers(const MatchSet& matches, std::vector<MergerPtr>& mergers) const override;

  /**
   * Returns a description of this merger creator.
   */
  virtual std::vector<CreatorDescription> getAllCreators() const override;

  virtual bool isConflicting(
    const ConstOsmMapPtr& map, ConstMatchPtr m1, ConstMatchPtr m2,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const;
};

}

#endif // MULTIARYPOIMERGERCREATOR_H
