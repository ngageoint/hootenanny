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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef WAY_GENERALIZE_VISITOR_H
#define WAY_GENERALIZE_VISITOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/MultipleCriterionConsumerVisitor.h>

namespace hoot
{

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

  static QString className() { return "WayGeneralizeVisitor"; }

  WayGeneralizeVisitor();
  ~WayGeneralizeVisitor() override = default;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  /**
    Recursively applies a way generalize operation to each visited way

    @see ConstElementVisitor
    */
  void visit(const std::shared_ptr<Element>& element) override;

  void setOsmMap(OsmMap* map) override;

  QString getInitStatusMessage() const override
  { return "Generalizing ways..."; }
  QString getCompletedStatusMessage() const override
  {
    return QString("Generalized %1 / %2 ways. Removed %3 total nodes.")
            .arg(StringUtils::formatLargeNumber(_numAffected), StringUtils::formatLargeNumber(_numProcessed),
                 StringUtils::formatLargeNumber(_totalNodesRemoved));
  }

  void addCriterion(const ElementCriterionPtr& crit) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Simplifies ways by removing nodes"; }

  /**
    @see RdpWayGeneralizer::setEpsilon
    */
  void setEpsilon(double epsilon) { _epsilon = epsilon; }
  void setRemoveNodesSharedByWays(bool remove) { _removeNodesSharedByWays = remove; }

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
