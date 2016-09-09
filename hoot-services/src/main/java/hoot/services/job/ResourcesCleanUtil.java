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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.job;

import java.sql.Connection;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.utils.DbUtils;


public class ResourcesCleanUtil implements Executable {
    private static final Logger logger = LoggerFactory.getLogger(ResourcesCleanUtil.class);

    private String finalStatusDetail;

    @Override
    public String getFinalStatusDetail() {
        return finalStatusDetail;
    }

    public ResourcesCleanUtil() {}

    @Override
    public void exec(JSONObject command) {
        JSONObject json = deleteLayers(command.get("mapId").toString());
        this.finalStatusDetail = json.toJSONString();
    }

    private static JSONObject deleteLayers(String mapId) {
        try (Connection connection = DbUtils.createConnection()){
            DbUtils.deleteBookmarksById(connection, mapId);
            DbUtils.deleteRenderDb(connection, mapId);
            DbUtils.deleteOSMRecordByName(connection, mapId);
        }
        catch (Exception e) {
            String msg = "Error deleting layer with mapId = " +  mapId;
            throw new RuntimeException(msg, e);
        }

        JSONObject json = new JSONObject();
        json.put("mapId", mapId);
        json.put("result", "success");

        return json;
    }
}
