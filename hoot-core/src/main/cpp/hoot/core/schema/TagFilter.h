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
#ifndef TAG_FILTER_H
#define TAG_FILTER_H

// Qt
#include <QString>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;

namespace hoot
{

/**
 *
 */
class TagFilter
{

public:

  TagFilter(const QString key, const QString value, const bool allowAliases,
            const double similarityThreshold);

  QString getKey() const { return _key; }
  QString getValue() const { return _value; }
  bool getAllowAliases() const { return _allowAliases; }
  double getSimilarityThreshold() const { return _similarityThreshold; }

  static TagFilter fromJson(const pt::ptree::value_type& tagFilterPart);

private:

  QString _key;
  QString _value;
  bool _allowAliases;
  double _similarityThreshold;
};

}

#endif // TAG_FILTER_H
