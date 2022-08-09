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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MERGECREATOR_H
#define MERGECREATOR_H

// hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchSet.h>
#include <hoot/core/conflate/merging/Merger.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/info/CreatorDescription.h>

namespace hoot
{

/**
 * Abstract base class for creating conflation feature mergers
 */
class MergerCreator
{
public:

  static QString className() { return "MergerCreator"; }

  MergerCreator() = default;
  virtual ~MergerCreator() = default;

  virtual QString toString() { return className(); }

  /**
   * Either creates one or more mergers that can merge all the matches in the
   * provided set, or does nothing.
   *
   * @param matches Set of matches that should evaluated to look for mergers.
   * @param mergers Vector of mergers that will contain the new mergers created. Newly created
   * mergers should only be added and no elements in the vector should be modified.
   * @return Returns true if one or more mergers were created and added to the provided mergers
   * vector.
   */
  virtual bool createMergers(const MatchSet& matches, std::vector<MergerPtr>& mergers) const = 0;

  /**
   * Generally this just returns the class name of this creator. However, creators that take
   * arguments to specify scripts such as the ScriptMatchCreator may return multiple results.
   */
  virtual std::vector<CreatorDescription> getAllCreators() const = 0;

  /**
   * Returns true if m1 and m2 are conflicting. If the MergerCreator has no information on the two
   * input matches then false is returned.
   *
   * @param map map owning the elements involved in the matches
   * @param m1 the first match
   * @param m2 the second match
   * @param matches an optional set of all matches found during conflation; this allows match
   * caching to be used in situations where duplicated match calculation is prohibitively expensive
   * @return true if the two matches are conflicting; false otherwise
   */
  virtual bool isConflicting(
    const ConstOsmMapPtr& map, ConstMatchPtr m1, ConstMatchPtr m2,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const = 0;

  virtual void setArguments(QStringList /*args*/) { throw IllegalArgumentException(); }
};

using MergerCreatorPtr = std::shared_ptr<MergerCreator>;
using ConstMergerCreatorPtr = std::shared_ptr<const MergerCreator>;

}

#endif // MERGECREATOR_H
