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

import java.io.File;
import java.util.Collection;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.filefilter.FileFilterUtils;
import org.apache.commons.io.filefilter.IOFileFilter;
import org.apache.commons.lang3.StringUtils;

import hoot.services.command.common.UnZIPFileCommand;


final class ImportUtils {

    private ImportUtils() {}

    static String classifyUploadedFile(int zipCnt, int shpZipCnt, int fgdbZipCnt, int osmZipCnt,
                                       int geonamesZipCnt, int shpCnt, int fgdbCnt, int osmCnt, int geonamesCnt) {
        // if fgdb zip > 0 then all becomes fgdb so it can be uzipped first
        // if fgdb zip == 0 and shp zip > then it is standard zip.
        // if fgdb zip == 0 and shp zip == 0 and osm zip > 0 then it is osm zip
        String classification;

        if (zipCnt > 0) {
            if (fgdbZipCnt > 0) {
                classification = "OGR";
            }
            else {
                // Mix of shape and zip then we will unzip and treat it like OGR
                if (shpCnt > 0) { // One or more all ogr zip + shape
                    classification = "OGR";
                }
                else if (osmCnt > 0) { // Mix of One or more all osm zip + osm
                    classification = "OSM";
                }
                else if (geonamesCnt > 0) { // Mix of One or more all osm zip + osm
                    classification = "GEONAMES";
                }
                else {
                    // One or more zip (all ogr) || One or more zip (all osm)
                    // If contains zip of just shape or osm then we will etl zip directly
                    classification = "ZIP";
                }
            }
        }
        else if (shpCnt > 0) {
            classification = "OGR";
        }
        else if (osmCnt > 0) {
            classification = "OSM";
        }
        else if (fgdbCnt > 0) {
            classification = "FGDB";
        }
        else if (geonamesCnt > 0) {
            classification = "GEONAMES";
        }
        else {
            throw new RuntimeException("Error during classification: unable to classify uploaded file!");
        }

        return classification;
    }

    static List<Map<String, String>> handleUploadedFile(String ext, File inputFile, Map<String, Integer> stats, File workDir, String inputType) {
        List<Map<String, String>> etlRequests = new LinkedList<>();

        int osmZipCnt = stats.getOrDefault("osmzipcnt", 0);
        int shpZipCnt = stats.getOrDefault("shpzipcnt", 0);
        int fgdbZipCnt = stats.getOrDefault("fgdbzipcnt", 0);
        int geonamesZipCnt = stats.getOrDefault("geonameszipcnt", 0);
        int osmCnt = stats.getOrDefault("osmcnt", 0);
        int shpCnt = stats.getOrDefault("shpcnt", 0);
        int fgdbCnt = stats.getOrDefault("fgdbcnt", 0);
        int geonamesCnt = stats.getOrDefault("geonamescnt", 0);

        if (ext.equalsIgnoreCase("OSM") || ext.equalsIgnoreCase("PBF")) {
            Map<String, String> etlRequest = new HashMap<>();
            etlRequest.put("type", "OSM");
            etlRequest.put("name", inputFile.getAbsolutePath());
            etlRequests.add(etlRequest);
            osmCnt++;
        }
        else if (ext.equalsIgnoreCase("GEONAMES") || ext.equalsIgnoreCase("TXT")) {
            Map<String, String> etlRequest = new HashMap<>();
            etlRequest.put("type", "GEONAMES");
            etlRequest.put("name", inputFile.getAbsolutePath());
            etlRequests.add(etlRequest);
            geonamesCnt++;
        }
        else if (ext.equalsIgnoreCase("SHP")) {
            Map<String, String> etlRequest = new HashMap<>();
            etlRequest.put("type", "OGR");
            etlRequest.put("name", inputFile.getAbsolutePath());
            etlRequests.add(etlRequest);
            shpCnt++;
        }
        else if (ext.equalsIgnoreCase("ZIP")) {
            // Check to see the type of zip (osm, ogr or fgdb)
            Map<String, Integer> results = specialHandleWhenZIP(inputFile, etlRequests, workDir);

            shpZipCnt += results.get("shpzipcnt");
            fgdbZipCnt += results.get("fgdbzipcnt");
            osmZipCnt += results.get("osmzipcnt");
            geonamesZipCnt += results.get("geonameszipcnt");
        }
        else if (inputFile.getName().equalsIgnoreCase("GDB") && inputType.equalsIgnoreCase("DIR")) {
            Map<String, String> etlRequest = new HashMap<>();
            etlRequest.put("type", "FGDB");
            etlRequest.put("name", inputFile.getParentFile().getAbsolutePath());
            etlRequests.add(etlRequest);
            fgdbCnt++;
        }

        stats.put("shpzipcnt", shpZipCnt);
        stats.put("fgdbzipcnt", fgdbZipCnt);
        stats.put("osmzipcnt", osmZipCnt);
        stats.put("geonameszipcnt", geonamesZipCnt);
        stats.put("shpcnt", shpCnt);
        stats.put("fgdbcnt", fgdbCnt);
        stats.put("osmcnt", osmCnt);
        stats.put("geonamescnt", geonamesCnt);

        return etlRequests;
    }

    /**
     * Look inside of the zip and decide how to classify what's inside.
     *
     * @param zip archive to analyse
     * @param etlRequests
     * @return Map of counters after looking inside of the ZIP
     */
    static Map<String, Integer> specialHandleWhenZIP(File zip, List<Map<String, String>> etlRequests, File workDir) {
        String basename = FilenameUtils.getBaseName(zip.getName());

        File targetFolder = new File(workDir, FilenameUtils.getBaseName(basename));

        // Uncompress the zip file
        new UnZIPFileCommand(zip, targetFolder, ImportResource.class).execute();

        IOFileFilter fileFilter = FileFilterUtils.or(
                FileFilterUtils.suffixFileFilter("shp"),
                FileFilterUtils.suffixFileFilter("osm"),
                FileFilterUtils.suffixFileFilter("geonames"),
                FileFilterUtils.suffixFileFilter("pbf"),
                FileFilterUtils.nameFileFilter("gdb"));

        Collection<File> files = FileUtils.listFiles(targetFolder, fileFilter, null);

        int shpCnt = 0;
        int osmCnt = 0;
        int geonamesCnt = 0;
        int fgdbCnt = 0;

        for (File file : files) {
            String ext = FilenameUtils.getExtension(file.getName());

            if (file.getName().equalsIgnoreCase("GDB") && StringUtils.isBlank(ext) &&
                    FilenameUtils.getExtension(targetFolder.getName()).equalsIgnoreCase("GDB")) {
                Map<String, String> contentType = new HashMap<>();
                contentType.put("type", "FGDB_ZIP");
                contentType.put("name", file.getParentFile().getAbsolutePath());
                etlRequests.add(contentType);
                fgdbCnt++;
            }
            else if (ext.equalsIgnoreCase("SHP")) {
                Map<String, String> contentType = new HashMap<>();
                contentType.put("type", "OGR_ZIP");
                contentType.put("name", file.getAbsolutePath());
                etlRequests.add(contentType);
                shpCnt++;
            }
            else if (ext.equalsIgnoreCase("OSM")) {
                Map<String, String> contentType = new HashMap<>();
                contentType.put("type", "OSM_ZIP");
                contentType.put("name", file.getAbsolutePath());
                etlRequests.add(contentType);
                osmCnt++;
            }
            else if (ext.equalsIgnoreCase("GEONAMES")) {
                Map<String, String> contentType = new HashMap<>();
                contentType.put("type", "GEONAMES_ZIP");
                contentType.put("name", file.getAbsolutePath());
                etlRequests.add(contentType);
                geonamesCnt++;
            }
        }

        // We do not allow mix of ogr and osm in zip
        if (((shpCnt + fgdbCnt) > 0) && (osmCnt > 0)) {
            throw new IllegalArgumentException(zip.getAbsolutePath() + " cannot contain both OSM and OGR types.");
        }

        Map<String, Integer> stats = new HashMap<>();

        stats.put("shpzipcnt", shpCnt);
        stats.put("fgdbzipcnt", fgdbCnt);
        stats.put("osmzipcnt", osmCnt);
        stats.put("geonameszipcnt", geonamesCnt);

        return stats;
    }
}
