.DEFAULT_GOAL := all
MAJOR=0
VERSION=v${MAJOR}.2
CC=gcc
# -Q --help=optimizers
#DEBUG=-DDEBUG
#PROFILING=-pg
CCFLAGS=-Wall -g -Og -Wextra -Werror -Werror=format-security -std=c11 $(PROFILING) $(DEBUG) -DJDUMPERTOOLS_VERSION=$(VERSION)
TARGETS=jdu jutmp libjdumpertools.so.${MAJOR}
NAME=jdumpertools
TARFILE=$(NAME)-$(VERSION).tar.gz
DOCKER_FLAGS=--tag josevnz/jdumpertools:$(VERSION) --rm --pull --file Dockerfile --compress

all: $(TARGETS)

libjdumpertools.so.${MAJOR}: jdumpertools.h jdumpertools.c
	$(CC) $(CCFLAGS) -fPIC $^ -I $(PWD) -shared -Wl,-soname,libjdumpertools.so.${MAJOR} -o $@

jdu: jdumpertools.h jdu.c libjdumpertools.so.${MAJOR}
	$(CC) $^ $(CCFLAGS) -L $(PWD) -l jdumpertools -o $@

jutmp: jdumpertools.h jutmp.c
	$(CC) $^ $(CCFLAGS) -L $(PWD) -l jdumpertools -o $@

clean:
	/bin/rm -f $(TARGETS)

rpm: all
	test -x /usr/bin/rpmdev-setuptree && /usr/bin/rpmdev-setuptree|| /bin/mkdir -p -v ${HOME}/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
	/usr/bin/tar --exclude-vcs --directory ../ --create --verbose --gzip --file $(HOME)/rpmbuild/SOURCES/$(TARFILE) $(NAME)
	/usr/bin/rpmbuild -ba jdumpertools.spec

docker: all
    # Not ready yet
	DOCKER_BUILDKIT=1 /usr/bin/docker build $(DOCKER_FLAGS) $(PWD)
