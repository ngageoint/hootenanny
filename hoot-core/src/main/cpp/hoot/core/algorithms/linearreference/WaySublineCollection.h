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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAYSUBLINECOLLECTION_H
#define WAYSUBLINECOLLECTION_H

// Standard
#include <deque>

#include "WaySubline.h"

namespace hoot
{
using namespace std;

/**
 * Contains a continuous collection of ways. Each way subline must touch the next subline end to
 * start.
 */
class WaySublineCollection
{
public:

  typedef vector<WaySubline> SublineCollection;

  WaySublineCollection();

  void addSubline(const WaySubline& subline);

  /**
   * Create a new WaySublineCollection that represents the negative of the sublines on this
   * WaySublineCollection. This will only include sublines on the ways that are in this subline. If
   * the subline is empty then it will not be included.
   */
  WaySublineCollection invert() const;

  /**
   * Return the sum of the lengths of all thes sublines.
   */
  Meters getLength() const;

  const SublineCollection& getSublines() const { return _sublines; }

  QString toString() const;

  bool touches(const WaySublineCollection& other) const;

private:
  SublineCollection _sublines;

};

}

#endif // WAYSUBLINECOLLECTION_H
