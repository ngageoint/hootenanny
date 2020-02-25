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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef RANDOM_WAY_SPLITTER_H
#define RANDOM_WAY_SPLITTER_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/algorithms/linearreference/MultiLineStringLocation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/RngConsumer.h>

namespace hoot
{

/**
 * Performs random splitting of a map's ways in accordance with [1].  The probability that a given
 * way is split and the minimum allowable spacing between nodes on a split way are configurable in
 * in the Hootenanny configuration settings.  Relation multi-line strings are assumed to only
 * contain ways.  An exception will be thrown if they contain any other type of feature.
 *
 * One difference between this implementation and that of the paper is that this implementation will
 * not randomly select a split point that violates the minimum node distance setting unless the way
 * to be split is small enough that it is not possible to select a split point that does not violate
 * it.  The paper's algorithm can randomly select a split point that violates the minimum node
 * spacing, even if the way being split is large enough to make avoiding doing so possible.
 *
 * 1. Evaluating conflation methods using uncertainty modeling - Peter Doucette, et al. 2013
 *    https://insightcloud.digitalglobe.com/redmine/attachments/download/1667/2013%20Evaluating%20conflation%20methods%20using%20uncertainty%20modeling.pdf
 *    http://proceedings.spiedigitallibrary.org/proceeding.aspx?articleid=1691369
 *
 *  The input map data will be projected to an orthographic coordinate system.
 */
class RandomWaySplitter : public ElementOsmMapVisitor, public RngConsumer, public Configurable
{

public:

  static std::string className() { return "hoot::RandomWaySplitter"; }

  RandomWaySplitter();

  virtual void setConfiguration(const Settings& conf);

  /**
    Randomly and recursively applies the PERTY way split operation to each visited way or multi-line
    string relation while honoring a configurable minimum spacing between way nodes.

    @see ConstElementVisitor
    */
  virtual void visit(const std::shared_ptr<Element>& e) override;

  /**
    @see RngConsumer
    */
  virtual void setRng(boost::minstd_rand& rng) { _rng = &rng; }

  virtual void setOsmMap(OsmMap* map) override;

  /**
    the probability that any way will be split into smaller segements
   */
  void setWaySplitProbability(double probability)
  {
    if (probability < 0.0 || probability > 1.0)
    {
      throw HootException("Invalid split probability: " + QString::number(probability));
    }
    _waySplitProbability = probability;
  }

  /**
    minimum space allowed between nodes added as a result of way splitting; in meters
    */
  void setMinNodeSpacing(double spacing)
  {
    if (spacing < 0.0)
    {
      throw HootException("Invalid minimum node spacing: " + QString::number(spacing));
    }
    _minNodeSpacing = spacing;
  }

  virtual QString getDescription() const { return "Randomly splits ways"; }

  virtual std::string getClassName() const { return className(); }

private:

  boost::minstd_rand* _rng;
  std::shared_ptr<boost::minstd_rand> _localRng;

  double _waySplitProbability;
  double _minNodeSpacing;

  //for testing purposes
  int _splitRecursionLevel;

  std::vector<ElementPtr> _split(ElementPtr element);
  WayLocation _calcSplitPoint(ConstWayPtr way) const;
  MultiLineStringLocation _calcSplitPoint(ConstRelationPtr relation, ElementId& wayId) const;

  NodePtr _getNodeAddedBySplit(const QList<long>& nodeIdsBeforeSplit,
                               const std::vector<ElementPtr>& newElementsAfterSplit) const;

  void _updateNewNodeProperties(NodePtr newNode,
                                ConstNodePtr firstSplitBetweenNode,
                                ConstNodePtr lastSplitBetweenNode);
};

}

#endif // RANDOM_WAY_SPLITTER_H
