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

#include "HootServicesLanguageInfoMockClient.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(LanguageInfoProvider, HootServicesLanguageInfoMockClient)

HootServicesLanguageInfoMockClient::HootServicesLanguageInfoMockClient() :
HootServicesLanguageInfoClient()
{
  _useCookies = false;
}

boost::shared_ptr<boost::property_tree::ptree>
  HootServicesLanguageInfoMockClient::getAvailableApps(const QString /*type*/)
{
  const QString jsonStr =
    "{\"apps\":[{\"name\":\"TikaLanguageDetector\",\"description\":\"The language detection portion of a library which detects and extracts metadata and text from many different file types\",\"url\":\"https://tika.apache.org\"},{\"name\":\"OpenNlpLanguageDetector\",\"description\":\"The language detector portion of a machine learning based toolkit for the processing of natural language text\",\"url\":\"https://opennlp.apache.org\"}]}";
  return StringUtils::jsonStringToPropTree(jsonStr);
}

boost::shared_ptr<boost::property_tree::ptree>
  HootServicesLanguageInfoMockClient::getAvailableLanguages(const QString /*type*/)
{
  const QString jsonStr = "{\"languages\":[{\"name\":\"German\",\"available\":true,\"iso6391Code\":\"de\",\"iso6392Code\":\"deu\"},{\"name\":\"Spanish\",\"available\":true,\"iso6391Code\":\"es\",\"iso6392Code\":\"spa\"}]}";
  return StringUtils::jsonStringToPropTree(jsonStr);
}

}
