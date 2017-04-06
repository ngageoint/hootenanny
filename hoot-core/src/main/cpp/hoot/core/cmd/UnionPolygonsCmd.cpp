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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/MergeNearbyNodes.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryConverter.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

class UnionPolygonsCmd : public BaseCommand
{
public:

  static string className() { return "hoot::UnionPolygonsCmd"; }

  UnionPolygonsCmd() { }

  virtual QString getName() const { return "union-polygons"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes at least two parameters.").arg(getName()));
    }

    QString output = args[0];

    boost::shared_ptr<OsmMap> map(new OsmMap());

    for (int i = 1; i < args.size(); i++)
    {
      QString input = args[i];
      loadMap(map, input, false);
    }

    boost::shared_ptr<Geometry> g(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
    int count = 0;
    const RelationMap& rm = map->getRelations();
    for (RelationMap::const_iterator it = rm.begin(); it != rm.end(); ++it)
    {
      const ConstRelationPtr r = it->second;
      g.reset(g->Union(ElementConverter(map).convertToGeometry(r).get()));
      count++;
    }

    if (count == 0)
    {
      LOG_INFO("No polygons were found in the input.");
    }

    boost::shared_ptr<OsmMap> result(new OsmMap());
    GeometryConverter(result).convertGeometryToElement(g.get(), Status::Unknown1, -1);

    saveMap(result, output);

    LOG_INFO("Done writing file.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, UnionPolygonsCmd)

}

