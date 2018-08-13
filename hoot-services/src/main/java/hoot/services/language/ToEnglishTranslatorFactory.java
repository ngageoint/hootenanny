
package hoot.services.language;

import org.apache.commons.lang3.ClassUtils;
import org.apache.commons.lang3.reflect.MethodUtils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ToEnglishTranslatorFactory
{
  private static final Logger logger = LoggerFactory.getLogger(ToEnglishTranslatorFactory.class);

  private ToEnglishTranslatorFactory()
  {
  }

  public static ToEnglishTranslator create(String className) throws Exception
  {
    String fullClassName = null;
    try 
    {
      fullClassName = ClassUtils.getPackageName(ToEnglishTranslatorFactory.class) + "." + className;
      return (ToEnglishTranslator)MethodUtils.invokeStaticMethod(Class.forName(fullClassName), "getInstance", null);
    }
    catch (Exception e) 
    {
      String msg = "Error creating ";
      if (fullClassName != null)
      {
        msg += fullClassName;
      }
      else
      {
        msg += className;
      }
      throw new Exception(msg + "; error: " + e.getMessage());
    }
  }
}
