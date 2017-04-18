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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ArffReader.h"

// Boost
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

// Hoot
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/scoring/DataSamples.h>

// Qt
#include <QStringList>

// standard
#include <set>

namespace hoot
{

ArffReader::ArffReader(istream* strm) : _strm(strm)
{
}

ArffReader::ArffReader(QString path)
{
  LOG_DEBUG("Opening " + path + " for input.");
  fstream* fs = new fstream();
  fs->exceptions(fstream::badbit);
  QByteArray arr = path.toUtf8();
  fs->open(arr.data(), ios_base::in | ios_base::binary);
  _autoStrm.reset(fs);
  _strm = fs;

  if (path.endsWith(".bz2"))
  {
    _bstrm.reset(new boost::iostreams::filtering_istream);
    boost::iostreams::filtering_istream& zdat = *_bstrm;
    zdat.push(boost::iostreams::bzip2_decompressor());
    zdat.push(*fs);
    _strm = &zdat;
  }
}

bool ArffReader::_eof()
{
  bool result;
  if (_bstrm.get())
  {
    result = !(*_bstrm);
  }
  else
  {
    result = _strm->eof();
  }

  return result;
}

boost::shared_ptr<DataSamples> ArffReader::read()
{
  boost::shared_ptr<DataSamples> result(new DataSamples());
  DataSamples& ds = *result;
  QStringList columnNames;

  QString line;
  // read until we see the first @ATTRIBUTE
  while (line.startsWith("@ATTRIBUTE") == false && !_eof())
  {
    line = _readLine();
  }

  // read in all column names
  while (line.startsWith("@ATTRIBUTE") && !_eof())
  {
    QStringList l = line.split(" ");
    columnNames.append(l[1]);
    if (l[2] != "NUMERIC")
    {
      if (l[1] != "class")
      {
        throw IoException("Got a non-numeric column. " + line);
      }
    }
    line = _readLine();
  }

  while (line.startsWith("@DATA") == false && !_eof())
  {
    line = _readLine();
  }

  if (_eof())
  {
    throw IoException("File contains no data.");
  }

  if (columnNames[columnNames.size() - 1] != "class")
  {
    throw IoException("Expected the last column to be 'class'");
  }

  line = _readLine();
  // read in each record as input
  do
  {
    QStringList l = line.split(",");

    if (l.size() != columnNames.size())
    {
      throw IoException("Wrong number of data samples.");
    }

    Sample s;
    for (int i = 0; i < l.size() - 1; i++)
    {
      if (l[i] != "?")
      {
        bool ok = false;
        s[columnNames[i]] = l[i].toDouble(&ok);
        if (!ok)
        {
          throw IoException("Error parsing number: " + l[i]);
        }
      }
    }

    s["class"] = MatchType(l[l.size() - 1]).toEnum();
    ds.push_back(s);

    line = _readLine();
  } while (!_eof());

  return result;
}

QString ArffReader::_readLine()
{
  string buffer;
  std::getline(*_strm, buffer);
  QString result = QString::fromUtf8(buffer.data());
  return result;
}

}
