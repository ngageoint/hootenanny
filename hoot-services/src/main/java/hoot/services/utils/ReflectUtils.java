
package hoot.services.utils;

import java.io.IOException;

import com.google.common.reflect.ClassPath;
import com.google.common.collect.ImmutableSet;

/**
 * 
 */
public final class ReflectUtils 
{
  //given "JoshuaTranslator" and "hoot.services.language", returns "hoot.services.language.joshua.JoshuaTranslator"
  //This can be expensive, so you may want to cache results if possible.
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
