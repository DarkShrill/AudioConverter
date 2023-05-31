#include "audiofilesdecoder.h"

#include <QFile>
#include <QFileInfo>


extern "C"{
    #include "transcode_audio.h"
    #include "g711.h"
}


AudioFilesDecoder::AudioFilesDecoder() : QThread()
{
    thread  = new QThread();

    moveToThread(this->thread);

    this->thread->start(QThread::TimeCriticalPriority);
    this->start(QThread::TimeCriticalPriority);
}

void AudioFilesDecoder::onDecodeFiles(QString file_raw, QString sample_rate, bool also_wav_file)
{

    QStringList files = file_raw.split(",");

    if(!files.isEmpty()){

        for(uint32_t i = 0; i < files.count(); i++){

            QFile           file_tmp(files.at(i));
            QFileInfo       file_info(file_tmp);
            QString         file_name(file_info.fileName());
            QByteArray      audio_buffer;
            char            * in_file_name;
            char            * in_file_name_wav;
            QByteArray      temp_buff;
            QByteArray      temp_buff_wav;
            uint32_t        c1;
            QByteArray      *out_buff;
            unsigned long   out_buff_len;
            uint32_t        size_for_g711 = 0;
            int16_t         * buff_out;
            uint32_t        val = 0;
            QString         tmp_str;
            QString         path_no_name;



            temp_buff       = files.at(i).toLocal8Bit();

            in_file_name    = temp_buff.data();

            out_buff        = new QByteArray();
            out_buff->resize(90*1024*1024);




            tmp_str         = files.at(i);
            val             = tmp_str.lastIndexOf('/');
            path_no_name    = QString(tmp_str).remove(val, tmp_str.length() - val);
            file_name       = file_name.remove(file_name.lastIndexOf("."), file_name.length() - file_name.lastIndexOf("."));
            file_name       = file_name.replace(" ","_");


            temp_buff_wav   = (path_no_name + "/"+file_name+"_"+sample_rate+"_Hz.wav").toLocal8Bit();
            in_file_name_wav= temp_buff_wav.data();



            try{

                if(!also_wav_file)
                    convert_audio(in_file_name, sample_rate.toUInt() , (unsigned char *)out_buff->constData(), &out_buff_len);
                else
                    convert_audio_and_save_wav(in_file_name, in_file_name_wav, sample_rate.toUInt() , (unsigned char *)out_buff->constData(), &out_buff_len);

            } catch (...) {
                qDebug() << "Exception on: " << file_name;
            }


            size_for_g711 = (out_buff_len);
            audio_buffer.resize(size_for_g711); // per G711
            buff_out = (int16_t *)&out_buff[0];

            for(c1=0; c1 < size_for_g711; c1++){
                //audio_buffer[c1] = __af_linear2ulaw(buff_out[c1]); // lo converto in G711
                audio_buffer[c1] = out_buff->constData()[c1];
            }


            writeFile(path_no_name + "/"+file_name+"_"+sample_rate+"_Hz.pcm", (const uint8_t *)audio_buffer.constData(), size_for_g711);

            free(out_buff);
        }
    }
}


bool AudioFilesDecoder::writeFile(QString path, const uint8_t *src, uint32_t size)
{
    qDebug() << path;

    QFile file(path);
    if(file.open(QIODevice::ReadWrite)){
        file.write((const char *)src, size);
        file.close();
    }else{
        qDebug() << "open file failed: " << path << endl;
        return false;
    }

    return true;
}

AudioFilesDecoderConnector::AudioFilesDecoderConnector() : QObject()
{

}

void AudioFilesDecoderConnector::decode(QString files, QString sample_rate, bool also_wav_file)
{
    emit decodeFiles(files, sample_rate, also_wav_file);
}
