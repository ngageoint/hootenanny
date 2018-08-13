
package hoot.services.language;

import org.apache.commons.lang3.ClassUtils;
import org.apache.commons.lang3.reflect.ConstructorUtils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class LanguageDetectorFactory
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageDetectorFactory.class);

  private LanguageDetectorFactory()
  {
  }

  public static LanguageDetector create(String detectorClass)
  {
    try 
    {
      return 
        (LanguageDetector)ConstructorUtils.invokeConstructor(
          Class.forName(ClassUtils.getPackageName(LanguageDetectorFactory.class) + "." + detectorClass), new Object[]{});
    }
    catch (Exception e) 
    {
      throw new RuntimeException("Error creating " + detectorClass + ".", e);
    }
  }
}
