Given(/^I am on the Hootenanny site$/) do
  visit "http://localhost:8080/hootenanny-id" # may need to change URL
end

When(/^I click Get Started$/) do
  begin
    el = find('.col6.start')
  rescue Capybara::ElementNotFound
    # In Capybara 0.4+ #find_field raises an error instead of returning nil
    el = nil
  end
  el.click unless el.nil?
end

When(/^I click the "([^"]*)" link$/) do |linkText|
  find('a', :text=> linkText).click
end

Then(/^I should see "([^"]*)"$/) do |text|
  page.should have_content(text)
end


When(/^I select the "([^"]*)" div$/) do |cls|
  find('div.' + cls).click
  sleep 1
end

When(/^I click the "([^"]*)" Dataset$/) do |dataset|
  text = page.find('text',:text=>dataset, :match => :prefer_exact)
  parent = text.find(:xpath,"..")
  parent.find('rect').click
end

When (/^I click first "([^"]*)"$/) do |text|
  Capybara.ignore_hidden_elements = false
  elements = all('a', :text => text)
  if elements.empty?
    elements = all(:xpath, '//input[@value="' + text + '"]')
  end
  elements[0].click
  Capybara.ignore_hidden_elements = true
end

When (/^I click second "([^"]*)"$/) do |text|
  Capybara.ignore_hidden_elements = false
  e1 = all('a', :text => text)
  e2 = all(:xpath, '//input[@value="' + text + '"]')
  elements = combine(e1, e2)
  elements[1].click
  Capybara.ignore_hidden_elements = true
end

When(/^I click the "([^"]*)" key$/) do |arg1|
  find("body").native.send_key(arg1)
end

Then(/^I should see options in this order:$/) do |table|
  expected_order = table.raw.flatten
  actual_order = page.all('#settingsSidebar label').collect(&:text)
  actual_order.should == expected_order
end

Then(/^I click on the "([^"]*)" option in the "([^"]*)"$/) do |label,div|
  find('#' + div).find('label', :text => label).click
end

When(/^I select the "([^"]*)" option in the "([^"]*)" combobox$/) do |opt, cb|
  combobox = page.find(:css, 'input[placeholder="' + cb + '"]')
  combobox.find(:xpath, '..').find('.combobox-caret').click
  find('a', :text=> opt).click
end

When(/^I click the "([^"]*)" button$/) do |el|
  find('button.' + el).click
end

When(/^I click the "([^"]*)" button and accept the alert$/) do |el|
  find('button.' + el).click
  page.driver.browser.switch_to.alert.accept
end

When(/^I click the "([^"]*)" at "([^"]*)","([^"]*)"$/) do |el, x, y|
  find('#' + el).click_at(x,y)
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

When (/^I press "([^"]*)"$/) do |button|
  click_link_or_button(button)
end

When (/^I hover over "([^"]*)"$/) do |el|
  find(el).hover
end

When (/^I press "([^"]*)" big loud span$/) do |txt|
  find('span.big.loud', :text=>txt).click
end

When (/^I wait$/) do
  sleep 5
end

When (/^I remove the first layer$/) do
  trash_cans = all('button._icon.trash')
  trash_cans[0].click
  sleep 5
  page.driver.browser.switch_to.alert.accept
  sleep 2
end

When (/^I wait ([0-9]*) "([^"]*)" to see "([^"]*)"$/) do |timeout, unit, text|
  if unit == "seconds"
    multiplier = 1
  elsif unit == "minutes"
    multiplier = 60
  else
    throw :badunits
  end
  oldTimeout = Capybara.default_wait_time
  Capybara.default_wait_time = Float(timeout) * multiplier
  page.should have_content(text)
  Capybara.default_wait_time = oldTimeout
end

When (/^I close the UI alert$/) do
  find('#alerts').all('.x')[0].click
end