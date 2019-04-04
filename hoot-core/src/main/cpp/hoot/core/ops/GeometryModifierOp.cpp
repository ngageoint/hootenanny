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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "GeometryModifierOp.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace bpt = boost::property_tree;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, GeometryModifierOp)

GeometryModifierOp::GeometryModifierOp()
{
  _rulesFileName = ConfigOptions().getGeometryModifierRulesFile();
}

void GeometryModifierOp::apply(boost::shared_ptr<OsmMap>& map)
{
  // read the json rules
  bpt::ptree propPtree;
  bpt::read_json(_rulesFileName.toLatin1().constData(), propPtree );

  BOOST_FOREACH(bpt::ptree::value_type &v, propPtree)
  {
    LOG_VAR(v.first);

    // read command here

    if( !v.second.empty())
    {
      BOOST_FOREACH(bpt::ptree::value_type &c, v.second)
      {
        // read filter and parameters here

        LOG_VAR(c.first);
      }
    }
  }

  // re-project and set visitor map
  MapProjector::projectToPlanar(map);
  _geometryModifierVisitor.setOsmMap(map.get());

  // process
  map->visitRw(_geometryModifierVisitor);

  // update operation status info
  _numAffected = _geometryModifierVisitor._numAffected;
  _numProcessed = _geometryModifierVisitor._numProcessed;
}

}
