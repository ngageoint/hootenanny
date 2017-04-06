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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MATCHDETAILS_H
#define MATCHDETAILS_H


// Qt
#include <QString>
#include <QVariant>

// Standard
#include <map>

namespace hoot
{
using namespace std;

/**
 * An interface that provides some deep dive details on a given match. This is useful for
 * determining why a match has made a given decision and possibly in experimenting with
 * reweighting the match through experimentation.
 */
class MatchDetails
{
public:

  virtual ~MatchDetails() {}

  /**
   * Returns a map of features that went into determining this as a match and their values. The
   * QVariant can either be a QString (nominal) or double (numeric).
   */
  virtual map<QString, double> getFeatures(const boost::shared_ptr<const OsmMap>& m) const = 0;
};

}

#endif // MATCHDETAILS_H
