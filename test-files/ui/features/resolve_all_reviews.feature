Feature: Resolve all reviews feature

  Scenario: I can conflate the DC GIS data and resolve all reviews at once
    Given I am on Hootenanny
    And I resize the window
    And I press "Get Started"
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
    Then I should see "Complete Review"
    Then I wait 30 seconds to see "Reviews remaining:"
    Then I should see "Hide Table"
    Then I should see "Previous"
    Then I should see "Next"
    Then I should see "Resolved"
    And I press first "Complete Review"
    Then I should see "Resolve all remaining reviews"
    And I press first "Resolve all remaining reviews"
    Then I wait 30 seconds to not see "Reviews remaining:"
    Then I should not see "Hide Table"
    Then I should not see "Previous"
    Then I should not see "Next"
# Disabling till we figure out how to capture popup
   # Then I wait 30 seconds to see "All Reviews Resolved!"
