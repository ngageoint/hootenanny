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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */
#ifndef ATTRIBUTEBUILDINGMERGER_H
#define ATTRIBUTEBUILDINGMERGER_H

// hoot
#include <hoot/core/conflate/merging/MergerBase.h>

namespace hoot
{

/**
 * Merges two or more buildings
 */
class AttributeBuildingMerger : public MergerBase
{
public:

  static QString className() { return "AttributeBuildingMerger"; }

  AttributeBuildingMerger() = default;
  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  explicit AttributeBuildingMerger(const std::set<std::pair<ElementId, ElementId>>& pairs);
  ~AttributeBuildingMerger() override = default;

  /**
   * @see Merger
   */
  void apply(const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  QString toString() const override;

  QString getDescription() const override { return "Merges building attributes"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  /** set to true if the current building merge involves two buildings, each part of multiple matches */
  bool _manyToManyMatch;

  Tags _getMergedTags(const ElementPtr& e1, const ElementPtr& e2);
};

}

#endif // ATTRIBUTEBUILDINGMERGER_H
