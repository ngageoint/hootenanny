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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.wps;

import hoot.services.HootProperties;

import java.io.IOException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * @author Jong Choi
 * 
 * WPS processlet for ETL operation. The operation is done using make file.
 * See hoot-services.conf for dependencies.
 * Please note that the arguments used by the make file is driven by ETLProcesslet.xml
 *
 */
public class ETLProcesslet extends JobProcesslet {
	
  private static final Logger log = LoggerFactory.getLogger(ETLProcesslet.class);
  
  private String makefileName = null;
	/**
	 * Constructor.
	 * Configures the makefile name through hoot-services.conf so it can modified
	 * externally.
	 */
	public ETLProcesslet() throws Exception {
		try {
    	makefileName = HootProperties.getInstance().getProperty("ETLMakefile",
		          HootProperties.getDefault("ETLMakefile"));
    	this.setProcessScriptName(makefileName);
			} catch (IOException e) {
				log.error(e.getMessage());
			}	
	}
		
}
