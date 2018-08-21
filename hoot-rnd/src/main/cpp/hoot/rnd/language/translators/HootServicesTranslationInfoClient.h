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
#include <hoot/rnd/language/translators/ToEnglishTranslator.h>

// Qt
#include <QStringList>
#include <QNetworkAccessManager>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

namespace hoot
{

/**
 *
 */
class HootServicesTranslationInfoClient : public Configurable
{
public:

  HootServicesTranslationInfoClient();

  virtual void setConfiguration(const Settings& conf);

  boost::shared_ptr<boost::property_tree::ptree> getAvailableLanguages(const QString type);

private:

  QString _translator;
  QStringList _detectors;
  QString _detectableUrl;
  QString _translatableUrl;

  boost::shared_ptr<QNetworkAccessManager> _client;
};

}

#endif // HOOT_SERVICES_TRANSLATION_INFO_CLIENT_H
