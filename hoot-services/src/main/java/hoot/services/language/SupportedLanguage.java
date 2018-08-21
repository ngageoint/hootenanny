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

package hoot.services.language;

/*
*/
public final class SupportedLanguage
{
  private String iso6391Code;
  private String iso6392Code;
  private String name;
  private boolean available = false;

  public SupportedLanguage() {}

  public String getIso6391code() { return iso6391Code; }
  public void setIso6391Code(String code) { this.iso6391Code = code; }

  public String getIso6392code() { return iso6392Code; }
  public void setIso6392Code(String code) { this.iso6392Code = code; }

  public String getName() { return name; }
  public void setName(String name) { this.name = name; }

  public boolean getAvailable() { return available; }
  public void setAvailable(boolean available) { this.available = available; }
}
