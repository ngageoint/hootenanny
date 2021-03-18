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
 * @copyright Copyright (C) 2012 Sebastian Morr <sebastian@morr.cc>
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef NELDERMEAD_H
#define NELDERMEAD_H

/**
 * @author Sebastian Morr <sebastian@morr.cc>
 * @copyright GPLv3
 * adapted from https://github.com/blinry/nelder-mead-optimizer
 * 2013-06-07 I've sent an email to the author requesting permission to use the code. -JRS
 * 2013-06-07 Got a response and he has updated the license on github to GPLv3. -JRS
 */

// Standard
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

// Tgs
#include <tgs/TgsException.h>
#include <tgs/Statistics/Random.h>

namespace Tgs
{

// Float vector with standard operations
class Vector
{
public:
    Vector() = default;
    ~Vector() = default;

    Vector(double c0)
    {
      coords.push_back(c0);
    }

    Vector(double c0, double c1)
    {
        coords.push_back(c0);
        coords.push_back(c1);
    }
    Vector(double c0, double c1, double c2)
    {
        coords.push_back(c0);
        coords.push_back(c1);
        coords.push_back(c2);
    }

    // add more constructors when N gets > 3

    double& operator[](int i)
    {
        return coords[i];
    }
    double at(int i) const
    {
        return coords[i];
    }
    int dimension() const
    {
        return coords.size();
    }
    void prepare(int size)
    {
        for (int i=0; i<size; i++)
        {
            coords.push_back(0);
        }
    }
    Vector operator+(Vector other)
    {
        Vector result;
        result.prepare(dimension());
        for (int i=0; i<dimension(); i++)
        {
            result[i] = coords[i] + other[i];
        }
        return result;
    }
    void operator+=(Vector other)
    {
        for (int i=0; i<dimension(); i++)
        {
            coords[i] += other[i];
        }
    }
    Vector operator-(Vector other)
    {
        Vector result;
        result.prepare(dimension());
        for (int i=0; i<dimension(); i++)
        {
            result[i] = coords[i] - other[i];
        }
        return result;
    }
    bool operator==(Vector other)
    {
        if (dimension() != other.dimension())
        {
            return false;
        }
        for (int i=0; i<dimension(); i++)
        {
            if (other[i] != coords[i])
            {
                return false;
            }
        }
        return true;
    }
    Vector operator*(double factor)
    {
        Vector result;
        result.prepare(dimension());
        for (int i=0; i<dimension(); i++)
        {
            result[i] = coords[i]*factor;
        }
        return result;
    }
    Vector operator/(double factor)
    {
        Vector result;
        result.prepare(dimension());
        for (int i=0; i<dimension(); i++)
        {
            result[i] = coords[i]/factor;
        }
        return result;
    }
    void operator/=(double factor)
    {
        for (int i=0; i<dimension(); i++)
        {
            coords[i] /= factor;
        }
    }
    bool operator<(const Vector& other) const
    {
        for (int i=0; i<dimension(); i++)
        {
            if (at(i) < other.at(i))
                return false;
            else if (at(i) > other.at(i))
                return true;
        }
        return false;
    }
    double length()
    {
        double sum = 0;
        for (int i=0; i<dimension(); i++)
        {
            sum += coords[i]*coords[i];
        }
        return pow(sum, 0.5);
    }

    const std::vector<double>& getVector() { return coords; }

private:

    std::vector<double> coords;
};

// This class stores known values for vectors. It throws unknown vectors.
class ValueDB
{
public:
  ValueDB() = default;

  double lookup(Vector vec)
  {
    if (!contains(vec))
    {
      throw Tgs::Exception("Vector was not found in DB. Internal error.");
    }
    else
    {
      return values[vec];
    }
  }

  void insert(Vector vec, double value)
  {
    values[vec] = value;
  }

  bool contains(Vector vec)
  {
    std::map<Vector, double>::iterator it = values.find(vec); // TODO add tolerance
    return it != values.end();
  }

private:
  // not implemented.
  ValueDB(const ValueDB&);
  ValueDB& operator=(const ValueDB&);

  std::map<Vector, double> values;
};

class NelderMead
{
public:

  class VectorSort
  {
  public:
    VectorSort(NelderMead* nm) : _nm(nm) { }

    // used in `step` to sort the vectors
    bool operator()(const Vector& a, const Vector& b)
    {
        return _nm->f(a) < _nm->f(b);
    }

    NelderMead* _nm;
  };

  /**
   * The function to optimize.
   */
  class Function
  {
  public:
    virtual ~Function() = default;

    virtual double f(Vector v) = 0;
  };

  /**
   * Takes ownership of the function object.
   */
  NelderMead(size_t dimension, Function* function, double termination_distance=0.001)
  {
    _function.reset(function);
    this->dimension = dimension;
    alpha = 1;
    gamma = 2;
    rho = -0.5;
    sigma = 0.5;
    this->termination_distance = termination_distance;
    _bestDistance = std::numeric_limits<double>::max();
    _noChange = 0;
    _maxNoChange = 4;
  }

  ~NelderMead() = default;

  // termination criteria: each pair of vectors in the simplex has to
  // have a distance of at most `termination_distance`
  bool done()
  {
    bool result = true;
    if (vectors.size() < dimension)
    {
      result = false;
    }
    else
    {
      for (size_t i=0; i<dimension+1; i++)
      {
        for (size_t j=0; j<dimension+1; j++)
        {
          if (i==j)
          {
            continue;
          }

          double l = (vectors[i] - vectors[j]).length();
          if (l < _bestDistance)
          {
            _noChange = 0;
            _bestDistance = l;
          }
          if (l > termination_distance)
          {
            result = false;
          }
        }
      }
    }

    _noChange++;
    if (_noChange > _maxNoChange)
    {
      result = true;
    }

    return result;
  }
  void insert(Vector vec)
  {
      if (vectors.size() < dimension+1)
      {
          vectors.push_back(vec);
      }
  }

  void setMaxNoChange(int maxNoChange) { _maxNoChange = maxNoChange; }

  Vector step(Vector vec, double score)
  {
    db.insert(vec, score);

    if (vectors.size() < dimension+1)
    {
        vectors.push_back(vec);
    }

    // otherwise: optimize!
    if (vectors.size() == dimension+1)
    {
        while (!done())
        {
          //cout << "count: " << count++ << endl;
          sort(vectors.begin(), vectors.end(), VectorSort(this));
            Vector cog; // center of gravity
            cog.prepare(dimension);
            for (size_t i = 1; i<=dimension; i++)
            {
                cog += vectors[i];
            }
            cog /= dimension;
            Vector best = vectors[dimension];
            Vector worst = vectors[0];
            Vector second_worst = vectors[1];
            // reflect
            Vector reflected = cog + (cog - worst)*alpha;
            if (f(reflected) > f(second_worst) && f(reflected) < f(best))
            {
                vectors[0] = reflected;
            } else if (f(reflected) > f(best))
            {
                // expand
                Vector expanded = cog + (cog - worst)*gamma;
                if (f(expanded) > f(reflected))
                {
                    vectors[0] = expanded;
                } else
                {
                    vectors[0] = reflected;
                }
            } else
            {
                // contract
                Vector contracted = cog + (cog - worst)*rho;
                if (f(contracted) > f(worst))
                {
                    vectors[0] = contracted;
                }
                else
                {
                    for (size_t i=0; i<dimension; i++)
                    {
                        vectors[i] = best + (vectors[i] - best)*sigma;
                    }
                }
            }
        }

        // algorithm is terminating, output: simplex' center of gravity
        Vector cog;
        cog.prepare(dimension);
        for (size_t i = 0; i<=dimension; i++)
        {
            cog += vectors[i];
        }
        return cog/(dimension+1);
    }
    else
    {
        // as long as we don't have enough vectors, request random ones,
        // with coordinates between 0 and 1. If you want other start vectors,
        // simply ignore these and use `step` on the vectors you want.
        Vector result;
        result.prepare(dimension);
        for (size_t i = 0; i<dimension; ++i)
        {
            result[i] = 0.001*(Tgs::Random::instance()->generateInt(1000));
        }
        return result;
    }
  }

private:
  // not implememnted
  NelderMead();
  NelderMead(const NelderMead&);
  NelderMead& operator=(const NelderMead&);

  double f(Vector vec)
  {
    double s;
    if (db.contains(vec) == false)
    {
      s = _function->f(vec);
      db.insert(vec, s);
    }
    else
    {
      s = db.lookup(vec);
    }
    return s;
  }

  std::shared_ptr<Function> _function;
  size_t dimension;
  double alpha, gamma, rho, sigma;
  double termination_distance;
  std::vector<Vector> vectors;
  ValueDB db;
  double _bestDistance;
  int _noChange;
  int _maxNoChange;
};

}
#endif // NELDERMEAD_H
