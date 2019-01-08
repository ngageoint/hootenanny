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
  /*
   * non-sensical example, but illustrates all the possible filter inputs
   *
   * {
       "must":
       [
         {
           "filter": "tourism=hotel",
           "allowAliases": "true"
         }
       ]
       "should":
       [
         {
           "filter": "amenity=restaurant",
           "similarityThreshold": "0.8"
         },
         {
           "filter": "amenity=place_of_worship"
         },
         {
           "filter": "*address*=*"
         },
         {
           "filter": "poi*=*"
         },
         {
           "filter": "*building=*"
         },
         {
           "filter": "*=*address*"
         },
         {
           "filter": "*=poi*"
         },
         {
           "filter": "*=*building"
         }
       ],
       "must_not":
       [
         {
           "filter": "amenity=chapel"
         }
       ]
     }
   */

  const QString filter = QString::fromStdString(tagFilterPart.second.get<std::string>("filter"));
  LOG_VART(filter);
  if (filter.trimmed().isEmpty() || !filter.contains("="))
  {
    //throw
  }
  const QStringList filterParts = filter.split("=");
  const QString key = filterParts[0];
  LOG_VART(key);
  const QString value = filterParts[1];
  LOG_VART(value);

  // allowAliases and similarityThreshold are optional

  bool allowAliases = false;
  boost::optional<bool> allowAliasesProp = tagFilterPart.second.get_optional<bool>("allowAliases");
  if (allowAliasesProp)
  {
    allowAliases = allowAliasesProp.get();
  }
  LOG_VART(allowAliases);

  double similarityThreshold = -1.0;
  boost::optional<double> similarityThresholdProp =
    tagFilterPart.second.get_optional<double>("similarityThreshold");
  if (similarityThresholdProp)
  {
    similarityThreshold = similarityThresholdProp.get();
  }
  LOG_VART(similarityThreshold);

  return TagFilter(key, value, allowAliases, similarityThreshold);
}

QString TagFilter::toString() const
{
  QString str = "filter: " + _key + "=" + _value + ", allowAliases: ";
  if (_allowAliases)
  {
    str += "true";
  }
  else
  {
    str += "false";
  }
  if (_similarityThreshold != -1.0)
  {
    str += ", similarity threshold: " + QString::number(_similarityThreshold);
  }
  return str;
}

}
