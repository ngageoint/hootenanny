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
 * @copyright Copyright (C) 2015, 2016, 2017 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ogr;

import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static hoot.services.HootProperties.UPLOAD_FOLDER;
import static junit.framework.TestCase.assertNotNull;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import java.io.File;
import java.nio.charset.Charset;
import java.util.UUID;

import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.glassfish.jersey.media.multipart.FormDataMultiPart;
import org.glassfish.jersey.media.multipart.MultiPart;
import org.glassfish.jersey.media.multipart.MultiPartFeature;
import org.glassfish.jersey.media.multipart.file.FileDataBodyPart;
import org.json.simple.JSONObject;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.jerseyframework.HootServicesJerseyTestAbstract;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.job.Job;
import hoot.services.utils.HootCustomPropertiesSetter;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class ExtractAttributesResourceTest extends HootServicesJerseyTestAbstract {

    @Test
    public void processUpload() throws Exception {
        String original_UPLOAD_FOLDER = UPLOAD_FOLDER;
        File uploadFolder = new File(FileUtils.getTempDirectory(), "ExtractAttributesResourceTest");

        FileUtils.forceMkdir(uploadFolder);
        File fakeShapeFile = new File(FileUtils.getTempDirectory(), "fakeshape.shp");

        try {
            HootCustomPropertiesSetter.setProperty("UPLOAD_FOLDER", uploadFolder.getAbsolutePath());
            FileUtils.write(fakeShapeFile, "This is a fake shape file!", Charset.defaultCharset());

            FileDataBodyPart filePart = new FileDataBodyPart("file", fakeShapeFile);
            FormDataMultiPart formDataMultipart = new FormDataMultiPart();
            MultiPart form = formDataMultipart.bodyPart(filePart);

            Response response = target("info/upload")
                    .register(MultiPartFeature.class)
                    .queryParam("DEBUG_LEVEL", "info")
                    .queryParam("INPUT_TYPE", "FILE")
                    .request(MediaType.APPLICATION_JSON)
                    .post(Entity.entity(form, MediaType.MULTIPART_FORM_DATA_TYPE), Response.class);

            assertNotNull(response);

            Job job = super.getSubmittedJob();

            assertNotNull(job);
            assertEquals(1, job.getCommands().length);
            assertEquals(ExtractAttributesCommand.class, job.getCommands()[0].getClass());
        }
        finally {
            HootCustomPropertiesSetter.setProperty("UPLOAD_FOLDER", original_UPLOAD_FOLDER);
            FileUtils.deleteQuietly(uploadFolder);
            FileUtils.deleteQuietly(fakeShapeFile);
        }
    }

    @Test
    public void getAttributes() throws Exception {
        String original_TEMP_OUTPUT_PATH = TEMP_OUTPUT_PATH;
        try {
            HootCustomPropertiesSetter.setProperty("TEMP_OUTPUT_PATH", FileUtils.getTempDirectoryPath());

            String jobId = UUID.randomUUID().toString();

            File outputFile = new File(TEMP_OUTPUT_PATH, jobId + ".out");

            JSONObject json = new JSONObject();
            json.put("key1", "hello");
            json.put("key2", "world");

            FileUtils.write(outputFile, json.toJSONString(), Charset.defaultCharset());

            Response response = target("info/" + jobId)
                    .queryParam("deleteoutput", "true")
                    .request(MediaType.APPLICATION_JSON).get();

            String jsonStr = response.readEntity(String.class);

            assertEquals("{\"key1\":\"hello\",\"key2\":\"world\"}", jsonStr);
            assertFalse(outputFile.exists());
        }
        finally {
            HootCustomPropertiesSetter.setProperty("TEMP_OUTPUT_PATH", original_TEMP_OUTPUT_PATH);
        }
    }
}