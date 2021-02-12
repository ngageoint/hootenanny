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
 * @copyright Copyright (C) 2015, 2016, 2021 Maxar (http://www.maxar.com/)
 */

#include "PluginFactory.h"

// STL includes
#include <iostream>
#include <map>
#include <string>

// CPP Unit Includes
#include <cppunit/plugin/TestPlugIn.h>
#include <cppunit/extensions/HelperMacros.h>

#ifndef SA_VERSION
#define SA_VERSION "3.2.0"
#endif

#pragma GCC diagnostic ignored "-Wunused-parameter"
CPPUNIT_PLUGIN_IMPLEMENT();

boost::any createObjectNoParams(const std::string& className)
{
  if (className == "Tgs::TestSuite" || className == "TestSuite")
  {
    return CppUnit::TestFactoryRegistry::getRegistry(PluginFactory::testName()).makeTest();
  }
  return boost::any();
}

const char* getVersion() 
{ 
  return SA_VERSION; 
}

