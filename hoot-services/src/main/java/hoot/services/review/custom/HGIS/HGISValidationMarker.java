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
package hoot.services.review.custom.HGIS;

import java.sql.Connection;
import java.sql.SQLException;

import hoot.services.db2.QMaps;
import hoot.services.exceptions.review.custom.HGIS.ReviewMapTagUpdateException;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.writers.review.custom.HGIS.HGISValidationDbWriter;


public class HGISValidationMarker {
    private long mapId;
    private Connection conn;

    public HGISValidationMarker(final Connection cn, final String mapName) throws Exception {
        this.conn = cn;

        QMaps maps = QMaps.maps;
        this.mapId = ModelDaoUtils.getRecordIdForInputString(mapName, conn, maps, maps.id, maps.displayName);
    }

    public void updateValidationMapTag() throws SQLException, ReviewMapTagUpdateException {
        HGISValidationDbWriter writer = new HGISValidationDbWriter(conn, mapId);
        writer.updateMapTagWithReviewType();
    }
}
