#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <cassert>
#include <filesystem>

#include <TFile.h>
#include <TTree.h>


constexpr void calibrate(std::array<float, 1024> &time, const std::array<float, 1024> &dt, const std::uint16_t tcell,
        std::array<float, 1024> &wf, const std::array<std::uint16_t, 1024> &wf_adc, const std::uint16_t rc) {
    for (std::uint16_t i = 0; i < 1024; ++i) {
        float tmp = 0;
        for (std::uint16_t j = 0; j < i; ++j) {
            tmp += dt.at((j + tcell) % 1024);
        }
        time.at(i) = tmp;

        wf.at(i) = static_cast<float>(wf_adc.at(i)) / 65535. + rc - 0.5;
    }
}


void convert_drs4(const std::string inputfile, const std::string& outputfile){
    assert(std::filesystem::exists(inputfile));
    std::ifstream ifs(inputfile, std::ios::in | std::ios::binary);

    auto f = new TFile(outputfile.c_str(), "recreate");
    auto t = new TTree("tree", "DRS4 waveform data");

    std::array<std::uint16_t, 9> buf_event1;
    std::array<std::uint16_t, 3> buf_event2;
    std::array<std::uint16_t, 2> buf_event3;

    std::uint16_t range_center;
    std::uint16_t trigger_cell;
    std::array<float, 1024> ch1_dt {};
    std::array<float, 1024> ch2_dt {};
    std::array<float, 1024> ch3_dt {};
    std::array<float, 1024> ch4_dt {};
    std::array<float, 1024> ch1_time {};
    std::array<float, 1024> ch2_time {};
    std::array<float, 1024> ch3_time {};
    std::array<float, 1024> ch4_time {};
    std::array<std::uint16_t, 1024> ch1_wf_adc {};
    std::array<std::uint16_t, 1024> ch2_wf_adc {};
    std::array<std::uint16_t, 1024> ch3_wf_adc {};
    std::array<std::uint16_t, 1024> ch4_wf_adc {};
    std::array<float, 1024> ch1_wf {};
    std::array<float, 1024> ch2_wf {};
    std::array<float, 1024> ch3_wf {};
    std::array<float, 1024> ch4_wf {};

    t->Branch("range_center", &range_center);
    t->Branch("trigger_cell", &trigger_cell);

    bool has_ch1 = false;
    bool has_ch2 = false;
    bool has_ch3 = false;
    bool has_ch4 = false;

    // start reading DRS4 binary data
    std::uint64_t size = 0;
    std::array<std::uint8_t, 4> header {};
    ifs.read(reinterpret_cast<char *>(&header), sizeof(header));

    // time header
    ifs.read(reinterpret_cast<char *>(&header), sizeof(header));
    ifs.read(reinterpret_cast<char *>(&header), sizeof(header));
    while (true) {
        ifs.read(reinterpret_cast<char *>(&header), sizeof(header));
        if (header.at(0) == 'C' && header.at(1) == '0' && header.at(2) == '0' && header.at(3) == '1') {
            has_ch1 = true;
            t->Branch("ch1_time", &ch1_time);
            t->Branch("ch1_wf", &ch1_wf);
            ifs.read(reinterpret_cast<char *>(&ch1_dt), sizeof(ch1_dt));
        } else if (header.at(0) == 'C' && header.at(1) == '0' && header.at(2) == '0' && header.at(3) == '2') {
            has_ch2 = true;
            t->Branch("ch2_time", &ch2_time);
            t->Branch("ch2_wf", &ch2_wf);
            ifs.read(reinterpret_cast<char *>(&ch2_dt), sizeof(ch2_dt));
        } else if (header.at(0) == 'C' && header.at(1) == '0' && header.at(2) == '0' && header.at(3) == '3') {
            has_ch3 = true;
            t->Branch("ch3_time", &ch3_time);
            t->Branch("ch3_wf", &ch3_wf);
            ifs.read(reinterpret_cast<char *>(&ch3_dt), sizeof(ch3_dt));
        } else if (header.at(0) == 'C' && header.at(1) == '0' && header.at(2) == '0' && header.at(3) == '4') {
            has_ch4 = true;
            t->Branch("ch4_time", &ch4_time);
            t->Branch("ch4_wf", &ch4_wf);
            ifs.read(reinterpret_cast<char *>(&ch4_dt), sizeof(ch4_dt));
        } else {
            ifs.seekg(-sizeof(header), std::ios::cur);
            break;
        }
    }

    // event header
    while (true) {
        if (ifs.read(reinterpret_cast<char *>(&header), sizeof(header))) {
            ifs.read(reinterpret_cast<char *>(&buf_event1), sizeof(buf_event1));
            ifs.read(reinterpret_cast<char *>(&range_center), sizeof(range_center));
            ifs.read(reinterpret_cast<char *>(&buf_event2), sizeof(buf_event2));
            ifs.read(reinterpret_cast<char *>(&trigger_cell), sizeof(trigger_cell));

            if (has_ch1) {
                ifs.read(reinterpret_cast<char *>(&header), sizeof(header));
                ifs.read(reinterpret_cast<char *>(&buf_event3), sizeof(buf_event3));
                ifs.read(reinterpret_cast<char *>(&ch1_wf_adc), sizeof(ch1_wf_adc));
                calibrate(ch1_time, ch1_dt, trigger_cell, ch1_wf, ch1_wf_adc, range_center);
            }

            if (has_ch2) {
                ifs.read(reinterpret_cast<char *>(&header), sizeof(header));
                ifs.read(reinterpret_cast<char *>(&buf_event3), sizeof(buf_event3));
                ifs.read(reinterpret_cast<char *>(&ch2_wf_adc), sizeof(ch2_wf_adc));
                calibrate(ch2_time, ch2_dt, trigger_cell, ch2_wf, ch2_wf_adc, range_center);
            }

            if (has_ch3) {
                ifs.read(reinterpret_cast<char *>(&header), sizeof(header));
                ifs.read(reinterpret_cast<char *>(&buf_event3), sizeof(buf_event3));
                ifs.read(reinterpret_cast<char *>(&ch3_wf_adc), sizeof(ch3_wf_adc));
                calibrate(ch3_time, ch3_dt, trigger_cell, ch3_wf, ch3_wf_adc, range_center);
            }

            if (has_ch4) {
                ifs.read(reinterpret_cast<char *>(&header), sizeof(header));
                ifs.read(reinterpret_cast<char *>(&buf_event3), sizeof(buf_event3));
                ifs.read(reinterpret_cast<char *>(&ch4_wf_adc), sizeof(ch4_wf_adc));
                calibrate(ch4_time, ch4_dt, trigger_cell, ch4_wf, ch4_wf_adc, range_center);
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


std::tuple<std::string, std::string> parse_arguments(int argc, char **argv) {
    std::vector<std::string> argvec(argv, argv + argc);

    std::string input = "";
    std::string output = "output.root";

    for (std::size_t i = 0; i < argvec.size(); ++i) {
        if (argvec.at(i) == "-i" || argvec.at(i) == "--input") {
            input = argvec.at(i + 1);
        } else if (argvec.at(i) == "-o" || argvec.at(i) == "--output") {
            output = argvec.at(i + 1);
        } else if (argvec.at(i) == "-h" || argvec.at(i) == "--help") {
            std::cout << "usage: drs2root [options]" << std::endl;
            std::cout << "options:" << std::endl;
            std::cout << "  -i, --input                 specify the input DRS4 binary file (required)" << std::endl;
            std::cout << "  -o, --output                specify the output root file" << std::endl;
            std::cout << "  -h, --help                  display the information" << std::endl;
            std::exit(0);
        }
    }
    return {input, output};
}


int main(int argc, char **argv){
    const auto [input, output] = parse_arguments(argc, argv);
    convert_drs4(input, output);
}
