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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CRITERIONCOUNTVISITOR_H
#define CRITERIONCOUNTVISITOR_H

// hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/filters/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Counts the number of elements matching the criterion
 */
class CriterionCountVisitor :
    public ConstElementVisitor,
    public ConstOsmMapConsumer,
    public ElementCriterionConsumer,
    public Configurable
{
public:

  static std::string className() { return "hoot::CriterionCountVisitor"; }

  CriterionCountVisitor();
  CriterionCountVisitor(const ElementCriterionPtr& pCrit);

  virtual void addCriterion(const ElementCriterionPtr& pCrit)
  {
    assert(_pCrit.get() == 0);
    _pCrit = pCrit;
  }

  virtual ~CriterionCountVisitor() {}

  virtual void visit(const ConstElementPtr& e);

  virtual void setConfiguration(const Settings& conf);

  virtual void setOsmMap(OsmMap* /*map*/) {  assert(false); }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  uint64_t getCount() const { return _count; }
  uint64_t getTotal() const { return _visited; }

private:

  const OsmMap* _map;
  uint64_t _count;
  uint64_t _visited;
  boost::shared_ptr<ElementCriterion> _pCrit;
};

}

#endif // CRITERIONCOUNTVISITOR_H
