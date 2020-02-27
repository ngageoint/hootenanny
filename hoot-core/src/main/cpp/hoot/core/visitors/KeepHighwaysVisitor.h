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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef KEEPHIGHWAYSVISITOR_H
#define KEEPHIGHWAYSVISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/OsmMapConsumer.h>

namespace hoot
{

/**
 * Removes all ways and relations that are not part of a linear highway.
 * Note: You may have to run this multiple times to get the desired effect.
 *
 * It may be possible to remove this class after both #3276 are completed and any regression tests
 * using it can be converted to use RemoveElementsVisitor combined with a NotCriterion instead
 * (second part may not be possible given how this class has been written).
 */
class KeepHighwaysVisitor : public ConstElementVisitor, public OsmMapConsumer
{
public:

  static std::string className() { return "hoot::KeepHighwaysVisitor"; }

  KeepHighwaysVisitor() {}

  virtual void setOsmMap(OsmMap* map) { _map = map; }
  /**
   * KeepHighwaysVisitor requires a read/write map.
   */
  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  virtual void visit(const ConstElementPtr& e);

  virtual QString getDescription() const
  { return "Removes all ways and relations that are not part of a linear highway"; }

  virtual std::string getClassName() const { return className(); }

private:

  OsmMap* _map;
};

}

#endif // KEEPHIGHWAYSVISITOR_H
