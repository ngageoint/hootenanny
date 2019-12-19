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
#include <hoot/core/jni/JniConversion.h>

// Qt
#include <QTemporaryFile>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, JosmMapCleaner)

JosmMapCleaner::JosmMapCleaner() :
JosmMapValidatorAbstract(),
_addDetailTags(false),
_numElementsCleaned(0),
_numFailedCleaningOperations(0)
{
  // Don't see this changing any time soon, so not making it configurable until necessary
  _josmInterfaceName = "hoot/services/josm/JosmMapCleaner";
}

void JosmMapCleaner::setConfiguration(const Settings& conf)
{
  JosmMapValidatorAbstract::setConfiguration(conf);

  ConfigOptions opts(conf);
  _addDetailTags = opts.getMapCleanerJosmAddDetailTags();
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
  if ((int)inputMap->size() > _maxElementsForMapString)
  {
    // pass map as temp file and get it back as a temp file

    std::shared_ptr<QTemporaryFile> tempInputFile(
      new QTemporaryFile(
        ConfigOptions().getApidbBulkInserterTempFileDir() + "/JosmMapCleaner-in.osm"));
    tempInputFile->setAutoRemove(true);
    if (!tempInputFile->open())
    {
      throw HootException(
        "Unable to open temp input file for cleaning: " + tempInputFile->fileName() + ".");
    }
    LOG_DEBUG("Writing temp map to " << tempInputFile->fileName() << "...");
    OsmXmlWriter().write(inputMap, tempInputFile->fileName());

    const QString tempOutputPath = tempInputFile->fileName().replace("in", "out");

    _clean(_josmValidators, tempInputFile->fileName(), tempOutputPath, _addDetailTags);

    LOG_DEBUG("Reading cleaned map from " << tempOutputPath << "...");
    OsmMapPtr cleanedMap(new OsmMap());
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

QString JosmMapCleaner::_clean(
  const QStringList& validators, const QString& map, const bool addDetailTags)
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
  JniConversion::checkForErrors(_javaEnv, "cleanFromMapString");
  return JniConversion::fromJavaString(_javaEnv, cleanedMapResultStr);
}

void JosmMapCleaner::_clean(
  const QStringList& validators, const QString& inputMapPath, const QString& outputMapPath,
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
  JniConversion::checkForErrors(_javaEnv, "cleanFromMapFile");
}

void JosmMapCleaner::_getStats()
{
  LOG_DEBUG("Retrieving stats...");

  // call back into the hoot-josm validator to get the stats after validation

  JosmMapValidatorAbstract::_getStats();
  _numElementsCleaned = _getNumElementsCleaned();
  _deletedElementIds = _getDeletedElementIds();
  _numFailingValidators = _getNumFailingValidators();
  _numFailedCleaningOperations = _getNumFailedCleaningOperations();

  // create a readable error summary

  _errorSummary =
    "Total JOSM validation errors: " + StringUtils::formatLargeNumber(_numValidationErrors) +
    " found in " + StringUtils::formatLargeNumber(_numAffected) + " total features.\n";
  _errorSummary +=
    "Total elements cleaned: " + StringUtils::formatLargeNumber(_numElementsCleaned) + "\n";
  _errorSummary +=
    "Total elements deleted: " + StringUtils::formatLargeNumber(_deletedElementIds.size()) + "\n";
  _errorSummary +=
    "Total failing JOSM validators: " + QString::number(_numFailingValidators) + "\n";
  _errorSummary +=
    "Total failing JOSM cleaning operations: " + QString::number(_numFailedCleaningOperations) +
    "\n";
  _errorSummary +=
     _errorCountsByTypeToSummaryStr(
       _getValidationErrorCountsByType(), _getValidationErrorFixCountsByType());
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
  JniConversion::checkForErrors(_javaEnv, "getNumElementsCleaned");
  return numCleaned;
}

QSet<ElementId> JosmMapCleaner::_getDeletedElementIds()
{
  jobject deletedElementIdsJavaSet =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig: Set<String> getDeletedElementIds()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getDeletedElementIds", "()Ljava/util/Set;"));
  JniConversion::checkForErrors(_javaEnv, "getDeletedElementIds");
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
  JniConversion::checkForErrors(_javaEnv, "getValidationErrorFixCountsByType");
  JniConversion::checkForErrors(_javaEnv, "getValidationErrorFixCountsByType");
  return JniConversion::fromJavaStringIntMap(_javaEnv, validationErrorFixCountsByTypeJavaMap);
}

int JosmMapCleaner::_getNumFailedCleaningOperations()
{
  const int numFailedCleaningOperations =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      // Java sig: int getNumFailedCleaningOperations()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumFailedCleaningOperations", "()I"));
  JniConversion::checkForErrors(_javaEnv, "getNumFailedCleaningOperations");
  return numFailedCleaningOperations;
}

QSet<ElementId> JosmMapCleaner::_elementIdStringsToElementIds(
  const QSet<QString>& elementIdStrs) const
{
  QSet<ElementId> result;
  for (QString elementIdStr : elementIdStrs)
  {
    const QStringList elementIdParts = elementIdStr.split(":");
    if (elementIdParts.size() == 2)
    {
      bool ok = false;
      const long id = elementIdParts[1].toLong(&ok);
      if (ok)
      {
        result.insert(ElementId(ElementType::fromString(elementIdParts[0]), id));
      }
    }
  }
  return result;
}

QString JosmMapCleaner::_errorCountsByTypeToSummaryStr(
  const QMap<QString, int>& errorCountsByType, const QMap<QString, int>& errorFixCountsByType) const
{
  assert(errorCountsByType.size() == errorFixCountsByType.size());

  const int longestErrorNameSize = 33;
  const int longestCountSize = 11;
  QString summary = "";
  for (QMap<QString, int>::const_iterator errorItr = errorCountsByType.begin();
       errorItr != errorCountsByType.end(); ++errorItr)
  {
    assert(errorFixCountsByType.contains(errorItr.key()));
    const int indentAfterName = longestErrorNameSize - errorItr.key().size() + 1;
    const QString numErrorsForTypeStr = StringUtils::formatLargeNumber(errorItr.value());
    const int indentAfterCount = longestCountSize - numErrorsForTypeStr.size();
    summary +=
      errorItr.key() + " errors: " + QString(indentAfterName, ' ') + numErrorsForTypeStr + " " +
      QString(indentAfterCount, ' ') + " elements cleaned: " +
      StringUtils::formatLargeNumber(errorFixCountsByType[errorItr.key()]) + "\n";
  }
  return summary;
}

}
