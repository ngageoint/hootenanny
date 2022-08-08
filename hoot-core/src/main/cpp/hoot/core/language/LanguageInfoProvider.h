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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */


#ifndef LANGUAGE_INFO_PROVIDER_H
#define LANGUAGE_INFO_PROVIDER_H

// Hoot
#include <hoot/core/util/Configurable.h>

// Boost
#include <boost/property_tree/json_parser.hpp>

namespace hoot
{

/**
 * Interface for providing info about available to English translation capabilities
 */
class LanguageInfoProvider : public Configurable
{
public:

  static QString className() { return "LanguageInfoProvider"; }

  LanguageInfoProvider() = default;
  virtual ~LanguageInfoProvider() = default;

  /**
   * Retrieves available language apps
   *
   * @param type type of app to retrieve; "translator" or "detector"
   * @return a property tree containing the language app information
   */
  virtual std::shared_ptr<boost::property_tree::ptree> getAvailableApps(const QString& type) = 0;

  /**
   * Retrieves translation available languages info
   *
   * @param type type of language information to retrieve; "translatable" or "detectable"
   * @return a property tree containing the language information
   */
  virtual std::shared_ptr<boost::property_tree::ptree> getAvailableLanguages(const QString& type) = 0;
};

}

#endif // LANGUAGE_INFO_PROVIDER_H
