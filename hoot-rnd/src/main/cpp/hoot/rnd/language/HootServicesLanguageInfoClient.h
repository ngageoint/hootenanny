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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef HOOT_SERVICES_LANGUAGE_INFO_CLIENT_H
#define HOOT_SERVICES_LANGUAGE_INFO_CLIENT_H

// hoot
#include <hoot/rnd/language/LanguageInfoProvider.h>

// Qt
#include <QStringList>

namespace hoot
{

class HootNetworkCookieJar;

/**
 * Retrieves information from the Hootenanny web translation service about available languages
 */
class HootServicesLanguageInfoClient : public LanguageInfoProvider
{
public:

  static QString className() { return "hoot::HootServicesLanguageInfoClient"; }

  HootServicesLanguageInfoClient() : _useCookies(true), _timeout(500) { }
  virtual ~HootServicesLanguageInfoClient() = default;

  virtual void setConfiguration(const Settings& conf);

  /**
   * @see LanguageInfoProvider
   */
  virtual std::shared_ptr<boost::property_tree::ptree> getAvailableApps(const QString& type) override;

  /**
   * @see LanguageInfoProvider
   */
  virtual std::shared_ptr<boost::property_tree::ptree> getAvailableLanguages(const QString& type) override;

protected:

  bool _useCookies; //see note in HootServicesTranslatorClient

private:

  friend class HootServicesLanguageInfoClientTest;

  //a single translator used to determine what translatable languages are supported
  QString _translator;
  //detectors used to determine what detectable languages are supported; if left empty, then
  //all language detectors will be used by the service
  QStringList _detectors;

  QString _detectableUrl;
  QString _translatableUrl;
  QString _detectorsUrl;
  QString _translatorsUrl;

  std::shared_ptr<HootNetworkCookieJar> _cookies;

  static QString _getBaseUrl();
  static QString _getDetectableUrl();
  static QString _getTranslatableUrl();
  static QString _getDetectorsUrl();
  static QString _getTranslatorsUrl();

  QString _getAvailableLanguagesRequestData(const QStringList& apps);
  /** Timeout for HTTP requests in seconds */
  int _timeout;
};

}

#endif // HOOT_SERVICES_LANGUAGE_INFO_CLIENT_H
