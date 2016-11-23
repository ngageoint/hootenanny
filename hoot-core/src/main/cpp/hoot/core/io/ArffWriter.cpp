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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ArffWriter.h"

// Hoot
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QStringList>

// standard
#include <set>

namespace hoot
{

ArffWriter::ArffWriter(ostream* strm, bool useNulls) : _strm(strm), _useNulls(useNulls)
{
}

ArffWriter::ArffWriter(QString path, bool useNulls) : _useNulls(useNulls)
{
  fstream* fs = new fstream();
  fs->exceptions(fstream::failbit | fstream::badbit);
  fs->open(path.toUtf8().data(), ios_base::out);
  _autoStrm.reset(fs);
  _strm = fs;
}

void ArffWriter::_w(const QString& s)
{
  (*_strm) << s.toUtf8().data() << endl;
}

void ArffWriter::write(const vector<Sample> &samples)
{
  LOG_INFO("Writing .arff file...");

  set<QString> attributes;

  for (vector<Sample>::const_iterator it = samples.begin(); it != samples.end(); ++it)
  {
    const Sample& s = *it;
    for (Sample::const_iterator jt = s.begin(); jt != s.end(); ++jt)
    {
      if (jt->first != "class")
      {
        attributes.insert(jt->first);
      }
    }
  }

  QString result;

  _w("@RELATION manipulations");
  _w("");

  for (set<QString>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
  {
    _w(QString("@ATTRIBUTE %1 NUMERIC").arg(*it));
  }
  _w("@ATTRIBUTE class {match,miss,review}\n");

  _w("@DATA");

  for (vector<Sample>::const_iterator st = samples.begin(); st != samples.end(); ++st)
  {
    const Sample& s = *st;

    QStringList l;
    for (set<QString>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
    {
      if (s.find(*it) != s.end())
      {
        double v = s.find(*it)->second;
        if (__isnan(v))
        {
          if (_useNulls)
          {
            l.append("?");
          }
          else
          {
            l.append("-1");
          }
        }
        else
        {
          // 17 digits is guaranteed to be the same by IEEE 754
          // http://en.wikipedia.org/wiki/Double-precision_floating-point_format
          l.append(QString("%1").arg(v, 0, 'g', 17));
        }
      }
      else
      {
        if (_useNulls)
        {
          l.append("?");
        }
        else
        {
          l.append("-1");
        }
      }
    }
    MatchType type = MatchType(s.find("class")->second);
    _w(l.join(",") + "," + type.toString().toLower());
  }
}

}
