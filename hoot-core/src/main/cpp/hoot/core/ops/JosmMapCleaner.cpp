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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "JosmMapCleaner.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/JavaEnvironment.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

JosmMapCleaner::JosmMapCleaner() :
_cleanersJosmNamespace("org.openstreetmap.josm.data.validation.tests"),
_cleanerInterfaceName("hoot/services/josm/JosmMapCleaner"),
_addValidationTags(true),
_addDebugTags(false),
_cleanerInterfaceInitialized(false),
_numValidationErrors(0),
_numGroupsOfElementsCleaned(0)
{
  _javaEnv = JavaEnvironment::getEnvironment();
}

void JosmMapCleaner::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _addDebugTags = opts.getMapCleanerJosmAddDebugTags();
  _addValidationTags = opts.getMapCleanerJosmAddValidationTags();
  _josmCleanersExclude = opts.getMapCleanerJosmValidatorsExclude();
  _josmCleanersInclude = opts.getMapCleanerJosmValidatorsInclude();
  _cleanersJosmNamespace = opts.getMapCleanerJosmJavaNamespace();
  _cleanerInterfaceName = opts.getMapCleanerJosmJavaImplementation();
}

void JosmMapCleaner::_initCleanerImplementation()
{
  LOG_DEBUG("Initializing cleaner implementation...");

  // TODO: change back
  _cleanerInterfaceClass =
    _javaEnv->FindClass(/*_cleanerInterfaceName.toStdString().c_str()*/
                        "hoot/services/josm/JosmMapCleaner");
  LOG_VARD(_cleanerInterfaceClass == 0);
  jmethodID constructorMethodId =
    _javaEnv->GetMethodID(_cleanerInterfaceClass, "<init>", "(Ljava/lang/String;)V");
  LOG_VARD(constructorMethodId == 0);
  // TODO: change back
  jstring logLevelStr =
    _javaEnv->NewStringUTF(Log::getInstance().getLevelAsString().toStdString().c_str());
  LOG_VARD(logLevelStr == 0);
  //jstring logLevelStr = _javaEnv->NewStringUTF(QString("TRACE").toStdString().c_str());
  _cleanerInterface = _javaEnv->NewObject(_cleanerInterfaceClass, constructorMethodId, logLevelStr);
  LOG_VARD(_cleanerInterface == 0);
  // TODO: _javaEnv->ReleaseStringUTFChars
  _cleanerInterfaceInitialized = true;
}

void JosmMapCleaner::_initJosmCleanersList()
{
  _josmCleanersInclude.sort();
  _josmCleanersExclude.sort();

  // assuming class names only for validators passed in
  _updateJosmCleanersWithNamepace(_josmCleanersInclude);
  _updateJosmCleanersWithNamepace(_josmCleanersExclude);

  // If an include list was specified, let's start with that.
  _josmCleanersToUse = _josmCleanersInclude;

  // The exclude list overrides the include list, so check it too.
  StringUtils::removeAll(_josmCleanersToUse, _josmCleanersExclude);

  if (!_josmCleanersInclude.isEmpty() && _josmCleanersToUse.isEmpty())
  {
    throw IllegalArgumentException(
      "Cleaner include/exclude lists resulted in no JOSM cleaners specified.");
  }

  // If validators are empty at this point, then use them all.
  if (_josmCleanersToUse.isEmpty())
  {
    _josmCleanersToUse = getAvailableCleaners().keys();
    _updateJosmCleanersWithNamepace(_josmCleanersToUse);
  }
}

void JosmMapCleaner::_updateJosmCleanersWithNamepace(QStringList& cleaners)
{
  QStringList cleanersTemp;
  for (int i = 0; i < cleaners.size(); i++)
  {
    cleanersTemp.append(_cleanersJosmNamespace + "." + cleaners.at(i));
  }
  cleaners = cleanersTemp;
}

QMap<QString, QString> JosmMapCleaner::getAvailableCleaners()
{
  LOG_DEBUG("Getting available cleaners...");

  QMap<QString, QString> cleaners;
  if (!_cleanerInterfaceInitialized)
  {
    _initCleanerImplementation();
  }

  jobject availableCleanersResult =
    _javaEnv->CallObjectMethod(
      _cleanerInterface,
      _javaEnv->GetMethodID(_cleanerInterfaceClass, "getAvailableCleaners", "()Ljava/lang/String;"));
  if (_javaEnv->ExceptionCheck())
  {
    _javaEnv->ExceptionDescribe();
    _javaEnv->ExceptionClear();
    // TODO: get exception message from object and add here
    throw HootException("Error calling getAvailableValidators.");
  }

  const char* str = _javaEnv->GetStringUTFChars((jstring)availableCleanersResult, NULL);
  QString cleanersStr(str);
  assert(cleanersStr.contains(";"));
  QStringList cleanerStrings = cleanersStr.split(";");
  for (int i = 0; i < cleanerStrings.size(); i++)
  {
    const QString cleanerStr = cleanerStrings.at(i);
    if (!cleanerStr.isEmpty())
    {
      assert(cleanerStr.contains(","));
      const QStringList cleanerStrParts = cleanerStr.split(",");
      assert(cleanerStrParts.size() == 2);

      const QString cleanerName = cleanerStrParts[0];
      const QString cleanerDescription = cleanerStrParts[1];
      cleaners[cleanerName] = cleanerDescription;
    }
  }
  //TODO: env->ReleaseStringUTFChars //??

  return cleaners;
}

void JosmMapCleaner::apply(std::shared_ptr<OsmMap>& map)
{
  LOG_VARD(map->size());
  //LOG_TRACE("Input map: " << OsmXmlWriter::toString(map, true));

  _numAffected = 0;
  _numValidationErrors = 0;
  _numGroupsOfElementsCleaned = 0;
  _numAffected = map->size();
  _deletedElementIds.clear();

  if (!_cleanerInterfaceInitialized)
  {
    _initCleanerImplementation();
  }
  if (_josmCleanersToUse.isEmpty())
  {
    _initJosmCleanersList();
  }

  // update map with fixed features and add validation info to validation results collection
  OsmMapPtr cleanedMap = _getCleanedMap(map);
  LOG_VARD(cleanedMap->size());
  map = cleanedMap;

  _getCleanStats();
}

OsmMapPtr JosmMapCleaner::_getCleanedMap(OsmMapPtr& inputMap)
{
  // pass in the validators to use, the features to be examined, and whether to just validate or to
  // validate and fix them

  OsmMapPtr cleanedMap;

  // validators delimited by ';'
  jstring cleanersStr = _javaEnv->NewStringUTF(_josmCleanersToUse.join(";").toStdString().c_str());
  // convert input map to xml string
  jstring mapXml =
    _javaEnv->NewStringUTF(OsmXmlWriter::toString(inputMap, false).toStdString().c_str());
  jobject cleanResult =
    _javaEnv->CallObjectMethod(
      _cleanerInterface,
      // JNI sig format: (input params...)return type
      // Java sig: String clean(String cleanersStr, String featuresXml, boolean addDebugTags)
      _javaEnv->GetMethodID(
        _cleanerInterfaceClass, "clean", "(Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;"),
      cleanersStr,
      mapXml,
      _addDebugTags);
  // TODO: env->ReleaseStringUTFChars
  if (_javaEnv->ExceptionCheck())
  {
    _javaEnv->ExceptionDescribe();
    _javaEnv->ExceptionClear();
    // TODO: get exception message from object and add here
    throw HootException("Error calling JosmValidator::apply.");
  }

  const char* xml = _javaEnv->GetStringUTFChars((jstring)cleanResult, NULL);
  cleanedMap = OsmXmlReader::fromXml(QString(xml).trimmed(), true, true, false, true);
  // TODO: env->ReleaseStringUTFChars

  // return a null map if no features had validation issues
  //LOG_VART(OsmXmlWriter::toString(cleanedMap, true));
  return cleanedMap;
}

QSet<ElementId> JosmMapCleaner::_elementIdsStrToElementIds(const QString elementIdsStr) const
{
  QSet<ElementId> elementIds;
  if (!elementIdsStr.trimmed().isEmpty())
  {
    QStringList elementIdsParts = elementIdsStr.split(";");
    for (int i = 0; i < elementIdsParts.size(); i++)
    {
      const QString elementIdStr = elementIdsParts.at(i);
      const QStringList elementIdParts = elementIdStr.split(":");
      if (elementIdParts.size() == 2)
      {
        bool ok = false;
        const long id = elementIdParts[1].toLong(&ok);
        if (ok)
        {
          elementIds.insert(ElementId(ElementType::fromString(elementIdParts[0]), id));
        }
      }
    }
  }
  return elementIds;
}

void JosmMapCleaner::_getCleanStats()
{
  // call back into Java validator to get the validation stats
  _numValidationErrors =
    (int)_javaEnv->CallIntMethod(
      _cleanerInterface,
      _javaEnv->GetMethodID(_cleanerInterfaceClass, "getNumValidationErrors", "()I"));
  _numGroupsOfElementsCleaned =
    (int)_javaEnv->CallIntMethod(
      _cleanerInterface,
      _javaEnv->GetMethodID(_cleanerInterfaceClass, "getNumGroupsOfElementsCleaned", "()I"));
  jstring deletedElementIdsResult =
    (jstring)_javaEnv->CallObjectMethod(
      _cleanerInterface,
      _javaEnv->GetMethodID(_cleanerInterfaceClass, "getDeletedElementIds", "()Ljava/lang/String;"));
  const char* deletedElementIdsStr =
    _javaEnv->GetStringUTFChars((jstring)deletedElementIdsResult, NULL);
  const QString deletedElementIdsQStr(deletedElementIdsStr);
   // TODO: env->ReleaseStringUTFChars
  _deletedElementIds = _elementIdsStrToElementIds(deletedElementIdsQStr);
  // TODO: not sure what to do with this info yet...
  LOG_INFO("Deleted " << _deletedElementIds.size() << " elements from map.");
}

}
