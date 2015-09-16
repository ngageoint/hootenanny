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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OCTAVE_H
#define OCTAVE_H

// octave
#include <octave/config.h>
#include <octave/octave.h>
#include <octave/oct.h>

// opencv
#include <opencv/cv.h>

namespace hoot
{

/**
 * Light weight wrapper around Octave.
 *
 * @copyright GPL
 * @note This code references the Octave library which is GPL.
 */
class Octave
{
public:
  static Octave& getInstance();

  /**
   * Calculates the principal square root of m.
   * http://octave.sourceforge.net/octave/function/sqrtm.html
   */
  cv::Mat sqrtm(const cv::Mat& m);

  Matrix toOctave(const cv::Mat& m);

  cv::Mat toOpenCv(const Matrix& m);

private:
  Octave();

  static Octave _theInstance;

  bool _initialized;

  void _init();

};

}

#endif // OCTAVE_H
