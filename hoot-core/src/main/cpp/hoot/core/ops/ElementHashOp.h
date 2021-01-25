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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENT_HASH_OP_H
#define ELEMENT_HASH_OP_H

// hoot
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/visitors/ElementHashVisitor.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * This is an alternative implementation of element hashing to ElementHashVisitor that attempts to
 * better determine the difference between way nodes. In particular, even when two way nodes are
 * very close together, it can determine if they actually belong to different ways.
 * _addParentToWayNodes must be enabled for this to occur. Its possible this implementation could
 * replace ElementHashVisitor at some point, but right now it doesn't work well against some data
 * and much more testing of it needs to occur before that could happen.
 */
class ElementHashOp : public ConstOsmMapOperation
{
public:

  static std::string className() { return "hoot::ElementHashOp"; }

  ElementHashOp();
  virtual ~ElementHashOp() = default;

  virtual void apply(const OsmMapPtr& map);

  virtual QString getDescription() const { return _hashVis.getDescription(); }

  virtual QString getName() const { return QString::fromStdString(className()); }

  QMap<QString, ElementId> getHashesToElementIds() const
  { return _hashVis.getHashesToElementIds(); }
  QMap<ElementId, QString> getElementIdsToHashes() const
  { return _hashVis.getElementIdsToHashes(); }
  QSet<std::pair<ElementId, ElementId>> getDuplicates() const { return _hashVis.getDuplicates(); }

  void setCoordinateComparisonSensitivity(int sensitivity)
  { _hashVis.setCoordinateComparisonSensitivity(sensitivity); }
  void setIncludeCircularError(bool include) { _hashVis.setIncludeCircularError(include); }
  void setUseNodeTags(bool use) { _hashVis.setUseNodeTags(use); }
  void setWriteHashes(bool write) { _hashVis.setWriteHashes(write); }
  void setCollectHashes(bool collect) { _hashVis.setCollectHashes(collect); }
  void setAddParentToWayNodes(bool add) { _addParentToWayNodes = add; }

private:

  // If enabled, parent way types are added to way node hashes to better identify way node
  // differences.
  bool _addParentToWayNodes;

  ElementHashVisitor _hashVis;
};

}

#endif // ELEMENT_HASH_VISITOR_H
