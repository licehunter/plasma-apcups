%package plasma-dataengine-apcups
Name:                   plasma-dataengine-apcups
License:                GPLv3
Group:                  System/GUI/KDE
Summary:                Uninterruptible Power Supply data engine for APC brand UPSes.
Version:                @VERSION@
Release:                1
URL:                    http://navlost.eu/devel/apcups/
Source:                 %{name}-%{version}.tar.gz
BuildRoot:              %{_tmppath}/%{name}-%{version}-build
BuildRequires:          cmake >= 2.6, libqt4-devel >= 4.5, libkde4-devel >= 4.3, update-desktop-files
Requires:               libqt4 >= 4.5.1, kdebase4-runtime >= 4.3.2
Recommends:             plasmoid-apcups

%package plasmoid-apcups
Name:                   plasmoid-apcups
License:                GPLv3
Group:                  System/GUI/KDE
Summary:                Uninterruptible Power Supply monitor for APC brand UPSes.
Version:                @VERSION@
Release:                1
URL:                    http://navlost.eu/devel/apcups/
Source:                 %{name}-%{version}.tar.gz
BuildRoot:              %{_tmppath}/%{name}-%{version}-build
BuildRequires:          cmake >= 2.6, libqt4-devel >= 4.5, libkde4-devel >= 4.3, update-desktop-files
Requires:               libqt4 >= 4.5.1, kdebase4-runtime >= 4.3.2, plasma-dataengine-apcups

%description plasmoid-apcups
A KDE Plasma widget that monitors uninterruptible power supplies
controlled by apcupsd <http://www.apcupsd.com/>.  It can run
iconified (e.g., in a panel) and shows whether the target UPS
is running low on battery or is subject to overload.  It can
connect to any UPS (or more precisely: any apcupsd daemon)
which is reachable over the network.

%description plasma-dataengine-apcups
A KDE Plasma data engine that monitors uninterruptible power
supplies controlled by apcupsd <http://www.apcupsd.com/>.
It can connect to any UPS (or more precisely: any apcupsd
daemon) which is reachable over the network.
See the apcups applet for a plasmoid using this engine.

%prep
%setup -n %{name}-%{version}  

%build  
%cmake_kde4 -d builddir    
%make_jobs    
   
%install plasma-dataengine-apcups
cd builddir  
%kde4_makeinstall
%suse_update_desktop_file -n $RPM_BUILD_ROOT/usr/share/kde4/services/plasma-engine-apcups.desktop
%kde_post_install

%install plasmoid-apcups
cd builddir  
%kde4_makeinstall    
%suse_update_desktop_file -n $RPM_BUILD_ROOT/usr/share/kde4/services/plasma-applet-apcups.desktop
%kde_post_install

%clean  
rm -rf $RPM_BUILD_ROOT  
   
%files plasma-dataengine-apcups
%defattr(-,root,root)
%_kde4_modulesdir/plasma_engine_apcups.so
%_kde4_servicesdir/plasma-engine-apcups.desktop

%files plasmoid-apcups
%defattr(-,root,root)
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
