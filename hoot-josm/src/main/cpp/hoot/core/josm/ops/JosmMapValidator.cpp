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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "JosmMapValidator.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/jni/JniConversion.h>
#include <hoot/core/jni/JniUtils.h>

// Qt
#include <QTemporaryFile>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, JosmMapValidator)

JosmMapValidator::JosmMapValidator() :
JosmMapValidatorAbstract()
{
}

OsmMapPtr JosmMapValidator::_getUpdatedMap(OsmMapPtr& inputMap)
{
  LOG_DEBUG("Retrieving validated map...");
  LOG_VARD(inputMap->size());
  LOG_VARD(_josmValidators.size());

  // see related note in JosmMapCleaner::_getUpdatedMap
  if ((int)inputMap->size() > _maxElementsForMapString)
  {
    // pass map as temp file and get it back as a temp file

    std::shared_ptr<QTemporaryFile> tempInputFile(
      new QTemporaryFile(
        ConfigOptions().getApidbBulkInserterTempFileDir() + "/JosmMapValidator-in.osm"));
    tempInputFile->setAutoRemove(true);
    if (!tempInputFile->open())
    {
      throw HootException(
          "Unable to open temp input file for validation: " + tempInputFile->fileName() + ".");
    }
    LOG_DEBUG("Writing temp map to " << tempInputFile->fileName() << "...");
    OsmXmlWriter().write(inputMap, tempInputFile->fileName());

    const QString tempOutputPath = tempInputFile->fileName().replace("in", "out");

    _validate(_josmValidators, tempInputFile->fileName(), tempOutputPath);

    LOG_DEBUG("Reading validated map from " << tempOutputPath << "...");
    OsmMapPtr validatedMap(new OsmMap());
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    reader.setUseFileStatus(true);
    reader.open(tempOutputPath);
    reader.read(tempOutputPath, validatedMap);
    return validatedMap;
  }
  else
  {
    // pass map as string and get it back as a string
    return
      OsmXmlReader::fromXml(
        _validate(
          _josmValidators,
          OsmXmlWriter::toString(inputMap, false)).trimmed(), true, true, false, true);
  }
}

QString JosmMapValidator::_validate(const QStringList& validators, const QString& map)
{
  // JNI sig format: (input params...)return type
  jstring validatedMapJavaStr =
    (jstring)_javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig:
      // String validate(
      //   List<String> validators, String elementsXml, boolean cleanValidated, boolean addTags)
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "validate",
        "(Ljava/util/List;Ljava/lang/String;ZZ)Ljava/lang/String;"),
      JniConversion::toJavaStringList(_javaEnv, validators),
      JniConversion::toJavaString(_javaEnv, map),
      false,
      true);
  JniUtils::checkForErrors(_javaEnv, "validateFromMapString");
  return JniConversion::fromJavaString(_javaEnv, validatedMapJavaStr);
}

void JosmMapValidator::_validate(
  const QStringList& validators, const QString& inputMapPath, const QString& outputMapPath)
{
  // JNI sig format: (input params...)return type
  _javaEnv->CallVoidMethod(
    _josmInterface,
    // Java sig:
    // void validate(
    //   List<String> validators, String elementsFileInputPath, String elementsFileOutputPath,
    //   boolean cleanValidated, boolean addTags)
    _javaEnv->GetMethodID(
      _josmInterfaceClass, "validate",
      "(Ljava/util/List;Ljava/lang/String;Ljava/lang/String;ZZ)V"),
    JniConversion::toJavaStringList(_javaEnv, validators),
    JniConversion::toJavaString(_javaEnv, inputMapPath),
    JniConversion::toJavaString(_javaEnv, outputMapPath),
    false,
    true);
  JniUtils::checkForErrors(_javaEnv, "validateFromMapFile");
}

}
