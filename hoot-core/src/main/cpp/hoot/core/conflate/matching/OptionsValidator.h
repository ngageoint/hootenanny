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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */
#ifndef OPTIONS_VALIDATOR_H
#define OPTIONS_VALIDATOR_H

// Qt
#include <QString>

namespace hoot
{

/**
 * Validates some of matcher related configuration options
 */
class OptionsValidator
{
public:

  /**
   * Does basic validation of the configured matchers/mergers
   */
  static void validateMatchers();

  /**
   * This automatically fixes some erroneous matcher configurations.
   */
  static void fixGenericMatcherOrdering();

  /**
   * This is intended primarily as a temporary method to fix the subline matcher, classifier, etc.
   * configurations to ensure consistent conflation results between the command line and the UI
   * via services. It can be disabled via the configuration option, autocorrect.options. See
   * details in method.
   */
  static void fixMisc();

private:

  static bool _isGenericMatcher(const QString& matcher);
  static bool _containsGenericMatcher(const QStringList& matchers);
};

}

#endif // OPTIONS_VALIDATOR_H
