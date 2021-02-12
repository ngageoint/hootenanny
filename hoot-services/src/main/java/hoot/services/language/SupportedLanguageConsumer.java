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
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */

package hoot.services.language;

import java.lang.CloneNotSupportedException;

/**
 * Interface for a language app that has supported languages
 */
public interface SupportedLanguageConsumer 
{
  /**
   * Return all languages supported by this app
   *
   * @return an array of supported languages
   */
  SupportedLanguage[] getSupportedLanguages() throws CloneNotSupportedException;

  /**
   * Determines whether this language available for the given app (has it been activated; 
     subset of supported)
   *
   * @return true if the language is supported; false otherwise
   */
  boolean isLanguageAvailable(String langCode);

  /**
   * Determines the language name given an ISO-639-1 code
   *
   * @param an ISO-639-1 code
   * @return a language name
   */
  String getLanguageName(String langCode);
}
