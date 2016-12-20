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
package hoot.services.controllers.job;

import static hoot.services.HootProperties.*;

import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.dom.DOMSource;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.models.osm.User;
import hoot.services.utils.DbUtils;
import hoot.services.utils.XmlDocumentBuilder;


/**
 * Allows for deriving a diff between two OSM inputs, where one is the source and the other a target, 
 * in the form of an OSM XML changeset.
 * 
 * Keeping this logic separate from ChangesetResource since its is not part of the OSM web API.
 */
@Controller
@Path("/changeset")
@Transactional
public class DeriveChangesetResource extends JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(DeriveChangesetResource.class);

    public DeriveChangesetResource() {
        super(DERIVE_CHANGESET_SCRIPT);
    }

    /**
     * 
     * 
     * @param params
     * @return
     */
    @POST
    @Path("/derive")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public JobId process(String params) {
        
        /*
         * - create a temp file to pass to derivechangeset, made up of the configured temp folder
         * - 
         */
        
        String jobId = UUID.randomUUID().toString();
        jobId = "ex_" + jobId.replace("-", "");

        try {
            JSONArray commandArgs = parseParams(params);

            JSONObject arg = new JSONObject();
            arg.put("outputfolder", TEMP_OUTPUT_PATH + "/" + jobId);
            commandArgs.add(arg);

            arg = new JSONObject();
            arg.put("output", jobId);
            commandArgs.add(arg);

            String type = getParameterValue("outputtype", commandArgs);
            if ("osm_api_db".equalsIgnoreCase(type)) {
                commandArgs = getExportToOsmApiDbCommandArgs(commandArgs);
                postJobRequest(jobId, createPostBody(commandArgs));
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error exporting data! params = " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return new JobId(jobId);
    }

    /**
     * 
     * 
     * @param jobId
     * @return
     */
    @GET
    @Produces(MediaType.TEXT_XML)
    public Response get(@PathParam("jobId") String jobId) {
        Document responseDoc;

        try {
            

            responseDoc = null;//writeResponse(new User(user));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String message = "Error fetching OSM user data!";
            throw new WebApplicationException(e, Response.serverError().entity(message).build());
        }

        return Response.ok(new DOMSource(responseDoc)).build();
    }
    
    JSONArray getExportToOsmApiDbCommandArgs(JSONArray inputCommandArgs) {
        if (!Boolean.parseBoolean(OSM_API_DB_ENABLED)) {
            String msg = "Attempted to export to an OSM API database but OSM API database support is disabled";
            throw new WebApplicationException(Response.serverError().entity(msg).build());
        }

        JSONArray commandArgs = new JSONArray();
        commandArgs.addAll(inputCommandArgs);

        if (!"db".equalsIgnoreCase(getParameterValue("inputtype", commandArgs))) {
            String msg = "When exporting to an OSM API database, the input type must be a Hootenanny API database.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        String translation = getParameterValue("translation", commandArgs);
        if ((StringUtils.trimToNull(translation) != null) && !translation.toUpperCase().equals("NONE")) {
            String msg = "Custom translation not allowed when exporting to OSM API database.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        // ignoring outputname, since we're only going to have a single mapedit
        // connection configured in the core for now configured in the core for
        // now
        JSONObject arg = new JSONObject();
        arg.put("temppath", TEMP_OUTPUT_PATH);
        commandArgs.add(arg);

        // This option allows the job executor return std out to the client.
        // This is the only way
        // I've found to get the conflation summary text back from hoot command
        // line to the UI.
        arg = new JSONObject();
        arg.put("writeStdOutToStatusDetail", "true");
        commandArgs.add(arg);

        Map conflatedMap = getConflatedMap(commandArgs);

        // pass the export timestamp to the export bash script
        addMapForExportTag(conflatedMap, commandArgs);

        // pass the export aoi to the export bash script
        setAoi(conflatedMap, commandArgs);

        return commandArgs;
    }
    
    private Map getConflatedMap(JSONArray commandArgs) {
        String mapName = getParameterValue("input", commandArgs);
        Long mapId = getMapIdByName(mapName);

        // this may be checked somewhere else down the line...not sure
        if (mapId == null) {
            String msg = "Error exporting data.  No map exists with name: " + mapName;
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        Map conflatedMap = new Map(mapId);
        conflatedMap.setDisplayName(mapName);

        return conflatedMap;
    }

    // adding this to satisfy the mock
    Long getMapIdByName(String conflatedMapName) {
        return DbUtils.getMapIdByName(conflatedMapName);
    }

    // adding this to satisfy the mock
    java.util.Map<String, String> getMapTags(long mapId) {
        return DbUtils.getMapsTableTags(mapId);
    }

    // adding this to satisfy the mock
    BoundingBox getMapBounds(Map map) {
        return map.getBounds();
    }

    private void addMapForExportTag(Map map, JSONArray commandArgs) {
        java.util.Map<String, String> tags = getMapTags(map.getId());

        if (!tags.containsKey("osm_api_db_export_time")) {
            String msg = "Error exporting data.  Map with ID: " + map.getId()
                    + " and name: " + map.getDisplayName() + " has no osm_api_db_export_time tag.";
            throw new WebApplicationException(Response.status(Status.CONFLICT).entity(msg).build());
        }

        JSONObject arg = new JSONObject();
        arg.put("changesetsourcedatatimestamp", tags.get("osm_api_db_export_time"));
        commandArgs.add(arg);
    }

    private void setAoi(Map conflatedMap, JSONArray commandArgs) {
        BoundingBox bounds = getMapBounds(conflatedMap);
        JSONObject arg = new JSONObject();
        arg.put("changesetaoi", bounds.getMinLon() + "," + bounds.getMinLat() +
                "," + bounds.getMaxLon() + "," + bounds.getMaxLat());
        commandArgs.add(arg);
    }
    
    private static Document writeResponse(User user) throws ParserConfigurationException {
        Document responseDoc = XmlDocumentBuilder.create();
        Element osmElement = null;//OsmResponseHeaderGenerator.getOsmHeader(responseDoc);
        Element userElement = user.toXml(osmElement, /* user.numChangesetsModified() */ -1);
        osmElement.appendChild(userElement);
        responseDoc.appendChild(osmElement);

        return responseDoc;
    }
}
