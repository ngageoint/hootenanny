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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef DISABLECOUT_H
#define DISABLECOUT_H

// Standard
#include <ostream>

namespace Tgs
{

/**
 * Temporarily disable cout by redirecting to an empty ostream. When destructed cout is
 * re-established. This is mostly useful when instantiating poorly behaved libraries (e.g. STXXL)
 */
class DisableCout
{
public:

  DisableCout();

  ~DisableCout();

private:

  std::ostream _nullStream;
  std::streambuf* _oldBuf;
};

}

#endif // DISABLECOUT_H
