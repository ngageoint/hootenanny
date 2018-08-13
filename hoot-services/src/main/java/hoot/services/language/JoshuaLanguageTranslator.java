
package hoot.services.language;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.io.File;
import java.io.StringReader;

import java.util.Map;
import java.util.HashMap;
import java.util.ArrayList;

import org.apache.commons.io.FileUtils;

import org.apache.joshua.decoder.Decoder;
import org.apache.joshua.decoder.JoshuaConfiguration;
import org.apache.joshua.decoder.Translation;
import org.apache.joshua.decoder.TranslationResponseStream;
import org.apache.joshua.decoder.io.TranslationRequestStream;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/*
@article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015} }
*/
public final class JoshuaLanguageTranslator implements ToEnglishTranslator
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaLanguageTranslator.class);

  private Map<String, Decoder> decoders = new HashMap<String, Decoder>();
  private Map<String, JoshuaConfiguration> configs = new HashMap<String, JoshuaConfiguration>();
  private static final Charset FILE_ENCODING = Charset.forName("UTF-8");

  private static JoshuaLanguageTranslator instance;

  private JoshuaLanguageTranslator() throws IOException
  {
    logger.error("Initializing Joshua...");

    //TODO: read from hoot services config
    Map<String, String> langPacks = new HashMap<String, String>();    
    langPacks.put("de", "/home/vagrant/joshua-language-packs/apache-joshua-de-en-2016-11-18");
    //langPacks.put("es", "/home/vagrant/joshua-language-packs/apache-joshua-es-en-2016-11-18");

    int ctr = 1;
    for (Map.Entry<String, String> langPack : langPacks.entrySet()) 
    {
      long startTime = System.currentTimeMillis();

      String lang = langPack.getKey();
      String langPackPath = langPack.getValue();    
      logger.error("Loading language model " + ctr + " / " + langPacks.size() + " for lang: " + lang + " from path: " + langPackPath + "...");  
      
      //change all model relative paths to absolute in the config
      String configPath = langPackPath + "/joshua.config";
      File configFile = new File(configPath);
      String configContent = FileUtils.readFileToString(configFile, FILE_ENCODING);
      configContent = configContent.replaceAll(" model/", " " + langPackPath + "/model/");
      FileUtils.writeStringToFile(configFile, configContent, FILE_ENCODING);

      JoshuaConfiguration config = new JoshuaConfiguration();
      config.readConfigFile(configPath);
      config.setConfigFilePath(new File(configPath).getCanonicalFile().getParent());
      config.sanityCheck();
      configs.put(lang, config);
      
      Decoder decoder = new Decoder(config, configPath);
      decoders.put(lang, decoder);

      logger.error("Model loading for lang: " + lang + " took {} seconds", (System.currentTimeMillis() - startTime) / 1000);
      logger.error("Memory used {} MB", ((Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()) / 1000000.0));
      ctr++;
    }
  }

  public synchronized static JoshuaLanguageTranslator getInstance() throws IOException
  {
    if (instance == null)
    { 
      instance = new JoshuaLanguageTranslator();
    }
    return instance;
  }

  //this may end up being faster
  /*private static class StaticHolder 
  {
    static final JoshuaLanguageTranslator INSTANCE = new JoshuaLanguageTranslator();
  }
 
  public static JoshuaLanguageTranslator getInstance() 
  {
    return StaticHolder.INSTANCE;
  }*/

  public String translate(String sourceLangCode, String text) throws Exception
  {
    sourceLangCode = sourceLangCode.toLowerCase();
    if (!decoders.containsKey(sourceLangCode))
    {
      throw new Exception("No language translator available for language: " + sourceLangCode);
    }

    String translatedText = "";
    Decoder decoder = null;
    BufferedReader reader = null;
    try
    {
      long startTime = System.currentTimeMillis();
      logger.debug("Translating with " + getClass().getName() + "; text: " + text + "...");

      reader = new BufferedReader(new StringReader(text));
      TranslationRequestStream request = new TranslationRequestStream(reader, configs.get(sourceLangCode));
      decoder = decoders.get(sourceLangCode);
      TranslationResponseStream translationResponseStream = decoder.decodeAll(request);
      int numTranslations = 0;
      for (Translation translation: translationResponseStream) 
      {
        translatedText = translation.toString();
        numTranslations++;
      }
      if (numTranslations > 1)
      {
        throw new Exception("More than one translation found: " + numTranslations);
      }
    
      if (!translatedText.isEmpty() && !translatedText.equals(text))
      {
        logger.debug(getClass().getName() + " translated: " + text + " to: " + translatedText);
      }

      logger.debug("Memory used {} MB", ((Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()) / 1000000.0));
      logger.debug("Total running time: {} seconds",  (System.currentTimeMillis() - startTime) / 1000);
    }
    finally
    {
      if (reader != null)
      { 
        reader.close();
      }
      if (decoder != null)
      { 
        decoder.cleanUp();
      }
    }    

    return translatedText;
  }
}
