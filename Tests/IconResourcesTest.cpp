#include <QPixmap>
#include <QtTest>

class IconResourcesTest : public QObject
{
    Q_OBJECT

private slots:
    void loadsIcons();
};

void
IconResourcesTest::loadsIcons()
{
    for (const QString &code : {QStringLiteral("en"), QStringLiteral("ru"),
                                QStringLiteral("ru-ru"), QStringLiteral("ua")}) {
        const QPixmap icon(QStringLiteral(":/icons/") + code + QStringLiteral(".svg"));
        QVERIFY2(!icon.isNull(), qPrintable(code));
    }
}

QTEST_MAIN(IconResourcesTest)
#include "IconResourcesTest.moc"
