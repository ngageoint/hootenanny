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

import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;

public class Utils
{
  static org.apache.hadoop.conf.Configuration _c = null;
  
  public static boolean deleteDir(File dir)
  {
    if (dir.isDirectory())
    {
      String[] children = dir.list();
      for (int i = 0; i < children.length; i++)
      {
        boolean success = deleteDir(new File(dir, children[i]));
        if (!success)
        {
          return false;
        }
      }
    }

    // The directory is now empty so delete it
    return dir.delete();
  }
  
  public static org.apache.hadoop.conf.Configuration getConfiguration()
  {
    if (_c == null)
    {
      _c = new org.apache.hadoop.conf.Configuration();
    }
    return _c;
  }
  
  public static FileSystem getFileSystem() throws IOException
  {
    return getFileSystem(new Path(getConfiguration().get("fs.default.name")));
  }

  public static FileSystem getFileSystem(Path p) throws IOException
  {
    return p.getFileSystem(getConfiguration());
  }
}
