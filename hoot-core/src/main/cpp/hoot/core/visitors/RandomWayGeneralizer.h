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
#ifndef RANDOM_WAY_GENERALIZER_H
#define RANDOM_WAY_GENERALIZER_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/RngConsumer.h>

namespace hoot
{
  class Way;
  class RdpWayGeneralizer;

/**
 * Performs random generalization of a map's ways in accordance with [1].  Uses the
 * Ramer-Douglas Peucker algorithm [2].
 *
 * 1. Evaluating conflation methods using uncertainty modeling - Peter Doucette, et al. 2013
 *    https://insightcloud.digitalglobe.com/redmine/attachments/download/1667/2013%20Evaluating%20conflation%20methods%20using%20uncertainty%20modeling.pdf
 *    http://proceedings.spiedigitallibrary.org/proceeding.aspx?articleid=1691369
 * 2. http://en.wikipedia.org/wiki/Ramer%E2%80%93Douglas%E2%80%93Peucker_algorithm
 *
 * The input map data will be projected to an orthographic coordinate system.
 */
class RandomWayGeneralizer : public ElementOsmMapVisitor, public RngConsumer,
  public Configurable
{

public:

  static std::string className() { return "hoot::RandomWayGeneralizer"; }

  RandomWayGeneralizer();

  virtual void setConfiguration(const Settings& conf);

  /**
    Randomly and recursively applies a way generalize operation to each visited way

    @see ConstElementVisitor
    */
  virtual void visit(const std::shared_ptr<Element>& element);

  /**
    @see RngConsumer
    */
  virtual void setRng(boost::minstd_rand& rng) { _rng = &rng; }

  virtual void setOsmMap(OsmMap* map);

  /**
    the probability that any way will be generalized
   */
  void setWayGeneralizeProbability(double probability)
  {
    if (probability < 0.0 || probability > 1.0)
    {
      throw HootException("Invalid generalize probability: " + QString::number(probability));
    }
    _wayGeneralizeProbability = probability;
  }

  /**
    @see RdpWayGeneralizer::setEpsilon
    */
  void setEpsilon(double epsilon) { _epsilon = epsilon; }

  virtual QString getDescription() const { return "Randomly simplifies ways by removing nodes"; }

  void setRemoveNodesSharedByWays(bool remove) { _removeNodesSharedByWays = remove; }

  virtual std::string getClassName() const { return className(); }

private:

  boost::minstd_rand* _rng;
  std::shared_ptr<boost::minstd_rand> _localRng;

  double _wayGeneralizeProbability;
  double _epsilon;
  // see RdpWayGeneralizer
  bool _removeNodesSharedByWays;

  std::shared_ptr<RdpWayGeneralizer> _generalizer;

  void _generalize(const WayPtr& way);

};

}

#endif // RANDOM_WAY_GENERALIZER_H
