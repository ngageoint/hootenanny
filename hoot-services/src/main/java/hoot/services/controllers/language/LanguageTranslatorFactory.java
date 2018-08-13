
package hoot.services.controllers.language;

import org.apache.commons.lang3.ClassUtils;
import org.apache.commons.lang3.reflect.ConstructorUtils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class LanguageTranslatorFactory
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageTranslatorFactory.class);

  private LanguageTranslatorFactory()
  {
  }

  public static LanguageTranslator create(String translatorClass)
  {
    try 
    {
      return 
        (LanguageTranslator)ConstructorUtils.invokeConstructor(
          Class.forName(ClassUtils.getPackageName(LanguageTranslatorFactory.class) + "." + translatorClass), new Object[]{});
    }
    catch (Exception e) 
    {
      throw new RuntimeException("Error creating " + translatorClass + ".", e);
    }
  }
}
