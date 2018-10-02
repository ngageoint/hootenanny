package hoot.services.controllers.osm.map;

import static hoot.services.models.db.QFolderMapMappings.folderMapMappings;
import static hoot.services.models.db.QFolders.folders;
import static hoot.services.models.db.QMaps.maps;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.SQLQuery;

import hoot.services.models.db.FolderMapMappings;
import hoot.services.models.db.Folders;


@Controller
@Path("/api/0.6/map/folders")
@Transactional
public class FolderResource {
    private static final Logger logger = LoggerFactory.getLogger(FolderResource.class);

    /**
     * Returns a list of all folders in the services database.
     *
     * GET hoot-services/osm/api/0.6/map/folders
     *
     * @return a JSON object containing a list of folders
     */
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public FolderRecords getFolders() {
        FolderRecords folderRecords = null;

        List<Folders> folderRecordSet = createQuery().select(folders).from(folders).orderBy(folders.displayName.asc())
                .fetch();

        folderRecords = mapFolderRecordsToFolders(folderRecordSet);

        return folderRecords;
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
    public LinkRecords getLinks() {
        LinkRecords linkRecords = null;

        createQuery().delete(folderMapMappings)
                .where(new SQLQuery<>().from(maps).where(folderMapMappings.mapId.eq(maps.id)).notExists()).execute();

        try {
            createQuery().insert(folderMapMappings).columns(folderMapMappings.mapId, folderMapMappings.folderId)
                    .select(new SQLQuery<>().select(maps.id, Expressions.numberTemplate(Long.class, "0")).from(maps)
                            .where(maps.id.notIn(new SQLQuery<>().select(folderMapMappings.mapId).distinct()
                                    .from(folderMapMappings))))
                    .execute();
        }
        catch (Exception e) {
            logger.error("Could not add missing records...", e);
        }

        List<FolderMapMappings> linkRecordSet = createQuery().select(folderMapMappings).from(folderMapMappings)
                .orderBy(folderMapMappings.folderId.asc()).fetch();

        linkRecords = mapLinkRecordsToLinks(linkRecordSet);


        return linkRecords;
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
     * @return jobId Success = True/False
     */
    @POST
    @Path("/add/{parentId}/{folderName}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response addFolder(@PathParam("folderName") String folderName, @PathParam("parentId") Long parentId) {

        Long newId = createQuery().select(Expressions.numberTemplate(Long.class, "nextval('folders_id_seq')")).from()
                .fetchOne();

        Timestamp now = new Timestamp(System.currentTimeMillis());

        long userId = 1;
        createQuery()
                .insert(folders).columns(folders.id, folders.createdAt, folders.displayName, folders.publicCol,
                        folders.userId, folders.parentId)
                .values(newId, now, folderName, true, userId, parentId).execute();

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
    @Path("/delete/{folderId}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response deleteFolder(@PathParam("folderId") Long folderId) {

        List<Long> parentId = createQuery().select(folders.id).from(folders).where(folders.id.eq(folderId)).fetch();

        Long prntId = !parentId.isEmpty() ? parentId.get(0) : 0L;

        createQuery().update(folders).where(folders.parentId.eq(folderId)).set(folders.parentId, prntId).execute();

        createQuery().delete(folders).where(folders.id.eq(folderId)).execute();

        createQuery().update(folderMapMappings).where(folderMapMappings.folderId.eq(folderId))
                .set(folderMapMappings.folderId, 0L).execute();

        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        ret.put("success", true);
        return Response.ok().entity(ret).build();
    }

    /**
     *
     * POST hoot-services/osm/api/0.6/map/update/parent/{id}
     *
     *
     * @param folderId
     *            ID of folder
     * @param parentId
     *            ?
     * @param newRecord
     *            ?
     * @return jobId Success = True/False
     */
    @PUT
    @Path("/update/parent/{id}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response updateParentId(@QueryParam("folderId") Long folderId, @QueryParam("parentId") Long parentId,
            @QueryParam("newRecord") Boolean newRecord) {

        createQuery().update(folders).where(folders.id.eq(folderId)).set(folders.parentId, parentId).execute();
        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        ret.put("success", true);
        return Response.ok().entity(ret).build();
    }

    /**
     *
     * PUT hoot-services/osm/api/0.6/map/folders/modify/123456/New Dataset
     *
     *
     * @param mapId
     *            ID of map record or folder to be modified
     * @param modName
     *            The new name for the dataset
     * @param inputType
     *            Flag for either dataset or folder
     * @return jobId Success = True/False
     */
    @PUT
    @Path("/modify/{folderId : \\d+}/{modName}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response modifyName(@QueryParam("folderId") Long folderId, @QueryParam("modName") String modName,
            @PathParam("resource") String inputType) {

        createQuery().update(folders).where(folders.id.eq(folderId)).set(folders.displayName, modName)
                .execute();

        logger.debug("Renamed folder with id {} {}...", folderId, modName);

        return Response.ok().build();
    }

    /**
     * Converts a set of folder database records into an object returnable by a
     * web service
     *
     * @param folderRecordSet
     *            set of map layer records
     * @return folders web service object
     */
    private static FolderRecords mapFolderRecordsToFolders(List<Folders> folderRecordSet) {
        FolderRecords folderRecords = new FolderRecords();
        List<FolderRecord> folderRecordList = new ArrayList<>();

        for (Folders folderRecord : folderRecordSet) {
            FolderRecord folder = new FolderRecord();
            folder.setId(folderRecord.getId());
            folder.setName(folderRecord.getDisplayName());
            folder.setParentId(folderRecord.getParentId());
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
}
