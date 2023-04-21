#ifndef AUDIOFILESDECODER_H
#define AUDIOFILESDECODER_H

#include <QObject>
#include <QStringList>
#include <QThread>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>


class AudioFilesDecoderConnector : public QObject
{

    Q_OBJECT
    Q_PROPERTY(NOTIFY filesDecoded)

public:

    AudioFilesDecoderConnector();

    Q_INVOKABLE void decode(QString files, QString sample_rate, bool also_wav_file);


signals:
    // QML TO CPP
    void decodeFiles(QString files, QString sample_rate, bool also_wav_file);
    // CPP TO QML
    void filesDecoded();
};



class AudioFilesDecoder : public QThread
{
    Q_OBJECT

public:

    AudioFilesDecoder();

signals:

    void filesDecoded();

public slots:

    void onDecodeFiles(QString file_raw, QString sample_rate, bool also_wav_file);
private:

    bool writeFile(QString path, const uint8_t *src, uint32_t size);



    QThread                 * thread = nullptr;


};

#endif // AUDIOFILESDECODER_H
