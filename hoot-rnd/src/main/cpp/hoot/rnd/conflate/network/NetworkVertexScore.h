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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NETWORKVERTEXSCORE_H
#define NETWORKVERTEXSCORE_H

#include "NetworkVertex.h"

namespace hoot
{

class NetworkVertexScore
{
public:
  NetworkVertexScore(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2, double score12,
    double score21) :
    _v1(v1),
    _v2(v2),
    _score12(score12),
    _score21(score21)
  {
  }

  ConstNetworkVertexPtr getV1() const { return _v1; }

  ConstNetworkVertexPtr getV2() const { return _v2; }

  double getScore() const { return sqrt(_score12 * _score21); }

  double getScore12() const { return _score12; }

  double getScore21() const { return _score21; }

  QString toString() const
  {
    return QString("v1: %1 v2: %2 score: %3").arg(_v1->toString()).arg(_v2->toString()).
      arg(getScore());
  }

private:
  ConstNetworkVertexPtr _v1, _v2;
  double _score12, _score21;

};

typedef shared_ptr<NetworkVertexScore> NetworkVertexScorePtr;

// not implemented
bool operator<(NetworkVertexScorePtr, NetworkVertexScorePtr);

}

#endif // NETWORKVERTEXSCORE_H
