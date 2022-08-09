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
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */

package hoot.services.language;

import java.util.Set;
import java.util.TreeSet;

import org.apache.commons.lang3.reflect.MethodUtils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.reflections.Reflections;

/**
 * Creates language detectors
 */
public class LanguageDetectorFactory
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageDetectorFactory.class);

  private static Set<String> simpleClassNames = null;

  private LanguageDetectorFactory()
  {
  }

  /**
   * Creates a language detector
   *
   * @param className the simple class name of the detector
   * @return a language detector
   */
  public static LanguageDetector create(String className) throws Exception
  {
    String fullClassName = null;
    try 
    {
      fullClassName = LanguageUtils.getFullClassName(className);
      return 
        (LanguageDetector)
          MethodUtils.invokeStaticMethod(Class.forName(fullClassName), "getInstance", null);
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
      logger.error(e.getMessage());
      throw new IllegalArgumentException(msg + "; error: " + e.getMessage());
    }
  }

  /**
   * Returns all available detector names
   *
   * @see related comment in ToEnglishTranslator::getSimpleClassNames
   */
  public static Set<String> getSimpleClassNames()
  {
    if (simpleClassNames == null)
    {
      Set<String> classNames = new TreeSet<String>();
      Set<Class<? extends LanguageDetector>> classes = 
        (new Reflections("hoot.services.language")).getSubTypesOf(LanguageDetector.class);
      for (Class<? extends LanguageDetector> clazz : classes)
      {
        classNames.add(clazz.getSimpleName());
      }
      simpleClassNames = classNames;
    }    
    return simpleClassNames;
  }
}
