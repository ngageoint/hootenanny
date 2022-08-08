/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2021, 2022 Maxar (http://www.maxar.com/)
 */

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
