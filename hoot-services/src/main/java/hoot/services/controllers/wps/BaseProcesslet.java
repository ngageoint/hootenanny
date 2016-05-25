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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.wps;

import java.util.List;

import org.deegree.services.wps.Processlet;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.input.LiteralInput;
import org.deegree.services.wps.input.ProcessletInput;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import hoot.services.HootProperties;


/**
 * @author Jong Choi
 *         <p>
 *         Base class for WPS processlet
 */
public abstract class BaseProcesslet implements Processlet {
    private static final Logger log = LoggerFactory.getLogger(BaseProcesslet.class);

    protected static final String coreJobServerUrl = HootProperties.getPropertyOrDefault("coreJobServerUrl");

    private ClassPathXmlApplicationContext appContext;

    /**
     * @throws Exception
     */
    public BaseProcesslet() throws Exception {
        appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
    }

    /*
     * (non-Javadoc)
     *
     * @see org.deegree.services.wps.Processlet#destroy()
     */
    @Override
    public void destroy() {
        //
    }

    /*
     * (non-Javadoc)
     *
     * @see org.deegree.services.wps.Processlet#init()
     */
    @Override
    public void init() {
        //
    }

    /**
     * Parses WPS xml to JSON format.
     */
    @SuppressWarnings("unchecked")
    protected JSONArray parseRequestParams(ProcessletInputs in) {
        JSONArray commandArgs = new JSONArray();
        List<ProcessletInput> params = in.getParameters();

        for (ProcessletInput param : params) {
            JSONObject commandArg = new JSONObject();
            ProcessletInput input = param;
            String id = input.getIdentifier().getCode();
            String value = ((LiteralInput) in.getParameter(id)).getValue().trim();
            try {
                commandArg.put(id, value);
                commandArgs.add(commandArg);
            }
            catch (Exception e) {
                log.error(e.getMessage());
            }
        }
        return commandArgs;
    }

    public String getParameterValue(String key, JSONArray args) {
        for (Object arg : args) {
            JSONObject o = (JSONObject) arg;
            if (o.containsKey(key)) {
                return o.get(key).toString();
            }
        }
        return null;
    }

}
