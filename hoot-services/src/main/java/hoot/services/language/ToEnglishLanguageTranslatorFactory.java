
package hoot.services.controllers.language;

import org.apache.commons.lang3.ClassUtils;
import org.apache.commons.lang3.reflect.ConstructorUtils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ToEnglishLanguageTranslatorFactory
{
  private static final Logger logger = LoggerFactory.getLogger(ToEnglishLanguageTranslatorFactory.class);

  private ToEnglishLanguageTranslatorFactory()
  {
  }

  public static ToEnglishLanguageTranslator create(String translatorClass)
  {
    try 
    {
      return 
        (ToEnglishLanguageTranslator)ConstructorUtils.invokeConstructor(
          Class.forName(ClassUtils.getPackageName(ToEnglishLanguageTranslatorFactory.class) + "." + translatorClass), new Object[]{});
    }
    catch (Exception e) 
    {
      throw new RuntimeException("Error creating " + translatorClass + ".", e);
    }
  }
}
