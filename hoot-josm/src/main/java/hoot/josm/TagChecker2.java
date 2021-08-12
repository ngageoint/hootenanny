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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
package hoot.josm;

import java.io.IOException;
import java.lang.reflect.Method;
import java.lang.reflect.NoSuchMethodException;

import org.openstreetmap.josm.data.validation.tests.TagChecker;

public class TagChecker2 extends org.openstreetmap.josm.data.validation.tests.TagChecker
{
  @Override
  public void initialize() throws IOException, NoSuchMethodException
  {
    try
    {
      Method method = TagChecker.class.getDeclaredMethod("initializeData", null);
      method.setAccessible(true);
      method.invoke(null, null);
      method = TagChecker.class.getDeclaredMethod("initializePresets", null);
      method.setAccessible(true);
      method.invoke(null, null);
      method = TagChecker.class.getDeclaredMethod("analysePresets", null);
      method.setAccessible(true);
      method.invoke(null, null);
    }
    catch (Exception e)
    {
      System.out.println(e.getMessage());
      throw e;
    }
  }
}
