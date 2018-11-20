
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
	#a2x --dblatex-opts "-P latex.output.revhistory=0 -s styles/nga.sty --param doc.publisher.show=0" -a HasLatexMath -a authorinitials=DG -a revnumber=$$REVNUMBER -a "revdate=v$$REVNUMBER, $$REVDATE" -a "revremark=foo" $(A2X_QUIET) -f pdf $<
	asciidoc -a authorinitials=DG -a revnumber=$$REVNUMBER -a "revdate=$$REVDATE" -b html $<
	#a2x --dblatex-opts "-s styles/dg.sty --param doc.publisher.show=0" -a HasLatexMath -a authorinitials=DG -a revnumber=$$REVNUMBER -a revdate=$$REVDATE $(A2X_QUIET) -f pdf $<
	@mv $(patsubst %.asciidoc,%.pdf,$<) "$@"
endef

all: \
	Hootenanny\ -\ User\ Guide.pdf \
	Hootenanny\ -\ User\ Interface\ Guide.pdf \
	Hootenanny\ -\ Installation\ Instructions.pdf \
	Hootenanny\ -\ Developer\ Guide.pdf \
  Hootenanny\ -\ Algorithms.pdf \

# A generic dep for all asciidoc documents. Make sure the environment is setup
# properly.
tmp/asciidoc-config: $(HOME)/.asciidoc/filters/gnuplot/gnuplot-filter.conf node_modules
	mkdir -p tmp
	touch $@

# I haven't found a way to specify a filter based on a relative or absolute path
# so I'm making sure it is in the home config directory.
# http://www.methods.co.nz/asciidoc/userguide.html#X101
$(HOME)/.asciidoc/filters/gnuplot/gnuplot-filter.conf: filters/gnuplot/gnuplot-filter.conf
	mkdir -p $(HOME)/.asciidoc/filters/gnuplot/
	rm -f $(HOME)/.asciidoc/filters/gnuplot/*
	cp filters/gnuplot/* $(HOME)/.asciidoc/filters/gnuplot/

clean:
	rm -rf tmp *.pdf *.png *.html *-docinfo.xml restDocs

node_modules:
	npm install --silent xml2js htmlparser imagemagick mocha@1.20.1 express@3.1.2 async html-to-text restler

tmp/rules-lib-doxygen: RulesLib.dox ../rules/lib/*
	mkdir -p tmp
	doxygen RulesLib.dox

tmp/rules-template-doxygen: RulesTemplate.dox ../rules/template/*
	mkdir -p tmp
	doxygen RulesTemplate.dox

tmp/RulesLib.asciidoc: tmp/rules-lib-doxygen DoxygenToAsciiDoc.xslt
	xsltproc tmp/rules-lib-doxygen/xml/combine.xslt tmp/rules-lib-doxygen/xml/index.xml > tmp/rules-lib-doxygen/xml/combined.xml
	xsltproc DoxygenToAsciiDoc.xslt tmp/rules-lib-doxygen/xml/combined.xml > $@

tmp/RulesTemplate.asciidoc: tmp/rules-template-doxygen DoxygenToAsciiDoc.xslt
	xsltproc tmp/rules-template-doxygen/xml/combine.xslt tmp/rules-template-doxygen/xml/index.xml > tmp/rules-template-doxygen/xml/combined.xml
	xsltproc DoxygenToAsciiDoc.xslt tmp/rules-template-doxygen/xml/combined.xml > $@

Hootenanny\ -\ Algorithms.pdf: \
	HootenannyAlgorithms.asciidoc \
	tmp/asciidoc-config \
	../HOOT_VERSION_FILE \
	docinfo.xml \
	styles/* \
	References.asciidoc \
	algorithms/*.asciidoc
	$(call buildAsciidoc)

Hootenanny\ -\ User\ Guide.pdf: \
	HootenannyUserGuide.asciidoc \
	tmp/asciidoc-config \
	../HOOT_VERSION_FILE \
	docinfo.xml \
	styles/* \
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

Hootenanny\ -\ User\ Interface\ Guide.pdf: \
	HootenannyUI.asciidoc \
	tmp/asciidoc-config \
	../HOOT_VERSION_FILE \
	docinfo.xml \
	styles/* \
	tmp/RulesLib.asciidoc \
	tmp/RulesTemplate.asciidoc \
	tmp/rules-lib-doxygen \
	tmp/rules-template-doxygen \
	user/*.asciidoc
	$(call buildAsciidoc)

Hootenanny\ -\ Developer\ Guide.pdf: \
	HootenannyDeveloperGuide.asciidoc \
	tmp/asciidoc-config \
	../HOOT_VERSION_FILE \
	docinfo.xml \
	styles/* \
	developer/* \
	tmp/RulesLib.asciidoc \
	tmp/RulesTemplate.asciidoc \
	tmp/rules-lib-doxygen \
	tmp/rules-template-doxygen \
	user/*.asciidoc
	$(call buildAsciidoc)

Hootenanny\ -\ Installation\ Instructions.pdf: \
	HootenannyInstallDocs.asciidoc \
	tmp/asciidoc-config \
	../HOOT_VERSION_FILE \
	docinfo.xml \
	install/* \
	styles/*
	$(call buildAsciidoc)


