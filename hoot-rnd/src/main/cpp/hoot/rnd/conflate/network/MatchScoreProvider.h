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
#ifndef MATCHSCOREPROVIDER_H
#define MATCHSCOREPROVIDER_H

namespace hoot
{

/**
 * Provides match scores for specific aspects of a network.
 */
class MatchScoreProvider
{
public:

  /**
   * Returns the best estimate for the match score for the specified edges. Depending on the
   * class that implements this method the value may change over time.
   *
   * The same value should be returned regardless of the ordering of e1/e2.
   *
   * If there is no match or no score is available a zero is returned.
   */
  virtual double getEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const = 0;

  /**
   * Returns the best estimate for the match score for the specified vertices. Depending on the
   * class that implements this method the value may change over time.
   *
   * The same value should be returned regardless of the ordering of v1/v2.
   *
   * If there is no match or no score is available a zero is returned.
   */
  virtual double getVertexMatchScore(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2) const = 0;
};

typedef shared_ptr<MatchScoreProvider> MatchScoreProviderPtr;
typedef shared_ptr<const MatchScoreProvider> ConstMatchScoreProviderPtr;

}

#endif // MATCHSCOREPROVIDER_H
