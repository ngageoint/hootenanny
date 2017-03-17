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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.util.Map;
import java.util.stream.Collectors;


public class ExportWFSCommand extends ExportCommand {

    ExportWFSCommand(String jobId, Map<String, String> paramMap, String debugLevel, Class<?> caller) {
        super(jobId, paramMap, debugLevel, caller);

        String input = super.getInput();

        String pgUrl = "host='" + HOOTAPI_DB_HOST + "' port='" + HOOTAPI_DB_PORT + "' user='" + HOOTAPI_DB_USER + "'" +
                       " password='" + HOOTAPI_DB_PASSWORD + "' dbname='" + WFS_STORE_DB + "'";

        String translationScript = "\"" + new File(TRANSLATION_SCRIPT_PATH, paramMap.get("translation")).getAbsolutePath() + "\"";
        String removeReviewSwitch = "-C RemoveReview2Pre.conf";
        String hootOptions = super.getHootOptions().stream().collect(Collectors.joining(" "));

        //hoot osm2ogr -C RemoveReview2Pre.conf -D ogr.writer.pre.layer.name=$(outputname)_ $(HOOT_OPTS) "$(OP_TRANSLATION)" "$(INPUT_PATH)" $(OP_PG_URL)
        String command = "hoot osm2ogr --" + debugLevel + " " + removeReviewSwitch + " " + hootOptions + " " + translationScript + " " + input + " " + pgUrl;

        super.configureAsHootCommand(command, caller);
    }
}
