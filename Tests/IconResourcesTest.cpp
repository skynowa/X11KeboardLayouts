#include <QPixmap>
#include <QtTest>

#include "Widget.h"

class IconResourcesTest : public QObject
{
    Q_OBJECT

private slots:
    void loadsIcons();
    void showsTextForUnsupportedLayout();
    void popupDoesNotAcceptFocus();
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

void
IconResourcesTest::showsTextForUnsupportedLayout()
{
    Widget widget(QStringLiteral("de"));
    const QLabel *label = widget.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label != nullptr);
    QCOMPARE(label->text(), QStringLiteral("DE"));
}

void
IconResourcesTest::popupDoesNotAcceptFocus()
{
    Widget widget(QStringLiteral("us"));
    QVERIFY(widget.testAttribute(Qt::WA_ShowWithoutActivating));
    QVERIFY(widget.testAttribute(Qt::WA_X11DoNotAcceptFocus));
    QVERIFY(widget.windowFlags().testFlag(Qt::WindowDoesNotAcceptFocus));
    QVERIFY(widget.windowFlags().testFlag(Qt::WindowTransparentForInput));
}

QTEST_MAIN(IconResourcesTest)
#include "IconResourcesTest.moc"
