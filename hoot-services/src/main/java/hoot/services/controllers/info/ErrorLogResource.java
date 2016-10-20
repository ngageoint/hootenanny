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
package hoot.services.controllers.info;

import static hoot.services.HootProperties.ERROR_LOG_PATH;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.UUID;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;


@Controller
@Path("/logging")
public class ErrorLogResource {
    private static final Logger logger = LoggerFactory.getLogger(ErrorLogResource.class);

    @Autowired
    private AboutResource aboutResource;


    public ErrorLogResource() {}


    /**
     * Service method endpoint for retrieving the Hootenanny tomcat logger.
     * 
     * GET hoot-services/info/logging/debuglog
     * 
     * @return JSON containing debug logger
     */
    @GET
    @Path("/debuglog")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getDebugLog() {
        String errorLog;
        try {
            // 50k Length
            errorLog = getErrorLog(50000);
        }
        catch (Exception e) {
            String msg = "Error getting error log: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject entity = new JSONObject();
        entity.put("logger", errorLog);

        return Response.ok(entity.toJSONString()).build();
    }

    /**
     * Service method endpoint for exporting logging information.
     * 
     * GET hoot-services/info/logging/export
     * 
     * @return Binary octet stream
     */
    @GET
    @Path("/export")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportLog() {
        File out;

        try {
            String outputPath = generateExportLog();
            out = new File(outputPath);
        }
        catch (Exception e) {
            String message = "Error exporting log file!  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(message).build());
        }

        DateFormat dateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
        Date date = new Date();
        String dtStr = dateFormat.format(date);

        ResponseBuilder responseBuilder = Response.ok(out);
        responseBuilder.header("Content-Disposition", "attachment; filename=hootlog_" + dtStr + ".logger");

        return responseBuilder.build();
    }

    private static String getErrorLog(int maxLength) throws IOException {
        try (RandomAccessFile file = new RandomAccessFile(new File(ERROR_LOG_PATH), "r")) {
            long fileLength = file.length();

            long startOffset = 0;
            if (fileLength > maxLength) {
                startOffset = fileLength - maxLength;
            }

            file.seek(startOffset);

            byte[] buffer = new byte[maxLength];
            file.read(buffer, 0, maxLength);

            return new String(buffer);
        }
    }

    private String generateExportLog() throws IOException {
        UUID uuid = UUID.randomUUID();

        VersionInfo versionInfo = this.aboutResource.getCoreVersionInfo();
        String data = System.lineSeparator() + "************ CORE VERSION INFO ***********" + System.lineSeparator();
        data += versionInfo.toString();

        CoreDetail coreDetail = this.aboutResource.getCoreVersionDetail();
        data += System.lineSeparator() + "************ CORE ENVIRONMENT ***********" + System.lineSeparator();

        if (coreDetail != null) {
            data += StringUtils.join(coreDetail.getEnvironmentInfo(), System.lineSeparator());
        }

        data += System.lineSeparator() + "************ SERVICE VERSION INFO ***********" + System.lineSeparator();
        data += this.aboutResource.getServicesVersionInfo().toString();
        data += System.lineSeparator() + "************ CATALINA LOG ***********" + System.lineSeparator();

        // 5MB Max
        int maxSize = 5000000;

        String logStr = getErrorLog(maxSize);

        String outputPath = TEMP_OUTPUT_PATH + File.separator + uuid;
        try (RandomAccessFile raf = new RandomAccessFile(outputPath, "rw")) {
            raf.writeBytes(data + System.lineSeparator() + logStr);
            return outputPath;
        }
    }
}
