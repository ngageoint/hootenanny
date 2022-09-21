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
#ifndef RANDOM_ELEMENT_RENAMER_H
#define RANDOM_ELEMENT_RENAMER_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/RngConsumer.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * A simple random name changer.
 *
 * This is not part of the original PERTY paper.
 */
class RandomElementRenamer : public ElementVisitor, public RngConsumer, public Configurable
{
public:

  static QString className() { return "RandomElementRenamer"; }

  RandomElementRenamer();
  ~RandomElementRenamer() override = default;

  QString permuteName(const QString& s);

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  /**
   * @see RngConsumer
   */
  void setRng(boost::minstd_rand& rng) override { _rng = &rng; }

  /**
   * @see ElementVisitor
   */
  void visit(const std::shared_ptr<Element>& e) override;

  QString getInitStatusMessage() const override
  { return "Randomly changing element names..."; }
  QString getCompletedStatusMessage() const override
  { return "Randomly changed " + QString::number(_numAffected) + " element names"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Randomly changes element names"; }

  /**
   * The probability of a change to each character in the name. The expected number of changes is
   * perty.name.change.p * str.size()
   */
  void setChangeProbability(double changeP) { _changeP = changeP; }
  /**
   * Set the probability that a name will be modified.
   */
  void setProbability(double p) { _p = p; }

private:

  double _changeP;
  double _p;
  boost::minstd_rand* _rng;
  std::shared_ptr<boost::minstd_rand> _localRng;
};

}

#endif // RANDOM_ELEMENT_RENAMER_H
