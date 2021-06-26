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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

//Std Includes
#include <vector>

#include "Utility/Exception.h"
#include "Utility/NumericLimits.h"

namespace tbs
{

/**
 * A basic n-dimensional bounding box
 */
template<class T>
class BoundingBox
{
public:
  /**
   * @brief Constructor
   *
   * Constructs a 0 dimensional bounding box
   */
  BoundingBox();

  /**
   * @brief Constructor
   * @param dimensions the dimensionality of the bounding box
   */
  BoundingBox(int dimensions)
  {
    try
    {
      _bounds.resize(dimensions, std::pair<T,T>(numericLimitMax<T>(), numericLimitMin<T>()));
    }
    catch(const std::exception& e)
    {
      std::stringstream ss;
      ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
        ") -->" << std::endl << e.what();
      throw Exception(ss.str());
    }
  }

  /**
   * @brief floors the minimum values and ceilings the maximum value
   *
   * The delta is added to minimum value before flooring to handle ensure something like
   * 12.99999999 will be floored as 13 instead of 12.  The same applies for ceiling for
   * maximum values.
   *
   * @param delta a value to handle precision errors
   */
  void clampToInteger(double delta = 0.000001)
  {
    try
    {
      for(unsigned int i = 0; i < _bounds.size(); i++)
      {
        _bounds[i].first = floor((double)_bounds[i].first + delta);
        _bounds[i].second = ceil((double)_bounds[i].second - delta);
      }
    }
    catch(const std::exception& e)
    {
      std::stringstream ss;
      ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
        ") -->" << std::endl << e.what();
      throw Exception(ss.str());
    }
  }

  /**
   * @brief computeIntersection
   * @param otherBounds the bounding box to intersect with
   * @return the intersection of this bounding box with the other
   */
  BoundingBox<T> computeIntersection(const BoundingBox<T> & otherBounds)
  {
    try
    {
      _compareBoundingBox(otherBounds);

      BoundingBox<T> outputBounds(_bounds.size());

      T outputMin, outputMax;

      for(unsigned int i = 0; i < _bounds.size(); i++)
      {
        outputMin = std::max(_bounds[i].first, otherBounds.getMinimumValue(i));
        outputMax = std::min(_bounds[i].second, otherBounds.getMaximumValue(i));

        outputBounds.setRangeValues(i, outputMin, outputMax);
      }

      return outputBounds;
    }
    catch(const std::exception& e)
    {
      std::stringstream ss;
      ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
        ") -->" << std::endl << e.what();
      throw Exception(ss.str());
    }
  }

  /**
   * @brief computeUnion
   * @param otherBounds the bounding box to union with
   * @return the union of this bounding box with the other
   */
  BoundingBox<T> computeUnion(const BoundingBox<T> & otherBounds)
  {
    try
    {
      _compareBoundingBox(otherBounds);

      BoundingBox<T> outputBounds(_bounds.size());

      T outputMin, outputMax;

      for(unsigned int i = 0; i < _bounds.size(); i++)
      {
        outputMin = std::min(_bounds[i].first, otherBounds.getMinimumValue(i));
        outputMax = std::max(_bounds[i].second, otherBounds.getMaximumValue(i));

        outputBounds.setRangeValues(i, outputMin, outputMax);
      }

      return outputBounds;
    }
    catch(const std::exception& e)
    {
      std::stringstream ss;
      ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
        ") -->" << std::endl << e.what();
      throw Exception(ss.str());
    }
  }

  /**
   * @brief getMaximumValue
   * @param dimension the dimension of interest
   * @return the maximum value for the requested dimension
   */
  T getMaximumValue(unsigned int dimension)
  {
    try
    {
      _checkDimension(dimension);

      return _bounds[dimension].second;
    }
    catch(const std::exception& e)
    {
      std::stringstream ss;
      ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
        ") -->" << std::endl << e.what();
      throw Exception(ss.str());
    }
  }

  /**
   * @brief getMinimumValue
   * @param dimension the dimension of interest
   * @return the minimum value for the requested dimension
   */
  T getMinimumValue(unsigned int dimension)
  {
    try
    {
      _checkDimension(dimension);

      return _bounds[dimension].first;
    }
    catch(const std::exception& e)
    {
      std::stringstream ss;
      ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
        ") -->" << std::endl << e.what();
      throw Exception(ss.str());
    }
  }

  /**
   * @brief setRangeValues
   * @param dimension the dimension of interest
   * @param minValue the minimum range value
   * @param maxValue the maximum range value
   */
  void setRangeValues(unsigned int dimension, T minValue, T maxValue)
  {
    try
    {
      _checkDimension(dimension);

      _bounds[dimension] = std::pair<T,T>(minValue, maxValue);
    }
    catch(const std::exception& e)
    {
      std::stringstream ss;
      ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
        ") -->" << std::endl << e.what();
      throw Exception(ss.str());
    }

  }

  /**
   * @brief size
   * @return the dimensionality of the bounding box
   */
  unsigned int size(){return _bounds.size();}

  void updateRange(unsigned int dimension, T value)
  {
    try
    {
      _checkDimension(dimension);

      if(value < _bounds[dimension].first)
      {
        bounds[dimension].first = value;
      }

      if(value > _bounds[dimension].second)
      {
        bounds[dimension].second = value;
      }
    }
    catch(const std::exception& e)
    {
      std::stringstream ss;
      ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
        ") -->" << std::endl << e.what();
      throw Exception(ss.str());
    }
  }

  static const unsigned int DIM_X = 0;
  static const unsigned int DIM_Y = 1;
  static const unsigned int DIM_Z = 2;


private:
  /**
   * @brief checks to ensure the requested dimension exists within the boundsing box
   * @param dimension the dimension of interest
   */
  void _checkDimension(unsigned int dimension)
  {
    try
    {
      if(dimension >= _bounds.size())
      {
        std::stringstream ss;
        ss << "The requested dimension " << dimension <<
          " is equal to or exceeds the size of the bounding box (" << _bounds.size() << ")";
        throw Exception(__LINE__, ss.str());
      }
    }
    catch(const std::exception& e)
    {
      std::stringstream ss;
      ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
        ") -->" << std::endl << e.what();
      throw Exception(ss.str());
    }
  }

  /**
   * @brief check to ensure the input bounds have the same dimensionality as this one
   * @param otherBounds the other bounds to compare against
   */
  void _compareBoundingBox(const BoundingBox<T> & otherBounds)
  {
    try
    {
      if(otherBounds.size() != _bounds.size())
      {
        std::stringstream ss;
        ss << "The input bounding box's' dimensionality (" << otherBounds.size() <<
              ") does not equal this bounding box's dimensionality (" << _bounds.size() << ")";
        throw Exception(__LINE__, ss.str());
      }
    }
    catch(const std::exception& e)
    {
      std::stringstream ss;
      ss <<  typeid(this).name() << "::" << __FUNCTION__ << " Line(" << __LINE__ <<
        ") -->" << std::endl << e.what();
      throw Exception(ss.str());
    }
  }


  std::vector<std::pair<T,T> > _bounds;
};

}

#endif // BOUNDINGBOX_H
