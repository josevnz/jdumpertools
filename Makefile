.DEFAULT_GOAL := all
VERSION=0
CC=gcc
# -Q --help=optimizers
#DEBUG=-DDEBUG
CCFLAGS=-Wall -Wall -g -Og -Wextra -Werror -std=c11 $(DEBUG) -DJDUMPERTOOLS_VERSION=$(VERSION)
TARGETS=jdu jutmp
NAME=jdumpertools
TARFILE=$(NAME)-$(VERSION).tar.gz

all: $(TARGETS)

jdu: jdumpertools.h jdu.c
	$(CC) $^ $(CCFLAGS) -o $@

jutmp: jdumpertools.h jutmp.c
	$(CC) $^ $(CCFLAGS) -o $@

clean:
	/bin/rm -f $(TARGETS)

rpm:
	/usr/bin/rpmdev-setuptree
	/usr/bin/tar --exclude-vcs --directory ../ --create --verbose --gzip --file $(HOME)/rpmbuild/SOURCES/$(TARFILE) $(NAME)
	/usr/bin/rpmbuild -ba jdumpertools.spec

docker: all
	/usr/bin/docker build $(PWD)
