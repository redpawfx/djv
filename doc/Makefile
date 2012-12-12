SRC = \
  credits.html \
  djv_convert.html \
  djv_info.html \
  djv_ls.html \
  djv_view.html \
  doc.html \
  download.html \
  faq.html \
  file_browser.html \
  file.html \
  image.html \
  index.html \
  install.html \
  legal.html \
  quote.html \
  screenshot.html

all: $(SRC)

clean:
	rm -f $(SRC)

index.html: index.htm util.htm
	m4 util.htm $< > $@

.SUFFIXES:
.SUFFIXES: .htm .html

.htm.html: util.htm
	m4 util.htm $< > $@
