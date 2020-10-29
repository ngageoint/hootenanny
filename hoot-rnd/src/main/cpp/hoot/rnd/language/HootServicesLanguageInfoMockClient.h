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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HOOT_SERVICES_LANGUAGE_INFO_MOCK_CLIENT_H
#define HOOT_SERVICES_LANGUAGE_INFO_MOCK_CLIENT_H

// hoot
#include <hoot/rnd/language/HootServicesLanguageInfoClient.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * This is used during testing to mock the behavior of the class that makes calls to hoot services
 * to get translation language information.
 */
class HootServicesLanguageInfoMockClient : public HootServicesLanguageInfoClient
{

public:

  static const QString LANGS_STR;
  static const QString DETECTORS_STR;
  static const QString TRANSLATORS_STR;

  static std::string className() { return "hoot::HootServicesLanguageInfoMockClient"; }

  HootServicesLanguageInfoMockClient();
  virtual ~HootServicesLanguageInfoMockClient() = default;

  /**
   * Retrieves available language apps
   *
   * @param type type of app to retrieve; "translator" or "detector"
   * @return a property tree containing the language app information
   */
  virtual std::shared_ptr<boost::property_tree::ptree> getAvailableApps(const QString& type) override;

  /**
   * Retrieves translation available languages info
   *
   * @param type type of language information to retrieve; "translatable" or "detectable"
   * @return a property tree containing the language information
   */
  virtual std::shared_ptr<boost::property_tree::ptree> getAvailableLanguages(const QString& type) override;
};

}

#endif // HOOT_SERVICES_LANGUAGE_INFO_MOCK_CLIENT_H
