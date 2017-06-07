Feature: Command detail modal window

  Scenario: I can conflate the AllDataTypes data
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "DcOsmRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "DcOsmRoadsCucumber"
    And I press "Add Secondary Dataset"
    And I click the "DcTigerRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "DcTigerRoadsCucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I fill "saveAs" input with "MergedDcRoads"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 20 seconds
    Then I see the UI alert
    And I click on ".alertDiv"
    And I click on ".detailLink"
    Then I should see a paragraph with text "commandDetail"
    And I close the modal window
    Then I should not see a paragraph with text "commandDetail"
    And I close the UI alert
    Then I should not see the UI alert
    Then I click the "trash" button
    And I accept the alert
    And I wait 5 "seconds" to not see "MergedDcRoads"