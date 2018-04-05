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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVEMISSINGELEMENTSVISITOR_H
#define REMOVEMISSINGELEMENTSVISITOR_H

#include <hoot/core/ConstOsmMapConsumer.h>
#include "ReportMissingElementsVisitor.h"

namespace hoot
{

class ReportMissingElementsVisitor;

class RemoveMissingElementsVisitor : public ConstElementVisitor, public ConstOsmMapConsumer
{
public:

  static std::string className() { return "hoot::RemoveMissingElementsVisitor"; }

  RemoveMissingElementsVisitor();

  virtual ~RemoveMissingElementsVisitor() {}

  virtual void setOsmMap(OsmMap* map) { _v->setOsmMap(map);}

  virtual void setOsmMap(const OsmMap* /*map*/)
  { throw NotImplementedException("Set Map with const is not supported"); }

  virtual void visit(const ConstElementPtr& e);

  virtual QString getDescription() const
  { return "Removes references to any elements that do not exist in a map"; }

private:

  boost::shared_ptr<ReportMissingElementsVisitor> _v;
};

}

#endif // REMOVEMISSINGELEMENTSVISITOR_H
