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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "HootException.h"

#include <hoot/core/util/Factory.h>

namespace hoot
{

// the base class is a special case that doesn't need to be registered with the
// HootExceptionThrower.
HOOT_FACTORY_REGISTER_BASE(HootException)
HOOT_REGISTER_EXCEPTION(FileNotFoundException)
HOOT_REGISTER_EXCEPTION(IllegalArgumentException)
HOOT_REGISTER_EXCEPTION(InternalErrorException)
HOOT_REGISTER_EXCEPTION(NeedsReviewException)
HOOT_REGISTER_EXCEPTION(UnsupportedException)
HOOT_REGISTER_EXCEPTION(NotImplementedException)

HootExceptionThrower* HootExceptionThrower::_theInstance = 0;

HootExceptionThrower& HootExceptionThrower::getInstance()
{
  if (_theInstance == 0)
  {
    _theInstance = new HootExceptionThrower();
  }
  return *_theInstance;
}

void HootExceptionThrower::rethrowPointer(HootException* e)
{
  for (int i = 0; i < _throwMethods.size(); i++)
  {
    // try to throw the exception, if it isn't relevant nothing happens
    _throwMethods[i](e);
  }

  // couldn't find a specific exception type, throw the base class.
  throw HootException(*e);
}

}
