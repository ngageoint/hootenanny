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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */


#ifndef HOOT_SERVICES_LANGUAGE_INFO_RESPONSE_PARSER_H
#define HOOT_SERVICES_LANGUAGE_INFO_RESPONSE_PARSER_H

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

// Qt
#include <QString>
#include <QMap>

namespace hoot
{

/**
 * Prints response info from the hoot translation service
 */
class HootServicesLanguageInfoResponseParser
{
public:

  /**
   * Returns an available languages response string
   *
   * @param type type of response to print; "translatable" or "detectable"
   * @param response the response to print
   * @return a display string
   */
  static QString parseAvailableLanguagesResponse(const QString& type,
                                                 const std::shared_ptr<boost::property_tree::ptree>& response);

  /**
   * Returns an available language apps response string
   *
   * @param type type of response to print; "translators" or "detectors"
   * @param response the response to print
   * @return a display string
   */
  static QString parseAvailableAppsResponse(const QString& type,
                                            const std::shared_ptr<boost::property_tree::ptree>& response);

  /**
   * Returns a mapping of ISO-6391-1 language codes to language names
   *
   * @param response a languages info response
   * @return a mapping of ISO-6391-1 language codes to language names
   */
  static QMap<QString, QString> getLangCodesToLangs(
    const std::shared_ptr<boost::property_tree::ptree>& response);
};

}

#endif // HOOT_SERVICES_LANGUAGE_INFO_RESPONSE_PARSER_H
