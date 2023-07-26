#include <iostream>

#include <fstream>

#include <string>

// #include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

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


typedef struct {

    uint8_t part_type[16]; // partition type guid
    uint8_t part_guid[16]; // unique partition guid
    uint64_t first_lba; // first LBA
    uint64_t last_lba; // last LBA
    uint8_t attr_flags[8]; // attribute flags
    uint8_t part_name[72]; // partition name, 72 bytes

}gptPartEntry; // 총 128byte

void printInfo(gptPartEntry* entry) {
    printf("partition type GUID: ");
    for (uint8_t hex : entry->part_type)
        printf("%02x ", hex);
    putchar('\n');

    printf("unique partition GUID: ");
    for (uint8_t hex : entry->part_guid)
        printf("%02x ", hex);
    putchar('\n');

    printf("first LBA: ");
    printf("%llx\n", entry->first_lba);

    printf("last LBA: ");
    printf("%llx\n", entry->last_lba);

    uint64_t fsize = (entry->last_lba - entry->first_lba) * 512;
    printf("file size: ");
    printf("%lld\n", fsize);

}

int main() {

    ifstream source;

    // gpt 이미지를 바이너리 모드로 open
    source.open("data/gpt_128.dd", ios::binary | ios::in);

    if (!source) {
        cout << "파일 열기 오류" << endl;
        getchar();
        return 0;
    }

    
    gptHeader header;

    /*
    // gpt header로 이동
    source.seekg(0x200);
    source.read((char*)&header, sizeof(header));

    for (int i = 0; i < 8; i++) {
        printf("%02x ", header.sign[i]);
    }
    putchar('\n');
    */

    // gpt partiton table entry 이동
    source.seekg(0x400);

    // 6개 partition table entry 저장
    gptPartEntry entry[128];
    source.read((char*)entry, sizeof(gptPartEntry) * 128);

    uint64_t real_offset;
    for (int i = 0; i < 128; i++) {

        // 할당되지 않은 파티션이 나오면 정지
        if (entry[i].part_type[0] == 0x00)
            break;

        printf("partition %d\n", i+1);
        puts("---------------------------");
        printInfo(&entry[i]);

        // 실제 디스크 위치 계산
        real_offset = entry[i].first_lba * 512;
        printf("real offset: %llx\n", real_offset);
        source.seekg(real_offset + 3);
        uint8_t fileSys[5];
        source.read((char*)fileSys, 4);
        fileSys[4] = '\0';
        printf("file system: %s\n", fileSys);
        puts("\n\n");
    }
    

    
    
    source.close();
    

    return 0;

}
