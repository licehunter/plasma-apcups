# Introduction #

Once your ApcUps plasmoid has been [installed](Installation.md), you have to add and configure one or more instances of it to your desktop.


# Configuration #

To add it to your desktop (an [activity](http://userbase.kde.org/Glossary#Activities) in KDE-speak) simply click on on _Add Widgets..._ on the context menu or from the cashew, look for the ApcUps plasmoid and drag and drop it to the desktop, with the result shown in the image below.

![http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot01.png](http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot01.png)

By default, ApcUps attempts to connect to localhost on port 3551. If your [apcupsd](http://www.apcupsd.com/) daemon is running on a different machine, you will need to open the plasmoid's configuration dialogue to enter the correct host name or IP address.

You may also adjust the tresholds for triggering battery charge and load warnings on the same dialogue. Press Shift+F1 while the cursor is on one of the fields, or use the _What's This?_ question mark icon to get additional help on any parts of the configuration form.

![http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot02.png](http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot02.png)

Notifications from the plasmoid may be configured via KDE System Settings.

![http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot08.png](http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot08.png)

# Usage #

After a successful connection, ApcUps will show the target UPS load as a percentage of maximum rated load, and the battery charge, also as a percentage.

![http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot03.png](http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot03.png)

If the plasmoid is dragged into a panel it will iconify itself.  Click on the icon to show the load and charge details.

![http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot04.png](http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot04.png)

The icon itself will change colour according to the UPS status, as follows:

| **Colour** | **Meaning** |
|:-----------|:------------|
| Green      | Normal operation |
| Yellow     | The UPS is running on batteries |
| Red        | The UPS is running on batteries and the battery charge is below 20%, or the load percentage is above 85% |
| Gray       | A communications error has occurred. The status field will show the error message |

When the UPS is on the ONBATT (running on batteries) state, an indication of estimated time left is shown

![http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot05.png](http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot05.png)

Note that it is possible to have an instance of ApcUps on the system tray. If you only have one UPS to track, this might be the best approach in terms of saving screen real estate.

![http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot06.png](http://wiki.plasma-apcups.googlecode.com/hg/ScreenShot06.png)