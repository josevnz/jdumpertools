# Docker image to run jdumpertools from inside a container
# This tools need read only access to your bare metal utmp and disk, no privileged access
FROM fedora:33 as fedora
RUN useradd jdumpertools -g jdumpertools && \
    /usr/bin/mkdir -p -v /usr/local/bin/ && \
    /usr/bin/mkdir -p -v /usr/share/docs/jdumpertools/
COPY --chown=jdumpertools:jdumpertools jdu jdumpertools /usr/local/bin/
COPY --chown=jdumpertools:jdumpertools README.md /usr/local/share/docs/jdumpertools/
USER jdumpertools
ENTRYPOINT [ "/bin/bash" ]