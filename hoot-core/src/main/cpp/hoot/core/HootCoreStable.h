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
 * @copyright Copyright (C) 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef HOOTCORESTABLE_H
#define HOOTCORESTABLE_H

// GEOS Includes
#include <geos/geom/Coordinate.h>
#include <geos/geom/Envelope.h>
#include <ogr_core.h>

// Qt
#include <QString>
#include <QVector>
#include <QMap>
#include <QSet>
#include <QList>
#include <QHash>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QStringList>

// Standard
#include <deque>
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <string.h> // includes memcpy on Linux
#include <vector>

// Note that any hoot include for a frequently updated class added here may have a detrimental
// effect on compile times.
//
// Deliberately not adding ConfigOptions.h here to try to take some of the compile sting out of
// config options changes when not temporarily updating hoot.json. Please add ConfigOptions.h
// includes directly to the class you are modifying rather than adding it here.

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

#endif // HOOTCORESTABLE_H
