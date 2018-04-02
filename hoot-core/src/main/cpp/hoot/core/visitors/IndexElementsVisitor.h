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
#ifndef INDEXELEMENTVISITOR_H
#define INDEXELEMENTVISITOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/filters/ElementCriterionConsumer.h>

// TGS
#include <tgs/RStarTree/HilbertRTree.h>

// Standard
#include <deque>

// Boost
#include <boost/function.hpp>


namespace hoot
{

/**
 * This class was extracted from ScriptMatchCreator so that it could be used by the other
 * match creators (highway, building, etc). It is used to build an index of input elements
 * (which can be filterd by various criteria, if need be). The envelope plus the search
   radius is created as the index box for each element. This is more efficient than using
   the OsmMapIndex index.
 */
class ElementCriterion;

class IndexElementsVisitor :
    public ConstElementVisitor,
    public ElementCriterionConsumer
{
public:

  static std::string className() { return "hoot::CreateIndexVisitor"; }

  explicit IndexElementsVisitor(boost::shared_ptr<Tgs::HilbertRTree>& index,
                                std::deque<ElementId>& indexToEid,
                                const boost::shared_ptr<ElementCriterion>& filter,
                                boost::function<Meters (const ConstElementPtr& e)> getSearchRadius,
                                ConstOsmMapPtr pMap);

  void addCriterion(const ElementCriterionPtr& e);

  void visit(const ConstElementPtr& e);

  void finalizeIndex();

  static std::set<ElementId> findNeighbors(const geos::geom::Envelope& env,
                                           const boost::shared_ptr<Tgs::HilbertRTree>& index,
                                           const std::deque<ElementId>& indexToEid,
                                           ConstOsmMapPtr pMap);

  virtual QString getDescription() const { return "Build an index of input elements"; }

private:

  ConstOsmMapPtr _pMap;
  boost::shared_ptr<ElementCriterion> _filter;
  boost::function<Meters (const ConstElementPtr& e)> _getSearchRadius;

  boost::shared_ptr<Tgs::HilbertRTree>& _index;
  std::deque<ElementId>& _indexToEid;
  std::vector<Tgs::Box> _boxes;
  std::vector<int> _fids;
};


}

#endif // INDEXELEMENTVISITOR_H
