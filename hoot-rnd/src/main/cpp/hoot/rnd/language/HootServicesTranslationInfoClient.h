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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HOOT_SERVICES_TRANSLATION_INFO_CLIENT_H
#define HOOT_SERVICES_TRANSLATION_INFO_CLIENT_H

// hoot
#include <hoot/rnd/language/TranslationInfoProvider.h>

// Qt
#include <QStringList>

namespace hoot
{

class HootNetworkCookieJar;

/**
 * Retrieves information from the Hootenanny web translation service about available languages
 */
class HootServicesTranslationInfoClient : public TranslationInfoProvider
{
public:

  static std::string className() { return "hoot::HootServicesTranslationInfoClient"; }

  HootServicesTranslationInfoClient();
  virtual ~HootServicesTranslationInfoClient() {}

  virtual void setConfiguration(const Settings& conf);

  /**
   * @see TranslationInfoProvider
   */
  virtual boost::shared_ptr<boost::property_tree::ptree> getAvailableApps(const QString type);

  /**
   * @see TranslationInfoProvider
   */
  virtual boost::shared_ptr<boost::property_tree::ptree> getAvailableLanguages(const QString type);

protected:

  bool _useCookies; //see note in HootServicesTranslatorClient

private:

  friend class HootServicesTranslationInfoClientTest;

  //a single translator used to determine what translatable languages are supported
  QString _translator;
  //detectors used to determine what detectable languages are supported; if left empty, then
  //all language detectors will be used by the service
  QStringList _detectors;

  QString _detectableUrl;
  QString _translatableUrl;
  QString _detectorsUrl;
  QString _translatorsUrl;

  boost::shared_ptr<HootNetworkCookieJar> _cookies;

  QString _getAvailableLanguagesRequestData(const QStringList apps);
};

}

#endif // HOOT_SERVICES_TRANSLATION_INFO_CLIENT_H
