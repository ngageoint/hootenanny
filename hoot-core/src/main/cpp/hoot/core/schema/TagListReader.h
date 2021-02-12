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
 * @copyright Copyright (C) 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#ifndef TAGLISTREADER_H
#define TAGLISTREADER_H

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Reads tag lists from a file
 */
class TagListReader
{

public:

  /**
   * Reads a list of tags from a newline delimited file
   *
   * @param inputPath path to the input file
   * @param keysOnly if true; assumes line is made up of the tag key only and doesn't require a kvp
   * with '=' per record
   * @return a string list of tags
   */
  static QStringList readList(const QString& inputPath, const bool keysOnly = false);
};

}

#endif // TAGLISTREADER_H
