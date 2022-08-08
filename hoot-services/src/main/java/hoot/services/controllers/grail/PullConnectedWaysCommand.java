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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.replaceSensitiveData;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Attr;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;


/**
 * Used for pulling connected ways to nodes that fall outside the bounds for a reference layer
 */
class PullConnectedWaysCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(PullConnectedWaysCommand.class);

    private final GrailParams params;
    private final String jobId;
    private final Class<?> caller;

    PullConnectedWaysCommand(GrailParams params, String jobId, Class<?> caller) {
        this.params = params;
        this.jobId = jobId;
        this.caller = caller;

        logger.info("Params: "+ params.toString());

    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setJobId(jobId);
        commandResult.setCommand("[Pull connected ways] for " + params.getBounds());
        commandResult.setStart(LocalDateTime.now());
        commandResult.setCaller(caller.getName());

        getConnectedWays();

        commandResult.setFinish(LocalDateTime.now());
        commandResult.setExitCode(CommandResult.SUCCESS);

        return commandResult;
    }

    public static List<Long> getOsmXpath(InputStream is, String expression) {
        List<Long> nodeIds = new ArrayList<>();
        DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder;
        try {
            builder = builderFactory.newDocumentBuilder();
            Document xmlDocument;
            xmlDocument = builder.parse(is);
            XPath xPath = XPathFactory.newInstance().newXPath();
            NodeList nodeList = (NodeList) xPath.compile(expression).evaluate(xmlDocument, XPathConstants.NODESET);
            int length = nodeList.getLength();
            for( int i=0; i<length; i++) {
                Attr attr = (Attr) nodeList.item(i);
                Long id = Long.parseLong(attr.getValue());
                if (id >= 0) nodeIds.add(id);
            }
        } catch (ParserConfigurationException | SAXException | XPathExpressionException | IOException e1) {
            // TODO Auto-generated catch block
            e1.printStackTrace();
        }
        return nodeIds;
    }

    private void getConnectedWays() {
        String url = "";
        InputStream is;
        try {
            //Read the crop.osm file for node ids
            is = new FileInputStream(new File(params.getOutput()));
            List<Long> nodeIds = getOsmXpath(is, "/osm/node/@id");
            is.close();

            //Get the ways for those nodes
            //http://localhost:3000/api/0.6/node/6096481776/ways
            List<Long> allWayIds = new ArrayList<>();
            for (Long n : nodeIds) {
                url = replaceSensitiveData(params.getPullUrl()).replace("/api/0.6/map", "/api/0.6/node/" + n + "/ways");
                is = GrailResource.getUrlInputStreamWithNullHostnameVerifier(url);
                List<Long> wayIds = getOsmXpath(is, "/osm/way/@id");
                is.close();
                allWayIds.addAll(wayIds);
            }

            //Get the full ways
            //http://localhost:3000/api/0.6/way/649672297/full
            for (Long id : allWayIds) {
                url = replaceSensitiveData(params.getPullUrl()).replace("/api/0.6/map", "/api/0.6/way/" + id + "/full");

                File outputFile = new File(params.getWorkDir(), id + ".osm");
                is = GrailResource.getUrlInputStreamWithNullHostnameVerifier(url);
                FileUtils.copyInputStreamToFile(is, outputFile);
                is.close();
            }

            //delete the crop.osm file
            new File(params.getOutput()).delete();
        }
        catch (IOException ex) {
            String msg = "Failure to pull connected ways from the API [" + url + "]" + ex.getMessage();
            logger.error(msg);
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

    }

}
