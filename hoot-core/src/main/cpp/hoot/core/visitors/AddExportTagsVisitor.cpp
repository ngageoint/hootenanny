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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "AddExportTagsVisitor.h"

// hoot
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

AddExportTagsVisitor::AddExportTagsVisitor()
  : _includeIds(ConfigOptions().getWriterIncludeIdTag()),
    _includeStatus(ConfigOptions().getWriterIncludeStatusTag()),
    _textStatus(ConfigOptions().getWriterTextStatus()),
    _includeCircularError(ConfigOptions().getWriterIncludeCircularErrorTags()),
    _includeDebug(ConfigOptions().getWriterIncludeDebugTags())
{
}

void AddExportTagsVisitor::visit(const ElementPtr& pElement)
{
  //  Add the export tags back to the element
  pElement->addTags(getExportTags(pElement));
}

Tags AddExportTagsVisitor::getExportTags(const ConstElementPtr& pElement) const
{
  return getExportTags(pElement.get());
}

Tags AddExportTagsVisitor::getExportTags(const Element* pElement) const
{
  const Tags& tags = pElement->getTags();
  ElementType type = pElement->getElementType();
  Status status = pElement->getStatus();
  bool isNode = type == ElementType::Node;
  bool isRelation = type == ElementType::Relation;
  bool validStatus = status != Status::Invalid;
  bool hasStatus = tags.find(MetadataTags::HootStatus()) != tags.end();
  bool hasMappingTags = tags.getNonDebugCount() > 0;

  bool addStatus = _includeDebug ||
                   // Include status on all elements except nodes unless the node has other tags
                   ((_textStatus || _includeStatus) && (!isNode || (isNode && hasMappingTags))) ||
                   (hasStatus && (isRelation || (!isRelation && validStatus)));

  bool addCircularError = _includeCircularError &&
                          pElement->hasCircularError() &&
                          (!isNode || (isNode && hasMappingTags));

  Tags newTags;
  if (addStatus)
    newTags[MetadataTags::HootStatus()] = _textStatus ? status.toTextStatus() : toCompatString(status);
  if (_includeDebug || _includeIds)
  {
    newTags[MetadataTags::HootId()] = QString::number(pElement->getId());
  }
  if (addCircularError)
    newTags[MetadataTags::ErrorCircular()] = QString::number(pElement->getCircularError());
  return newTags;
}

void AddExportTagsVisitor::overrideDebugSettings()
{
  _includeIds = true;
  _includeStatus = true;
  _textStatus = false;
  _includeCircularError = true;
  _includeDebug = true;
}

QString AddExportTagsVisitor::toCompatString(Status status) const
{
  if (status.getEnum() <= Status::EnumEnd)
    return QString::number(status.getEnum());
  else
    return status.toString();
}

}
