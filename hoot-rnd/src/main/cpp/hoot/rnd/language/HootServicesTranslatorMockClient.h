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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HOOT_SERVICES_TRANSLATOR_MOCK_CLIENT_H
#define HOOT_SERVICES_TRANSLATOR_MOCK_CLIENT_H

// hoot
#include <hoot/rnd/language/HootServicesTranslatorClient.h>

// Qt
#include <QMap>

namespace hoot
{

/**
 * This is used during testing to mock the behavior of the translator that makes calls to hoot
 * services.
 */
class HootServicesTranslatorMockClient : public HootServicesTranslatorClient
{

public:

  static QString className() { return "hoot::HootServicesTranslatorMockClient"; }

  HootServicesTranslatorMockClient();
  virtual ~HootServicesTranslatorMockClient() = default;

  virtual QString translate(const QString& text) override;
  virtual void setSourceLanguages(const QStringList& langCodes) override;

private:

  QMap<QString, QString> _mockTranslations;
};

}

#endif // HOOT_SERVICES_TRANSLATOR_MOCK_CLIENT_H
