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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESET_STATS_FORMAT_H
#define CHANGESET_STATS_FORMAT_H

// Hoot
#include <hoot/core/util/HootException.h>

// Qt
#include <QString>

namespace hoot
{

class ChangesetStatsFormat
{
public:

  typedef enum Format
  {
    Text = 0,
    Json,
    Unknown
  } Format;

  ChangesetStatsFormat() : _format(Unknown) { }
  ChangesetStatsFormat(Format format) : _format(format) { }

  void setFormat(Format format) { _format = format; }

  bool operator==(ChangesetStatsFormat f) const { return f._format == _format; }
  bool operator!=(ChangesetStatsFormat f) const { return f._format != _format; }

  Format getEnum() const { return _format; }

  static bool isValidFileOutputFormat(const QString& extension)
  {
    // Obviously this could change in the future but for now, the commands calling this expect to
    // output table text to the display only and the only file output format is JSON.
    return fromString(extension) == Format::Json;
  }

  QString toString() const
  {
    switch (_format)
    {
      case ChangesetStatsFormat::Text:
        return "Text";
      case ChangesetStatsFormat::Json:
        return "Json";
      default:
        return QString("Unknown (%1)").arg(_format);
    }
  }

  static Format fromString(QString str)
  {
    str = str.toLower();
    if (str == "text")
    {
      return Text;
    }
    else if (str == "json")
    {
      return Json;
    }
    else if (str == "unknown" || str.trimmed().isEmpty())
    {
      return Unknown;
    }
    return Unknown;
//    else
//    {
//      throw IllegalArgumentException("Invalid changeset statistics format string: " + str);
//    }
  }

private:

  ChangesetStatsFormat::Format _format;
};

}

#endif // CHANGESET_STATS_FORMAT_H
