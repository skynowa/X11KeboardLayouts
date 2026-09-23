#include <QtTest>

#include "LayoutCode.h"

class LayoutCodeTest : public QObject
{
    Q_OBJECT

private slots:
    void resolvesConfiguredGroup();
    void usesFallbackForMissingGroup();
    void ignoresFocusAndDuplicateGroupNotifications();
};

void
LayoutCodeTest::resolvesConfiguredGroup()
{
    constexpr char namesData[] = "evdev\0pc105\0ru,us,ua\0,,\0grp:ctrl_shift_toggle\0";
    const QByteArray names(namesData, sizeof(namesData) - 1);
    QCOMPARE(xkbLayoutCodeFromProperty(names, 0), QStringLiteral("ru"));
    QCOMPARE(xkbLayoutCodeFromProperty(names, 1), QStringLiteral("us"));
    QCOMPARE(xkbLayoutCodeFromProperty(names, 2), QStringLiteral("ua"));
}

void
LayoutCodeTest::usesFallbackForMissingGroup()
{
    constexpr char namesData[] = "evdev\0pc105\0us,ru\0,\0\0";
    const QByteArray names(namesData, sizeof(namesData) - 1);
    QCOMPARE(xkbLayoutCodeFromProperty(names, 3), QStringLiteral("G4"));
    QCOMPARE(xkbLayoutCodeFromProperty({}, 0), QStringLiteral("G1"));
}

void
LayoutCodeTest::ignoresFocusAndDuplicateGroupNotifications()
{
    int observedGroup {0};
    XkbStateNotifyEvent event {};
    event.changed = XkbGroupStateMask;
    event.group = 1;

    // Focus changes update the observed group without opening a popup.
    event.event_type = 0;
    QVERIFY(!xkbShouldShowLayoutChange(event, &observedGroup));
    QCOMPARE(observedGroup, 1);

    event.event_type = KeyPress;
    QVERIFY(!xkbShouldShowLayoutChange(event, &observedGroup));

    event.group = 0;
    event.event_type = ButtonPress;
    QVERIFY(!xkbShouldShowLayoutChange(event, &observedGroup));
    QCOMPARE(observedGroup, 0);

    event.group = 1;
    event.event_type = KeyPress;
    QVERIFY(xkbShouldShowLayoutChange(event, &observedGroup));
    QCOMPARE(observedGroup, 1);

    event.changed = XkbModifierStateMask;
    event.group = 0;
    QVERIFY(!xkbShouldShowLayoutChange(event, &observedGroup));
    QCOMPARE(observedGroup, 1);
}

QTEST_GUILESS_MAIN(LayoutCodeTest)
#include "LayoutCodeTest.moc"
