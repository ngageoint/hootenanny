Feature: Conflate and Export OSM API Database Dataset

  Scenario: Conflate With an OSM API Database Dataset as the Secondary Layer
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "DcTigerRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I wait 15 "seconds" to see "span.strong" element with text "DcTigerRoadsCucumber"
    And I press "Add Secondary Dataset"
    And I should not see "OSM_API_DB_osmapi_test"

  Scenario: Conflate an OSM API Database Dataset with a Hootenanny Dataset and Export the Conflated Result Back Into the OSM API Database
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "OSM_API_DB_osmapi_test" Dataset
    And I press "Add Layer"
    Then I wait 15 "seconds" to see "span.strong" element with text "OSM_API_DB_osmapi_test"
    And I press "Add Secondary Dataset"
    And I click the "AllDataTypesBCucumber" Dataset
    And I press "Add Layer"
    Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"
    Then I wait 30 "seconds" to see "Conflate"
    And I press "Conflate"
    And I scroll element into view and press "conflate2"
    Then I wait 60 "seconds" to see "Conflating …"
    Then I wait 3 "minutes" to see "osmapi_test_AllDataTypesBCucumber"
    Then I should see "Complete Review"
    Then I wait 30 "seconds" to see "Reviews remaining:"
    Then I should see "Complete Review"
    And I click the "Complete Review" link
    Then I should see "Resolve all remaining reviews"
    And I press "Resolve all remaining reviews"
    Then I wait 30 "seconds" to not see "Reviews remaining:"
    Then I should see "All Reviews Resolved!"
    Then I select the "sprocket" div
    When I click the "osmapi_test_AllDataTypesBCucumber" Dataset
    And I context click the "osmapi_test_AllDataTypesBCucumber" Dataset
    And I click the "Export" context menu item
    Then I should see element "[id='fileExportOutputName']" with value "osmapi_test_AllDataTypesBCucumber"
    When I select the "OSM API Database" option in the "File Geodatabase" combobox
    And I press "big.loud" span with text "Export"
    And I wait 30 seconds
    Then I should see an alert containing "Successful export to an OSM API database"
    Then I accept the alert

  Scenario: Export Dataset into an OSM API Database With Conflicts
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "OSM_API_DB_osmapi_test" Dataset
    And I press "Add Layer"
    Then I wait 15 "seconds" to see "span.strong" element with text "OSM_API_DB_osmapi_test"
    And I press "Add Secondary Dataset"
    And I click the "DcTigerRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I wait 15 "seconds" to see "span.strong" element with text "DcTigerRoadsCucumber"
    Then I wait 30 "seconds" to see "Conflate"
    And I press "Conflate"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 3 "minutes" to see "osmapi_test_DcTigerRoadsCucumber"
    Then I should see "Complete Review"
    Then I wait 30 "seconds" to see "Reviews remaining:"
    Then I should see "Complete Review"
    And I click the "Complete Review" link
    Then I should see "Resolve all remaining reviews"
    And I press "Resolve all remaining reviews"
    Then I wait 30 "seconds" to not see "Reviews remaining:"
    Then I should see "All Reviews Resolved!"
    Then I select the "sprocket" div
    When I click the "osmapi_test_DcTigerRoadsCucumber" Dataset
    And I context click the "osmapi_test_DcTigerRoadsCucumber" Dataset
    And I click the "Export" context menu item
    Then I should see element "[id='fileExportOutputName']" with value "osmapi_test_DcTigerRoadsCucumber"
    When I select the "OSM API Database" option in the "File Geodatabase" combobox
    And I press "big.loud" span with text "Export"
    And I wait 30 seconds
    # A conflicting changeset record has already been written to the OSM API database.
    Then I should see an alert containing "The changeset will not be written because conflicts exist in the target OSM API database"
    Then I accept the alert
    Then I close the UI alert





