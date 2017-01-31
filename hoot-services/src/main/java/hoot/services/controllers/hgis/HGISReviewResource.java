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
package hoot.services.controllers.hgis;

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

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.Command;
import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.command.InternalCommand;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.utils.DbUtils;


@Controller
@Path("/review/custom/HGIS")
@Transactional
public class HGISReviewResource extends HGISResource {
    private static final Logger logger = LoggerFactory.getLogger(HGISReviewResource.class);


    public HGISReviewResource() {
        super(HGIS_PREPARE_FOR_VALIDATION_SCRIPT);
    }

    private static final class UpdateMapTagsCommand implements InternalCommand {

        private final String jobId;
        private final String mapName;

        private UpdateMapTagsCommand(String jobId, String mapName) {
            this.jobId = jobId;
            this.mapName = mapName;
        }

        @Override
        public CommandResult execute() {
            CommandResult commandResult = new CommandResult();
            commandResult.setCommand("updateMapTags");
            commandResult.setJobId(this.jobId);
            commandResult.setStart(LocalDateTime.now());

            this.updateMapTags();

            commandResult.setExitCode(CommandResult.SUCCESS);
            commandResult.setFinish(LocalDateTime.now());

            return commandResult;
        }

        private void updateMapTags() {
            long mapId = ModelDaoUtils.getRecordIdForInputString(this.mapName, maps, maps.id, maps.displayName);
            Map<String, String> tags = new HashMap<>();
            tags.put("reviewtype", "hgisvalidation");

            long count = DbUtils.updateMapsTableTags(tags, mapId);

            if (count < 1) {
                throw new RuntimeException("Error updating map " + mapId + "'s tags!");
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

        checkHGISCommandParams(request.getSourceMap(), request.getOutputMap());

        try {
            String jobId = UUID.randomUUID().toString();

            Command[] chainJob = {
                    () -> {
                        ExternalCommand validationCommand = super.createHGISCommand(request.getSourceMap(), request.getOutputMap());
                        return super.externalCommandManager.exec(jobId, validationCommand);
                    },
                    () -> {
                        InternalCommand updateMapTagsCommand = new UpdateMapTagsCommand(jobId, request.getOutputMap());
                        return super.internalCommandManager.exec(jobId, updateMapTagsCommand);
                    }
            };

            super.processChainJob(jobId, chainJob);

            response.setJobId(jobId);
        }
        catch (Exception ex) {
            String msg = ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return response;
    }
}
