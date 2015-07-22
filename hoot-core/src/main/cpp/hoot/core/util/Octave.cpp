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
#include "Octave.h"

// hoot
#include <hoot/core/Exception.h>
#include <hoot/core/util/Log.h>

// Octave
#include <octave/parse.h>

// Tgs
#include <tgs/System/DisableCerr.h>

namespace hoot
{

using namespace Tgs;

Octave Octave::_theInstance;

Octave::Octave()
{
  _initialized = false;
}

Octave& Octave::getInstance()
{
  _theInstance._init();
  return _theInstance;
}

void Octave::_init()
{
  if (!_initialized)
  {
    // disable octave's whining about readline and X11 DISPLAY
    DisableCerr dc;
    string_vector argv(2);
    argv(0) = "embedded";
    argv(1) = "-q";
    if (octave_main(2, argv.c_str_vec(), 1) != 1)
    {
      throw Exception("Error initializing octave.");
    }
    _initialized = true;
  }
}

Matrix Octave::toOctave(const cv::Mat& m)
{
  Matrix result(m.rows, m.cols, CV_64F);
  for (int i = 0; i < m.rows; ++i)
  {
    for (int j = 0; j < m.cols; j++)
    {
      result(i, j) = m.at<double>(i, j);
    }
  }
  return result;
}

cv::Mat Octave::toOpenCv(const Matrix& m)
{
  cv::Mat result(m.dim1(), m.dim2(), CV_64F);
  for (int i = 0; i < result.rows; ++i)
  {
    for (int j = 0; j < result.cols; j++)
    {
      result.at<double>(i, j) = m(i, j);
    }
  }
  return result;
}

cv::Mat Octave::sqrtm(const cv::Mat& m)
{
  Matrix mOctave = toOctave(m);
  octave_value_list r = feval("sqrtm", octave_value(mOctave), 1);
  Matrix x(r(0).matrix_value());
  return toOpenCv(x);
}

}
