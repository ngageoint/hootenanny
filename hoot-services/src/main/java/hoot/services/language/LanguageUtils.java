
package hoot.services.language;

import java.util.Map;
import java.util.HashMap;

import java.io.IOException;

import org.apache.commons.lang3.ClassUtils;

import hoot.services.utils.ReflectUtils;

/**
 * Various general utilities for language detection/translation
 */
public class LanguageUtils 
{
  private static Map<String, String> simpleClassNamesToFullClassNamesCache = new HashMap<String, String>();

  /**
   * Returns a full class name given a simple class name
   *
   * @param simpleClassName class name for which to retrieve the full class name; must reside at or below the package level of this class
   * @return a class name
   */
  public static String getFullClassName(String simpleClassName) throws IOException
  {
    String fullClassName = null;
    //full class name retrieval can be expensive, so let's cache
    fullClassName = simpleClassNamesToFullClassNamesCache.get(simpleClassName);
    if (fullClassName == null)
    {
      fullClassName = ReflectUtils.getFullClassName(simpleClassName, ClassUtils.getPackageName(LanguageUtils.class));
      simpleClassNamesToFullClassNamesCache.put(simpleClassName, fullClassName);
    }
    return fullClassName;
  }
}
