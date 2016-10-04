Feature: Advanced Conflation Options

	Scenario: I can add data
		Given I am on Hootenanny
	    And I resize the window
	    And I click Get Started
	    And I press "Add Reference Dataset"
	    And I click the "AllDataTypesACucumber" Dataset
	    And I press "Add Layer"
	    Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesACucumber"
	    And I press "Add Secondary Dataset"
	    And I click the "AllDataTypesBCucumber" Dataset
	    And I press "Add Layer"
	    Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"

	Scenario: I can set up standard conflation parameters
	    Then I should see "Conflate"
	    And I press "Conflate"
	    And I fill "saveAs" input with "Merged_AllDataTypes_Advanced"

	    Scenario: Check for Invalid Inputs in Reference Advanced Options
	    When I select the "Reference" option in "#containerofConfType"
		Then I click the "►" link
		And I should see "Advanced Conflation Options"
		Then I should see "Cleaning Options"
		Then I click on "#hoot_cleaning_options_label"
		And I should see element "#small_way_merger_threshold" with no value and placeholder "15"
		When I fill "small_way_merger_threshold" with "abc"
		When I press enter in the "#small_way_merger_threshold" input
		Then I should see an invalid input warning for "#small_way_merger_threshold"
		When I fill "small_way_merger_threshold" with ""
		When I press tab in the "#small_way_merger_threshold" input
		And I should see element "#small_way_merger_threshold" with no value and placeholder "15"
		When I fill "small_way_merger_threshold" with "-100"
		When I press enter in the "#small_way_merger_threshold" input
		Then I should see an invalid input warning for "#small_way_merger_threshold"
		When I fill "small_way_merger_threshold" with ""
		When I press tab in the "#small_way_merger_threshold" input
		And I should see element "#small_way_merger_threshold" with no value and placeholder "15"
		When I fill "small_way_merger_threshold" with "!@("
		When I press enter in the "#small_way_merger_threshold" input
		Then I should see an invalid input warning for "#small_way_merger_threshold"
		When I fill "small_way_merger_threshold" with ""
		When I press tab in the "#small_way_merger_threshold" input
		And I should see element "#small_way_merger_threshold" with no value and placeholder "15"
		Then I click on "#hoot_cleaning_options_label"
		And I should see "Rubber Sheeting Options"
		Then I click on "#hoot_rubber_sheeting_options_label"
		And I check the "Enabled" checkbox
		And I check the "Rubber Sheet Ref" checkbox
		And I should see element "#rubber_sheet_minimum_ties" with no value and placeholder "10"
		When I fill "rubber_sheet_minimum_ties" with "abc"
		When I press enter in the "#rubber_sheet_minimum_ties" input
		Then I should see an invalid input warning for "#rubber_sheet_minimum_ties"
		When I fill "rubber_sheet_minimum_ties" with ""
		When I press tab in the "#rubber_sheet_minimum_ties" input
		And I should see element "#rubber_sheet_minimum_ties" with no value and placeholder "10"
		When I fill "rubber_sheet_minimum_ties" with "-789"
		When I press enter in the "#rubber_sheet_minimum_ties" input
		Then I should see an invalid input warning for "#rubber_sheet_minimum_ties"
		When I fill "rubber_sheet_minimum_ties" with ""
		When I press tab in the "#rubber_sheet_minimum_ties" input
		And I should see element "#rubber_sheet_minimum_ties" with no value and placeholder "10"
		When I fill "rubber_sheet_minimum_ties" with "#%$@%"
		When I press enter in the "#rubber_sheet_minimum_ties" input
		Then I should see an invalid input warning for "#rubber_sheet_minimum_ties"
		When I fill "rubber_sheet_minimum_ties" with ""
		When I press tab in the "#rubber_sheet_minimum_ties" input
		Then I click on "#hoot_rubber_sheeting_options_label"
		And I should see "Waterway Options"
		Then I click on "#hoot_waterway_options_label"
		And I should see element "#waterway_way_matcher_heading_delta" with no value and placeholder "150.0"
		When I fill "waterway_way_matcher_heading_delta" with "abc"
		When I press enter in the "waterway_way_matcher_heading_delta" input
		Then I should see an invalid input warning for "#waterway_way_matcher_heading_delta"
		When I fill "waterway_way_matcher_heading_delta" with ""
		When I press tab in the "waterway_way_matcher_heading_delta" input
		And I should see element "#waterway_way_matcher_heading_delta" with no value and placeholder "150.0"
		When I fill "waterway_way_matcher_heading_delta" with "-483290432"
		When I press enter in the "waterway_way_matcher_heading_delta" input
		Then I should see an invalid input warning for "#waterway_way_matcher_heading_delta"
		When I fill "waterway_way_matcher_heading_delta" with ""
		When I press tab in the "waterway_way_matcher_heading_delta" input
		And I should see element "#waterway_way_matcher_heading_delta" with no value and placeholder "150.0"
		When I fill "waterway_way_matcher_heading_delta" with "^&#$*^"
		When I press enter in the "waterway_way_matcher_heading_delta" input
		Then I should see an invalid input warning for "#waterway_way_matcher_heading_delta"
		When I fill "waterway_way_matcher_heading_delta" with ""
		When I press tab in the "waterway_way_matcher_heading_delta" input
		And I should see element "#waterway_way_matcher_heading_delta" with no value and placeholder "150.0"
