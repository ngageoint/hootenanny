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
#ifndef REMOVEELEMENTSVISITOR_H
#define REMOVEELEMENTSVISITOR_H

// hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/filters/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Removes any elements where isFiltered() == true
 */
class RemoveElementsVisitor :
    public ElementVisitor,
    public ConstOsmMapConsumer,
    public ElementCriterionConsumer,
    public Configurable
{
public:

  static std::string className() { return "hoot::RemoveElementsVisitor"; }

  /**
   * Loads the filter from the config setting.
   */
  RemoveElementsVisitor();

  RemoveElementsVisitor(const boost::shared_ptr<ElementCriterion>& filter);

  virtual void addCriterion(const ElementCriterionPtr& e)
  {
    assert(_filter.get() == 0);
    _filter = e;
  }

  virtual void visit(const ConstElementPtr& e);

  virtual void setConfiguration(const Settings& conf);

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  void setRecursive(bool recursive) { _recursive = recursive; }

  static void removeWays(boost::shared_ptr<OsmMap> pMap,
                         const boost::shared_ptr<ElementCriterion>& pCrit);

private:

  OsmMap* _map;
  boost::shared_ptr<ElementCriterion> _filter;
  bool _recursive;
};


}

#endif // REMOVEELEMENTSVISITOR_H
