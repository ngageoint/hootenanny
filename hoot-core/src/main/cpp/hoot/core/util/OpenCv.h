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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */

#include <hoot/core/HootConfig.h>

#ifdef HOOT_HAVE_OPENCV2_OPENCV_HPP
// #1356 - OpenCV and Geos both have an int64 typedef
#define int64 opencv_broken_int
# include <opencv2/opencv.hpp>
#undef int64
#else
# if HOOT_HAVE_OPENCV_CV_H
#define int64 opencv_broken_int
#   include <opencv/cv.h>
#undef int64
# else
#   error "Expected an OpenCV header file to be defined."
# endif
#endif
