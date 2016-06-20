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
package hoot.services.info;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.UUID;

import org.apache.commons.lang3.StringUtils;

import hoot.services.HootProperties;
import hoot.services.controllers.info.AboutResource;


public final class ErrorLog {

    private static final String errLogPath = HootProperties.getProperty("ErrorLogPath");
    private static final String tempOutputPath = HootProperties.getProperty("tempOutputPath");

    private ErrorLog() {
    }

    public static String getErrorlog(long maxLength) throws IOException {
        File file = new File(errLogPath);

        try (RandomAccessFile randomAccessFile = new RandomAccessFile(file, "r")) {
            StringBuilder builder = new StringBuilder();
            long length = file.length();

            long startOffset = 0;
            if (length > maxLength) {
                startOffset = length - maxLength;
            }
            for (long seek = startOffset; seek < length; seek++) {
                randomAccessFile.seek(seek);
                char c = (char) randomAccessFile.read();
                builder.append(c);
            }
            return builder.toString();
        }
    }

    public static String generateExportLog() throws IOException {
        String fileId = UUID.randomUUID().toString();
        String outputPath = tempOutputPath + File.separator + fileId;

        AboutResource about = new AboutResource();

        VersionInfo vInfo = about.getCoreVersionInfo();
        String data = System.lineSeparator() + "************ CORE VERSION INFO ***********" + System.lineSeparator();
        data += vInfo.toString();

        CoreDetail cd = about.getCoreVersionDetail();
        data += System.lineSeparator() + "************ CORE ENVIRONMENT ***********" + System.lineSeparator();
        if (cd != null) {
            data += StringUtils.join(cd.getEnvironmentInfo(), System.lineSeparator());
        }

        data += System.lineSeparator() + "************ SERVICE VERSION INFO ***********" + System.lineSeparator();
        data += about.getServicesVersionInfo().toString();
        data += System.lineSeparator() + "************ CATALINA LOG ***********" + System.lineSeparator();

        // 5MB Max
        int maxSize = 5000000;

        String logStr = getErrorlog(maxSize);

        try (RandomAccessFile raf = new RandomAccessFile(outputPath, "rw")) {
            raf.writeBytes(data + System.lineSeparator() + logStr);
            return outputPath;
        }
    }
}
