# These macros are not present on the target distribution and are provided explicitly here
%if 0%{?fedora}
%define make_jobs %{__make} %{?_smp_mflags} VERBOSE=1
%define cmake_kde4(d:) \
  QTDIR="%{_qt4_prefix}" ; export QTDIR ; \
  PATH="%{_qt4_bindir}:$PATH" ; export PATH ; \
  CFLAGS="${CFLAGS:-%optflags}" ; export CFLAGS ; \
  CXXFLAGS="${CXXFLAGS:-%optflags}" ; export CXXFLAGS ; \
  FFLAGS="${FFLAGS:-%optflags}" ; export FFLAGS ; \
  bdir=. \
  %{-d:dir=%{-d*} \
  mkdir $dir \
  cd $dir \
  bdir=.. } \
  %{__cmake} $bdir \\\
    -DCMAKE_BUILD_TYPE=%{_kde4_buildtype} \\\
    -DBUILD_SHARED_LIBS:BOOL=ON \\\
    -DCMAKE_INSTALL_PREFIX:PATH=%{_kde4_prefix} \\\
    -DCMAKE_VERBOSE_MAKEFILE=ON \\\
    -DDATA_INSTALL_DIR:PATH=%{_kde4_appsdir} \\\
    -DINCLUDE_INSTALL_DIR:PATH=%{_kde4_includedir} \\\
    -DLIB_INSTALL_DIR:PATH=%{_kde4_libdir} \\\
    -DLIBEXEC_INSTALL_DIR:PATH=%{_kde4_libexecdir} \\\
    -DSYSCONF_INSTALL_DIR:PATH=%{_kde4_sysconfdir} \\\
%if "%{?_lib}" == "lib64" \
     %{?_cmake_lib_suffix64} \\\
%endif \
     %{?_cmake_skip_rpath} \\\
     %* \
%{nil}
%define kde4_makeinstall make DESTDIR=%{?buildroot:%{buildroot}} install/fast
%define kde_post_install %{nil}

## KDE4 Directories
%define _kde4_bindir          %{_bindir}
%define _kde4_modulesdir      %{_libdir}/kde4
%define _kde4_libdir          %{_libdir}
%define _kde4_iconsdir        %{_datadir}/icons
%define _kde4_applicationsdir %{_datadir}/applications/kde4
%define _kde4_appsdir         %{_datadir}/kde4/apps
%define _kde4_configdir       %{_datadir}/kde4/config
%define _kde4_kcfgdir         %{_datadir}/kde4/config.kcfg
%define _kde4_servicesdir     %{_datadir}/kde4/services
%define _kde4_servicetypesdir %{_datadir}/kde4/servicetypes
%define _kde4_htmldir         %{_docdir}/kde/HTML

%endif

Name:                   plasma-apcups
License:                GPLv3
Group:                  System/GUI/KDE
Summary:                Uninterruptible Power Supply monitor for APC brand UPSes
Version:                @VERSION@
Release:                1
URL:                    http://navlost.eu/devel/apcups/
Source:                 %{name}-%{version}.tar.gz
BuildRoot:              %{_tmppath}/%{name}-%{version}-build
BuildRequires:          cmake >= 2.6, libqt4-devel >= 4.5
%if 0%{?suse_version} 
BuildRequires:          libkde4-devel >= 4.3, update-desktop-files
Requires:               kdebase4-runtime >= 4.3.2
%else
BuildRequires:          kdelibs-devel >= 4.3, gcc-c++
Requires:               kdebase-runtime >= 4.3.2
%endif

%description
A KDE Plasma component that monitors uninterruptible power
supplies controlled by apcupsd <http://www.apcupsd.com/>.
It can connect to any UPS (or more precisely: any apcupsd
daemon) which is reachable over the network.

%prep
%setup -n %{name}-%{version}  

%build  
%cmake_kde4 %{?_cmake_skip_rpath}
%make_jobs    
   
%install
 
%kde4_makeinstall
%if 0%{?suse_version} 
%suse_update_desktop_file -n $RPM_BUILD_ROOT/usr/share/kde4/services/plasma-engine-apcups.desktop
%suse_update_desktop_file -n $RPM_BUILD_ROOT/usr/share/kde4/services/plasma-applet-apcups.desktop
%endif
%kde_post_install
install -d $RPM_BUILD_ROOT%{_datadir}/locale %{_datadir}/locale

%clean  
rm -rf $RPM_BUILD_ROOT  
   
%files
%defattr(-,root,root)
%dir %_kde4_appsdir/plasma-applet-apcups
%_kde4_modulesdir/plasma_engine_apcups.so
%_kde4_servicesdir/plasma-engine-apcups.desktop
%_kde4_modulesdir/plasma-applet-apcups.so
%_kde4_servicesdir/plasma-applet-apcups.desktop
%_kde4_appsdir/plasma-applet-apcups/plasma-applet-apcups.notifyrc
%_kde4_iconsdir/hicolor/64x64/apps/./apcups.png
%_kde4_iconsdir/hicolor/48x48/apps/./apcups.png
%_kde4_iconsdir/hicolor/32x32/apps/./apcups_warningstate.png
%_kde4_iconsdir/hicolor/48x48/apps/./apcups_criticalstate.png
%_kde4_iconsdir/hicolor/48x48/apps/./apcups_normalstate.png
%_kde4_iconsdir/hicolor/64x64/apps/./apcups_criticalstate.png
%_kde4_iconsdir/hicolor/32x32/apps/./apcups.png
%_kde4_iconsdir/hicolor/64x64/apps/./apcups_normalstate.png
%_kde4_iconsdir/hicolor/32x32/apps/./apcups_normalstate.png
%_kde4_iconsdir/hicolor/48x48/apps/./apcups_warningstate.png
%_kde4_iconsdir/hicolor/64x64/apps/./apcups_warningstate.png
%_kde4_iconsdir/hicolor/32x32/apps/./apcups_criticalstate.png
%_datadir/locale/*/*/*

%changelog
