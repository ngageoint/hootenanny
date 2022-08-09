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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ogr;


import static hoot.services.HootProperties.UPLOAD_FOLDER;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;

import org.apache.commons.io.FileUtils;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class ExtractAttributesCommandTest {

    @Test
    @Category(UnitTest.class)
    public void testExtractAttributesCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        File workDir = new File(UPLOAD_FOLDER, jobId);

        List<File> files = new LinkedList<>();
        files.add(new File(FileUtils.getTempDirectory(), "file1"));
        files.add(new File(FileUtils.getTempDirectory(), "file2"));
        files.add(new File(FileUtils.getTempDirectory(), "file3"));

        ExtractAttributesCommand extractAttributesCommand = new ExtractAttributesCommand(jobId, workDir, files, debugLevel, caller);

        assertEquals(jobId, extractAttributesCommand.getJobId());
        assertEquals(true, extractAttributesCommand.getTrackable());
        assertNotNull(extractAttributesCommand.getSubstitutionMap());
        assertNotNull(extractAttributesCommand.getWorkDir());
        assertNotNull(extractAttributesCommand.getCommand());

        String expectedCommand = "hoot.bin tag-info --${DEBUG_LEVEL} --tag-values-limit 30 ${INPUT_FILES}";
        assertEquals(expectedCommand, extractAttributesCommand.getCommand());

        assertTrue(extractAttributesCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, extractAttributesCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        List<String> expectedFiles = files.stream().map(File::getAbsolutePath).collect(Collectors.toList());
        assertEquals(expectedFiles, extractAttributesCommand.getSubstitutionMap().get("INPUT_FILES"));
    }
}
