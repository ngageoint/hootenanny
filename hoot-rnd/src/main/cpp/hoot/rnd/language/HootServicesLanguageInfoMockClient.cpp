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

#include "HootServicesLanguageInfoMockClient.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

const QString HootServicesLanguageInfoMockClient::LANGS_STR =
  "{\"languages\":[{\"name\":\"German\",\"available\":true,\"iso6391Code\":\"de\",\"iso6392Code\":\"deu\"},{\"name\":\"Spanish\",\"available\":true,\"iso6391Code\":\"es\",\"iso6392Code\":\"spa\"}]}";
const QString HootServicesLanguageInfoMockClient::DETECTORS_STR =
  "{\"apps\":[{\"name\":\"TikaLanguageDetector\",\"description\":\"blah\",\"url\":\"https://tika.apache.org\",\"supportsConfidence\":true},{\"name\":\"OpenNlpLanguageDetector\",\"description\":\"more blah\",\"url\":\"https://opennlp.apache.org\",\"supportsConfidence\":false}]}";
const QString HootServicesLanguageInfoMockClient::TRANSLATORS_STR =
  "{\"apps\":[{\"name\":\"JoshuaLanguageTranslator\",\"description\":\"blah\",\"url\":\"https://cwiki.apache.org/confluence/display/JOSHUA\"},{\"name\":\"HootLanguageTranslator\",\"description\":\"more blah\",\"url\":\"N/A\"}]}";

HOOT_FACTORY_REGISTER(LanguageInfoProvider, HootServicesLanguageInfoMockClient)

HootServicesLanguageInfoMockClient::HootServicesLanguageInfoMockClient() :
HootServicesLanguageInfoClient()
{
  _useCookies = false;
}

std::shared_ptr<boost::property_tree::ptree>
  HootServicesLanguageInfoMockClient::getAvailableApps(const QString& type)
{
  if (type == "detectors")
  {
    return StringUtils::jsonStringToPropTree(DETECTORS_STR);
  }
  else
  {
    return StringUtils::jsonStringToPropTree(TRANSLATORS_STR);
  }
}

std::shared_ptr<boost::property_tree::ptree>
  HootServicesLanguageInfoMockClient::getAvailableLanguages(const QString& /*type*/)
{
  return StringUtils::jsonStringToPropTree(LANGS_STR);
}

}
