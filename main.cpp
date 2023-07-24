#include "pch.h"

typedef struct {

    uint8_t sign[8]; // signiture
    uint8_t revision[4]; // revision
    uint8_t h_size[4]; // header size
    uint8_t crc[4]; // crc32 of header
    uint8_t rev[4]; // reserved
    uint8_t cur_lba[8]; // current LBA
    uint8_t bkup_lba[8]; // backup LBA
    uint8_t start_part_lba[8]; // start partition LBA
    uint8_t end_part_lba[8]; // ending partition LBA
    uint8_t disk_guid[16]; // disk guid

}gptHeader;

int main() {

    ifstream source;

    // gpt 이미지를 바이너리 모드로 open
    source.open("data/gpt_128.dd", ios::binary | ios::in);

    if (!source) {
        cout << "파일 열기 오류" << endl;
        getchar();
        return 0;
    }

    // 16 byte씩 읽어들임
    uint8_t buf[16] = { 0, };

    gptHeader header;

    // gpt header로 이동
    source.seekg(0x200);
    source.read((char*)&header, sizeof(header));

    for (int i = 0; i < 8; i++) {
        printf("%02x ", header.sign[i]);
    }


    
    /*
    for (int i = 0; i < 16; i++) {
        printf("%02x ", buf[i]);
    }
    cout << endl;
    */
    source.close();
    

    return 0;

}
