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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.osm.map;

import static hoot.services.models.db.QFolderMapMappings.folderMapMappings;
import static hoot.services.models.db.QFolders.folders;
import static hoot.services.models.db.QMaps.maps;
import static hoot.services.utils.DbUtils.createQuery;
import static hoot.services.utils.DbUtils.getConnection;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.BadRequestException;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.ForbiddenException;
import javax.ws.rs.GET;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import com.querydsl.core.Tuple;
import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.SQLQuery;

import hoot.services.controllers.osm.user.UserResource;
import hoot.services.models.db.FolderMapMappings;
import hoot.services.models.db.Folders;
import hoot.services.models.db.Users;
import hoot.services.utils.DbUtils;


@Controller
@Path("/api/0.6/map/folders")
@Transactional
public class FolderResource {
    private static final Logger logger = LoggerFactory.getLogger(FolderResource.class);

    public static boolean folderIsPublic(List<Folders> folders, Folders f, Users user) {
        // If the user is an admin
        if(UserResource.adminUserCheck(user)) {
            return true;
        }
        // If its public & attached to root (0)
        if(f.isPublic() && f.getParentId() != null && f.getParentId().equals(0L)) {
            return true;
        }
        // if we have a user, and folder is private, but its owned by this user:
        if(user != null && f.isPrivate() && f.getUserId().equals(user.getId())) {
            return true;
        }

        // Look for this folder's parent in the list:
        Folders parentFolder = null;
        for(Folders currentFolder : folders) {
            // if we find it, stop:
            if(f.getParentId() != null && f.getParentId().equals(currentFolder.getId())) {
                parentFolder = currentFolder;
                break;
            }
        }
        // We found the parent in the list, recurse down:
        if(parentFolder != null) {
            return folderIsPublic(folders, parentFolder, user);
        }

        // We did not find the parent, this means that the parent
        // was not visible to us -so- neither should this folder be:
        return false;
    }

    static public List<Folders> getFoldersForUser(Users user) {
        List<Folders> folderRecordSet = DbUtils.getFoldersForUser(user);
        List<Folders> out = new ArrayList<Folders>();
        for(Folders f : folderRecordSet) {
            if(folderIsPublic(folderRecordSet, f, user)) {
                out.add(f);
            }
        }
        return out;
    }

    /**
     * Returns a list of all folders in the services database.
     *
     * GET hoot-services/osm/api/0.6/map/folders
     *
     * @return a JSON object containing a list of folders
     */
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public FolderRecords getFolders(@Context HttpServletRequest request) {
        Users user = Users.fromRequest(request);
        return mapFolderRecordsToFolders(getFoldersForUser(user));
    }

    /**
     * Returns a list of all folders in the services database
     *
     * GET hoot-services/osm/api/0.6/map/folders/linked
     *
     * @return a JSON object containing a list of folders
     */
    @GET
    @Path("/linked")
    @Produces(MediaType.APPLICATION_JSON)
    public LinkRecords getLinks(@Context HttpServletRequest request) {
        Users user = Users.fromRequest(request);

        SQLQuery<FolderMapMappings> sql = createQuery()
            .select(folderMapMappings)
            .from(folderMapMappings)
            .leftJoin(folders).on(folders.id.eq(folderMapMappings.folderId))
            .where(folders.id.ne(0L));

        if (user != null && !UserResource.adminUserCheck(user)) {
            // public or folder owned by current user or user is admin
            sql.where(folders.publicCol.isTrue().or(folders.userId.eq(user.getId())));
        }

        List<FolderMapMappings> links = sql.orderBy(folderMapMappings.folderId.asc()).fetch();
        List<FolderMapMappings> linksOut = new ArrayList<>(links.size());

        // The above query is only a rough filter
        Set<Long> foldersUserCanSee = DbUtils.getFolderIdsForUser(user);
        for(FolderMapMappings link : links) {
            if(foldersUserCanSee.contains(link.getFolderId())) {
                linksOut.add(link);
            }
        }

        return mapLinkRecordsToLinks(linksOut);
    }

    /**
     * Adds new dataset folder
     *
     * POST hoot-services/osm/api/0.6/map/folders/add/{parent}/{name}
     *
     * @param folderName
     *            Display name of folder
     * @param parentId
     *            The parent folder of the new folder. If at root level, is
     *            equal to 0.
     * @param isPublic
     *            Default derived from parent
     * @return jobId Success = True/False
     */
    @POST
    @Path("/add/{parentId : \\d+}/{folderName}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response addFolder(@Context HttpServletRequest request, @PathParam("folderName") String folderName,
            @PathParam("parentId") Long parentId,
            @QueryParam("isPublic") Boolean isPublic) throws SQLException {
        Users user = Users.fromRequest(request);
        Long userid = -1L;
        if(user != null) {
            userid = user.getId();
        }

        // The user should own the entire folder chain, in the case of root
        // we will get a list w/ a single folder.
        List<Folders> folderChain = getFolderChainForUser(user, parentId);
        Folders parentFolder = folderFromList(folderChain, parentId);

        // If the API user didn't specify a visibility level, inherit from the parent
        // folder:
        if(isPublic == null) {
            isPublic = parentFolder.isPublic();
        // If the user did specify verify visibility:
        } else {
            if(isPublic && parentFolder.isPrivate()) {
                throw new BadRequestException("public folders cannot be created under private folders");
            }
            // don't allow private folders to be create under public folders -except- root.
            if(!isPublic && parentFolder.isPublic() && !parentFolder.getId().equals(0L)) {
                throw new BadRequestException("private folders cannot be create under public folders");
            }
        }

        Long newId = createQuery()
                .select(Expressions.numberTemplate(Long.class, "nextval('folders_id_seq')"))
                .from()
                .fetchOne();

        Timestamp now = new Timestamp(System.currentTimeMillis());


        createQuery()
                .insert(folders).columns(folders.id, folders.createdAt, folders.displayName, folders.publicCol, folders.userId, folders.parentId)
                .values(newId, now, folderName, isPublic, userid, parentId).execute();

        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        ret.put("success", true);
        ret.put("folderId", newId);

        return Response.ok().entity(ret).build();
    }

    /**
     * Deletes a dataset folder.
     *
     * POST hoot-services/osm/api/0.6/map/folder/delete/{id}
     *
     *
     * @param folderId
     *            Folder Id
     * @return jobId
     */
    @DELETE
    @Path("/{folderId : \\d+}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response deleteFolder(@Context HttpServletRequest request, @PathParam("folderId") Long folderId) {
        if(folderId.equals(0L)) {
            throw new BadRequestException();
        }
        Users user = Users.fromRequest(request);
        Folders folder = getFolderForUser(user, folderId);

        if(user != null && !folder.getUserId().equals(user.getId())) {
            throw new ForbiddenException(Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must own the folder to delete it").build());
        }

        createQuery()
            .update(folders)
            .where(folders.parentId.eq(folderId))
            .set(folders.parentId, folder.getParentId()) // move stuff in this folder to -this folder's- parent.
            .execute();

        createQuery()
            .delete(folders)
            .where(folders.id.eq(folderId))
            .execute();

        createQuery()
            .update(folderMapMappings)
            .where(folderMapMappings.folderId.eq(folderId))
            .set(folderMapMappings.folderId, folder.getParentId()) // Update mappings for objects inside this folder
            .execute();

        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        ret.put("success", true);
        return Response.ok().entity(ret).build();
    }

    /**
     *
     * POST hoot-services/osm/api/0.6/map/folders/{folderId}/move/{newParentFolderId}
     *
     *
     * @param folderId
     *            ID of folder
     * @param newParentFolderId
     *            ID of another folder
     * @return jobId Success = True/False
     */
    @PUT
    @Path("/{folderId : \\d+}/move/{newParentFolderId : \\d+}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response updateParentId(@Context HttpServletRequest request,
            @PathParam("folderId") Long folderId,
            @PathParam("newParentFolderId") Long newParentFolderId) {
        if(folderId.equals(0L)) {
            throw new BadRequestException();
        } else if(folderId.equals(newParentFolderId)) {
            return Response.status(Response.Status.BAD_REQUEST).entity("The new parent folder cannot be the current folder").build();
        }
        Users user = Users.fromRequest(request);
        // handle some ACL logic:
        Folders targetFolder = getFolderForUser(user, folderId);
        Folders parentFolder = getFolderForUser(user, newParentFolderId);

        if(user != null && (
                !targetFolder.getUserId().equals(user.getId())
                ||
                !parentFolder.getUserId().equals(user.getId())
        )) {
            throw new ForbiddenException(Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must own both folders to move it").build());
        }


        DbUtils.setFolderParent(folderId, parentFolder);

        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        ret.put("success", true);
        return Response.ok().entity(ret).build();
    }

    /**
     *
     * PUT hoot-services/osm/api/0.6/map/folders/modify/123456/New Dataset
     *
     *
     * @param folderId
     *            ID of map record or folder to be modified
     * @param modName
     *            The new name for the dataset
     * @return jobId Success = True/False
     */
    @PUT
    @Path("/{folderId : \\d+}/rename/{modName}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response renameFolder(@Context HttpServletRequest request, @PathParam("folderId") Long folderId, @PathParam("modName") String modName) {
        Users user = Users.fromRequest(request);
        // handle some ACL logic:
        Folders targetFolder = getFolderForUser(user, folderId);

        if(user != null && !targetFolder.getUserId().equals(user.getId())) {
            throw new ForbiddenException(Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must own the folder to rename it").build());
        }

        createQuery()
            .update(folders)
            .where(folders.id.eq(folderId))
            .set(folders.displayName, modName)
            .execute();

        return Response.ok().build();
    }

    @PUT
    @Path("/{folderId : \\d+}/visibility/{visibility : (public|private)}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response updateVisibility(@Context HttpServletRequest request,
            @PathParam("folderId") Long folderId,
            @PathParam("visibility") String visibility) throws SQLException {
        Users user = Users.fromRequest(request);

        // handle some ACL logic:
        Folders f = getFolderForUser(user, folderId);
        // User must also -own- the folder:
        if(!f.getUserId().equals(user.getId())) {
            throw new ForbiddenException(Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must own the folder to set/view it's attributes").build());
        }

        // If a folder is changed to public, it will recurse up the parents and set those folders to public
        // If a folder is changed private, it will recurse down that folder and set everything to private
        String query = String.format("with recursive related_folders as (" +
                "     select id, parent_id, display_name, user_id, public, created_at from folders where id = %d" +
                "     union" +
                "     select f.id, f.parent_id, f.display_name, f.user_id, f.public, f.created_at from folders f" +
                "     inner join related_folders rf on (" +
                "          f.id != 0 AND (" +
                            (visibility.equals("public") ? "f.id = rf.parent_id" : "f.parent_id = rf.id") +
                "          )" +
                "     )" +
                ")" +
                "update folders x set public = %s " +
                "where x.id in (select id from related_folders)", folderId, visibility.equals("public"));
        long updated = 0;
        try(Connection conn = getConnection() ) {
            Statement stmt = conn.createStatement();
            stmt.execute(query);
            updated = stmt.getUpdateCount();

            if(!conn.getAutoCommit()) {
                conn.commit();
            }
        }

        java.util.Map<String, Object> r = new HashMap<String,Object>();
        r.put("updated", updated);
        return Response.status(Status.OK).entity(r).build();
    }


    @GET
    @Path("/{folderId : \\d+}/visibility")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getVisibility(@Context HttpServletRequest request,
            @PathParam("folderId") Long folderId) throws SQLException {
        Users user = Users.fromRequest(request);
        // handle some ACL logic:
        Folders f = getFolderForUser(user, folderId);
        // User must also -own- the folder:
        if(!f.getUserId().equals(user.getId())) {
            throw new ForbiddenException(Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must own the folder to change it's visibility").build());
        }
        List<Folders> affectedFolders = new ArrayList<Folders>();
        try(Connection conn = getConnection() ) {
            Statement stmt = conn.createStatement();
            String query = String.format("with recursive related_folders as (" +
                    "    select id,parent_id,display_name,user_id,public,created_at from folders where id = %d" +
                    "    union" +
                    "    select f.id,f.parent_id,f.display_name,f.user_id,f.public,f.created_at from folders f" +
                    "    inner join related_folders rf on (" +
                    "     f.id != 0 AND (f.parent_id = rf.id" +
                    "     OR" +
                    "     f.id = rf.parent_id)" +
                    "    )" +
                    ")" +
                    "select * from related_folders order by id asc;", folderId.longValue());
            ResultSet rs = stmt.executeQuery(query);
            while(rs.next()) {
                affectedFolders.add(Folders.fromResultSet(rs));
            }
            rs.close();
            stmt.close();
        }

        return Response.status(Status.OK).entity(affectedFolders).build();
    }

    /**
     * Converts a set of folder database records into an object returnable by a
     * web service
     *
     * @param folderRecordSet
     *            set of map layer records
     * @return folders web service object
     */
    public static FolderRecords mapFolderRecordsToFolders(List<Folders> folderRecordSet) {
        FolderRecords folderRecords = new FolderRecords();
        List<FolderRecord> folderRecordList = new ArrayList<>();

        for (Folders folderRecord : folderRecordSet) {
            FolderRecord folder = new FolderRecord();
            folder.setId(folderRecord.getId());
            folder.setName(folderRecord.getDisplayName());
            folder.setUserId(folderRecord.getUserId());
            folder.setParentId(folderRecord.getParentId());
            folder.setPublic(folderRecord.getPublicCol());
            folder.setCreatedAt(folderRecord.getCreatedAt());
            folderRecordList.add(folder);
        }

        folderRecords.setFolders(folderRecordList.toArray(new FolderRecord[folderRecordList.size()]));

        return folderRecords;
    }

    /**
     * Converts a set of database records into an object returnable by a web
     * service
     *
     * @param linkRecordSet
     *            set of map layer records
     * @return folders web service object
     */
    private static LinkRecords mapLinkRecordsToLinks(List<FolderMapMappings> linkRecordSet) {
        LinkRecords linkRecords = new LinkRecords();
        List<LinkRecord> linkRecordList = new ArrayList<>();

        for (FolderMapMappings linkRecord : linkRecordSet) {
            LinkRecord link = new LinkRecord();
            link.setId(linkRecord.getId());
            link.setFolderId(linkRecord.getFolderId());
            link.setMapId(linkRecord.getMapId());
            linkRecordList.add(link);
        }

        linkRecords.setLinks(linkRecordList.toArray(new LinkRecord[linkRecordList.size()]));

        return linkRecords;
    }

    /**
     * Returns list of info Tuples in folder visible to user
     * @param user
     * @param folder_id folder's  id
     * @return list of all map ids
     *
     */
    public static List<Tuple> getFolderMaps(Users user, Long folder_id) {
        List<Tuple> mapsInfo;
        getFolderForUser(user, folder_id); // will throw an exception if folder not visible to user...

        mapsInfo = createQuery()
            .select(maps.id, maps.displayName)
            .from(maps)
            .innerJoin(folderMapMappings)
            .on(maps.id.eq(folderMapMappings.mapId))
            .where(folderMapMappings.folderId.eq(folder_id))
            .fetch();

        return mapsInfo;
    }

    public static String getFolderName(Long folder_id) {
        return createQuery().select(folders.displayName).from(folders).where(folders.id.eq(folder_id)).fetchFirst();
    }

    public static Folders getFolderForUser(Users user, Long folderId) {
        if(folderId == 0) {
            Folders f = new Folders();
            f.setId(0L);
            f.setPublicCol(true);
            f.setUserId(user == null ? -1 : user.getId());
            return f;
        }
        Folders folder = createQuery()
                .select(folders)
                .from(folders)
                .where(folders.id.eq(folderId))
                .fetchOne();

        if(folder == null) {
            throw new NotFoundException();
        }

        if(user == null || UserResource.adminUserCheck(user) || user.getId().equals(folder.getUserId()) || folder.isPublic()) {
            return folder;
        }
        throw new ForbiddenException(Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You do not have access to this folder").build());
    }
    public static Folders folderFromList(List<Folders> folders, Long folderId) {
        for(Folders folder : folders) {
            if(folder.getId().equals(folderId)) {
                return folder;
            }
        }

        return null;
    }
    public static List<Folders> getFolderChainForUser(Users user, Long folderId) throws SQLException {
        List<Folders> out = new ArrayList<Folders>();

        // special case, root folder:
        if(folderId.equals(0L)) {
            out.add(getFolderForUser(user, folderId));
            return out;
        }

        String query = String.format(""

                + "with recursive related_folders as ( " +
                "        select id,parent_id,display_name,user_id,public,created_at from folders where id = %d " +
                "        union " +
                "        select f.id,f.parent_id,f.display_name,f.user_id,f.public,f.created_at from folders f " +
                "        inner join related_folders rf on ( " +
                "          f.id != 0 AND (f.parent_id = rf.id" +
                "          OR" +
                "          f.id = rf.parent_id)" +
                "        ) " +
                ") " +
                "select * from related_folders;"

                + "", folderId);
        try(Connection conn = getConnection() ) {
            Statement stmt = conn.createStatement();
            ResultSet rs = stmt.executeQuery(query);
            while(rs.next()) {
                out.add(Folders.fromResultSet(rs));
            }
            rs.close();
            stmt.close();
        }
        if(out.size() == 0) {
            throw new NotFoundException();
        }
        for(Folders f : out) {
            if(user != null && !f.getUserId().equals(user.getId())) {
                throw new ForbiddenException(Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must own the folder to modify it.").build());
            }
        }
        return out;
    }
}
