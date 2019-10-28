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
 * @copyright Copyright (C) 2017, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SIMPLETESTLISTENER_H
#define SIMPLETESTLISTENER_H

// Cpp Unit
#include <cppunit/TestListener.h>

namespace hoot
{

/**
 * Wrapper around CPPUnit test listener for test failure notification purposes
 */
class SimpleTestListener : public CppUnit::TestListener
{

public:

  SimpleTestListener();

  virtual void addFailure( const CppUnit::TestFailure & /*failure*/ ) { _failure = true; }
  bool isFailure() const { return _failure; }

private:

  bool _failure;
};

}

#endif // SIMPLETESTLISTENER_H
