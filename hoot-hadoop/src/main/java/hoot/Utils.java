package hoot;

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
