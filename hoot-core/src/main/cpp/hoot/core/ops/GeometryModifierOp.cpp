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
#include "hoot-core/src/main/cpp/hoot/core/visitors/geometrymodifiers/GeometryModifierAction.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>

namespace bpt = boost::property_tree;
using namespace boost;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, GeometryModifierOp)

const std::string GeometryModifierAction::FILTER_TAG = "filter";
const std::string GeometryModifierAction::ARGUMENT_TAG = "arguments";

GeometryModifierOp::GeometryModifierOp()
{
  _rulesFileName = ConfigOptions().getGeometryModifierRulesFile();

  // get and instantiate available actions
  std::vector<std::string> availableActionTypes = Factory::getInstance().getObjectNamesByBase(GeometryModifierAction::className());

  LOG_INFO( "Available Geometry Modifiers:")
  for( std::vector<std::string>::iterator it = availableActionTypes.begin(); it != availableActionTypes.end(); it++ )
  {
    shared_ptr<GeometryModifierAction> pAction( Factory::getInstance().constructObject<GeometryModifierAction>(*it) );
    _actions.append(pAction);
    LOG_INFO( "class: " << *it << " command: " << pAction->getCommandName());
  }
}

void GeometryModifierOp::apply(boost::shared_ptr<OsmMap>& map)
{
  QList<GeometryModifierActionDesc> actionDescs = _readJsonRules();

  // re-project and set visitor map
  MapProjector::projectToPlanar(map);
  _geometryModifierVisitor.setOsmMap(map.get());

  // process
  foreach (GeometryModifierActionDesc actionDesc, actionDescs)
  {
    // visit with specific action, using proper arguments
    LOG_INFO("Processing geometry modifier " + actionDesc.command + "...");
    _geometryModifierVisitor.setActionDesc(actionDesc);
    actionDesc.pAction->parseArguments( actionDesc.arguments );
    map->visitRw(_geometryModifierVisitor);
  }

  // update operation status info
  _numAffected = _geometryModifierVisitor._numAffected;
  _numProcessed = _geometryModifierVisitor._numProcessed;
}

QList<GeometryModifierActionDesc> GeometryModifierOp::_readJsonRules()
{
  // read the json rules
  bpt::ptree propPtree;
  bpt::read_json(_rulesFileName.toLatin1().constData(), propPtree );

  QList<GeometryModifierActionDesc> actionDescs;

  BOOST_FOREACH(bpt::ptree::value_type &commandLevelValue, propPtree)
  {
    // read command
    GeometryModifierActionDesc actionDesc;
    actionDesc.command = QString::fromStdString(commandLevelValue.first);

    // check command availability
    foreach (shared_ptr<GeometryModifierAction> pAction, _actions)
    {
      if( pAction->getCommandName() == actionDesc.command )
      {
        actionDesc.pAction = pAction;
        break;
      }
    }

    if( !actionDesc.pAction )
    {
      throw HootException("Invalid geometry modifier action '" + actionDesc.command + "' in " + _rulesFileName);
    }

    if( !commandLevelValue.second.empty())
    {
      BOOST_FOREACH(bpt::ptree::value_type &dataLevelValue, commandLevelValue.second)
      {
        // read filter
        if( dataLevelValue.first == GeometryModifierAction::FILTER_TAG )
        {
          _parseFilter(actionDesc, dataLevelValue.second);
        }
        // read arguments
        else if( dataLevelValue.first == GeometryModifierAction::ARGUMENT_TAG )
        {
          _parseArguments(actionDesc, dataLevelValue.second);
        }
        else
        {
          throw HootException("Invalid geometry modifier tag '" + QString::fromStdString(dataLevelValue.first) + "' for action '" + actionDesc.command + "' in " + _rulesFileName);
        }
      }

      actionDescs.append(actionDesc);
    }
  }

  return actionDescs;
}

void GeometryModifierOp::_parseFilter(GeometryModifierActionDesc& actionDesc, bpt::ptree ptree)
{
  BOOST_FOREACH(bpt::ptree::value_type &data, ptree)
  {
    actionDesc.filter[QString::fromStdString(data.first)] = QString::fromStdString(data.second.data());
  }
}

void GeometryModifierOp::_parseArguments(GeometryModifierActionDesc& actionDesc, boost::property_tree::ptree ptree)
{
  QList<QString> availableParameters = actionDesc.pAction->getParameterNames();

  BOOST_FOREACH(bpt::ptree::value_type &data, ptree)
  {
    QString arg = QString::fromStdString(data.first);
    if( availableParameters.contains(arg) )
    {
      actionDesc.arguments[arg] = QString::fromStdString(data.second.data());
    }
    else
    {
      throw HootException("Invalid geometry modifier argument '" + arg + "' for action '" + actionDesc.command + "' in " + _rulesFileName);
    }
  }
}

}
