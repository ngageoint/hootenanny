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
#ifndef COPYMAPSUBSETOP_H
#define COPYMAPSUBSETOP_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Copies a subset of the map into a new map. The old map is unchanged.
 */
class CopyMapSubsetOp : public OsmMapOperation, public ConstOsmMapConsumer,
  public ElementCriterionConsumer, public Configurable
{
public:

  static QString className() { return "hoot::CopyMapSubsetOp"; }

  CopyMapSubsetOp();
  CopyMapSubsetOp(const ConstOsmMapPtr& from, const std::set<ElementId>& eids);
  CopyMapSubsetOp(const ConstOsmMapPtr& from, const std::vector<long>& wayIds);
  CopyMapSubsetOp(const ConstOsmMapPtr& from, ElementId eid);
  CopyMapSubsetOp(const ConstOsmMapPtr& from, ElementId eid1, ElementId eid2);
  CopyMapSubsetOp(const ConstOsmMapPtr& from, const ElementCriterionPtr& crit);
  ~CopyMapSubsetOp() = default;

  /**
   * A new map is created and the eids specified in the constructor and their dependencies will be
   * copied into the new map. The map will be set to point to the new map.
   */
  void apply(OsmMapPtr& map) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  /**
   * @see ConstOsmMapConsumer
   */
  void setOsmMap(const OsmMap* map) override { _from = map->shared_from_this(); }

  /**
   * @see ElementCriterionConsumer
   */
  void addCriterion(const ElementCriterionPtr& crit) override;

  /**
    @see OperationStatus
    */
  virtual QString getInitStatusMessage() const { return "Copying map subset..."; }
  /**
    @see OperationStatus
    */
  virtual QString getCompletedStatusMessage() const { return "Subset map copying complete."; }

  QString getDescription() const override { return "Copies a subset of the map into a new map"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  std::set<ElementId>& getEidsCopied() { return _eidsCopied; }

  void setEids(const std::set<ElementId>& eids) { _eids = eids; }
  void setCopyChildren(bool copy) { _copyChildren = copy; }

private:

  std::set<ElementId> _eids;
  ConstOsmMapPtr _from;
  // determines whether child elements are copied (way nodes and relation members)
  bool _copyChildren;
  std::set<ElementId> _eidsCopied;
};

}

#endif // COPYMAPSUBSETOP_H
