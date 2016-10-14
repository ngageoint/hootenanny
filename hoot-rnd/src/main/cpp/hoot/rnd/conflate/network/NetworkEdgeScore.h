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
#ifndef NETWORKEDGESCORE_H
#define NETWORKEDGESCORE_H

#include "EdgeMatch.h"

namespace hoot
{

class NetworkEdgeScore
{
public:
  NetworkEdgeScore(ConstEdgeMatchPtr em, double score12, double score21) :
    _em(em),
    _score12(score12),
    _score21(score21)
  {
  }

  ConstEdgeMatchPtr getEdgeMatch() const { return _em; }

  double getScore() const { return sqrt(_score12 * _score21); }

  double getScore12() const { return _score12; }

  double getScore21() const { return _score21; }

  QString getUid() const { return _uid; }

  void setUid(const QString uid) { _uid = uid; }

  QString toString() const
  {
    return QString("em: %1 score: %3").arg(_em->toString()).
      arg(getScore());
  }

private:
  ConstEdgeMatchPtr _em;
  double _score12, _score21;
  QString _uid;

};

typedef shared_ptr<NetworkEdgeScore> NetworkEdgeScorePtr;

// not implemented
bool operator<(NetworkEdgeScorePtr, NetworkEdgeScorePtr);

}

#endif // NETWORKEDGESCORE_H
