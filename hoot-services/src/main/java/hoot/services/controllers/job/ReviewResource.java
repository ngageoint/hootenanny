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

import static hoot.services.HootProperties.MAX_QUERY_NODES;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

import hoot.services.controllers.osm.MapResource;
import hoot.services.db2.QMaps;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.ElementInfo;
import hoot.services.models.review.AllReviewableItems;
import hoot.services.models.review.ReviewRef;
import hoot.services.models.review.ReviewRefsRequest;
import hoot.services.models.review.ReviewRefsResponse;
import hoot.services.models.review.ReviewRefsResponses;
import hoot.services.models.review.ReviewResolverRequest;
import hoot.services.models.review.ReviewResolverResponse;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableStatistics;
import hoot.services.readers.review.ReviewReferencesRetriever;
import hoot.services.readers.review.ReviewableReader;
import hoot.services.utils.DbUtils;
import hoot.services.utils.ReviewUtils;


/**
 * Service endpoint for the conflated data review process
 */
@Path("/review")
public class ReviewResource {
    private static final Logger logger = LoggerFactory.getLogger(ReviewResource.class);
    private static final long MAX_RESULT_SIZE;

    private final QMaps maps = QMaps.maps;

    static {
        long value;

        try {
            value = Long.parseLong(MAX_QUERY_NODES);
        }
        catch (NumberFormatException ignored) {
            value = 60000;
            logger.error("maxQueryNodes is not a valid number.  Defaulting to {}", value);
        }

        MAX_RESULT_SIZE = value;
    }

    public ReviewResource() {
    }

    /**
     * Resolves all reviews for a given map
     * 
     * Have to use a request object here, rather than a single map ID query
     * param, since d3 can't send plain text in a PUT statement.
     * 
     * PUT hoot-services/job/review/resolveall
     * 
     * //TODO: JSON input example
     * 
     * @param request
     *            a JSON request containing the map ID for the reviews to be
     *            resolved
     * @return a JSON response with the changeset ID used to resolve the reviews
     */
    @PUT
    @Path("/resolveall")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewResolverResponse resolveAllReviews(ReviewResolverRequest request) {
        logger.debug("Setting all items reviewed for map with ID: {}...", request.getMapId());

        long changesetId = -1;
        try (Connection conn = DbUtils.createConnection()) {
            conn.setAutoCommit(false);

            long mapIdNum = MapResource.validateMap(request.getMapId(), conn);

            long userId;
            try {
                logger.debug("Retrieving user ID associated with map having ID: {} ...", request.getMapId());
                userId = new SQLQuery(conn, DbUtils.getConfiguration()).from(maps).where(maps.id.eq(mapIdNum))
                        .singleResult(maps.userId);
                logger.debug("Retrieved user ID: {}", userId);
            }
            catch (Exception e) {
                String message = "Error locating user associated with map having ID: "
                        + request.getMapId() + " (" + e.getMessage() + ")";
                throw new WebApplicationException(e, Response.status(Status.BAD_REQUEST).entity(message).build());
            }

            try {
                changesetId = setAllReviewsResolved(mapIdNum, userId, conn);
                logger.debug("Committing set all items reviewed transaction...");
                conn.commit();
            }
            catch (Exception e) {
                conn.rollback();
                ReviewUtils.handleError(e, "Error setting all records to reviewed for map ID: " + request.getMapId());
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error resolving all reviews associated with map having ID: "
                    + request.getMapId() + " (" + e.getMessage() + ")";
            throw new WebApplicationException(e, Response.status(Status.INTERNAL_SERVER_ERROR).entity(msg).build());
        }

        logger.debug("Set all items reviewed for map with ID: {} using changesetId: {}", request.getMapId(), changesetId);

        return new ReviewResolverResponse(changesetId);

    }

    /**
     * Returns any review references to the elements associated with the ID's
     * passed in
     * 
     * Technically, this should be a GET request, but since the size of the
     * input could potentially be large, making it a POST request to get past
     * any size limit restrictions on GET requests.
     * 
     * POST hoot-services/job/review/refs
     * 
     * //TODO: input data example
     * 
     * @param request
     *            JSON request containing a collection of elements for which
     *            review references are to be retrieved
     * @return an array of review references in JSON; one set of references for
     *         each query element passed in; The returned ReviewRef object
     *         extends the ElementInfo object to add the associated review
     *         relation id.
     */
    @POST
    @Path("/refs")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewRefsResponses getReviewReferences(ReviewRefsRequest request) {
        logger.debug("Returning review references...");

        ReviewRefsResponses response = new ReviewRefsResponses();

        try (Connection conn = DbUtils.createConnection()) {
            ReviewReferencesRetriever refsRetriever = new ReviewReferencesRetriever(conn);
            List<ReviewRefsResponse> responseRefsList = new ArrayList<>();
            for (ElementInfo elementInfo : request.getQueryElements()) {
                ReviewRefsResponse responseRefs = new ReviewRefsResponse();
                // Now we are returning self since in one to many queried
                // element can be involved in many different relations and since we do not know the
                // element's parent relation (or even if there is one)
                // we are forced return all including self. (Client need to handle self)
                List<ReviewRef> references = refsRetriever.getAllReferences(elementInfo);
                logger.debug("Returning {} review references for requesting element: {}", references.size(), elementInfo);
                responseRefs.setReviewRefs(references.toArray(new ReviewRef[references.size()]));
                responseRefs.setQueryElementInfo(elementInfo);
                responseRefsList.add(responseRefs);
            }
            response.setReviewRefsResponses(responseRefsList.toArray(new ReviewRefsResponse[responseRefsList.size()]));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error getting review references!";
            throw new WebApplicationException(e, Response.status(Status.INTERNAL_SERVER_ERROR).entity(msg).build());
        }

        logger.debug("response : {}", StringUtils.abbreviate(response.toString(), 1000));

        return response;
    }

    /**
     * To retrieve the random reviewable item. If a reviewable is not available
     * then return 0 result count
     * 
     * GET hoot-services/job/review/random?mapid=15
     * 
     * @param mapId
     *            Target map id
     * @return JSON in ReviewableItem format
     */
    @GET
    @Path("/random")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewableItem getRandomReviewable(@QueryParam("mapid") String mapId) {
        try (Connection conn = DbUtils.createConnection()) {
            long nMapId = Long.parseLong(mapId);
            ReviewableReader reader = new ReviewableReader(conn);
            ReviewableItem ret = reader.getRandomReviewableItem(nMapId);
            return ret;
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error getting random reviewable item: " + mapId + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.status(Status.INTERNAL_SERVER_ERROR).entity(msg).build());
        }
    }

    /**
     * To retrieve the next reviewable item based on offset sequence id. If next
     * reviewable is not available then try to get random reviewable item.
     * 
     * GET hoot-services/job/review/next?mapid=15&offsetseqid=2
     * 
     * @param mapId
     *            Target map id
     * @param offsetSeqId
     *            Current Offset sequence id which gets incremented to
     *            offsetseqid+1.
     * @param direction
     *            ?
     * @return JSON in ReviewableItem format
     */
    @GET
    @Path("/next")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewableItem getNextReviewable(@QueryParam("mapid") String mapId,
                                            @QueryParam("offsetseqid") String offsetSeqId,
                                            @QueryParam("direction") String direction) {
        try (Connection conn = DbUtils.createConnection()) {
            long nMapId = Long.parseLong(mapId);
            long nOffsetSeqId = Long.parseLong(offsetSeqId);

            // if nextSquence is - or out of index value we will get random
            long nextSequence = nOffsetSeqId + 1;
            if ("backward".equalsIgnoreCase(direction)) {
                nextSequence = nOffsetSeqId - 1;
            }

            ReviewableReader reader = new ReviewableReader(conn);
            ReviewableItem ret = reader.getReviewableItem(nMapId, nextSequence);

            // get random if we can not find immediate next sequence item
            if (ret.getResultCount() < 1) {
                ret = getRandomReviewable(mapId);
            }

            return ret;
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error getting next reviewable item: " + mapId + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.status(Status.INTERNAL_SERVER_ERROR).entity(msg).build());
        }
    }

    /**
     * To retrieve the reviewable item based on offset sequence id. If
     * reviewable is not available then return 0 result count
     * 
     * GET hoot-services/job/review/next?mapid=15&offsetseqid=2
     * 
     * @param mapId
     *            Target map id
     * @param offsetSeqId
     *            Offset sequence id.
     * @return JSON in ReviewableItem format
     */
    @GET
    @Path("/reviewable")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewableItem getReviewable(@QueryParam("mapid") String mapId,
                                        @QueryParam("offsetseqid") String offsetSeqId) {
        try (Connection conn = DbUtils.createConnection()) {
            long nMapId = Long.parseLong(mapId);
            long nOffsetSeqId = Long.parseLong(offsetSeqId);
            ReviewableReader reader = new ReviewableReader(conn);
            ReviewableItem ret = reader.getReviewableItem(nMapId, nOffsetSeqId);
            return ret;
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error getting reviewable item: " + mapId + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.status(Status.INTERNAL_SERVER_ERROR).entity(msg).build());
        }
    }

    /**
     * To retrieve the reviewable statistics for a map
     * 
     * GET hoot-services/job/review/statistics?mapid=15
     * 
     * @param mapId
     *            Target map id
     * @return JSON in ReviewableStatistics format
     */
    @GET
    @Path("/statistics")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewableStatistics getReviewableSstatistics(@QueryParam("mapId") String mapId) {
        ReviewableStatistics ret;
        if (Long.parseLong(mapId) == -1) { // OSM API db
            ret = new ReviewableStatistics();
        }
        else {
            try (Connection conn = DbUtils.createConnection()) {
                long nMapId = Long.parseLong(mapId);
                ReviewableReader reader = new ReviewableReader(conn);
                ret = reader.getReviewablesStatistics(nMapId);
            }
            catch (WebApplicationException wae) {
                throw wae;
            }
            catch (Exception ex) {
                String msg = "Error getting reviewables statistics: " + mapId + " (" + ex.getMessage() + ")";
                throw new WebApplicationException(ex, Response.status(Status.INTERNAL_SERVER_ERROR).entity(msg).build());
            }
        }

        return ret;
    }

    /**
     * To retrieve GeoJson of all reviewable items within bounding box
     * 
     * GET
     * hoot-services/job/review/allreviewables?mapid=53&minlon=-180&minlat=-90&
     * maxlon=180&maxlat=90
     * 
     * @param mapId
     *            Target map id
     * @param minLon
     *            Target map id
     * @param minLat
     *            Minimum latitude
     * @param maxLon
     *            Maximum longitude
     * @param maxLat
     *            Maximum latitude
     * @return GeoJson containing reviewable bounding box and state
     */
    @GET
    @Path("/allreviewables")
    @Produces(MediaType.APPLICATION_JSON)
    public JSONObject getReviewable(@QueryParam("mapid") String mapId,
                                    @QueryParam("minlon") String minLon,
                                    @QueryParam("minlat") String minLat,
                                    @QueryParam("maxlon") String maxLon,
                                    @QueryParam("maxlat") String maxLat) {
        JSONObject ret = new JSONObject();
        ret.put("type", "FeatureCollection");
        ret.put("features", new JSONArray());

        try (Connection conn = DbUtils.createConnection()) {
            long nMapId = Long.parseLong(mapId);
            double minlon = Double.parseDouble(minLon);
            double minlat = Double.parseDouble(minLat);
            double maxlon = Double.parseDouble(maxLon);
            double maxlat = Double.parseDouble(maxLat);
            ReviewableReader reader = new ReviewableReader(conn);
            AllReviewableItems result = reader.getAllReviewableItems(nMapId, new BoundingBox(minlon, minlat, maxlon,
                    maxlat));
            ret = new JSONObject();

            if (result.getOverFlow()) {
                ret.put("warning", "The result size is greater than maximum limit of:" + MAX_RESULT_SIZE
                        + ". Returning truncated data.");
            }

            ret.put("total", result.getReviewableItems().size());
            ret.put("geojson", result.toGeoJson());
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String message = "Error getting reviewable item: " + mapId + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.status(Status.INTERNAL_SERVER_ERROR).entity(message).build());
        }

        return ret;
    }

    /**
     * Resolves all reviews for a given map
     *
     * @param mapId
     *            ID of the map owning the review data
     * @param userId
     *            user ID associated with the review data
     * @return the ID of the changeset used to resolve the reviews
     */
    private static long setAllReviewsResolved(long mapId, long userId, Connection connection) {
        // create a changeset
        Map<String, String> changesetTags = new HashMap<>();
        changesetTags.put("bot", "yes");
        changesetTags.put("created_by", "hootenanny");

        long changesetId = Changeset.createChangeset(mapId, userId, changesetTags, connection);
        Changeset.closeChangeset(mapId, changesetId, connection);

        /*
         * - mark all review relations belonging to the map as resolved - update
         * the changeset id for each review relation - increment the version for
         * each review relation
         */
        String sql = "";
        sql += "update current_relations_" + mapId;
        sql += " set tags = tags || hstore('hoot:review:needs', 'no'),";
        sql += " changeset_id = " + changesetId + ",";
        sql += " version = version + 1";
        sql += " where tags->'type' = 'review'";

        try (Statement stmt = connection.createStatement()) {
            int numRecordsUpdated = stmt.executeUpdate(sql);
            logger.debug("{} records updated.", numRecordsUpdated);
        }
        catch (SQLException e) {
            throw new RuntimeException("Error retrieving all resolved reviews!", e);
        }

        return changesetId;
    }
}
