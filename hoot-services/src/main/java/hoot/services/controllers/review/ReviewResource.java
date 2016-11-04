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
package hoot.services.controllers.review;

import static hoot.services.models.db.QCurrentRelationMembers.currentRelationMembers;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
import static hoot.services.models.db.QMaps.maps;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

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
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import com.querydsl.core.types.dsl.Expressions;

import hoot.services.controllers.osm.MapResource;
import hoot.services.geo.BoundingBox;
import hoot.services.models.db.CurrentRelationMembers;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element;


/**
 * Service endpoint for the conflated data review process
 */
@Controller
@Path("/review")
@Transactional
public class ReviewResource {
    private static final Logger logger = LoggerFactory.getLogger(ReviewResource.class);

    public ReviewResource() {}

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
        long changesetId = -1;
        try {
            long mapIdNum = MapResource.validateMap(request.getMapId());

            long userId;
            try {
                userId = createQuery().select(maps.userId).from(maps).where(maps.id.eq(mapIdNum)).fetchOne();
            }
            catch (Exception e) {
                String message = "Error locating user associated with map having ID: "
                        + request.getMapId() + " (" + e.getMessage() + ")";
                throw new WebApplicationException(e, Response.status(Status.BAD_REQUEST).entity(message).build());
            }

            try {
                changesetId = setAllReviewsResolved(mapIdNum, userId);
            }
            catch (Exception e) {
                handleError(e, "Error setting all records to reviewed for map ID: " + request.getMapId());
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error resolving all reviews associated with map having ID: "
                    + request.getMapId() + " (" + e.getMessage() + ")";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

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
        ReviewRefsResponses response = new ReviewRefsResponses();

        try {
            List<ReviewRefsResponse> responseRefsList = new ArrayList<>();
            for (ElementInfo elementInfo : request.getQueryElements()) {
                ReviewRefsResponse responseRefs = new ReviewRefsResponse();
                // Now we are returning self since in one to many queried
                // element can be involved in many different relations and since we do not know the
                // element's parent relation (or even if there is one)
                // we are forced return all including self. (Client need to handle self)
                List<ReviewRef> references = getAllReferences(elementInfo);
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
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return response;
    }

    /**
     * To retrieve the random reviewable item. If a reviewable is not available then return 0 result count
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
    public ReviewableItem getRandomReviewable(@QueryParam("mapid") Long mapId) {
        ReviewableItem randomReviewableItem;

        try {
            randomReviewableItem = getRandomReviewableItem(mapId);
        }
        catch (Exception e) {
            String msg = "Error getting random reviewable item: " + mapId + " (" + e.getMessage() + ")";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return randomReviewableItem;
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
    public ReviewableItem getNextReviewable(@QueryParam("mapid") Long mapId,
                                            @QueryParam("offsetseqid") Long offsetSeqId,
                                            @QueryParam("direction") String direction) {
        ReviewableItem reviewableItem;

        try {
            // if nextSquence is - or out of index value we will get random
            long nextSequence = offsetSeqId + 1;
            if ("backward".equalsIgnoreCase(direction)) {
                nextSequence = offsetSeqId - 1;
            }

            reviewableItem = getReviewableItem(mapId, nextSequence);

            // get random if we can not find immediate next sequence item
            if (reviewableItem.getResultCount() < 1) {
                reviewableItem = this.getRandomReviewable(mapId);
            }
        }
        catch (Exception e) {
            String msg = "Error getting next reviewable item: " + mapId + " (" + e.getMessage() + ")";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return reviewableItem;
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
    public ReviewableItem getReviewable(@QueryParam("mapid") Long mapId,
                                        @QueryParam("offsetseqid") Long offsetSeqId) {
        ReviewableItem reviewableItem;

        try {
            reviewableItem = getReviewableItem(mapId, offsetSeqId);
        }
        catch (Exception e) {
            String msg = "Error getting reviewable item: " + mapId + " (" + e.getMessage() + ")";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return reviewableItem;
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
    public ReviewableStatistics getReviewableStatistics(@QueryParam("mapId") Long mapId) {
        ReviewableStatistics reviewableStatistics;

        if (mapId == -1) { // OSM API db
            reviewableStatistics = new ReviewableStatistics();
        }
        else {
            try {
                reviewableStatistics = getReviewablesStatistics(mapId);
            }
            catch (Exception e) {
                String msg = "Error getting reviewables statistics: " + mapId + " (" + e.getMessage() + ")";
                throw new WebApplicationException(e, Response.serverError().entity(msg).build());
            }
        }

        return reviewableStatistics;
    }

    /**
     * To retrieve GeoJson of all reviewable items within bounding box
     * 
     * GET hoot-services/job/review/allreviewables?mapid=53&minlon=-180&minlat=-90&maxlon=180&maxlat=90
     * 
     * @param mapId
     *            Target map id
     * @param minLon
     *            Minimum longitude
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
    public JSONObject getReviewable(@QueryParam("mapid") Long mapId,
                                    @QueryParam("minlon") Double minLon,
                                    @QueryParam("minlat") Double minLat,
                                    @QueryParam("maxlon") Double maxLon,
                                    @QueryParam("maxlat") Double maxLat) {
        JSONObject response = new JSONObject();
        response.put("type", "FeatureCollection");
        response.put("features", new JSONArray());

        try {
            AllReviewableItems result = getAllReviewableItems(mapId, new BoundingBox(minLon, minLat, maxLon, maxLat));
            response = new JSONObject();

            if (result.getOverflow()) {
                response.put("warning", "The result size is greater than maximum limit of:" +
                        AllReviewableItemsQuery.MAX_RESULT_SIZE +
                        ". Returning truncated data.");
            }

            response.put("total", result.getReviewableItems().size());
            response.put("geojson", result.toGeoJson());
        }
        catch (Exception e) {
            String msg = "Error getting reviewable item: " + mapId + " (" + e.getMessage() + ")";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return response;
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
    private static long setAllReviewsResolved(long mapId, long userId) {
        // create a changeset
        Map<String, String> changesetTags = new HashMap<>();
        changesetTags.put("bot", "yes");
        changesetTags.put("created_by", "hootenanny");

        long changesetId = Changeset.createChangeset(mapId, userId, changesetTags);
        Changeset.closeChangeset(mapId, changesetId);

        /*
         * - mark all review relations belonging to the map as resolved - update
         * the changeset id for each review relation - increment the version for
         * each review relation
         */
        long numRecordsUpdated = createQuery(mapId)
                .update(currentRelations)
                .where(Expressions.booleanTemplate("tags->'type' = 'review'"))
                .set(Arrays.asList(currentRelations.changesetId, currentRelations.version, currentRelations.tags),
                     Arrays.asList(changesetId, Expressions.stringTemplate("version + 1"),
                                   Expressions.stringTemplate("tags || hstore('hoot:review:needs', 'no')")))
                .execute();

        return changesetId;
    }

    private static ReviewableItem getReviewableItem(long mapId, long sortOrder) {
        return (ReviewableItem) getReviewableQuery(mapId, sortOrder).execQuery();
    }

    private static ReviewableItem getRandomReviewableItem(long mapId) {
        return (ReviewableItem) getRandomReviewableQuery(mapId).execQuery();
    }

    private static ReviewableStatistics getReviewablesStatistics(long mapId) {
        return (ReviewableStatistics) getReviewableStatisticsQuery(mapId).execQuery();
    }

    private static AllReviewableItems getAllReviewableItems(long mapId, BoundingBox bbox) {
        return (AllReviewableItems) getAllReviewableItemsQuery(mapId, bbox).execQuery();
    }

    private static ReviewableQuery getReviewableQuery(long mapId, long seqId) {
        return new ReviewableQuery(mapId, seqId);
    }

    private static RandomReviewableQuery getRandomReviewableQuery(long mapId) {
        return new RandomReviewableQuery(mapId);
    }

    private static ReviewableStatisticsQuery getReviewableStatisticsQuery(long mapId) {
        return new ReviewableStatisticsQuery(mapId);
    }

    private static AllReviewableItemsQuery getAllReviewableItemsQuery(long mapId, BoundingBox bbox) {
        return new AllReviewableItemsQuery(mapId, bbox);
    }

    private static List<Long> getAllReviewRelations(ElementInfo queryElementInfo, long mapId) {
        return createQuery(mapId)
                .select(currentRelationMembers.relationId)
                .from(currentRelationMembers)
                .join(currentRelations).on(currentRelationMembers.relationId.eq(currentRelations.id))
                .where(currentRelationMembers.memberId.eq(queryElementInfo.getId())
                        .and(currentRelationMembers.memberType.eq(
                                Element.elementEnumForElementType(
                                        Element.elementTypeFromString(queryElementInfo.getType())))))
                .fetch();
    }

    /**
     * Retrieves all other unresolved element references to reviews for a given
     * element
     *
     * @param queryElementInfo
     *            element whose review references are to be retrieved
     * @return a list containing all features the input feature needs to be
     *         reviewed with
     */
    private static List<ReviewRef> getAllReferences(ElementInfo queryElementInfo) {
        long mapIdNum = MapResource.validateMap(queryElementInfo.getMapId());

        // check for query element existence
        Set<Long> elementIds = new HashSet<>();
        elementIds.add(queryElementInfo.getId());

        if ((StringUtils.trimToNull(queryElementInfo.getType()) == null) || !Element.allElementsExist(mapIdNum,
                Element.elementTypeFromString(queryElementInfo.getType()), elementIds)) {
            handleError(new Exception("Element with ID: " + queryElementInfo + " and type: "
                    + queryElementInfo.getType() + " does not exist."), "");
        }

        // select all review relation id's from current relation members where
        // member id = requesting element's member id and the element type = the requesting element type
        List<Long> allReviewRelationIds = getAllReviewRelations(queryElementInfo, mapIdNum);

        List<ReviewRef> references = new ArrayList<>();
        if (!allReviewRelationIds.isEmpty()) {
            // select all relation members where themember's id is not equal to the requesting element's id and the
            // member's type is not = to the requesting element's type
            List<CurrentRelationMembers> referencedMembers = createQuery(mapIdNum)
                    .select(currentRelationMembers)
                    .from(currentRelationMembers)
                    .where(currentRelationMembers.relationId.in(allReviewRelationIds))
                    .orderBy(currentRelationMembers.relationId.asc(),
                            currentRelationMembers.memberId.asc(),
                            currentRelationMembers.sequenceId.asc())
                    .fetch();

            // return all elements corresponding to the filtered down set of relation members
            for (CurrentRelationMembers member : referencedMembers) {
                references.add(new ReviewRef(queryElementInfo.getMapId(), member.getMemberId(),
                        Element.elementTypeForElementEnum(member.getMemberType()).toString().toLowerCase(),
                        member.getRelationId()));
            }
        }

        return references;
    }

    /**
     * Handles all thrown exceptions from review services
     *
     * @param e
     *            a thrown exception
     * @param errorMessageStart
     *            text to prepend to the error message
     * //TODO: go through and clean out these message text checks
     */
    private static void handleError(Exception e, String errorMessageStart) {
        Status status = null;
        if (!StringUtils.isEmpty(e.getMessage())) {
            if (e.getMessage().contains("Invalid input parameter") || e.getMessage().contains("Invalid reviewed item")
                    || e.getMessage().contains("Error parsing unique ID tag") || e.getMessage().contains("empty String")
                    || e.getMessage().contains("Invalid coordinate")) {
                status = Status.BAD_REQUEST;
            }
            else if (e.getMessage().contains("record exists") || e.getMessage().contains("records exist")
                    || e.getMessage().contains("to be updated does not exist")
                    || e.getMessage().contains("does not exist")) {
                status = Status.NOT_FOUND;
            }
            else if (e.getMessage().contains("Invalid version") || e.getMessage().contains("Invalid changeset ID")
                    || e.getMessage().contains("references itself")
                    || e.getMessage().contains("Changeset maximum element threshold exceeded")
                    || e.getMessage().contains("was closed at") || e.getMessage().contains("has become out of sync")) {
                status = Status.CONFLICT;
            }
            else if (e.getMessage().contains("exist specified for") || e.getMessage().contains("exist for")
                    || e.getMessage().contains("is still used by")) {
                status = Status.PRECONDITION_FAILED;
            }
        }

        if (status == null) {
            status = Status.INTERNAL_SERVER_ERROR;
        }

        String message = "Error " + errorMessageStart + ": ";
        if ((e.getMessage() != null) && e.getMessage().contains("empty String")) {
            // added for giving a better error message when passing invalid params to jersey
            message += "Invalid input parameter";
        }
        else {
            message += e.getMessage();
        }

        if (e instanceof SQLException) {
            SQLException sqlException = (SQLException) e;
            if (sqlException.getNextException() != null) {
                message += "  " + sqlException.getNextException().getMessage();
            }
        }

        if (e.getCause() instanceof SQLException) {
            SQLException sqlException = (SQLException) e.getCause();
            if (sqlException.getNextException() != null) {
                message += "  " + sqlException.getNextException().getMessage();
            }
        }

        String exceptionCode = status.getStatusCode() + ": " + status.getReasonPhrase();
        logger.error("{} {}", exceptionCode, message, e);

        throw new WebApplicationException(e, Response.status(status).entity(message).build());
    }
}
