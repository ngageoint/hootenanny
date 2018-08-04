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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

// import static hoot.services.HootProperties.*;

// import java.io.File;
// import java.io.IOException;
import java.util.HashMap;
// import java.util.LinkedList;
// import java.util.List;

// import org.apache.commons.io.FileUtils;
// import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.ExternalCommand;
// import hoot.services.command.common.UnTARFileCommand;
// import hoot.services.geo.BoundingBox;
// import hoot.services.models.osm.Map;
// import hoot.services.utils.DbUtils;


class GrailCommand extends ExternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(GrailCommand.class);

    private final GrailParams params;

    GrailCommand(String jobId, GrailParams grailParams) {
        super(jobId);
        this.params = grailParams;
    }

    GrailCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        this(jobId, params);

        java.util.Map<String, Object> substitutionMap = new HashMap<>();

        String command = "echo Grail!!";

        super.configureCommand(command, substitutionMap, caller);
    }

}
