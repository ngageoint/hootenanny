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

import static hoot.services.models.db.QReviewBookmarks.reviewBookmarks;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import com.querydsl.core.types.OrderSpecifier;
import com.querydsl.sql.SQLQuery;

import hoot.services.models.db.ReviewBookmarks;
import hoot.services.utils.PostgresUtils;


/**
 * Service endpoint for the conflated data review process
 */
@Controller
@Path("/review/bookmarks")
@Transactional
public class ReviewBookmarkResource {
    private static final Logger logger = LoggerFactory.getLogger(ReviewBookmarkResource.class);

    public ReviewBookmarkResource() {}

    /**
     * To create or update review bookmark
     * 
     * POST hoot-services/job/review/bookmarks/save * { "mapId":1,
     * "relationId":3, "detail": {"k1":"v1","l3":"v3"}, "userId":-1 }
     * 
     * @param request
     *            ReviewBookmarkSaveRequest class
     * @return json containing created/updated bookmark id
     */
    @POST
    @Path("/save")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewBookmarksSaveResponse createReviewBookmark(ReviewBookmarkSaveRequest request) {
        ReviewBookmarksSaveResponse response = new ReviewBookmarksSaveResponse();

        try {
            JSONObject detail = request.getDetail();
            Object notes = detail.get("bookmarknotes");
            if (notes != null) {
                JSONArray jsonArray = (JSONArray) notes;
                for (Object jsonObject : jsonArray) {
                    JSONObject note = (JSONObject) jsonObject;
                    if (!note.containsKey("id")) {
                        String sNewId = UUID.randomUUID().toString();
                        sNewId = sNewId.replace('-', '0');
                        note.put("id", sNewId);
                        long now = System.currentTimeMillis();
                        note.put("createdAt", now);
                        note.put("modifiedAt", now);
                    }

                    if (!note.containsKey("modifiedAt")) {
                        long now = System.currentTimeMillis();
                        note.put("modifiedAt", now);
                    }
                }
            }

            long count = save(request);
            response.setSavedCount(count);
        }
        catch (Exception ex) {
            String msg = "Error saving review bookmark: " + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return response;
    }

    /**
     * To retrieve review bookmark
     * 
     * GET hoot-services/job/review/bookmarks/get?mapId=1&relationId=2 * {
     * "reviewBookmarks": [ { "createdAt": 1453229299354, "createdBy": -1,
     * "detail": { "type": "hstore", "value": ""k1"=>"v1", "l3"=>"v3"" }, "id":
     * 2, "lastModifiedAt": null, "lastModifiedBy": null, "mapId": 1,
     * "relationId": 2 } ] }
     * 
     * @param bookmarkId
     *            bookmark id
     * @param mapId
     *            map Id
     * @param relationId
     *            relation id
     * @return json containing list of review bookmarks
     */
    @GET
    @Path("/get")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewBookmarksGetResponse getReviewBookmark(@QueryParam("bookmarkId") Long bookmarkId,
                                                        @QueryParam("mapId") Long mapId,
                                                        @QueryParam("relationId") Long relationId) {
        ReviewBookmarksGetResponse response = new ReviewBookmarksGetResponse();

        try {
            List<ReviewBookmarks> reviewBookmarks;
            if (bookmarkId != null) {
                reviewBookmarks = retrieveBy(bookmarkId);
            }
            else {
                reviewBookmarks = retrieveBy(mapId, relationId);
            }

            for (ReviewBookmarks reviewBookmark : reviewBookmarks) {
                Object detail = reviewBookmark.getDetail();

                Map<String, String> hstoreMap = PostgresUtils.postgresObjToHStore(detail);

                JSONObject json = new JSONObject();

                // TODO: find out exactly why we need to do this
                appendHstoreElement(hstoreMap.get("bookmarkdetail"), json, "bookmarkdetail");

                // TODO: find out exactly why we need to do this
                String bmkNotes = hstoreMap.get("bookmarknotes");
                if ((bmkNotes != null) && (!bmkNotes.isEmpty())) {
                    bmkNotes = bmkNotes.replace("\\\"", "\"");
                    bmkNotes = bmkNotes.replace("\\\\", "\\");
                    JSONParser parser = new JSONParser();
                    JSONArray jsonArray = (JSONArray) parser.parse(bmkNotes);
                    json.put("bookmarknotes", jsonArray);
                }

                // TODO: find out exactly why we need to do this
                appendHstoreElement(hstoreMap.get("bookmarkreviewitem"), json, "bookmarkreviewitem");

                reviewBookmark.setDetail(json);
            }

            response.setReviewBookmarks(reviewBookmarks);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error getting review bookmark: " + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return response;
    }

    /**
     * Helper function to handle JSON string conversion to Hstore friendly format
     *
     * @throws ParseException
     */
    private static void appendHstoreElement(String rawElem, JSONObject oBmkDetail, String elemName)
            throws ParseException {
        String bmkElem = rawElem;
        if ((bmkElem != null) && (!bmkElem.isEmpty())) {
            bmkElem = bmkElem.replace("\\\"", "\"");
            bmkElem = bmkElem.replace("\\\\", "\\");
            JSONParser parser = new JSONParser();
            JSONObject oParsed = (JSONObject) parser.parse(bmkElem);
            oBmkDetail.put(elemName, oParsed);
        }
    }

    /**
     * To retrieve all review bookmarks
     * 
     * GET
     * hoot-services/job/review/bookmarks/getall?orderBy=createdAt&asc=false&
     * limit=2&offset=1 * { "reviewBookmarks": [ { "createdAt": 1453229299354,
     * "createdBy": -1, "detail": { "type": "hstore", "value": ""k1"=>"v1", "
     * l3"=>"v3"" }, "id": 2, "lastModifiedAt": null, "lastModifiedBy": null,
     * "mapId": 1, "relationId": 2 } ] }
     * 
     * @param orderByCol
     *            order by column [createdAt | createdBy | id | lastModifiedAt |
     *            lastModifiedBy | mapId | relationId]
     * @param asc
     *            is ascending [true | false]
     * @param limitSize
     *            Limit count for paging
     * @param offset
     *            offset index for paging
     * @param filterByCreatedVal
     *            ?
     * @param filterByLayerVal
     *            ?     *
     * @return json containing list of review bookmarks
     */
    @GET
    @Path("/getall")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewBookmarksGetResponse getAllReviewBookmarks(@QueryParam("orderBy") String orderByCol,
                                                            @QueryParam("asc") String asc,
                                                            @QueryParam("limit") String limitSize,
                                                            @QueryParam("offset") String offset,
                                                            @QueryParam("filterby") String filterBy,
                                                            @QueryParam("filterbyval") String filterByVal,
                                                            @QueryParam("createFilterVal") String filterByCreatedVal,
                                                            @QueryParam("layerFilterVal") String filterByLayerVal) {
        ReviewBookmarksGetResponse response = new ReviewBookmarksGetResponse();

        try {
            boolean isAsc = true;
            if (asc != null) {
                isAsc = (asc.equalsIgnoreCase("true"));
            }

            long limit = -1;

            if (limitSize != null) {
                limit = Long.parseLong(limitSize);
            }

            long offsetCnt = -1;
            if (offset != null) {
                offsetCnt = Long.parseLong(offset);
            }

            Long[] creatorArray = null;
            if(filterByCreatedVal != null){
                String[] cA = filterByCreatedVal.split(",");
                if(cA.length > 0){
                    creatorArray = new Long[cA.length];
                    for(int i = 0; i < creatorArray.length; i++){
                        creatorArray[i] = Long.valueOf(cA[i]);
                    }
                }
            }

            Long[] layerArray = null;
            if(filterByLayerVal != null){
                String[] lA = filterByLayerVal.split(",");
                if(lA.length > 0){
                    layerArray = new Long[lA.length];
                    for(int i = 0; i < layerArray.length; i++){
                        layerArray[i] = Long.valueOf(lA[i]);
                    }
                }
            }

            List<ReviewBookmarks> reviewBookmarks = retrieveAll(orderByCol, isAsc, limit, offsetCnt, creatorArray, layerArray);

            for (ReviewBookmarks reviewBookmark : reviewBookmarks) {
                Object oDetail = reviewBookmark.getDetail();
                Map<String, String> hstoreMap = PostgresUtils.postgresObjToHStore(oDetail);

                JSONObject json = new JSONObject();

                // TODO: find out exactly why we need to do this
                appendHstoreElement(hstoreMap.get("bookmarkdetail"), json, "bookmarkdetail");

                reviewBookmark.setDetail(json);
            }

            response.setReviewBookmarks(reviewBookmarks);
        }
        catch (Exception ex) {
            String msg = "Error getting review bookmark: " + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return response;
    }

    /**
     * To retrieve review bookmarks stat
     * 
     * GET hoot-services/job/review/bookmarks/stat
     * 
     * @return json stat info
     */
    @GET
    @Path("/stat")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewBookmarksStatResponse getAllReviewBookmarkStat() {
        ReviewBookmarksStatResponse response = new ReviewBookmarksStatResponse();

        try {
            long bookmarksCount = getBookmarksCount();
            response.setTotalCount(bookmarksCount);
        }
        catch (Exception ex) {
            String msg = "Error getting review bookmark counts: " + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return response;
    }

    /**
     * To delete review bookmark
     * 
     * DELETE hoot-services/job/review/bookmarks/delete { "mapId":397,
     * "relationId":3 }
     * 
     * @param bookmarkId
     *            id of the bookmark to delete
     * @return json containing total numbers of deleted
     */
    @DELETE
    @Path("/delete")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewBookmarkDelResponse delReviewBookmark(@QueryParam("bookmarkId") Long bookmarkId) {
        ReviewBookmarkDelResponse response = new ReviewBookmarkDelResponse();

        try {
            long count = remove(bookmarkId);
            response.setDeleteCount(count);
        }
        catch (Exception ex) {
            String msg = "Error deleting review bookmark: " + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return response;
    }

    /**
     * Removes review tag.
     *
     * @param bookmarkId
     *            - bookmark ID
     * @return - total numbers of removed
     */
    private static long remove(Long bookmarkId) {
        return createQuery().delete(reviewBookmarks).where(reviewBookmarks.id.eq(bookmarkId)).execute();
    }

    private static List<ReviewBookmarks> retrieveBy(long mapId, long relationId) {
        return createQuery()
                .select(reviewBookmarks)
                .from(reviewBookmarks)
                .where(reviewBookmarks.mapId.eq(mapId).and(reviewBookmarks.relationId.eq(relationId)))
                .fetch();
    }

    private static List<ReviewBookmarks> retrieveBy(long bookmarkId) {
        return createQuery()
                .select(reviewBookmarks)
                .from(reviewBookmarks)
                .where(reviewBookmarks.id.eq(bookmarkId))
                .fetch();
    }

    /**
     * Retrieves all review tags
     *
     * @param orderByCol
     *            - order by column to sort
     * @param isAsc
     *            - is order by asc | desc
     * @param limit
     *            - limit for numbers of returned results
     * @param offset
     *            - offset row for paging
     * @return - list of Review tags
     */
    private static List<ReviewBookmarks> retrieveAll(String orderByCol, boolean isAsc, long limit,
            long offset, Long[] creatorArray, Long[] layerArray) {
        SQLQuery<ReviewBookmarks> query = createQuery().query().select(reviewBookmarks).from(reviewBookmarks);

        if ((creatorArray != null) && (layerArray != null)) {
            query.where(reviewBookmarks.createdBy.in((Number[]) creatorArray)
                    .and(reviewBookmarks.mapId.in((Number[]) layerArray)));
        }
        else if ((creatorArray != null) && (layerArray == null)) {
            query.where(reviewBookmarks.createdBy.in((Number[]) creatorArray));
        }
        else if ((creatorArray == null) && (layerArray != null)) {
            query.where(reviewBookmarks.mapId.in((Number[]) layerArray));
        }
        else {
            query.from(reviewBookmarks);
        }

        query.orderBy(getSpecifier(orderByCol, isAsc));

        if (limit > -1) {
            query.limit(limit);
        }

        if (offset > -1) {
            query.offset(offset);
        }

        return query.fetch();
    }

    /**
     * Get the total counts of all review tags
     *
     * @return - numbers of toal count
     */
    private static long getBookmarksCount() {
        return createQuery().from(reviewBookmarks).fetchCount();
    }


    /**
     * Filter for allowed columns for order by
     *
     * @param orderByCol
     *            - String representation of order by column
     * @param isAsc
     *            - asc | dsc
     * @return - OrderSpecifier
     */
    private static OrderSpecifier<?> getSpecifier(String orderByCol, boolean isAsc) {
        OrderSpecifier<?> order = reviewBookmarks.id.asc();
        if (orderByCol != null) {
            switch (orderByCol) {
                case "id":
                    order = (isAsc) ? reviewBookmarks.id.asc() : reviewBookmarks.id.desc();
                    break;
                case "createdAt":
                    order = (isAsc) ? reviewBookmarks.createdAt.asc() : reviewBookmarks.createdAt.desc();
                    break;
                case "createdBy":
                    order = (isAsc) ? reviewBookmarks.createdBy.asc() : reviewBookmarks.createdBy.desc();
                    break;
                case "lastModifiedAt":
                    order = (isAsc) ? reviewBookmarks.lastModifiedAt.asc() : reviewBookmarks.lastModifiedAt.desc();
                    break;
                case "lastModifiedBy":
                    order = (isAsc) ? reviewBookmarks.lastModifiedBy.asc() : reviewBookmarks.lastModifiedBy.desc();
                    break;
                case "mapId":
                    order = (isAsc) ? reviewBookmarks.mapId.asc() : reviewBookmarks.mapId.desc();
                    break;
                case "relationId":
                    order = (isAsc) ? reviewBookmarks.relationId.asc() : reviewBookmarks.relationId.desc();
                    break;
                default:
                    order = reviewBookmarks.id.asc();
                    break;
            }
        }

        return order;
    }

    /**
     * Saves review tags. It first checks to see if exists and if not insert else update
     *
     * @param request
     *            - request object containing inserted/updated fields
     * @return - numbers of saved tags
     */
    private static long save(ReviewBookmarkSaveRequest request) {
        long savedCount;

        if (request.getBookmarkId() > -1) {
            List<ReviewBookmarks> result = retrieveBy(request.getBookmarkId());
            savedCount = result.isEmpty() ? insert(request) : update(request, result.get(0));
        }
        else {
            // insert
            savedCount = insert(request);
        }

        return savedCount;
    }

    /**
     * Inserts review tag into database
     *
     * @param request
     *            - request object containing inserted fields
     * @return - total numbers of inserted
     */
    private static long insert(ReviewBookmarkSaveRequest request) {
        Timestamp now = new Timestamp(System.currentTimeMillis());

        return createQuery()
                .insert(reviewBookmarks)
                .columns(reviewBookmarks.mapId, reviewBookmarks.relationId, reviewBookmarks.createdAt,
                        reviewBookmarks.createdBy, reviewBookmarks.detail)
                .values(request.getMapId(), request.getRelationId(), now, request.getUserId(),
                        PostgresUtils.jsonToHStore(request.getDetail()))
                .execute();
    }

    /**
     * Updates review tag.
     *
     * @param request
     *            - request object containing updated fields
     * @param reviewBookmark
     *            - Current review tag
     * @return total numbers of updated
     */
    private static long update(ReviewBookmarkSaveRequest request, ReviewBookmarks reviewBookmark) {
        Timestamp now = new Timestamp(System.currentTimeMillis());

        reviewBookmark.setLastModifiedAt(now);
        reviewBookmark.setLastModifiedBy(request.getUserId());
        reviewBookmark.setDetail(PostgresUtils.jsonToHStore(request.getDetail()));

        return createQuery()
                .update(reviewBookmarks)
                .populate(reviewBookmark)
                .where(reviewBookmarks.id.eq(reviewBookmark.getId()))
                .execute();
    }
}
