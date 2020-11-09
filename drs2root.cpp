#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cassert>
#include <filesystem>

#include <TFile.h>
#include <TTree.h>


void convert_drs4(const std::string inputfile, const std::string& outputfile, bool has_ch1, bool has_ch2, bool has_ch3, bool has_ch4){
    assert(std::filesystem::exists(inputfile));
    std::ifstream ifs(inputfile, std::ios::in | std::ios::binary);

    auto f = new TFile(outputfile.c_str(), "recreate");
    auto t = new TTree("tree", "DRS4 waveform data");

    std::uint16_t buf_time[2048];
    std::uint16_t buf_event1[9];
    std::uint16_t buf_event2[3];
    std::uint16_t buf_ch[2];

    std::uint16_t range_center;
    std::uint16_t trigger_cell;
    std::uint16_t ch1_wf[1024];
    std::uint16_t ch2_wf[1024];
    std::uint16_t ch3_wf[1024];
    std::uint16_t ch4_wf[1024];

    t->Branch("range_center", &range_center);
    t->Branch("trigger_cell", &trigger_cell);
    if (has_ch1) t->Branch("ch1_wf", &ch1_wf, "ch1_wf[1024]/s");
    if (has_ch2) t->Branch("ch2_wf", &ch2_wf, "ch2_wf[1024]/s");
    if (has_ch3) t->Branch("ch3_wf", &ch3_wf, "ch3_wf[1024]/s");
    if (has_ch4) t->Branch("ch4_wf", &ch4_wf, "ch4_wf[1024]/s");

    std::uint64_t size = 0;
    std::uint8_t header[4];

    // start reading DRS4 binary data
    ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);

    // time header
    ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);
    ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);
    if (has_ch1) {
        ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);
        ifs.read(reinterpret_cast<char *>(&buf_time), sizeof(std::uint16_t) * 2048);
    }
    if (has_ch2) {
        ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);
        ifs.read(reinterpret_cast<char *>(&buf_time), sizeof(std::uint16_t) * 2048);
    }
    if (has_ch3) {
        ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);
        ifs.read(reinterpret_cast<char *>(&buf_time), sizeof(std::uint16_t) * 2048);
    }
    if (has_ch4) {
        ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);
        ifs.read(reinterpret_cast<char *>(&buf_time), sizeof(std::uint16_t) * 2048);
    }

    // event header
    while (true) {
        if (ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4)) {
            ifs.read(reinterpret_cast<char *>(&buf_event1), sizeof(std::uint16_t) * 9);
            ifs.read(reinterpret_cast<char *>(&range_center), sizeof(std::uint16_t));
            ifs.read(reinterpret_cast<char *>(&buf_event2), sizeof(std::uint16_t) * 3);
            ifs.read(reinterpret_cast<char *>(&trigger_cell), sizeof(std::uint16_t));

            if (has_ch1) {
                ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);
                ifs.read(reinterpret_cast<char *>(&buf_ch), sizeof(std::uint16_t) * 2);
                ifs.read(reinterpret_cast<char *>(&ch1_wf), sizeof(std::uint16_t) * 1024);
            }

            if (has_ch2) {
                ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);
                ifs.read(reinterpret_cast<char *>(&buf_ch), sizeof(std::uint16_t) * 2);
                ifs.read(reinterpret_cast<char *>(&ch2_wf), sizeof(std::uint16_t) * 1024);
            }

            if (has_ch3) {
                ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);
                ifs.read(reinterpret_cast<char *>(&buf_ch), sizeof(std::uint16_t) * 2);
                ifs.read(reinterpret_cast<char *>(&ch3_wf), sizeof(std::uint16_t) * 1024);
            }

            if (has_ch4) {
                ifs.read(reinterpret_cast<char *>(&header), sizeof(std::uint8_t) * 4);
                ifs.read(reinterpret_cast<char *>(&buf_ch), sizeof(std::uint16_t) * 2);
                ifs.read(reinterpret_cast<char *>(&ch4_wf), sizeof(std::uint16_t) * 1024);
            }
            ++size;
            t->Fill();
        } else {
            break;
        }
    }

    t->Write();
    f->Close();
    std::cout << size << " events written" << std::endl;
}

std::tuple<std::string, std::string, std::array<bool, 4>> parse_arguments(int argc, char **argv){
    std::vector<std::string> argvec(argv, argv + argc);

    std::string input = "";
    std::string output = "output.root";
    std::array<bool, 4> channel_activation = {};

    for (std::size_t i = 0; i < argvec.size(); ++i) {
        if (argvec.at(i) == "-i" || argvec.at(i) == "--input") {
            input = argvec.at(i + 1);
        } else if (argvec.at(i) == "-o" || argvec.at(i) == "--output") {
            output = argvec.at(i + 1);
        } else if (argvec.at(i) == "-c" || argvec.at(i) == "--channel-activation") {
            channel_activation = {std::stoi(argvec.at(i + 1)), std::stoi(argvec.at(i + 2)), std::stoi(argvec.at(i + 3)), std::stoi(argvec.at(i + 4))};
        } else if (argvec.at(i) == "-h" || argvec.at(i) == "--help") {
            std::cout << "usage: drs2root [options]" << std::endl;
            std::cout << "options:" << std::endl;
            std::cout << "  -i, --input                 specify the input DRS4 binary file (required)" << std::endl;
            std::cout << "  -o, --output                specify the output root file" << std::endl;
            std::cout << "  -c, --channel-activation    activate/deactivate each channel simultaneously" << std::endl;
            std::cout << "  -h, --help                  display the information" << std::endl;
            std::exit(0);
        }
    }

    return {input, output, channel_activation};
}

int main(int argc, char **argv){
    const auto [input, output, ch] = parse_arguments(argc, argv);

    // be careful to activate correct channels
    convert_drs4(input, output, ch.at(0), ch.at(1), ch.at(2), ch.at(3));
}
