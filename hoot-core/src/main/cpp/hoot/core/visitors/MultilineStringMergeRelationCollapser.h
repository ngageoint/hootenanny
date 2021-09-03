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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
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
 * The option, conflate.mark.merge.created.multilinestring.relations, must be enabled in order for
 * the merged output from Mergers to be properly processed by this visitor. Also the option,
 * multilinestring.relation.collapser.types, must be populated with at least one type or key/value
 * pair.
 *
 * These relations can be unwanted byproducts of merging (see LinearSnapMerger) in certain
 * situations. In some cases, its easier to remove them using this as a conflate post op than it is
 * to not create them in the first place due to the complexity of the merging logic. Currently, this
 * is called by select conflate routines on an as-needed basis only and populated with only the
 * minimal feature types to be operated on. Alternatively, use of this class could be made automatic
 * and moved to LinearSnapMerger to be called directly from there.
 */
class MultilineStringMergeRelationCollapser : public MultipleCriterionConsumerVisitor,
  public OsmMapConsumer, public Configurable
{
public:

  static QString className() { return "MultilineStringMergeRelationCollapser"; }

  MultilineStringMergeRelationCollapser();
  ~MultilineStringMergeRelationCollapser() = default;

  /**
   * @see ElementVisitor
   */
  void visit(const ElementPtr& element) override;

  /**
   * @see OsmMapConsumer
   */
  void setOsmMap(OsmMap* map) override;

  QString getInitStatusMessage() const override
  { return "Collapsing multilinestring merge relations..."; }
  QString getCompletedStatusMessage() const override
  {
    return
      "Collapsed " + StringUtils::formatLargeNumber(_numAffected) + " relations and modified " +
      StringUtils::formatLargeNumber(_numRelationMembersModified) + " members.";
  }

  QString getDescription() const override
  { return "Collapses multilinestring relations created during conflation merging"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  bool hasTypes() const { return !_typeKeys.isEmpty() || !_typeKvps.isEmpty(); }

  void setTypes(const QStringList& types);

private:

  OsmMapPtr _map;

  // a list of type keys to be removed from the parent relation and copied to its members
  QStringList _typeKeys;
  // a list of type key/value pairs to be removed from the parent relation and copied to its members
  QStringList _typeKvps;
  // overrides use of _typesKeys/_typeKvps and allows relations of any type to be collapsed
  bool _mergeAllTypes;

  // merges the ms relation contained by other relation into them
  RelationMerger _relationMerger;

  // total number of relation members which had tags transferred to them
  int _numRelationMembersModified;

  QString _getMatchingTypeTag(const ConstElementPtr& element, bool& matchingTypeTagIsKey) const;
  bool _hasValidType(
    const ConstRelationPtr& relation, QString& matchingTypeTag, bool& matchingTypeTagIsKey) const;
};

}

#endif // MULTILINESTRING_MERGE_RELATION_COLLAPSER_H
