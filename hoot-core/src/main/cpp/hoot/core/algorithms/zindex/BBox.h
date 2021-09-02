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
 * @copyright Copyright (C) 2016, 2017, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef BBOX_H
#define BBOX_H




namespace hoot
{

class BBox
{
public:

  static QString className() { return "BBox"; }

  BBox(const std::vector<double>& min, const std::vector<double>& max);
  BBox(const geos::geom::Envelope& envelope);
  virtual ~BBox();

  int getDimensions() const { return _max.size(); }

  std::vector<double> getMax() const { return _max; }
  std::vector<double> getMin() const { return _min; }

  bool in(const std::vector<double>& p) const;

  bool intersects(const BBox& b) const;

  QString toString() const;

  double getWidth(int d) const;

private:

  void _check() const;
  std::vector<double> _min;
  std::vector<double> _max;
};

}

#endif // BBOX_H
