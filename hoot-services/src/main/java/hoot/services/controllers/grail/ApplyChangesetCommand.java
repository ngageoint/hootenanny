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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import hoot.services.models.db.Users;


/**
 * Used to construct a changeset-apply command
 */
class ApplyChangesetCommand extends GrailCommand {

    ApplyChangesetCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);

        Users user = params.getUser();

        List<String> options = new LinkedList<>();
        //append any tags from UI
        options.add("changeset.description=" + params.getComment());
        options.add("changeset.hashtags=" + params.getHashtags());
        options.add("changeset.source=" + params.getSource());
        options.add("hoot.osm.auth.access.token=" + user.getProviderAccessKey());
        options.add("hoot.osm.auth.access.token.secret=" + user.getProviderAccessToken());
        options.add("hoot.osm.auth.consumer.key=" + params.getConsumerKey());
        options.add("hoot.osm.auth.consumer.secret=" + params.getConsumerSecret());

        List<String> hootOptions = toHootOptions(options);

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("OSC_FILE", params.getOutput());
        substitutionMap.put("API_URL", params.getPushUrl());
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("DEBUG_LEVEL", debugLevel);

        String command = "hoot changeset-apply --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${OSC_FILE} ${API_URL} --stats --progress";

        super.configureCommand(command, substitutionMap, caller);
    }
}
