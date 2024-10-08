
SHELL=/bin/bash

# If the silent flag is passed to make then make a2x quiet
ifneq (,$(findstring s,$(MAKEFLAGS)))
»···A2X_QUIET=""
else
	A2X_QUIET=-v
endif

define buildAsciidoc
	mkdir -p tmp
	git log --simplify-by-decoration --no-merges "--pretty=%h|$$|%ct|$$|%s|$$|%aN" `find $(filter-out tmp/asciidoc-config,$^) || true` | node ParseRevisions.js > $(patsubst %.asciidoc,%-docinfo.xml,$<)
	@REVNUMBER=`$$HOOT_HOME/scripts/git/GitVersion.sh` ; \
	REVDATE=`date -d @$$(git log -n 1 "--pretty=%at" ../.) "+%B %e, %Y"`; \
	a2x -a docinfo --dblatex-opts "-P latex.output.revhistory=0 -P latex.unicode.use=1 -s styles/nga.sty --param doc.publisher.show=0" -a HasLatexMath -a "revdate=v$$REVNUMBER, $$REVDATE" $(A2X_QUIET) -f pdf $<
	a2x -a docinfo --dblatex-opts "-P latex.output.revhistory=0 -P latex.unicode.use=1 -s styles/nga.sty --param doc.publisher.show=0" -a "revdate=v$$REVNUMBER, $$REVDATE" $(A2X_QUIET) -f text $<
	asciidoc -a authorinitials=DG -a revnumber=$$REVNUMBER -a "revdate=$$REVDATE" -b html $<
	#@mv $(patsubst %.asciidoc,%.pdf,$<) "$@"
endef

# Note that we're not calling the schema documentation targets here, as it breaks an archive job. 
# See the "Core Schema" section under "Documentation" in the Developer Guide for more information.

all: \
	HootenannyUserGuide.pdf \
	CoreAPI

# A generic dep for all asciidoc documents. Make sure the environment is setup properly.
tmp/asciidoc-config: $(HOME)/.asciidoc/filters/gnuplot/gnuplot-filter.conf node_modules
	mkdir -p tmp
	touch $@

# I haven't found a way to specify a filter based on a relative or absolute path so I'm making sure 
# it is in the home config directory.
# http://www.methods.co.nz/asciidoc/userguide.html#X101
$(HOME)/.asciidoc/filters/gnuplot/gnuplot-filter.conf: filters/gnuplot/gnuplot-filter.conf
	mkdir -p $(HOME)/.asciidoc/filters/gnuplot/
	rm -f $(HOME)/.asciidoc/filters/gnuplot/*
	cp filters/gnuplot/* $(HOME)/.asciidoc/filters/gnuplot/

clean: clean-schema
	rm -rf tmp *.html *-docinfo.xml restDocs developer/CoreAPI
	
clean-schema:
	rm -f schema.dot

node_modules:
	npm install --silent xml2js htmlparser imagemagick mocha@1.20.1 express@3.1.2 async html-to-text restler

tmp/rules-lib-doxygen: RulesLib.dox ../rules/lib/*
	mkdir -p tmp
	doxygen RulesLib.dox

tmp/rules-template-doxygen: RulesTemplate.dox ../rules/template/*
	mkdir -p tmp
	doxygen RulesTemplate.dox

# This target builds the the core API documentation with doxygen. Tried without success to embed the 
# Core API docs into asciidoc via XML, like the Javascript API is done, but it errors out with a 
# non-descript message during the conversion from xml to asciidoc. May be worth trying to fix at 
# some point. Going with standalone HTML that isn't added to the pdf instead for now.
CoreAPI: developer/CoreAPI/html/index.html

developer/CoreAPI/html/index.html: CoreAPI.dox
	mkdir -p developer/CoreAPI
	doxygen CoreAPI.dox

tmp/RulesLib.asciidoc: tmp/rules-lib-doxygen DoxygenToAsciiDoc.xslt
	xsltproc tmp/rules-lib-doxygen/xml/combine.xslt tmp/rules-lib-doxygen/xml/index.xml > tmp/rules-lib-doxygen/xml/combined.xml
	xsltproc DoxygenToAsciiDoc.xslt tmp/rules-lib-doxygen/xml/combined.xml > $@

tmp/RulesTemplate.asciidoc: tmp/rules-template-doxygen DoxygenToAsciiDoc.xslt
	xsltproc tmp/rules-template-doxygen/xml/combine.xslt tmp/rules-template-doxygen/xml/index.xml > tmp/rules-template-doxygen/xml/combined.xml
	xsltproc DoxygenToAsciiDoc.xslt tmp/rules-template-doxygen/xml/combined.xml > $@

HootenannyUserGuide.pdf: \
	HootenannyUserGuide.asciidoc \
	tmp/asciidoc-config \
	../HOOT_VERSION_FILE \
	docinfo.xml \
	install/* \
	styles/* \
	developer/* \
	tmp/RulesLib.asciidoc \
	tmp/RulesTemplate.asciidoc \
	tmp/rules-lib-doxygen \
	tmp/rules-template-doxygen \
	JavascriptOverview.asciidoc \
	HootJavaScriptInterfaces.asciidoc \
	HootenannyConfigOptions.asciidoc \
	commands/* \
	../conf/core/ConfigOptions.asciidoc \
	References.asciidoc \
	user/*.asciidoc
	$(call buildAsciidoc)

schema.asciidoc:
	hoot schema -D tag.printing.format=asciidoc > schema.asciidoc

schema.png:
	hoot schema schema.png
	
