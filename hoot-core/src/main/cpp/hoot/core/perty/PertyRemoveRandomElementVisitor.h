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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef PERTYREMOVERANDOMELEMENTVISITOR_H
#define PERTYREMOVERANDOMELEMENTVISITOR_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/util/Configurable.h>

#include "RngConsumer.h"

namespace hoot
{

class PertyRemoveRandomElementVisitor : public ElementOsmMapVisitor, public RngConsumer,
    public Configurable
{
public:

  static std::string className() { return "hoot::PertyRemoveRandomElementVisitor"; }

  /**
   * @arg p Probability that any given feature will be removed.
   */
  PertyRemoveRandomElementVisitor();

  virtual void setConfiguration(const Settings& conf);

  /**
   * Set the probability that a feature will be removed.
   */
  void setProbability(double p) { _p = p; }

  virtual void setRng(boost::minstd_rand& rng) { _rng = &rng; }

  virtual void visit(const ConstElementPtr& e);
  virtual void visit(const boost::shared_ptr<Element>& /*e*/) {}

  /**
    @see ElementVisitor
    */
  QString toString();

private:
  boost::minstd_rand* _rng;
  std::auto_ptr<boost::minstd_rand> _localRng;
  double _p;
};

}

#endif // PERTYREMOVERANDOMELEMENTVISITOR_H
