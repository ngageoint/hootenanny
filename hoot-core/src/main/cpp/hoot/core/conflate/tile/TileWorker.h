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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef TILEWORKER_H
#define TILEWORKER_H

// GDAL Includes
#include <ogrsf_frmts.h>

// GEOS
#include <geos/geom/Envelope.h>

// Hoot
#include <hoot/core/util/OpenCv.h>

// Qt
#include <QString>

// Standard
#include <vector>

namespace hoot
{

/**
 * A class that can perform the operations needed by the TileConflator.
 */
class TileWorker
{
public:

  TileWorker() {}

  virtual ~TileWorker() {}

  /**
   * Reads data from in1 & in2, breaks the ways up into smaller pieces if needed.
   * @param maxWaySize The maximum size of a way after breaking the way into smaller pieces.
   */
  virtual void breakWays(QString out) = 0;

  /**
   * Calculate the envelope that contains both in1 and in2.
   */
  virtual OGREnvelope calculateEnvelope() = 0;

  /**
   * Calculate the density of nodes for in1 & in2 and return a matrix representing the density.
   * @param in The path to the input map
   * @param pixelSize The size of the pixel in degrees.
   */
  virtual void calculateNodeDensity(cv::Mat& r1, cv::Mat& r2) = 0;

  /**
   * This is called after all conflate methods have been called and is a last opportunity to
   * cleanup the data for presentation to the user. At the very least this should move the data
   * from mapIn to mapOut. It may also remove internal information such as node replacement
   * maps.
   */
  virtual void cleanup(QString mapIn, QString mapOut) = 0;

  /**
   * Conflates all the elements that intersect an entry in "tiles".
   * @param tiles - The tiles to conflate. The tiles have already been expanded by buffer before
   *                the call.
   * @param mapIn - The path to the input map.
   * @param mapOut - The path to store the output map.
   */
  virtual void conflate(const std::vector<geos::geom::Envelope>& tiles, QString mapIn, QString mapOut) = 0;

  /**
   * Returns true if the specified path exists.
   */
  virtual bool exists(QString dir) = 0;

  /**
   * Returns the directory where work files should be stored. At the end of the conflation run
   * this directory will be removed.
   */
  virtual QString getWorkDir() const = 0;

  /**
   * Initializes the worker with necessary values. This is guaranteed to be called before any other
   * method.
   * @param in1 First input, guaranteed to not change throughout the life of the object.
   * @param in2 Second input, guaranteed to not change throughout the life of the object.
   * @param buffer Size of the buffer around tiles. This is also the max way size and pixel size.
   *        specified in degrees.
   */
  virtual void init(QString in1, QString in2, double buffer) = 0;

  /**
   * Creates a new directory and all the necessary parent directories.
   */
  virtual void mkdir(QString dir) = 0;

  /**
   * Recursively removes a directory and all its children.
   */
  virtual void rmdir(QString dir) = 0;
};

}

#endif // TILEWORKER_H
