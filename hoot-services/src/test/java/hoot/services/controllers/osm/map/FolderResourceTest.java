package hoot.services.controllers.osm.map;

import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

import java.util.Map;

import javax.ws.rs.client.Entity;
import javax.ws.rs.core.GenericType;
import javax.ws.rs.core.MediaType;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.controllers.osm.OSMResourceTestAbstract;

public class FolderResourceTest extends OSMResourceTestAbstract {
    @Test
    @Category(UnitTest.class)
    public void testAdd() throws Exception {
        Map<String,Object> response = target("api/0.6/map/folders/add/0/testFolder" + Long.toString(System.currentTimeMillis()))
            .request(MediaType.APPLICATION_JSON)
            .post(Entity.entity("", MediaType.TEXT_PLAIN), new GenericType<Map<String, Object>>() {});

        Number folderId = (Number) response.get("folderId");
        boolean success = (boolean) response.get("success");
        assertTrue(folderId.longValue() > 0);
        assertTrue(success);
    }

    @Test
    @Category(UnitTest.class)
    public void testDelete() throws Exception {
        Map<String,Object> response = target("api/0.6/map/folders/add/0/testFolder" + Long.toString(System.currentTimeMillis()))
            .request(MediaType.APPLICATION_JSON)
            .post(Entity.entity("", MediaType.TEXT_PLAIN), new GenericType<Map<String, Object>>() {});
        Number folderId = (Number) response.get("folderId");
        boolean success = (boolean) response.get("success");
        assertTrue(folderId.longValue() > 0);
        assertTrue(success);

        Map<String,Object> response1 = target("api/0.6/map/folders/" + folderId)
            .request(MediaType.APPLICATION_JSON)
            .delete(new GenericType<Map<String, Object>>() {});

        boolean success1 = (boolean) response1.get("success");
        assertTrue(success1);

        FolderRecords response2 = target("api/0.6/map/folders")
            .request(MediaType.APPLICATION_JSON)
            .get(FolderRecords.class);

        for(FolderRecord f : response2.getFolders()) {
            assertNotEquals(f.getId(), folderId);
        }

    }

    @Test
    @Category(UnitTest.class)
    public void testModify() throws Exception {
        final String folderName = "testFolder" + Long.toString(System.currentTimeMillis());
        final String folderNameRenamed = folderName + "Renamed";

        Map<String,Object> response = target("api/0.6/map/folders/add/0/" + folderName)
            .request(MediaType.APPLICATION_JSON)
            .post(Entity.entity("", MediaType.TEXT_PLAIN), new GenericType<Map<String, Object>>() {});
        Number folderId = (Number) response.get("folderId");
        boolean success = (boolean) response.get("success");
        assertTrue(folderId.longValue() > 0);
        assertTrue(success);

        target(String.format("api/0.6/map/folders/%s/rename/%s", folderId, folderNameRenamed))
            .request(MediaType.APPLICATION_JSON)
            .put(Entity.entity("", MediaType.TEXT_PLAIN));

        FolderRecords response2 = target("api/0.6/map/folders")
            .request(MediaType.APPLICATION_JSON)
            .get(FolderRecords.class);

        FolderRecord folder = response2.getFolderById(folderId.longValue());
        Assert.assertNotNull(folder);
        Assert.assertEquals(folder.getName(), folderNameRenamed);

    }

    @Test
    @Category(UnitTest.class)
    public void testMove() throws Exception {
        Map<String,Object> response = target("api/0.6/map/folders/add/0/testFolder" + Long.toString(System.currentTimeMillis()))
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity("", MediaType.TEXT_PLAIN), new GenericType<Map<String, Object>>() {});
            Number parentFolderId = (Number) response.get("folderId");
            boolean success = (boolean) response.get("success");
            assertTrue(parentFolderId.longValue() > 0);
            assertTrue(success);

            Map<String,Object> response1 = target("api/0.6/map/folders/add/0/testFolder" + Long.toString(System.currentTimeMillis()))
                    .request(MediaType.APPLICATION_JSON)
                    .post(Entity.entity("", MediaType.TEXT_PLAIN), new GenericType<Map<String, Object>>() {});
                Number childFolderId = (Number) response1.get("folderId");
                boolean success1 = (boolean) response1.get("success");
                assertTrue(childFolderId.longValue() > 0);
                assertTrue(success1);

            Map<String,Object> response2 = target(String.format("api/0.6/map/folders/%s/move/%s", childFolderId, parentFolderId))
                .request(MediaType.APPLICATION_JSON)
                .put(Entity.entity("", MediaType.TEXT_PLAIN), new GenericType<Map<String, Object>>() {});

            boolean success2 = (boolean) response2.get("success");
            assertTrue(success2);

            FolderRecords response3 = target("api/0.6/map/folders")
                    .request(MediaType.APPLICATION_JSON)
                    .get(FolderRecords.class);

            FolderRecord childFolder = response3.getFolderById(childFolderId.longValue());
            FolderRecord parentFolder = response3.getFolderById(parentFolderId.longValue());

            Assert.assertEquals(childFolder.getParentId(), parentFolder.getId());
            Assert.assertEquals(0, parentFolder.getParentId());

            Map<String,Object> response4 = target("api/0.6/map/folders/" + parentFolderId)
                    .request(MediaType.APPLICATION_JSON)
                    .delete(new GenericType<Map<String, Object>>() {});

            boolean success3 = (boolean) response4.get("success");
            assertTrue(success3);

            FolderRecords response5 = target("api/0.6/map/folders")
                    .request(MediaType.APPLICATION_JSON)
                    .get(FolderRecords.class);

            childFolder = response5.getFolderById(childFolderId.longValue());
            Assert.assertEquals(0, childFolder.getParentId());


    }
}
