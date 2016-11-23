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
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.RASTER_TO_TILES;
import static hoot.services.HootProperties.TILE_SERVER_PATH;

import java.util.UUID;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.controllers.job.JobControllerBase;
import hoot.services.geo.BoundingBox;
import hoot.services.controllers.job.JobStatusManager;
import hoot.services.models.db.QMaps;
import hoot.services.models.osm.Map;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.nativeinterfaces.JobExecutionManager;


@Transactional
@Component
public class RasterToTilesService extends JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(RasterToTilesService.class);

    @Autowired
    private JobExecutionManager jobExecManager;

    @Autowired
    private JobStatusManager jobStatusManager;


    public RasterToTilesService() {
        super(RASTER_TO_TILES);
    }

    public String ingestOSMResourceDirect(String name, String userEmail) {
        String jobId = UUID.randomUUID().toString();
        return ingestOSMResourceDirect(name, userEmail, jobId);
    }

    /**
     * This function executes directly. This should be used when called from
     * JobResource it prevents the thread race condition when threadpool maxes out.
     */
    public String ingestOSMResourceDirect(String name, String userEmail, String jobId) {
        // _zoomLevels
        try {
            try {
                jobStatusManager.addJob(jobId);

                QMaps maps = QMaps.maps;
                long mapIdNum = ModelDaoUtils.getRecordIdForInputString(name, maps, maps.id, maps.displayName);

                BoundingBox queryBounds;
                try {
                    queryBounds = new BoundingBox("-180,-90,180,90");
                    logger.debug("Query bounds area: {}", queryBounds.getArea());
                }
                catch (Exception e) {
                    throw new RuntimeException("Error parsing bounding box from bbox param: " + "-180,-90,180,90" + " ("
                            + e.getMessage() + ")", e);
                }

                Map currMap = new Map(mapIdNum);
                JSONObject extents = currMap.retrieveNodesMBR(queryBounds);

                Object oMinLon = extents.get("minlon");
                Object oMaxLon = extents.get("maxlon");
                Object oMinLat = extents.get("minlat");
                Object oMaxLat = extents.get("maxlat");

                String warn = null;

                // Make sure we have valid bbox. We may end up with invalid bbox and
                // in that case we should not produce raster density map
                if ((oMinLon != null) && (oMaxLon != null) && (oMinLat != null) && (oMaxLat != null)) {
                    double dMinLon = (Double) extents.get("minlon");
                    double dMaxLon = (Double) extents.get("maxlon");
                    double dMinLat = (Double) extents.get("minlat");
                    double dMaxLat = (Double) extents.get("maxlat");

                    double deltaLon = dMaxLon - dMinLon;
                    double deltaLat = dMaxLat - dMinLat;

                    double maxDelta = deltaLon;
                    if (deltaLat > maxDelta) {
                        maxDelta = deltaLat;
                    }

                    JSONObject zoomInfo = getZoomInfo(maxDelta);

                    String zoomList = zoomInfo.get("zoomlist").toString();
                    int rasterSize = (Integer) zoomInfo.get("rastersize");

                    JSONObject argStr = createCommandObj(name, zoomList, rasterSize, userEmail, mapIdNum);
                    argStr.put("jobId", jobId);

                    JSONObject res = this.jobExecManager.exec(argStr);
                    Object oRes = res.get("warnings");

                    if (oRes != null) {
                        warn = oRes.toString();
                    }
                }

                if (warn == null) {
                    jobStatusManager.setComplete(jobId);
                }
                else {
                    jobStatusManager.setComplete(jobId, "WARNINGS: " + warn);
                }
            }
            catch (Exception ex) {
                jobStatusManager.setFailed(jobId, ex.getMessage());
                String msg = "Failure ingesting resource: " + ex.getMessage();
                throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Failure resource ingestion: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return jobId;
    }

    private JSONObject createCommandObj(String name, String zoomList, int rasterSize, String userEmail, long mapId) {
        JSONArray commandArgs = new JSONArray();
        JSONObject arg = new JSONObject();
        arg.put("RASTER_OUTPUT_DIR", TILE_SERVER_PATH);
        commandArgs.add(arg);

        arg = new JSONObject();
        arg.put("INPUT", name);
        commandArgs.add(arg);

        arg = new JSONObject();
        arg.put("ZOOM_LIST", zoomList);
        commandArgs.add(arg);

        arg = new JSONObject();
        arg.put("RASTER_SIZE", String.valueOf(rasterSize));
        commandArgs.add(arg);

        arg = new JSONObject();
        arg.put("MAP_ID", String.valueOf(mapId));
        commandArgs.add(arg);

        if (userEmail != null) {
            arg = new JSONObject();
            arg.put("USER_EMAIL", userEmail);
            commandArgs.add(arg);
        }

        JSONObject jsonArgs = _createPostBody(commandArgs);
        jsonArgs.put("erroraswarning", "true");

        return jsonArgs;
    }

    private String createCommand(String name, String zoomList, int rasterSize, long mapId) {
        return createCommandObj(name, zoomList, rasterSize, null, mapId).toJSONString();
    }

    private JSONObject getZoomInfo(double maxDelta) {
        JSONObject zoomInfo = new JSONObject();
        int rasterSize = 500;

        // losely based on http://wiki.openstreetmap.org/wiki/Zoom_levels
        int zoom = 0;
        if (maxDelta >= 360.0) {
            rasterSize = 1900;
            zoom = 0;
        }
        else if ((maxDelta < 360.0) && (maxDelta >= 180.0)) {
            rasterSize = 1800;
            zoom = 1;
        }
        else if ((maxDelta < 180.0) && (maxDelta >= 90.0)) {
            rasterSize = 1700;
            zoom = 2;
        }
        else if ((maxDelta < 90.0) && (maxDelta >= 45.0)) {
            rasterSize = 1600;
            zoom = 3;
        }
        else if ((maxDelta < 45.0) && (maxDelta >= 22.5)) {
            rasterSize = 1500;
            zoom = 4;
        }
        else if ((maxDelta < 22.5) && (maxDelta >= 11.25)) {
            rasterSize = 1400;
            zoom = 5;
        }
        else if ((maxDelta < 11.25) && (maxDelta >= 5.625)) {
            rasterSize = 1300;
            zoom = 6;
        }
        else if ((maxDelta < 5.625) && (maxDelta >= 2.813)) {
            rasterSize = 1200;
            zoom = 7;
        }
        else if ((maxDelta < 2.813) && (maxDelta >= 1.406)) {
            rasterSize = 1100;
            zoom = 8;
        }
        else if ((maxDelta < 1.406) && (maxDelta >= 0.703)) {
            rasterSize = 1000;
            zoom = 9;
        }
        else if ((maxDelta < 0.703) && (maxDelta >= 0.352)) {
            rasterSize = 900;
            zoom = 10;
        }
        else if ((maxDelta < 0.352) && (maxDelta >= 0.176)) {
            rasterSize = 800;
            zoom = 11;
        }
        else if ((maxDelta < 0.176) && (maxDelta >= 0.088)) {
            rasterSize = 700;
            zoom = 12;
        }
        else if ((maxDelta < 0.088) && (maxDelta >= 0.044)) {
            rasterSize = 600;
            zoom = 13;
        }
        else if ((maxDelta < 0.044) && (maxDelta >= 0.022)) {
            rasterSize = 500;
            zoom = 14;
        }
        else if ((maxDelta < 0.022) && (maxDelta >= 0.011)) {
            rasterSize = 400;
            zoom = 15;
        }
        else if ((maxDelta < 0.005) && (maxDelta >= 0.003)) {
            rasterSize = 300;
            zoom = 16;
        }
        else if ((maxDelta < 0.003) && (maxDelta >= 0.001)) {
            rasterSize = 200;
            zoom = 17;
        }
        else if ((maxDelta < 0.001) && (maxDelta >= 0.0005)) {
            rasterSize = 200;
            zoom = 18;
        }
        else if (maxDelta < 0.0005) {
            rasterSize = 200;
            zoom = 19;
        }

        if (zoom < 17) {
            zoom += 3;
        }
        else {
            zoom = 19;
        }

        String zoomList = "";
        for (int i = 0; i < zoom; i++) {
            if (!zoomList.isEmpty()) {
                zoomList += " ";
            }
            zoomList += i + "-" + (i + 1);
            i += 1;
        }

        zoomInfo.put("zoomlist", zoomList);
        zoomInfo.put("rastersize", rasterSize);

        return zoomInfo;
    }
}
