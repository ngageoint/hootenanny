/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef HADOOPTILEWORKER2_H
#define HADOOPTILEWORKER2_H

#include <hoot/core/fourpass/TileWorker2.h>

#include "stats/MapStats.h"

namespace hoot
{

class HadoopTileWorker2 : public TileWorker2
{
public:

  static QString maxNodeCountKey() { return "hoot.hadoop.maxNodeCount"; }

  static QString pixelSizeKey() { return "hoot.hadoop.pixelSize"; }

  HadoopTileWorker2();

  virtual void applyOp(boost::shared_ptr<OsmMapOperation> op, const vector<Envelope>& tiles, QString mapIn,
                       QString mapOut);

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
  QString _in1, _in2;
  bool _initialized;
  double _maxWaySize;
  double _pixelSize;
  double _buffer;
  OGREnvelope _envelope;

  MapStats _stats1, _stats2;
  cv::Mat _r1, _r2;

  MapStats _calculateStats(QString in);
};

}

#endif // HADOOPTILEWORKER2_H
