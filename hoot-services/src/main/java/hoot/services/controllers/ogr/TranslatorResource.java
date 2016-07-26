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
package hoot.services.controllers.ogr;

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

import hoot.services.nodejs.ServerControllerBase;


@Path("")
public class TranslatorResource extends ServerControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(TranslatorResource.class);

    private static final Object procLock = new Object();
    private static final Object portLock = new Object();

    private static String currentPort;
    private static Process transProc;

    public TranslatorResource() {
    }

    public void startTranslationService() {
        // set default default port and threadcount
        try {
            // Make sure to wipe out previosuly running servers.
            stopServer(HOME_FOLDER + "/scripts/" + TRANSLATION_SERVER_SCRIPT);

            // Probably an overkill but just in-case using synch lock
            String currPort = TRANSLATION_SERVER_PORT;
            synchronized (portLock) {
                currentPort = currPort;
            }

            synchronized (procLock) {
                String currThreadCnt = TRANSLATION_SERVER_THREAD_COUNT;
                transProc = startServer(currPort, currThreadCnt, HOME_FOLDER + "/scripts/" + TRANSLATION_SERVER_SCRIPT);
            }
        }
        catch (Exception ex) {
            String msg = "Error starting translation service request: " + ex;
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }
    }

    public static void stopTranslationService() {
        // This also gets called automatically from HootServletContext when
        // service exits but should not be reliable since there are many path where it will not be invoked.
        try {
            // Destroy the reference to the process directly here via the Java
            // API vs having the base class kill it with a unix command. Killing it via command causes
            // the stxxl temp files created by hoot threads not to be cleaned up.
            // stopServer(homeFolder + "/scripts/" + translationServerScript);
            transProc.destroy();
        }
        catch (Exception ex) {
            String msg = "Error starting translation service request: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }
    }

    /**
     * Gets current status of translation server.
     * <p>
     * GET hoot-services/ogr/translationserver/status
     *
     * @return JSON containing state and port it is running#background=Bing&map=17.20/-105.00217/39.91295
     */
    @GET
    @Path("/translationserver/status")
    @Produces(MediaType.APPLICATION_JSON)
    public Response isTranslationServiceRunning() {
        boolean isRunning;
        try {
            isRunning = getStatus(transProc);
        }
        catch (Exception ex) {
            String msg = "Error starting translation service request: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        JSONObject res = new JSONObject();
        res.put("isRunning", isRunning);
        res.put("port", currentPort);

        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }
}
