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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_CHANGESET_MAP_H
#define OSM_CHANGESET_MAP_H

//  Qt
#include <QList>
#include <QMap>
#include <QString>
#include <QXmlDefaultHandler>

//  Hootenanny
#include <tgs/BigContainers/BigMap.h>

namespace hoot
{

class XmlElement;

typedef boost::shared_ptr<XmlElement> ElementItem;
typedef QList<ElementItem> ElementList;

class OsmChangesetMap : public QXmlDefaultHandler
{
public:
  OsmChangesetMap();
  OsmChangesetMap(const QString& filename);
  OsmChangesetMap(const QList<QString>& filenames);

  void loadMap();
  void loadMap(const QString& filename);
  void loadMap(const QList<QString>& filenames);

  void updateNodeMap(const QMap<long, long>& nodeMap);
  void updateWayMap(const QMap<long, long>& wayMap);
  void updateRelationMap(const QMap<long, long>& relationMap);

  /// QXmlDefaultHandler overrides
  virtual bool startElement(const QString& namespaceURI, const QString& localName,
                            const QString& qName, const QXmlAttributes& attributes);
  virtual bool endElement(const QString& namespaceURI, const QString& localName,
                  const QString& qName);
  virtual bool fatalError(const QXmlParseException& exception);

private:
  QList<QString> _filenames;
  Tgs::BigMap<long, long> _nodeMap;
  Tgs::BigMap<long, long> _wayMap;
  Tgs::BigMap<long, long> _relationMap;

  ElementItem _currentElement;
  ElementList* _currentList;
  ElementList _createList;
  ElementList _modifyList;
  ElementList _deleteList;

  bool _isChangeset;
  QString _errorString;

};

}

#endif  //  OSM_CHANGESET_MAP_H
