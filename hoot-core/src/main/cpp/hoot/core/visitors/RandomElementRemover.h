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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef RANDOM_ELEMENT_REMOVER_H
#define RANDOM_ELEMENT_REMOVER_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/util/RngConsumer.h>

namespace hoot
{

/**
 * Randomly removes elements from a map
 */
class RandomElementRemover : public ElementOsmMapVisitor, public RngConsumer, public Configurable
{
public:

  static QString className() { return "hoot::RandomElementRemover"; }

  /**
   * @arg p Probability that any given feature will be removed.
   */
  RandomElementRemover();
  ~RandomElementRemover() = default;

  void setConfiguration(const Settings& conf) override;

  void setRng(boost::minstd_rand& rng) override { _rng = &rng; }

  void visit(const ConstElementPtr& e) override;
  void visit(const std::shared_ptr<Element>& /*e*/) override { }

  QString getInitStatusMessage() const override
  { return "Removing random elements..."; }
  QString getCompletedStatusMessage() const override
  { return "Randomly removed " + QString::number(_numAffected) + " elements"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Randomly removes elements from a map"; }

  /**
   * Set the probability that a feature will be removed.
   */
  void setProbability(double p) { _p = p; }

private:

  boost::minstd_rand* _rng;
  std::shared_ptr<boost::minstd_rand> _localRng;
  double _p;
};

}

#endif // RANDOM_ELEMENT_REMOVER_H
