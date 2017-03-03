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
#ifndef SPLITLONGLINEARWAYSVISITOR_H
#define SPLITLONGLINEARWAYSVISITOR_H

#include <string>
#include <cassert>

#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class OsmMap;
class Element;
class Settings;

class SplitLongLinearWaysVisitor : public ElementOsmMapVisitor, public Configurable
{
public:

  static std::string className() { return "hoot::SplitLongLinearWaysVisitor"; }

  static unsigned int logWarnCount;

  SplitLongLinearWaysVisitor();

  virtual ~SplitLongLinearWaysVisitor() { }

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void setOsmMap(const OsmMap*) { assert(false); }

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual void setConfiguration(const Settings& conf);

  unsigned int getMaxNumberOfNodes() const { return _maxNodesPerWay; }

private:

  unsigned int _maxNodesPerWay;

  // Actual max is 2000, but in order to allow editors to insert nodes without issues,
  //    leaving some breathing room
  static const unsigned int _defaultMaxNodesPerWay = 1900;
};

}

#endif // SPLITLONGLINEARWAYSVISITOR_H
