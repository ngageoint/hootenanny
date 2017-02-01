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
			| General Conflation Options | hoot_general_conflation_options_label | element_cache_size_node | 2000000 |
			| General Conflation Options | hoot_general_conflation_options_label | element_cache_size_relation | 200000 |
			| General Conflation Options | hoot_general_conflation_options_label | element_cache_size_way | 200000 |
			| Waterway Options | hoot_waterway_options_label | waterway_way_angle_sample_distance | "20.0" |
			| Waterway Options | hoot_waterway_options_label | waterway_way_matcher_heading_delta | "150.0" |
			| Waterway Options | hoot_waterway_options_label | waterway_rubber_sheet_minimum_ties | 5 |

		Scenario: Open options that require checkbox for input
			Then I should see "Rubber Sheeting Options"
			Then I click on "#hoot_rubber_sheeting_options_label"
			Then I check the "Enabled" checkbox
			Then I check the "Rubber Sheet Ref" checkbox
			Then I should see "Waterway Options"
			Then I click on "#hoot_waterway_options_label"
			Then I uncheck the "Waterway Auto Calc Search Radius" checkbox

			Scenario Outline: Check for Invalid Inputs that require checkboxes
				And I should see element "#<element>" with no value and placeholder <placeholder>
				When I fill "#<element>" with <invalid argument>
				When I press enter in the "#<element>" input
				Then I should see an invalid input warning for "#<element>"
				When I fill "#<element>" with ""
				When I press tab in the "#<element>" input

				Examples:
				| element | placeholder | invalid argument |
				| rubber_sheet_minimum_ties | 10 | "abcdef" |
				| rubber_sheet_minimum_ties | 10 | "-10000" |
				| rubber_sheet_minimum_ties | 10 | "&*^@(!" |
				| search_radius_waterway | "-1" | "abcdef" |
				| search_radius_waterway | "-1" | "-10000" |
				| search_radius_waterway | "-1" | "&*^@(!" |

	Scenario: Test Cookie Cutter Options that are different than Reference or Average Options
		Then I press "big.loud-red" span with text "Cancel"
		When I select the "Cookie Cutter & Horizontal" option in "#containerofConfType"
		Then I click the "►" link
		And I should see "Advanced Conflation Options"

		Scenario Outline:
			Then I should see "<option name>"
			Then I click on "#<option id>"
			And I should see element "#<element>" with no value and placeholder <placeholder>
			When I fill "#<element>" with "abc"
			When I press enter in the "#<element>" input
			Then I should see an invalid input warning for "#<element>"
			When I fill "#<element>" with ""
			When I press tab in the "#<element>" input
			And I should see element "#<element>" with no value and placeholder <placeholder>
			When I fill "#<element>" with "-10000000"
			When I press enter in the "#<element>" input
			Then I <might> see an invalid input warning for "#<element>"
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
				| option name | option id | element | placeholder | might |
				| Road Options | hoot_road_options_label | search_radius_highway | "-1" | should |
				| Road Options | hoot_road_options_label | highway_matcher_heading_delta | "5.0" | should|
				| Road Options | hoot_road_options_label | highway_matcher_max_angle | 60 | should |
				| Road Options | hoot_road_options_label | way_merger_min_split_size | 5 | should |
				| Cookie Cutter & Horizontal Options | horizontal_conflation_options_label | horizontal_cookie_cutter_alpha_shape_buffer | 0 | should_not |
