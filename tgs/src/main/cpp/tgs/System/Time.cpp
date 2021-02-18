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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */

#include "Time.h"

#ifdef _WIN32
// Windows Includes
# include <windows.h>
#else
# include <sys/time.h>
# include <time.h>
#endif

namespace Tgs
{

  double Time::getTime()
  {
    double result;
#   ifdef _WIN32
      LARGE_INTEGER liStop;
      QueryPerformanceCounter(&liStop);

      LARGE_INTEGER liFrequency;
      QueryPerformanceFrequency(&liFrequency);

      result = (double)(liStop.QuadPart) / (double)(liFrequency.QuadPart);
#   else
      struct timeval tv;
      gettimeofday(&tv, NULL);
      result = (double)tv.tv_sec + (double)tv.tv_usec / 1e6;
#   endif
    return result;
  }
}
