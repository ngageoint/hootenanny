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
#ifndef MAP_VALIDATOR_H
#define MAP_VALIDATOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * This is a wrapper around both JOSM and hoot validators.
 *
 * Note: DataQualityMetricTagger has some validator type logic in it, but think there's a lot of
 * overlap between the JOSM validators. May be worth looking into using parts of it.
 */
class MapValidator
{

public:

  static const int FILE_PRINT_SIZE;

  MapValidator() = default;
  ~MapValidator() = default;

  /**
   * @brief validate Runs validations on inputs, optionally writes validation errors to output, and
   * displays a report or generates a report file
   * @param inputs URLs to validate
   * @param output optional combined copy of the input with validation errors
   * @return validation report text
   */
  QString validate(const QStringList& inputs, const QString& output = QString()) const;

  /**
   * @brief printValidators Displays the available validators
   */
  static void printValidators();

  /**
   * @brief setReportPath Sets a path for an output report
   * @param filePath path to write the report to
   */
  void setReportPath(const QString& filePath);

private:

  // If this is populated, the validation summary will be written to a file instead of displayed.
  QString _reportFile;

  static void _printValidatorOutput(const QMap<QString, QString>& validatorInfo);
  static QMap<QString, QString> _getJosmValidators();
  static QMap<QString, QString> _getHootValidators();

  // This combines all inputs, writes them all to the same optional output, and calculates a
  // combined validation summary for all the inputs.
  QString _validate(const QStringList& inputs, const QString& output) const;
  // This writes a separate output for each input and calculates the combined validation summary.
  QString _validateSeparateOutput(const QStringList& inputs) const;
  QString _validate(OsmMapPtr& map) const;
  QString _validateWithJosm(OsmMapPtr& map) const;
  QString _validateWithHoot(OsmMapPtr& map) const;
};

}

#endif // MAP_VALIDATOR_H
