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
#ifndef VISITOROP_H
#define VISITOROP_H

// hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/visitors/ElementVisitorConsumer.h>

namespace hoot
{

class OsmMap;
class ElementVisitor;

/**
 * Given a visitor this operation applies the visitor to the map as a read/write operation.
 */
class VisitorOp : public OsmMapOperation, public ElementVisitorConsumer
{
public:

  static std::string className() { return "hoot::VisitorOp"; }

  VisitorOp() {}
  VisitorOp(const ElementVisitorPtr& v) { _visitor = v; }

  /**
   * Takes ownership of the visitor.
   */
  VisitorOp(ElementVisitor* v) { _visitor.reset(v); }

  virtual void addVisitor(const ElementVisitorPtr& e);

  virtual void apply(std::shared_ptr<OsmMap>& map);

  virtual QString getDescription() const { return ""; }

  virtual std::string getClassName() const { return className(); }

private:

  std::shared_ptr<ElementVisitor> _visitor;
};

}

#endif // VISITOROP_H
