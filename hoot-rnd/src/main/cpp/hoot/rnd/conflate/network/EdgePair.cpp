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
#include "EdgePair.h"

namespace hoot
{

int EdgePair::count = 0;

EdgePair::EdgePair(ConstNetworkEdgePtr ep1, ConstNetworkEdgePtr ep2) : e1(ep1), e2(ep2)
{
  count++;
  id = count;
}

/**
 * Returns from for the second edge accounting for the reversal. E.g. getFrom1() will always
 * match to getFrom2().
 */
ConstNetworkVertexPtr EdgePair::getFrom2() const
{
  if (reversed)
  {
    return e2->getTo();
  }
  else
  {
    return e2->getFrom();
  }
}

/**
 * Returns from for the second edge accounting for the reversal. E.g. getTo1() will always
 * match to getTo2().
 */
ConstNetworkVertexPtr EdgePair::getTo2() const
{
  if (reversed)
  {
    return e2->getFrom();
  }
  else
  {
    return e2->getTo();
  }
}

QString EdgePair::toString() const
{
  return QString::number(id) + " " + e1->toString() + ", " + e2->toString() +
    (reversed ? "(rev)" : "");
}

}
