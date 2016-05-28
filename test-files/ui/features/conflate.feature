Feature: Conflate feature

  Scenario: I can conflate the DC GIS data
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "DcGisRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "DcGisRoadsCucumber"
    And I press "Add Secondary Dataset"
    And I click the "DcTigerRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "DcTigerRoadsCucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 5 "minutes" to see "Merged_RoadsCucumber"
    Then I click the "trash" button
    And I accept the alert
    And I wait 5 "seconds" to not see "Merged_RoadsCucumber"