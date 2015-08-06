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
package pp;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Field;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;


public class HootHadoop
{
  private static boolean _initialized = false;
  
  static public String join(List<String> list, String conjunction)
  {
     StringBuilder sb = new StringBuilder();
     boolean first = true;
     for (String item : list)
     {
        if (first)
           first = false;
        else
           sb.append(conjunction);
        sb.append(item);
     }
     return sb.toString();
  }
  
  public static void addDir(String s) throws IOException {
    try {
      // This enables the java.library.path to be modified at runtime
      // From a Sun engineer at http://forums.sun.com/thread.jspa?threadID=707176
      //
      Field field = ClassLoader.class.getDeclaredField("usr_paths");
      field.setAccessible(true);
      String[] paths = (String[])field.get(null);
      for (int i = 0; i < paths.length; i++) {
        if (s.equals(paths[i])) {
          return;
        }
      }
      String[] tmp = new String[paths.length+1];
      System.arraycopy(paths,0,tmp,0,paths.length);
      tmp[paths.length] = s;
      field.set(null,tmp);
      System.setProperty("java.library.path", System.getProperty("java.library.path") + File.pathSeparator + s);
    } catch (IllegalAccessException e) {
      throw new IOException("Failed to get permissions to set library path");
    } catch (NoSuchFieldException e) {
      throw new IOException("Failed to get field handle to set library path");
    }
  }

  
  public static void init()
  {
    if (_initialized == false)
    {
      String tryS = "";
      try
      {
        String ldLib = System.getenv("LD_LIBRARY_PATH");
        String[] ldLibArr = ldLib.split(":");
        String[] javaPathArr = System.getProperty("java.library.path").split(":");
        LinkedList<String> allLib = new LinkedList<String>();
        allLib.addAll(Arrays.asList(javaPathArr));
        allLib.addAll(Arrays.asList(ldLibArr));
        
        for (String s : allLib)
        {
          tryS = s;
          addDir(s);
        }
        
        System.loadLibrary("PrettyPipes");
        _initialized = true;
      }
      catch (UnsatisfiedLinkError e)
      {
        
        System.out.flush();
        System.err
          .println("Error linking with PrettyPipes library. Is libPrettyPipes.so in your LD_LIBRARY_PATH?");
        System.err.println("LD_LIBRARY_PATH: " + System.getenv("LD_LIBRARY_PATH"));
        System.err.println("java.library.path: " + System.getProperty("java.library.path"));
        System.err.flush();
        throw e;
      }
      catch (IOException e)
      {
        System.out.flush();
        System.err
          .println("Error adding dir to path. " + tryS);
        System.err.println("LD_LIBRARY_PATH: " + System.getenv("LD_LIBRARY_PATH"));
        System.err.println("java.library.path: " + System.getProperty("java.library.path"));
        System.err.flush();
        e.printStackTrace();
      }
    }
  }
}
