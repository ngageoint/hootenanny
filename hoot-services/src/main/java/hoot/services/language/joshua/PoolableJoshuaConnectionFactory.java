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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.language.joshua;

import java.net.Socket;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.utils.PoolableSocketConnectionFactory;

/**
 * Allows for creating poolable Joshua connections 
 *
   @article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015} }
 */
public class PoolableJoshuaConnectionFactory extends PoolableSocketConnectionFactory
{
  private static final Logger logger = 
    LoggerFactory.getLogger(PoolableJoshuaConnectionFactory.class);

  public PoolableJoshuaConnectionFactory(String host, int port, int timeout)
  {
    super(host, port, timeout);
  }

  public Object makeObject() throws Exception 
  {
    logger.trace("Making new connection...");
    return new JoshuaConnection(new Socket(host, port), timeout);
  }
}
