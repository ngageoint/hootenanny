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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.nodejs;

import static hoot.services.HootProperties.*;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;


@Controller
public class ElementMergeServiceResource extends NodejsService {
    private static final Logger logger = LoggerFactory.getLogger(ElementMergeServiceResource.class);

    // The purpose of this service is to provide the hoot-ui fast way to merge POI to POI.
    private static Process elementMergeServiceProcess;

    public ElementMergeServiceResource() {}

    /**
     * Destroys all Element Merge Server processes, where it effectively is shutting them down.
     *
     * GET hoot-services/services/elementmergeserver/stop
     *
     * @return JSON containing state
     */
    @GET
    @Path("/elementmergeserver/stop")
    @Produces(MediaType.APPLICATION_JSON)
    public Response stopElementMergeServiceViaWeb() {
        // This also gets called automatically from HootServletContext when
        // service exits but should not be reliable since there are many path where it will not be invoked.
        try {
            // Destroy the reference to the process directly here via the Java
            // API vs having the base class kill it with a unix command. Killing it via command causes
            // the stxxl temp files created hoot threads not to be cleaned up.
            stopElementMergeService();
        }
        catch (Exception e) {
            String msg = "Error stopping Element Merge Service: " + e;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject json = new JSONObject();
        json.put("isRunning", "false");

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Gets current status of Element Merge Server.
     *
     * GET hoot-services/services/elementmergeserver/status
     *
     * @return JSON containing state and port it is running
     */
    @GET
    @Path("/elementmergeserver/status")
    @Produces(MediaType.APPLICATION_JSON)
    public Response isElementMergeServiceRunning() {
        boolean isRunning;

        try {
            isRunning = getStatus(elementMergeServiceProcess);
        }
        catch (Exception e) {
            String msg = "Error getting status of Element Merge Service.  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject json = new JSONObject();
        json.put("isRunning", isRunning);
        json.put("port", ELEMENT_MERGE_SERVER_PORT);

        return Response.ok(json.toJSONString()).build();
    }

    public static void startElementMergeService() {
        try {
            String elementMergeServiceScript = HOME_FOLDER + ELEMENT_MERGE_SERVER_SCRIPT;

            // Make sure to wipe out previosuly running servers.
            stopServer(elementMergeServiceScript);

            logger.info("Starting POI-To-POI Merge Service by running {} script", elementMergeServiceScript);

            elementMergeServiceProcess = startServer(ELEMENT_MERGE_SERVER_PORT, ELEMENT_MERGE_SERVER_THREAD_COUNT, elementMergeServiceScript);

            logger.info("Element Merge Service started");
        }
        catch (Exception e) {
            String msg = "Error starting Element Merge Service.  Cause: " + e.getMessage();
            throw new RuntimeException(msg, e);
        }
    }

    public static void stopElementMergeService() {
        // This also gets called automatically from HootServletContext when
        // service exits but should not be reliable since there are many path where it will not be invoked.
        try {
            // Destroy the reference to the process directly here via the Java
            // API vs having the base class kill it with a unix command. Killing it via command causes
            // the stxxl temp files created by hoot threads not to be cleaned up.
            // stopServer(homeFolder + "/scripts/" + translationServerScript);
            elementMergeServiceProcess.destroy();
        }
        catch (Exception e) {
            String msg = "Error stopping Element Merge Service.  Cause: " + e.getMessage();
            throw new RuntimeException(msg, e);
        }
    }
}
