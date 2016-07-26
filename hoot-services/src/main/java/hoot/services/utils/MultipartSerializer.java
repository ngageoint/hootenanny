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
package hoot.services.utils;

import static hoot.services.HootProperties.HOME_FOLDER;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.ArrayUtils;
import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public final class MultipartSerializer {
    private static final Logger logger = LoggerFactory.getLogger(MultipartSerializer.class);

    private MultipartSerializer() {
    }

    static void serializeFGDB(List<FileItem> fileItemsList, String jobId, Map<String, String> uploadedFiles,
            Map<String, String> uploadedFilesPaths) {
        try {
            Iterator<FileItem> fileItemsIterator = fileItemsList.iterator();
            Map<String, String> folderMap = new HashMap<>();

            while (fileItemsIterator.hasNext()) {
                FileItem fileItem = fileItemsIterator.next();
                String fileName = fileItem.getName();

                String relPath = FilenameUtils.getPath(fileName);
                if (relPath.endsWith("/")) {
                    relPath = relPath.substring(0, relPath.length() - 1);
                }
                fileName = FilenameUtils.getName(fileName);

                String fgdbFolderPath = HOME_FOLDER + "/upload/" + jobId + "/" + relPath;

                boolean isPathSafe = validatePath(HOME_FOLDER + "/upload", fgdbFolderPath);

                if (isPathSafe) {
                    String pathVal = folderMap.get(fgdbFolderPath);
                    if (pathVal == null) {
                        File folderPath = new File(fgdbFolderPath);
                        FileUtils.forceMkdir(folderPath);
                        folderMap.put(fgdbFolderPath, relPath);
                    }

                    if (fileName == null) {
                        throw new IOException("File name cannot be null!");
                    }

                    String uploadedPath = fgdbFolderPath + "/" + fileName;
                    boolean isFileSafe = validatePath(fgdbFolderPath, uploadedPath);
                    if (isFileSafe) {
                        File file = new File(uploadedPath);
                        fileItem.write(file);
                    }
                    else {
                        throw new IOException("Illegal file path:" + uploadedPath);
                    }
                }
                else {
                    throw new IOException("Illegal path:" + fgdbFolderPath);
                }
            }

            for (Map.Entry<String, String> pairs : folderMap.entrySet()) {
                String nameOnly = "";
                String fgdbName = pairs.getValue();
                String[] nParts = fgdbName.split("\\.");

                for (int i = 0; i < (nParts.length - 1); i++) {
                    if (!nameOnly.isEmpty()) {
                        nameOnly += ".";
                    }
                    nameOnly += nParts[i];
                }
                uploadedFiles.put(nameOnly, "GDB");
                uploadedFilesPaths.put(nameOnly, fgdbName);
            }
        }
        catch (Exception e) {
            throw new RuntimeException("Error trying to serialize FGDB!", e);
        }
    }

    static void serializeUploadedFiles(List<FileItem> fileItemsList, Map<String, String> uploadedFiles,
            Map<String, String> uploadedFilesPaths, String repFolderPath) {
        try {
            for (FileItem fileItem : fileItemsList) {
                String fileName = fileItem.getName();

                if (fileName == null) {
                    throw new RuntimeException("A valid file name was not specified.");
                }

                String uploadedPath = repFolderPath + "/" + fileName;

                boolean isPathSafe = validatePath(HOME_FOLDER + "/upload", uploadedPath);
                if (isPathSafe) {
                    File file = new File(uploadedPath);
                    fileItem.write(file);

                    String[] nameParts = fileName.split("\\.");
                    if (nameParts.length > 1) {
                        String extension = nameParts[nameParts.length - 1].toUpperCase();

                        String[] subArr = ArrayUtils.removeElement(nameParts, nameParts[nameParts.length - 1]);
                        String filename = StringUtils.join(subArr, '.');
                        if (extension.equalsIgnoreCase("OSM") || extension.equalsIgnoreCase("GEONAMES")
                                || extension.equalsIgnoreCase("SHP") || extension.equalsIgnoreCase("ZIP")) {
                            uploadedFiles.put(filename, extension);
                            uploadedFilesPaths.put(filename, fileName);
                            logger.debug("Saving uploaded:{}", filename);
                        }
                    }
                }
                else {
                    throw new IOException("Illegal path: " + uploadedPath);
                }
            }
        }
        catch (Exception ioe) {
            throw new RuntimeException("Error trying to serialize uploaded files!", ioe);
        }
    }

    /**
     * Serializes uploaded multipart data into files. It can handle file or
     * folder type.
     * 
     * @param jobId
     *            = unique id to identify uploaded files group
     * @param inputType
     *            = ["FILE" | "DIR"] where DIR type is treated as FGDB
     * @param uploadedFiles
     *            = The list of files uploaded
     * @param uploadedFilesPaths
     *            = The list of uploaded files paths
     * @param request
     *            = The request object that holds post data
     */
    public static void serializeUpload(String jobId, String inputType, Map<String, String> uploadedFiles,
            Map<String, String> uploadedFilesPaths, HttpServletRequest request) {

        try {
            // Uploaded data container folder path. It is unique to each job
            String repFolderPath = HOME_FOLDER + "/upload/" + jobId;
            boolean isPathSafe = validatePath(HOME_FOLDER + "/upload", repFolderPath);

            if (isPathSafe) {
                File dir = new File(repFolderPath);
                FileUtils.forceMkdir(dir);

                if (!ServletFileUpload.isMultipartContent(request)) {
                    throw new Exception("Content type is not multipart/form-data");
                }

                DiskFileItemFactory fileFactory = new DiskFileItemFactory();
                File filesDir = new File(repFolderPath);
                fileFactory.setRepository(filesDir);
                ServletFileUpload uploader = new ServletFileUpload(fileFactory);

                List<FileItem> fileItemsList = uploader.parseRequest(request);

                // If user request type is DIR then treat it as FGDB folder
                if (inputType.equalsIgnoreCase("DIR")) {
                    serializeFGDB(fileItemsList, jobId, uploadedFiles, uploadedFilesPaths);
                }
                else {
                    // Can be shapefile or zip file
                    serializeUploadedFiles(fileItemsList, uploadedFiles, uploadedFilesPaths, repFolderPath);
                }
            }
            else {
                throw new IOException("Illegal path: " + repFolderPath);
            }
        }
        catch (Exception e) {
            throw new RuntimeException("Error trying to serialize upload!", e);
        }
    }

    // See #6760
    // Stop file path manipulation vulnerability by validating the new path is
    // within container path
    static boolean validatePath(String basePath, String newPath) {
        boolean isValidated = false;

        try {
            File f = new File(newPath);
            String potentialPath = f.getCanonicalPath();

            f = new File(basePath);
            String containerPath = f.getCanonicalPath();

            // verify that newPath is within basePath
            if (potentialPath.indexOf(containerPath) == 0) {
                isValidated = true;
            }
        }
        catch (Exception ex) {
            logger.error("failed to validate MultipartSerializer path: {}", ex.getMessage());
        }

        return isValidated;
    }
}
