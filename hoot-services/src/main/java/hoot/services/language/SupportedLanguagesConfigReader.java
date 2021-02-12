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

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import java.nio.charset.Charset;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Loads information from a supported languages configuration file
 */
public final class SupportedLanguagesConfigReader
{
  private static final Logger logger = LoggerFactory.getLogger(SupportedLanguagesConfigReader.class);

  private Map<String, String> iso6392To1 = new HashMap<String, String>();
  private Map<String, String> iso6391ToLangName = new HashMap<String, String>();
  private SupportedLanguage[] langs = null;

  public SupportedLanguagesConfigReader()
  {
  }

  /**
   * Reads a supported languages config file
   *
   * @param configStrm config input stream
   * @return an array of supported languages
   */
  public void readConfig(InputStream configStrm) throws Exception
  { 
    String line = null;
    List<SupportedLanguage> supportedLangs = new ArrayList<SupportedLanguage>();
    BufferedReader reader = 
      new BufferedReader(new InputStreamReader(configStrm, Charset.forName("UTF-8")));                         
    while ((line = reader.readLine()) != null) 
    {    
      line = line.trim();
      logger.trace("line: " + line);
      if (!line.isEmpty() && !line.startsWith("#"))
      {  
        String[] lineParts = line.split("\t");
        if (lineParts.length != 3)
        {
          throw new IOException("Invalid supported languages config entry: " + line);
        }

        String iso6391 = lineParts[0];
        String iso6392 = lineParts[1];
        String langName = lineParts[2];
        logger.trace(langName);

        SupportedLanguage supportedLanguage = new SupportedLanguage();
        //This probably isn't the best way to handle this.  Only a small number of the languages 
        //we're working with don't have 1 codes and only have 2 codes...works for now, though.
        if (iso6391.toUpperCase().equals("N/A"))
        {
          iso6391 = iso6392;
        }
        supportedLanguage.setIso6391Code(iso6391);
        supportedLanguage.setIso6392Code(iso6392);
        supportedLanguage.setName(langName);
        supportedLangs.add(supportedLanguage);

        if (iso6392To1.containsKey(iso6392))
        {
          throw new IOException(
            "Invalid supported languages configuration.  Duplicate ISO-6392-2 value: " + iso6392);
        }
        iso6392To1.put(iso6392, iso6391);
        if (iso6391ToLangName.containsKey(iso6391))
        {
          throw new IOException(
            "Invalid supported languages configuration.  Duplicate ISO-6392-1 value: " + iso6391);
        }
        iso6391ToLangName.put(iso6391, langName);
      }
    }
    langs = supportedLangs.toArray(new SupportedLanguage[]{});
  }

  public SupportedLanguage[] getSupportedLanguages() { return langs; }

  /**
   * Determines if a language is supported
   *
   * @param iso6391Code language code
   * @return true if the language is supported; false otherwise
   */
  public boolean isSupportedLanguage(String iso6391Code)
  {
    return iso6391ToLangName.containsKey(iso6391Code.toLowerCase());
  }

  /**
   * Given a language code, retrieves the ISO-639-1 code
   *
   * @param iso639Code a language code
   * @return a ISO-639-1 code; empty string if the code cannot be determined
   */
  public String getIso6391Code(String iso639Code)
  {
    if (iso6391ToLangName.containsKey(iso639Code))
    {
      return iso639Code;
    }
    if (iso6392To1.containsKey(iso639Code))
    {
      return iso6392To1.get(iso639Code);
    }
    return "";
  }

  public String getLanguageName(String iso6391Code)
  { 
    return iso6391ToLangName.get(iso6391Code);
  }
}
