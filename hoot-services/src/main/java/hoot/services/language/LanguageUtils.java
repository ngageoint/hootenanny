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
