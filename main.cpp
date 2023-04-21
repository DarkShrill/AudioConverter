#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTimer>

#include "audiofilesdecoder.h"


static AudioFilesDecoder                * decoder = nullptr;
static AudioFilesDecoderConnector       * decoder_connector = nullptr;

static QObject * Get_AudioFilesDecoder(QQmlEngine * engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if(decoder == nullptr){
        decoder = new AudioFilesDecoder();
    }

    if(decoder_connector == nullptr){
        decoder_connector = new AudioFilesDecoderConnector();
    }

    QObject::connect(decoder, &AudioFilesDecoder::started, [&](){

        QObject::connect(decoder, &AudioFilesDecoder::filesDecoded, decoder_connector, &AudioFilesDecoderConnector::filesDecoded, Qt::UniqueConnection);
        QObject::connect(decoder_connector, &AudioFilesDecoderConnector::decodeFiles, decoder, &AudioFilesDecoder::onDecodeFiles);
    });


    return decoder_connector;
}




int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);




    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<AudioFilesDecoderConnector>("it.videx.audio_decoder",1,0,"AudioFilesDecoder",Get_AudioFilesDecoder);

    qRegisterMetaType<QString>("QString");
    qRegisterMetaType<QStringList>("QStringList");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
