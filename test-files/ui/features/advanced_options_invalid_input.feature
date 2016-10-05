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

	Scenario: I set up the Advanced Conflation Options menu
	    When I select the "Reference" option in "#containerofConfType"
		Then I click the "►" link
		And I should see "Advanced Conflation Options"

	Scenario Outline: Check for Invalid Inputs in Reference Advanced Options
		Then I should see "<option name>"
		Then I click on "#<option id>"
		And I should see element "#<element>" with no value and placeholder <placeholder>
		When I fill "#<element>" with "abc"
		When I press enter in the "#<element>" input
		Then I should see an invalid input warning for "#<element>"
		When I fill "#<element>" with ""
		When I press tab in the "#<element>" input
		And I should see element "#<element>" with no value and placeholder <placeholder>
		When I fill "#<element>" with "-100"
		When I press enter in the "#<element>" input
		Then I should see an invalid input warning for "#<element>"
		When I fill "#<element>" with ""
		When I press tab in the "#<element>" input
		And I should see element "#<element>" with no value and placeholder <placeholder>
		When I fill "#<element>" with "!@("
		When I press enter in the "#<element>" input
		Then I should see an invalid input warning for "#<element>"
		When I fill "#<element>" with ""
		When I press tab in the "#<element>" input
		And I should see element "#<element>" with no value and placeholder <placeholder>
		Then I click on "#<option id>"

		Examples:
			| option name | option id | element | placeholder | 
			| Cleaning Options | hoot_cleaning_options_label | small_way_merger_threshold | 15 | 
			| General Conflation Options | hoot_general_conflation_options_label | unify_optimizer_time_limit | 30 |
			| General Conflation Options | hoot_general_conflation_options_label | element_cache_size_relation | 200000 |

		# And I should see element "#rubber_sheet_minimum_ties" with no value and placeholder "10"
		# When I fill "rubber_sheet_minimum_ties" with "abc"
		# When I press enter in the "#rubber_sheet_minimum_ties" input
		# Then I should see an invalid input warning for "#rubber_sheet_minimum_ties"
		# When I fill "rubber_sheet_minimum_ties" with ""
		# When I press tab in the "#rubber_sheet_minimum_ties" input
		# And I should see element "#rubber_sheet_minimum_ties" with no value and placeholder "10"
		# When I fill "rubber_sheet_minimum_ties" with "-789"
		# When I press enter in the "#rubber_sheet_minimum_ties" input
		# Then I should see an invalid input warning for "#rubber_sheet_minimum_ties"
		# When I fill "rubber_sheet_minimum_ties" with ""
		# When I press tab in the "#rubber_sheet_minimum_ties" input
		# And I should see element "#rubber_sheet_minimum_ties" with no value and placeholder "10"
		# When I fill "rubber_sheet_minimum_ties" with "#%$@%"
		# When I press enter in the "#rubber_sheet_minimum_ties" input
		# Then I should see an invalid input warning for "#rubber_sheet_minimum_ties"
		# When I fill "rubber_sheet_minimum_ties" with ""
		# When I press tab in the "#rubber_sheet_minimum_ties" input
		# Then I click on "#hoot_rubber_sheeting_options_label"
		# And I should see "Waterway Options"
		# Then I click on "#hoot_waterway_options_label"
		# And I should see element "#waterway_way_matcher_heading_delta" with no value and placeholder "150.0"
		# When I fill "waterway_way_matcher_heading_delta" with "abc"
		# When I press enter in the "waterway_way_matcher_heading_delta" input
		# Then I should see an invalid input warning for "#waterway_way_matcher_heading_delta"
		# When I fill "waterway_way_matcher_heading_delta" with ""
		# When I press tab in the "waterway_way_matcher_heading_delta" input
		# And I should see element "#waterway_way_matcher_heading_delta" with no value and placeholder "150.0"
		# When I fill "waterway_way_matcher_heading_delta" with "-483290432"
		# When I press enter in the "waterway_way_matcher_heading_delta" input
		# Then I should see an invalid input warning for "#waterway_way_matcher_heading_delta"
		# When I fill "waterway_way_matcher_heading_delta" with ""
		# When I press tab in the "waterway_way_matcher_heading_delta" input
		# And I should see element "#waterway_way_matcher_heading_delta" with no value and placeholder "150.0"
		# When I fill "waterway_way_matcher_heading_delta" with "^&#$*^"
		# When I press enter in the "waterway_way_matcher_heading_delta" input
		# Then I should see an invalid input warning for "#waterway_way_matcher_heading_delta"
		# When I fill "waterway_way_matcher_heading_delta" with ""
		# When I press tab in the "waterway_way_matcher_heading_delta" input
		# And I should see element "#waterway_way_matcher_heading_delta" with no value and placeholder "150.0"

