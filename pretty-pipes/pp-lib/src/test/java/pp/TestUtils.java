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
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;

import pp.Utils;

public class TestUtils
{
  public final static String TEST_RESOURCES = "src/test/resources/";
  
  public static String readPath(Path p) throws FileNotFoundException, IOException
  {
    FileSystem fs = p.getFileSystem(new org.apache.hadoop.conf.Configuration());
    FSDataInputStream fdis = fs.open(p);
    FileStatus stat = fs.getFileStatus(p);
    
    byte[] baselineBuffer = new byte[(int)stat.getLen()];
    fdis.read(baselineBuffer);
    return new String(baselineBuffer);
  }
  
  public static String readFile(File f) throws FileNotFoundException, IOException
  {
    byte[] baselineBuffer = new byte[(int)f.length()];
    new FileInputStream(f).read(baselineBuffer);
    return new String(baselineBuffer);
  }
  
  public static String calculateHash(File f) throws IOException
  {
    return calculateHash(readFile(f));
  }

  public static String calculateHash(String s) throws IOException
  {
    MessageDigest digest;
    try
    {
      digest = MessageDigest.getInstance("MD5");
    }
    catch (NoSuchAlgorithmException e)
    {
      e.printStackTrace();
      assert (false);
      return "error";
    }

    digest.update(s.getBytes(), 0, s.getBytes().length);
    return new BigInteger(1, digest.digest()).toString(16);
  }

  public static String calculateHash(Path p) throws IOException
  {
    MessageDigest digest;
    try
    {
      digest = MessageDigest.getInstance("MD5");
    }
    catch (NoSuchAlgorithmException e)
    {
      e.printStackTrace();
      assert (false);
      return "error";
    }

    FileSystem fs = Utils.getFileSystem(p);
    byte[] buffer = new byte[(int) fs.getFileStatus(p).getLen()];

    for (int i = 0; i < buffer.length; i++)
    {
      buffer[i] = 0;
    }

    digest.update(buffer, 0, buffer.length);
    fs.open(p).read(buffer);
    return new BigInteger(1, digest.digest()).toString(16);
  }
  
  public static String composeInputDir(Object o) throws IOException
  {
    return composeInputDir(o.getClass());
  }

  public static String composeInputDir(Class<?> c) throws IOException
  {
    String pn = packageName(c);
    String cn = c.getName().replace(pn + ".", "");
    String result = TEST_RESOURCES + pn + "/" + cn + "/";
    return result;
  }

  public static String composeOutputDir(Object o) throws IOException
  {
    return composeOutputDir(o.getClass());
  }

  public static String composeOutputDir(Class<?> c) throws IOException
  {
    String pn = packageName(c);
    String cn = c.getName().replace(pn + ".", "");
    String result = TEST_RESOURCES + pn + "/" + cn + "/";
    File dir = new File(result);
    if (dir.exists())
    {
      Utils.deleteDir(dir);
    }

    if (dir.mkdirs() == false)
    {
      throw new IOException("Error creating test output directory (" + result + ")");
    }

    return result;
  }

  public static Path composeOutputHdfs(Object o) throws IOException
  {
    return composeOutputHdfs(o.getClass());
  }

  public static Path composeOutputHdfs(Class<?> c) throws IOException
  {
    String pn = packageName(c);
    String cn = c.getName().replace(pn + ".", "");
    Path result = new Path(TEST_RESOURCES + "/" + pn + "/" + cn + "/");
    FileSystem fs = Utils.getFileSystem(result);
    fs.delete(result, true);
    if (fs.mkdirs(result) == false)
    {
      throw new IOException("Error creating test output directory (" + result + ")");
    }
    return result;
  }

  public static Path composeInputHdfs(Object o) throws IOException
  {
    return composeInputHdfs(o, -1);
  }

  /**
   * Creates an input directory for placing test inputs the first time it is
   * called. The input directory will automatically get a subdirectory that
   * contains the version number. If the directory already exists and contains
   * the correct version number then nothing is done the subsequent times
   * through. If the version doesn't match the old directory is removed and a
   * new one is created.
   * 
   * @param o
   * @param version
   *          Version of the input files. This should be increment each time an
   *          input changes.
   * @return
   * @throws IOException
   */
  public static Path composeInputHdfs(Object o, int version) throws IOException
  {
    Class<?> c = o.getClass();
    String pn = packageName(c);
    String cn = c.getName().replace(pn + ".", "");
    Path result = new Path(TEST_RESOURCES + "/" + pn + "/" + cn + "/");
    Path versionPath = new Path(result, Integer.toString(version));
    FileSystem fs = Utils.getFileSystem(result);

    // if this isn't the right version
    if (fs.exists(versionPath) == false || version == -1)
    {
      // delete the old directory and create a new one.
      fs.delete(result, true);
      if (fs.mkdirs(versionPath) == false)
      {
        throw new IOException("Error creating test input directory (" + versionPath + ")");
      }
    }
    return result;
  }

  public static Path composeInputHdfs(Class<?> c) throws IOException
  {
    String pn = packageName(c);
    String cn = c.getName().replace(pn + ".", "");
    Path result = new Path(TEST_RESOURCES + "/" + pn + "/" + cn + "/");
    FileSystem fs = Utils.getFileSystem(result);
    Utils.getFileSystem(result).delete(result, true);
    if (fs.mkdirs(result) == false)
    {
      throw new IOException("Error creating test output directory (" + result + ")");
    }
    return result;
  }

  public static String packageName(Object o)
  {
    return packageName(o.getClass());
  }

  public static String packageName(Class<?> c)
  {
    String name = c.getName();
    int li = name.lastIndexOf(".");
    return name.substring(0, li);
  }
}
