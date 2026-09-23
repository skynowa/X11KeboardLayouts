#include <QtTest>

#include "LayoutCode.h"

class LayoutCodeTest : public QObject
{
    Q_OBJECT

private slots:
    void resolvesConfiguredGroup();
    void usesFallbackForMissingGroup();
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

QTEST_GUILESS_MAIN(LayoutCodeTest)
#include "LayoutCodeTest.moc"
