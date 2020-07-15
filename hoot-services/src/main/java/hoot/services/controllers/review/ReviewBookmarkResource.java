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
 * @copyright Copyright (C) 2016, 2017, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.review;

import static hoot.services.models.db.QFolderMapMappings.folderMapMappings;
import static hoot.services.models.db.QFolders.folders;
import static hoot.services.models.db.QMaps.maps;
import static hoot.services.models.db.QReviewBookmarks.reviewBookmarks;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import com.querydsl.core.types.OrderSpecifier;
import com.querydsl.core.types.dsl.BooleanExpression;
import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.SQLQuery;

import hoot.services.controllers.osm.user.UserResource;
import hoot.services.models.db.QMaps;
import hoot.services.models.db.QUsers;
import hoot.services.models.db.ReviewBookmarks;
import hoot.services.models.db.Users;
import hoot.services.utils.DbUtils;
import hoot.services.utils.PostgresUtils;


/**
 * Service endpoint for the conflated data review process
 */
@Controller
@Path("/review/bookmarks")
@Transactional
public class ReviewBookmarkResource {

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
                    JSONParser parser = new JSONParser();
                    JSONArray jsonArray = (JSONArray) parser.parse(PostgresUtils.unescapeJson(bmkNotes));
                    json.put("bookmarknotes", jsonArray);
                }

                // TODO: find out exactly why we need to do this
                appendHstoreElement(hstoreMap.get("bookmarkreviewitem"), json, "bookmarkreviewitem");

                JSONParser parser = new JSONParser();
                String usersArray = hstoreMap.get("taggedUsers");
                if(usersArray != null) {
                    JSONArray jsonArray = (JSONArray) parser.parse(hstoreMap.get("taggedUsers"));
                    json.put("taggedUsers", jsonArray);
                }

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
            JSONParser parser = new JSONParser();
            JSONObject oParsed = (JSONObject) parser.parse(PostgresUtils.unescapeJson(bmkElem));
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
     * @param limitSize
     *            Limit count for paging
     * @param offset
     *            offset index for paging
     * @param creatorFilter
     *            id of creator user to filter by
     * @param layerNameFilter
     *            id of layer to filter by
     * @return json containing list of review bookmarks
     */
    @GET
    @Path("/getall")
    @Produces(MediaType.APPLICATION_JSON)
    public JSONObject getAllReviewBookmarks(@Context HttpServletRequest request,
            @QueryParam("limit") String limitSize,
            @QueryParam("orderBy") @DefaultValue("") String orderByCol,
            @QueryParam("creatorFilter") String creatorFilter,
            @QueryParam("layerNameFilter") String layerNameFilter,
            @QueryParam("offset") @DefaultValue("0") String offset,
            @QueryParam("showTagged") boolean showTagged) {

        Users user = Users.fromRequest(request);
        long userId = user.getId();
        JSONObject response = new JSONObject();

        try {
            long limit = -1;
            if (limitSize != null && !limitSize.equals("")) {
                limit = Long.parseLong(limitSize);
            }

            long offsetCnt = -1;
            if (offset != null && !offset.equals("")) {
                offsetCnt = Long.parseLong(offset);
            }

            long creatorId = -1;
            if(creatorFilter != null && !creatorFilter.equals("")){
                creatorId = Long.parseLong(creatorFilter);
            }

            long layerId = -1;
            if(layerNameFilter != null && !layerNameFilter.equals("")) {
                layerId = Long.parseLong(layerNameFilter);
            }

            SQLQuery<ReviewBookmarks> getQuery = retrieveAll(userId, orderByCol, limit, offsetCnt, creatorId, layerId, showTagged);
            List<ReviewBookmarks> reviewBookmarks = getQuery.fetch();

            for (ReviewBookmarks reviewBookmark : reviewBookmarks) {
                Object oDetail = reviewBookmark.getDetail();
                Map<String, String> hstoreMap = PostgresUtils.postgresObjToHStore(oDetail);

                JSONObject json = new JSONObject();

                // TODO: find out exactly why we need to do this
                appendHstoreElement(hstoreMap.get("bookmarkdetail"), json, "bookmarkdetail");

                reviewBookmark.setDetail(json);
            }
            response.put("reviewBookmarks", reviewBookmarks);

            SQLQuery filteredBookmarkQuery = retrieveAll(userId, orderByCol, -1, -1, creatorId, layerId, showTagged);
            response.put("totalCount", filteredBookmarkQuery.fetchCount());

            List<String> creators = getUsers();
            response.put("creators", creators);

            List<String> layers = getLayers(userId);
            response.put("layerNames", layers);
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
     * @param limit
     *            - limit for numbers of returned results
     * @param offset
     *            - offset row for paging
     * @return - list of Review tags
     */
    private static SQLQuery<ReviewBookmarks> retrieveAll(long userId, String orderByCol, long limit, long offset, long creator, long layer, boolean showTagged) {
        SQLQuery<ReviewBookmarks> query = createQuery()
                .select(reviewBookmarks)
                .from(reviewBookmarks)
                .leftJoin(QMaps.maps).on(QMaps.maps.id.eq(reviewBookmarks.mapId));

        if ((creator != -1) && (layer != -1)) {
            query.where(reviewBookmarks.createdBy.eq(creator)
                    .and(reviewBookmarks.mapId.eq(layer)));
        }
        else if (creator != -1) {
            query.where(reviewBookmarks.createdBy.eq(creator));
        }
        else if (layer != -1) {
            query.where(reviewBookmarks.mapId.eq(layer));
        }
        else {
            query.from(reviewBookmarks);
        }

        // adds on to where clause a check to see if current user is able to see the maps associated with the bookmarks
        if (!UserResource.adminUserCheck(DbUtils.getUser(userId))) {
            query.leftJoin(folderMapMappings).on(folderMapMappings.mapId.eq(maps.id))
                    .leftJoin(folders).on(folders.id.eq(folderMapMappings.folderId));

            BooleanExpression isVisible = maps.userId.eq(userId) // Owned by the current user
                .or(folderMapMappings.id.isNull().or(folderMapMappings.folderId.eq(0L)) // or not in a folder
                .or(folders.publicCol.isTrue())); // or in a public folder

            query.where(isVisible);
        }

        if (showTagged) {
            query.where(Expressions.booleanTemplate("{0} = ANY(TRANSLATE((detail->'taggedUsers')::jsonb::text, '[]','{}')::INT[])", userId));
        }

        query.orderBy(getSpecifier(orderByCol, true));

        if (limit > -1) {
            query.limit(limit);
        }

        if (offset > -1) {
            query.offset(offset);
        }

        return query;
    }

    /**
     * Retrieves all the creators that have a review bookmark
     * @return all the creators that have a review bookmark
     */
    private static List<String> getUsers() {
        SQLQuery<String> userNamesQuery = createQuery()
                .select(QUsers.users.displayName)
                .from(reviewBookmarks)
                .leftJoin(QUsers.users).on(QUsers.users.id.eq(reviewBookmarks.createdBy))
                .groupBy(QUsers.users.displayName)
                .orderBy(QUsers.users.displayName.asc());

        return userNamesQuery.fetch();
    }

    /**
     * Retrieves all the layer names that have a review bookmark and that the specified user can access
     * @return all the layer names that have a review bookmark
     */
    private static List<String> getLayers(long userId) {
        SQLQuery<String> layerNamesQuery = createQuery()
                .select(maps.displayName)
                .from(reviewBookmarks)
                .leftJoin(maps).on(maps.id.eq(reviewBookmarks.mapId))
                .leftJoin(folderMapMappings).on(folderMapMappings.mapId.eq(maps.id))
                .leftJoin(folders).on(folders.id.eq(folderMapMappings.folderId))
                .groupBy(maps.displayName)
                .orderBy(maps.displayName.asc());

        // Current user should only be able to see another users bookmarks if they created the map or the map is public
        if (!UserResource.adminUserCheck(DbUtils.getUser(userId))) {
            BooleanExpression isVisible = maps.userId.eq(userId) // Owned by the current user
                .or(folderMapMappings.id.isNull().or(folderMapMappings.folderId.eq(0L)) // or not in a folder
                .or(folders.publicCol.isTrue())); // or in a public folder

            layerNamesQuery.where(isVisible);
        }

        return layerNamesQuery.fetch();
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
