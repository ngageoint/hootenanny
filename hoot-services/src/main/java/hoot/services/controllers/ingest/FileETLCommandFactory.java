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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.ingest;


import java.util.List;

import org.json.simple.JSONArray;
import org.springframework.stereotype.Component;


@Component
class FileETLCommandFactory {

    FileETLCommand build(JSONArray reqList, int zipCnt, int shpZipCnt, int fgdbZipCnt, int osmZipCnt,
                         int geonamesZipCnt, int shpCnt, int fgdbCnt, int osmCnt, int geonamesCnt,
                         List<String> zipList, String translation, String jobId, String etlName,
                         List<String> inputsList, String userEmail, String isNoneTranslation, String fgdbFeatureClasses,
                         Class<?> caller) {
        return new FileETLCommand(reqList, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                                  shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, translation, jobId,
                                  etlName, inputsList, userEmail, isNoneTranslation, fgdbFeatureClasses, caller);
    }
}
