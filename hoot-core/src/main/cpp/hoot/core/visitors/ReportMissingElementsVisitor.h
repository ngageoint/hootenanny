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
#ifndef REPORTMISSINGELEMENTSVISITOR_H
#define REPORTMISSINGELEMENTSVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{
using namespace std;

/**
 * Reports references to missing elements in a given map. If removeMissing is set to true then
 * all missing references are removed. Note: This may not give desired results if your data
 * shouldn't contain missing references.
 */
class ReportMissingElementsVisitor : public ElementVisitor, public ConstOsmMapConsumer,
    public Configurable
{
public:

  static string className() { return "hoot::ReportMissingElementsVisitor"; }

  ReportMissingElementsVisitor(bool removeMissing = false, int maxReport = 10);

  virtual ~ReportMissingElementsVisitor() {}

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void setOsmMap(const OsmMap* map) { assert(!_removeMissing); _constMap = map; }

  virtual void visit(const ConstElementPtr& e);

  virtual void setConfiguration(const Settings& conf);

  void setMaxReport(int maxReport) { _maxReport = maxReport; }

protected:

  OsmMap* _map;
  const OsmMap* _constMap;
  int _maxReport;
  int _missingCount;
  bool _removeMissing;

  virtual void _reportMissing(ElementId referer, ElementId missing);

  virtual void _visitRo(ElementType type, long id);

  virtual void _visitRw(ElementType type, long id);
};

}

#endif // REPORTMISSINGELEMENTSVISITOR_H
