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
#ifndef __OSM_XML_DISK_CACHE_H__
#define __OSM_XML_DISK_CACHE_H__

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>

// GDAL
class OGRSpatialReference;

namespace hoot
{

class OsmXmlDiskCache : ElementProvider
{
public:

  OsmXmlDiskCache(QString tempFileName);
  ~OsmXmlDiskCache();

  void addElement(ConstElementPtr &newElement);

  virtual std::shared_ptr<OGRSpatialReference> getProjection() const override;

  bool containsElement(const ElementId& eid) const override;
  ConstElementPtr getElement(const ElementId& eid) const override;

  ConstNodePtr getNode(long id) const override;
  NodePtr getNode(long id) override;

  ConstRelationPtr getRelation(long id) const override;
  RelationPtr getRelation(long id) override;

  ConstWayPtr getWay(long id) const override;
  WayPtr getWay(long id) override;

  bool containsNode(long id) const override;
  bool containsRelation(long id) const override;
  bool containsWay(long id) const override;

private:

  QString _tempFileName;
  OsmXmlReader _reader;
  OsmXmlWriter _writer;
  QMap<uint64_t, uint64_t> _node2pos;
  QMap<uint64_t, uint64_t> _way2pos;
  QMap<uint64_t, uint64_t> _relation2pos;
};

using ElementProviderPtr = std::shared_ptr<ElementProvider>;
using ConstElementProviderPtr = std::shared_ptr<const ElementProvider>;

}

#endif // __OSM_XML_DISK_CACHE_H__
