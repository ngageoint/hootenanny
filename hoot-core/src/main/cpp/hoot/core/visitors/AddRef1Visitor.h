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
#ifndef ADDREF1VISITOR_H
#define ADDREF1VISITOR_H

// hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{
using namespace std;

class AddRef1Visitor :
    public ElementVisitor,
    public ConstOsmMapConsumer,
    public Configurable
{
public:

  static string className() { return "hoot::AddRef1Visitor"; }

  AddRef1Visitor();

  virtual ~AddRef1Visitor() {}

  virtual void setConfiguration(const Settings& conf);

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  /**
   * AddRef1Visitor requires a read/write map.
   */
  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  /**
   * Adds a REF1 tag with a unique string to all valid elements. If the Tags::getNonDebugCount() is
   * greater than zero then the element is tagged with a REF1. The unique string is simply a counter
   * so it isn't going to be unique across multiple runs.
   */
  virtual void visit(const ConstElementPtr& e);

private:
  OsmMap* _map;
  int _count;
  QString _prefix;
  bool _informationOnly;
};

}

#endif // ADDREF1VISITOR_H
