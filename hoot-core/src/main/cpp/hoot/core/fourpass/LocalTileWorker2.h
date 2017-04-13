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

#ifndef LOCALTILEWORKER2_H
#define LOCALTILEWORKER2_H

// Boost
#include <boost/shared_ptr.hpp>

// Qt
#include <QDir>

// TGS
#include <tgs/HashMap.h>

#include "TileWorker2.h"

namespace hoot
{

class OsmMap;
class NodeReplacements;

using namespace boost;

/**
 * A tile worker that performs all its operations locally. This is good for testing, but it is
 * memory bound. All files are stored on the local filesystem.
 */
class LocalTileWorker2 : public TileWorker2
{
public:
  LocalTileWorker2();

  virtual void applyOp(boost::shared_ptr<OsmMapOperation> op, const vector<Envelope>& tiles,
                       QString mapIn, QString mapOut);

  virtual void breakWays(QString out);

  virtual OGREnvelope calculateEnvelope();

  virtual void calculateNodeDensity(cv::Mat& r1, cv::Mat& r2);

  virtual void cleanup(QString mapIn, QString mapOut);

  virtual bool exists(QString dir);

  virtual QString getWorkDir() const { return _workDir; }

  virtual void init(QString in1, QString in2, double buffer);

  virtual void mkdir(QString dir);

  virtual void rmdir(QString dir);

private:
  QString _workDir;
  int _mapPart;
  double _maxWaySize;
  double _pixelSize;
  QString _in1, _in2;
  double _buffer;

  /**
   * Read all the part files from the specified directory into a single map.
   */
  boost::shared_ptr<OsmMap> _readAllParts(QString dir);

  /**
   * Reads node replacements from the specified input directory. The result is a map that maps
   * the keys to replaced nodes and the values to the new node ids.
   */
  HashMap<long, long> _readNodeReplacements(QString inputDir);

  boost::shared_ptr<OsmMap> _readTile(QString input, const Envelope& e, double buffer);

  void _replaceNodes(boost::shared_ptr<OsmMap> map, const HashMap<long, long>& replacements);

  void _simplifyNodeReplacements(HashMap<long, long>& r);

  /**
   * Stores the specified map as a file in the specified directory. The file will not overlap with
   * other existing files in the directory.
   */
  void _storeMapPart(boost::shared_ptr<OsmMap> map, QString dir);

  void _writeNodeReplacements(QString dir, size_t i,
    NodeReplacements& replacements);

  void _writeTheRest(QString dirIn, QString dirOut, const vector<Envelope>& conflatedBits,
    double buffer);

};

}

#endif // LOCALTILEWORKER2_H
