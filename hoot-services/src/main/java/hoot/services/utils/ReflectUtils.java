
package hoot.services.utils;

import java.io.IOException;
import java.util.Set;
import java.util.HashSet;

import com.google.common.reflect.ClassPath;
import com.google.common.collect.ImmutableSet;

/**
 * Various utilities for obtaining reflective information about classes
 */
public class ReflectUtils 
{
  /**
   * Retrieves a full class name for a class
   * 
   * e.g. given "JoshuaTranslator" and "hoot.services.language", returns "hoot.services.language.joshua.JoshuaTranslator"
   *
   * This can be expensive, so you may want to cache results if possible.
   *
   * @param className simple class name
   * @param topLevelPackage package the class is known to be under (doesn't have to be directly under it)
   * @return the full class name of the class, if found; null otherwise
   */
  public static String getFullClassName(String className, String topLevelPackage) throws IOException
  {
    ImmutableSet<ClassPath.ClassInfo> classInfos = 
      ClassPath.from(Thread.currentThread().getContextClassLoader()).getTopLevelClassesRecursive(topLevelPackage);
    for (ClassPath.ClassInfo classInfo : classInfos)
    {
      String fullClassName = classInfo.getName();
      if (fullClassName.contains(className))
      {
        return fullClassName;
      }
    }
    return null;
  }
}
