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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef COUNTMANUALMATCHESVISITOR_H
#define COUNTMANUALMATCHESVISITOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>

#include "SingleStatistic.h"

namespace hoot
{
using namespace std;

/**
 * Counts matches made with ref tags by a manual matcher
 */
class CountManualMatchesVisitor : public ElementVisitor, public ConstOsmMapConsumer,
  public SingleStatistic
{

public:

  static string className() { return "hoot::CountManualMatchesVisitor"; }

  CountManualMatchesVisitor();

  virtual ~CountManualMatchesVisitor() {}

  double getStat() const { return _numManualMatches; }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e);

private:

  const OsmMap* _map;
  long _numManualMatches;

};

}
#endif // COUNTMANUALMATCHESVISITOR_H
