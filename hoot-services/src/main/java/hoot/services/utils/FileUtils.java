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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;

import java.io.File;
import java.util.List;

import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.filefilter.DirectoryFileFilter;
import org.apache.commons.io.filefilter.NotFileFilter;
import org.apache.commons.io.filefilter.TrueFileFilter;


public final class FileUtils {

    private FileUtils() {
    }

    public static File getSubFolderFromFolder(String targetFolder, String subFolderName) {
        File ret = null;

        File folder = new File(targetFolder);
        List<File> files = (List<File>) org.apache.commons.io.FileUtils.listFilesAndDirs(folder,
                new NotFileFilter(TrueFileFilter.INSTANCE), DirectoryFileFilter.DIRECTORY);

        for (File file : files) {
            if (file.getName().equals(subFolderName)) {
                ret = file;
                break;
            }
        }

        return ret;
    }

    public static File getFileFromFolder(String targetFolder, String fileName, String ext) {
        File ret = null;
        String[] extension = { ext };
        File dir = new File(targetFolder);
        if (dir.isDirectory()) {
            List<File> files = (List<File>) org.apache.commons.io.FileUtils.listFiles(dir, extension, false);
            for (File file : files) {
                if (file.getName().equals(fileName + "." + ext)) {
                    ret = file;
                    break;
                }
            }
        }

        return ret;
    }

    public static boolean validateFilePath(String expectedPath, String actualPath) {
        String path = FilenameUtils.getFullPathNoEndSeparator(actualPath);
        return expectedPath.equals(path);
    }
}
