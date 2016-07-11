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

import java.sql.Connection;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.db.DbUtils;
import hoot.services.job.Executable;


public class ResourcesCleanUtil implements Executable {
    private static final Logger logger = LoggerFactory.getLogger(ResourcesCleanUtil.class);

    private String finalStatusDetail;

    @Override
    public String getFinalStatusDetail() {
        return finalStatusDetail;
    }

    public void init() {
    }

    public void destroy() {
    }

    public ResourcesCleanUtil() {
    }

    @Override
    public void exec(JSONObject command) throws Exception {
        JSONObject res = deleteLayers(command.get("mapId").toString());
        finalStatusDetail = res.toJSONString();
    }

    public static JSONObject deleteLayers(String mapId) throws Exception {
        JSONObject res = new JSONObject();
        res.put("mapId", mapId);
        res.put("result", "success");

        try (Connection conn = DbUtils.createConnection()){
            DbUtils.deleteBookmarksById(conn, mapId);
            DbUtils.deleteRenderDb(conn, mapId);
            DbUtils.deleteOSMRecordByName(conn, mapId);
        }
        catch (Exception e) {
            logger.error("Error deleting layer with mapId = {}", mapId, e);
            throw e;
        }

        return res;
    }
}
