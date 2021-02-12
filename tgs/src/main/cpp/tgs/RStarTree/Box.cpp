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
 * @copyright Copyright (C) 2015, 2018 Maxar (http://www.maxar.com/)
 */

#include "Box.h"

// Standard Includes
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include <tgs/RStarTree/RTreeNode.h>

namespace Tgs
{
  Box::Box()
  {
    _valid = false;
    _dimensions = 0;
    for (int i = 0; i < MAX_DIMENSIONS; ++i)
      _lowerBound[i] = _upperBound[i] = 0.0;
  }

  Box::Box(const Box& b)
  {
    _dimensions = b._dimensions;
    _valid = b._valid;
    _copyArray(_lowerBound, b._lowerBound);
    _copyArray(_upperBound, b._upperBound);
  }

  Box::Box(int dimensions)
  {
    _valid = false;
    _dimensions = dimensions;
    for (int i = 0; i < MAX_DIMENSIONS; ++i)
      _lowerBound[i] = _upperBound[i] = 0.0;
  }

  double Box::calculateOverlap(const Box& b) const
  {
    assert(b.getDimensions() == getDimensions());
    double result = 1.0;
    for (int i = 0; i < _dimensions; i++)
    {
      double ub = std::min(getUpperBound(i), b.getUpperBound(i));
      double lb = std::max(getLowerBound(i), b.getLowerBound(i));
      if (ub - lb <= 0.0)
      {
        return 0.0;
      }
      result *= ub - lb;
    }
    return result;
  }

  double Box::calculateOverlap(const BoxInternalData& b) const
  {
    assert(b.getDimensions() == getDimensions());
    double result = 1.0;
    for (int i = 0; i < _dimensions; i++)
    {
      double ub = std::min(getUpperBound(i), b.getUpperBound(i));
      double lb = std::max(getLowerBound(i), b.getLowerBound(i));
      if (ub - lb <= 0.0)
      {
        return 0.0;
      }
      result *= ub - lb;
    }
    return result;
  }

  double Box::calculatePerimeter() const
  {
    double result = 0.0;
    for (int i = 0; i < _dimensions; i++)
    {
      result += getUpperBound(i) - getLowerBound(i);
    }
    return result * 2.0;
  }

  double Box::calculateVolume() const
  {
    double result = 1;
    for (int i = 0; i < _dimensions; i++)
    {
      result *= getUpperBound(i) - getLowerBound(i);
    }
    return result;
  }

  Box& Box::expand(const BoxInternalData& b)
  {
    assert(getDimensions() == b.getDimensions());
    if (isValid() == false)
    {
      _valid = true;
      for (int i = 0; i < getDimensions(); i++)
      {
        _lowerBound[i] = b.getLowerBound(i);
        _upperBound[i] = b.getUpperBound(i);
      }
    }
    else
    {
      for (int i = 0; i < getDimensions(); i++)
      {
        _lowerBound[i] = std::min(getLowerBound(i), b.getLowerBound(i));
        _upperBound[i] = std::max(getUpperBound(i), b.getUpperBound(i));
      }
    }
    // for convenience
    return *this;
  }

  Box& Box::expand(const Box& b)
  {
    assert(getDimensions() == b.getDimensions());
    if (isValid() == false)
    {
      _valid = true;
      for (int i = 0; i < getDimensions(); i++)
      {
        _lowerBound[i] = b.getLowerBound(i);
        _upperBound[i] = b.getUpperBound(i);
      }
    }
    else
    {
      for (int i = 0; i < getDimensions(); i++)
      {
        _lowerBound[i] = std::min(getLowerBound(i), b.getLowerBound(i));
        _upperBound[i] = std::max(getUpperBound(i), b.getUpperBound(i));
      }
    }
    // for convenience
    return *this;
  }

  void Box::setBounds(int d, double lower, double upper)
  {
    _valid = true;
    _lowerBound[d] = lower;
    _upperBound[d] = upper;
  }

  void Box::setDimensions(int d)
  {
    _valid = false;
    _dimensions = d;
  }

  double Box::getLowerBound(int d) const
  {
    return std::min(_lowerBound[d], _upperBound[d]);
  }

  double Box::getUpperBound(int d) const
  {
    return std::max(_lowerBound[d], _upperBound[d]);
  }

  double Box::getLowerBoundRaw(int d) const
  {
    return _lowerBound[d];
  }

  double Box::getUpperBoundRaw(int d) const
  {
    return _upperBound[d];
  }

  bool Box::isValid() const
  {
    return _valid;
  }

  bool Box::operator==(const Box& b)
  {
    if (_dimensions != b._dimensions)
    {
      return false;
    }
    for (int i = 0; i < _dimensions; i++)
    {
      if (getLowerBound(i) != b.getLowerBound(i) ||
          getUpperBound(i) != b.getUpperBound(i))
      {
        return false;
      }
    }
    return true;
  }

  std::string Box::toString() const
  {
    std::stringstream lower;
    std::stringstream upper;
    std::string comma;
    for (int i = 0; i < _dimensions; i++)
    {
      lower << comma << getLowerBoundRaw(i);
      upper << comma << getUpperBoundRaw(i);
      comma = ", ";
    }
    return "(" + lower.str() + ") - (" + upper.str() + ")";
  }

  bool Box::isContained(const Box& b)
  {
    bool result = true;
    for (int i = 0; i < _dimensions; i++)
    {
      result = result && (b.getLowerBound(i) >= getLowerBound(i));
      result = result && (b.getUpperBound(i) <= getUpperBound(i));
    }
    return result;
  }

}
