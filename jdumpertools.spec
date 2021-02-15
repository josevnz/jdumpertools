Name:           jdumpertools
# TODO: Figure out a better way to update version here and on Makefile
Version:        0        
Release:        1%{?dist}
Summary:        Tools to dump output from several tools in JSON format, quickly.

License:        GPL
URL:            https://github.com/kodegeek-test/jdumpertools
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  bash,tar,gzip,rpmdevtools,rpmlint
Requires:       bash

%global debug_package %{nil}

%description

This is a collection of programs that can be used to dump output in JSON format, so it can be ingested by other tools.

* jdu: Similar to UNIX 'du' command.
* jutmp: UTMP database dumper

All the tools support the '--help' flag.

%prep
%setup -q -n jdumpertools

%build
make all

%install
rm -rf $RPM_BUILD_ROOT
/usr/bin/mkdir -p $RPM_BUILD_ROOT/%{_bindir}
/usr/bin/cp -v -p jdu jutmp $RPM_BUILD_ROOT/%{_bindir}

%clean
rm -rf $RPM_BUILD_ROOT

%files
%{_bindir}/jdu
%{_bindir}/jutmp
%license LICENSE
%doc README.md


%changelog
* Mon Jan  4 2021 Jose Vicente Nunez <kodegeek.com@protonmail.com> - 0.1
- First version being packaged
