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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef LENGTHOFWAYSVISITOR_H
#define LENGTHOFWAYSVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/OsmMap.h>

#include "SingleStatistic.h"

namespace hoot
{

class OsmMap;

/**
 * Sums the length of all the ways. The map projection is used so to get meters the map must be
 * first projected into meters.
 */
class LengthOfWaysVisitor : public ElementVisitor, public ConstOsmMapConsumer,
  public SingleStatistic
{
public:

  static std::string className() { return "hoot::LengthOfWaysVisitor"; }

  LengthOfWaysVisitor() : _total(0) {}

  virtual ~LengthOfWaysVisitor() {}

  static Meters getLengthOfWays(const OsmMapPtr& map, ElementPtr e);

  Meters getLengthOfWays() const { return _total; }

  double getStat() const { return getLengthOfWays(); }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e);

private:
  const OsmMap* _map;
  Meters _total;
};

}

#endif // ADDREFVISITOR_H
