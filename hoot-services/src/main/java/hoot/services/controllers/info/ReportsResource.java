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
package hoot.services.controllers.info;

import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.RPT_STORE_PATH;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.filefilter.DirectoryFileFilter;
import org.apache.commons.io.filefilter.NotFileFilter;
import org.apache.commons.io.filefilter.TrueFileFilter;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;


@Controller
@Path("/reports")
public class ReportsResource {
    private static final Logger logger = LoggerFactory.getLogger(ReportsResource.class);

    private static final String REPORTS_PATH = HOME_FOLDER + "/" + RPT_STORE_PATH;

    public ReportsResource() {}

    /**
     * Gets the file stream of requested report.
     * 
     * GET hoot-services/info/reports/get?id=123-456&reportname=hello
     * 
     * @param id
     *            Report id
     * @param name
     *            Name of file to output
     * @return OCTET Stream of report pdf
     */
    @GET
    @Path("/get")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response getReport(@QueryParam("id") String id, @QueryParam("reportname") String name) {
        File report;

        try {
            report = getReportFile(id);
        }
        catch (Exception e) {
            String msg = "Error returning report with id = " + id + ", reportname = "  + name +
                    ".  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        if (report == null) {
            String msg = "Report with id = " + id + " not found!";
            throw new WebApplicationException(Response.status(Response.Status.NOT_FOUND).entity(msg).build());
        }

        ResponseBuilder responseBuilder = Response.ok(report, "application/pdf");
        responseBuilder.header("Content-Disposition", "attachment; filename=" + name);

        return responseBuilder.build();
    }

    /**
     * Gets list of all available reports.
     * 
     * hoot-services/info/reports/list
     * 
     * @return JSON Array of reports meta data
     */
    @GET
    @Path("/list")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getReportList() {
        JSONArray reports;

        try {
            reports = getReportsList();
        }
        catch (Exception e) {
            String message = "Error getting reports list!  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(message).build());
        }

        return Response.ok(reports.toJSONString()).build();
    }

    /**
     * Deletes requested report
     * 
     * hoot-services/info/reports/delete?id=123-456
     *
     * @param reportId
     *            Report id for deletion
     * @return JSON Object
     */
    @GET
    @Path("/delete")
    @Produces(MediaType.APPLICATION_JSON)
    public Response deleteReport(@QueryParam("id") String reportId) {
        boolean wasDeleted;

        try {
            File folder = hoot.services.utils.FileUtils.getSubFolderFromFolder(REPORTS_PATH, reportId);
            if ((folder != null) && folder.exists()) {
                FileUtils.forceDelete(folder);
                wasDeleted = true;
            }
            else {
                String msg = "Folder with id = " + reportId + " under " + REPORTS_PATH + " wasn't found!";
                throw new WebApplicationException(Response.status(Response.Status.NOT_FOUND).entity(msg).build());
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error deleting report with id = " + reportId + ".  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject entity = new JSONObject();
        entity.put("id", reportId);
        entity.put("deleted", String.valueOf(wasDeleted));

        return Response.ok(entity.toJSONString()).build();
    }

    // Gets the meta data of the report
    private static JSONObject getMetaData(String id) throws IOException, ParseException {
        JSONObject metadata = new JSONObject();

        File metaFolder = hoot.services.utils.FileUtils.getSubFolderFromFolder(REPORTS_PATH, id);

        if (metaFolder != null) {
            String metaDataPath = REPORTS_PATH + "/" + id + "/meta.data";
            File file = new File(metaDataPath);
            if (file.exists()) {
                String meta = FileUtils.readFileToString(file, "UTF-8");
                JSONParser p = new JSONParser();
                metadata = (JSONObject) p.parse(meta);
            }
        }

        return metadata;
    }

    private static JSONArray getReportsList() {
        JSONArray reportsList = new JSONArray();

        // sort by name
        Map<String, JSONObject> sorted = new TreeMap<>();

        File dir = new File(REPORTS_PATH);
        if (dir.exists()) {
            List<File> files = (List<File>) FileUtils.listFilesAndDirs(dir, new NotFileFilter(TrueFileFilter.INSTANCE),
                    DirectoryFileFilter.DIRECTORY);
            for (File file : files) {
                try {
                    if (file.isDirectory()) {
                        String id = file.getName();
                        String absPath = file.getAbsolutePath();
                        if (!absPath.equals(REPORTS_PATH)) {
                            JSONObject meta = getMetaData(id);
                            meta.put("id", id);
                            sorted.put(meta.get("name").toString(), meta);
                        }
                    }
                }
                catch (Exception ignored) {
                    // we ignore and continue
                }
            }
        }

        reportsList.addAll(sorted.values());

        return reportsList;
    }

    // retrieves the report file
    private static File getReportFile(String reportId) throws IOException, ParseException {
        File reportFile = null;

        JSONObject meta = getMetaData(reportId);

        Object reportPath = meta.get("reportpath");
        if (reportPath != null) {
            String repPath = reportPath.toString();
            File file = new File(repPath);
            if (file.exists()) {
                reportFile = file;
            }
        }

        return reportFile;
    }
}
