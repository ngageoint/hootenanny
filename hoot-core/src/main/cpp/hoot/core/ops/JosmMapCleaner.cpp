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
_numGroupsOfElementsCleaned(0),
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
  _numGroupsOfElementsCleaned = 0;
  _numFailedCleaningOperations = 0;
  _deletedElementIds.clear();
  LOG_VARD(_addDetailTags);

  JosmMapValidatorAbstract::apply(map);
}

OsmMapPtr JosmMapCleaner::_getUpdatedMap(OsmMapPtr& inputMap)
{
  LOG_DEBUG("Retrieving cleaned map...");
  LOG_VARD(inputMap->size());

  // call into hoot-josm to clean features in the map and return the cleaned map

  // JNI sig format: (input params...)return type

  if ((int)inputMap->size() > _inMemoryMapSizeMax)
  {
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

    const QString tempOutput = tempInputFile->fileName().replace("in", "out");

    _javaEnv->CallVoidMethod(
      _josmInterface,
      // Java sig:
      //  void clean(
      //    List<String> validators, String elementsFileInputPath, String elementsFileOutputPath,
      //    boolean addDetailTags)
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "clean",
        "(Ljava/util/List;Ljava/lang/String;Ljava/lang/String;Z)V"),
      // validators to use
      JniConversion::toJavaStringList(_javaEnv, _josmValidators),
      // convert input map to xml string to pass in
      JniConversion::toJavaString(_javaEnv, tempInputFile->fileName()),
      JniConversion::toJavaString(_javaEnv, tempOutput),
      // add explanation tags for cleaning
      _addDetailTags);
    JniConversion::checkForErrors(_javaEnv, "cleanFromMapFile");

    LOG_DEBUG("Reading cleaned map from " << tempOutput << "...");
    OsmMapPtr cleanedMap(new OsmMap());
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    reader.setUseFileStatus(true);
    reader.open(tempOutput);
    reader.read(tempOutput, cleanedMap);
    return cleanedMap;
  }
  else
  {
    jstring cleanedMapResultStr =
      (jstring)_javaEnv->CallObjectMethod(
        _josmInterface,
        // Java sig:
        //  String clean(List<String> validators, String elementsXml, boolean addDetailTags)
        _javaEnv->GetMethodID(
          _josmInterfaceClass, "clean",
          "(Ljava/util/List;Ljava/lang/String;Z)Ljava/lang/String;"),
        // validators to use
        JniConversion::toJavaStringList(_javaEnv, _josmValidators),
        // convert input map to xml string to pass in
        JniConversion::toJavaString(_javaEnv, OsmXmlWriter::toString(inputMap, false)),
        // add explanation tags for cleaning
        _addDetailTags);
    JniConversion::checkForErrors(_javaEnv, "cleanFromMapString");

    return
      OsmXmlReader::fromXml(
        JniConversion::fromJavaString(_javaEnv, cleanedMapResultStr).trimmed(), true, true, false,
        true);
  }
}

void JosmMapCleaner::_getStats()
{
  LOG_DEBUG("Retrieving stats...");

  // call back into the hoot-josm validator to get the stats after validation

  JosmMapValidatorAbstract::_getStats();

  // JNI sig format: (input params...)return type

  _numGroupsOfElementsCleaned =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      // Java sig: int getNumGroupsOfElementsCleaned()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumGroupsOfElementsCleaned", "()I"));
  JniConversion::checkForErrors(_javaEnv, "getNumGroupsOfElementsCleaned");

  jobject deletedElementIdsJavaSet =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig: Set<String> getDeletedElementIds()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getDeletedElementIds", "()Ljava/util/Set;"));
  JniConversion::checkForErrors(_javaEnv, "getDeletedElementIds");
  _deletedElementIds =
    _elementIdStringsToElementIds(
      JniConversion::fromJavaStringSet(_javaEnv, deletedElementIdsJavaSet));
  // TODO: need to add this to the map tags??
  LOG_INFO("Deleted " << _deletedElementIds.size() << " elements from map.");

  jobject validationErrorCountsByTypeJavaMap =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig: Map<String, Integer> getValidationErrorCountsByType()
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getValidationErrorCountsByType", "()Ljava/util/Map;"));
  JniConversion::checkForErrors(_javaEnv, "getValidationErrorCountsByType");

  jobject validationErrorFixCountsByTypeJavaMap =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig: Map<String, Integer> getValidationErrorFixCountsByType()
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getValidationErrorFixCountsByType", "()Ljava/util/Map;"));
  JniConversion::checkForErrors(_javaEnv, "getValidationErrorFixCountsByType");

  _numFailingValidators =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      // Java sig: int getNumFailingValidators()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumFailingValidators", "()I"));
  JniConversion::checkForErrors(_javaEnv, "getNumFailingValidators");

  _numFailedCleaningOperations =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      // Java sig: int getNumFailedCleaningOperations()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumFailedCleaningOperations", "()I"));
  JniConversion::checkForErrors(_javaEnv, "getNumFailedCleaningOperations");

  // create a readable error summary

  _errorSummary =
    "Total JOSM validation errors: " + StringUtils::formatLargeNumber(_numValidationErrors) + " / " +
    StringUtils::formatLargeNumber(_numAffected) + " total features.\n";
  _errorSummary +=
    "Total groups of elements cleaned: " +
    StringUtils::formatLargeNumber(_numGroupsOfElementsCleaned) + "\n";
  _errorSummary +=
    "Total elements deleted: " + StringUtils::formatLargeNumber(_deletedElementIds.size()) + "\n";
  _errorSummary +=
    "Total failing JOSM validators: " + QString::number(_numFailingValidators) + "\n";
  _errorSummary +=
    "Total failing JOSM cleaning operations: " + QString::number(_numFailedCleaningOperations) +
    "\n";
  _errorSummary +=
     _errorCountsByTypeToSummaryStr(
       JniConversion::fromJavaStringIntMap(_javaEnv, validationErrorCountsByTypeJavaMap),
       JniConversion::fromJavaStringIntMap(_javaEnv, validationErrorFixCountsByTypeJavaMap));
  _errorSummary = _errorSummary.trimmed();
  LOG_VART(_errorSummary);
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
      QString(indentAfterCount, ' ') + " groups of elements cleaned: " +
      StringUtils::formatLargeNumber(errorFixCountsByType[errorItr.key()]) + "\n";
  }
  return summary;
}

}
