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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TagFilter.h"

// Hoot
#include <hoot/core/util/Log.h>

namespace hoot
{

TagFilter::TagFilter(const QString key, const QString value, const bool allowAliases,
                     const double similarityThreshold) :
_key(key),
_value(value),
_allowAliases(allowAliases),
_similarityThreshold(similarityThreshold)
{
}

TagFilter TagFilter::fromJson(const pt::ptree::value_type& tagFilterPart)
{
  const QString filter = QString::fromStdString(tagFilterPart.second.get<std::string>("filter"));
  if (filter.trimmed().isEmpty() || !filter.contains("="))
  {
    //throw
  }
  const QStringList filterParts = filter.split("=");
  const QString key = filterParts[0];
  LOG_VART(key);
  const QString value = filterParts[1];
  LOG_VART(value);

  bool allowAliases = false;
  if (tagFilterPart.second.get<bool>("allowAliases"))
  {
    allowAliases = true;
  }
  LOG_VART(allowAliases);

  double similarityThreshold = -1.0;
  similarityThreshold = tagFilterPart.second.get<double>("similarityThreshold");
  LOG_VART(similarityThreshold);

  return TagFilter(key, value, allowAliases, similarityThreshold);
}


}
