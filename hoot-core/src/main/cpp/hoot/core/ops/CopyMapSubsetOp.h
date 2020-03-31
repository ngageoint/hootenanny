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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef COPYMAPSUBSETOP_H
#define COPYMAPSUBSETOP_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * Copies a subset of the map into a new map. The old map is unchanged.
 *
 * TODO: implement OperationStatusInfo
 */
class CopyMapSubsetOp : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::CopyMapSubsetOp"; }

  CopyMapSubsetOp(const ConstOsmMapPtr& from, const std::set<ElementId>& eids);
  CopyMapSubsetOp(const ConstOsmMapPtr& from, const std::vector<long>& wayIds);
  CopyMapSubsetOp(const ConstOsmMapPtr& from, ElementId eid);
  CopyMapSubsetOp(const ConstOsmMapPtr& from, ElementId eid1, ElementId eid2);
  CopyMapSubsetOp(const ConstOsmMapPtr& from, const ElementCriterionPtr& crit);

  /**
   * A new map is created and the eids specified in the constructor and their depedencies will be
   * copied into the new map. The map will be set to point to the new map.
   */
  virtual void apply(OsmMapPtr& map);

  void setFrom(const ConstOsmMapPtr& from) { _from = from; }
  void setEids(const std::set<ElementId>& eids) { _eids = eids; }

  virtual QString getDescription() const { return "Copies a subset of the map into a new map"; }

  std::set<ElementId>& getEidsCopied() { return _eidsCopied; }

  virtual std::string getClassName() const { return className(); }

private:

  std::set<ElementId> _eids;
  ConstOsmMapPtr _from;
  ElementCriterionPtr _crit;
  std::set<ElementId> _eidsCopied;
};

}

#endif // COPYMAPSUBSETOP_H
