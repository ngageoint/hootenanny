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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAY_GENERALIZE_VISITOR_H
#define WAY_GENERALIZE_VISITOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/MultipleCriterionConsumerVisitor.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class Way;
class RdpWayGeneralizer;

/**
 * Performs generalization (simplification) of a map's ways.
 *
 * @see RdpWayGeneralizer
 */
class WayGeneralizeVisitor : public ElementOsmMapVisitor, public Configurable,
  public ElementCriterionConsumer
{
public:

  static std::string className() { return "hoot::WayGeneralizeVisitor"; }

  WayGeneralizeVisitor();

  virtual void setConfiguration(const Settings& conf);

  /**
    Recursively applies a way generalize operation to each visited way

    @see ConstElementVisitor
    */
  virtual void visit(const std::shared_ptr<Element>& element);

  virtual void setOsmMap(OsmMap* map);

  /**
    @see RdpWayGeneralizer::setEpsilon
    */
  void setEpsilon(double epsilon) { _epsilon = epsilon; }

  virtual QString getDescription() const { return "Simplifies ways by removing nodes"; }

  virtual QString getInitStatusMessage() const
  { return "Generalizing ways..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Generalized " + StringUtils::formatLargeNumber(_numAffected) + " / " +
            StringUtils::formatLargeNumber(_numProcessed) + " ways. Removed " +
            StringUtils::formatLargeNumber(_totalNodesRemoved) + " total nodes."; }

  virtual void addCriterion(const ElementCriterionPtr& crit);

  void setRemoveNodesSharedByWays(bool remove) { _removeNodesSharedByWays = remove; }

  virtual std::string getClassName() const { return className(); }

private:

  double _epsilon;

  std::shared_ptr<RdpWayGeneralizer> _generalizer;
  // a criterion used to further filter the ways to be generalized
  ElementCriterionPtr _crit;
  // see RdpWayGeneralizer
  bool _removeNodesSharedByWays;
  int _totalNodesRemoved;

  void _generalize(const WayPtr& way);
};

}

#endif // PERTY_WAY_GENERALIZE_VISITOR_H
