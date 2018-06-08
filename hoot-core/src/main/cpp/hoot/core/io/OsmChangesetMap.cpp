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

#include "OsmChangesetMap.h"

//  Qt
#include <QFile>

//  Hootenanny
#include <hoot/core/io/OsmChangesetElement.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

namespace hoot
{

OsmChangesetMap::OsmChangesetMap()
  : _currentList(NULL)
{
}

OsmChangesetMap::OsmChangesetMap(const QString& filename)
  : _currentList(NULL)
{
  _filenames.append(filename);
}

OsmChangesetMap::OsmChangesetMap(const QList<QString>& filenames)
  : _filenames(filenames),
    _currentList(NULL)
{
}

void OsmChangesetMap::loadMap(const QString& filename)
{
  _filenames.append(filename);
  loadMap();
}

void OsmChangesetMap::loadMap(const QList<QString>& filenames)
{
  _filenames = filenames;
  loadMap();
}

void OsmChangesetMap::loadMap()
{
  for (QList<QString>::iterator it = _filenames.begin(); it != _filenames.end(); ++it)
  {
    if (!QFile::exists(*it))
    {
      LOG_WARN("File doesn't exist: " << *it);
      continue;
    }
    // do xml parsing
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);

    QFile file(*it);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
      throw Exception(QObject::tr("Error opening OSC file for parsing: %1").arg(*it));
    }
    LOG_DEBUG("File " << *it << " opened for read");

    QXmlInputSource xmlInputSource(&file);
    if (reader.parse(xmlInputSource) == false)
    {
      throw HootException(_errorString);
    }
    file.close();
  }
}

void OsmChangesetMap::updateNodeMap(const QMap<long, long>& nodeMap)
{
  for (QMap<long, long>::const_iterator it = nodeMap.begin(); it != nodeMap.end(); ++it)
    _nodeMap[it.key()] = it.value();
}

void OsmChangesetMap::updateWayMap(const QMap<long, long>& wayMap)
{
  for (QMap<long, long>::const_iterator it = wayMap.begin(); it != wayMap.end(); ++it)
    _wayMap[it.key()] = it.value();
}

void OsmChangesetMap::updateRelationMap(const QMap<long, long>& relationMap)
{
  for (QMap<long, long>::const_iterator it = relationMap.begin(); it != relationMap.end(); ++it)
    _relationMap[it.key()] = it.value();
}

bool OsmChangesetMap::startElement(const QString& /*namespaceURI*/, const QString& /*localName*/,
                                   const QString& qName, const QXmlAttributes& attributes)
{
  try
  {
    if (!_isChangeset)
    {
      if (qName != "osmChange")
        throw Exception("The file is not an OSM Changeset file.");
      else if (attributes.value("version") != "0.6")
        throw Exception("Only OSM API version 0.6 is supported.");
      else
      {
        _isChangeset = true;
        return true;
      }
    }
    else if (!_currentList)
    {
      if (qName == "create")
      {
        _currentList = &_createList;
      }
      else if (qName == "modify")
      {
        _currentList = &_modifyList;
      }
      else if (qName == "delete")
      {
        _currentList = &_deleteList;
      }
    }
    else if (_currentElement)
    {
      if (qName == "tag")
      {
        //  Just add the tag to the tag list
      }
      else if (qName == "nd")
      {
        //  Parse the id and add it to the list
      }
      else if (qName == "member")
      {
        //  Parse the id and the type and add it to the list
      }
    }
    else
    {
      if (qName == "node")
      {
        if (_currentList)
        {

        }
      }
      else if (qName == "way")
      {
        if (_currentList)
        {

        }
      }
      else if (qName == "relation")
      {
        if (_currentList)
        {

        }
      }
    }
  }
  catch (const Exception& e)
  {
    _errorString = e.what();
    return false;
  }
  return true;
}

bool OsmChangesetMap::endElement(const QString& /*namespaceURI*/, const QString& /*localName*/,
                                 const QString& qName)
{
  if (_currentElement)
  {
    if (qName == "node" || qName == "way" || qName == "relation")
    {
      _currentList->push_back(_currentElement);
      _currentElement = NULL;
    }
  }
  else if (_currentList && (qName == "create" || qName == "modify" || qName == "delete"))
    _currentList = NULL;
  return true;
}

bool OsmChangesetMap::fatalError(const QXmlParseException& exception)
{
  _errorString = QObject::tr("OsmChangesetMap: Parse error at line %1, column %2:\n%3")
      .arg(exception.lineNumber())
      .arg(exception.columnNumber())
      .arg(exception.message());
  return false;
}


}
