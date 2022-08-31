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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "EdgeMatch.h"

namespace hoot
{

const QRegExp EdgeMatch::_portionReplaceRegEx = QRegExp("_portion: \\d+\\.\\d+");

// This could be made faster by only looking at the pointer for comparison. Unfortunately
// this would likely break IndexedEdgeMatchSet::contains. Another data structure in there could
// certainly alleviate this.
bool operator==(const ConstEdgeMatchPtr& em1, const ConstEdgeMatchPtr& em2)
{
  return em1.get() == em2.get() ||
        (em1->getString1() == em2->getString1() && em1->getString2() == em2->getString2());
}

EdgeMatch::EdgeMatch()
  : _edges1(std::make_shared<EdgeString>()),
    _edges2(std::make_shared<EdgeString>())
{
  _resetHash();
}

EdgeMatch::EdgeMatch(ConstEdgeStringPtr es1, ConstEdgeStringPtr es2)
  : _edges1(es1->clone()),
    _edges2(es2->clone())
{
  _resetHash();
}

std::shared_ptr<EdgeMatch> EdgeMatch::clone() const
{
  EdgeMatchPtr result = std::make_shared<EdgeMatch>();
  result->_edges1 = _edges1->clone();
  result->_edges2 = _edges2->clone();

  return result;
}

bool EdgeMatch::contains(const std::shared_ptr<const EdgeMatch>& other) const
{
  return getString1()->contains(other->getString1()) &&
    getString2()->contains(other->getString2());
}

bool EdgeMatch::contains(ConstNetworkEdgePtr e) const
{
  return getString1()->contains(e) || getString2()->contains(e);
}

bool EdgeMatch::contains(ConstNetworkVertexPtr v) const
{
  return getString1()->contains(v) || getString2()->contains(v);
}

int EdgeMatch::countPartialMatches() const
{
  int count = 0;
  if (getString1()->isFromOnVertex() == false)
    count++;
  if (getString1()->isToOnVertex() == false)
    count++;
  if (getString2()->isFromOnVertex() == false)
    count++;
  if (getString2()->isToOnVertex() == false)
    count++;
  return count;
}

bool EdgeMatch::overlaps(const std::shared_ptr<const EdgeMatch>& other) const
{
  if (other->getString1()->overlaps(getString1()))
  {
    LOG_TRACE("Overlaps; this edge: " << toString() << " other edge: " << other);
    return true;
  }

  if (other->getString2()->overlaps(getString2()))
  {
    LOG_TRACE("Overlaps; this edge: " << toString() << " other edge: " << other);
    return true;
  }

  return false;
}

QString EdgeMatch::getUid() const
{
  QString hexStr;
  hexStr.setNum(_hash, 16);
  return hexStr;
}

QString EdgeMatch::toString() const
{
  return QString("s1: %1 s2: %2").arg(_edges1->toString(), _edges2->toString());
}

QString EdgeMatch::getSimilarityString() const
{
  return
    QString("s1: %1 s2: %2")
      .arg(getString1()->toString().replace(_portionReplaceRegEx, ""),
           getString2()->toString().replace(_portionReplaceRegEx, ""));
}

QString EdgeMatch::getFirstReversedSimilarityString() const
{
  EdgeMatchPtr edgeMatch = clone();
  const QString edgeStr2 = edgeMatch->getString2()->toString().replace(_portionReplaceRegEx, "");

  edgeMatch->reverse();
  const QString edgeStr1Reversed = edgeMatch->getString1()->toString().replace(_portionReplaceRegEx, "");

  return QString("s1r: %1 s2: %2").arg(edgeStr1Reversed, edgeStr2);
}

QString EdgeMatch::getSecondReversedSimilarityString() const
{
  EdgeMatchPtr edgeMatch = clone();
  const QString edgeStr1 = edgeMatch->getString1()->toString().replace(_portionReplaceRegEx, "");

  edgeMatch->reverse();
  const QString edgeStr2Reversed = edgeMatch->getString2()->toString().replace(_portionReplaceRegEx, "");

  return QString("s1: %1 s2r: %2").arg(edgeStr1, edgeStr2Reversed);
}

}
