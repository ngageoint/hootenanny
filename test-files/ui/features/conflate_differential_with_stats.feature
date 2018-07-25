Feature: Conflate differential with stats

  Scenario: I can conflate data with Differential Conflation and see statistics
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "ToyTestACucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "ToyTestACucumber"
    And I press "Add Secondary Dataset"
    And I click the "ToyTestBCucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "ToyTestBCucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I fill "saveAs" input with "TTRoadsDifferentialConflationCucumber"
    And I select the "Differential" option in "#containerofConfType"
    And I select the "true" option in "#containerofisCollectStats"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 4 "minutes" to see "TTRoadsDifferentialConflationCucumber"
    Then I click the "info" button
    And I wait 5 seconds
    When I press "Statistics"
    And I should see stats "featurepercents" "pois" "review" "0.0%"
    And I should see stats "featurecounts" "buildings" "merged" "0"
    And I should see stats "featurepercents" "roads" "unmatched" "100.0%"
    And I should see stats "featurecounts" "waterways" "review" "0.0%"
    When I press "Download"
    And I wait 30 seconds
    Then the download file "TTRoadsDifferentialConflationCucumber-stats.tsv" should exist
    Then I click the "trash" button
    And I accept the alert
    And I wait 5 "seconds" to not see "TTRoadsDifferentialConflationCucumber"
