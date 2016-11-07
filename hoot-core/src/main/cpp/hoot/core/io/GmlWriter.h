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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef GMLWRITER_H
#define GMLWRITER_H

// hoot
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/OsmMap.h>

// Qt
#include <QHash>
#include <QString>
#include <QStringList>
#include <QXmlDefaultHandler>
class QString;

// Standard
#include <vector>

namespace hoot {

/**
 * Writes a map or portion of a map to a GML file. Not completely implemented.
 *
 * @todo write a generic OGR writer.
 */
class GmlWriter
{
public:

  GmlWriter() { }

  void setColumns(QStringList columns) { _columns = columns; }

  void setIncludeIds(bool includeIds) { _includeIds = includeIds; }

  void setIncludeHootInfo(bool includeInfo) { _includeInfo = includeInfo; }

  void write(shared_ptr<const OsmMap> map, QString path);

  void writePoints(shared_ptr<const OsmMap> map, const QString& path);

protected:
  QStringList _columns;
  bool _includeIds;
  bool _includeInfo;

  void _writeRelationPolygon(const shared_ptr<Relation> &relation, OGRLayer *poLayer,
    const QStringList &shpColumns);

  void _writeWayPolygon(const shared_ptr<Way>& way, OGRLayer *poLayer,
    const QStringList& shpColumns);
};

} // hoot


#endif
