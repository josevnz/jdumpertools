# Figure out a better way to update version here and on the Makefile
VERSION=0
CC=gcc
CCFLAGS=-Wall -g -O2 -Wextra -std=c11 -ljansson -DJDUMPERTOOLS_VERSION=$(VERSION)
TARGETS=jdu jutmp
NAME=jdumpertools
TARFILE=$(NAME)-$(VERSION).tar.gz

jdu: jdu.c
	$(CC) $^ $(CCFLAGS) -o $@

jutmp: jutmp.c
	$(CC) $^ $(CCFLAGS) -o $@

clean:
	/bin/rm -f $(TARGETS)

rpm:
	/usr/bin/rpmdev-setuptree
	/usr/bin/tar --exclude-vcs --directory ../ --create --verbose --gzip --file $(HOME)/rpmbuild/SOURCES/$(TARFILE) $(NAME)
	/usr/bin/rpmbuild -ba jdumpertools.spec

docker: all
	/usr/bin/docker build $(PWD)

all: $(TARGETS)
