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
package hoot.services.utils;

import static hoot.services.HootProperties.HOME_FOLDER;

import java.io.File;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.ws.rs.core.MediaType;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemFactory;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.io.FileUtils;
import org.glassfish.jersey.media.multipart.FormDataContentDisposition;
import org.glassfish.jersey.media.multipart.FormDataMultiPart;
import org.glassfish.jersey.media.multipart.file.FileDataBodyPart;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class MultipartSerializerTest {
    private static File homefolder;
    private static String original_HOME_FOLDER;

    @BeforeClass
    public static void oneTimeSetup() throws Exception {
        original_HOME_FOLDER = HOME_FOLDER;
        homefolder = new File(FileUtils.getTempDirectory(), "MultipartSerializerTest");
        FileUtils.forceMkdir(homefolder);
        Assert.assertTrue(homefolder.exists());
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", homefolder.getAbsolutePath());
    }

    @AfterClass
    public static void afterClass() throws Exception {
        FileUtils.deleteDirectory(homefolder);
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", original_HOME_FOLDER);
    }

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void TestserializeFGDB() throws Exception {
        String jobId = "123-456-789";
        String wkdirpath = HOME_FOLDER + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        Assert.assertTrue(workingDir.exists());

        String textFieldValue = "0123456789";
        File out = new File(wkdirpath + "/fgdbTest.gdb");
        FileUtils.write(out, textFieldValue);

        // MediaType of the body part will be derived from the file.
        FileDataBodyPart filePart = new FileDataBodyPart("", out, MediaType.APPLICATION_OCTET_STREAM_TYPE);
        FormDataContentDisposition formDataContentDisposition =
                new FormDataContentDisposition("form-data; name=\"eltuploadfile0\"; filename=\"fgdbTest.gdb\"");
        filePart.setContentDisposition(formDataContentDisposition);
        FormDataMultiPart multiPart = new FormDataMultiPart();
        multiPart.bodyPart(filePart);

        Map<String, String> uploadedFiles = new HashMap<>();
        Map<String, String> uploadedFilesPaths = new HashMap<>();

        MultipartSerializer.serializeUpload(jobId, "DIR", uploadedFiles, uploadedFilesPaths, multiPart);

        Assert.assertEquals("GDB", uploadedFiles.get("fgdbTest"));
        Assert.assertEquals("fgdbTest.gdb", uploadedFilesPaths.get("fgdbTest"));

        File fgdbpath = new File(wkdirpath + "/fgdbTest.gdb");
        Assert.assertTrue(fgdbpath.exists());

        File content = new File(wkdirpath + "/fgdbTest.gdb/dummy1.gdbtable");
        Assert.assertTrue(content.exists());

        FileUtils.forceDelete(workingDir);
    }

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void TestserializeUploadedFiles() throws Exception {
        // homeFolder + "/upload/" + jobId + "/" + relPath;
        // Create dummy FGDB

        String jobId = "123-456-789-testosm";
        String wkdirpath = HOME_FOLDER + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        Assert.assertTrue(workingDir.exists());

        FileItemFactory factory = new DiskFileItemFactory(16, null);
        String textFieldName = "textField";

        FileItem item = factory.createItem(textFieldName, "application/octet-stream", true, "dummy1.osm");

        String textFieldValue = "0123456789";
        byte[] testFieldValueBytes = textFieldValue.getBytes();

        try (OutputStream os = item.getOutputStream()) {
            os.write(testFieldValueBytes);
        }

        File out = new File(wkdirpath + "/buffer.tmp");
        item.write(out);

        List<FileItem> fileItemsList = new ArrayList<>();
        fileItemsList.add(item);
        Assert.assertTrue(out.exists());

        Map<String, String> uploadedFiles = new HashMap<>();
        Map<String, String> uploadedFilesPaths = new HashMap<>();

//        MultipartSerializer.serializeUploadedFiles(fileItemsList, uploadedFiles, uploadedFilesPaths, wkdirpath);

        Assert.assertEquals("OSM", uploadedFiles.get("dummy1"));
        Assert.assertEquals("dummy1.osm", uploadedFilesPaths.get("dummy1"));

        File content = new File(wkdirpath + "/dummy1.osm");
        Assert.assertTrue(content.exists());

        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestValidatePath() throws Exception {
        Method m = MultipartSerializer.class.getDeclaredMethod("validatePath", String.class, String.class);
        m.setAccessible(true);

        boolean isValid = (Boolean) m.invoke(null, //use null if the method is static
                "/projects/hoot/upload/123456", "/projects/hoot/upload/123456/DcGisRoads.gdb");
        Assert.assertTrue(isValid);
        isValid = (Boolean) m.invoke(null, //use null if the method is static
                "/projects/hoot/upload/123456", "/projects/hoot/upload/123456/../DcGisRoads.gdb");
        Assert.assertFalse(isValid);
        isValid =(Boolean) m.invoke(null, //use null if the method is static
                "/projects/hoot/upload/123456", "\0//DcGisRoads.gdb");
        Assert.assertFalse(isValid);
    }
}
