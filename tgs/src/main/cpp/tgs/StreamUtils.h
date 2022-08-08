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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__STREAM_UTILS_H__
#define __TGS__STREAM_UTILS_H__

// Standard
#include <deque>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <typeinfo>
#include <vector>

// Tgs
#include <tgs/HashMap.h>
#include <tgs/TgsException.h>
#include <tgs/TgsExport.h>

// Qt
#ifdef QT_CORE_LIB
# include <QHash>
# include <QList>
# include <QMap>
# include <QString>
# include <QVariant>
# include <QVector>
#endif

#if HAVE_LIBNEWMAT
// newmat
namespace NEWMAT
{
  class Matrix;
}
#endif

namespace Tgs
{
#if HAVE_LIBNEWMAT
  TGS_EXPORT std::ostream& operator<<(std::ostream & o, const NEWMAT::Matrix& matrix);
#endif

#include <tgs/StreamUtils.hh>

}

#endif
