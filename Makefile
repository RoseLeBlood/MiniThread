VERSION=2.10.7
NAME=miniThread-$(VERSION).tar.gz
BINDIR = $(DESTDIR)/opt/miniThread/

all: build
clean:
	rm -r release/*
build:
	pio package pack >> make.log
	mv *.tar.gz release/$(NAME)
install:
	mkdir -p $(BINDIR)/miniThread/
	install --mode=666 release/$(NAME) $(BINDIR)/$(NAME)
	@printf "\e[1;32mAdd \"lib_deps = $(BINDIR)/$(NAME)\" to your platformio.ini\n\n\e[0m"
uninstall:
	rm -r $(BINDIR)/miniThread

publish:
	pio package publish