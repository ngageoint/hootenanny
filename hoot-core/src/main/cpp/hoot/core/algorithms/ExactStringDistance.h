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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef __EXACT_STRING_DISTANCE_H__
#define __EXACT_STRING_DISTANCE_H__

// hoot
#include <hoot/core/algorithms/StringDistance.h>

// Qt
#include <QString>

// standard
#include <string>

namespace hoot
{
using namespace std;

/**
 * Returns 1 if the strings match exactly (ignoring case) and 0 otherwise.
 */
class ExactStringDistance : public StringDistance
{
public:

  static std::string className() { return "hoot::ExactStringDistance"; }

  ExactStringDistance();

  virtual double compare(const QString& s1, const QString& s2) const;

  QString toString() const { return "ExactStringDistance"; }
};

}

#endif // __EXACT_STRING_DISTANCE_H__
