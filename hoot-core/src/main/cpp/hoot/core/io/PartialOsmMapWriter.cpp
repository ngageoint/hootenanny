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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "PartialOsmMapWriter.h"

#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

PartialOsmMapWriter::PartialOsmMapWriter()
  : _includeDebug(ConfigOptions().getWriterIncludeDebugTags()),
    _includePid(false),
    _sortSourceImageryTag(ConfigOptions().getWriterSortTagsImagerySource())
{
}

void PartialOsmMapWriter::write(const ConstOsmMapPtr& map)
{
  writePartial(map);
  finalizePartial();
}

void PartialOsmMapWriter::writePartial(const ConstOsmMapPtr& map)
{
  const NodeMap& nm = map->getNodes();
  for (auto it = nm.begin(); it != nm.end(); ++it)
    writePartial((ConstNodePtr)it->second);

  const WayMap& wm = map->getWays();
  for (auto it = wm.begin(); it != wm.end(); ++it)
    writePartial((ConstWayPtr)it->second);

  const RelationMap& rm = map->getRelations();
  for (auto it = rm.begin(); it != rm.end(); ++it)
    writePartial((ConstRelationPtr)it->second);
}

void PartialOsmMapWriter::writePartial(const ConstElementPtr& e)
{
  switch (e->getElementType().getEnum())
  {
  case ElementType::Node:
    writePartial(std::dynamic_pointer_cast<const Node>(e));
    break;
  case ElementType::Way:
    writePartial(std::dynamic_pointer_cast<const Way>(e));
    break;
  case ElementType::Relation:
    writePartial(std::dynamic_pointer_cast<const Relation>(e));
    break;
  default:
    throw HootException("Unexpected element type: " + e->getElementType().toString());
  }
}

void PartialOsmMapWriter::writeElement(ElementPtr& element)
{
  writePartial(element);
}

Tags PartialOsmMapWriter::_getElementTags(const ConstElementPtr& element)
{
  //  Tag order is important here, current tags first and then add export tags
  Tags tags = element->getTags();
  //  Rather than cloning the element, get the export tags from the visitor
  tags.add(_addExportTagsVisitor.getExportTags(element));

  const ElementType type = element->getElementType();
  assert(type != ElementType::Unknown);

  if (type == ElementType::Relation)
  {
    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(element);
    if (relation->getType() != "")
      tags.insert("type", relation->getType());
  }

  if (type == ElementType::Way)
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(element);
    //  Output the PID as a tag if desired for debugging purposes
    if (_includePid && way->hasPid())
      tags.insert(MetadataTags::HootSplitParentId(), QString("%1").arg(way->getPid()));
    //  Output the node count tag if desired for debugging purposes
    if (_includeDebug)
      tags.insert(MetadataTags::HootWayNodeCount(), QString("%1").arg(way->getNodeCount()));
  }

  //  Sort the `source:imagery` value for output
  if (_sortSourceImageryTag && tags.contains(MetadataTags::SourceImagery()))
  {
    QString v = tags.get(MetadataTags::SourceImagery());
    if (v.contains(";"))
    {
      QStringList values = v.split(";");
      values.sort();
      tags.set(MetadataTags::SourceImagery(), values.join(";"));
    }
  }
  return tags;
}

}
