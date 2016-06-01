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

import java.sql.Connection;
import java.util.ArrayList;
import java.util.List;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.support.DefaultTransactionDefinition;

import com.mysema.query.sql.SQLQuery;

import hoot.services.HootProperties;
import hoot.services.controllers.osm.MapResource;
import hoot.services.db.DbUtils;
import hoot.services.db2.QMaps;
import hoot.services.geo.BoundingBox;
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
import hoot.services.review.ReviewUtils;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.writers.review.ReviewResolver;


/**
 * Service endpoint for the conflated data review process
 */
@Path("/review")
public class ReviewResource {
    private static final Logger log = LoggerFactory.getLogger(ReviewResource.class);

    private QMaps maps = QMaps.maps;

    private ClassPathXmlApplicationContext appContext;
    private PlatformTransactionManager transactionManager;

    private static long MAX_RESULT_SIZE = 60000;
    static {
        try {
            String maxQuerySize = HootProperties.getProperty("maxQueryNodes");
            MAX_RESULT_SIZE = Long.parseLong(maxQuerySize);
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    public ReviewResource() throws Exception {
        appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
        transactionManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);
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
     * @throws Exception
     */
    @PUT
    @Path("/resolveall")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewResolverResponse resolveAllReviews(final ReviewResolverRequest request) throws Exception {
        log.debug("Setting all items reviewed for map with ID: " + request.getMapId() + "...");

        Connection conn = DbUtils.createConnection();
        TransactionStatus transactionStatus = transactionManager
                .getTransaction(new DefaultTransactionDefinition(TransactionDefinition.PROPAGATION_REQUIRED));
        conn.setAutoCommit(false);

        final long mapIdNum = MapResource.validateMap(request.getMapId(), conn);
        assert (mapIdNum != -1);

        long userId = -1;
        try {
            log.debug(
                    "Retrieving user ID associated with map having ID: " + String.valueOf(request.getMapId()) + " ...");
            userId = new SQLQuery(conn, DbUtils.getConfiguration()).from(maps).where(maps.id.eq(mapIdNum))
                    .singleResult(maps.userId);
            log.debug("Retrieved user ID: " + userId);
        }
        catch (Exception e) {
            ResourceErrorHandler.handleError("Error locating user associated with map having ID: " + request.getMapId()
                    + " (" + e.getMessage() + ")", Status.BAD_REQUEST, log);
        }
        assert (userId != -1);

        long changesetId = -1;
        try {
            changesetId = (new ReviewResolver(conn)).setAllReviewsResolved(mapIdNum, userId);

            log.debug("Committing set all items reviewed transaction...");
            transactionManager.commit(transactionStatus);
            conn.commit();
        }
        catch (Exception e) {
            transactionManager.rollback(transactionStatus);
            conn.rollback();
            ReviewUtils.handleError(e, "Error setting all records to reviewed for map ID: " + request.getMapId(),
                    false);
        }
        finally {
            conn.setAutoCommit(true);
            DbUtils.closeConnection(conn);
        }

        log.debug(
                "Set all items reviewed for map with ID: " + request.getMapId() + " using changesetId: " + changesetId);
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
     * @throws Exception
     */
    @POST
    @Path("/refs")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewRefsResponses getReviewReferences(final ReviewRefsRequest request) throws Exception {
        log.debug("Returning review references...");

        ReviewRefsResponses response = new ReviewRefsResponses();
        Connection conn = DbUtils.createConnection();
        try {
            ReviewReferencesRetriever refsRetriever = new ReviewReferencesRetriever(conn);
            List<ReviewRefsResponse> responseRefsList = new ArrayList<ReviewRefsResponse>();
            for (ElementInfo elementInfo : request.getQueryElements()) {
                ReviewRefsResponse responseRefs = new ReviewRefsResponse();
                // Now we are returning self since in one to many queried
                // element can be involved in
                // many different relations and since we do not know the
                // element's parent relation (or even if there is one)
                // we are forced return all including self. (Client need to
                // handle self)
                List<ReviewRef> references = refsRetriever.getAllReferences(elementInfo);
                log.debug("Returning " + references.size() + " review references for requesting element: "
                        + elementInfo.toString());
                responseRefs.setReviewRefs(references.toArray(new ReviewRef[] {}));
                responseRefs.setQueryElementInfo(elementInfo);
                responseRefsList.add(responseRefs);
            }
            response.setReviewRefsResponses(responseRefsList.toArray(new ReviewRefsResponse[] {}));
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        log.debug("response : " + StringUtils.abbreviate(response.toString(), 1000));
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
        ReviewableItem ret = new ReviewableItem(-1, -1, -1);
        try (Connection conn = DbUtils.createConnection()) {
            long nMapId = Long.parseLong(mapId);
            ReviewableReader reader = new ReviewableReader(conn);
            ret = reader.getRandomReviewableItem(nMapId);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError(
                    "Error getting random reviewable item: " + mapId + " (" + ex.getMessage() + ")", Status.BAD_REQUEST,
                    log);
        }
        return ret;
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
            @QueryParam("offsetseqid") String offsetSeqId, @QueryParam("direction") String direction) {

        ReviewableItem ret = new ReviewableItem(-1, -1, -1);
        try (Connection conn = DbUtils.createConnection()) {

            long nMapId = Long.parseLong(mapId);
            long nOffsetSeqId = Long.parseLong(offsetSeqId);

            // if nextSquence is - or out of index value we will get random
            long nextSequence = nOffsetSeqId + 1;
            if (direction != null && direction.equalsIgnoreCase("backward")) {
                nextSequence = nOffsetSeqId - 1;
            }

            ReviewableReader reader = new ReviewableReader(conn);
            ret = reader.getReviewableItem(nMapId, nextSequence);
            // get random if we can not find immediate next sequence item
            if (ret.getResultCount() < 1) {
                ret = getRandomReviewable(mapId);
            }
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError(
                    "Error getting next reviewable item: " + mapId + " (" + ex.getMessage() + ")", Status.BAD_REQUEST,
                    log);
        }
        return ret;
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

        ReviewableItem ret = new ReviewableItem(-1, -1, -1);
        try (Connection conn = DbUtils.createConnection()) {
            long nMapId = Long.parseLong(mapId);
            long nOffsetSeqId = Long.parseLong(offsetSeqId);
            ReviewableReader reader = new ReviewableReader(conn);
            ret = reader.getReviewableItem(nMapId, nOffsetSeqId);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting reviewable item: " + mapId + " (" + ex.getMessage() + ")",
                    Status.BAD_REQUEST, log);
        }
        return ret;
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

        ReviewableStatistics ret = new ReviewableStatistics(-1, -1);
        try (Connection conn = DbUtils.createConnection()) {
            long nMapId = Long.parseLong(mapId);
            ReviewableReader reader = new ReviewableReader(conn);
            ret = reader.getReviewablesStatistics(nMapId);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError(
                    "Error getting reviewables statistics: " + mapId + " (" + ex.getMessage() + ")", Status.BAD_REQUEST,
                    log);
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
    public JSONObject getReviewable(@QueryParam("mapid") String mapId, @QueryParam("minlon") String minLon,
            @QueryParam("minlat") String minLat, @QueryParam("maxlon") String maxLon,
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
            AllReviewableItems result = reader.getAllReviewableItems(nMapId,
                    new BoundingBox(minlon, minlat, maxlon, maxlat));
            ret = new JSONObject();
            if (result.getOverFlow() == true) {
                ret.put("warning", "The result size is greater than maximum limit of:" + MAX_RESULT_SIZE
                        + ". Returning truncated data.");
            }
            ret.put("total", result.getReviewableItems().size());
            ret.put("geojson", result.toGeoJson());
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting reviewable item: " + mapId + " (" + ex.getMessage() + ")",
                    Status.BAD_REQUEST, log);
        }
        return ret;
    }
}
