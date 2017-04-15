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


#ifndef __TGS__BOX_H__
#define __TGS__BOX_H__

// Standard Includes
#include <string>
#include <string.h> // includes memcpy on Linux
#include <vector>

#include "../TgsExport.h"

namespace Tgs
{
  class BoxInternalData;

  class  TGS_EXPORT Box
  {
  public:
    static const int MAX_DIMENSIONS = 4;

    Box();

    Box(const Box& b);

    Box(int dimensions);

    ~Box() {}

    double calculateOverlap(const Box& b) const;

    double calculateOverlap(const BoxInternalData& b) const;

    double calculatePerimeter() const;

    double calculateVolume() const;

    // yes, technically this introduces a circular dependency, but the alternative is horribly
    // slow. Bleh! :P
    Box& expand(const BoxInternalData& b);

    Box& expand(const Box& b);

    void setBounds(int d, double lower, double upper);

    int getDimensions() const { return _dimensions; }

    double getLowerBound(int d) const;

    double getUpperBound(int d) const;

    double getLowerBoundRaw(int d) const;

    double getUpperBoundRaw(int d) const;

    bool isValid() const;

    bool operator==(const Box& b);

    void setDimensions(int d);

    std::string toString() const;

    bool isContained(const Box& b);

  private:
    double _lowerBound[MAX_DIMENSIONS];
    double _upperBound[MAX_DIMENSIONS];
    bool _valid;
    int _dimensions;

    void _copyArray(double* destination, const double* source)
    {
      memcpy(destination, source, sizeof(double) * MAX_DIMENSIONS);
    }
  };
}

#endif
