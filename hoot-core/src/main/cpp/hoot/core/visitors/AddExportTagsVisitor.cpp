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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "AddExportTagsVisitor.h"

#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

AddExportTagsVisitor::AddExportTagsVisitor()
{  
  _textStatus = ConfigOptions().getWriterTextStatus();
  _includeDebug = ConfigOptions().getWriterIncludeDebugTags();
  _includeCircularError = ConfigOptions().getWriterIncludeCircularErrorTags();
}

void AddExportTagsVisitor::visit(const ElementPtr& pElement)
{ 
  Tags& tags = pElement->getTags();
  const ElementType type = pElement->getElementType();

  if (_includeDebug)
  {
    Status status = pElement->getStatus();
    bool includeStatus =
        status != Status::Invalid ||
        type == ElementType::Relation ||
        type == ElementType::Node;

    if( includeStatus )
    {
      tags[MetadataTags::HootStatus()] = _textStatus ? status.toTextStatus() : status.toString();
    }

    tags[MetadataTags::HootId()] = QString::number(pElement->getId());
  }

  if (_includeCircularError &&
       pElement->hasCircularError() &&
       type != ElementType::Node
     )
  {
    tags[MetadataTags::ErrorCircular()] = QString::number(pElement->getCircularError());
  }
}

}
