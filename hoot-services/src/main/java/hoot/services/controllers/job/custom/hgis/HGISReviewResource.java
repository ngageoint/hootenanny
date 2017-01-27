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

import java.time.LocalDateTime;
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
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.controllers.job.Command;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.nativeinterfaces.CommandResult;
import hoot.services.utils.DbUtils;


@Controller
@Path("/review/custom/HGIS")
@Transactional
public class HGISReviewResource extends HGISResource {
    private static final Logger logger = LoggerFactory.getLogger(HGISReviewResource.class);


    public HGISReviewResource() {
        super(HGIS_PREPARE_FOR_VALIDATION_SCRIPT);
    }

    private class UpdateMapTagsCommand implements Command {

        private final String jobId;
        private final String mapName;

        UpdateMapTagsCommand(String jobId, String mapName) {
            this.jobId = jobId;
            this.mapName = mapName;
        }

        @Override
        public CommandResult execute() {
            CommandResult commandResult = new CommandResult();
            commandResult.setCommand("updateMapsTag");
            commandResult.setJobId(this.jobId);
            commandResult.setStart(LocalDateTime.now());

            updateMapsTag();

            commandResult.setExitCode(CommandResult.SUCCESS);
            commandResult.setFinish(LocalDateTime.now());

            return commandResult;
        }

        private void updateMapsTag() {
            try {
                long mapId = ModelDaoUtils.getRecordIdForInputString(this.mapName, maps, maps.id, maps.displayName);
                updateMapTagWithReviewType(mapId);
            }
            catch (ReviewMapTagUpdateException e) {
                throw new RuntimeException("Error updating map " + mapName + "'s tags!", e);
            }
        }

        private void updateMapTagWithReviewType(long mapId) throws ReviewMapTagUpdateException {
            Map<String, String> tags = new HashMap<>();
            tags.put("reviewtype", "hgisvalidation");

            long count = DbUtils.updateMapsTableTags(tags, mapId);

            if (count < 1) {
                throw new ReviewMapTagUpdateException("Failed to update maps table for mapid = " + mapId);
            }
        }
    }

    /**
     * This resource prepares existing map for 30% of random HGIS specific validation.
     * <p>
     *     POST hoot-services/job/review/custom/HGIS/preparevalidation
     * <p>
     * {
     *   "sourceMap":"AllDataTypesA", //Name of source layer
     *   "outputMap":"AllDataTypesAtest1" //Name of new output layer with reviewables
     * }
     *
     * @param request
     * @return Job ID
     */
    @POST
    @Path("/preparevalidation")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public PrepareForValidationResponse prepareItemsForValidationReview(PrepareForValidationRequest request) {
        PrepareForValidationResponse response = new PrepareForValidationResponse();

        String src = request.getSourceMap();
        String outputMap = request.getOutputMap();

        if (src == null) {
            String msg = "Invalid or empty sourceMap.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        if (outputMap == null) {
            String msg = "Invalid or empty outputMap.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        if (!mapExists(src)) {
            String msg = "sourceMap does not exist.";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        try {
            String jobId = UUID.randomUUID().toString();
            JSONArray commandArgs = new JSONArray();

            JSONObject arg = new JSONObject();
            arg.put("SOURCE", src);
            commandArgs.add(arg);

            arg = new JSONObject();
            arg.put("OUTPUT", outputMap);
            commandArgs.add(arg);

            JSONObject validationCommand = super.createBashScriptJobReq(commandArgs);
            Command updateMapTagsCommand = new UpdateMapTagsCommand(jobId, outputMap);

            Command[] commands = {
                    () -> { return super.externalCommandInterface.exec(jobId, validationCommand); },
                    () -> { return super.internalCommandInterface.exec(jobId, updateMapTagsCommand); }
            };

            super.processChainJob(jobId, commands);

            response.setJobId(jobId);
        }
        catch (Exception ex) {
            String msg = ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return response;
    }
}
