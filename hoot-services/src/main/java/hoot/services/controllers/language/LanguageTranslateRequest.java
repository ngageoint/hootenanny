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

package hoot.services.controllers.language;

/**
 * A language translation request
 */
public class LanguageTranslateRequest 
{
  private String[] detectors;
  private String translator;
  private String[] sourceLangCodes;
  private String text;
  private boolean detectedLanguageOverridesSpecifiedSourceLanguages = false;
  private boolean performExhaustiveTranslationSearchWithNoDetection = false;

  public LanguageTranslateRequest() {}

  public String[] getDetectors() { return detectors; }
  public void setDetectors(String[] detectors) { this.detectors = detectors; }

  public String getTranslator() { return translator; }
  public void setTranslator(String translator) { this.translator = translator; }

  public String[] getSourceLangCodes() { return sourceLangCodes; }
  public void setSourceLangCodes(String[] sourceLangCodes) 
  { this.sourceLangCodes = sourceLangCodes; }

  public String getText() { return text; }
  public void setText(String text) { this.text = text; }

  public boolean getDetectedLanguageOverridesSpecifiedSourceLanguages() 
  { return detectedLanguageOverridesSpecifiedSourceLanguages; }
  public void setDetectedLanguageOverridesSpecifiedSourceLanguages(boolean override) 
  { this.detectedLanguageOverridesSpecifiedSourceLanguages = override; }

  public boolean getPerformExhaustiveTranslationSearchWithNoDetection() 
  { return performExhaustiveTranslationSearchWithNoDetection; }
  public void setPerformExhaustiveTranslationSearchWithNoDetection(boolean performExhaustiveSearch) 
  { this.performExhaustiveTranslationSearchWithNoDetection = performExhaustiveSearch; }
}
