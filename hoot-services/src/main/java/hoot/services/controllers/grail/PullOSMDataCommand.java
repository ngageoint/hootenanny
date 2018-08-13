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

// import java.io.File;
// import java.io.FileUtils;
import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.geo.BoundingBox;


class PullOSMDataCommand extends GrailCommand {
    private static final Logger logger = LoggerFactory.getLogger(PullOSMDataCommand.class);

    PullOSMDataCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId,params);

        // Testing: logger.info("PullOSMCommand: " + params.toString());

        BoundingBox boundingBox = new BoundingBox(params.getBounds());
        // double bboxArea = boundingBox.getArea();

        // double maxBboxArea = params.getMaxBBoxSize();

        // if (bboxArea > maxBboxArea) {
        //     throw new IllegalArgumentException("The bounding box area (" + bboxArea + ") is too large. It must be less than " + maxBboxArea + " degrees");
        // }

        // String fullUrl = params.getPullUrl() + "/map?bbox=" + boundingBox.toOverpassString();

        // Map<String, Object> substitutionMap = new HashMap<>();
        // substitutionMap.put("OUTPUT_FILE", params.getOutput());
        // substitutionMap.put("API_URL", fullUrl);

        // // wget -O <output.osm> "http://api.openstreetmap.org/api/0.6/map?bbox=$EXTENT"
        // String command = "wget -O ${OUTPUT_FILE} ${API_URL}";

        String fullUrl = params.getPullUrl() + "/api/interpreter?data='(node(" + boundingBox.toOverpassString() + ");<;>;);out meta qt;'";

        logger.info("PullOSMCommand: " + fullUrl);


        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("OUTPUT_FILE", params.getOutput());
        substitutionMap.put("API_URL", fullUrl);

        // wget -O <output.osm> "http://api.openstreetmap.org/api/0.6/map?bbox=$EXTENT"
        String command = "wget -O ${OUTPUT_FILE} ${API_URL}";

        super.configureCommand(command, substitutionMap, caller);
    }

    /*

    wget -O op.osm https://overpass-api.de/api/interpreter?data='(node(-34.0044,150.9982,-33.9728,151.0656);<;>;);out meta;'

    curl --globoff -o output.xml http://overpass-api.de/api/interpreter?data=node(1);out;
    where the previously crafted query comes after data= and the query needs to be urlencoded. 
    The --globoff is important in order to use square and curly brackets without being interpreted by curl. 


public static synchronized void downloadINI(CoreFrame cf, String path){
Operations.LogSetup(log,true);
    File ini=new File(path+"\\bin64\\arcdps.ini");
    if (ini.exists()) ini.delete();         //Delete existing ini file to prevent an exception
    log.log( Level.INFO,"Downloading configuration file");
    try {
        //Download default configuration from the website
    FileUtils.copyURLToFile(new URL("http://www.deltaconnected.com/arcdps/x64/arcdps.ini"),ini, 10000, 10000);
    log.log( Level.INFO,"archdps.ini installed successfully");
//Exceptions if something goes wrong (Connection/IO)
    } catch (IOException e) {
    
    e.printStackTrace();
    cf.status.setText("- Cannot connect to the update server");
          cf.status.setForeground(Color.RED);
          log.log( Level.SEVERE,"IOException when downloading ini");
}
    Operations.closeLogHandlers(log);
  }
    
    */
}
