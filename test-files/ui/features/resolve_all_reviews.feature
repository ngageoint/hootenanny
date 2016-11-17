Feature: Resolve all reviews feature

  Scenario: I can conflate the DC GIS data and resolve all reviews at once
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "DcGisRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I wait 15 "seconds" to see "span.strong" element with text "DcGisRoadsCucumber"
    And I press "Add Secondary Dataset"
    And I click the "DcTigerRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I wait 15 "seconds" to see "span.strong" element with text "DcTigerRoadsCucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 3 "minutes" to see "Merged_RoadsCucumber"
    Then I should see "Complete Review"
    Then I wait 30 "seconds" to see "Reviews remaining:"
    Then I should see "Hide Table"
    Then I should see a "a.previous" on the map
    Then I should see a "a.next" on the map
    Then I should see "Resolved"
    Then I should see "Complete Review"
    And I click the "Complete Review" link
    Then I should see "Resolve all remaining reviews"
    And I press "Resolve all remaining reviews"
    Then I wait 30 "seconds" to not see "Reviews remaining:"
    Then I should not see "Hide Table"
    Then I should not see "a.previous"
    Then I should not see "a.next"
    Then I should see "All Reviews Resolved!"

  Scenario: I can conflate with another dataset
    Then I click on "input.fill-dark[type='submit']"
    Then I wait 30 seconds
    And I press "Add Secondary Dataset"
    And I click the "DcOsmRoadsCucumber" Dataset
    And I press "Add Layer"
    Then I wait 15 "seconds" to see "span.strong" element with text "DcOsmRoadsCucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 3 "minutes" to see "Merged_RoadsCucumber"
    Then I should see "Complete Review"
    Then I wait 30 "seconds" to see "Reviews remaining:"
    Then I should see "Hide Table"
    Then I should see a "a.previous" on the map
    Then I should see a "a.next" on the map
    Then I should see "Resolved"
    Then I should see "Complete Review"
    And I click the "Complete Review" link
    Then I should see "Resolve all remaining reviews"
    And I press "Resolve all remaining reviews"
    Then I wait 30 "seconds" to not see "Reviews remaining:"
    Then I should not see "Hide Table"
    Then I should not see the "a.previous" on the page
    Then I should not see the "a.next" on the page
    Then I should see "All Reviews Resolved!"

  # Disable until resolved in https://github.com/ngageoint/hootenanny/issues/1147
  # Scenario: I can export the merged dataset
  #   When I click on "input.fill-darken0[type='submit']"
  #   Then I type "export_conflated" in input "fileExportOutputName"
  #   Then I click on "input.fill-dark[type='submit']"
  #   And I wait 5 "minutes" to not see "Output Name"
  #   Then the download file "export_conflated.zip" should exist
