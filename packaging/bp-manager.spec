Name:       bp-manager
Summary:    BestPractice Tizen native API
Version:    0.0.1
Release:    0
Group:      System/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1:    bp-manager.service
Source1001:     bp-manager.manifest
BuildRequires:  cmake
BuildRequires:  tidl
BuildRequires:  pkgconfig(rpc-port)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(bundle)
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(eina)
BuildRequires:  pkgconfig(ecore)
#BuildRequires:  pkgconfig(gmock)
%if 0%{?gcov:1}
BuildRequires:  lcov
BuildRequires:  zip
%endif

%description
An best practice tizen native library in Tizen C API

%package devel
Summary:  An best practice tizen native library in Tizen C API (Development)
Group:    System/API
Requires: %{name} = %{version}-%{release}

%description devel
An best practice tizen native library in Tizen C API (Development) package.

#################################################
# bp-manager-unittests
#################################################
%package -n bp-manager-unittests
Summary:    GTest for bp-tizen API
Group:      Development/Libraries
Requires:   %{name}

%description -n bp-manager-unittests
GTest for bp-tizen API

#################################################
# bp-manager-gcov
#################################################
%if 0%{?gcov:1}
%package gcov
Summary:    Best practice tizen native API(gcov)
Group:      Application Framework/Testing

%description gcov
Best practice tizen gcov objects
%endif

#################################################
# bp-manager build
#################################################

%prep
%setup -q
cp %{SOURCE1001} .
tidlc -p -l C -i ./src/tidl/remote_service.tidl -o remote_service_proxy
mv remote_service_proxy.c src/
mv remote_service_proxy.h src/ 
tidlc -s -l C -i ./src/tidl/stub_service.tidl -o stub_service_stub
mv stub_service_stub.c src/ 
mv stub_service_stub.h src/

%build
%if 0%{?gcov:1}
export CFLAGS+=" -fprofile-arcs -ftest-coverage"
export CXXFLAGS+=" -fprofile-arcs -ftest-coverage"
export FFLAGS+=" -fprofile-arcs -ftest-coverage"
export LDFLAGS+=" -lgcov"
%endif

MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER} \
	-DBIN_INSTALL_DIR:PATH=%{_bindir}
#	-DTZ_SYS_ETC=%{TZ_SYS_ETC} 
#	-D_APPFW_FEATURE_ALARM_MANAGER_MODULE_LOG:BOOL=${_APPFW_FEATURE_ALARM_MANAGER_MODULE_LOG} \
#	-DALARM_CONF_DIR=%{_datadir}/alarm-manager \
#	-DBUILD_GTESTS=%{?gtests:1}%{!?gtests:0} \
#	-DBUILD_GCOV=%{?gcov:1}%{!?gcov:0} \

%__make %{?jobs:-j%jobs}

%if 0%{?gcov:1}
mkdir -p gcov-obj
find . -name '*.gcno' -exec cp '{}' gcov-obj ';'
%endif

%check
ctest --output-on-failure %{?_smp_mflags}
%if 0%{?gcov:1}
lcov -c --ignore-errors graph --no-external -q -d . -o bp-tizen.info
genhtml bp-tizen.info -o bp-tizen.out
zip -r bp-tizen.zip bp-tizen.out bp-tizen.info
install -m 0644 bp-tizen.zip %{buildroot}%{_datadir}/gcov/
%endif

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}%{_unitdir}/multi-user.target.wants
mkdir -p %{buildroot}%{_unitdir_user}/sockets.target.wants

# TODO(vincent): Need to copy service file for systemd
install -m 0644 %SOURCE1 %{buildroot}%{_unitdir}/bp-manager.service

# TODO(vincent): Need to launch at boot
ln -s bp-manager.service %{buildroot}%{_unitdir}/multi-user.target.wants/bp-manager.service

# TODO(vincent): missing this module's conf file (aka alarm-manager.conf)
# install -m 0644 %source4 /buildroot/tmpfilesdir/alarm-manager.conf 
# /usr/lib/tmpfiles.d/alarm-manager.conf
# d /run/alarm_agent 0777 root users -
# d /var/log/appfw/alarmmgr_log 0775 app_fw app_fw -

# TODO(vincent): if necessary udev register
# mkdir %{buildroot}%{_libdir}/udev/rules.d
# /lib/udev/rules.d/99-rtc.rules

%if 0%{?gcov:1}
mkdir -p %{buildroot}%{_datadir}/gcov/obj
install -m 0644 gcov-obj/* %{buildroot}%{_datadir}/gcov/obj
%endif

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

echo %{_sysconfdir}

#rm -f src/remote_service_proxy.c
#rm -f src/remote_service_stub.c
#rm -f src/remote_service_proxy.h
#rm -f src/remote_service_stub.h

# TODO(vincent): what is -n option?
%files -n bp-manager

# TODO(vincent): what is this manifest do? SMACK?
%manifest bp-manager.manifest

# TODO(vincent): _bindir is /usr/bin. This put the exec file.
#                May cmake create this in /usr/bin (may make install?)
%{_bindir}/bp-manager*

# TODO(vincent): _unitdir is /usr/lib/systemd/systemd
# TODO(vincent): Purpose: All services must be defined in this folder to be handled by systemd.
#                Line 115 install it
%attr(0644,root,root) %{_unitdir}/bp-manager.service

# TODO(vincent): Add to /usr/lib/systemd/systemd/multi-user.target.wants to start this service at boot.
%{_unitdir}/multi-user.target.wants/bp-manager.service

# TODO(vincent): {_datadir} is /usr/share and it describes d-bus service name, systemservice info
#                 Why we need it? for what? why it is org.tizen.xxx format?
#                 How /usr/share/dbus-1/system-service/org.tizen.bp.manager.service installed? in src/CMakeFiles.txt
#                 INSTALL(FILES ${DBUS_INTERFACE}.service DESTINATION ${SHARE_INSTALL_PREFIX}/dbus-1/system-services/)
#                 if applications want to be activated on the system and session buses, then service files
#                 should be installed 
%attr(0644,root,root) %{_datadir}/dbus-1/system-services/org.tizen.bp.manager.service

%license LICENSE

# TODO(vincent): what is this file???
%config %{_sysconfdir}/dbus-1/system.d/org.tizen.bp.manager.conf

#%post -n bp-manager-unittests
#%if 0%{?gcov:1}
#%{_bindir}/bp-manager_unittests
#%endif

#################################################
# bp-manager-unittests
#################################################
#%files -n bp-manager-unittests
#%{_bindir}/bp-manager_unittests

#################################################
# bp-manager-gcov
#################################################
%if 0%{?gcov:1}
%files gcov
%{_datadir}/gcov/*
%endif
