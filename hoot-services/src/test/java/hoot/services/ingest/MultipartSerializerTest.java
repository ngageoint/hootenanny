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
package hoot.services.ingest;

import java.io.File;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemFactory;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.io.FileUtils;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.HootProperties;
import hoot.services.UnitTest;


public class MultipartSerializerTest {
    private static String homeFolder = null;
    private MultipartSerializer _ms = new MultipartSerializer();

    @BeforeClass
    public static void oneTimeSetup() throws Exception {
        homeFolder = HootProperties.getProperty("homeFolder");
        org.junit.Assert.assertNotNull(homeFolder);
        org.junit.Assert.assertTrue(homeFolder.length() > 0);
    }

    @Test
    @Category(UnitTest.class)
    public void TestserializeFGDB() throws Exception {
        String jobId = "123-456-789";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        org.junit.Assert.assertTrue(workingDir.exists());

        List<FileItem> fileItemsList = new ArrayList<FileItem>();

        FileItemFactory factory = new DiskFileItemFactory(16, null);
        String textFieldName = "textField";

        FileItem item = factory.createItem(textFieldName, "application/octet-stream", true,
                "fgdbTest.gdb/dummy1.gdbtable");

        String textFieldValue = "0123456789";
        byte[] testFieldValueBytes = textFieldValue.getBytes();

        OutputStream os = item.getOutputStream();
        os.write(testFieldValueBytes);
        os.close();

        File out = new File(wkdirpath + "/buffer.tmp");
        item.write(out);
        fileItemsList.add(item);
        org.junit.Assert.assertTrue(out.exists());

        Map<String, String> uploadedFiles = new HashMap<String, String>();
        Map<String, String> uploadedFilesPaths = new HashMap<String, String>();

        _ms._serializeFGDB(fileItemsList, jobId, uploadedFiles, uploadedFilesPaths);

        org.junit.Assert.assertEquals("GDB", uploadedFiles.get("fgdbTest"));
        org.junit.Assert.assertEquals("fgdbTest.gdb", uploadedFilesPaths.get("fgdbTest"));

        File fgdbpath = new File(wkdirpath + "/fgdbTest.gdb");
        org.junit.Assert.assertTrue(fgdbpath.exists());

        File content = new File(wkdirpath + "/fgdbTest.gdb/dummy1.gdbtable");
        org.junit.Assert.assertTrue(content.exists());

        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestserializeUploadedFiles() throws Exception {
        // homeFolder + "/upload/" + jobId + "/" + relPath;
        // Create dummy FGDB

        String jobId = "123-456-789-testosm";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        org.junit.Assert.assertTrue(workingDir.exists());

        List<FileItem> fileItemsList = new ArrayList<FileItem>();

        FileItemFactory factory = new DiskFileItemFactory(16, null);
        String textFieldName = "textField";

        FileItem item = factory.createItem(textFieldName, "application/octet-stream", true, "dummy1.osm");

        String textFieldValue = "0123456789";
        byte[] testFieldValueBytes = textFieldValue.getBytes();

        OutputStream os = item.getOutputStream();
        os.write(testFieldValueBytes);
        os.close();

        File out = new File(wkdirpath + "/buffer.tmp");
        item.write(out);
        fileItemsList.add(item);
        org.junit.Assert.assertTrue(out.exists());

        Map<String, String> uploadedFiles = new HashMap<String, String>();
        Map<String, String> uploadedFilesPaths = new HashMap<String, String>();

        _ms._serializeUploadedFiles(fileItemsList, uploadedFiles, uploadedFilesPaths, wkdirpath);

        org.junit.Assert.assertEquals("OSM", uploadedFiles.get("dummy1"));
        org.junit.Assert.assertEquals("dummy1.osm", uploadedFilesPaths.get("dummy1"));

        File content = new File(wkdirpath + "/dummy1.osm");
        org.junit.Assert.assertTrue(content.exists());

        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestValidatePath() throws Exception {
        boolean isValid = _ms._validatePath("/projects/hoot/upload/123456",
                "/projects/hoot/upload/123456/DcGisRoads.gdb");
        org.junit.Assert.assertTrue(isValid);
        isValid = _ms._validatePath("/projects/hoot/upload/123456", "/projects/hoot/upload/123456/../DcGisRoads.gdb");
        org.junit.Assert.assertFalse(isValid);
        isValid = _ms._validatePath("/projects/hoot/upload/123456", "\0//DcGisRoads.gdb");
        org.junit.Assert.assertFalse(isValid);
    }
}
