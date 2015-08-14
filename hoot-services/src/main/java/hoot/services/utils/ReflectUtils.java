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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;

import java.beans.BeanInfo;
import java.beans.IntrospectionException;
import java.beans.Introspector;
import java.beans.PropertyDescriptor;
import java.lang.reflect.Method;

/**
 * Various utilities related to reflection
 */
public class ReflectUtils
{
  /**
   * Returns the name of the class that calls a method
   * 
   * @return class name string
   */
  public static String getCallingClassName()
  {
    final StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
    final StackTraceElement stackTraceElement = stackTrace[stackTrace.length - 2];
    if (stackTraceElement != null)
    {
      return stackTraceElement.getClassName();
    }
    return null;
  }
  
  /**
   * Returns a printable string for a POJO
   * 
   * @param bean a POJO
   * @param baseClass highest level base class for which to add property values to the output 
   * string
   * @return a printable string with the object's data 
   * @throws IntrospectionException
   * @todo update this to call toString() on objects, if they have implementations of it; i.e.
   * make this handle objects recursively
   */
  public static String toString(final Object bean, @SuppressWarnings("rawtypes") 
    final Class baseClass) throws IntrospectionException
  {
    BeanInfo info = null;
    String str = "";
    info = Introspector.getBeanInfo(bean.getClass(), baseClass);
    for (PropertyDescriptor prop : info.getPropertyDescriptors())
    {
      try
      {
        Method getter = prop.getReadMethod();
        Object propVal = getter.invoke(bean);
        String propValStr = null;
        if (propVal != null)
        {
          propValStr = String.valueOf(propVal);
        }
        str += 
          "  " + prop.getName() + 
          //" (" + prop.getPropertyType().getName() + ")" + 
          ": " + 
          "\"" + propValStr + "\"" + "\n";
      }
      catch (Exception e)
      {
        str += "{Error reading property: " + e.toString() + "}\n";
      }
    }
    return str;
  }
}
