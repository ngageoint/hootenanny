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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.GRAIL_OVERPASS_QUERY;
import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.PUBLIC_OVERPASS_URL;
import static hoot.services.HootProperties.replaceSensitiveData;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.time.LocalDateTime;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.geo.BoundingBox;


/**
 * Used for pulling OSM data from Overpass
 */
class PullOverpassCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(PullOverpassCommand.class);

    private final GrailParams params;
    private final String jobId;
    private final Class<?> caller;

    PullOverpassCommand(GrailParams params, String jobId, Class<?> caller) {
        this.params = params;
        this.jobId = jobId;
        this.caller = caller;

        logger.info("Params: "+ params.toString());
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setJobId(jobId);
        commandResult.setCommand("[Pull OSM Data from Overpass] for " + params.getBounds());
        commandResult.setStart(LocalDateTime.now());
        commandResult.setCaller(caller.getName());

        getOverpass();

        commandResult.setFinish(LocalDateTime.now());
        commandResult.setExitCode(CommandResult.SUCCESS);

        return commandResult;
    }

    private void getOverpass() {
        String url = "";
        try {
            String customQuery = params.getCustomQuery();
            if (customQuery == null || customQuery.equals("")) {
                url = replaceSensitiveData(getOverpassUrl(replaceSensitiveData(params.getPullUrl()), params.getBounds(), "xml", null));
            } else {
                url = replaceSensitiveData(getOverpassUrl(replaceSensitiveData(params.getPullUrl()), params.getBounds(), "xml", customQuery));
            }

            File outputFile = new File(params.getOutput());
            //write to intermediate file
            InputStream is = PullOverpassCommand.getOverpassInputStream(url);
            FileUtils.copyInputStreamToFile(is, outputFile);
            is.close();

        }
        catch (Exception ex) {
            String msg = "Failure to pull data from Overpass [" + url + "]" + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }
    }

    /**
     * Returns the overpass query, with the expected output format set to json
     * @param bounds
     * @param outputFormat if set to 'xml' then the output of the returned query, when run, will be xml. json is the default if non xml is specified
     * @param query optional custom overpass query
     *
     * @return overpass query url
     */
    static String getOverpassUrl(String overpassUrl, String bounds, String outputFormat, String query) {
        // Get grail overpass query from the file and store it in a string
        String overpassQuery;

        if (query == null || query.equals("")) {
            overpassQuery = getDefaultOverpassQuery();
        } else {
            overpassQuery = query;
        }

        //replace the {{bbox}} from the overpass query with the actual coordinates and encode the query
        if (bounds != null) {
            // polygon contains coordinates separated by ';'
            if (bounds.contains(";")) {
                // We need to reverse the coordinates from lon,lat to lat,long for overpass
                String polyBounds = PullOverpassCommand.boundsToOverpassPolyString(bounds);
                overpassQuery = overpassQuery.replace("{{bbox}}", "poly:\"" + polyBounds + "\"");
            } else {
                overpassQuery = overpassQuery.replace("{{bbox}}", new BoundingBox(bounds).toOverpassString());
            }
        }

        if (outputFormat.equals("xml") && overpassQuery.contains("out:json")) {
            overpassQuery = overpassQuery.replace("out:json", "out:xml"); // Need this because the rails pull data is also xml
        } else if (outputFormat.equals("json") && overpassQuery.contains("out:xml")) {
            overpassQuery = overpassQuery.replace("out:xml", "out:json");
        }

        try {
            overpassQuery = URLEncoder.encode(overpassQuery, "UTF-8").replace("+", "%20");
        } catch (UnsupportedEncodingException ignored) {} // Can be safely ignored because UTF-8 is always supported

        return overpassUrl + "?data=" + overpassQuery;
    }

    static InputStream getOverpassInputStream(String url) throws IOException {
        InputStream inputStream;
        if ("https://overpass-api.de/api/interpreter".equalsIgnoreCase(replaceSensitiveData(PUBLIC_OVERPASS_URL))) {
            String[] splitUrl = url.split("(?=data)"); // prevents removal of 'data' text
            URL urlReq = new URL(splitUrl[0]);
            HttpURLConnection connection = (HttpURLConnection) urlReq.openConnection();
            connection.setRequestMethod("POST");
            connection.setDoOutput(true);

            // Just a safety check but splitUrl[1] should be the query data
            if (splitUrl.length == 2) {
                connection.getOutputStream().write(splitUrl[1].getBytes(StandardCharsets.UTF_8));
            }

            inputStream = connection.getInputStream();
        } else { // add no cert checker if using a public mirror
            inputStream = GrailResource.getUrlInputStreamWithNullHostnameVerifier(url);
        }
        return inputStream;
    }

    static String getDefaultOverpassQuery() {
        String overpassQuery;

        File overpassQueryFile = new File(HOME_FOLDER, GRAIL_OVERPASS_QUERY);
        try {
            overpassQuery = FileUtils.readFileToString(overpassQueryFile, "UTF-8");
        } catch(Exception exc) {
            throw new IllegalArgumentException("Grail pull overpass error. Couldn't read overpass query file: " + overpassQueryFile.getName());
        }

        return overpassQuery;
    }

    // Overpass poly requires the coordinates to be in lat,long instead of the long,lat that we use everywhere else
    static String boundsToOverpassPolyString(String bounds) {
        // We need to reverse the coordinates from lon,lat to lat,long for overpass
        List<String> lonLatCoords = Arrays.asList(bounds.split("([,;])"));
        Collections.reverse(lonLatCoords);
        return lonLatCoords.toString().replaceAll("[\\[\\],]", ""); // removes brackets and commas
    }
}
