#include "io.hpp"

using namespace std;
namespace io { 
    const uint8_t POLYNOM = 0b10001001; // x^7 + x^3 + 1
    const uint8_t CRC7MASK = 0b01111111; 

    uint8_t crc8_calc(const std::vector<bool>& inputData) {
        uint8_t crc = 0;

        for (bool bit : inputData) {
            uint8_t topBit = (crc & 0x40) >> 6; 
            crc = ((crc << 1) & CRC7MASK) | (bit ? 1 : 0);
            if (topBit) {
                crc ^= POLYNOM;
            }
        }

        for (int i = 0; i < 7; ++i) {
            uint8_t topBit = (crc & 0x40) >> 6;
            crc = (crc << 1) & CRC7MASK;
            if (topBit) {
                crc ^= POLYNOM;
            }
        }

        return crc;
    }

     bool crc8_check(const std::vector<bool>& inputWithCrc) {
        if (inputWithCrc.size() == 1) { 
            return false;
        }

        uint8_t resultCrc = crc8_calc(inputWithCrc);
        return resultCrc == 0;
    }
    int BitsToVal(vector<bool> bits) {
            int val = 0;
            int size = bits.size();

            for (int i = 0; i < size; ++i) {
                if (bits[size - 1 - i]) {
                    val += (1 << i);
                }
            }

            return val;
        }

        vector<double> BitsToSignal(vector<bool> bits, int bitLevel = 2, double maxPower = 1.0) {
            vector<double> signal;

            double step = 2 * maxPower / ((1 << bitLevel) - 1);

            for (size_t i = 0; i < bits.size(); i += bitLevel) {
                vector<bool> subBits;
                for (size_t j = i; j < i + bitLevel && j < bits.size(); ++j) {
                    subBits.push_back(bits[j]);
                }

                int bitVal = BitsToVal(subBits);
                double signalValue = -maxPower + bitVal * step;
                signal.push_back(signalValue);
            }

            return signal;
        }

        // vector<bool> SignalToBits(vector<double> samples, int samplesPerBit = 1, int bitLevel = 2, double maxPower = 1.0, double P) {
        //     vector<bool> bits;

        //     double step = 2 * maxPower / ((1 << bitLevel) - 1);
        //     double sum = 0.0;
        //     int cnt = 0;

        //     for (size_t i = 0; i < samples.size(); ++i) {
        //         sum += samples[i];
        //         ++cnt;

        //         if (cnt == samplesPerBit) {
        //             double avg = sum / cnt;
        //             double idealValue = round((avg + maxPower) / step);
        //             double actualValue = (avg + maxPower) / step;

        //             if (std::abs(actualValue - idealValue) > P) {
        //                 idealValue = std::max(0.0, std::min((1 << bitLevel) - 1.0, idealValue));
        //             }

        //             int value = static_cast<int>(idealValue);

        //             for (int j = bitLevel - 1; j >= 0; --j) {
        //                 bits.push_back((value >> j) & 1);
        //             }

        //             sum = 0.0;
        //             cnt = 0;
        //         }
        //     }

        //     return bits;
        // }
}
