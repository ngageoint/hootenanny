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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.sql.Connection;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.job.Executable;


public class ResourcesCleanUtil implements Executable {
    private static final Logger logger = LoggerFactory.getLogger(ResourcesCleanUtil.class);

    private static final String ingestPath;

    static {
        ingestPath = HootProperties.getProperty("tileServerPath");
    }

    private ClassPathXmlApplicationContext appContext;

    private String finalStatusDetail;

    @Override
    public String getFinalStatusDetail() {
        return finalStatusDetail;
    }

    public ResourcesCleanUtil() {
        appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
    }

    @Override
    public void exec(JSONObject command) throws Exception {
        JSONObject res = deleteLayers(command.get("mapId").toString());
        finalStatusDetail = res.toJSONString();
    }

    public JSONObject deleteLayers(final String mapId) throws Exception {
        JSONObject res = new JSONObject();
        res.put("mapId", mapId);
        res.put("result", "success");
        Connection conn = DbUtils.createConnection();
        try {
            logger.debug("Initializing database connection...");

            // List<Long> ids = DbUtils.getMapIdsByName( conn, mapId);
            // int nMapCnt = ids.size();

            DbUtils.deleteRenderDb(conn, mapId);
            DbUtils.deleteOSMRecordByName(conn, mapId);
            // Modify when core implements broad casting map id when conflation
            // completes
            // _deleteIngestResource(mapId, nMapCnt);
        }
        catch (Exception e) {
            logger.error(e.getMessage());
            throw e;
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        return res;
    }

    // TODO: Change mapName to mapId
    protected void _deleteIngestResource(String mapName, int nMapCnt)
            throws NullPointerException, FileNotFoundException, IOException, Exception {

        Connection conn = DbUtils.createConnection();
        try {
            logger.debug("Initializing database connection...");

            // we will not delete resource for layer with duplicate names
            if (nMapCnt == 1) {
                // This block is to check if we have file path manipulation by
                // validating
                // the new path is within container path
                String basePath = ingestPath;
                String newPath = ingestPath + "/" + mapName;

                // Fortify fix
                if (!hoot.services.utils.FileUtils.validateFilePath(ingestPath, newPath)) {
                    throw new Exception("Map name can not contain path.");
                }

                // Fortify fix
                if (!hoot.services.utils.FileUtils.validateFilePath(ingestPath, newPath)) {
                    throw new Exception("Map name can not contain path.");
                }

                boolean isValidated = false;
                File fDel = new File(newPath);
                String potentialPath = fDel.getCanonicalPath();

                File fBase = new File(basePath);
                String containerPath = fBase.getCanonicalPath();

                // verify that newPath is within basePath
                if (potentialPath.indexOf(containerPath) == 0) {
                    isValidated = true;
                }

                // If it is safe to delete then delete
                if (isValidated) {
                    org.apache.commons.io.FileUtils.forceDelete(fDel);
                }

            }
        }
        catch (NullPointerException npe) {
            logger.error(npe.getMessage());
            throw npe;
        }
        catch (FileNotFoundException fne) {
            logger.error(fne.getMessage());
            throw fne;
        }
        catch (IOException ioe) {
            logger.error(ioe.getMessage());
            throw ioe;
        }
        finally {
            DbUtils.closeConnection(conn);
        }
    }

    /**
     * see CoreServiceContext.xml
     */
    public void init() {
        //
    }

    /**
     * see CoreServiceContext.xml
     */
    public void destroy() {
        //
    }
}
