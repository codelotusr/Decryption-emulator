#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>

int main() {
    unsigned char regs[16];
    char prog_mem[256];
    bool zero_flag = false, eof_flag = false;
    long decr_size;
    auto *decr_file = fopen("decryptor.bin", "rb"), *encr_file = fopen("q1_encr.txt", "r"), *ats_file = fopen("solution.txt", "w");
    if (decr_file == NULL || encr_file == NULL || ats_file == NULL) {
        fputs("File error", stderr); 
        exit(1);
    }

    fseek(decr_file, 0 , SEEK_END);
    decr_size = ftell(decr_file);
    rewind(decr_file);

    while (!feof(decr_file)) {
        fread(prog_mem, sizeof(prog_mem), 1, decr_file);
    }
    fclose(decr_file);

    int i = 0;
    while(true) {
        char &rule = prog_mem[i % decr_size];
        char &byte = prog_mem[(i + 1) % decr_size];
        
        if (rule == 1) {
            zero_flag = false;
            char current_index = byte & 0x0F;
            ++regs[current_index];
            zero_flag = !regs[current_index];
        } else if (rule == 2) {
            zero_flag = false;
            char current_index = byte & 0x0F;
            --regs[current_index];
            zero_flag = !regs[current_index];
        } else if (rule == 3) {
            zero_flag = false;
            char rx_index = byte & 0x0F;
            char ry_index = (byte & 0xF0) >> 4;
            regs[rx_index] = regs[ry_index];
        } else if (rule == 4) {
            regs[0] = byte;
        } else if (rule == 5) {
            zero_flag = false;
            char current_index = byte & 0x0F;
            regs[current_index] = regs[current_index] << 1;
            zero_flag = !regs[current_index];
        } else if (rule == 6) {
            zero_flag = false;
            char current_index = byte & 0x0F;
            regs[current_index] = regs[current_index] >> 1;
            zero_flag = !regs[current_index];
        } else if (rule == 7) {
            i += (byte % decr_size) - 2;
        } else if (rule == 8) {
            if (zero_flag) {
                i += (byte % decr_size) - 2;
            }
        } else if (rule == 9) {
            if (!zero_flag) {
                i += (byte % decr_size) - 2;
            }
        } else if (rule == 10) {
            if (eof_flag) {
                i += (byte % decr_size) - 2;
            }
        } else if (rule == 11) {
            fclose(ats_file);
            fclose(encr_file);
            break;
        } else if (rule == 12) {
            zero_flag = false;
            char rx_index = byte & 0x0F;
            char ry_index = (byte & 0xF0) >> 4;
            regs[rx_index] += regs[ry_index];
            zero_flag = !regs[rx_index];
        } else if (rule == 13) {
            zero_flag = false;
            char rx_index = byte & 0x0F;
            char ry_index = (byte & 0xF0) >> 4;
            regs[rx_index] -= regs[ry_index];
            zero_flag = !regs[rx_index];
        } else if (rule == 14) {
            zero_flag = false;
            char rx_index = byte & 0x0F;
            char ry_index = (byte & 0xF0) >> 4;
            regs[rx_index] ^= regs[ry_index];
            zero_flag = !regs[rx_index];
        } else if (rule == 15) {
            zero_flag = false;
            char rx_index = byte & 0x0F;
            char ry_index = (byte & 0xF0) >> 4;
            regs[rx_index] |= regs[ry_index];
            zero_flag = !regs[rx_index];
        } else if (rule == 16) {
            char current_index = byte & 0x0F;
            regs[current_index] = fgetc(encr_file);
            eof_flag = feof(encr_file);
        } else if (rule == 17) {
            zero_flag = false;
            char current_index = byte & 0x0F;
            fprintf(ats_file, "%c", regs[current_index]);
        }
        i += 2;
    }

    return 0;
}
