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
 * @copyright Copyright (C) 2022-2023 Maxar (http://www.maxar.com/)
 */

#include "OverpassReaderInterface.h"

//  Qt
#include <QRegularExpression>

//  Hootenanny
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

OverpassReaderInterface::OverpassReaderInterface()
  : _isOverpass(false),
    _queryFilepath(ConfigOptions().getOverpassApiQueryPath())
{
}

bool OverpassReaderInterface::isOverpassUrl(const QString& url) const
{
  QUrl overpassUrl(url);
  return overpassUrl.host() == ConfigOptions().getOverpassApiHost() && ("http" == overpassUrl.scheme() || "https" == overpassUrl.scheme());
}

QString OverpassReaderInterface::parseOverpassError(const QString& result) const
{
  //  <p><strong style="color:#FF0000">Error</strong>: line 1: parse error: Unknown query clause </p>
  static QRegularExpression regex("<p><strong.*?>Error</strong>: (.*?) *</p>", QRegularExpression::OptimizeOnFirstUsageOption);
  QStringList errors;
  auto match_it = regex.globalMatch(result);
  while (match_it.hasNext())
    errors << match_it.next().captured(1);
  return errors.join("\n");
}

bool OverpassReaderInterface::isOverpassXml(const QString& url) const
{
  //  Must be an overpass query
  if (!isOverpassUrl(url))
    return false;
  //  Load the query text if requested
  QString query = _readOverpassQueryFile(_queryFilepath);
  //  Search the URL and the query for the output format
  if (url.contains("[out:xml]") || query.contains("[out:xml]"))
    return true;
  //  Since the default return type is XML, if nothing is stated, then it is XML
  if (!url.contains("[out:") && !query.contains("[out:"))
    return true;
  //  Not XML
  return false;
}

bool OverpassReaderInterface::isOverpassJson(const QString& url) const
{
  //  Must be an overpass query
  if (!isOverpassUrl(url))
    return false;
  //  Load the query text if requested
  QString query = _readOverpassQueryFile(_queryFilepath);
  //  Search the URL and the query for the output format
  if (url.contains("[out:json]") || query.contains("[out:json]"))
    return true;
  //  Not JSON
  return false;
}

void OverpassReaderInterface::setOverpassUrl(const QString& url)
{
  _isOverpass = isOverpassUrl(url);
}

QString OverpassReaderInterface::_readOverpassQueryFile(const QString& path) const
{
  //  C-Style comments are valid and should be removed
  static QRegularExpression singleLineComment("\\s*//.*", QRegularExpression::OptimizeOnFirstUsageOption);
  static QRegularExpression multiLineComment("/\\*.*?\\*/", QRegularExpression::OptimizeOnFirstUsageOption | QRegularExpression::DotMatchesEverythingOption);
  QString query;
  try
  {
    //  Load the contents of the overpass query file
    if (!path.isEmpty())
    {
      //  Remove all of the comment lines from the file
      query = FileUtils::readFully(path).replace(singleLineComment, "").replace(multiLineComment, "");
    }
  }
  catch (const HootException&)
  {
    //  Catch any errors, log the warning, and return empty query
    LOG_WARN("Unable to load overpassql query file");
  }
  return query;
}

}
