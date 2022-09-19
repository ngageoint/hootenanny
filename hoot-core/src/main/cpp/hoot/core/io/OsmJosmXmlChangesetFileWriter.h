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
#ifndef OSM_JOSM_XML_CHANGESET_FILE_WRITER_H
#define OSM_JOSM_XML_CHANGESET_FILE_WRITER_H

// Hoot
#include <hoot/core/io/OsmBaseXmlChangesetFileWriter.h>

// Qt
class QXmlStreamWriter;

namespace hoot
{

/**
 * Writes a JOSM OSM changeset to an XML file.
 *
 * This file uses an 'action' attribute to describe the change
 */
class OsmJosmXmlChangesetFileWriter : public OsmBaseXmlChangesetFileWriter
{

public:

  static QString className() { return "OsmJosmXmlChangesetFileWriter"; }

  OsmJosmXmlChangesetFileWriter() = default;
  ~OsmJosmXmlChangesetFileWriter() override = default;

  /**
   * @see ChangesetFileWriter
   */
  bool isSupported(const QString& output) const override { return output.endsWith(".osm", Qt::CaseInsensitive); }

protected:

  /** See OsmBaseXmlChangesetFileWriter */
  void _writeXmlFileHeader(QXmlStreamWriter& writer) const override;
  void _writeXmlFileSectionHeader(QXmlStreamWriter& writer, Change::ChangeType last) const override;
  void _writeXmlActionAttribute(QXmlStreamWriter& writer) const override;
  void _getOptionalTags(Tags& tags, const Element* element) const override;
};

}

#endif // OSM_JOSM_XML_CHANGESET_FILE_WRITER_H
