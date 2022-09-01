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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "OsmOscChangesetFileWriter.h"

// hoot
/*
#include <hoot/core/conflate/ConflateUtils.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/DateTimeUtils.h>
*/
#include <hoot/core/util/Factory.h>
// Qt
#include <QXmlStreamWriter>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmChangesetFileWriter, OsmOscChangesetFileWriter)

void OsmOscChangesetFileWriter::_writeXmlFileHeader(QXmlStreamWriter& writer) const
{
  writer.writeStartElement("osmChange");
  writer.writeAttribute("version", "0.6");
  writer.writeAttribute("generator", HOOT_PACKAGE_NAME);
}

void OsmOscChangesetFileWriter::_writeXmlFileSectionHeader(QXmlStreamWriter& writer, Change::ChangeType last) const
{
  LOG_VART(Change::changeTypeToString(last));
  if (_change.getType() != last)
  {
    //  Close the last open OSC section
    if (last != Change::ChangeType::Unknown)
      writer.writeEndElement();
    //  Create the OSC section
    switch (_change.getType())
    {
    case Change::ChangeType::Create:
      LOG_TRACE("Writing create start element...");
      writer.writeStartElement("create");
      break;
    case Change::ChangeType::Delete:
      LOG_TRACE("Writing delete start element...");
      writer.writeStartElement("delete");
      break;
    case Change::ChangeType::Modify:
      LOG_TRACE("Writing modify start element...");
      writer.writeStartElement("modify");
      break;
    case Change::ChangeType::Unknown:
      break;
    default:
      throw IllegalArgumentException("Unexpected change type.");
    }
  }
}

void OsmOscChangesetFileWriter::_writeXmlActionAttribute(QXmlStreamWriter& /*writer*/) const
{
  //  OSC files don't use the action attribute
}

}
