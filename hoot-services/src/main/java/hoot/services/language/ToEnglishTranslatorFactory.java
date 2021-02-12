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
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */

package hoot.services.language;

import java.util.Set;
import java.util.TreeSet;

import org.apache.commons.lang3.reflect.MethodUtils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.reflections.Reflections;

/**
 * Creates language translators
 */
public class ToEnglishTranslatorFactory
{
  private static final Logger logger = LoggerFactory.getLogger(ToEnglishTranslatorFactory.class);

  private static Set<String> simpleClassNames = null;

  private ToEnglishTranslatorFactory()
  {
  }

  /**
   * Creates a language translator
   *
   * @param className the simple class name of the translator
   * @return a language translator
   */
  public static ToEnglishTranslator create(String className) throws Exception
  {
    ToEnglishTranslator translator = null;
    String fullClassName = null;
    try 
    {
      fullClassName = LanguageUtils.getFullClassName(className);
      logger.trace("fullClassName: " + fullClassName);
      if (MethodUtils.getAccessibleMethod(Class.forName(fullClassName), "getInstance", null) != null)
      {
        translator = 
          (ToEnglishTranslator)
            MethodUtils.invokeStaticMethod(Class.forName(fullClassName), "getInstance", null);
      }
      else
      {
        translator = (ToEnglishTranslator)Class.forName(fullClassName).newInstance();
      }
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
      throw new IllegalArgumentException(msg + "; error: " + e.getMessage());
    }
    return translator;
  }

  /**
   * Returns all available detector names
   */
  public static Set<String> getSimpleClassNames()
  {
    //really wanted to make this a generic method in ReflectUtils, just haven't quite figured out 
    //how yet; see the source history of ReflectUtils for a not quite working example

    if (simpleClassNames == null)
    {
      Set<String> classNames = new TreeSet<String>();
      Set<Class<? extends ToEnglishTranslator>> classes = 
        (new Reflections("hoot.services.language")).getSubTypesOf(ToEnglishTranslator.class);
      for (Class<? extends ToEnglishTranslator> clazz : classes)
      {
        classNames.add(clazz.getSimpleName());
      }
      simpleClassNames = classNames;
    }
    return simpleClassNames;
  }
}
