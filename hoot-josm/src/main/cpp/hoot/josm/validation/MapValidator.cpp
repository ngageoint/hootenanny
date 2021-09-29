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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "MapValidator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/info/ApiEntityInfo.h>
#include <hoot/core/info/ApiEntityDisplayInfo.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/validation/Validator.h>

#include <hoot/josm/ops/JosmMapValidator.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

const int MapValidator::FILE_PRINT_SIZE = 40;

void MapValidator::setReportPath(const QString& filePath)
{
  _reportFile = filePath;
  if (!IoUtils::isUrl(_reportFile))
  {
    // Write the output dir the report is in now so we don't get a nasty surprise at the end of a
    // long job that it can't be written.
    IoUtils::writeOutputDir(_reportFile);
  }
}

void MapValidator::printValidators()
{
  QMap<QString, QString> validators = _getJosmValidators();
  validators.unite(_getHootValidators());
  _printValidatorOutput(validators);
}

QString MapValidator::validate(const QStringList& inputs, const QString& output) const
{
  LOG_VARD(inputs);
  LOG_VARD(output);
  if (output.trimmed().isEmpty())
  {
    return _validateSeparateOutput(inputs);
  }
  else
  {
    if (!IoUtils::isSupportedOutputFormat(output))
    {
      throw IllegalArgumentException("Invalid output location: " + output);
    }
    return _validate(inputs, output);
  }
}

QMap<QString, QString> MapValidator::_getJosmValidators()
{
  JosmMapValidator validator;
  validator.setConfiguration(conf());
  return validator.getValidatorDetail();
}

QMap<QString, QString> MapValidator::_getHootValidators()
{
  const QStringList hootValidators = ConfigOptions().getHootValidators();
  LOG_VARD(hootValidators);
  QMap<QString, QString> validatorsInfo;
  for (int i = 0; i < hootValidators.size(); i++)
  {
    std::shared_ptr<ApiEntityInfo> validatorInfo =
      std::dynamic_pointer_cast<ApiEntityInfo>(
        Factory::getInstance().constructObject<OsmMapOperation>(hootValidators.at(i)));
    if (validatorInfo)
    {
      validatorsInfo[validatorInfo->getName()] = validatorInfo->getDescription();
    }
  }
  return validatorsInfo;
}

void MapValidator::_printValidatorOutput(const QMap<QString, QString>& validatorInfo)
{
  for (QMap<QString, QString>::const_iterator itr = validatorInfo.begin();
       itr != validatorInfo.end(); ++itr)
  {
    const QString name = itr.key();
    const QString description = itr.value();
    const int indentAfterName = ApiEntityDisplayInfo::MAX_NAME_SIZE - name.size();
    const int indentAfterDescription = ApiEntityDisplayInfo::MAX_TYPE_SIZE - description.size();
    std::cout << name << QString(indentAfterName, ' ') << description <<
                 QString(indentAfterDescription, ' ') << std::endl;
  }
}

QString MapValidator::_validate(OsmMapPtr& map) const
{
  return _validateWithJosm(map) + "\n\n" + _validateWithHoot(map);
}

QString MapValidator::_validateWithJosm(OsmMapPtr& map) const
{
  JosmMapValidator validator;
  validator.setConfiguration(conf());
  validator.apply(map);
  return validator.getSummary().trimmed();
}

QString MapValidator::_validateWithHoot(OsmMapPtr& map) const
{
  const QStringList hootValidators = ConfigOptions().getHootValidators();
  QString validationSummary;
  int numFeaturesValidated = 0;
  int numValidationErrors = 0;
  int numFailingValidators = 0;
  for (int i = 0; i < hootValidators.size(); i++)
  {
    std::shared_ptr<OsmMapOperation> op =
      Factory::getInstance().constructObject<OsmMapOperation>(hootValidators.at(i));
    if (op)
    {
      std::shared_ptr<Validator> validator = std::dynamic_pointer_cast<Validator>(op);
      if (validator)
      {
        validator->enableValidation();
        try
        {
          op->apply(map);
          const QString validationErrorMessage = validator->getValidationErrorMessage();
          if (!validationErrorMessage.isEmpty())
          {
            validationSummary += validationErrorMessage + "\n";
          }
          numFeaturesValidated += validator->getNumFeaturesValidated();
          numValidationErrors += validator->getNumValidationErrors();
        }
        catch (...)
        {
          numFailingValidators++;
        }
      }
    }
  }
  validationSummary.prepend(
    "Found " + QString::number(numValidationErrors) + " validation errors in " +
    QString::number(numFeaturesValidated) + " features with Hootenanny.\n");
  validationSummary +=
    "Total failing Hootenanny validators: " + QString::number(numFailingValidators);
  return validationSummary.trimmed();
}

QString MapValidator::_validate(const QStringList& inputs, const QString& output) const
{
  QString errorMsg;

  LOG_STATUS("Loading " << inputs.size() << " map(s)...");
  OsmMapPtr map = std::make_shared<OsmMap>();
  QString inputName;
  try
  {
    // We need the whole map in memory to validate it, so no point in trying to stream it in.
    if (inputs.size() == 1)
    {
      inputName = inputs.at(0);
      IoUtils::loadMap(
        map, inputName, true, Status::Unknown1, ConfigOptions().getSchemaTranslationScript());
    }
    else
    {
      // Avoid ID conflicts across multiple inputs.
      for (int i = 0; i < inputs.size(); i++)
      {
        inputName += inputs.at(i) + ";";
      }
      inputName.chop(1);
      IoUtils::loadMaps(
        map, inputs, false, Status::Unknown1, ConfigOptions().getSchemaTranslationScript());
    }
  }
  catch (const HootException& e)
  {
    errorMsg =
      "Validation failed for ..." + inputName.right(FILE_PRINT_SIZE) + ": " + e.getWhat() + ".\n\n";
    LOG_ERROR(errorMsg);
    return errorMsg;
  }

  LOG_STATUS("Validating combined map...");
  // Removing HOOT_HOME to appease the case tests so validation report outputs match between
  // different environments. If this validation wasn't run on a file under HOOT_HOME, this will do
  // nothing.
  QString hootHome = ConfPath::getHootHome();
  if (!hootHome.endsWith("/"))
  {
    hootHome += "/";
  }
  QString inputDisplayName = inputName;
  inputDisplayName.replace(hootHome, "");
  QString validationSummary = "Input: " + inputDisplayName + "\n\n";

  try
  {
    QElapsedTimer timer;
    timer.start();

    validationSummary += _validate(map);

    LOG_STATUS("Validation took " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");

    if (!output.isEmpty())
    {
      LOG_STATUS("Writing validated output to: " << output << "...");
      MapProjector::projectToWgs84(map);
      IoUtils::saveMap(map, output);
    }
  }
  catch (const HootException& e)
  {
    errorMsg =
      "Validation failed for ..." + inputName.right(FILE_PRINT_SIZE) + ": " + e.getWhat() + ".\n\n";
    LOG_ERROR(errorMsg);
    validationSummary += errorMsg;
  }

  LOG_VART(_reportFile);
  if (!_reportFile.isEmpty())
  {
    LOG_STATUS(
      "Writing validation report summary to: ..." << _reportFile.right(FILE_PRINT_SIZE) << "...");
    FileUtils::writeFully(_reportFile, validationSummary);
  }
  return validationSummary;
}

QString MapValidator::_validateSeparateOutput(const QStringList& inputs) const
{
  QString validationSummary;
  QString errorMsg;
  for (int i = 0; i < inputs.size(); i++)
  {
    const QString input = inputs.at(i);
    LOG_VARD(input);

    LOG_INFO(
      "Loading map " << i + 1 << " of " << inputs.size() << ": ..." <<
      input.right(FILE_PRINT_SIZE) << "...");
    OsmMapPtr map = std::make_shared<OsmMap>();
    try
    {
      IoUtils::loadMap(
        map, input, true, Status::Unknown1, ConfigOptions().getSchemaTranslationScript());
    }
    catch (const HootException& e)
    {
      errorMsg = "Validation failed for ..." + input.right(FILE_PRINT_SIZE) + ".\n\n";
      LOG_ERROR(errorMsg << ": " << e.getWhat());
      validationSummary += errorMsg;
      continue;
    }

    LOG_STATUS(
      "Validating map " << i + 1 << " of " << inputs.size() << ": ..." <<
      input.right(FILE_PRINT_SIZE) << "...");
    // See note above.
    QString hootHome = ConfPath::getHootHome();
    if (!hootHome.endsWith("/"))
    {
      hootHome += "/";
    }
    QString inputDisplayName = input;
    inputDisplayName.replace(hootHome, "");
    validationSummary += "Input: " + inputDisplayName + "\n\n";

    try
    {   
      QElapsedTimer timer;
      timer.start();

      validationSummary += _validate(map) + "\n\n";

      LOG_STATUS("Validation took " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");

      // Write the output to a similarly named path as the input with some text appended to the
      // input name.
      const QString output = IoUtils::getOutputUrlFromInput(input, "-validated");
      LOG_INFO(
        "Saving map " << i + 1 << " of " << inputs.size() << ": ..." <<
        output.right(FILE_PRINT_SIZE) << "...");
      MapProjector::projectToWgs84(map);
      IoUtils::saveMap(map, output);
    }
    catch (const HootException& e)
    {
      errorMsg = "Validation failed for ..." + input.right(FILE_PRINT_SIZE) + ".\n\n";
      LOG_ERROR(errorMsg << ": " << e.getWhat());
      validationSummary += errorMsg;
    }
  }
  validationSummary = validationSummary.trimmed();
  if (!_reportFile.isEmpty())
  {
    LOG_STATUS(
      "Writing validation report summary to: ..." << _reportFile.right(FILE_PRINT_SIZE) << "...");
    FileUtils::writeFully(_reportFile, validationSummary);
  }
  return validationSummary;
}

}
