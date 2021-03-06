/*
** EPITECH PROJECT, 2021
** B-CPP-500-NCE-5-1-babel-lionel.da-rocha-da-silva
** File description:
** Buffer
*/

#ifndef BUFFER_HPP_
#define BUFFER_HPP_

#include "BabelException.hpp"
#include <iostream>
#include <vector>
#include "BabelDefines.hpp"
#include <array>
#include <cstring>

namespace Babel {
    class Buffer {
        public:
            Buffer();
            ~Buffer();
            SAMPLE *data() const;
            int getFrameIndex() const;
            int getMaxFrameIndex() const;
            void setFrameIndex(int idx);
            void setMaxFrameIndex(int idx);
            void setBuffer(SAMPLE *buffer);
            void setSize(int size);
            int size() const;
        private:
            int numSamples;
            int _frameIndex;
            int _maxFrameIndex;
            SAMPLE *_sampleBuffer;
    };
}


#endif /* !BUFFER_HPP_ */
