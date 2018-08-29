Feature: Conflate differential with stats

  Scenario: I can conflate data with Differential Conflation and see statistics
    Given I am on Hootenanny
    And I resize the window
    And I click Get Started
    And I press "Add Reference Dataset"
    And I click the "DiffInput1Cucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "DiffInput1Cucumber"
    And I press "Add Secondary Dataset"
    And I click the "DiffInput2Cucumber" Dataset
    And I press "Add Layer"
    Then I wait 30 "seconds" to see "span.strong" element with text "DiffInput2Cucumber"
    Then I should see "Conflate"
    And I press "Conflate"
    And I fill "saveAs" input with "DiffConflateCucumber"
    And I select the exact "Differential" option in "#containerofConfType"
    And I select the "true" option in "#containerofisCollectStats"
    And I scroll element into view and press "conflate2"
    Then I wait 30 "seconds" to see "Conflating …"
    Then I wait 4 "minutes" to see "DiffConflateCucumber"
    Then I click the "info" button
    And I wait 5 seconds
    When I press "Statistics"
    And I should see stats "diffstats" "POIs" "new" "1"
    And I should see stats "diffstats" "Buildings" "new" "1"
    And I should see stats "diffstats" "Km of Road" "new" "16.91"
    When I press "Download"
    And I wait 30 seconds
    Then the download file "DiffConflateCucumber-stats.tsv" should exist
    Then I click the "trash" button
    And I accept the alert
    And I wait 5 "seconds" to not see "DiffConflateCucumber"
