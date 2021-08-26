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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MULTIARYUTILITIES_H
#define MULTIARYUTILITIES_H

#ifndef SWIG
// hoot
#include <hoot/core/elements/OsmMap.h>
#endif

namespace hoot
{

class SearchBoundsCalculator;

/**
 * A simple class to represent relationships between nodes (Singleton).
 */
class MultiarySimpleMatch
{

public:

  MultiarySimpleMatch() = default;
  MultiarySimpleMatch(int nIndex, double s) : neighborIndex(nIndex), score(s) { }

  QString toString() const
  { return QString("{neighborIndex: %1, score: %2}").arg(neighborIndex).arg(score); }

  // The index of the elements that match in the provided vector.
  int neighborIndex;
  double score;
};

/**
 * A simple class to represent relationships between nodes.
 */
class MultiaryElement
{

public:

  MultiaryElement() = default;

#ifndef SWIG
  MultiaryElement(QByteArray h, geos::geom::Envelope b, QByteArray p) :
    _hash(h), _bounds(b), _payload(p) {}
#endif

  const geos::geom::Envelope& getBounds() const { return _bounds; }

  QString getHash() const { return _hash; }
  QByteArray getPayload() const { return _payload; }

  void setBounds(const geos::geom::Envelope& bounds) { _bounds = bounds; }
  void setHash(QString hash) { _hash = hash; }
  void setPayload(QByteArray payload) { _payload = payload; }

  QString toString() const { return QString("{hash: '%1', bounds: '%2', payload: '%3'}").
        arg(_hash).arg(_bounds.toString().data()).arg(_payload.toBase64().data()); }

private:

  QString _hash;
  geos::geom::Envelope _bounds;
  QByteArray _payload;
};

/**
 * Centralize some operations that occur over and over when using Multiary Conflation (Singleton).
 */
class MultiaryUtilities
{

public:

// OsmMap hasn't been wrapped, yet.
#ifndef SWIG
  /**
   * Configures the unifying conflator and perform a multiary conflation. This is a convenience
   * function to avoid a bunch of duplicate code.
   */
  static void conflate(OsmMapPtr map);
#endif

  /**
   * Conflate the provided elements when appropriate. These elements should have some
   * interrelationship, but even if they don't the data will be handled appropriately.
   *
   * @param pbfElements PBF representation of the elements to conflate.
   * @return A new list of elements that represent the conflated result.
   */
  static QList<MultiaryElement> conflateCluster(QList<QByteArray> pbfElements);

#ifndef SWIG
  static QByteArray convertElementToPbf(ConstNodePtr n);
#endif

  /**
   * Match from to the list of neighbors. If the score is zero it will be omitted from the list of
   * matches.
   * @param checkElement Compare this element against all its neighbors
   * @param againstElement neighbors to compare.
   * @return A list of all matches that have a score > 0.
   */
  static QList<hoot::MultiarySimpleMatch> findMatches(QByteArray checkElement,
    QList<QByteArray> againstElements);

  std::shared_ptr<SearchBoundsCalculator> getBoundsCalculator();

  static MultiaryUtilities& getInstance();

private:

  /** Default constructor/destructor */
  MultiaryUtilities() = default;
  ~MultiaryUtilities() = default;
  /** Delete copy constructor and assignment operator */
  MultiaryUtilities(const MultiaryUtilities&) = delete;
  MultiaryUtilities& operator=(const MultiaryUtilities&) = delete;

  std::shared_ptr<SearchBoundsCalculator> _searchBoundsCalculator;
};

}

#endif // MULTIARYUTILITIES_H
