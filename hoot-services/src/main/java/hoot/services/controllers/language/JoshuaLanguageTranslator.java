
package hoot.services.controllers.language;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

//no setters
//all fields private and final
//constructor inits all fields with a deep copy
//Perform cloning of objects in the getter methods to return a copy rather than returning the actual object reference.
/*
@article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015} }
*/
public final class JoshuaLanguageTranslator implements LanguageTranslator
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaLanguageTranslator.class);

  

  private static JoshuaLanguageTranslator instance;

  private JoshuaLanguageTranslator() //throws RuntimeException
  {
    /*try
    {
      
    }
    catch (IOException ioe) 
    {
      throw new RuntimeException("", ioe);
    }*/
  }

  public synchronized static JoshuaLanguageTranslator getInstance()
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

  public String translate(String text)
  {
    logger.debug("Translating with " + getClass().getName() + "; text: " + text + "...");
    String translatedText = "";
    if (!translatedText.isEmpty() && !translatedText.equals(text))
    {
      logger.debug(getClass().getName() + " translated: " + text + " to: " + translatedText);
    }
    return translatedText;
  }
}
