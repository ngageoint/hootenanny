
package hoot.services.controllers.language;

import org.apache.commons.lang3.ClassUtils;

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
      //TODO: fix
      return (LanguageDetector)Class.forName(ClassUtils.getPackageName(LanguageDetectorFactory.class) + "." + detectorClass);
    }
    catch (Exception e) 
    {
      throw new RuntimeException("Error creating " + detectorClass + ".", e);
    }
  }
}
