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

import java.sql.Connection;
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
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.db.DbUtils;
import hoot.services.db.postgres.PostgresUtils;
import hoot.services.db2.ReviewBookmarks;
import hoot.services.models.review.ReviewBookmarkDelRequest;
import hoot.services.models.review.ReviewBookmarkDelResponse;
import hoot.services.models.review.ReviewBookmarkSaveRequest;
import hoot.services.models.review.ReviewBookmarksGetResponse;
import hoot.services.models.review.ReviewBookmarksSaveResponse;
import hoot.services.models.review.ReviewBookmarksStatResponse;
import hoot.services.readers.review.ReviewBookmarkRetriever;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.writers.review.ReviewBookmarksRemover;
import hoot.services.writers.review.ReviewBookmarksSaver;


/**
 * Service endpoint for the conflated data review process
 */
@Path("/review/bookmarks")
public class ReviewBookmarkResource {
    private static final Logger log = LoggerFactory.getLogger(ReviewBookmarkResource.class);

    public ReviewBookmarkResource() throws Exception {
    }

    /**
     * To create or update review bookmark
     * 
     * POST hoot-services/job/review/bookmarks/save * { "mapId":1,
     * "relationId":3, "detail": {"k1":"v1","l3":"v3"}, "userId":-1 }
     * 
     * @param request
     *            ReviewBookmarkSaveRequest class
     * @return json containing created/updated bookmark id
     * @throws Exception
     */
    @POST
    @Path("/save")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewBookmarksSaveResponse createReviewBookmark(final ReviewBookmarkSaveRequest request) throws Exception {

        ReviewBookmarksSaveResponse response = new ReviewBookmarksSaveResponse();

        try (Connection conn = DbUtils.createConnection()) {
            JSONObject oDetail = request.getDetail();
            Object oNotes = oDetail.get("bookmarknotes");
            if (oNotes != null) {

                JSONArray aNotes = (JSONArray) oNotes;

                for (int i = 0; i < aNotes.size(); i++) {
                    JSONObject note = (JSONObject) aNotes.get(i);
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
            ReviewBookmarksSaver saver = new ReviewBookmarksSaver(conn);
            long nSaved = saver.save(request);
            response.setSavedCount(nSaved);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error saving review bookmark: " + " (" + ex.getMessage() + ")",
                    Status.BAD_REQUEST, log);
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
     * @param bookmarkid
     *            bookmark id
     * @param mapid
     *            map Id
     * @param relid
     *            relation id
     * @return json containing list of review bookmarks
     * @throws Exception
     */
    @GET
    @Path("/get")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewBookmarksGetResponse getReviewBookmark(@QueryParam("bookmarkId") String bookmarkid,
            @QueryParam("mapId") String mapid, @QueryParam("relationId") String relid) throws Exception {
        ReviewBookmarksGetResponse response = new ReviewBookmarksGetResponse();

        try (Connection conn = DbUtils.createConnection()) {
            ReviewBookmarkRetriever retriever = new ReviewBookmarkRetriever(conn);
            List<ReviewBookmarks> res = null;
            if (bookmarkid != null) {
                long bookmarkId = Long.parseLong(bookmarkid);
                res = retriever.retrieve(bookmarkId);
            }
            else {
                long mapId = Long.parseLong(mapid);
                long relationId = Long.parseLong(relid);
                res = retriever.retrieve(mapId, relationId);
            }

            for (ReviewBookmarks mk : res) {
                Object oDetail = mk.getDetail();
                Map<String, String> hstoreMap = PostgresUtils
                        .postgresObjToHStore((org.postgresql.util.PGobject) oDetail);
                JSONObject oBmkDetail = new JSONObject();
                _appendHstoreElement(hstoreMap.get("bookmarkdetail"), oBmkDetail, "bookmarkdetail");

                String bmkNotes = hstoreMap.get("bookmarknotes");
                if (bmkNotes != null && bmkNotes.length() > 0) {
                    bmkNotes = bmkNotes.replace("\\\"", "\"");
                    bmkNotes = bmkNotes.replace("\\\\", "\\");
                    JSONParser parser = new JSONParser();
                    JSONArray oParsed = (JSONArray) parser.parse(bmkNotes);

                    oBmkDetail.put("bookmarknotes", oParsed);
                }

                _appendHstoreElement(hstoreMap.get("bookmarkreviewitem"), oBmkDetail, "bookmarkreviewitem");

                assert (oBmkDetail != null);
                mk.setDetail(oBmkDetail);
            }

            response.setReviewBookmarks(res);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting review bookmark: " + " (" + ex.getMessage() + ")",
                    Status.BAD_REQUEST, log);
        }
        return response;
    }

    /**
     * Helper function to handle JSON string conversion to Hstore friendly
     * format
     * 
     * @throws Exception
     */
    protected void _appendHstoreElement(final String rawElem, final JSONObject oBmkDetail, final String elemName)
            throws Exception {
        String bmkElem = rawElem;
        if (bmkElem != null && bmkElem.length() > 0) {
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
     * @param filterBy
     *            ?
     * @param filterByVal
     *            ?
     * @return json containing list of review bookmarks
     * @throws Exception
     */
    @GET
    @Path("/getall")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewBookmarksGetResponse getAllReviewBookmark(@QueryParam("orderBy") String orderByCol,
            @QueryParam("asc") String asc, @QueryParam("limit") String limitSize, @QueryParam("offset") String offset,
            @QueryParam("filterby") String filterBy, @QueryParam("filterbyval") String filterByVal) throws Exception {
        ReviewBookmarksGetResponse response = new ReviewBookmarksGetResponse();

        try (Connection conn = DbUtils.createConnection()) {

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

            String filterByCol = null;
            Long filterVal = null;

            if (filterBy != null && filterBy.length() > 0 && filterByVal != null && filterByVal.length() > 0) {
                if (filterBy.equalsIgnoreCase("createdBy")) {
                    filterByCol = "createdBy";
                    filterVal = Long.parseLong(filterByVal);
                }
                else if (filterBy.equalsIgnoreCase("mapId")) {
                    filterByCol = "mapId";
                    filterVal = Long.parseLong(filterByVal);
                }
            }

            ReviewBookmarkRetriever retriever = new ReviewBookmarkRetriever(conn);
            List<ReviewBookmarks> res = retriever.retrieveAll(orderByCol, isAsc, limit, offsetCnt, filterByCol,
                    filterVal);

            for (ReviewBookmarks mk : res) {
                Object oDetail = mk.getDetail();
                Map<String, String> hstoreMap = PostgresUtils
                        .postgresObjToHStore((org.postgresql.util.PGobject) oDetail);

                String bmkDetail = hstoreMap.get("bookmarkdetail");
                if (bmkDetail != null && bmkDetail.length() > 0) {
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
            ResourceErrorHandler.handleError("Error getting review bookmark: " + " (" + ex.getMessage() + ")",
                    Status.BAD_REQUEST, log);
        }
        return response;
    }

    /**
     * To retrieve review bookmarks stat
     * 
     * GET hoot-services/job/review/bookmarks/stat
     * 
     * @return json stat info
     * @throws Exception
     */
    @GET
    @Path("/stat")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewBookmarksStatResponse getAllReviewBookmarkStat() throws Exception {
        ReviewBookmarksStatResponse response = new ReviewBookmarksStatResponse();
        try (Connection conn = DbUtils.createConnection()) {
            ReviewBookmarkRetriever retriever = new ReviewBookmarkRetriever(conn);
            long nCnt = retriever.getbookmarksCount();
            response.setTotalCount(nCnt);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting review bookmark counts: " + " (" + ex.getMessage() + ")",
                    Status.BAD_REQUEST, log);
        }
        return response;
    }

    /**
     * To delete review bookmark
     * 
     * DELETE hoot-services/job/review/bookmarks/delete { "mapId":397,
     * "relationId":3 }
     * 
     * @param bmkId
     *            id of the bookmark to delete
     * @return json containing total numbers of deleted
     * @throws Exception
     */
    @DELETE
    @Path("/delete")
    @Produces(MediaType.APPLICATION_JSON)
    public ReviewBookmarkDelResponse delReviewBookmark(@QueryParam("bookmarkId") final String bmkId) throws Exception {
        ReviewBookmarkDelRequest request = new ReviewBookmarkDelRequest(Long.parseLong(bmkId));
        ReviewBookmarkDelResponse response = new ReviewBookmarkDelResponse();

        try (Connection conn = DbUtils.createConnection()) {
            ReviewBookmarksRemover remover = new ReviewBookmarksRemover(conn);
            long nDel = remover.remove(request);
            response.setDeleteCount(nDel);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error deleting review bookmark: " + " (" + ex.getMessage() + ")",
                    Status.BAD_REQUEST, log);
        }
        return response;
    }
}
