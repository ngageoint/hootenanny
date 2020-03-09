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

import static hoot.services.HootProperties.PRIVATE_OVERPASS_CERT_PATH;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_CERT_PHRASE;
import static hoot.services.HootProperties.PRIVATE_OVERPASS_URL;
import static hoot.services.HootProperties.replaceSensitiveData;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.KeyStore;
import java.time.LocalDateTime;

import javax.net.ssl.SSLContext;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.conn.ssl.SSLContexts;
import org.apache.http.impl.client.HttpClients;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.geo.BoundingBox;


/**
 * Used for pulling Private Rails Port API data
 */
class PullApiCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(PullApiCommand.class);

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
            BoundingBox boundingBox = new BoundingBox(params.getBounds());
            //buffer the reference data bounding box
            //to include neighboring data that may be snapped to
// Disable this for now as pulling a buffered extent does not guarantee that
// connected ways will be present in the output
//            boundingBox.adjust(Double.parseDouble(CHANGESET_DERIVE_BUFFER));

            double bboxArea = boundingBox.getArea();

            double maxBboxArea = params.getMaxBBoxSize();

            if (bboxArea > maxBboxArea) {
                throw new IllegalArgumentException("The bounding box area (" + bboxArea +
                        ") is too large. It must be less than " + maxBboxArea + " degrees");
            }

            InputStream responseStream = null;
            // Checks to see the set pull url is for the private overpass url
            if (params.getPullUrl().equals(PRIVATE_OVERPASS_URL)) {
                url = PullOverpassCommand.getOverpassUrl(replaceSensitiveData(params.getPullUrl()), boundingBox.toServicesString(), "xml", params.getCustomQuery());

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
                url = replaceSensitiveData(params.getPullUrl()) + "?bbox=" + boundingBox.toServicesString();
            }

            File outputFile = new File(params.getOutput());

            if (responseStream == null) {
                responseStream = GrailResource.getUrlInputStreamWithNullHostnameVerifier(url);
            }

            FileUtils.copyInputStreamToFile(responseStream, outputFile);
            responseStream.close();
        }
        catch (IOException ex) {
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
        HttpResponse response = httpClient.execute(new HttpGet(url));
        HttpEntity entity = response.getEntity();

        return entity.getContent();
    }
}
