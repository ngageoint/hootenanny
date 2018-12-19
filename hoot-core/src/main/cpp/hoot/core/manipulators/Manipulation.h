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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef MANIPULATION_H
#define MANIPULATION_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ElementId.h>

// Qt
#include <QString>

// Standard
#include <set>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{

class Manipulation
{
public:

  Manipulation() : _addBogusReviewTags(false) {}

  virtual ~Manipulation() {}

  /**
   * This is a handy little function to aid in the April 2014 deadline. This marks tags that look
   * like they should be reviewed with compliant, but bogus review tags. See ticket #3242.
   *
   * This should only be called if applyManipulation is not called. E.g. call it on all the
   * remaining manipulations after the real conflation is done.
   */
  virtual void addBogusReviewTags(const OsmMapPtr& /*map*/) const {}

  /**
   * Applies the manipulation to the input WorkingMap. Ideally this is a lazy manipulation. Any
   * way that is significantly modified should get a new ID.
   *
   * @param impactedElements Set of all elements that were substantively impacted by this
   *  manipulation. Substantive is subjective, but if the changes made may impact other
   *  manipulations then it is substantive. The list of impacted elements should not include any
   *  elements that were removed when the manipulation is applied.
   * @param newElements Set of all substantive elements created by this manipulation. Nodes that
   *  make up a road don't need to be included.
   */
  virtual void applyManipulation(OsmMapPtr wm, std::set<ElementId>& impactedElements,
                                               std::set<ElementId>& newElements) const = 0;

  /**
   * Calculates an approximation of the probability of this being a valid match.
   */
  virtual double calculateProbability(ConstOsmMapPtr map) const = 0;

  /**
   * Returns the exact score delta for the given map. This should always be less than or equal to
   * the estimate.
   */
  virtual double calculateScore(ConstOsmMapPtr map) const = 0;

  /**
   * Returns all the elements that are significantly impacted by this manipulation. If the way is
   * simply connected to the endpoint then it is not considered significant in most cases.
   */
  virtual const std::set<ElementId>& getImpactedElementIds(const ConstOsmMapPtr& map) const = 0;

  /**
   * Returns the estimated probability that this manipulation is correct.
   */
  virtual double getProbabilityEstimate() const = 0;

  /**
   * Returns the estimated score delta as a result of applying this manipulation. This delta should
   * over-estimate the impact. An estimate that is way too high may slow the conflation process. An
   * estimate that is too low will speed the process but may give incorrect results.
   */
  virtual double getScoreEstimate() const = 0;

  /**
   * Returns true if this manipulation is a valid option on the given input map.
   */
  virtual bool isValid(ConstOsmMapPtr map) const = 0;

  bool operator<(const Manipulation& other) const
  {
    return getScoreEstimate() < other.getScoreEstimate();
  }

  /**
   * If set to true then bogus review tags will be added when applyManipulation is called.
   */
  virtual void setAddBogusReviewTags(bool addEm) { _addBogusReviewTags = addEm; }

  virtual QString toString() const { return QString(); }

  virtual void updateEstimate(ConstOsmMapPtr map) { _estimate = calculateScore(map); }

protected:
  bool _addBogusReviewTags;
  double _estimate;
};

struct GreaterThanManipulation
{
  bool operator() (const Manipulation& a, const Manipulation& b)
  {
    return a.getScoreEstimate() > b.getScoreEstimate();
  }

  bool operator() (const boost::shared_ptr<const Manipulation>& a,
                   const boost::shared_ptr<const Manipulation>& b)
  {
    return a->getScoreEstimate() > b->getScoreEstimate();
  }
};

struct LessThanManipulation
{
  bool operator() (const Manipulation& a, const Manipulation& b)
  {
    return a.getScoreEstimate() < b.getScoreEstimate();
  }

  bool operator() (const boost::shared_ptr<const Manipulation>& a,
                   const boost::shared_ptr<const Manipulation>& b)
  {
    return a->getScoreEstimate() < b->getScoreEstimate();
  }
};

struct ManipulationHolder
{
  ManipulationHolder(boost::shared_ptr<Manipulation> m, double score)
  {
    this->m = m;
    this->score = score;
  }

  boost::shared_ptr<Manipulation> m;
  double score;

  bool operator() (const ManipulationHolder& a, const ManipulationHolder& b)
  {
    return a.score < b.score;
  }
};

struct LessThanManipulationHolder
{
  bool operator() (const ManipulationHolder& a, const ManipulationHolder& b)
  {
    return a.score < b.score;
  }
};


}

#endif // MANIPULATION_H
