Name:           jdumpertools
# TODO: Figure out a better way to update version here and on Makefile
Version:        v0.1        
Release:        1%{?dist}
Summary:        Programs that can be used to dump Linux usage data in JSON format. 

License:        Apache License 2.0
URL:            https://github.com/josevnz/jdumpertools
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  bash,tar,gzip,rpmdevtools,rpmlint,make,gcc >= 10.2.1
Requires:       bash

%global debug_package %{nil}

%description

Jdumpertools is a collection of programs that can be used to dump linux usage data in JSON format, so it can be ingested by other tools.

* jdu: Similar to UNIX 'du' command.
* jutmp: UTMP database dumper

%prep
%setup -q -n jdumpertools

%build
make all

%install
rm -rf %{buildroot}
/usr/bin/mkdir -p %{buildroot}/%{_bindir}
/usr/bin/cp -v -p jdu jutmp %{buildroot}/%{_bindir}
/usr/bin/mkdir -p %{buildroot}/%{_libdir}
/usr/bin/cp -v -p libjdumpertools.so %{buildroot}/%{_libdir}

%clean
rm -rf %{buildroot}

%files
%{_bindir}/jdu
%{_bindir}/jutmp
%{_libdir}/libjdumpertools.so
%license LICENSE
%doc README.md


%changelog
* Mon Jan  4 2021 Jose Vicente Nunez <kodegeek.com@protonmail.com> - 0.1
- First version being packaged
