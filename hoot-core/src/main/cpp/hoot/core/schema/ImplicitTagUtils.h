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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef IMPLICITTAGUTILS_H
#define IMPLICITTAGUTILS_H

// Hoot
#include <hoot/core/language/ToEnglishTranslator.h>

// Qt
#include <QStringList>

namespace hoot
{

class Tags;

/**
 * Miscellaneous functionality used by multiple implicit tagging classes
 */
class ImplicitTagUtils
{

public:

  /**
   * Cleans unwanted parts from a feature name string before implicit tag processing
   *
   * @param name name to clean
   */
  static void cleanName(QString& name);

  /**
   * Translates all input names to English
   *
   * @param names names to translate
   * @param tags tags to retrieve additional names from
   * @param translator a to English word translator
   * @return a collection of translated name strings
   */
  static QStringList translateNamesToEnglish(const QStringList names, const Tags& tags,
                                             std::shared_ptr<ToEnglishTranslator> translator);

  /**
   * Determines if the UNIX sort command is available
   *
   * @return true if the command is available; false otherwise
   */
  static bool sortCommandExists();

private:

  static QStringList _nameCleaningTokens;
  static QStringList _streetTypes;

  static void _modifyUndesirableTokens(QString& name);
  static void _filterOutStreets(QString& name);
};

}

#endif // IMPLICITTAGUTILS_H
