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

#ifndef __PLUGIN_FACTORY_H__
#define __PLUGIN_FACTORY_H__

// Standard Includes
#include <string>

// Boost includes
#include <boost/any.hpp>


#ifdef _WIN32
#pragma warning(disable: 4190)//ignore c-linkage warning
#define PLUGIN_EXPORT __declspec(dllexport)
# pragma warning (disable : 4251) // disable exported dll function
#else
#define PLUGIN_EXPORT
#endif

extern "C" {
  PLUGIN_EXPORT boost::any createObjectNoParams(const std::string& className);
  PLUGIN_EXPORT const char* getVersion();
}

class PluginFactory
{
public:
  static std::string testName() { return "TgsTest"; }
};

#endif
