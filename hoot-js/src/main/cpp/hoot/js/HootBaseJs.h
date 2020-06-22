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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HOOT_BASE_JS_H
#define HOOT_BASE_JS_H

// node.js
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{

/**
 * @brief The HootBaseJs class is a base class for all Hootenanny v8 Javascript
 * classes that cleans itself up correctly
 */
class HootBaseJs : public node::ObjectWrap
{
public:

  /** Virtual destructor for cleaning up the v8 objects */
  virtual ~HootBaseJs();

protected:

  HootBaseJs() = default;

};

}

#endif // HOOT_BASE_JS_H
