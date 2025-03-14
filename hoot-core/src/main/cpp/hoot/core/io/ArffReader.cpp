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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "ArffReader.h"

// Boost
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

// Hoot
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/scoring/DataSamples.h>

using namespace std;

namespace hoot
{

ArffReader::ArffReader(istream* strm)
  : _strm(strm)
{
}

ArffReader::ArffReader(QString path)
  : _autoStrm(std::make_shared<std::fstream>())
{
  LOG_DEBUG("Opening " + path + " for input.");
  _autoStrm->exceptions(fstream::badbit);
  QByteArray arr = path.toUtf8();
  _autoStrm->open(arr.data(), ios_base::in | ios_base::binary);
  _strm = _autoStrm.get();

  if (path.endsWith(".bz2"))
  {
    _bstrm = std::make_shared<boost::iostreams::filtering_istream>();
    boost::iostreams::filtering_istream& zdat = *_bstrm;
    zdat.push(boost::iostreams::bzip2_decompressor());
    zdat.push(*_autoStrm.get());
    _strm = &zdat;
  }
}

bool ArffReader::_eof() const
{
  bool result;
  if (_bstrm.get())
    result = !(*_bstrm);
  else
    result = _strm->eof();
  return result;
}

std::shared_ptr<DataSamples> ArffReader::read()
{
  std::shared_ptr<DataSamples> result = std::make_shared<DataSamples>();
  DataSamples& ds = *result;
  QStringList columnNames;

  QString line;
  // read until we see the first @ATTRIBUTE
  while (line.startsWith("@ATTRIBUTE") == false && !_eof())
    line = _readLine();

  // read in all column names
  while (line.startsWith("@ATTRIBUTE") && !_eof())
  {
    QStringList l = line.split(" ");
    columnNames.append(l[1]);
    if (l[2] != "NUMERIC" && l[1] != "class")
      throw IoException("Got a non-numeric column. " + line);
    line = _readLine();
  }

  while (line.startsWith("@DATA") == false && !_eof())
    line = _readLine();

  if (_eof())
    throw IoException("File contains no data.");

  if (columnNames[columnNames.size() - 1] != "class")
    throw IoException("Expected the last column to be 'class'");

  line = _readLine();
  // read in each record as input
  do
  {
    QStringList l = line.split(",");

    if (l.size() != columnNames.size())
      throw IoException("Wrong number of data samples.");

    Sample s;
    for (int i = 0; i < l.size() - 1; i++)
    {
      if (l[i] != "?")
      {
        bool ok = false;
        s[columnNames[i]] = l[i].toDouble(&ok);
        if (!ok)
          throw IoException("Error parsing number: " + l[i]);
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
