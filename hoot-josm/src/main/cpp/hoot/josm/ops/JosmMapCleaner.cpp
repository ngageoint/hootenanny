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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "JosmMapCleaner.h"

// hoot
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/josm/jni/JniConversion.h>
#include <hoot/josm/jni/JniUtils.h>

// Qt
#include <QTemporaryFile>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, JosmMapCleaner)

JosmMapCleaner::JosmMapCleaner()
  : JosmMapValidatorAbstract(),
    _addDetailTags(false),
    _numElementsCleaned(0),
    _numFailedCleaningOperations(0)
{
}

void JosmMapCleaner::setConfiguration(const Settings& conf)
{
  JosmMapValidatorAbstract::setConfiguration(conf);

  ConfigOptions opts(conf);
  _addDetailTags = opts.getJosmMapCleanerAddDetailTags();
}

void JosmMapCleaner::apply(std::shared_ptr<OsmMap>& map)
{
  _numElementsCleaned = 0;
  _numFailedCleaningOperations = 0;
  _deletedElementIds.clear();
  LOG_VARD(_addDetailTags);

  JosmMapValidatorAbstract::apply(map);
}

OsmMapPtr JosmMapCleaner::_getUpdatedMap(OsmMapPtr& inputMap)
{
  LOG_DEBUG("Retrieving cleaned map...");
  LOG_VARD(inputMap->size());

  // JNI sig format: (input params...)return type

  // If the map is large enough, we want to avoid string serialization.
  if (static_cast<int>(inputMap->size()) > _maxElementsForMapString)
  {
    // pass map as temp file and get it back as a temp file

    std::shared_ptr<QTemporaryFile> tempInputFile =
      std::make_shared<QTemporaryFile>(
        ConfigOptions().getApidbBulkInserterTempFileDir() + "/JosmMapCleaner-in.osm");
    tempInputFile->setAutoRemove(true);
    if (!tempInputFile->open())
      throw HootException("Unable to open temp input file for cleaning: " + tempInputFile->fileName() + ".");

    LOG_DEBUG("Writing temp map to " << tempInputFile->fileName() << "...");
    OsmXmlWriter().write(inputMap, tempInputFile->fileName());

    const QString tempOutputPath = tempInputFile->fileName().replace("in", "out");

    _clean(_josmValidators, tempInputFile->fileName(), tempOutputPath, _addDetailTags);

    LOG_DEBUG("Reading cleaned map from " << tempOutputPath << "...");
    OsmMapPtr cleanedMap = std::make_shared<OsmMap>();
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    reader.setUseFileStatus(true);
    reader.open(tempOutputPath);
    reader.read(tempOutputPath, cleanedMap);
    return cleanedMap;
  }
  else
  {
    // pass map as string and get it back as a string
    return
      OsmXmlReader::fromXml(
        _clean(_josmValidators, OsmXmlWriter::toString(inputMap, false), _addDetailTags).trimmed(),
        true, true, false, true);
  }
}

QString JosmMapCleaner::_clean(const QStringList& validators, const QString& map, const bool addDetailTags)
{
  // JNI sig format: (input params...)return type
  jstring cleanedMapResultStr =
    (jstring)_javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig:
      //  String validate(
      //    List<String> validators, String elementsXml, boolean cleanValidated, boolean addTags)
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "validate",
        "(Ljava/util/List;Ljava/lang/String;ZZ)Ljava/lang/String;"),
      JniConversion::toJavaStringList(_javaEnv, validators),
      JniConversion::toJavaString(_javaEnv, map),
      true,
      addDetailTags);
  JniUtils::checkForErrors(_javaEnv, "cleanFromMapString");
  return JniConversion::fromJavaString(_javaEnv, cleanedMapResultStr);
}

void JosmMapCleaner::_clean(const QStringList& validators, const QString& inputMapPath, const QString& outputMapPath,
                            const bool addDetailTags)
{
  // JNI sig format: (input params...)return type
  _javaEnv->CallVoidMethod(
    _josmInterface,
    // Java sig:
    //  void validate(
    //    List<String> validators, String elementsFileInputPath, String elementsFileOutputPath,
    //    boolean cleanValidated, boolean addTags)
    _javaEnv->GetMethodID(
      _josmInterfaceClass, "validate", "(Ljava/util/List;Ljava/lang/String;Ljava/lang/String;ZZ)V"),
    JniConversion::toJavaStringList(_javaEnv, validators),
    JniConversion::toJavaString(_javaEnv, inputMapPath),
    JniConversion::toJavaString(_javaEnv, outputMapPath),
    true,
    addDetailTags);
  JniUtils::checkForErrors(_javaEnv, "cleanFromMapFile");
}

void JosmMapCleaner::_getStats()
{
  LOG_DEBUG("Retrieving stats...");

  // Call back into the hoot-josm validator to get the stats after validation.

  JosmMapValidatorAbstract::_getStats();
  _numElementsCleaned = _getNumElementsCleaned();
  _deletedElementIds = _getDeletedElementIds();
  const QMap<QString, QString> failingValidatorInfo = _getFailingValidatorInfo();
  _numFailingValidators = failingValidatorInfo.size();
  const QMap<QString, QString> failingCleanerInfo = _getFailingCleanerInfo();

  _numFailedCleaningOperations = failingCleanerInfo.size();

  // create a readable error summary

  _errorSummary =
    "Found " + StringUtils::formatLargeNumber(_numValidationErrors) +
    " validation errors in " + StringUtils::formatLargeNumber(_numAffected) +
    " features with JOSM.\n";
  _errorSummary += "Total elements cleaned: " + StringUtils::formatLargeNumber(_numElementsCleaned) + "\n";
  _errorSummary += "Total elements deleted: " + StringUtils::formatLargeNumber(_deletedElementIds.size()) + "\n";
  _errorSummary += "Total failing JOSM validators: " + QString::number(_numFailingValidators) + "\n";
  _errorSummary += "Total failing JOSM cleaning operations: " + QString::number(_numFailedCleaningOperations) + "\n";
  _errorSummary += _errorCountsByTypeToSummaryStr(_getValidationErrorCountsByType(), _getValidationErrorFixCountsByType()) + "\n";

  for (const auto& validatorName : failingValidatorInfo.keys())
    _errorSummary += "Validator: " + validatorName + " failed with error: " + failingValidatorInfo[validatorName] + ".\n";

  for (const auto& cleanerName : failingCleanerInfo.keys())
    _errorSummary += "Cleaner: " + cleanerName + " failed with error: " + failingCleanerInfo[cleanerName] + ".\n";

  _errorSummary = _errorSummary.trimmed();
  LOG_VART(_errorSummary);
}

// JNI sig format: (input params...)return type

int JosmMapCleaner::_getNumElementsCleaned()
{
  const int numCleaned =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      // Java sig: int getNumElementsCleaned()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumElementsCleaned", "()I"));
  JniUtils::checkForErrors(_javaEnv, "getNumElementsCleaned");
  return numCleaned;
}

QSet<ElementId> JosmMapCleaner::_getDeletedElementIds()
{
  jobject deletedElementIdsJavaSet =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig: Set<String> getDeletedElementIds()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getDeletedElementIds", "()Ljava/util/Set;"));
  JniUtils::checkForErrors(_javaEnv, "getDeletedElementIds");
  return
    _elementIdStringsToElementIds(
      JniConversion::fromJavaStringSet(_javaEnv, deletedElementIdsJavaSet));
}

QMap<QString, int> JosmMapCleaner::_getValidationErrorFixCountsByType()
{
  jobject validationErrorFixCountsByTypeJavaMap =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig: Map<String, Integer> getValidationErrorFixCountsByType()
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getValidationErrorFixCountsByType", "()Ljava/util/Map;"));
  JniUtils::checkForErrors(_javaEnv, "getValidationErrorFixCountsByType");
  return JniConversion::fromJavaStringIntMap(_javaEnv, validationErrorFixCountsByTypeJavaMap);
}

QMap<QString, QString> JosmMapCleaner::_getFailingCleanerInfo()
{
  jobject failingCleanerInfo =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig: Map<String, String> getFailingCleanerInfo()
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getFailingCleanerInfo", "()Ljava/util/Map;"));
  JniUtils::checkForErrors(_javaEnv, "getFailingCleanerInfo");
  return JniConversion::fromJavaStringMap(_javaEnv, failingCleanerInfo);
}

QSet<ElementId> JosmMapCleaner::_elementIdStringsToElementIds(const QSet<QString>& elementIdStrs) const
{
  QSet<ElementId> result;
  for (const auto& elementIdStr : elementIdStrs)
  {
    const QStringList elementIdParts = elementIdStr.split(":");
    if (elementIdParts.size() == 2)
    {
      bool ok = false;
      const long id = elementIdParts[1].toLong(&ok);
      if (ok)
        result.insert(ElementId(ElementType::fromString(elementIdParts[0]), id));
    }
  }
  return result;
}

QString JosmMapCleaner::_errorCountsByTypeToSummaryStr(const QMap<QString, int>& errorCountsByType,
                                                       const QMap<QString, int>& errorFixCountsByType) const
{
  const int longestErrorNameSize = 33;
  const int longestCountSize = 11;
  QString summary = "";
  for (auto errorItr = errorCountsByType.begin(); errorItr != errorCountsByType.end(); ++errorItr)
  {
    const int indentAfterName = longestErrorNameSize - errorItr.key().size() + 1;
    const QString numErrorsForTypeStr = StringUtils::formatLargeNumber(errorItr.value());
    const int indentAfterCount = longestCountSize - numErrorsForTypeStr.size();
    summary += errorItr.key() + " errors: " + QString(indentAfterName, ' ') + numErrorsForTypeStr + " " +
               QString(indentAfterCount, ' ') + " elements cleaned: " +
               StringUtils::formatLargeNumber(errorFixCountsByType[errorItr.key()]) + "\n";
  }
  return summary;
}

}
