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
package hoot.services.controllers.job;

import static hoot.services.models.db.QReviewBookmarks.reviewBookmarks;
import static hoot.services.utils.DbUtils.createQuery;

import java.util.Calendar;
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
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.models.db.ReviewBookmarks;
import hoot.services.models.review.ReviewBookmarkDelRequest;
import hoot.services.models.review.ReviewBookmarkDelResponse;
import hoot.services.models.review.ReviewBookmarkSaveRequest;
import hoot.services.models.review.ReviewBookmarksGetResponse;
import hoot.services.models.review.ReviewBookmarksSaveResponse;
import hoot.services.models.review.ReviewBookmarksStatResponse;
import hoot.services.readers.review.ReviewBookmarkRetriever;
import hoot.services.utils.PostgresUtils;


/**
 * Service endpoint for the conflated data review process
 */
@Controller
@Path("/review/bookmarks")
@Transactional
public class ReviewBookmarkResource {
    private static final Logger logger = LoggerFactory.getLogger(ReviewBookmarkResource.class);

    @Autowired
    private ReviewBookmarksSaver reviewBookmarksSaver;

    @Autowired
    private ReviewBookmarkRetriever reviewBookmarkRetriever;


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
            JSONObject oDetail = request.getDetail();
            Object oNotes = oDetail.get("bookmarknotes");
            if (oNotes != null) {
                JSONArray aNotes = (JSONArray) oNotes;
                for (Object aNote : aNotes) {
                    JSONObject note = (JSONObject) aNote;
                    if (!note.containsKey("id")) {
                        String sNewId = UUID.randomUUID().toString();
                        sNewId = sNewId.replace('-', '0');
                        note.put("id", sNewId);
                        Calendar calendar = Calendar.getInstance();
                        long now = calendar.getTimeInMillis();
                        note.put("createdAt", now);
                        note.put("modifiedAt", now);
                    }

                    if (!note.containsKey("modifiedAt")) {
                        Calendar calendar = Calendar.getInstance();
                        long now = calendar.getTimeInMillis();
                        note.put("modifiedAt", now);
                    }
                }
            }

            long nSaved = reviewBookmarksSaver.save(request);
            response.setSavedCount(nSaved);
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
                reviewBookmarks = reviewBookmarkRetriever.retrieve(bookmarkId);
            }
            else {
                reviewBookmarks = reviewBookmarkRetriever.retrieve(mapId, relationId);
            }

            for (ReviewBookmarks mk : reviewBookmarks) {
                Object oDetail = mk.getDetail();

                Map<String, String> hstoreMap = PostgresUtils.postgresObjToHStore(oDetail);

                JSONObject oBmkDetail = new JSONObject();

                appendHstoreElement(hstoreMap.get("bookmarkdetail"), oBmkDetail, "bookmarkdetail");

                String bmkNotes = hstoreMap.get("bookmarknotes");
                if ((bmkNotes != null) && (!bmkNotes.isEmpty())) {
                    bmkNotes = bmkNotes.replace("\\\"", "\"");
                    bmkNotes = bmkNotes.replace("\\\\", "\\");
                    JSONParser parser = new JSONParser();
                    JSONArray oParsed = (JSONArray) parser.parse(bmkNotes);
                    oBmkDetail.put("bookmarknotes", oParsed);
                }

                appendHstoreElement(hstoreMap.get("bookmarkreviewitem"), oBmkDetail, "bookmarkreviewitem");

                mk.setDetail(oBmkDetail);
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
    public ReviewBookmarksGetResponse getAllReviewBookmark(@QueryParam("orderBy") String orderByCol,
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

            List<ReviewBookmarks> res = reviewBookmarkRetriever.retrieveAll(orderByCol, isAsc, limit,
                    offsetCnt, creatorArray, layerArray);

            for (ReviewBookmarks mk : res) {
                Object oDetail = mk.getDetail();
                Map<String, String> hstoreMap = PostgresUtils.postgresObjToHStore(oDetail);

                String bmkDetail = hstoreMap.get("bookmarkdetail");
                if ((bmkDetail != null) && (!bmkDetail.isEmpty())) {
                    bmkDetail = bmkDetail.replace("\\\"", "\"");
                    bmkDetail = bmkDetail.replace("\\\\", "\\");
                    JSONParser parser = new JSONParser();
                    JSONObject oParsed = (JSONObject) parser.parse(bmkDetail);

                    JSONObject oBmkDetail = new JSONObject();
                    oBmkDetail.put("bookmarkdetail", oParsed);

                    mk.setDetail(oBmkDetail);
                }
            }
            response.setReviewBookmarks(res);
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
            long bookmarksCount = reviewBookmarkRetriever.getBookmarksCount();
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
        ReviewBookmarkDelRequest request = new ReviewBookmarkDelRequest(bookmarkId);
        ReviewBookmarkDelResponse response = new ReviewBookmarkDelResponse();

        try {
            long count = remove(request);
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
     * @param request
     *            - Request containing mapid and relationid
     * @return - total numbers of removed
     */
    private static long remove(ReviewBookmarkDelRequest request) {
        return createQuery().delete(reviewBookmarks).where(reviewBookmarks.id.eq(request.getBookmarkId())).execute();
    }
}
