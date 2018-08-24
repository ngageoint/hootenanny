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

#include "HootServicesTranslationInfoMockClient.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

HootServicesTranslationInfoMockClient::HootServicesTranslationInfoMockClient() :
HootServicesTranslationInfoClient()
{
}

/**
 * Retrieves available language apps
 *
 * @param type type of app to retrieve; "translator" or "detector"
 * @return a property tree containing the language app information
 */
boost::shared_ptr<boost::property_tree::ptree>
  HootServicesTranslationInfoMockClient::getAvailableApps(const QString type)
{
  LOG_DEBUG("Checking apps available for: " << type << "...");
  const QString jsonStr =
    "{\"apps\":[{\"name\":\"TikaLanguageDetector\",\"description\":\"The language detection portion of a library which detects and extracts metadata and text from many different file types\",\"url\":\"https://tika.apache.org\"},{\"name\":\"OpenNlpLanguageDetector\",\"description\":\"The language detector portion of a machine learning based toolkit for the processing of natural language text\",\"url\":\"https://opennlp.apache.org\"}]}";
  return StringUtils::jsonStringToPropTree(jsonStr);
}

/**
 * Retrieves translation available languages info
 *
 * @param type type of language information to retrieve; "translatable" or "detectable"
 * @return a property tree containing the language information
 */
boost::shared_ptr<boost::property_tree::ptree>
  HootServicesTranslationInfoMockClient::getAvailableLanguages(const QString type)
{
  LOG_DEBUG("Checking languages available for: " << type << "...");
  const QString jsonStr = "{\"languages\":[{\"name\":\"Arabic\",\"available\":false,\"iso6391code\":\"ar\",\"iso6392code\":\"ara\"},{\"name\":\"Amharic\",\"available\":false,\"iso6391code\":\"am\",\"iso6392code\":\"afr\"}]}";
  return StringUtils::jsonStringToPropTree(jsonStr);
}

}
