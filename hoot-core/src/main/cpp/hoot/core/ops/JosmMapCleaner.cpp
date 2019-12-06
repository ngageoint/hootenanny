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
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, JosmMapCleaner)

JosmMapCleaner::JosmMapCleaner() :
JosmMapValidatorAbstract(),
_addDebugTags(false),
_numGroupsOfElementsCleaned(0)
{
  _josmInterfaceName = "hoot/services/josm/JosmMapCleaner";
}

void JosmMapCleaner::setConfiguration(const Settings& conf)
{
  JosmMapValidatorAbstract::setConfiguration(conf);

  ConfigOptions opts(conf);
  _addDebugTags = opts.getMapCleanerJosmAddDebugTags();
}

void JosmMapCleaner::apply(std::shared_ptr<OsmMap>& map)
{
  _numGroupsOfElementsCleaned = 0;
  _deletedElementIds.clear();

  JosmMapValidatorAbstract::apply(map);
}

OsmMapPtr JosmMapCleaner::_getUpdatedMap(OsmMapPtr& inputMap)
{
  LOG_DEBUG("Retrieving cleaned map...");

  // pass in the validators to use, the features to be examined, and whether to just validate or to
  // validate and fix them;

  // validators delimited by ';'
  jstring validatorsStr = _javaEnv->NewStringUTF(_josmValidators.join(";").toStdString().c_str());
  // convert input map to xml string
  jstring mapXml =
    _javaEnv->NewStringUTF(OsmXmlWriter::toString(inputMap, false).toStdString().c_str());
  jobject cleanResult =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // JNI sig format: (input params...)return type
      // Java sig: String clean(String validatorsStr, String elementsXml, boolean addDebugTags)
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "clean", "(Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;"),
      validatorsStr,
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
  OsmMapPtr cleanedMap = OsmXmlReader::fromXml(QString(xml).trimmed(), true, true, false, true);
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

void JosmMapCleaner::_getStats()
{
  LOG_DEBUG("Retrieving stats...");

  // call back into Java validator to get the validation stats
  JosmMapValidatorAbstract::_getStats();

  _numGroupsOfElementsCleaned =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumGroupsOfElementsCleaned", "()I"));

  jstring deletedElementIdsResult =
    (jstring)_javaEnv->CallObjectMethod(
      _josmInterface,
      _javaEnv->GetMethodID(_josmInterfaceClass, "getDeletedElementIds", "()Ljava/lang/String;"));
  const char* deletedElementIdsStr =
    _javaEnv->GetStringUTFChars((jstring)deletedElementIdsResult, NULL);
  const QString deletedElementIdsQStr(deletedElementIdsStr);
   // TODO: env->ReleaseStringUTFChars
  _deletedElementIds = _elementIdsStrToElementIds(deletedElementIdsQStr);
  // TODO: not sure what to do with this info yet...
  LOG_INFO("Deleted " << _deletedElementIds.size() << " elements from map.");

  jstring validationErrorCountsByTypeResult =
    (jstring)_javaEnv->CallObjectMethod(
      _josmInterface,
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getValidationErrorCountsByType", "()Ljava/lang/String;"));
  const char* validationErrorCountsByTypeTempStr =
    _javaEnv->GetStringUTFChars((jstring)validationErrorCountsByTypeResult, NULL);
  const QString validationErrorCountsByType(validationErrorCountsByTypeTempStr);
  // TODO: env->ReleaseStringUTFChars

  jstring validationErrorFixCountsByTypeResult =
    (jstring)_javaEnv->CallObjectMethod(
      _josmInterface,
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getValidationErrorFixCountsByType", "()Ljava/lang/String;"));
  const char* validationErrorFixCountsByTypeTempStr =
    _javaEnv->GetStringUTFChars((jstring)validationErrorFixCountsByTypeResult, NULL);
  const QString validationErrorFixCountsByType(validationErrorFixCountsByTypeTempStr);
  // TODO: env->ReleaseStringUTFChars
  LOG_VART(validationErrorFixCountsByType);

  _errorSummary = "Total validation errors: " + QString::number(_numValidationErrors) + "\n";
  _errorSummary +=
    "Total groups of elements cleaned: " + QString::number(_numGroupsOfElementsCleaned) + "\n";
  _errorSummary += "Total elements deleted: " + QString::number(_deletedElementIds.size()) + "\n";
  if (!validationErrorCountsByType.trimmed().isEmpty())
  {
    _errorSummary +=
       _errorCountsByTypeStrToSummaryStr(
         validationErrorCountsByType, validationErrorFixCountsByType);
  }
  _errorSummary = _errorSummary.trimmed();
  LOG_VART(_errorSummary);
}

QString JosmMapCleaner::_errorCountsByTypeStrToSummaryStr(
  const QString& errorCountsByTypeStr, const QString& errorFixCountsByTypeStr) const
{
  QString summary = "";

  QMap<QString, int> errorCountsByType;
  const QStringList errorsByTypeParts = errorCountsByTypeStr.split(";");
  for (int i = 0; i < errorsByTypeParts.size(); i++)
  {
    const QStringList errorByTypeParts = errorsByTypeParts.at(i).split(":");
    errorCountsByType[errorByTypeParts.at(0)] = errorByTypeParts.at(1).toInt();
  }
  LOG_VART(errorCountsByType.size());

  QMap<QString, int> errorFixCountsByType;
  const QStringList fixesByTypeParts = errorFixCountsByTypeStr.split(";");
  for (int i = 0; i < fixesByTypeParts.size(); i++)
  {
    const QStringList fixByTypeParts = fixesByTypeParts.at(i).split(":");
    errorFixCountsByType[fixByTypeParts.at(0)] = fixByTypeParts.at(1).toInt();
  }
  LOG_VART(errorFixCountsByType.size());

  assert(errorCountsByType.size() == errorFixCountsByType.size());

  for (QMap<QString, int>::const_iterator errorItr = errorCountsByType.begin();
       errorItr != errorCountsByType.end(); ++errorItr)
  {
    assert(errorFixCountsByType.contains(errorItr.key()));
    summary +=
      errorItr.key() + " errors: " + QString::number(errorItr.value()) +
      ", groups cleaned: " + QString::number(errorFixCountsByType[errorItr.key()]) + "\n";
  }

  return summary;
}

}
