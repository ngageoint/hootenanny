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

public class ErrorLog {

    private static String errLogPath = null;
    private static String tempOutputPath = null;

    public ErrorLog() throws IOException {
        // ErrorLogPath
        if (errLogPath == null) {
            errLogPath = HootProperties.getProperty("ErrorLogPath");
        }

        if (tempOutputPath == null) {
            tempOutputPath = HootProperties.getProperty("tempOutputPath");
        }
    }

    public String getErrorlog(long maxLength) throws IOException {
        File file = new File(errLogPath);

        try (RandomAccessFile randomAccessFile = new RandomAccessFile(file, "r")){
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

    public String generateExportLog() throws IOException {
        String fileId = UUID.randomUUID().toString();
        String outputPath = tempOutputPath + File.pathSeparator + fileId;

        String data = "";

        AboutResource about = new AboutResource();

        VersionInfo vInfo = about.getCoreVersionInfo();
        data = System.lineSeparator() + "************ CORE VERSION INFO ***********" + System.lineSeparator();
        data += vInfo.toString();

        CoreDetail cd = about.getCoreVersionDetail();
        data += System.lineSeparator() + "************ CORE ENVIRONMENT ***********" + System.lineSeparator();
        if (cd != null) {
            data += StringUtils.join(cd.getEnvironmentInfo(), System.lineSeparator());
        }

        data = System.lineSeparator() + "************ SERVICE VERSION INFO ***********" + System.lineSeparator();
        data += about.getServicesVersionInfo().toString();

        ServicesDetail sd = about.getServicesVersionDetail();
        if (sd != null) {
            data += System.lineSeparator() + "************ SERVICE DETAIL PROPERTY ***********" + System.lineSeparator();
            for (ServicesDetail.Property prop : sd.getProperties()) {
                String str = prop.getName() + " : " + prop.getValue() + System.lineSeparator();
                data += str;
            }

            data += System.lineSeparator() + "************ SERVICE DETAIL RESOURCE ***********" + System.lineSeparator();
            for (ServicesDetail.ServicesResource res : sd.getResources()) {
                String str = res.getType() + " : " + res.getUrl() + System.lineSeparator();
                data += str;
            }
        }

        data += System.lineSeparator() + "************ CATALINA LOG ***********" + System.lineSeparator();

        // 5MB Max
        int maxSize = 5000000;

        String logStr = getErrorlog(maxSize);

        try (RandomAccessFile raf = new RandomAccessFile(outputPath, "rw")){
            raf.writeBytes(data + System.lineSeparator() + logStr);
            return outputPath;
        }
    }
}
