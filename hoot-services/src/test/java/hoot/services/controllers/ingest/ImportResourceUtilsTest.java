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
package hoot.services.controllers.ingest;

import static hoot.services.controllers.ingest.UploadClassification.*;
import static org.junit.Assert.assertEquals;

import org.junit.Test;


public class ImportResourceUtilsTest {

    @Test
    public void testClassifyUploadedFile() throws Exception {
        int zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt;

        shpCnt = 1; zipCnt = shpZipCnt = fgdbZipCnt = osmZipCnt = geonamesZipCnt = fgdbCnt = osmCnt = geonamesCnt = 0;
        UploadClassification classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                                        geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(SHP, classification);

        osmCnt = 1; zipCnt = shpZipCnt = fgdbZipCnt = osmZipCnt = geonamesZipCnt = shpCnt = fgdbCnt = geonamesCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                                            geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(OSM, classification);

        fgdbCnt = 1; zipCnt = shpZipCnt = fgdbZipCnt = osmZipCnt = geonamesZipCnt = shpCnt = osmCnt = geonamesCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(FGDB, classification);

        geonamesCnt = 1; zipCnt = shpZipCnt = fgdbZipCnt = osmZipCnt = geonamesZipCnt = shpCnt = osmCnt = fgdbCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(GEONAMES, classification);

        zipCnt = 1; fgdbZipCnt = 1; shpZipCnt = osmZipCnt = geonamesZipCnt = shpCnt = osmCnt = fgdbCnt = geonamesCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(FGDB, classification);

        zipCnt = 1; shpCnt = 1; shpZipCnt = osmZipCnt = geonamesZipCnt = osmCnt = fgdbZipCnt = fgdbCnt = geonamesCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(SHP, classification);

        zipCnt = 1; osmCnt = 1; shpZipCnt = osmZipCnt = geonamesZipCnt = shpCnt = fgdbZipCnt = fgdbCnt = geonamesCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(OSM, classification);

        zipCnt = 1; osmZipCnt = 1; shpZipCnt = geonamesCnt = geonamesZipCnt = shpCnt = osmCnt = fgdbZipCnt = fgdbCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(ZIP, classification);
    }

    @Test
    public void testHandleUploadedFile() throws Exception {
        //ImportResourceUtils.handleUploadedFile();
    }

    @Test
    public void testSpecialHandleWhenZIP() throws Exception {
        //ImportResourceUtils.specialHandleWhenZIP();
    }

    @Test
    public void testHandleOSMZip() throws Exception {
        //ImportResourceUtils.handleOSMZip();
    }

    @Test
    public void testHandleGEONAMESWithTxtExtension() throws Exception {
        //ImportResourceUtils.handleGEONAMESWithTxtExtension();
    }
}