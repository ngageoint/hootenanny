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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;


import static hoot.services.HootProperties.OSMAPI_DB_URL;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static org.junit.Assert.*;

import java.io.File;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.utils.MapUtils;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class OSMAPIDBDeriveChangesetCommandTest {

    @Test
    public void testOSMAPIDBDeriveChangesetCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();

        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams exportParams = new ExportParams();
        exportParams.setInput("map-with-id-" + mapId);
        exportParams.setOutputName("output");
        exportParams.setTextStatus(true);
        exportParams.setAppend(false);
        exportParams.setInputType("file");
        exportParams.setOutputType("shp");
        exportParams.setBounds(aoi);
        exportParams.setUserId(String.valueOf(userId));

        OSMAPIDBDeriveChangesetCommand exportCommand = new OSMAPIDBDeriveChangesetCommand(jobId, exportParams, debugLevel, caller);

        List<String> options = exportCommand.getCommonExportHootOptions();
        options.add("convert.bounding.box=" + aoi);
        options.add("osm.changeset.sql.file.writer.generate.new.ids=false");
        options.add("changeset.user.id=" + exportParams.getUserId());

        List<String> hootOptions = new LinkedList<>();
        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, exportCommand.getJobId());
        assertEquals(true, exportCommand.getTrackable());
        assertNotNull(exportCommand.getSubstitutionMap());
        assertNotNull(exportCommand.getWorkDir());
        assertNotNull(exportCommand.getCommand());

        String expectedCommand = "hoot derive-changeset --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${OSMAPI_DB_URL} " +
                "${INPUT} ${CHANGESET_OUTPUT_PATH} ${OSMAPI_DB_URL}";
        assertEquals(expectedCommand, exportCommand.getCommand());

        assertTrue(exportCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, exportCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, exportCommand.getSubstitutionMap().get("HOOT_OPTIONS"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("OSMAPI_DB_URL"));
        assertEquals(OSMAPI_DB_URL, exportCommand.getSubstitutionMap().get("OSMAPI_DB_URL"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("INPUT"));
        assertEquals(exportParams.getInput(), exportCommand.getSubstitutionMap().get("INPUT"));

        String expectedChangesetOutputPath = new File(new File(TEMP_OUTPUT_PATH, jobId),
                "changeset-" + jobId + ".osc.sql").getAbsolutePath();
        assertTrue(exportCommand.getSubstitutionMap().containsKey("CHANGESET_OUTPUT_PATH"));
        assertEquals(expectedChangesetOutputPath, exportCommand.getSubstitutionMap().get("CHANGESET_OUTPUT_PATH"));
    }
}