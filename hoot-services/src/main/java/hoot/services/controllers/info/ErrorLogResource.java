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
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.info.ErrorLog;
import hoot.services.utils.ResourceErrorHandler;


@Path("/logging")
public class ErrorLogResource {
    private static final Logger logger = LoggerFactory.getLogger(ErrorLogResource.class);

    private String exportLogPath;

    public ErrorLogResource() {

    }

    @PreDestroy
    public void PreDestroy() {
        try {
            if ((exportLogPath != null) && (!exportLogPath.isEmpty())) {
                FileUtils.forceDelete(new File(exportLogPath));
            }
        }
        catch (Exception ex) {
            logger.error(ex.getMessage());
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
    @Produces(MediaType.TEXT_PLAIN)
    public Response getDebugLog() {
        String logStr = null;
        try {
            // 50k Length
            logStr = ErrorLog.getErrorlog(50000);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting error logger: " + ex, Status.INTERNAL_SERVER_ERROR, logger);
        }
        JSONObject res = new JSONObject();
        res.put("logger", logStr);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    /**
     * Service method endpoint for exporting logging information.
     * 
     * GET hoot-services/info/logging/export
     * 
     * @return Binary octet stream
     * @throws IOException
     */
    @GET
    @Path("/export")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportLog() throws IOException {
        File out = null;
        try {
            String outputPath = ErrorLog.generateExportLog();
            out = new File(outputPath);
            exportLogPath = outputPath;
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error exporting logger file: " + ex, Status.INTERNAL_SERVER_ERROR, logger);
        }
        DateFormat dateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
        Date dd = new Date();
        String dtStr = dateFormat.format(dd);
        ResponseBuilder rBuild = Response.ok(out, MediaType.APPLICATION_OCTET_STREAM);
        rBuild.header("Content-Disposition", "attachment; filename=hootlog_" + dtStr + ".logger");

        return rBuild.build();
    }
}
