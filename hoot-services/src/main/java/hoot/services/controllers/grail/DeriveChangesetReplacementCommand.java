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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.CHANGESET_OPTION_KEYS;
import static hoot.services.HootProperties.HOME_FOLDER;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.commons.io.FileUtils;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;


/**
 * Used to construct a changeset-derive-replacement command
 */
class DeriveChangesetReplacementCommand extends GrailCommand {

    protected static Map<String, String> advOptionKeys = null;

    static {
        try {
            ObjectMapper mapper = new ObjectMapper();
            TypeReference<?> schema = new TypeReference<Map<String, String>>(){};
            // get json of advanced options
            String file = FileUtils.readFileToString(new File(HOME_FOLDER, CHANGESET_OPTION_KEYS), Charset.defaultCharset());
            advOptionKeys = mapper.readValue(file, schema);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    DeriveChangesetReplacementCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);

        List<String> options = new LinkedList<>();
        options.add("api.db.email=" + params.getUser().getEmail());

        List<String> hootOptions = toHootOptions(options);

        Map<String, String> advOptions = params.getAdvancedOptions();
        List<String> advancedOptions = new ArrayList<>();

        if (advOptions != null && !advOptions.isEmpty()) {
            for (Entry<String, String> option: advOptions.entrySet()) {
                if (advOptionKeys.containsKey(option.getKey()) && option.getValue().equalsIgnoreCase("true")) { // if option key in possible values, add new option value
                    advancedOptions.add(advOptionKeys.get(option.getKey()));
                }
            }
        }

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("INPUT1", params.getInput1());
        substitutionMap.put("INPUT2", params.getInput2());
        substitutionMap.put("BOUNDS", params.getBounds());
        substitutionMap.put("OSC_FILE", params.getOutput());
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("ADV_OPTIONS", advancedOptions);
        substitutionMap.put("STATS_FILE", new File(params.getWorkDir(), "stats.json").getPath());

        String command = "hoot.bin changeset-derive-replacement --${DEBUG_LEVEL} -C DeriveChangeset.conf ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${BOUNDS} ${OSC_FILE} ${ADV_OPTIONS} --stats ${STATS_FILE}";

        super.configureCommand(command, substitutionMap, caller);
    }

}
