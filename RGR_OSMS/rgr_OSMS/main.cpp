#include <iostream> 
#include <memory>
#include "io.hpp"
#include "BitSequence.h"
#include "SignalAnalysis.h"
#include "SignalProcessing.h"
#include "Utilities.h"
#include "Visualization.h"
#define N 10

#define SIGNAL_CORRELATION_REFERENCE_THRES 0.9
#define DATA_DIR "./data" 
#define SIGNAL_CHANNEL_LENGTH 30000
#define SIGNAL_BUFFER_SIZE 20000
#define SIGNAL_OVERSAMPLING 10
using namespace std;
using namespace io;
size_t read_referenceChannel(vector <double> ref,int repeatSamples);
vector <bool> generateSamples(int repeatSamples, vector <bool> payload);
vector<bool> encodeHamming(const vector<bool>& input);
vector<bool> decodeHamming(const vector<bool>& encoded);
// int additionalTask(std::vector<Visualization::GraphDescription>& descriptions);
void testHamming(vector <bool> payload);
void testHammingNoImprove(vector <bool> payload, int tmp1, int tmp2, int tmp3);
std::vector<bool> repeat_elements(const std::vector<bool>& input, int repeat_count) {
    std::vector<bool> result;
    result.reserve(input.size() * repeat_count);  // Резервируем память для результата

    for (const bool& element : input) {
        for (int i = 0; i < repeat_count; ++i) {
            result.push_back(element);
        }
    }

    return result;
}
std::vector<int> getInversionPositions(int sequenceLength) {
    std::vector<int> positions;
    std::string input;
    
    std::cout << "Введите номера позиций битов для инвертирования (через пробел): ";
    std::getline(std::cin, input);
    
    std::stringstream ss(input);
    int pos;
    while (ss >> pos) {
        if (pos >= 1 && pos <= sequenceLength) {
            positions.push_back(pos);  // Преобразуем к индексации с нуля
        } else {
            std::cout << "Позиция " << pos << " выходит за пределы последовательности. Пропускаем.\n";
        }
    }
    
    return positions;
}
int main(int argc, const char *argv[]) {
    std::vector<Visualization::GraphDescription> descriptions = {};
    //
    // vector <bool> test = {1,1,0,1,0,0,1,0,0,1,0,1,0,0,1,1,0,1,0,0,1,1,1,0,1,1};
    // vector <bool> encTest = encodeHamming(test);
    // vector <bool> testDecode;
    // cout << "Test input:" << endl;
    // for (bool val : test){
    //     cout << val;
    // }
    // cout << endl;
    // cout << "Test enecode:" << endl;
    // for (bool val : encTest){
    //     cout << val;
    // }
    // cout << endl;
    
    // cout << "Test decode:" << endl;
    // testDecode = decodeHamming(encTest);
    // for (bool val : testDecode){
    //     cout << val;
    // }
    // cout << endl;
    // имя-фамилия  
    string payload;
    cout << "Enter a firstname lastname: ";

    if (!getline(cin, payload)) {
        cerr << "Error" << "\n";
    }

    // signal to noise ratio
    float sigma = 0; 
    cout << "Enter a sigma range(0:1): ";

    if (!(cin >> sigma)) {
        cerr << "Error reading input sigma" << "\n";
    }

    if (sigma > 1 || sigma < 0) {
        cerr << "Error incorrect sigma" << "\n";
    }

    //int temp1,temp2, temp3;
    cout << "Sigma= " << sigma << endl << "Name= " << payload << endl; 
    // cout << "Enter 3 Position: ";
    // cin >> temp1;
    // cin >> temp2;
    // cin >> temp3;
    

    vector<bool> ascii_payload = ascii_encode_string(payload);
    cout << "Before Encode: "<<endl;
    for(bool val : ascii_payload){
        cout << val;
    }
    cout << endl;
    std::mt19937 generator(std::random_device{}());

    std::normal_distribution<double> distribution (0.0, sigma);

    vector<double> radioChannelNoise;
    vector<double> radioChannelReference;
    for (int i = 0; i < SIGNAL_CHANNEL_LENGTH; ++i) {
        double noise = distribution(generator); 
        radioChannelNoise.push_back(noise);
        radioChannelReference.push_back(0);
    }

    std::string simFrameNoise = "noise.txt";

    std::ofstream fileNoise(simFrameNoise);
     if (!fileNoise.is_open()) { 
        cerr << "Error" << "\n"; 
        return EXIT_SUCCESS; 
    }

    for (double val : radioChannelNoise) {
        fileNoise << val << endl; 
    }

    fileNoise.close();


    vector <bool> encodePayload = encodeHamming(ascii_payload);
    cout << "After Encode: "<<endl;
    for(bool val : encodePayload){
        cout << val;
    }
    cout << endl;

    vector <bool> repeated_frame = generateSamples(10, encodePayload);
    
    std::string simFrame1 = "buffer_samples.txt";

    std::ofstream fileFrameSamples(simFrame1);
     if (!fileFrameSamples.is_open()) { 
        cerr << "Error" << "\n"; 
        return EXIT_SUCCESS; 
    }

    for (bool val : repeated_frame) {
        fileFrameSamples << val; 
    }

    fileFrameSamples.close();

    
    std::string simNoiseFrame = "noise_and_frame.txt";
    std::ofstream fileNoiseFrame(simNoiseFrame);

     if (!fileNoiseFrame.is_open()) { 
        cerr << "Error" << "\n"; 
        return EXIT_SUCCESS; 
    } 
    std::string simReference = "reference.txt";
    std::ofstream fileReference(simReference);

     if (!fileReference.is_open()) { 
        cerr << "Error" << "\n"; 
        return EXIT_SUCCESS; 
    }   
    
    int startPos = 100;

    for(bool val : repeated_frame){
        if(val ){
        radioChannelNoise[startPos] = radioChannelNoise[startPos] + 1; 
        }
        else{
            radioChannelNoise[startPos] = radioChannelNoise[startPos] - 1; 
        }  
        if(val ){
        radioChannelReference[startPos] = radioChannelReference[startPos] + 1; 
        }
        else{
            radioChannelReference[startPos] = radioChannelReference[startPos] - 1; 
        }  
        startPos++;
    }

    for(double val : radioChannelNoise){
        fileNoiseFrame << val << endl;
    }
    for(double val : radioChannelReference){
        fileReference << val << endl;
    }
    fileNoiseFrame.close();
    fileReference.close();


    read_referenceChannel(radioChannelReference, 10);
    // additionalTask(descriptions);
    //testHammingNoImprove(ascii_payload, temp1, temp2, temp3);

    
    
    testHamming(ascii_payload);
    return EXIT_SUCCESS; 
}

bool window_slide_correlation(vector <bool> buff, std::vector<bool> gold_seq, double threshold) { 
        

        if (buff.size() >= gold_seq.size()) {
            std::vector<bool> recent_bits(buff.end() - gold_seq.size(), buff.end());
            

           long double corr = io::seq::nbitcorr(recent_bits, gold_seq);
            

            
             if (corr > threshold) { 
                return true;
             }
         }

        return false;
    }

size_t read_referenceChannel(vector <double> ref, int repeatSamples){
        std::vector<bool> buff;
        const uint8_t regXGold = 0b01011; 
        const uint8_t regYGold = 0b11011; 
    
        std::vector<bool> gold_seq = repeat_elements(io::seq::gold_generate(regXGold, regYGold), 10); 
        
        size_t corrIndex = 0;

        
        for (size_t scan = 0; scan < 500; ++scan) { 

            if (ref.at(scan) > 0) {
                buff.push_back(1); 
            } else { 
                buff.push_back(0);
            } 

            // cout << ref.at(scan);

            if (window_slide_correlation(buff, gold_seq, SIGNAL_CORRELATION_REFERENCE_THRES)) { // search correlation
                corrIndex = scan; 
                break;
            }
        }
        corrIndex +=1;
        
        buff.erase(buff.begin(),buff.begin()+corrIndex);
        vector<bool> tmpBuff;




        for(size_t scan = corrIndex; scan < SIGNAL_CHANNEL_LENGTH; scan+=repeatSamples){
            if (ref.at(scan) > 0) {
                tmpBuff.push_back(1); 
            } else { 
                tmpBuff.push_back(0);
            } 

            if (io::crc8_check(tmpBuff)) {
                bool hasTenOnes = true; 
                for (size_t i = 0; i < 5; ++i) {
                    size_t checkIndex = scan + 1 + i;
                    if (checkIndex >= SIGNAL_CHANNEL_LENGTH || ref[checkIndex] != 1) {
                        hasTenOnes = false; 
                        break; 
                    }
                }


                if (hasTenOnes) {
                    break;
                }
            }

        }
        if (tmpBuff.size() > 7) { 
            size_t elementsToCopy = tmpBuff.size() - 7; 

            buff.insert(buff.end(), tmpBuff.begin(), tmpBuff.begin() + elementsToCopy); 
        }
        
        // 1010101 1010101
        vector <bool> testEnc = decodeHamming(buff);
        // cout << "After Decode: "<<endl;
        // for(bool val : testEnc){
        //     cout << val;
        // }
        // cout << endl;

        std::string receivedString = io::ascii_decode_string(testEnc);

        std::cout << receivedString << std::endl; 
        
        
}
vector <bool> generateSamples(int repeatSamples, vector <bool> payload){
    const uint8_t regXGold = 0b01011; 
    const uint8_t regYGold = 0b11011; 

    // gold seq

    std::vector<bool> gold_seq = io::seq::gold_generate(regXGold, regYGold);
    // frame struct 
    // |---------PREAMBULA----------|-------PAYLOAD-------|-------CRC-------|
    // -------------31---------------
   
    vector <bool> frame;
    vector <bool> combained;
    vector <bool> tempCrc;
    frame.insert(frame.end(),payload.begin(),payload.end() );

    uint8_t crc3 = crc8_calc(frame);
    cout << "CRC: " << endl;
    for (int i = 6; i >= 0; --i) { 
         bool bit = (crc3 >> i) & 1;
         tempCrc.push_back(bit);
         frame.push_back(bit); 
     }
     for(bool val: tempCrc){
        cout << val;
     }
     cout << endl;
    combained.insert(combained.end(), gold_seq.begin(), gold_seq.end() );
    std::string simReference = "reference.txt";
    std::ofstream fileReference(simReference);

     if (!fileReference.is_open()) { 
        cerr << "Error" << "\n"; 
        exit(0);
    }   
    // for(bool val : gold_seq){
    //     cout << val;
    // }
    // cout << endl;
    combained.insert(combained.end(), frame.begin(), frame.end() );

    vector <bool> div = {1,1,1,1,1};
    
    combained.insert(combained.end(), div.begin(), div.end() );
    // cout << endl;
    // for(bool val : combained){
    //     cout << val;
    // }
    // cout << endl;
    return repeat_elements(combained, repeatSamples);
}
vector<bool> encodeHamming(const vector<bool>& input) {
    vector<bool> encodedMessage;

    for (size_t i = 0; i < input.size(); i += 4) {
        vector<bool> block(4, 0);
        for (size_t j = 0; j < 4; ++j) {
            if (i + j < input.size()) {
                block[j] = input[i + j];
            }
        }

        // Кодирование блока
        vector<bool> code(7);
        code[2] = block[0];
        code[4] = block[1];
        code[5] = block[2];
        code[6] = block[3];

        // Вычисление битов контроля
        code[0] = code[2] ^ code[4] ^ code[6];  // p1
        code[1] = code[2] ^ code[5] ^ code[6];  // p2
        code[3] = code[4] ^ code[5] ^ code[6];  // p3

        encodedMessage.insert(encodedMessage.end(), code.begin(), code.end());
    }

    return encodedMessage;
}

vector<bool> decodeHamming(const vector<bool>& encoded) {
    std::string findErrorImp = "find_errors_with_improve.txt";
    std::ofstream filefindErrorImp(findErrorImp);
    vector <int> errorPosImp;

    vector<bool> decodedMessage;
    size_t totalErrorCount = 0;  // Счетчик для абс. позиции

    for (size_t i = 0; i < encoded.size(); i += 7) {
        vector<bool> block(7);
        for (size_t j = 0; j < 7; ++j) {
            if (i + j < encoded.size()) {
                block[j] = encoded[i + j];
            }
        }

        // Проверка контр. битов
        int p1 = block[0] ^ block[2] ^ block[4] ^ block[6];  
        int p2 = block[1] ^ block[2] ^ block[5] ^ block[6];  
        int p3 = block[3] ^ block[4] ^ block[5] ^ block[6];  

        int errorPosition = p1 * 1 + p2 * 2 + p3 * 4;  // Позиция ошибки (1-7)
        
        if (errorPosition != 0) {
            totalErrorCount = i + (errorPosition - 1);  
            cout << "Error at absolute position: " << totalErrorCount << endl;
            errorPosImp.push_back(totalErrorCount);
            block[errorPosition - 1] = block[errorPosition - 1] ^ 1;  // Инвертируем неправильный бит
        }

        // Извлечение информационных битов:
         
            decodedMessage.push_back(block[2]);
            decodedMessage.push_back(block[4]);
            decodedMessage.push_back(block[5]);
            decodedMessage.push_back(block[6]);
        
    } 
    
    for(int val : errorPosImp){
        filefindErrorImp << val << endl;
    }
    filefindErrorImp.close();
    return decodedMessage;
}
// vector<bool> decodeHammingNoImprove(const vector<bool>& encoded) {
//     std::string findError = "find_errors_no_improve.txt";
//     std::ofstream filefindError(findError);

     
//     vector<bool> decodedMessage;
//     size_t totalErrorCount = 0;  // Счетчик для абс. позиции
//     vector <int> errorPos;
//     for (size_t i = 0; i < encoded.size(); i += 7) {
//         vector<bool> block(7);
//         for (size_t j = 0; j < 7; ++j) {
//             if (i + j < encoded.size()) {
//                 block[j] = encoded[i + j];
//             }
//         }

//         // Проверка контр. битов
//         int p1 = block[0] ^ block[2] ^ block[4] ^ block[6];  
//         int p2 = block[1] ^ block[2] ^ block[5] ^ block[6];  
//         int p3 = block[3] ^ block[4] ^ block[5] ^ block[6];  

//         int errorPosition = p1 * 1 + p2 * 2 + p3 * 4;  // Позиция ошибки (1-7)

//         if (errorPosition != 0) {
//             totalErrorCount = i + (errorPosition - 1);  
//             cout << "Error at absolute position(no improve): " << totalErrorCount << endl;
//             errorPos.push_back(totalErrorCount);
//            // block[errorPosition - 1] = block[errorPosition - 1] ^ 1;  // Инвертируем неправильный бит
//         }
//         // Извлечение информационных битов:
        
//             decodedMessage.push_back(block[2]);
//             decodedMessage.push_back(block[4]);
//             decodedMessage.push_back(block[5]);
//             decodedMessage.push_back(block[6]);
         
//     }

//     for(int val : errorPos){
//         filefindError << val << endl;
//     }
//     filefindError.close();
//     return decodedMessage;
// }
void testHamming(vector <bool> payload){
    cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
    cout << "                                          TEST HAMMING                                                           " << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
    cout << endl << "Изначальная последовательность до encode: " << endl;
    for(bool val : payload){
        cout << val;

    }
    cout << endl;
    vector<bool> hamming = encodeHamming(payload); 

    cout << endl << "Изначальная последовательность после encode: " << endl;
    for(bool val : hamming){
        cout << val;

    }
    cout << endl;

    int size = hamming.size();

    // Очистка буфера ввода
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Запрос позиций для инверсии битов
    vector<int> positions = getInversionPositions(size);
    for (int pos : positions) {
        if (pos >= 0 && pos < size) {
            hamming[pos] = !hamming[pos];  // Инвертируем бит
        } else {
            std::cout << "Позиция " << pos << " вне диапазона последовательности. Пропускаем.\n";
        }
    }

    cout << endl << "Последовательность после biterr: " << endl;
    for(bool val : hamming){
        cout << val;

    }
    cout << endl;
    
//    cout << endl << "Do biterror: " << endl;
//     for(bool val : hamming){
//         cout << val;
//     }

//     cout << endl;

//     int size = hamming.size();
//     for (int i = 0; i < size;i+=7){
//         hamming[i]= !hamming[i];
//     }

//      cout << endl << "Posle biterror: " << endl;
//     for(bool val : hamming){
//         cout << val;
//     }

//     cout << endl;


    vector<bool> dhemming = decodeHamming(hamming);
    cout << "Последовательность после decode и improve err: " << endl;
    for(bool val : dhemming){
        cout << val;
    }
    cout << endl;
}
// void testHammingNoImprove(vector <bool> payload, int tmp1, int tmp2, int tmp3){
//     // payload[tmp1] = payload[tmp1] ^ 1;
//     // payload[tmp2] = payload[tmp2] ^ 1;
//     // payload[tmp3] = payload[tmp3] ^ 1;
//     vector<bool> hamming = encodeHamming(payload); 
    
//     // hamming[tmp1] = hamming[tmp1] ^ 1;
//     // hamming[tmp2] = hamming[tmp2] ^ 1;
//     // hamming[tmp3] = hamming[tmp3] ^ 1;
//     cout << endl << "Do biterror: " << endl;
//     for(bool val : hamming){
//         cout << val;
//     }

//     cout << endl;

//     int size = hamming.size();
//     for (int i = 0; i < size;i+=3){
//         hamming[i]= !hamming[i];
//     }

//      cout << endl << "Posle biterror: " << endl;
//     for(bool val : hamming){
//         cout << val;
//     }

//     cout << endl;

//     vector<bool> dhamming = decodeHammingNoImprove(hamming);
//     cout << "Test Hamming No Improve after error: " << endl;
//     for(bool val : dhamming){
//         cout << val;
//     }
//     cout << endl;
// }

// int additionalTask(std::vector<Visualization::GraphDescription>& descriptions) {

//     BitSequence bitSequence = BitSequenceModule::encodeToASCII("Nikita Makedon");

//     BitSequence bitSequence_ = BitSequenceModule::encodeHamming(bitSequence);
   

//     constexpr size_t CRC_LENGTH = 16;
//     constexpr BitSequenceModule::CRCType CRC_TYPE =
//         BitSequenceModule::CRCType::CRC16;
//     u_int32_t crc =
//         BitSequenceModule::generateCRC(bitSequence, CRC_TYPE, CRC_LENGTH);
//     u_int32_t crc_ =
//         BitSequenceModule::generateCRC(bitSequence_, CRC_TYPE, CRC_LENGTH);

//     BitSequence crc_bit =
//         BitSequenceModule::numberToBitSequence(crc, CRC_LENGTH);
//     BitSequence crc_bit_ =
//         BitSequenceModule::numberToBitSequence(crc_, CRC_LENGTH);

//     BitSequence gold = BitSequenceModule::generateGoldSequence();

//     size_t messageLength = bitSequence.size();
//     size_t messageLength_ = bitSequence_.size();
//     // std::cout << messageLength << "\n";
//     if ((messageLength > (1 << 16)) && (messageLength > (1 << 16))) {
//         throw std::overflow_error("Размер сообщения превышает 2^16 бит.");
//     }

//     BitSequence len(16, 0);
//     for (int i = 0; i < 16; ++i) {
//         len[15 - i] = (messageLength >> i) & 1;
//     }

//     BitSequence len_(16, 0);
//     for (int i = 0; i < 16; ++i) {
//         len_[15 - i] = (messageLength_ >> i) & 1;
//     }

//     BitSequence combinedSeq = Utilities::combineBitSequences(
//         Utilities::combineBitSequences(gold, len),
//         Utilities::combineBitSequences(bitSequence, crc_bit));
//     Signal signal = Utilities::convertToSignal(combinedSeq);
//     signal = SignalProcessing::oversample(signal, N);
//     size_t Nx = signal.size();  // Размер сообщения

//     BitSequence combinedSeq_ = Utilities::combineBitSequences(
//         Utilities::combineBitSequences(gold, len_),
//         Utilities::combineBitSequences(bitSequence_, crc_bit_));
//     Signal signal_ = Utilities::convertToSignal(combinedSeq_);
//     signal_ = SignalProcessing::oversample(signal_, N);
//     size_t Nx_ = signal_.size();  // Размер сообщения

//     // Signal signalArray(Nx * 2, 0.0f);
//     // std::copy(signal.begin(), signal.end(),
//     //           signalArray.begin() + signal.size() / 2);

//     Signal signalArray = signal;
//     Signal signalArray_ = signal_;

//     double mu = 0.0;

//     std::vector<std::pair<int, double>> sigmaErrors;
//     std::vector<std::pair<int, double>> sigmaErrors_;
//     std::vector<std::pair<int, double>> sigmaErrors__;

//     size_t goldSize = gold.size() * N;
//     // size_t lenStart = goldSize;  // len начинается сразу за GOLD
//     size_t lenStart = 0;
//     size_t lenEnd = lenStart + 16 * N + goldSize;



//     for (double sigma = 0.00; sigma <= 0.30; sigma += 0.00005) {
//         Signal noise = SignalProcessing::generateWhiteNoise(Nx_, mu, sigma);

//         for (size_t i = 0; i < signalArray.size() && i < noise.size(); i++) {
//             if (i < lenStart || i >= lenEnd) {  // Пропускаем диапазон
//                 signalArray[i] += noise[i];
//             }
//         }

//         for (size_t i = 0; i < signalArray_.size() && i < noise.size(); i++) {
//             if (i < lenStart || i >= lenEnd) {  // Пропускаем диапазон
//                 signalArray_[i] += noise[i];
//             }
//         }

//         // Прием
//         BitSequence gold_oversampled = SignalProcessing::oversample(gold, N);
//         Signal received = signalArray;
//         // 0.5 это thrust factor для поиска корреляции
//         size_t start_index = SignalAnalysis::findSequenceInSignal(
//             received, gold_oversampled, 0.5);
//         Utilities::removeRange(received, 0, start_index);
//         BitSequence interpreted =
//             SignalAnalysis::interpretSymbols(received, N, sigma);
//         // SignalAnalysis::interpretSymbols(received, N, 0.5);
//         Utilities::removeRange(interpreted, 0, gold.size());
//         BitSequence receivedLen = Utilities::getRange(interpreted, 0, 16);
//         Utilities::removeRange(interpreted, 0, 16);
//         size_t messageLengthReceive =
//             SignalAnalysis::getMessageLength(receivedLen);

//         BitSequence receivedBitMessage =
//             Utilities::getRange(interpreted, 0, messageLengthReceive);


//         Signal received_ = signalArray_;
//         size_t start_index_ = SignalAnalysis::findSequenceInSignal(
//             received_, gold_oversampled, 0.6);
//         Utilities::removeRange(received_, 0, start_index_);
//         BitSequence interpreted_ =
//             // SignalAnalysis::interpretSymbols(received_, N, 0.5);
//             SignalAnalysis::interpretSymbols(received_, N, sigma);
//         Utilities::removeRange(interpreted_, 0, gold.size());
//         BitSequence receivedLen_ = Utilities::getRange(interpreted_, 0, 16);
//         Utilities::removeRange(interpreted_, 0, 16);
//         size_t messageLengthReceive_ =
//             SignalAnalysis::getMessageLength(receivedLen_);

//         BitSequence receivedBitMessage_ =
//             Utilities::getRange(interpreted_, 0, messageLengthReceive_);

//         // Подсчет числа ошибок
//         size_t bitErrors = 0;
//         for (size_t i = 0; i < receivedBitMessage.size(); ++i) {
//             if (i < bitSequence.size() &&
//                 receivedBitMessage[i] != bitSequence[i]) {
//                 ++bitErrors;
//             }
//         }
//         size_t bitErrors__ = 0;
//         for (size_t i = 0; i < receivedBitMessage_.size(); ++i) {
//             if (i < bitSequence.size() &&
//                 receivedBitMessage_[i] != bitSequence_[i]) {
//                 ++bitErrors__;
//             }
//         }

//         receivedBitMessage_ =
//             BitSequenceModule::decodeHamming(receivedBitMessage_);
//         // BitSequenceModule::decodeHammingWithParity(receivedBitMessage_);
//         size_t bitErrors_ = 0;
//         for (size_t i = 0; i < receivedBitMessage_.size(); ++i) {
//             if (i < bitSequence.size() &&
//                 receivedBitMessage_[i] != bitSequence[i]) {
//                 ++bitErrors_;
//             }
//         }

//         sigmaErrors.emplace_back(bitErrors, sigma);
//         sigmaErrors_.emplace_back(bitErrors_, sigma);
//         sigmaErrors__.emplace_back(bitErrors__, sigma);

//         // std::cout << "Sigma: " << sigma << " | Bit errors: " << bitErrors
//         //           << "\n";
//     }

//     Visualization::saveGraphData("./data/(1,3,1)extra_1.txt", sigmaErrors);
//     descriptions.push_back({"./data/(1,3,1)extra_1.txt", "plot", "x,y",
//                             "Message len: " + std::to_string(messageLength),
//                             "sigma", "bit err", true});
//     Visualization::saveGraphData("./data/(1,3,2)extra_2.txt", sigmaErrors_);
//     descriptions.push_back(
//         {"./data/(1,3,2)extra_2.txt", "plot", "x,y",
//          "Hamming. Message len: " + std::to_string(messageLength), "sigma",
//          "bit err", true});
//     Visualization::saveGraphData("./data/(1,3,3)extra_3.txt", sigmaErrors__);
//     descriptions.push_back(
//         {"./data/(1,3,3)extra_3.txt", "plot", "x,y",
//          "Hamming (origin). Message len: " + std::to_string(messageLength),
//          "sigma", "bit err", true});

//     return 0;
// }
