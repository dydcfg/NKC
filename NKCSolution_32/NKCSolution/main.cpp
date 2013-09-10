
#include <QtWidgets/QApplication>
#include <qtranslator.h>
#include <QFont>
#include <QFontDatabase>
#include <qtextcodec>
#include "nkcsolution.h"

#define _CHINESE_LANGUAGE_

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));


#ifdef _CHINESE_LANGUAGE_
    //translation
    QTranslator translator;
    translator.load(":/NKCSolution/nkcsolution_zh");
    app.installTranslator(&translator);
    int id = QFontDatabase::addApplicationFont(":/NKCSolution/Sun");
#else
    int id = QFontDatabase::addApplicationFont(":/NKCSolution/NTR");
#endif
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family,9);
    app.setFont(font);

    NKCSolution solution;
    solution.hide();

    return app.exec();
}
