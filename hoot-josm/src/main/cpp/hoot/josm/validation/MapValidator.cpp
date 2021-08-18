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
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/info/ApiEntityDisplayInfo.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

void MapValidator::setReportFile(const QString& file)
{
  _reportFile = file;
  if (!IoUtils::isUrl(file))
  {
    // Write the output dir the report is in now so we don't get a nasty surprise at the end
    // of a long job that it can't be written.
    IoUtils::writeOutputDir(file);
  }
}

void MapValidator::printValidators()
{
  _printJosmValidators();
}

void MapValidator::_printJosmValidators()
{
  JosmMapValidator validator;
  validator.setConfiguration(conf());
  _printValidatorOutput(validator.getValidatorDetail());
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

QString MapValidator::validate(const QStringList& inputs, const QString& output) const
{
  if (output.trimmed().isEmpty())
  {
    return _validateSeparateOutput(inputs);
  }
  else
  {
    return _validate(inputs, output);
  }
}

std::shared_ptr<JosmMapValidator> MapValidator::validate(OsmMapPtr& map) const
{
  std::shared_ptr<JosmMapValidator> validator = std::make_shared<JosmMapValidator>();
  validator->setConfiguration(conf());
  LOG_STATUS(validator->getInitStatusMessage());
  validator->apply(map);
  return validator;
}

QString MapValidator::_validate(const QStringList& inputs, const QString& output) const
{
  LOG_STATUS("Loading " << inputs.size() << " map(s)...");
  OsmMapPtr map = std::make_shared<OsmMap>();
  QString inputName;
  // We need the whole map in memory to validate it, so no point in trying to stream it in.
  if (inputs.size() == 1)
  {
    inputName = inputs.at(0);
    IoUtils::loadMap(map, inputs.at(0), true, Status::Unknown1);
  }
  else
  {
    // Avoid ID conflicts across multiple inputs.
    for (int i = 0; i < inputs.size(); i++)
    {
      inputName += inputs.at(i) + ";";
    }
    inputName.chop(1);
    IoUtils::loadMaps(map, inputs, false, Status::Unknown1);
  }

  LOG_STATUS("Validating combined map...");
  std::shared_ptr<JosmMapValidator> validator = validate(map);

  if (!output.isEmpty())
  {
    MapProjector::projectToWgs84(map);
    IoUtils::saveMap(map, output);
  }

  const QString validationSummary = "Input: " + inputName + "\n" + validator->getSummary();
  if (!_reportFile.isEmpty())
  {
    LOG_STATUS("Writing validation report summary to: ..." << _reportFile.left(25) << "...");
    FileUtils::writeFully(_reportFile, validationSummary);
  }
  return validationSummary;
}

QString MapValidator::_validateSeparateOutput(const QStringList& inputs) const
{
  QString validationSummary;
  for (int i = 0; i < inputs.size(); i++)
  {
    const QString input = inputs.at(i);

    LOG_STATUS(
      "Loading map " << i + 1 << " of " << inputs.size() << ": ..." << input.right(25) <<
      "...");
    OsmMapPtr map = std::make_shared<OsmMap>();
    IoUtils::loadMap(map, input, true, Status::Unknown1);

    LOG_STATUS(
      "Validating map " << i + 1 << " of " << inputs.size() << ": ..." << input.right(25) <<
      "...");
    validationSummary += "Input: " + input + "\n";
    validationSummary += validate(map)->getSummary() + "\n\n";

    // Write the output to a similarly named path as the input with some text appended to the
    // input name.
    const QString output = IoUtils::getOutputUrlFromInput(input, "-validated");
    LOG_STATUS(
      "Saving map " << i + 1 << " of " << inputs.size() << ": ..." << output.right(25) <<
      "...");
    MapProjector::projectToWgs84(map);
    IoUtils::saveMap(map, output);
  }
  validationSummary = validationSummary.trimmed();
  if (!_reportFile.isEmpty())
  {
    LOG_STATUS("Writing validation report summary to: ..." << _reportFile.left(25) << "...");
    FileUtils::writeFully(_reportFile, validationSummary);
  }
  return validationSummary;
}

}
