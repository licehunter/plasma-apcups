Name:                   plasma-apcups
License:                GPLv3
Group:                  System/GUI/KDE
Summary:                Uninterruptible Power Supply monitor for APC brand UPSes
Version:                @VERSION@
Release:                1
URL:                    http://navlost.eu/devel/apcups/
Source:                 %{name}-%{version}.tar.gz
BuildRoot:              %{_tmppath}/%{name}-%{version}-build
BuildRequires:          cmake >= 2.6, libqt4-devel >= 4.5, libkde4-devel >= 4.3, update-desktop-files
Requires:               libqt4 >= 4.5.1, kdebase4-runtime >= 4.3.2

%description
A KDE Plasma component that monitors uninterruptible power
supplies controlled by apcupsd <http://www.apcupsd.com/>.
It can connect to any UPS (or more precisely: any apcupsd
daemon) which is reachable over the network.

%prep
%setup -n %{name}-%{version}  

%build  
%cmake_kde4 -d builddir    
%make_jobs    
   
%install
cd builddir  
%kde4_makeinstall
%suse_update_desktop_file -n $RPM_BUILD_ROOT/usr/share/kde4/services/plasma-engine-apcups.desktop
%suse_update_desktop_file -n $RPM_BUILD_ROOT/usr/share/kde4/services/plasma-applet-apcups.desktop
%kde_post_install

%clean  
rm -rf $RPM_BUILD_ROOT  
   
%files
%defattr(-,root,root)
%_kde4_modulesdir/plasma_engine_apcups.so
%_kde4_servicesdir/plasma-engine-apcups.desktop
%_kde4_modulesdir/plasma-applet-apcups.so
%_kde4_servicesdir/plasma-applet-apcups.desktop
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

%changelog
