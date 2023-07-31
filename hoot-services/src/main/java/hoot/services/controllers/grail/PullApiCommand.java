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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.GRAIL_CONNECTED_WAYS_QUERY;
import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_CERT_PATH;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_CERT_PHRASE;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_URL;
import static hoot.services.HootProperties.replaceSensitiveData;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.net.URLDecoder;
import java.security.KeyStore;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.net.ssl.SSLContext;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.ssl.SSLContexts;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicNameValuePair;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.geo.BoundingBox;


/**
 * Used for pulling Private Rails Port API data
 */
public class PullApiCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(PullApiCommand.class);

    // matches [xxx] pattern but without timeout or maxsize
    public static Pattern overpassqlFilterPattern = Pattern.compile(".+\\[((?!timeout|maxsize).*?)\\]");
    // matches the [out:json] with optional timeout and maxsize props
    public static String overpassqlFormatPattern = "^\\[out:(json|xml)\\].*";

    private final GrailParams params;
    private final String jobId;
    private final Class<?> caller;

    PullApiCommand(GrailParams params, String jobId, Class<?> caller) {
        this.params = params;
        this.jobId = jobId;
        this.caller = caller;

        logger.info("Params: "+ params.toString());

    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setJobId(jobId);
        commandResult.setCommand("[Pull OSM Data from Api DB] for " + params.getBounds());
        commandResult.setStart(LocalDateTime.now());
        commandResult.setCaller(caller.getName());

        getApiData();

        commandResult.setFinish(LocalDateTime.now());
        commandResult.setExitCode(CommandResult.SUCCESS);

        return commandResult;
    }

    private void getApiData() {
        String url = "";
        try {
            InputStream responseStream = null;
            // Checks to see the set pull url is for the private overpass url
            if (params.getPullUrl().equals(PRIVATE_OVERPASS_URL)) {
                url = PullOverpassCommand.getOverpassUrl(replaceSensitiveData(params.getPullUrl()), params.getBounds(), "xml", params.getCustomQuery());

                // if cert path and phrase are specified then we assume to use them for the request
                // this will have a valid cert so no need to support self-signed
                if (!replaceSensitiveData(PRIVATE_OVERPASS_CERT_PATH).equals(PRIVATE_OVERPASS_CERT_PATH)) {
                    try {
                        responseStream = getHttpResponseWithSSL(url);
                    } catch (Exception e) {
                        String msg = "Failure to pull data from the private OSM API with the specified cert. [" + url + "]" + e.getMessage();
                        throw new WebApplicationException(e, Response.serverError().entity(msg).build());
                    }
                }
            } else {
                BoundingBox boundingBox = new BoundingBox(params.getBounds());
                double bboxArea = boundingBox.getArea();
                double maxBboxArea = params.getMaxBoundsSize();

                if (bboxArea > maxBboxArea) {
                    String errorMsg = "The bounding box area (" + bboxArea + ") is too large for OSM API. It must be less than " + maxBboxArea + " degrees";
                    throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(errorMsg).build());
                }

                url = replaceSensitiveData(params.getPullUrl()) + "?bbox=" + boundingBox.toServicesString();
            }

            File outputFile = new File(params.getOutput());

            if (responseStream == null) {
                responseStream = GrailResource.getUrlInputStreamWithNullHostnameVerifier(url);
            }

            FileUtils.copyInputStreamToFile(responseStream, outputFile);
            responseStream.close();
        }
        catch (Exception ex) {
            String msg = "Failure to pull data from the OSM API [" + url + "] " + ex.getMessage();
            logger.error(msg);
            throw new WebApplicationException(ex, Response.serverError().entity(ex.getMessage()).build());
        }

    }

    public static InputStream getHttpResponseWithSSL(String url) throws Exception {
        String certPath = replaceSensitiveData(PRIVATE_OVERPASS_CERT_PATH);
        String keyPassphrase = replaceSensitiveData(PRIVATE_OVERPASS_CERT_PHRASE);

        KeyStore keyStore = KeyStore.getInstance("PKCS12");
        keyStore.load(new FileInputStream(certPath), keyPassphrase.toCharArray());
        SSLContext sslContext = SSLContexts.custom()
                .loadKeyMaterial(keyStore, keyPassphrase.toCharArray())
                .build();

        HttpClient httpClient = HttpClients.custom().setSslcontext(sslContext).build();

        String[] splitUrl = url.split("data=");
        HttpPost post = new HttpPost(splitUrl[0]);

        // Just a safety check but splitUrl[1] should be the query data
        if (splitUrl.length == 2) {
            logger.info(splitUrl[0]);
            logger.info(URLDecoder.decode(splitUrl[1], "UTF-8"));
            List<NameValuePair> params = new ArrayList<>();
            params.add(new BasicNameValuePair("data", URLDecoder.decode(splitUrl[1], "UTF-8")));
            post.setEntity(new UrlEncodedFormEntity(params));
        }

        HttpResponse response = httpClient.execute(post);
        HttpEntity entity = response.getEntity();

        return entity.getContent();
    }

    /**
     * String query that will retrieve the connected ways.
     *
     * @param query if query is provided then we append the pull connected ways query to the end.
     *              else we get the default overpass query and append the pull connected ways query to that.
     *
     * @return String query that will retrieve the connected ways.
     */
    static String connectedWaysQuery(String query) {
        String newQuery;
        List<String> filterList = new ArrayList<>();

        // if no query provided then use default overpass query
        if (query == null || query.equals("")) {
            newQuery = PullOverpassCommand.getDefaultOverpassQuery();
        } else {
            newQuery = query;
            //check for any filters in custom query
            //find the first term within square brackets
            Matcher matcher = overpassqlFilterPattern.matcher(query);

            while (matcher.find()) {
                filterList.add(matcher.group(1));
            }
        }

        // connected ways query
        String connectedWaysQuery;
        File connectedWaysQueryFile = new File(HOME_FOLDER, GRAIL_CONNECTED_WAYS_QUERY);
        try {
            connectedWaysQuery = FileUtils.readFileToString(connectedWaysQueryFile, "UTF-8");
            //swap in filter term to connected ways query
            if (filterList.size() > 0) {
                // creating union for all queries created from the filter list
                String filter = "(\n";
                for (String filterOpt : filterList) {
                    filter += "way[" + filterOpt + "](bn.oobnd);\n";
                }
                filter += ");\n";
                connectedWaysQuery = connectedWaysQuery.replace("way(bn.oobnd);", filter);
            }

        } catch(Exception exc) {
            throw new IllegalArgumentException("Grail pull connected ways error. Couldn't read connected ways overpass query file: " + connectedWaysQueryFile.getName());
        }

        if (newQuery.lastIndexOf("out meta;") != -1) {
            newQuery = newQuery.substring(0, newQuery.lastIndexOf("out meta;")) + connectedWaysQuery;
        } else if (newQuery.lastIndexOf("out count;") != -1) {
            newQuery = newQuery.substring(0, newQuery.lastIndexOf("out count;")) + connectedWaysQuery;
        }

        return newQuery;
    }
}
