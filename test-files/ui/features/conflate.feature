Feature: Conflate feature

  Scenario: I can conflate the DC GIS data
    Given I am on Hootenanny
    And I resize the window
    And I press "Get Fubar"
    And I press first "Add Reference Dataset"
    And I select reference dataset "DcGisRoadsCucumber"
    And I press first "Add Layer"
    Then I should see "DcGisRoadsCucumber"
    And I press first "Add Secondary Dataset"
    And I select secondary dataset "DcTigerRoadsCucumber"
    And I press first "Add Layer"
    Then I should see "DcTigerRoadsCucumber"
    Then I should see "Conflate"
    And I press first "Conflate"
    And I scroll element into view and press "conflate2"
    Then I wait 30 seconds to see "Conflating …"
    Then I wait 10 minutes to see "Merged_RoadsCucumber"
