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
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */

package hoot.services.language;

/**
 * Interface for language translators
 */
public interface ToEnglishTranslator 
{
  /**
   * Sets a configuration
   *
   * @param config a configuration object
   */
  void setConfig(Object config);

  /**
   * Translates text to English
   *
   * @param sourceLangCodes the specified source languages to attempt to translate from
   * @param text the text to translate
   */
  String translate(String[] sourceLangCodes, String text) throws Exception;

  /**
   * Translates text to English
   *
   * @param sourceLangCodes the specified source language to attempt to translate from
   * @param text the text to translate
   */
  String translate(String sourceLangCode, String text) throws Exception;
}
