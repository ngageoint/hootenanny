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
package hoot.services.controllers.services;

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


public class P2PResource extends ServerControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(P2PResource.class);
    private static final Object procLock = new Object();
    private static final Object portLock = new Object();

    private static String currentPort;
    private static Process _P2PProc;

    public P2PResource() {
    }

    public void startP2PService() {
        // set default default port and threadcount
        try {
            // Make sure to wipe out previosuly running servers.
            stopServer(HOME_FOLDER + "/scripts/" + P_2_P_SERVER_SCRIPT);

            // Probably an overkill but just in-case using synch lock
            String currPort = P_2_P_SERVER_PORT;
            synchronized (portLock) {
                currentPort = currPort;
            }

            synchronized (procLock) {
                String currThreadCnt = P_2_P_SERVER_THREAD_COUNT;
                _P2PProc = startServer(currPort, currThreadCnt, HOME_FOLDER + "/scripts/" + P_2_P_SERVER_SCRIPT);
            }
        }
        catch (Exception ex) {
            String msg = "Error starting P2P service request: " + ex;
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }
    }

    /**
     * Destroys all POI to POI server process where it effectively shutting them
     * down.
     * 
     * GET hoot-services/services/p2pserver/stop
     * 
     * @return JSON containing state
     */
    @GET
    @Path("/p2pserver/stop")
    @Produces(MediaType.APPLICATION_JSON)
    public Response stopP2PService() {
        // This also gets called automatically from HootServletContext when
        // service exits but should not be reliable since there are many path where it will not be invoked.
        try {
            // Destroy the reference to the process directly here via the Java
            // API vs having the base class kill it with a unix command. Killing it via command causes
            // the stxxl temp files created hoot threads not to be cleaned up.
            _P2PProc.destroy();
        }
        catch (Exception ex) {
            String msg = "Error starting P2P service request: " + ex;
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        JSONObject res = new JSONObject();
        res.put("isRunning", "false");

        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    /**
     * Gets current status of P2P server.
     * 
     * GET hoot-services/services/p2pserver/status
     * 
     * @return JSON containing state and port it is running
     */
    @GET
    @Path("/p2pserver/status")
    @Produces(MediaType.APPLICATION_JSON)
    public Response isP2PServiceRunning() {
        boolean isRunning;

        try {
            isRunning = getStatus(_P2PProc);
        }
        catch (Exception ex) {
            String msg = "Error starting P2P service request: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        JSONObject res = new JSONObject();
        res.put("isRunning", isRunning);
        res.put("port", currentPort);

        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }
}
