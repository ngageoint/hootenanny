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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

package hoot.services.language;

import java.util.Map;
import java.util.HashMap;

import org.apache.commons.lang3.ClassUtils;
import org.apache.commons.lang3.reflect.MethodUtils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.utils.ReflectUtils;

public class LanguageDetectorFactory
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageDetectorFactory.class);

  private static Map<String, String> classNamesToFullClassNamesCache = new HashMap<String, String>();

  private LanguageDetectorFactory()
  {
  }

  public static LanguageDetector create(String className) throws Exception
  {
    String fullClassName = null;
    try 
    {
      //full class name retrieval is expensive, so let's cache
      if (!classNamesToFullClassNamesCache.containsKey(className))
      {
        fullClassName = ReflectUtils.getFullClassName(className, ClassUtils.getPackageName(LanguageDetectorFactory.class));
        classNamesToFullClassNamesCache.put(className, fullClassName);
      }
      else
      {
        fullClassName = classNamesToFullClassNamesCache.get(className);
      }
      return (LanguageDetector)MethodUtils.invokeStaticMethod(Class.forName(fullClassName), "getInstance", null);
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
