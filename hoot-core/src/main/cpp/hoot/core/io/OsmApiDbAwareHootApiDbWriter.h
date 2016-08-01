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
#ifndef OSMAPIDBAWAREHOOTAPIDBWRITER_H
#define OSMAPIDBAWAREHOOTAPIDBWRITER_H

#include "HootApiDbWriter.h"
#include "OsmApiDb.h"

namespace hoot
{

/**
 * Hoot api db writer that doesn't allow hoot api db element id's to clash with a specified osm api
 * db's id's (osm api db is the id master reference).
 */
class OsmApiDbAwareHootApiDbWriter : public HootApiDbWriter
{
public:

  static std::string className() { return "hoot::OsmApiDbAwareHootApiDbWriter"; }

  OsmApiDbAwareHootApiDbWriter();

  virtual ~OsmApiDbAwareHootApiDbWriter();

  virtual void open(QString urlStr);

  virtual void writePartial(const shared_ptr<const Node>& n);

  virtual void writePartial(const shared_ptr<const Way>& w);

  virtual void writePartial(const shared_ptr<const Relation>& r);

protected:

  virtual long _getRemappedElementId(const ElementId& eid);

private:

  OsmApiDb _osmApiDb;

};

}

#endif // OSMAPIDBAWAREHOOTAPIDBWRITER_H
