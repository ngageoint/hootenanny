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

import java.io.File;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.annotation.PreDestroy;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


@Path("/logging")
public class ErrorLogResource {
    private static final Logger logger = LoggerFactory.getLogger(ErrorLogResource.class);

    private String exportLogPath;

    public ErrorLogResource() {
    }

    @PreDestroy
    public void preDestroy() throws IOException {
        if ((exportLogPath != null) && (!exportLogPath.isEmpty())) {
            FileUtils.forceDelete(new File(exportLogPath));
        }
    }

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
            errorLog = ErrorLog.getErrorlog(50000);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error getting error log: " + ex;
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
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
            String outputPath = ErrorLog.generateExportLog();
            out = new File(outputPath);
            // TODO: Really not sure about the line below.  Will the be always a single export?  Probably not.
            exportLogPath = outputPath;
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String message = "Error exporting log file!";
            throw new WebApplicationException(ex, Response.serverError().entity(message).build());
        }

        DateFormat dateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
        Date date = new Date();
        String dtStr = dateFormat.format(date);

        ResponseBuilder responseBuilder = Response.ok(out);
        responseBuilder.header("Content-Disposition", "attachment; filename=hootlog_" + dtStr + ".logger");

        return responseBuilder.build();
    }
}
