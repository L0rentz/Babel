/*
** EPITECH PROJECT, 2021
** Untitled (Workspace)
** File description:
** PortAudio
*/

#include "PortAudio.hpp"
#include <stdio.h>

/**
 * Creates an instance of PortAudio
*/
Babel::PortAudio::PortAudio()
{
    _compressor = new Opus();
    _error = Pa_Initialize();
    if (_error != paNoError) {
        throw PortAudioException(std::string("error initializing portaudio: ") + Pa_GetErrorText(_error));
    }
    _isInit = true;
    init();
    _numSamples = NUM_SECONDS * SAMPLE_RATE;
}

Babel::PortAudio::~PortAudio()
{
}

/**
 * Returns an instance of ICompressor 
*/
Babel::ICompressor &Babel::PortAudio::getCompressor()
{
    return *_compressor;
}

/**
 * Inits input and output devices from default devices
*/
void Babel::PortAudio::init()
{
    _inputParams.device = Pa_GetDefaultInputDevice();
    _inputParams.channelCount = NUM_CHANNELS;
    _inputParams.sampleFormat = PA_SAMPLE_TYPE;
    _inputParams.suggestedLatency = Pa_GetDeviceInfo(_inputParams.device)->defaultLowInputLatency;
    _inputParams.hostApiSpecificStreamInfo = NULL;

    _outputParams.device = Pa_GetDefaultOutputDevice();
    _outputParams.channelCount = NUM_CHANNELS;
    _outputParams.sampleFormat = PA_SAMPLE_TYPE;
    _outputParams.suggestedLatency = Pa_GetDeviceInfo(_outputParams.device)->defaultLowOutputLatency;
    _outputParams.hostApiSpecificStreamInfo = NULL;
}

/**
 * Terminates portaudio library
*/
void Babel::PortAudio::terminate()
{
    if (_isInit) {
        _error = Pa_Terminate();
        if (_error != paNoError) {
            throw PortAudioException(
                std::string("error terminating portaudio: ") + Pa_GetErrorText(_error)
            );
        }
    }
    _isInit = false;
}

/**
 * Opens an input stream
*/
void Babel::PortAudio::openInputStream()
{
    _error = Pa_OpenStream(&_inputStream, &_inputParams, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, recordCallback, &_buffer);
    if (_error != paNoError) {
        terminate();
        throw PortAudioException(std::string("Failed to open input stream") + Pa_GetErrorText(_error));
    }
}

/**
 * Opens an output stream
*/
void Babel::PortAudio::openOutputStream()
{
    _error = Pa_OpenStream(&_outputStream, NULL, &_outputParams, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, playCallback, &_buffer);
    if (_error != paNoError) {
        terminate();
        throw PortAudioException(std::string("Failed to open output stream") + Pa_GetErrorText(_error));
    }
}

/**
 * Starts the input stream
*/
void Babel::PortAudio::startInputStream()
{
    _error = Pa_StartStream(_inputStream);
    if (_error != paNoError) {
        terminate();
        throw PortAudioException(std::string("Failed to start input stream") + Pa_GetErrorText(_error));
    }
}

/**
 * Starts the output stream
*/
void Babel::PortAudio::startOutputStream()
{
    _error = Pa_StartStream(_outputStream);
    if (_error != paNoError) {
        terminate();
        throw PortAudioException(std::string("Failed to start output stream") + Pa_GetErrorText(_error));
    }
}

/**
 * Closes the input stream
*/
void Babel::PortAudio::closeInputStream()
{
    _error = Pa_CloseStream(_inputStream);
    if (_error != paNoError) {
        terminate();
        throw PortAudioException(std::string("Failed to close input stream") + Pa_GetErrorText(_error));
    }
}

/**
 * Closes the output stream
*/
void Babel::PortAudio::closeOutputStream()
{
    _error = Pa_CloseStream(_outputStream);
    if (_error != paNoError) {
        terminate();
        throw PortAudioException(std::string("Failed to close output stream") + Pa_GetErrorText(_error));
    }
}

/**
 * Records for 20 milliseconds
*/
void Babel::PortAudio::record()
{
    std::cout << "Recording" << std::endl;
    openInputStream();
    startInputStream();
    while (Pa_IsStreamActive(_inputStream) == 1) {
        // Pa_Sleep(1000);
        // printf("index = %d\n", _buffer.getFrameIndex() ); fflush(stdout);
    }
    closeInputStream();
    _buffer.setFrameIndex(0);
    // _audioData = _compressor->encodeFrame(_buffer);
    std::cout << "Finished recording" << std::endl;
}

/**
 * Plays recorded audio
*/
void Babel::PortAudio::play()
{
    std::cout << "Playing" << std::endl;
    // _buffer = _compressor->decodeFrame(_audioData);
    _buffer.setFrameIndex(0);
    openOutputStream();
    startOutputStream();
    while (Pa_IsStreamActive(_outputStream) == 1) {
        // Pa_Sleep(1000);
        // printf("index = %d\n", _buffer.getFrameIndex() ); fflush(stdout);
    }
    closeOutputStream();
    std::cout << "Finished playing" << std::endl;
}

/**
 * Returns raw audio buffer
 * Raw audio buffer is a Buffer class
*/
Babel::Buffer Babel::PortAudio::getBuffer() const
{
    return _buffer;
}

/**
 * Sets raw audio data to the value of @param buffer
*/
void Babel::PortAudio::setBuffer(Buffer buffer)
{
    _buffer = buffer;
}

/**
 * Returns compressed audio data
 * Compressed audio data is a CBuffer class
*/
Babel::CBuffer Babel::PortAudio::getAudioData() const
{
    return _audioData;
}

/**
 * Sets compressed data to the value of @param data
*/
void Babel::PortAudio::setAudioData(CBuffer data)
{
    _audioData = data;
}

int Babel::PortAudio::recordCallback(const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData)
{
    Buffer *data = (Buffer*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr = &data->data()[data->getFrameIndex() * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->getMaxFrameIndex() - data->getFrameIndex();
    (void)outputBuffer;
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;
    if (framesLeft < framesPerBuffer) {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }
    if (inputBuffer == NULL) {
        for (i = 0; i < framesToCalc; i++) {
            *wptr++ = SAMPLE_SILENCE;  /* left */
            if (NUM_CHANNELS == 2) *wptr++ = SAMPLE_SILENCE;  /* right */
        }
    }
    else {
        for (i = 0; i < framesToCalc; i++) {
            *wptr++ = *rptr++;  /* left */
            if (NUM_CHANNELS == 2) *wptr++ = *rptr++;  /* right */
        }
    }
    data->setFrameIndex(data->getFrameIndex() + framesToCalc);
    return finished;
}

int Babel::PortAudio::playCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
    Buffer *data = (Buffer*)userData;
    SAMPLE *rptr = &data->data()[data->getFrameIndex() * NUM_CHANNELS];
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->getMaxFrameIndex() - data->getFrameIndex();
    (void)inputBuffer; /* Prevent unused variable warnings. */
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;
    if (framesLeft < framesPerBuffer) {
        /* final buffer... */
        for (i = 0; i < framesLeft; i++) {
            *wptr++ = *rptr++;  /* left */
            if (NUM_CHANNELS == 2) *wptr++ = *rptr++;  /* right */
        }
        for (; i < framesPerBuffer; i++) {
            *wptr++ = 0;  /* left */
            if (NUM_CHANNELS == 2) *wptr++ = 0;  /* right */
        }
        data->setFrameIndex(data->getFrameIndex() + framesLeft);
        finished = paComplete;
    }
    else {
        for (i = 0; i < framesPerBuffer; i++) {
            *wptr++ = *rptr++;  /* left */
            if (NUM_CHANNELS == 2) *wptr++ = *rptr++;  /* right */
        }
        data->setFrameIndex(data->getFrameIndex() + framesPerBuffer);
        finished = paContinue;
    }
    return finished;
}