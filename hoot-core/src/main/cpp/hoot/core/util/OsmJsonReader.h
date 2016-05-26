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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_JSON_READER_H
#define OSM_JSON_READER_H

// Qt
#include <QHash>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QVariant>

// Standard
#include <set>
#include <string>

// Boost
#include <boost/property_tree/ptree.hpp>

// Hoot
#include "hoot/core/OsmMap.h"

namespace hoot
{

/**
 * This class is intended to create an OsmMap from a given json string, sourced
 * from the overpass-api. (http://overpass-api.de/output_formats.html#json)
 *
 * This class doesn't do anything clever to handle large datasets - it
 * simply keeps everything in memory. Beware if you want to handle lots of data!
 */

class OsmJsonReader
{
public:

  // Default consructor
  OsmJsonReader();

  OsmMapPtr loadFromString(QString jsonStr);

  OsmMapPtr loadFromFile(QString path);


private:
  void _loadJSON(QString jsonStr);
  OsmMapPtr _parseOverpassJson();

  // Our property tree that holds JSON
  boost::property_tree::ptree _propTree;

  string _version;
  string _generator;
  string _timestamp_base;
  string _copyright;

  void parseOverpassNode(const boost::property_tree::ptree &item,
                         OsmMapPtr pMap);
  void parseOverpassWay(const boost::property_tree::ptree &item,
                        OsmMapPtr pMap);
  void parseOverpassRelation(const boost::property_tree::ptree &item,
                             OsmMapPtr pMap);
  void addTags(const boost::property_tree::ptree &item,
               hoot::ElementPtr pElement);

};

} // end namespace hoot

#endif // OSM_JSON_READER_H
