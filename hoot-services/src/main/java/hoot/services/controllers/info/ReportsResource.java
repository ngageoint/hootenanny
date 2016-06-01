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

import java.io.File;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.filefilter.DirectoryFileFilter;
import org.apache.commons.io.filefilter.NotFileFilter;
import org.apache.commons.io.filefilter.TrueFileFilter;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;


@Path("/reports")
public class ReportsResource {
    private static final Logger log = LoggerFactory.getLogger(ReportsResource.class);

    protected static String _rptStorePath = null;
    protected static String _homeFolder = null;

    public ReportsResource() {
        try {
            if (_rptStorePath == null) {
                _rptStorePath = HootProperties.getProperty("reportDataPath");
            }

            if (_homeFolder == null) {
                _homeFolder = HootProperties.getProperty("homeFolder");
            }

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

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
    public Response getReport(@QueryParam("id") final String id, @QueryParam("reportname") final String name) {
        File out = null;
        try {
            out = _getReportFile(id);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error exporting report file: " + ex.toString(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }
        ResponseBuilder rBuild = Response.ok(out, "application/pdf");
        rBuild.header("Content-Disposition", "attachment; filename=" + name);

        return rBuild.build();
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
    @Produces(MediaType.TEXT_PLAIN)
    public Response getReport() {
        JSONArray reps = new JSONArray();
        try {
            reps = _getReportsList();

        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error getting reports list: " + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }

        return Response.ok(reps.toString(), MediaType.TEXT_PLAIN).build();
    }

    /**
     * Deletes requested report
     * 
     * hoot-services/info/reports/delete?id=123-456
     * 
     * @param id
     *            Report id for deletion
     * @return JSON Object
     */
    @GET
    @Path("/delete")
    @Produces(MediaType.TEXT_PLAIN)
    public Response delReport(@QueryParam("id") final String id) {
        JSONObject resp = new JSONObject();
        boolean isDeleted = false;
        try {
            isDeleted = _deleteReport(id);
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error exporting report file: " + ex.toString(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }
        resp.put("id", id);
        resp.put("deleted", "" + isDeleted);
        return Response.ok(resp.toString(), MediaType.TEXT_PLAIN).build();
    }

    // Gets the meta data of the report
    protected JSONObject _getMetaData(final String id) throws Exception {
        JSONObject res = new JSONObject();

        String metaDataPath = _homeFolder + "/" + _rptStorePath + "/" + id + "/meta.data";
        File metaFolder = hoot.services.utils.FileUtils.getSubFolderFromFolder(_homeFolder + "/" + _rptStorePath, id);
        if (metaFolder != null) {
            File f = new File(metaDataPath);
            if (f.exists()) {
                String meta = FileUtils.readFileToString(f, "UTF-8");
                JSONParser p = new JSONParser();
                res = (JSONObject) p.parse(meta);
            }
        }
        return res;
    }

    // gets the list of meta data. This could get slow if there is large numbers
    // of reports
    // One solution may be using Runtime to do bash to get folder count natively
    // and that should be the fastest way..
    protected JSONArray _getReportsList() throws Exception {
        JSONArray res = new JSONArray();
        // sort by name
        Map<String, JSONObject> sorted = new TreeMap<String, JSONObject>();

        String storePath = _homeFolder + "/" + _rptStorePath;
        File f = new File(storePath);
        if (f.exists()) {
            List<File> files = (List<File>) FileUtils.listFilesAndDirs(f, new NotFileFilter(TrueFileFilter.INSTANCE),
                    DirectoryFileFilter.DIRECTORY);
            for (File file : files) {
                try {
                    if (file.isDirectory()) {
                        String id = file.getName();
                        String absPath = file.getAbsolutePath();
                        if (!absPath.equals(storePath)) {
                            JSONObject meta = _getMetaData(id);
                            meta.put("id", id);
                            sorted.put(meta.get("name").toString(), meta);
                        }
                    }
                }
                catch (Exception ee) {
                    // we ignore and continue
                    // log.error(ee.getMessage());
                }
            }
        }

        res.addAll(sorted.values());

        return res;
    }

    // retrieves the report file
    protected File _getReportFile(String id) throws Exception {
        File res = null;

        JSONObject meta = _getMetaData(id);
        Object oRepPath = meta.get("reportpath");
        if (oRepPath != null) {
            String repPath = oRepPath.toString();
            File f = new File(repPath);
            if (f.exists()) {
                res = f;
            }
        }

        return res;

    }

    // deletes requested report by deleting folder
    protected boolean _deleteReport(String id) throws Exception {
        boolean deleted = false;

        File f = hoot.services.utils.FileUtils.getSubFolderFromFolder(_homeFolder + "/" + _rptStorePath, id);
        if (f != null && f.exists()) {
            FileUtils.forceDelete(f);
            deleted = true;
        }

        return deleted;
    }

}
