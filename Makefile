.DEFAULT_GOAL := all
VERSION=0
CC=gcc
# -Q --help=optimizers
#DEBUG=-DDEBUG
PROFILING=-pg
CCFLAGS=-Wall -g -Og -Wextra -Werror -std=c11 $(PROFILING) $(DEBUG) -DJDUMPERTOOLS_VERSION=$(VERSION)
TARGETS=jdu jutmp
NAME=jdumpertools
TARFILE=$(NAME)-$(VERSION).tar.gz

all: $(TARGETS)

libjdumpertools.so: jdumpertools.h jdumpertools.c
	$(CC) $(CCFLAGS) -fPIC $^ -I $(PWD) -shared -o $@

jdu: jdumpertools.h jdu.c libjdumpertools.so
	$(CC) $^ $(CCFLAGS) -L $(PWD) -l jdumpertools -o $@

jutmp: jdumpertools.h jutmp.c
	$(CC) $^ $(CCFLAGS) -L $(PWD) -l jdumpertools -o $@

clean:
	/bin/rm -f $(TARGETS)

rpm:
	/usr/bin/rpmdev-setuptree
	/usr/bin/tar --exclude-vcs --directory ../ --create --verbose --gzip --file $(HOME)/rpmbuild/SOURCES/$(TARFILE) $(NAME)
	/usr/bin/rpmbuild -ba jdumpertools.spec

docker: all
    # Not ready yet
	/usr/bin/docker build $(PWD)
