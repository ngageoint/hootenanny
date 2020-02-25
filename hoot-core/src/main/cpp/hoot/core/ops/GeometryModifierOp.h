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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef GEOMETRYMODIFIEROP_H
#define GEOMETRYMODIFIEROP_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/geometrymodifiers/GeometryModifierVisitor.h>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

namespace hoot
{
  class GeometryModifierAction;
  struct GeometryModifierActionDesc;

  /*
   * Geometry modifier operation used to run specific processes changing the actual
   * geometry of elements, e.g. turning ways into polygons, collapsing polygons to points, etc.
   *
   * Available processes are implemented in GeometryModifierAction classes which are
   * registered with the hoot factory.
   * The GeometryModifierOp constructor instantiates all available actions and reads the json
   * rules file which defines the filter and arguments for each requested action.
   *
   * Multiple actions can be specified in the rules file and the apply() method uses the
   * GeometryModifierVisitor to go through all map elements to apply each specified action.
   * Each action is performed on the entire map before it moves on to the next action.
   */
  class GeometryModifierOp : public OsmMapOperation, public Configurable
  {
  public:

    GeometryModifierOp();

    // OsmMapOperation
    static std::string className() { return "hoot::GeometryModifierOp"; }
    QString getDescription() const { return "Modifies map geometry as specified"; }

    // applies all actions specified in the rules file '_rulesFileName' to the map.
    // runs through each action in the sequence they appear in the rules file
    void apply(std::shared_ptr<OsmMap>& map);

    // OperationStatusInfo
    virtual QString getInitStatusMessage() const { return "Modifying geometry..."; }
    virtual QString getCompletedStatusMessage() const { return "Modified " + QString::number(_numAffected) + " elements"; }

    // Configurable
    virtual void setConfiguration(const Settings& conf);

    virtual std::string getClassName() const { return className(); }

  private:
    // json rules file name
    QString _rulesFileName;

    // list of instances of all implemented geometry modifier actions
    QList<std::shared_ptr<GeometryModifierAction>> _actions;

    GeometryModifierVisitor _geometryModifierVisitor;

    // json rules file parser, creates a list of 'GeometryModifierActionDesc's
    QList<GeometryModifierActionDesc> _readJsonRules();

    // json filter parser
    void _parseFilter(GeometryModifierActionDesc& actionDesc, boost::property_tree::ptree ptree);

    // json action arguments parser
    void _parseArguments(GeometryModifierActionDesc& actionDesc, boost::property_tree::ptree ptree);

    const Settings* _pConf;
  };

}

#endif // GEOMETRYMODIFIEROP_H
