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
package hoot.services;

import hoot.services.controllers.ingest.BasemapResource;
import hoot.services.controllers.ogr.TranslatorResource;
import hoot.services.controllers.services.P2PResource;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;;

public class HootServletContext implements ServletContextListener {

	private TranslatorResource _transRes = null;
	private P2PResource _P2PRes = null;
	private BasemapResource _BRes = null;
	public void contextInitialized(ServletContextEvent arg0) 
	{
		_transRes = new TranslatorResource();
		_transRes.startTranslationService();
		
		_P2PRes = new P2PResource();
		_P2PRes.startP2PService();
		
		// Doing this to make sure we create ingest folder
		_BRes = new BasemapResource();
		_BRes.createTileServerPath();
	}
	
	public void contextDestroyed(ServletContextEvent arg0) 
	{
		_transRes.stopTranslationService();
		_P2PRes.stopP2PService();
	}
}
