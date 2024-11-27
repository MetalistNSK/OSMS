#include <iostream>
#include <vector>
#include <bitset>
#include <ctime>
#include <cstdlib>

using namespace std;

const uint8_t CRC_POLY = 0b11011110; // x^7 + x^6 + x^4 + x^3 + x^2 + x^1

class DataBlock {
public:
    explicit DataBlock(const vector<uint8_t>& inputBits) : bits(inputBits) {}

    void appendCRC() {
        uint8_t crc = computeCRC(bits);
        for (int i = 7; i >= 0; --i) {
            bits.push_back((crc >> i) & 1);
        }
        cout << "CRC add: " << bitset<7>(crc) << endl;
    }

    bool validateCRC() const {
        return computeCRC(bits) == 0;
    }

    void flipBit(size_t index) {
        if (index < bits.size()) {
            bits[index] ^= 1;
        }
    }

    static vector<uint8_t> generateRandomBits(size_t length) {
        vector<uint8_t> randomBits(length);
        for (auto& bit : randomBits) {
            bit = rand() % 2;
        }
        return randomBits;
    }

private:
    vector<uint8_t> bits;

    uint8_t computeCRC(const vector<uint8_t>& data) const {
        uint32_t remainder = 0;
        for (uint8_t bit : data) {
            remainder = (remainder << 1) | bit;
            if (remainder & (1 << 7)) {
                remainder ^= CRC_POLY;
            }
        }
        return remainder;
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    vector<uint8_t> predefinedBits = {1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1};
    DataBlock block(predefinedBits);

    block.appendCRC(); // Вывод CRC
    block.validateCRC();

    DataBlock randomBlock(DataBlock::generateRandomBits(250));
    randomBlock.appendCRC(); // Вывод CRC

    int detectedErrors = 0, undetectedErrors = 0;
    size_t totalBits = 250 + 8; // Размер данных + CRC
    for (size_t i = 0; i < totalBits; ++i) {
        randomBlock.flipBit(i);
        if (!randomBlock.validateCRC()) {
            detectedErrors++;
        } else {
            undetectedErrors++;
        }
        randomBlock.flipBit(i); // Возврат к исходному состоянию
    }

    cout << "Detected errors: " << detectedErrors << endl;
    cout << "Missed errors: " << undetectedErrors << endl;

    return 0;
}
