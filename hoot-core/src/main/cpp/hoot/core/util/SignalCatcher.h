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
#include <csignal>
#include <stdio.h>

#include <map>
#include <stack>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

/**
 * A class for catching and reporting a stack trace (hopefully) on error. This is very handy when
 * running code outside the debugger.
 */
class SignalCatcher
{
public:

  static boost::shared_ptr<SignalCatcher> getInstance();

  void registerDefaultHandlers();

  void registerHandler(unsigned int sig, __sighandler_t handler);

  void unregisterHandler(unsigned int sig);

private:

  SignalCatcher();

  static void default_handler(int sig);
  static void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63);
  static void terminateHandler();

  static boost::shared_ptr<SignalCatcher> _instance;

  std::map<unsigned int, std::stack<__sighandler_t> > _handlers;

  bool _defaultSet;

};

}

#endif // SIGNALCATCHER_H
