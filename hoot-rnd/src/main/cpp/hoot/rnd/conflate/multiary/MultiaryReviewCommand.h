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
 * @copyright Copyright (C) 2017, 2018, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef __MULTIARY_REVIEW_COMMAND_H__
#define __MULTIARY_REVIEW_COMMAND_H__

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Element.h>

//QT
#include <QSet>
#include <QByteArray>

namespace hoot
{

/**
 * @brief The MultiaryReviewCommand class encapsulates the Multiary Review Command concept, as it
 * applies to Multiary POI Conflation. In a nutshell, the goal of this class is to capture the
 * decisions made by humans when they review Multiary POI Conflation results, store that state,
 * and re-apply it during subsequent conflations when input datasets are modified and reprocessed.
 *
 * The ultimate goal here is to keep from having to ask a user the same questions over and over,
 * any time data is added or modified to the POI source corpus.
 *
 * These commands operate on map elements (nodes for now) that are uniquely identified by thier
 * hoot:hash metadata tag. The hoot:hash is a sha1 hash - to see how it is generated, please have
 * a look at the MultiaryPoiHashVisitor class.
 */
class MultiaryReviewCommand
{
public:
  static QString className() { return "hoot::MultiaryReviewCommand"; }

  // Review operations. Feel free to extend.
  enum Operation
  {
    NoOp   = 0,
    Match  = 1,
    Miss   = 2,
    Modify = 3,
    Delete = 4
  };

  // Construtor
  MultiaryReviewCommand();

  /**
   * @brief getOp gets the Operation this command performs
   * @return Match/Miss/Modify/Delete, etc.
   */
  Operation getOp() const;

  /**
   * @brief setOp sets the op this command is supposed to perform
   * @param op Operation
   */
  void setOp(Operation op);

  /**
   * @brief getElementHashes gets the list of hoot:hash IDs that uniquely identify the elements
   *        this command operates on.
   * @return A set of sha1 hashes
   */
  QSet<QByteArray> getElementHashes() const;

  /**
   * @brief setElementHashes sets the list of hoot:hash IDs that this command will operate on.
   * @param hashes the set of hoot:hash (sha1 hashes) to operate on
   */
  void setElementHashes(QSet<QByteArray> hashes);

  /**
   * @brief clearElementHahses clears the internal hash set
   */
  void clearElementHahses();

  /**
   * @brief addElementHash Add the given hash to the internal list that this command operates on
   * @param hash hoot:hash (sha1)
   */
  void addElementHash(QByteArray hash);

  /**
   * @brief addElementHash Add the given hash to the internal list that this command operates on.
   *        The string will be interpreted as hex
   * @param hashString sha1 string of hex characters
   */
  void addElementHash(QString hashString);

  /**
   * @brief addElementHash Add the given element's hash to the internal list that this command
   *        operates on. If the element does not have a hoot:hash tag, it will be calculated.
   * @param e Element to add a hash for.
   */
  void addElementHash(ElementPtr e);

  /**
   * @brief containsElement Check to see if this command operates on this element (by looking
   *        for its hash)
   * @param e Map Element
   * @return true if this command operates on this element
   */
  bool containsElement(ElementPtr e) const;

  /**
   * @brief containsElementHash Checks to see if the supplied hash is in our internal set of hashes
   * @param hash The hash to check for
   * @return true if our internal set contains this hash
   */
  bool containsElementHash(QByteArray hash) const;

  /**
   * @brief getFinalElement gets the final representation of the element, after the command has
   *        been performed. Only valid for match commands and modify commands. As an example,
   *        consider when a user chooses to merge the elements in a review. This will represent
   *        the single, final, merged element.
   * @return The final element (element resulting from the command)
   */
  ElementPtr getFinalElement() const;

  /**
   * @brief setFinalElement sets the final element - basically how the thing should look after the
   *        command is executed. Only for use with match or modify commands.
   * @param e Element representing the final match / modify
   */
  void setFinalElement(ElementPtr e);

  /**
   * @brief toJsonString gets a json string representing this command. Contains a
   *        MultiaryReviewCommand object which has three children: Operation, ElementHashes,
   *        and a FinalElement.
   * @return string of json
   */
  QString toJsonString() const;

  /**
   * @brief toTSVString gets a tab-separated-value string representing this command. This should
   *        be suitable for passing to Apache Spark for distributed processing. The resulting
   *        string will look something like this:
   *        <opcode>\t<comma-separated list of hashes>\t<payload>
   *
   *        The opcode will represent the command to be performed, and you can look it up in the
   *        opToTsvAbbrev() function. The list of hashes will be one or more hashes, identifying
   *        the elements affected by the the command. The payload is to be opaque to Spark, as it
   *        won't be parsed by the Spark systems, but simply passed through to hoot. In
   *        implementation, this payload is simply the toJsonString() string... beceause that was
   *        easy.
   * @return a tsv string representing the command
   */
  QString toTSVString() const;

  /**
   * @brief fromJsonString parses a command object out of the given json string
   * @param jsonStr this should be the output of a call to toJsonString()
   * @return a populated MultiaryReviewCommand object
   */
  static MultiaryReviewCommand fromJsonString(QString jsonStr);

  /**
   * @brief operator == check equality by comparing json string representations
   * @param rhs
   * @return true if commands are equal
   */
  bool operator ==(const MultiaryReviewCommand& rhs) const;

  /**
   * @brief applyToMap applies this command to the given map. If the map does not actually contain
   *        the element(s) that the command refers to, nothing will be done, and the function will
   *        return false.
   * @param pMap
   * @return true on success, false otherwise
   */
  bool applyToMap(OsmMapPtr pMap) const;

  /**
   * @brief operationToString convert Operation enumeration value to a string
   * @param op
   * @return a string: Match, Miss, Modify, etc.
   */
  static QString operationToString(Operation op)
  {
    if (NoOp == op)
      return "NoOp";
    else if (Match == op)
      return "Match";
    else if (Miss == op)
      return "Miss";
    else if (Modify == op)
      return "Modify";
    else if (Delete == op)
      return "Delete";
    return "Error";
  }

  /**
   * @brief opToTsvAbbrev gets the appropriate opcode for passing to Spark
   * @param op Operation
   * @return A, I, O, etc.
   */
  static QString opToTsvAbbrev(Operation op)
  {
    if (NoOp == op)
      return "N";
    else if (Match == op)
      return "A";
    else if (Miss == op)
      return "I";
    else if (Modify == op)
      return "O";
    else if (Delete == op)
      return "E";
    return "Error";
  }

  /**
   * @brief operationFromString converts a string input to an Operation enumeration
   * @param str string like "Match", "Miss", "Modify", etc.
   * @return Operation enumeration
   */
  static Operation operationFromString(QString str)
  {
    if (str.contains("NoOp", Qt::CaseInsensitive))
      return NoOp;
    else if (str.contains("Match", Qt::CaseInsensitive))
      return Match;
    else if (str.contains("Miss", Qt::CaseInsensitive))
      return Miss;
    else if (str.contains("Modify", Qt::CaseInsensitive))
      return Modify;
    else if (str.contains("Delete", Qt::CaseInsensitive))
      return Delete;
    return NoOp;
  }


private:

  // The operation this command will perform
  Operation _op;

  // List of SHA1 hashes, each element should have a MetadataTags::HootHash()
  // See MultiaryPoiHashVisitor for more info
  QSet<QByteArray> _elementHashes;

  // What the final matched / modified element should look like
  ElementPtr _finalElement;

  // Get the Hood:Hash identifier for the element. Calculate it if required.
  QByteArray _getElementHashID(ElementPtr e) const;

  // Get a comma-separated list of hashes
  QString _getHashIDList() const;

  // Get a copy of the element
  ElementPtr _copyElement (ElementPtr e) const;
};

}

#endif // __MULTIARY_REVIEW_COMMAND_H__
