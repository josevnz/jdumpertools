# Docker image to run jdumpertools from inside a container
# This tools need read only access to your bare metal utmp and disk, no privileged access
# Author: Jose Vicente Nunez
FROM --platform=linux/amd64 fedora:33 as fedora
LABEL "author"="Jose Vicente Nunez"
LABEL "description"="jdumper tools - Programs that can be used to dump Linux usage data in JSON format."
RUN useradd --comment "jdumpertools batch user" -no-log-init --no-create-home --user-group jdumpertools
COPY --chown=jdumpertools jdu jutmp /usr/bin/
COPY --chown=jdumpertools README.md /usr/local/share/docs/jdumpertools/
COPY --chown=jdumpertools libjdumpertools.so /usr/lib64/
RUN /bin/chmod a+xr /usr/bin/jdu /usr/bin/jutmp
USER jdumpertools:jdumpertools
HEALTHCHECK NONE
ENTRYPOINT [ "/bin/bash" ]