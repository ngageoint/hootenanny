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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef NODEREPLACEMENTS_H
#define NODEREPLACEMENTS_H

// Qt
#include <QString>

// TGS
#include <tgs/HashMap.h>

namespace hoot
{

/**
 * This class keeps track of all the node replacements that have occured in an OsmMap. It also
 * supports import/export and simplifying node replacements.
 */
class NodeReplacements
{
public:

  NodeReplacements();

  HashMap<long, long>& getReplacements() { return _r; }

  /**
   * Reads all the files from the specified directory that end in .replacement.
   */
  void readDir(QString dir);

  void read(QString file) { _read(file); simplify(); }

  void read(std::istream& is) { _read(is); simplify(); }

  /**
   * Removes extra references. For example a->b, b->c simplifies to: a->c, b->c.
   */
  void simplify();

  QString toString() const;

  void write(QString fn);

  void write(std::ostream& os) { simplify(); _write(os); }

private:

  HashMap<long, long> _r;

  long _getFinalReplacement(long oldId);

  void _read(QString file);

  void _read(std::istream& is);

  void _write(std::ostream& os);
};

}

#endif // NODEREPLACEMENTS_H
