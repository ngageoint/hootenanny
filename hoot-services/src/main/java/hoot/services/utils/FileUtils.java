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

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class FileUtils
{
  /**
   * Removes the first line from a file
   *
   * @param file file to modify
   * @throws IOException
   */
  public static void removeFirstLineFromFile(File file) throws IOException
  {
    Scanner fileScanner = new Scanner(file);
    fileScanner.nextLine();
    FileWriter fileStream = new FileWriter(file.getAbsolutePath());
    BufferedWriter out = new BufferedWriter(fileStream);
    while (fileScanner.hasNextLine())
    {
      String next = fileScanner.nextLine();
      if (next.equals("\n"))
      {
        out.newLine();
      }
      else out.write(next);
      {
        out.newLine();
      }
    }
    out.close();
    fileStream.close();
  }
}
