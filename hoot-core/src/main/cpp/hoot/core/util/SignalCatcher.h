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

#ifndef SIGNALCATCHER_H
#define SIGNALCATCHER_H

#include <iostream>
#include <stdio.h>

namespace hoot
{

/**
 * A class for catching and reporting a stack trace (hopefully) on error. This is very handy when
 * running code outside the debugger.
 */
class SignalCatcher
{
public:

  static void handler(int sig);

  /** Print a demangled stack backtrace of the caller function to FILE* out. */
  static void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63);

  static void registerHandlers();

  static void terminateHandler();

};

}

#endif // SIGNALCATCHER_H
