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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef LONGBOX_H
#define LONGBOX_H

//Std Includes
#include <vector>

//Qt includes
#include <QString>

namespace hoot
{

/**
 * Used to represent a bounding box in Z-Value space.
 */
class LongBox
{
public:

  static QString className() { return "hoot::LongBox"; }

  LongBox() = default;
  LongBox(const std::vector<long int>& min, const std::vector<long int>& max);
   virtual ~LongBox();

  std::shared_ptr<LongBox> copy() const;

  long int calculateVolume() const;

  /**
    * Returns true if at least one edge overlaps with one of the other boxes
    * edges.
    */
  bool edgeOverlaps(const LongBox& b) const;

  LongBox expand(int size) const;

  int getDimensions() const { return getMin().size(); }

  std::vector<long int> getMax() const { return _max; }
  std::vector<long int> getMin() const { return _min; }

  void setMax(const std::vector<long int>& max) { _max = max; }
  void setMin(const std::vector<long int>& min) { _min = min; }

  bool in(const std::vector<long int>& p) const;

  /**
   * @brief toString
   * @return QString
   */
  QString toString() const;

  long getWidth(int d) const;

private:

  std::vector<long int> _min;
  std::vector<long int> _max;
};

}

#endif // LONGBOX_H
