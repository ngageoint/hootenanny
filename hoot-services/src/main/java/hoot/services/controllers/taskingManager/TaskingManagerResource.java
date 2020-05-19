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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.taskingManager;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import hoot.services.controllers.grail.GrailResource;
import hoot.services.controllers.grail.PullApiCommand;
import hoot.services.models.db.Users;

import static hoot.services.HootProperties.PRIVATE_OVERPASS_CERT_PATH;
import static hoot.services.HootProperties.replaceSensitiveData;
import static hoot.services.HootProperties.TASKING_MANAGER_URL;


@Controller
@Path("")
@Transactional
public class TaskingManagerResource {

    public TaskingManagerResource() {}

    /**
     * Get all the tasks currently in tasking manager
     *
     * GET hoot-services/taskingmanager/getprojects
     *
     * @param request
     * @return
     * @throws Exception
     */
    @GET
    @Path("/getprojects")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getTasks(@Context HttpServletRequest request) throws Exception {
        Users user = Users.fromRequest(request);

        GrailResource.advancedUserCheck(user);

        String getTasksUrl = replaceSensitiveData(TASKING_MANAGER_URL) + "projects.json";
        InputStream inputStream = executeTaskingManagerRequest(getTasksUrl);

        JSONParser jsonParser = new JSONParser();
        JSONObject oParsed = (JSONObject) jsonParser.parse(new InputStreamReader(inputStream));

        return Response.ok(oParsed.toJSONString()).build();
    }

    /**
     * Get all the tasks currently in tasking manager for the specified project
     *
     * GET hoot-services/taskingmanager/gettasks
     *
     * @param request
     * @param projectId id of project which we are getting tasks for
     * @return
     * @throws Exception
     */
    @GET
    @Path("/gettasks/{projectId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getTasks(@Context HttpServletRequest request,
            @PathParam("projectId") long projectId) throws Exception {
        Users user = Users.fromRequest(request);

        GrailResource.advancedUserCheck(user);

        String getTasksUrl = replaceSensitiveData(TASKING_MANAGER_URL) + "project/" + projectId + "/tasks.json";
        InputStream inputStream = executeTaskingManagerRequest(getTasksUrl);

        JSONParser jsonParser = new JSONParser();
        JSONObject oParsed = (JSONObject) jsonParser.parse(new InputStreamReader(inputStream));

        return Response.ok(oParsed.toJSONString()).build();
    }

    /**
     * Execute the request to tasking manager. Will use cert is one is specified, otherwise will run url request.
     *
     * @param urlString tasking manager url request string
     * @return InputStream result of the url call to tasking manager
     */
    private InputStream executeTaskingManagerRequest(String urlString) {
        InputStream inputStream;

        try {
            if (certExists()) {
                inputStream = PullApiCommand.getHttpResponseWithSSL(urlString);
            } else {
                URL url = new URL(urlString);
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                inputStream = conn.getInputStream();
            }
        }
        catch (IOException exc) {
            String msg = "Failure to pull data from tasking manager." + exc.getMessage();
            throw new WebApplicationException(exc, Response.serverError().entity(msg).build());
        }
        catch (Exception exc) {
            String msg = "Failure to pull data from tasking manager with the specified cert." + exc.getMessage();
            throw new WebApplicationException(exc, Response.serverError().entity(msg).build());
        }

        return inputStream;
    }

    /**
     * Make sure there is a cert specified in db.properties
     */
    public boolean certExists() {
        return !replaceSensitiveData(PRIVATE_OVERPASS_CERT_PATH).equals(PRIVATE_OVERPASS_CERT_PATH);
    }

}
