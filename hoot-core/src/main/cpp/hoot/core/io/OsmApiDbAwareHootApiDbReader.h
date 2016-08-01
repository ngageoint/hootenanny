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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMAPIDBAWAREHOOTAPIDBREADER_H
#define OSMAPIDBAWAREHOOTAPIDBREADER_H

#include "HootApiDbReader.h"
#include "OsmApiDb.h"

namespace hoot
{

/**
 * Hoot api db reader that doesn't allow hoot api db element id's to clash with a specified osm api
 * db's id's (osm api db is the id master reference).
 */
class OsmApiDbAwareHootApiDbReader : public HootApiDbReader
{
public:

  static std::string className() { return "hoot::OsmApiDbAwareHootApiDbReader"; }

  OsmApiDbAwareHootApiDbReader();

  virtual ~OsmApiDbAwareHootApiDbReader();

  virtual void open(QString urlStr);

protected:

  virtual ElementId _mapElementId(const OsmMap& map, ElementId oldId);

private:

  OsmApiDb _osmApiDb;

};

}

#endif // OSMAPIDBAWAREHOOTAPIDBREADER_H
