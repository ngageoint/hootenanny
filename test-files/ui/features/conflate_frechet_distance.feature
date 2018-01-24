 Feature: Conflation Using Frechet Distance

  Scenario: I can conflate using Frechet Distance
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
    And I fill "saveAs" input with "DcRoadsFrechetCucumber"
    And I press "confAdvOptsLnk"
    And I click the "label" with text "General Conflation Options"
    And I check the "Frechet Distance" checkbox
    And I scroll "Apply" element into view and press it
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 4 "minutes" to see "DcRoadsFrechetCucumber"
    Then I click the "trash" button
    And I accept the alert
    And I wait 5 "seconds" to not see "DcRoadsFrechetCucumber"