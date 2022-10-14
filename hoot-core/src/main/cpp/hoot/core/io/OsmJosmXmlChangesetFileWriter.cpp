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
#include "OsmJosmXmlChangesetFileWriter.h"

// hoot
#include <hoot/core/conflate/ConflateUtils.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QXmlStreamWriter>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmChangesetFileWriter, OsmJosmXmlChangesetFileWriter)

OsmJosmXmlChangesetFileWriter::OsmJosmXmlChangesetFileWriter()
{
  //  Only include circular error when explicitly requested
  _addExportTagsVisitor.setIncludeCircularError(_includeCircularErrorTags);
}

void OsmJosmXmlChangesetFileWriter::setConfiguration(const Settings &conf)
{
  OsmBaseXmlChangesetFileWriter::setConfiguration(conf);
  _addExportTagsVisitor.setIncludeCircularError(_includeCircularErrorTags);
}


void OsmJosmXmlChangesetFileWriter::_writeXmlFileHeader(QXmlStreamWriter& writer) const
{
  writer.writeStartElement("osm");
  writer.writeAttribute("version", "0.6");
  writer.writeAttribute("generator", HOOT_PACKAGE_NAME);
  geos::geom::Envelope bounds;
  if (!_map1List.empty())
  {
    writer.writeAttribute("srs", _map1List.at(0)->getProjectionEpsgString());
    bounds.expandToInclude(CalculateMapBoundsVisitor::getGeosBounds(_map1List.at(0)));
  }
  if (!_map2List.empty())
    bounds.expandToInclude(CalculateMapBoundsVisitor::getGeosBounds(_map2List.at(0)));
  if (!bounds.isNull())
  {
    writer.writeStartElement("bounds");
    writer.writeAttribute("minlat", QString::number(bounds.getMinY(), 'g', _precision));
    writer.writeAttribute("minlon", QString::number(bounds.getMinX(), 'g', _precision));
    writer.writeAttribute("maxlat", QString::number(bounds.getMaxY(), 'g', _precision));
    writer.writeAttribute("maxlon", QString::number(bounds.getMaxX(), 'g', _precision));
    writer.writeEndElement();
  }
}

void OsmJosmXmlChangesetFileWriter::_writeXmlFileSectionHeader(QXmlStreamWriter& /*writer*/, Change::ChangeType /*last*/) const
{
  //  JOSM OSM XML files don't use the file section header
}

void OsmJosmXmlChangesetFileWriter::_writeXmlActionAttribute(QXmlStreamWriter& writer) const
{
  QString action;
  switch (_change.getType())
  {
  case Change::ChangeType::NoChange:
    //  Create doesn't require an 'action' attribute
    return;
  case Change::ChangeType::Delete:
    action = "delete";
    break;
  case Change::ChangeType::Create:
  case Change::ChangeType::Modify:
    action = "modify";
    break;
  case Change::ChangeType::Unknown:
    break;
  default:
    throw IllegalArgumentException("Unexpected change action type.");
  }
  writer.writeAttribute("action", action);
}

void OsmJosmXmlChangesetFileWriter::_getOptionalTags(Tags& tags, const Element* element) const
{
  //  Rather than cloning the element, get the export tags from the visitor
  tags.add(_addExportTagsVisitor.getExportTags(element));
}

}
