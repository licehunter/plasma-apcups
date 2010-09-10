#!/bin/bash

[ -z "$(which extractrc)" ] && {
    echo "Required application extractrc not found in search path." >&2
    echo "Please install the application or add it to your \$PATH" >&2
    exit -1
}

[ -z "$(which xgettext)" ] && {
    echo "Required application xgettext not found in search path." >&2
    echo "Please install the application or add it to your \$PATH" >&2
    exit -1
}

[ -z "$(which msgmerge)" ] && {
    echo "Required application msgmerge not found in search path." >&2
    echo "Please install the application or add it to your \$PATH" >&2
    exit -1
}

WDIR=$(dirname $0)

UIFILELIST=$(find $WDIR/../ -name '*.ui')
extractrc $UIFILELIST >$WDIR/rc.cpp

CPPFILELIST=$(find $WDIR/../ -name '*.cpp')

xgettext -C -o $WDIR/plasma_applet_apcups.pot -kde -ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 \
        -ktr2i18n:1 -kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 \
        -kki18ncp:1c,2,3 $CPPFILELIST

rm $WDIR/rc.cpp

for f in $WDIR/*.po; do
    echo -n "Merging $f "
    msgmerge -U "$f" "$WDIR/plasma_applet_apcups.pot"
done
