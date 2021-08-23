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

#include "NodeReplacements.h"

// Hoot
#include <hoot/core/util/HootException.h>


// Qt




// Standard
#include <fstream>


using namespace std;

namespace hoot
{

long NodeReplacements::_getFinalReplacement(long oldId)
{
  set<long> touched;
  touched.insert(oldId);

  long last = oldId;
  while (_r.find(last) != _r.end())
  {
    last = _r[last];
    if (touched.find(last) != touched.end())
    {
      throw HootException("Circular reference.");
    }
    touched.insert(last);
  }

  return last;
}

void NodeReplacements::_read(QString fn)
{
  fstream is;
  QByteArray ba = fn.toLatin1();
  is.open(ba.data(), ios_base::in);
  _read(is);
  is.close();
}

void NodeReplacements::_read(istream& is)
{
  ////
  // NOTE: For optimization reasons ConflateMapper has internal knowledge of this format. If you
  // change this format please address that class as well.
  ////
  int64_t ids[2];
  while (!is.eof())
  {
    is.read((char*)&ids, sizeof(ids));
    if (is.gcount() != 0 && is.gcount() != sizeof(ids))
    {
      throw HootException("Didn't read the expected number of bytes.");
    }

    if (is.gcount() == sizeof(ids))
    {
      _r[ids[0]] = ids[1];
    }
    else
    {
      break;
    }
  }
}

void NodeReplacements::readDir(QString inputDir)
{
  QStringList filters;
  filters << "*.replacement";
  QDir d(inputDir);
  Q_FOREACH(QFileInfo info, d.entryList(filters, QDir::Files, QDir::Name))
  {
    QString fn = inputDir + "/" + info.filePath();
    _read(fn);
  }
  simplify();
}

void NodeReplacements::simplify()
{
  HashMap<long, long> copy = _r;
  for (HashMap<long, long>::const_iterator it = copy.begin(); it != copy.end(); ++it)
  {
    long oldId = it->first;
    long newId = _getFinalReplacement(oldId);
    _r[oldId] = newId;
  }
}

QString NodeReplacements::toString() const
{
  QString result = "";

  for (HashMap<long, long>::const_iterator it = _r.begin(); it != _r.end(); ++it)
  {
    result += QString("%1 : %2\n").arg(it->first).arg(it->second);
  }

  return result;
}

void NodeReplacements::write(QString fn)
{
  fstream os;
  QByteArray ba = fn.toLatin1();
  os.open(ba.data(), ios_base::out);
  write(os);
  os.close();
}

void NodeReplacements::_write(ostream& os)
{
  ////
  // NOTE: For optimization reasons ConflateMapper has internal knowledge of this format. If you
  // change this format please address that class as well.
  ////
  int64_t ids[2];
  for (HashMap<long, long>::const_iterator it = _r.begin(); it != _r.end(); ++it)
  {
    ids[0] = it->first;
    ids[1] = it->second;

    os.write((char*)&ids, sizeof(ids));
  }
}



}
