
package hoot.services.language;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

/*
*/
public final class SupportedLanguagesConfigReader
{
  private static final Logger logger = LoggerFactory.getLogger(SupportedLanguagesConfigReader.class);

  private Map<String, String> iso6392To1 = new HashMap<String, String>();
  private Map<String, String> iso6391ToLangName = new HashMap<String, String>();

  public SupportedLanguagesConfigReader()
  {
  }

  public SupportedLanguage[] readConfig(InputStream configStrm) throws Exception
  { 
    String line = null;
    List<SupportedLanguage> supportedLangs = new ArrayList<SupportedLanguage>();
    BufferedReader reader = new BufferedReader(new InputStreamReader(configStrm));                         
    while ((line = reader.readLine()) != null) 
    {    
      line = line.trim();
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

        SupportedLanguage supportedLanguage = new SupportedLanguage();
        //TODO: this isn't the best way to handle this
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
          throw new Exception("Invalid supported languages configuration.  Duplicate ISO-6392-2 value: " + iso6392);
        }
        iso6392To1.put(iso6392, iso6391);
        if (iso6391ToLangName.containsKey(iso6391))
        {
          throw new Exception("Invalid supported languages configuration.  Duplicate ISO-6392-1 value: " + iso6391);
        }
        iso6391ToLangName.put(iso6391, langName);
      }
    }
    return supportedLangs.toArray(new SupportedLanguage[]{});
  }

  public boolean isSupportedLanguage(String iso6391Code)
  {
    return iso6391ToLangName.containsKey(iso6391Code.toLowerCase());
  }

  public String getIso6391Code(String iso639Code)
  {
    if (iso6391ToLangName.containsKey(iso639Code))
    {
      return iso639Code;
    }
    //TODO: don't think we need this check
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
