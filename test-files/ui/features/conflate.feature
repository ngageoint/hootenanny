Feature: Conflate feature

  Scenario: I can conflate the DC GIS data
    Given I am on Hootenanny
    #And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "DcGisRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I should see "DcGisRoadsCucumber"
    And I press "Add Secondary Dataset"
    And I click the "DcTigerRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I should see "DcTigerRoadsCucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 3 "minutes" to see "Merged_RoadsCucumber"
