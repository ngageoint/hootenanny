
package hoot.services.utils;

import java.io.IOException;
import java.util.Set;
import java.util.HashSet;

import com.google.common.reflect.ClassPath;
import com.google.common.collect.ImmutableSet;

//import org.reflections.Reflections;

/**
 * 
 */
public class ReflectUtils 
{
  //given "JoshuaTranslator" and "hoot.services.language", returns "hoot.services.language.joshua.JoshuaTranslator"
  //This can be expensive, so you may want to cache results if possible.
  //This could be redone with Reflections in place of guava.
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

  //TODO: any way to make this work?
  /*public static <T> Set<String> getSimpleClassNames(String topLevelPackage)
  {
    Set<String> classNames = new HashSet<String>();
    Set<Class<? extends T>> classes = (new Reflections(topLevelPackage)).getSubTypesOf(T.class);
    for (Class<? extends T> clazz : classes)
    {
      classNames.add(clazz.getSimpleName());
    }
    return classNames;
  }*/
}
