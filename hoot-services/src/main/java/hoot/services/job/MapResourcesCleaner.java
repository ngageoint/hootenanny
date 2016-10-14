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

import java.util.UUID;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.utils.DbUtils;


@Service
@Transactional
public class MapResourcesCleaner {
    private static final Logger logger = LoggerFactory.getLogger(MapResourcesCleaner.class);

    @Autowired
    private JobStatusManager jobStatusManager;

    public MapResourcesCleaner() {}

    public String exec(String mapId) {
        return deleteLayers(mapId);
    }

    private String deleteLayers(String mapId) {
        String jobId = UUID.randomUUID().toString();

        try {
            jobStatusManager.addJob(jobId);

            DbUtils.deleteBookmarksById(mapId);
            DbUtils.deleteRenderDb(mapId);
            DbUtils.deleteOSMRecordByName(mapId);

            jobStatusManager.setComplete(jobId);
        }
        catch (Exception e) {
            jobStatusManager.setFailed(jobId, e.getMessage());
            String msg = "Error deleting layers for map with mapId = " +  mapId;
            throw new RuntimeException(msg, e);
        }

        return jobId;
    }
}
