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
#ifndef EDGESUBLINEMATCH_H
#define EDGESUBLINEMATCH_H

#include "EdgeSubline.h"

namespace hoot
{

/**
 * This class represents a matching subline between two edges.
 *
 * This class parallels WaySublineMatch.
 */
class EdgeSublineMatch
{
public:
  EdgeSublineMatch(ConstEdgeSublinePtr es1, ConstEdgeSublinePtr es2) : _es1(es1), _es2(es2) {}

  const ConstEdgeSublinePtr& getSubline1() const { return _es1; }

  const ConstEdgeSublinePtr& getSubline2() const { return _es2; }

  QString toString() const;

private:
  ConstEdgeSublinePtr _es1, _es2;

};

typedef shared_ptr<EdgeSublineMatch> EdgeSublineMatchPtr;
typedef shared_ptr<const EdgeSublineMatch> ConstEdgeSublineMatchPtr;

// not implemented
bool operator<(ConstEdgeSublineMatchPtr, ConstEdgeSublineMatchPtr);

}

#endif // EDGESUBLINEMATCH_H
