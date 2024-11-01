#include "stdio.h"
#include "SynthesizerTrn.h"
#include "utils.h"
#include "string.h"
#include "portaudio.h" 
#include "Hanz2Piny.h"
#include "hanzi2phoneid.h"
#include <iostream>
#include <memory>  
#include <vector>  

#define FRAMES_PER_BUFFER   (512)
#define SAMPLERATE          (16000)
#define CHANNELS            (1)


using namespace std;

typedef struct
{
    float *wData;
    int32_t read;
    int32_t total;
}
paTestData;

static int audioCallback(const void *inputBuffer, void *outputBuffer,  
                         unsigned long framesPerBuffer,  
                         const PaStreamCallbackTimeInfo* timeInfo,  
                         PaStreamCallbackFlags statusFlags,  
                         void *userData) {  
    paTestData *audioData = (paTestData *)userData; 
    float *wDataP = (float *)audioData->wData; 
    float *out =  (float *)outputBuffer;  
    memset(out, 0, sizeof(float) * framesPerBuffer);

    // 将音频数据复制到输出缓冲区（这里假设是单声道）  
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        *out++ =  wDataP[audioData->read++]; 
        audioData->total--; 
    } 
    if ( audioData->total < framesPerBuffer) {
        return paComplete;
    }

      
    // 返回0表示成功，非0表示错误  
    return paContinue;  
}  

int main(int argc, char ** argv)
{
    const Hanz2Piny hanz2piny;
    const std::string line = argv[1];

    float * dataW = NULL;
    int32_t modelSize = ttsLoadModel(argv[2],&dataW);

    SynthesizerTrn * synthesizer = new SynthesizerTrn(dataW, modelSize);

    int32_t spkNum = synthesizer->getSpeakerNum();
    
    printf("Available speakers in the model are %d\n",spkNum);

    paTestData data;
    int32_t retLen = 0;
    int16_t * wavData = synthesizer->infer(line,0, 1.0,retLen);

    float *wavDataFloat = new float[retLen]; 
    for (int i = 0; i < retLen; i++) {  
        wavDataFloat[i] = (float) wavData[i] / 32767.0; 
    }
    data.wData =  wavDataFloat;
    data.read = 0;
    data.total = retLen;

    PaStream *stream;
    PaStreamParameters  inputParameters, outputParameters;
    PaError err = Pa_Initialize();

    if (err != paNoError) {  
        std::cerr << "Error opening PortAudio 3 stream: " << Pa_GetErrorText(err) << std::endl;  
        return 1;
    }

    /* Open PaStream with values read from the file */
    err = Pa_OpenDefaultStream(&stream
                             ,0                     /* no input */
                             ,CHANNELS              /* stereo out */
                             ,paFloat32             /* floating point */
                             ,SAMPLERATE
                             ,FRAMES_PER_BUFFER
                             ,audioCallback
                             ,&data);               /* our sndfile data struct */


    if (err != paNoError) {  
        std::cerr << "Error opening PortAudio 4 stream: " << Pa_GetErrorText(err) << std::endl;  
        return 1;
    }

    // 开始播放音频流  
    err = Pa_StartStream(stream);  
    if (err != paNoError) {  
        std::cerr << "Error starting PortAudio stream: " << Pa_GetErrorText(err) << std::endl;  
        Pa_CloseStream(stream);  
        return 1;  
    }  
  
    // 等待音频播放完成（在这个例子中，我们依赖于 paCallback 函数返回 paComplete）  
    // 在实际应用中，你可能需要一种更复杂的同步机制  
    while ((err = Pa_IsStreamActive(stream)) == 1) {  
        Pa_Sleep(100); // 休眠以避免忙等待  
    }

    if (err < 0) {  
        std::cerr << "Error checking PortAudio stream activity: " << Pa_GetErrorText(err) << std::endl;  
    }  
  
    // 关闭音频流并清理  
    err = Pa_CloseStream(stream);

    if(err != paNoError) {
        std::cerr << "Error close PortAudio stream: " << Pa_GetErrorText(err) << std::endl;  
    }

    err = Pa_Terminate();

    if(err != paNoError) {
        std::cerr << "Error terminat PortAudio: " << Pa_GetErrorText(err) << std::endl;  
    }
    
    //
    tts_free_data(wavData);
    tts_free_data(wavDataFloat);

    delete synthesizer;

    return 0;
}


