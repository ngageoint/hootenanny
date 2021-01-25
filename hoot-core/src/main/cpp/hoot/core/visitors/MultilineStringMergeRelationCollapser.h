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
 * @copyright Copyright (C) 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef MULTILINESTRING_MERGE_RELATION_COLLAPSER_H
#define MULTILINESTRING_MERGE_RELATION_COLLAPSER_H

// Hoot
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/MultipleCriterionConsumerVisitor.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/conflate/merging/RelationMerger.h>

namespace hoot
{

/**
 * Collapses multilinestring relations created during conflate merging and passing a filter by
 * removing them and copying their type tags to members.
 *
 * These relations can be unwanted byproducts of merging (see LinearSnapMerger) in certain
 * situations. In some cases, its easier to remove them using this as a conflate post op than it is
 * to not create them in the first place due to the complexity of the merging logic.
 */
class MultilineStringMergeRelationCollapser : public MultipleCriterionConsumerVisitor,
  public OsmMapConsumer, public Configurable
{
public:

  static QString className() { return "hoot::MultilineStringMergeRelationCollapser"; }

  MultilineStringMergeRelationCollapser();
  virtual ~MultilineStringMergeRelationCollapser() = default;

  /**
   * @see ElementVisitor
   */
  virtual void visit(const ElementPtr& e);

  /**
   * @see OsmMapConsumer
   */
  virtual void setOsmMap(OsmMap* map);

  virtual QString getInitStatusMessage() const
  { return "Collapsing multilinestring merge relations..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Collapsed " + StringUtils::formatLargeNumber(_numAffected) + " relations and modified " +
      StringUtils::formatLargeNumber(_numRelationMembersModified) + " members.";
  }

  virtual QString getDescription() const
  { return "Collapses multilinestring relations created during conflation merging"; }

  virtual QString getName() const { return className(); }

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

private:

  OsmMapPtr _map;

  // a list of type keys to be removed from the parent relation and copied to its members
  QStringList _typeKeys;
  // a list of type key/value pairs to be removed from the parent relation and copied to its members
  QStringList _typeKvps;

  // merges the ms relation contained by other relation into them
  RelationMerger _relationMerger;

  // total number of relation members which had tags transferred to them
  int _numRelationMembersModified;
};

}

#endif // MULTILINESTRING_MERGE_RELATION_COLLAPSER_H
