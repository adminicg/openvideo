Summary:	OpenVideo
Name:		openvideo
Version:	1.0.0
Release:	1
License:	GPL
Group:		Development/Libraries
Source:		%{name}-%{version}.tar.bz2
#Source1:	%{name}-scons.tar.gz
#Vendor:		...
#URL:		http://www.grinninglizard.com/tinyxml
Packager:	Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Prefix:		/usr
BuildRoot: 	%{_tmppath}/buildroot-%{name}-%{version}
Requires:	tinyxmlmod giflib ACE
BuildRequires:	scons tinyxmlmod-devel giflib-devel ACE-devel

%define _prefix %{prefix}

%description
...

%prep
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
%setup

%build
scons
#PREFIX=%{_prefix} LIBDIR=%{_libdir} INCLUDEDIR=%{_includedir}

%install
scons --cache-disable DESTDIR=$RPM_BUILD_ROOT PREFIX=%{_prefix} LIBDIR=%{_libdir} INCLUDEDIR=%{_includedir} install

%clean
scons -c
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_bindir}/openvideo
%{_libdir}/*.so*

%package devel
Summary:	OpenVideo headers
Group:		Development/Libraries
Requires:	%{name} = %{version}

%description devel
This package contains header files and include files that are needed for development using OpenVideo.

%files devel
%defattr(-,root,root)
%{_libdir}/pkgconfig/*
%{_prefix}/include/*
