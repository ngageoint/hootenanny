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
package hoot.services.testsupport;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.io.FileUtils;

import hoot.services.controllers.osm.ChangesetDbWriter;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;


/*
 * Various utilities for review process testing
 */
public class ReviewTestUtils {
    public Map<ElementType, Map<Long, Element>> parsedElementIdsToElementsByType;

    /**
     * Writes the conflated review relation output for conflating
     * AllDataTypesA.osm with AllDataTypesB.osm
     * 
     * @throws Exception
     */
    public long populateReviewDataForAllDataTypes(long mapId, long userId) throws Exception {
        // write the reviewable data to the OSM tables
        long changesetId = Changeset.insertNew(mapId, userId, new HashMap<String, String>());
        ChangesetDbWriter elementWriter = new ChangesetDbWriter();
        /* final Document response = */
        elementWriter.write(mapId, changesetId,
                FileUtils
                        .readFileToString(new File(Thread.currentThread().getContextClassLoader()
                                .getResource("hoot.services.review/allDataTypesConflatedOut.osm").getPath()))
                        .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\""));
        parsedElementIdsToElementsByType = elementWriter.getParsedElementIdsToElementsByType();
        return changesetId;
    }
}
