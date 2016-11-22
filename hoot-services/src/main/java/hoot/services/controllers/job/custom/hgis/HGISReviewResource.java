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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job.custom.hgis;

import static hoot.services.HootProperties.HGIS_PREPARE_FOR_VALIDATION_SCRIPT;
import static hoot.services.models.db.QMaps.maps;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.controllers.job.JobStatusManager;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.utils.DbUtils;


@Controller
@Path("/review/custom/HGIS")
@Transactional
public class HGISReviewResource extends HGISResource {
    private static final Logger logger = LoggerFactory.getLogger(HGISReviewResource.class);

    @Autowired
    private JobStatusManager jobStatusManager;


    public HGISReviewResource() {
        super(HGIS_PREPARE_FOR_VALIDATION_SCRIPT);
    }

    /**
     * This resource prepares existing map for 30% of random HGIS specific
     * validation.
     * <p>
     * POST hoot-services/job/review/custom/HGIS/preparevalidation
     * <p>
     * { "sourceMap":"AllDataTypesA", //Name of source layer
     * "outputMap":"AllDataTypesAtest1" //Name of new output layer with
     * reviewables }
     *
     * @param request
     * @return Job ID
     */
    @POST
    @Path("/preparevalidation")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public PrepareForValidationResponse prepareItemsForValidationReview(PrepareForValidationRequest request) {
        PrepareForValidationResponse res = new PrepareForValidationResponse();

        String src = request.getSourceMap();
        String output = request.getOutputMap();

        if (src == null) {
            String msg = "Invalid or empty sourceMap.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        if (output == null) {
            String msg = "Invalid or empty outputMap.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        if (!mapExists(src)) {
            String msg = "sourceMap does not exist.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        try {
            String jobId = UUID.randomUUID().toString();

            JSONObject validationCommand = _createBashPostBody(createParamObj(src, output));
            JSONObject updateMapTagCommand = createUpdateMapTagCommand(output);

            // with new relation based review process we will no longer need to run prepare review
            // Instead core will take care of generation relation review for POI validation
            JSONArray jobArgs = new JSONArray();
            jobArgs.add(validationCommand);
            jobArgs.add(updateMapTagCommand);

            postChainJobRequest(jobId, jobArgs.toJSONString());

            res.setJobId(jobId);
        }
        catch (Exception ex) {
            String msg = ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return res;
    }

    private static JSONObject createUpdateMapTagCommand(String mapName) {
        JSONArray reviewArgs = new JSONArray();
        JSONObject param = new JSONObject();

        param.put("value", mapName);
        param.put("paramtype", String.class.getName());
        param.put("isprimitivetype", "false");
        reviewArgs.add(param);

        return createReflectionJobReq(reviewArgs, HGISReviewResource.class.getName(), "updateMapsTag");
    }

    // Warning: do not remove this method even though it will appear as unused in your IDE of choice.
    // The method is invoked reflectively
    public String updateMapsTag(String mapName) {
        String jobId = UUID.randomUUID().toString();
        try {
            jobStatusManager.addJob(jobId);

            long mapId = ModelDaoUtils.getRecordIdForInputString(mapName, maps, maps.id, maps.displayName);

            updateMapTagWithReviewType(mapId);

            jobStatusManager.setComplete(jobId);
        }
        catch (ReviewMapTagUpdateException e) {
            jobStatusManager.setFailed(jobId);
            throw new RuntimeException("Error updating map " + mapName + "'s tags!", e);
        }

        return jobId;
    }

    private static void updateMapTagWithReviewType(long mapId) throws ReviewMapTagUpdateException {
        Map<String, String> tags = new HashMap<>();
        tags.put("reviewtype", "hgisvalidation");

        long count = DbUtils.updateMapsTableTags(tags, mapId);

        if (count < 1) {
            throw new ReviewMapTagUpdateException("Failed to update maps table for mapid = " + mapId);
        }
    }
}
