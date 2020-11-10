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

#ifndef LOCALJOBRUNNER_H
#define LOCALJOBRUNNER_H

// Boost
#include <boost/shared_ptr.hpp>

// Pretty Pipes
#include <pp/InputFormat.h>

// Qt
// declaring our qHash before we include QHash. This is hackish, but I didn't see a better way.
uint qHash(const std::string& str);
#include <QHash>
#include <QMap>

namespace pp
{

/**
 * This simulates the execution of a hadoop job by running the job locally.
 */
class LocalJobRunner
{
public:
  // I started out using a QHashMap, but that gave inconsistent results from run to run. Switching
  // to QMap makes it consistent and makes unit testing easier.
  typedef QMap<QByteArray, std::vector<QByteArray> > ShuffleMap;

  LocalJobRunner(const Configuration& conf);

  void run();

private:
  Configuration _conf;

  int _reducerCount;

  // these are the shuffle results.
  ShuffleMap _shuffle;

  void _createInputSplits();
  void _createObjects();
  void _dumpShuffle();
  void _runMapper();
  void _runReducer();
};

}

#endif // LOCALJOBRUNNER_H

