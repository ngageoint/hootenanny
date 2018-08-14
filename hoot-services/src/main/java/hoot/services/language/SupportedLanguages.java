
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
public final class SupportedLanguages
{
  private static final Logger logger = LoggerFactory.getLogger(SupportedLanguages.class);

  private List<SupportedLanguage> supportedLanguages = new ArrayList<SupportedLanguage>();

  Map<String, String> iso6392To1 = new HashMap<String, String>();
  Map<String, String> iso6391ToLang = new HashMap<String, String>();
  Map<String, Boolean> iso6391ToDetectable = new HashMap<String, Boolean>();

  private static SupportedLanguages instance;

  private SupportedLanguages() throws IOException, RuntimeException
  {
    InputStream configStrm = null;
    try
    {
      //TODO: read path from config
      configStrm = 
        SupportedLanguages.class.getClassLoader().getResourceAsStream("language-translation/supportedToEnglishTranslationLanguages"); 
      if (configStrm != null)
      {
        readConfig(configStrm); 
      }
      else 
      {
        throw new IOException("");
      }  
    }
    catch (IOException ioe) 
    {
      throw new RuntimeException("Error reading supported languages", ioe);
    }
    finally
    {
      if (configStrm != null)
      {
        configStrm.close();
      }
    }
  }

  public synchronized static SupportedLanguages getInstance() throws IOException
  {
    if (instance == null)
    { 
      instance = new SupportedLanguages();
    }
    return instance;
  }

  //this may end up being faster
  /*private static class StaticHolder 
  {
    static final SupportedLanguages INSTANCE = new SupportedLanguages();
  }
 
  public static SupportedLanguages getInstance() 
  {
    return StaticHolder.INSTANCE;
  }*/

  public void readConfig(InputStream configStrm) throws IOException
  { 
    String line = null;
    BufferedReader reader = new BufferedReader(new InputStreamReader(configStrm));                         
    while ((line = reader.readLine()) != null) 
    {    
      line = line.trim();
      if (!line.isEmpty() && !line.startsWith("#"))
      {  
        String[] lineParts = line.split("\t");
        if (lineParts.length != 4)
        {
          throw new IOException("Invalid supported languages config entry: " + line);
        }

        String iso6391 = lineParts[0];
        String iso6392 = lineParts[1];
        String lang = lineParts[2];
        final boolean detectable = lineParts[3].toLowerCase() == "yes" ? true : false;

        SupportedLanguage supportedLanguage = new SupportedLanguage();
        supportedLanguage.setIso6391Code(iso6391);
        supportedLanguage.setIso6392Code(iso6392);
        supportedLanguage.setLanguage(lang);
        supportedLanguage.setDetectable(detectable);
        supportedLanguages.add(supportedLanguage);

        if (iso6392To1.containsKey(iso6392))
        {
          throw new Exception("Invalid supported languages configuration.  Duplicate ISO-6392-2 value: " + iso6392);
        }
        iso6392To1.put(iso6392, iso6391);
        if (iso6391ToLang.containsKey(iso6391))
        {
          throw new Exception("Invalid supported languages configuration.  Duplicate ISO-6392-1 value: " + iso6391);
        }
        iso6391ToLang.put(iso6391, lang);
        iso6391ToDetectable.put(iso6391, detectable);
      }
    }
  }

  public SupportedLanguage[] getSupportedLanguages() { return supportedLanguages.toArray(new SupportedLanguage[]{}); }

  public boolean isSupportedLanguage(String iso6391Code)
  {
    return iso6391ToLang.containsKey(iso6391Code);
  }

  public String getIso6391Code(String iso639Code)
  {
    if (iso6391ToLang.containsKey(iso639Code))
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
    return iso6391ToLang.get(iso6391Code);
  }

  boolean isDetectableLanguage(String iso6391Code)
  { 
    return iso6391ToDetectable.get(iso6391Code);
  } 
}
