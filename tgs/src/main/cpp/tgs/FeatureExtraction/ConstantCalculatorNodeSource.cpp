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
 * @copyright Copyright (C) 2015, 2016, 2017, 2021 Maxar (http://www.maxar.com/)
 */

#include "ConstantCalculatorNodeSource.h"

// Standard Includes
#include <iostream>
#include <math.h>
#include <sstream>
using namespace std;

#include "../StreamUtils.h"
#include "../TgsException.h"
#include "../Statistics/Random.h"
#include "../Xml/XmlHelper.h"

namespace Tgs
{
  void ConstantCalculatorNodeSource::_copyInternalData(const CalculatorGenomeNode& node)
  {
    const ConstantCalculatorNodeSource& tmp =
      dynamic_cast<const ConstantCalculatorNodeSource&>(node);

    _v = tmp._v;
    _sigma = tmp._sigma;
    _oldVs = tmp._oldVs;
  }

  void ConstantCalculatorNodeSource::_loadInternals(std::istream& s)
  {
    std::string buffer;
    std::string tag, value;

    std::getline(s, buffer);
    XmlHelper::parseTagValue(buffer, tag, value);
    if (tag != "value")
    {
      throw Exception("Error parsing ConstantCalculatorNodeSource internals");
    }
    _v = conv<double>(value);

    std::getline(s, buffer);
    XmlHelper::parseTagValue(buffer, tag, value);
    if (tag != "sigma")
    {
      throw Exception("Error parsing ConstantCalculatorNodeSource internals");
    }
    _sigma = conv<double>(value);
  }

  void ConstantCalculatorNodeSource::mutate(double /*p*/)
  {
    if (_sigma <= 0)
    {
      _sigma = 1.0;
    }
    double newV = Random::instance()->generateGaussian(_v, _sigma);
    _oldVs.push_back(newV);

    double mean = 0.0;
    for (unsigned int i = 0; i < _oldVs.size(); i++)
    {
      mean += _oldVs[i] / (double)_oldVs.size();
    }
    double sum = 0.0;
    for (unsigned int i = 0; i < _oldVs.size(); i++)
    {
      sum += (_oldVs[i] - mean) * (_oldVs[i] - mean) / (double)_oldVs.size();
    }
    _sigma = sqrt(sum);
    _v = newV;
  }

  void ConstantCalculatorNodeSource::_saveInternals(std::ostream& s,
    const std::string& indent) const
  {
    s << indent << "<value>" << _v << "</value>" << endl;
    s << indent << "<sigma>" << _sigma << "</sigma>" << endl;
  }

  std::string ConstantCalculatorNodeSource::_toLabel() const
  {
    stringstream strm;
    strm << _v;
    return strm.str();
  }
}
