Name:           jdumpertools
# TODO: Figure out a better way to update version here and on Makefile
%global major 0
Version:        v%{major}.2
Release:        1%{?dist}
Summary:        Programs that can be used to dump Linux usage data in JSON format

License:        ASL 2.0
URL:            https://github.com/josevnz/jdumpertools
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  bash,tar,gzip,rpmdevtools,rpmlint,make,gcc >= 10.2.1
Requires:       bash

%global debug_package %{nil}

%description

Jdumpertools is a collection of programs that can be used to dump
linux usage data in JSON format, so it can be ingested by other tools.

* jdu: Similar to UNIX '/bin/du' command.
* jutmp: UTMP database dumper

%prep
%setup -q -n jdumpertools

%build
make all

%install
rm -rf %{buildroot}
/usr/bin/mkdir -p %{buildroot}/%{_bindir}
/usr/bin/mkdir -p %{buildroot}/%{_mandir}/man8
/usr/bin/cp -v -p jdu jutmp %{buildroot}/%{_bindir}
/usr/bin/cp -v -p jdu.1 jutmp.1 %{buildroot}/%{_mandir}/man8/
/usr/bin/gzip %{buildroot}/%{_mandir}/man8/*
/usr/bin/mkdir -p %{buildroot}/%{_libdir}
/usr/bin/cp -v -p libjdumpertools.so.%{major} %{buildroot}/%{_libdir}
/usr/bin/strip %{buildroot}/%{_bindir}/{jdu,jutmp}
/usr/bin/strip %{buildroot}/%{_libdir}/*

%clean
rm -rf %{buildroot}

%files
%{_bindir}/jdu
%{_bindir}/jutmp
%{_libdir}/libjdumpertools.so.*
%license LICENSE
%doc README.md
%doc %{_mandir}/man8/jdu.1.gz
%doc %{_mandir}/man8/jutmp.1.gz


%changelog
* Sun Oct  3 2021 Jose Vicente Nunez <kodegeek.com@protonmail.com> - v0.2-1
- Applied fixes from rpmlint: man page, typos on spec file, striped binaries, etc.
* Mon Jan  4 2021 Jose Vicente Nunez <kodegeek.com@protonmail.com> - v0.1-1
- First version being packaged
