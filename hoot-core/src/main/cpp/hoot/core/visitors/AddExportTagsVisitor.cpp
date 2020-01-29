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
 * @copyright Copyright (C) 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "AddExportTagsVisitor.h"

#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

AddExportTagsVisitor::AddExportTagsVisitor()
  : _includeIds(false),
    _textStatus(ConfigOptions().getWriterTextStatus()),
    _includeCircularError(ConfigOptions().getWriterIncludeCircularErrorTags()),
    _includeDebug(ConfigOptions().getWriterIncludeDebugTags())
{
}

void AddExportTagsVisitor::visit(const ElementPtr& pElement)
{ 
  Tags& tags = pElement->getTags();
  ElementType type = pElement->getElementType();
  Status status = pElement->getStatus();
  bool isNode = type == ElementType::Node;
  bool isRelation = type == ElementType::Relation;  
  bool validStatus = status != Status::Invalid;
  bool hasStatus = tags.find(MetadataTags::HootStatus()) != tags.end();
  bool hasMappingTags = tags.getNonDebugCount() > 0;

  // TODO: in 3076 branch:
  // - decide on universal status include rules:
  //    - as discussed, we are considering these options:
  //      - removal of TextStatus config altogether
  //      - no longer using the includeDebug flag
  //      - instead individual specific -D flags for each: status, id and circ.error
  //      - keep the special cases of (!isNode || (isNode && hasMappingTags))
  //        and (isRelation || (!isRelation && validStatus)) when deciding
  //        whether we add the tag even when the status flag is on
  //      - consider an additional verbose flag to turn everything on always,
  //        no matter node type, etc.
  // - for now:
  //    - revert all test changes (done)
  //    - revert includeCircularError change (done)
  //    - bring stringcompat back (done)
  //    - see if now we can make it through all tests (done)
  //    - if too many issues: only support OsmXmlWriter for now (done)
  //    - put task on hold until universal status rules clarified
  // - later (on hold):
  //    - set includeCircularError default to false
  //    - fix all tests
  //    - remove stringcompat and use string status enum always
  //    - fix all tests if possible

  // deciding on status based on previous OsmXmlWriter implementation
  bool addStatus = _includeDebug ||
                   (_textStatus && (!isNode || (isNode && hasMappingTags))) ||
                   (hasStatus && (isRelation || (!isRelation && validStatus)));

  bool addCircularError = _includeCircularError &&
                          pElement->hasCircularError() &&
                          (!isNode || (isNode && hasMappingTags));

  // HootStatus
  if (addStatus)
  {
    tags[MetadataTags::HootStatus()] = _textStatus ? status.toTextStatus() : toCompatString(status);
  }

  // HootId
  if (_includeDebug || _includeIds)
  {
    tags[MetadataTags::HootId()] = QString::number(pElement->getId());
  }

  // Circular Error
  if (addCircularError)
  {
    tags[MetadataTags::ErrorCircular()] = QString::number(pElement->getCircularError());
  }
}

void AddExportTagsVisitor::overrideDebugSettings()
{
  _includeIds = true;
  _textStatus = false;
  _includeCircularError = true;
  _includeDebug = true;
}

QString AddExportTagsVisitor::toCompatString(Status status) const
{
  if (status.getEnum() <= Status::EnumEnd)
  {
    return QString::number(status.getEnum());
  }
  else
  {
    return status.toString();
  }
}

}
