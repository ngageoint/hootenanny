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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ingest;

import static hoot.services.controllers.ingest.UploadClassification.*;

import java.io.File;
import java.io.IOException;
import java.util.Collection;
import java.util.EnumMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.filefilter.FileFilterUtils;
import org.apache.commons.io.filefilter.IOFileFilter;
import org.apache.commons.lang3.StringUtils;

import hoot.services.command.common.UnZIPFileCommand;

final class ImportResourceUtils {

    private ImportResourceUtils() {
    }

    static UploadClassification finalizeUploadClassification(int zipCnt, int shpZipCnt, int fgdbZipCnt, int osmZipCnt,
            int geojsonZipCnt, int geonamesZipCnt, int gpkgZipCnt, int shpCnt, int fgdbCnt, int osmCnt, int geojsonCnt,
            int geonamesCnt, int gpkgCnt) {
        // if fgdb zip > 0 then all becomes fgdb so it can be uzipped first
        // if fgdb zip == 0 and shp zip > then it is standard zip.
        // if fgdb zip == 0 and shp zip == 0 and osm zip > 0 then it is osm zip
        UploadClassification classification;

        if (zipCnt > 0) {
            if (fgdbZipCnt > 0) {
                classification = FGDB;
            } else {
                // Mix of shape and zip then we will unzip and treat it like OGR
                if (shpCnt > 0) { // One or more all ogr zip + shape
                    classification = SHP;
                } else if (osmCnt > 0) { // Mix of One or more all osm zip + osm
                    classification = OSM;
                } else if (geojsonCnt > 0) { // Mix of One or more all osm + osm
                    classification = GEOJSON;
                } else if (geonamesCnt > 0) { // Mix of One or more all osm zip + osm
                    classification = GEONAMES;
                } else if (gpkgCnt > 0) {
                    classification = GPKG;
                } else {
                    // One or more zip (all ogr) || One or more zip (all osm)
                    // If contains zip of just shape or osm then we will etl zip directly
                    classification = ZIP;
                }
            }
        } else if (shpCnt > 0) {
            classification = SHP;
        } else if (osmCnt > 0) {
            classification = OSM;
        } else if (fgdbCnt > 0) {
            classification = FGDB;
        } else if (geojsonCnt > 0) {
            classification = GEOJSON;
        } else if (geonamesCnt > 0) {
            classification = GEONAMES;
        } else if (gpkgCnt > 0) {
            classification = GPKG;
        } else {
            throw new RuntimeException("Error during classification: unable to classify uploaded file!");
        }

        return classification;
    }

    static List<File> handleUploadedFile(UploadClassification uploadClassification, File uploadedFile,
            Map<UploadClassification, Integer> counts, File workDir, String uploadType) {
        List<File> filesToImport = new LinkedList<>();

        int osmZipCnt = counts.getOrDefault(OSM_ZIP, 0);
        int shpZipCnt = counts.getOrDefault(SHAPE_ZIP, 0);
        int fgdbZipCnt = counts.getOrDefault(FGDB_ZIP, 0);
        int geojsonZipCnt = counts.getOrDefault(GEOJSON_ZIP, 0);
        int geonamesZipCnt = counts.getOrDefault(GEONAMES_ZIP, 0);
        int gpkgZipCnt = counts.getOrDefault(GPKG_ZIP, 0);
        int osmCnt = counts.getOrDefault(OSM, 0);
        int shpCnt = counts.getOrDefault(SHP, 0);
        int fgdbCnt = counts.getOrDefault(FGDB, 0);
        int geojsonCnt = counts.getOrDefault(GEOJSON, 0);
        int geonamesCnt = counts.getOrDefault(GEONAMES, 0);
        int gpkgCnt = counts.getOrDefault(GPKG, 0);

        if ((uploadClassification == OSM) || (uploadClassification == PBF)) {
            filesToImport.add(uploadedFile);
            osmCnt++;
        } else if ((uploadClassification == GEOJSON)) {
            filesToImport.add(uploadedFile);
            geojsonCnt++;
        } else if ((uploadClassification == GEONAMES) || (uploadClassification == TXT)) {
            filesToImport.add(uploadedFile);
            geonamesCnt++;
        } else if ((uploadClassification == GPKG)) {
            filesToImport.add(uploadedFile);
            gpkgCnt++;
        } else if (uploadClassification == SHP) {
            filesToImport.add(uploadedFile);
            shpCnt++;
        } else if (uploadClassification == ZIP) {
            // Check to see the type of zip (osm, ogr or fgdb)
            Map<UploadClassification, Integer> zipCounts = specialHandleWhenZIP(uploadedFile, filesToImport, workDir);

            shpZipCnt += zipCounts.get(SHAPE_ZIP);
            fgdbZipCnt += zipCounts.get(FGDB_ZIP);
            osmZipCnt += zipCounts.get(OSM_ZIP);
            geojsonZipCnt += zipCounts.get(GEOJSON_ZIP);
            geonamesZipCnt += zipCounts.get(GEONAMES_ZIP);
            gpkgZipCnt += zipCounts.get(GPKG_ZIP);
        } else if (uploadedFile.getName().equalsIgnoreCase("gdb")) {
            if (uploadType.equalsIgnoreCase("DIR")) {
                filesToImport.add(uploadedFile.getParentFile());
                fgdbCnt++;
            }
        }

        counts.put(SHAPE_ZIP, shpZipCnt);
        counts.put(FGDB_ZIP, fgdbZipCnt);
        counts.put(OSM_ZIP, osmZipCnt);
        counts.put(GEOJSON_ZIP, geojsonZipCnt);
        counts.put(GEONAMES_ZIP, geonamesZipCnt);
        counts.put(GPKG_ZIP, gpkgZipCnt);
        counts.put(SHP, shpCnt);
        counts.put(FGDB, fgdbCnt);
        counts.put(OSM, osmCnt);
        counts.put(GEOJSON, geojsonCnt);
        counts.put(GEONAMES, geonamesCnt);
        counts.put(GPKG, gpkgCnt);

        return filesToImport;
    }

    /**
     * Look inside of the zip and decide how to classify what's inside.
     *
     * @param zipToImport   archive to analyse
     * @param filesToImport
     * @return Map of counters after looking inside of the ZIP
     */
    static Map<UploadClassification, Integer> specialHandleWhenZIP(File zipToImport, List<File> filesToImport,
            File workDir) {
        String basename = FilenameUtils.getBaseName(zipToImport.getName());

        File targetFolder = new File(workDir, basename);

        // Uncompress the zip file
        new UnZIPFileCommand(zipToImport, targetFolder, ImportResource.class).execute();

        IOFileFilter fileFilter = FileFilterUtils.or(FileFilterUtils.suffixFileFilter(SHP.toString().toLowerCase()),
                FileFilterUtils.suffixFileFilter(OSM.toString().toLowerCase()),
                FileFilterUtils.suffixFileFilter(GEOJSON.toString().toLowerCase()),
                FileFilterUtils.suffixFileFilter(GEONAMES.toString().toLowerCase()),
                FileFilterUtils.suffixFileFilter(GPKG.toString().toLowerCase()),
                FileFilterUtils.suffixFileFilter(PBF.toString().toLowerCase()), FileFilterUtils.nameFileFilter("gdb"));

        Collection<File> files = FileUtils.listFiles(targetFolder, fileFilter, null);

        int shpCnt = 0, osmCnt = 0, geojsonCnt = 0, geonamesCnt = 0, gpkgCnt = 0, fgdbCnt = 0;

        for (File file : files) {
            String ext = FilenameUtils.getExtension(file.getName());
            UploadClassification uploadedFileType = UploadClassification.valueOf(ext.toUpperCase());

            if (file.getName().equalsIgnoreCase("gdb") && StringUtils.isBlank(ext)
                    && FilenameUtils.getExtension(targetFolder.getName()).equalsIgnoreCase("gdb")) {
                filesToImport.add(file.getParentFile());
                fgdbCnt++;
            } else if (uploadedFileType == SHP) {
                filesToImport.add(file);
                shpCnt++;
            } else if (uploadedFileType == OSM) {
                filesToImport.add(file);
                osmCnt++;
            } else if (uploadedFileType == GEOJSON) {
                filesToImport.add(file);
                geojsonCnt++;
            } else if (uploadedFileType == GEONAMES) {
                filesToImport.add(file);
                geonamesCnt++;
            } else if (uploadedFileType == GPKG) {
                filesToImport.add(file);
                gpkgCnt++;
            }
        }

        // We do not allow mix of ogr and osm in zip
        if (((shpCnt + fgdbCnt) > 0) && (osmCnt > 0)) {
            throw new IllegalArgumentException(
                    zipToImport.getAbsolutePath() + " cannot contain both OSM and OGR types.");
        }

        Map<UploadClassification, Integer> stats = new EnumMap<>(UploadClassification.class);

        stats.put(SHAPE_ZIP, shpCnt);
        stats.put(FGDB_ZIP, fgdbCnt);
        stats.put(OSM_ZIP, osmCnt);
        stats.put(GEOJSON_ZIP, geojsonCnt);
        stats.put(GEONAMES_ZIP, geonamesCnt);
        stats.put(GPKG_ZIP, gpkgCnt);

        return stats;
    }

    static void handleOSMZip(File workDir, List<File> zipsToImport, List<File> filesToImport, List<String> fileNames) {
        File zipFolder = new File(workDir, FilenameUtils.getBaseName(zipsToImport.get(0).getName()));

        IOFileFilter fileFilter = FileFilterUtils.suffixFileFilter("osm");
        Collection<File> osmFiles = FileUtils.listFiles(zipFolder, fileFilter, null);

        // reset
        zipsToImport.clear();
        filesToImport.clear();
        fileNames.clear();

        for (File osmFile : osmFiles) {
            filesToImport.add(osmFile);
            fileNames.add(osmFile.getName());
        }
    }

    static void handleGEONAMESWithTxtExtension(File workDir, File geonamesFile, List<String> fileNames,
            List<File> filesToImport) {
        String uploadedFileName = FilenameUtils.getBaseName(geonamesFile.getName()) + "." + "geonames";
        File srcFile = new File(workDir, geonamesFile.getName());
        File destFile = new File(workDir, uploadedFileName);

        // we need to rename the file for hoot to ingest
        try {
            FileUtils.moveFile(srcFile, destFile);
        } catch (IOException ioe) {
            throw new RuntimeException(
                    "Error trying to rename " + srcFile.getAbsolutePath() + " to " + destFile.getAbsolutePath(), ioe);
        }

        fileNames.set(0, uploadedFileName);

        filesToImport.clear();
        filesToImport.add(destFile);
    }
}
