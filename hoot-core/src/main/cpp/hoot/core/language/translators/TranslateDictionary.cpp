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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "TranslateDictionary.h"

// Hoot
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;

namespace hoot
{

boost::shared_ptr<TranslateDictionary> TranslateDictionary::_theInstance = NULL;

TranslateDictionary::TranslateDictionary()
{
}

TranslateDictionary& TranslateDictionary::getInstance()
{
  if (_theInstance == NULL)
  {
    QString dictionary = ConfPath::search("dictionary.json");
    _theInstance.reset(new TranslateDictionary());
    _theInstance->load(dictionary);
  }
  return *_theInstance;
}

void TranslateDictionary::load(QString path)
{
  try
  {
    pt::ptree pt;
    pt::read_json(path.toAscii().data(), pt);

    _loadTags(pt);
  }
  catch (const std::exception& e)
  {
    QString reason = e.what();
    throw HootException("Error parsing JSON. " + reason);
  }
}

void TranslateDictionary::_loadTags(pt::ptree& tree)
{
  BOOST_FOREACH(pt::ptree::value_type& translation, tree.get_child("Dictionary"))
  {
    int i = 0;
    QString from;
    BOOST_FOREACH(pt::ptree::value_type& t, translation.second.get_child(""))
    {
      std::string s = t.second.data();
      if (i == 0)
        from = QString::fromUtf8(s.c_str()).toLower();
      else
        _translations[from].push_back(QString::fromUtf8(s.c_str()).toLower());
      i++;
    }
  }
}

}
