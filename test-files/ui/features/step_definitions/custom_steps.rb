Given(/^I am on Hootenanny$/) do
  visit "http://localhost:" + (ENV['TOMCAT_PORT'] ? ENV['TOMCAT_PORT'] : "8080") + "/hootenanny-id" # may need to change URL
end

When(/^I click Get Started$/) do
  begin
    el = find_button('Get Started')
  rescue Capybara::ElementNotFound
    # In Capybara 0.4+ #find_field raises an error instead of returning nil
    el = nil
  end
  el.click unless el.nil?
end

When(/^I click the "([^"]*)" icon$/) do |cls|
  find('._icon.' + cls).click
end

When(/^I click the "([^"]*)" icon under the "([^"]*)" link$/) do |cls, txt|
  parent = find('a', :text=> txt).find('._icon.' + cls).click
end

When(/^I click the "([^"]*)" link$/) do |linkText|
  find('a', :text=> linkText).click
end

When(/^I click the "([^"]*)" link under "([^"]*)"$/) do |linkText, parent|
  find('#' + parent).find('a', :text=> linkText).click
end

When(/^I click the "([^"]*)" classed link under "([^"]*)"$/) do |classed, parent|
  find('div.' + parent).find('a.' + classed).click
end

When(/^I select a way map feature with id "([^"]*)"$/) do |id|
  oldTimeout = Capybara.default_max_wait_time
  Capybara.default_max_wait_time = 10
  find('div.layer-data').all('path[class*=" ' + id + '"]').last.click
  Capybara.default_max_wait_time = oldTimeout
end

When(/^I select a node map feature with id "([^"]*)"$/) do |id|
  oldTimeout = Capybara.default_max_wait_time
  Capybara.default_max_wait_time = 10
  find('div.layer-data').all('g[class*=" ' + id + '"]').last.click
  Capybara.default_max_wait_time = oldTimeout
end

Then (/^I should (not )?see an element "([^"]*)"$/) do |negate, selector|
  expectation = negate ? :should_not : :should
  page.send(expectation, have_css(selector))
end

Then (/^I should (not )?see the element (.*)$/) do |negate, selector|
  expectation = negate ? :should_not : :should
  page.send(expectation, have_css(selector))
end

Then(/^I should see "([^"]*)"$/) do |text|
  #page.should have_content(text)
  expect(page).to have_content(text)
end

Then(/^I should not see "([^"]*)"$/) do |text|
  #page.should have_no_content(text)
  expect(page).to have_no_content(text)
end

Then(/^I click on the "([^"]*)" label$/) do |txt|
  find('label', :text=> txt, :match => :prefer_exact).click
end

Then(/^I should see (checkbox )?"([^"]*)" (not )?enabled$/) do |cbox, text,state|
  lbl = find('label', :text=> text, :match => :prefer_exact)
  if cbox
    el = lbl.find('input')
  else
    el = lbl.find(:xpath, '..').find('input')
  end

  if state
    el[:disabled].should eq "true"
  else
    el[:disabled].should_not be
  end
end

Then(/^I should see "([^"]*)" combobox (not )?enabled$/) do |text,state|
  lbl = find('label', :text=> text, :match => :prefer_exact)
  el = lbl.find(:xpath,"..").find('.combobox-input')

  if state
    el[:disabled].should eq "true"
  else
    el[:disabled].should_not be
  end
end

Then(/^I should see checkbox "([^"]*)" (un)?checked$/) do |text, unchk|
  lbl = find('label', :text=> text, :match => :prefer_exact)
  cbox = lbl.find('input')
  if unchk
    expect(cbox).to_not be_checked
  else
    expect(cbox).to be_checked
  end
end

Then(/^I (un)?check the "([^"]*)" checkbox$/) do |unchk,text|
  lbl = find('label', :text=> text, :match => :prefer_exact)
  cbox = lbl.find('input')
  if unchk
    cbox.set(false)
  else
    cbox.set(true)
  end
end

Then(/^I should see an alert containing "([^"]*)"$/) do |text|
  alertText = page.driver.browser.switch_to.alert.text
  expect(alertText).to include text
end

Then (/^I should( not)? see a link "([^"]*)"$/) do |negate, txt|
  expectation = negate ? :should_not : :should
  if negate
    page.should_not have_selector('a', :text=> txt)
  else
    page.should have_selector('a', :text=> txt)
  end
end

When(/^I select the "([^"]*)" div$/) do |cls|
  begin
    el = find('div.' + cls)
  rescue Capybara::ElementNotFound
    el = find('#' + cls)
  end
  el.click
  sleep 1
end

Then(/^I close the modal$/) do
  find('div.modal').find('div.x').click
end

When(/^I select the first "([^"]*)" div$/) do |cls|
  all('div.' + cls).first.click
end

When(/^I select the last "([^"]*)" div$/) do |cls|
  all('div.' + cls).last.click
end

When(/^I click the "([^"]*)" Dataset$/) do |dataset|
  text = page.find('text',:text=>dataset, :match => :prefer_exact)
  parent = text.find(:xpath,"..")
  parent.find('rect').click
end

When(/^I expand the "([^"]*)" folder$/) do |folder|
  text = page.find('text',:text=>folder, :match => :prefer_exact)
  parent = text.find(:xpath,"..")
  begin
    el = parent.find('.folder')
  rescue Capybara::ElementNotFound
    # In Capybara 0.4+ #find_field raises an error instead of returning nil
    el = nil
  end
  parent.find('rect').click unless el.nil?
end

When(/^I click the "([^"]*)" Dataset and the "([^"]*)" Dataset$/) do |d1, d2|
  text1 = page.find('text',:text=>d1, :match => :prefer_exact)
  parent1 = text1.find(:xpath,"..")
  rect1 = parent1.find('rect').click

  text2 = page.find('text',:text=>d2, :match => :prefer_exact)
  parent2 = text2.find(:xpath,"..")

  page.driver.browser.action.key_down(:control).click(parent2.native).key_up(:control).perform
end

When(/^I context click the "([^"]*)" Dataset$/) do |dataset|
  text = page.find('text',:text=>dataset, :match => :prefer_exact)
  parent = text.find(:xpath,"..")
  parent.find('rect').context_click
end

When(/^I context click "([^"]*)"$/) do |txt|
  page.find('a',:text=>txt, :match => :prefer_exact).context_click
end


When(/^I click first "([^"]*)"$/) do |text|
  Capybara.ignore_hidden_elements = false
  elements = all('a', :text => text)
  if elements.empty?
    elements = all(:xpath, '//input[@value="' + text + '"]')
  end
  elements[0].click
  Capybara.ignore_hidden_elements = true
end

When(/^I click second "([^"]*)"$/) do |text|
  Capybara.ignore_hidden_elements = false
  e1 = all('a', :text => text)
  e2 = all(:xpath, '//input[@value="' + text + '"]')
  elements = combine(e1, e2)
  elements[1].click
  Capybara.ignore_hidden_elements = true
end

When(/^I click the "([^"]*)" key$/) do |arg1|
  find("body").native.send_keys(arg1)
end

When(/^I click the "([^"]*)" key in the "([^"]*)"$/) do |key, el|
  find(el).native.send_keys(key)
end

When(/^I press the escape key$/) do
  find('body').native.send_keys(:escape)
end
Then(/^I should see options in this order:$/) do |table|
  expected_order = table.raw.flatten
  actual_order = page.all('#settingsSidebar label').collect(&:text)
  actual_order.should == expected_order
end

Then(/^I should see these tags in the table:$/) do |table|
  include_hidden_fields do
    expected = table.raw
    keys = page.all('ul.tag-list input.key').map(&:value)
    values = page.all('ul.tag-list input.value').map(&:value)
    actual = keys.zip(values)
    actual.should == expected
  end
end
Then(/^I click on the "([^"]*)" option in the "([^"]*)"$/) do |label,div|
  find('#' + div).find('label', :text => label).click
end

When(/^I select the "([^"]*)" option in the "([^"]*)" combobox$/) do |opt, cb|
  combobox = page.find(:css, 'input[placeholder="' + cb + '"]')
  combobox.find(:xpath, '..').find('.combobox-caret').click
  page.find('div.combobox').find('a', :text=> opt).click
end

When(/^I select the "([^"]*)" option labelled "([^"]*)"$/) do |opt, lbl|
  combobox = page.find('label', :text=> lbl).find(:xpath,"..")
  combobox.find('.combobox-caret').click
  page.find('div.combobox').find('a', :text=> opt).click
end

When(/^I select the "([^"]*)" option in "([^"]*)"$/) do |opt, el|
  combobox = page.find(el)
  combobox.find('.combobox-caret').click
  page.find('div.combobox').find('a', :text=> opt).click
end

When(/^The value of "([^"]*)" option in "([^"]*)"$/) do |opt, el|
  combobox = page.find(el)
  combobox.find('.combobox-caret').click
  page.find('div.combobox').find('a', :text=> opt).click
end

When(/^I click the "([^"]*)" button$/) do |el|
  find('button.' + el).click
end

When(/^I click the "([^"]*)" button and accept the alert$/) do |el|
  find('button.' + el).click
  page.driver.browser.switch_to.alert.accept
end

When(/^I click on the "([^"]*)" button in the "([^"]*)"$/) do |button,div|
  elements = all('#' + div  + ' button.' + button)
  elements[0].click
end

When(/^I click the "([^"]*)" at "([^"]*)","([^"]*)"$/) do |el, x, y|
  find('#' + el).click_at(x,y)
  sleep 3
end

When(/^I double-click the "([^"]*)" at "([^"]*)","([^"]*)"$/) do |el, x, y|
  find('#' + el).double_click_at(x,y)
  sleep 3
end

When(/^I double-click the "([^"]*)"$/) do |el|
  find('#' + el).double_click
  sleep 1
end

When(/^I fill "([^"]*)" with "([^"]*)"$/) do |el, value|
  find('#' + el).set(value)
  sleep 1
end

When(/^I fill "([^"]*)" input with "([^"]*)"$/) do |el, value|
  find('input.' + el).set(value)
  sleep 1
end

When(/^I fill "([^"]*)" textarea with: (.*)$/) do |el, value|
  find('textarea.' + el).set(value)
  sleep 1
end

When(/^I add to "([^"]*)" textarea with: (.*)$/) do |el, value|
  txt = find('textarea.' + el).value + "\n" + value
  find('textarea.' + el).set(txt)
  sleep 1
end

When(/^I fill input under "([^"]*)" with "([^"]*)"$/) do |parent, value|
  all('div.' + parent).last.find('input[type="text"]').set(value)
  sleep 1
end

When(/^I append "([^"]*)" input with "([^"]*)"$/) do |el, value|
  find('input.' + el).native.send_keys(value)
end

When(/^I press enter in the "([^"]*)" input$/) do |selector|
  find('input' + selector).native.send_keys(:enter)
end

When(/^I press tab in the "([^"]*)" input$/) do |selector|
  find('input' + selector).native.send_keys(:tab)
end

When(/^I press escape in the "([^"]*)" input$/) do |selector|
  find('input' + selector).native.send_keys(:escape)
end

When(/^I press the left arrow key$/) do
  find('body').native.send_keys(:arrow_left)
end

When(/^I press the right arrow key$/) do
  find('body').native.send_keys(:arrow_right)
end

When(/^I press the up arrow key on "([^"]*)"$/) do |target|
  find(target).native.send_keys(:arrow_up)
end

When(/^I press the down arrow key on "([^"]*)"$/) do |target|
  find(target).native.send_keys(:arrow_down)
end

When(/^I press "([^"]*)"$/) do |button|
  click_link_or_button(button)
end

When(/^I hover over "([^"]*)"$/) do |el|
  find(el).hover
end

When(/^I click on "([^"]*)"$/) do |el|
  find(el).click
end

When(/^I press "([^"]*)" span with text "([^"]*)"$/) do |cls,txt|
  find('span.' + cls, :text=>txt).click
end

When(/^I press span with text "([^"]*)"$/) do |txt|
  find('span', :text=>txt).click
end

When(/^I press "([^"]*)" big loud link$/) do |cls|
  find('a.big.loud.' + cls).click
end


When(/^I wait ([0-9]+) seconds$/) do |seconds|
  sleep Float(seconds)
end

When(/^I wait$/) do
  sleep 5
end

When(/^I remove the first layer$/) do
  trash_cans = all('button._icon.trash')
  trash_cans[0].click
  sleep 5
  page.driver.browser.switch_to.alert.accept
  sleep 2
end

When(/^I wait ([0-9]*) "([^"]*)" to see "([^"]*)"$/) do |timeout, unit, text|
  if unit == "seconds"
    multiplier = 1
  elsif unit == "minutes"
    multiplier = 60
  else
    throw :badunits
  end
  oldTimeout = Capybara.default_max_wait_time
  Capybara.default_max_wait_time = Float(timeout) * multiplier
  page.should have_content(text)
  Capybara.default_max_wait_time = oldTimeout
end

When(/^I wait ([0-9]*) "([^"]*)" to not see "([^"]*)"$/) do |timeout, unit, text|
  if unit == "seconds"
    multiplier = 1
  elsif unit == "minutes"
    multiplier = 60
  else
    throw :badunits
  end
  oldTimeout = Capybara.default_max_wait_time
  Capybara.default_max_wait_time = Float(timeout) * multiplier
  page.should have_no_content(text)
  Capybara.default_max_wait_time = oldTimeout
end

When(/^I wait ([0-9]*) "([^"]*)" to see "([^"]*)" element with text "([^"]*)"$/) do |timeout, unit, el, text|
  if unit == "seconds"
    multiplier = 1
  elsif unit == "minutes"
    multiplier = 60
  else
    throw :badunits
  end
  oldTimeout = Capybara.default_max_wait_time
  Capybara.default_max_wait_time = Float(timeout) * multiplier
  page.find(el, :text => text)
  Capybara.default_max_wait_time = oldTimeout
end

When(/^I close the UI alert$/) do
  find('#alerts').all('.x')[0].click
end

When(/^I change the reference layer color to ([^"]*)$/) do |color|
  page.first('div.big.data').click
  swatch = find('a[data-color="' + color + '"')
  rgb = swatch.native.css_value('background').split(")").first + ')'
  swatch.click
  expect(page.first('path.stroke.tag-hoot').native.css_value('stroke')).to eq(rgb)
end

When(/^I scroll element into view and press "([^"]*)"$/) do |id|
  Capybara.ignore_hidden_elements = false
  element = page.driver.browser.find_element(:id, id)
  page.driver.browser.execute_script("arguments[0].scrollIntoView(true)", element)
  element.click
  Capybara.ignore_hidden_elements = true
end

Then(/^I resize the window$/) do
  if Capybara.javascript_driver != :webkit
    window = Capybara.current_session.driver.browser.manage.window
    window.resize_to(1920, 1080) # width, height
  end
end

When(/^I upload a shapefile/) do
  include_hidden_fields do
    # Seems to be a bug where only the last file in the array gets attached.
    # Then can't be attached separately because then each file gets uploaded
    # individually.
    page.attach_file('taFiles', [ENV['HOOT_HOME'] + '/test-files/translation_assistant/cali-test.shp', ENV['HOOT_HOME'] + '/test-files/translation_assistant/cali-test.dbf', ENV['HOOT_HOME'] + '/test-files/translation_assistant/cali-test.shx', ENV['HOOT_HOME'] + '/test-files/translation_assistant/cali-test.prj'])
    # page.execute_script("var event = document.createEvent('Event'); event.initEvent('change', true, true); d3.select('input[name=taFiles]').node().dispatchEvent(event);")
    puts page.driver.browser.manage.logs.get("browser")
  end
end

When(/^I upload a zipped shapefile/) do
  include_hidden_fields do
    page.attach_file('taFiles', ENV['HOOT_HOME'] + '/test-files/translation_assistant/cali-test.zip')
  end
end

When(/^I upload a zipped folder of shapefiles/) do
  include_hidden_fields do
    page.attach_file('taFiles', ENV['HOOT_HOME'] + '/test-files/translation_assistant/calizip.zip')
  end
end

When(/^I upload an invalid dataset/) do
  include_hidden_fields do
    page.attach_file('taFiles', ENV['HOOT_HOME'] + '/test-files/translation_assistant/cali-test.shx')
  end
end

When(/^I select "([^"]*)" dataset/) do |file|
  include_hidden_fields do
    page.attach_file('ingestfileuploader', ENV['HOOT_HOME'] + file)
  end
end

Then(/^I take a screenshot/) do
  screenshot_and_save_page
end

Then(/^I type "([^"]*)" in input "([^"]*)"$/) do |text, id|
  page.fill_in id, :with => text
end

Then(/^I click the "([^"]*)" with text "([^"]*)"$/) do |el, text|
  page.find(el, :text => text).click
end

Then(/^I hover over the "([^"]*)" with text "([^"]*)"$/) do |el, text|
  page.find(el, :text => text).hover
end

Then(/^I should see a measurement line$/) do
  page.should have_css('line.measure-line-0')
  page.should have_css('text.measure-label-text')
end

Then(/^I should see a measurement area$/) do
  page.should have_css('polygon.measure-area')
  page.should have_css('text.measure-label-text')
end

Then(/^I accept the alert$/) do
  sleep 2
  page.driver.browser.switch_to.alert.accept
end

Then(/^I should see element "([^"]*)" with value "([^"]*)"$/) do |id, value|
  # expect(page).to have_selector("input[value='" + value + "']")
  # page.should have_xpath("//input[@value='" + value + "']")
  find(id).value.should eq value
end

Then(/^I should see element "([^"]*)" with no value and placeholder "([^"]*)"$/) do |id, value|
  el = find(id)
  el.value.should eq ""
  el['placeholder'].should eq value
end

Then(/^I choose "([^"]*)" radio button$/) do |text|
  choose(text)
end

Then(/^I POST coverage info$/) do
  page.execute_script("(function () { if (window.__coverage__) { d3.xhr('http://localhost:8880/coverage/client').header('Content-Type', 'application/json').post(JSON.stringify(window.__coverage__),function(err, data){});}}())");
end

Then(/^I click the "([^"]*)" context menu item$/) do |text|
  find('div.context-menu').find('li', :text => text).click
end

Then(/^the download file should exists$/) do
  expect( DownloadHelpers::download_exists ).to be true
end

Then(/^the download file "([^"]*)" should exist$/) do |file|
  name = ENV['HOME'] + '/Downloads/' + file
  # puts name
  expect( File.exists?(name) ).to be true
  File.delete(name)
end

Then(/^the download file pattern "([^"]*)" should exist$/) do |file|
  name = ENV['HOME'] + '/Downloads/' + file
  # puts name
  expect( Dir.glob(name).empty? ).to be false
  File.delete(*Dir.glob(name))
end

Then(/^the log file "([^"]*)" should exist$/) do |file|
  name = ENV['HOME'] + '/Downloads/' + file
  # puts name
  expect(!Dir.glob(name).empty?).to be true
  File.delete(Dir[ENV['HOME'] + '/Downloads/' + file].last)
end

Then "the downloaded file content should be:" do |content|
  page.response_headers["Content-Disposition"].should == "attachment"
  page.source.should == content
end

When(/^I click the "([^"]*)" classed element under "([^"]*)" with text "([^"]*)"$/) do |classed, el, text|
  find(el, :text => text).find('.' + classed).click
end

When(/^I select "([^"]*)" basemap/) do |file|
  include_hidden_fields do
    page.attach_file('basemapfileuploader', ENV['HOOT_HOME'] + file)
  end
end

When(/^I click the map background button$/) do
  find('div.background-control').find('button').click
end

When(/^I click the "([^"]*)" map layer$/) do |text|
  find('label', :text => text).click
end

When(/^I should see stats "([^"]*)" "([^"]*)" "([^"]*)" "([^"]*)"$/) do |type, row, column, value|
  # And I should see stats "count" "buildings" "merged" "4"
  find('table.' + type).find('td.key', :text => row).find(:xpath,"..").all('td', :text => value).first()
end

Then(/^I click the first "([^"]*)" list item$/) do |cls|
  all('li.' + cls).first.click
end

When(/^I select in row ([0-9]*) the "([^"]*)" dataset$/) do |rowNum, file|
  include_hidden_fields do
    page.attach_file('ingestfileuploader-' + rowNum, ENV['HOOT_HOME'] + file)
  end
end

When(/^I select in row ([0-9]*) the "([^"]*)" option in the "([^"]*)" combobox$/) do |rowNum, opt, cb|
  combobox = find(:xpath, "//input[@row='" + rowNum + "'][@placeholder='" + cb + "']")
  combobox.find(:xpath, '..').find('.combobox-caret').click
  page.find('div.combobox').find('a', :text=> opt).click
end

When(/^I should see row ([0-9]*) input "([^"]*)" with value "([^"]*)"$/) do |rowNum, placeholder, value|
  el = find(:xpath , "//input[@placeholder='" + placeholder + "'][@row='" + rowNum + "']")
  el.value.should eq value
end

When(/^I fill row ([0-9]*) input "([^"]*)" with value "([^"]*)"$/) do |rowNum, placeholder, value|
  el = find(:xpath , "//input[@placeholder='" + placeholder + "'][@row='" + rowNum + "']")
  el.set(value)
end

When(/^I delete any existing "([^"]*)" basemap if necessary$/) do |text|
  begin
    find('span.fill-white.small', :text => text).find('.trash').click
    sleep 2
    page.driver.browser.switch_to.alert.accept
    oldTimeout = Capybara.default_max_wait_time
    Capybara.default_max_wait_time = 30
    page.should have_no_content(text)
    Capybara.default_max_wait_time = oldTimeout
  rescue Capybara::ElementNotFound
  end
end

When(/^I delete any existing "([^"]*)" folder if necessary$/) do |text|
  begin
        step "I context click the \"#{text}\" Dataset"
        step "I click the \"Delete\" context menu item"
        step "I accept the alert"
  rescue Capybara::ElementNotFound
  end
end

Then(/^I open the wfs export url$/) do
  url = find('input.wfsfileExportOutputName').value
  visit url
  # need a way to check the WFS GetCapabilities response is valid
end

Then(/^I should see "([^"]*)" bookmark first and "([^"]*)" bookmark second$/) do |rb1, rb2|
  spans = find('#reviewBookmarksContent').all('span.strong')
  expect(spans.first).to have_content(rb1)
  expect(spans.last).to have_content(rb2)
end

Then(/^I should see "([^"]*)" with a value between "([^"]*)" and "([^"]*)"$/) do |input, low, high|
  el = page.find(input)
  val = el.value.to_f
  expect(val).to be > low.to_f
  expect(val).to be < high.to_f
end

Then(/^I should see "([^"]*)" with a value greater than "([^"]*)"$/) do |el1, el2|
  max = page.find(el1).value.to_f
  min = page.find(el2).value.to_f
  expect(max).to be > min
end

Then(/^I should see a "([^"]*)" on the map$/) do |el|
  page.should have_css(el)
end
