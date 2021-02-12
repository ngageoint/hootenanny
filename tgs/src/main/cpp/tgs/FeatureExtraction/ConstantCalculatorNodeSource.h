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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__CONSTANT_CALCULATOR_NODE_SOURCE_H__
#define __TGS__CONSTANT_CALCULATOR_NODE_SOURCE_H__

// Standard Includes
#include <vector>

#include "CalculatorGenomeNode.h"

namespace Tgs
{
  class TGS_EXPORT ConstantCalculatorNodeSource : public CalculatorGenomeNode
  {
  public:
    ConstantCalculatorNodeSource() { _v = 0.0; _sigma = 1.0; }

    ConstantCalculatorNodeSource(double v, double sigma = -1) { _v = v; _sigma = sigma; }

    virtual ~ConstantCalculatorNodeSource() {}

    virtual const std::map<std::string, std::string> getInputs() const
    { return std::map<std::string, std::string>(); }

    virtual double getOutput(const int /*uid*/) const { return _v; }

    virtual void mutate(double p);

  protected:
    double _v, _sigma;
    std::vector<double> _oldVs;

    virtual void _copyInternalData(const CalculatorGenomeNode& node);

    virtual CalculatorGenomeNode* _createNew() const { return new ConstantCalculatorNodeSource(); }

    double generateRandomGaussian(double mean, double sigma) const;

    virtual void _loadInternals(std::istream& s);

    virtual void _saveInternals(std::ostream& s, const std::string& indent = "") const;

    virtual std::string _toLabel() const;
  };
}


#endif
