
#include "MainWindow.h"

#include <QApplication>
#include "ImageProcessing.h"

#include <QCommandLineParser>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("ImageDSP");
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Augmentations");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
        {
            {"n", "prog-name"},
            QApplication::translate("main", "program name."),
            QApplication::translate("main", "name")
        },
        {
            {"i", "in-img"},
            QApplication::translate("main", "input image [*.jpg, *.png, *.bmp...]."),
            QApplication::translate("main", "image")
        },
        {
            {"o", "out-img"},
            QApplication::translate("main", "Output image [*.jpg, *.png, *.bmp...]."),
            QApplication::translate("main", "image")
        },
        {
            {"p", "params"},
            QApplication::translate("main", "Parameters (; separated list)."),
            QApplication::translate("main", "params")
        },
    });

    parser.process(app);

    bool gui_mode = !(
        parser.isSet("n") ||
        parser.isSet("i") ||
        parser.isSet("o")
    );
    if(gui_mode){
        MainWindow* w = new MainWindow(QString("prjs/augmentations.imgdsp"), 0);
        w->showMaximized();
        return app.exec();
    }else{
        if(!parser.isSet("n")){
            qCritical() << "Program name not set!";
            return -1;
        }
        if(!parser.isSet("i")){
            qCritical() << "Input image not set!";
            return -1;
        }
        if(!parser.isSet("o")){
            qCritical() << "Output image not set!";
            return -1;
        }

        QImage in_img(parser.value("i"));
        in_img=in_img.convertToFormat(QImage::Format_RGB888);
        QImage out_img;
        QVector<double> params;
        QStringList pl = parser.value("p").split(';');
        for(const auto& s : pl){
            bool ok = false;
            double d = s.toDouble(&ok);
            if(!ok){
                qCritical() << "Bad param " << s << "!";
                return -1;
            }else{
                params.append(d);
            }
        }

        imageProcessingFun(
            parser.value("n"),
            out_img,
            in_img,
            params
        );

        out_img.save(parser.value("o"));
        qDebug() << "i w = " << in_img.width();
        qDebug() << "i h = " << in_img.height();
        qDebug() << "o w = " << out_img.width();
        qDebug() << "o h = " << out_img.height();

        return 0;
    }
}
