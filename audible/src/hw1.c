#include <stdlib.h>

#include "debug.h"
#include "hw1.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the content of three frames of audio data and
 * two annotation fields have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

int validKeyset(char *pointer,char tag){
    int len = 0;
    char first = *pointer;
    if(tag == 'f'){
        while(first != '\0'){ /*iterate over string */
            if(first < '0' || first > '9'){
                return 0;
            }
            len++;
            first = *(pointer + len);
        }
        if(len == 1){
            first = *pointer;
            if(first == '0'){
                return 0;
            }
        }
        if(len > 4){
                return 0;
            }
        else if(len ==4){
            first = *pointer;
            if(first != '0' && first != '1'){
                return 0;
            }
            else{
                first = *(pointer+1);
                if(first != '0' ){
                 return 0;
                }
                else{
                    first = *(pointer+2);
                    if(first < '0' || first > '2' ){
                        return 0;
                    }
                    else{
                        first = *(pointer+3);
                        if(first < '0' || first > '4' ){
                            return 0;
                        }
                    }
                }
            }
        }
    return 1;
    }
    else if(tag == 'k'){
        while(first != '\0'){ /*iterate over string */
            if((first >= '0' && first <= '9') ||
                (first >= 'a' && first <= 'f') ||
                (first >= 'A' && first <= 'F')){
                len++;
                first = *(pointer + len);
            }
            else return 0;
        }
        if(len >8){
            return 0;
        }
        else return 1;
    }
    return 0;
}

int flag(char *pointer,int index){
    char first = *pointer;
    int len = 0;
    while(first != '\0'){ /*iterate over string */
        if(first != '-' && len == 0){
            return 0;
        }
        else{
            len ++;
            first = *(pointer + len);
        }
    }
    if(len > 2){
        return 0;
    }
    first = *(pointer + 1);
    if(first == 'h' && index == 1){
        return 1;
    }
    else if(first == 'u' && index == 1){
        return 2;
    }
    else if(first == 'd' && index == 1){
        return 3;
    }
    else if(first == 'c' && index == 1){
        return 4;
    }
    else if(first == 'f' && index != 1){
        return 5;
    }
     else if(first == 'k' && index != 1){
        return 6;
    }
     else if(first == 'p' && index != 1){
        return 7;
    }
    else return 0;
}

unsigned long  keyBuilder(char *pointer,char keyChoice){
    unsigned long construct = 0x0000000000000000;
    if(keyChoice == 'k'){
        int len = 0;
        char first = *pointer;
        while(first != '\0'){ /*iterate over string */
            len ++;
            first = *(pointer + len);
        }

        for(int i = 0; i < len; i++){
            first = *(pointer + i);
            if(first >= 'A' && first <= 'F'){
                unsigned long x = first - 55;
                x = x << (((len-1) - i) * 4);
                construct = construct | x;
            }
            else if(first >= 'a' && first <= 'f'){
                unsigned long x = first - 87;
                x = x << (((len-1) - i) * 4);
                construct = construct | x;
            }
            else{
                unsigned long x = first - 48;
                x = x << (((len-1) - i) * 4);
                construct = construct | x;
            }
        }
    }
    else{
        unsigned long character;
         int len = 0;
        char first = *pointer;
        while(first != '\0'){ /*iterate over string */
            len ++;
            first = *(pointer + (len-1));
        }
        unsigned long builder = 0;

        for(int i = 0; i < len-1; i++){
            first = *(pointer + i);
            int shift = 1;
            for(int j = (len -2)- i;j > 0; j--){
                shift = shift * 10;
            }
            character = first - 48;
            character = character * shift;
            builder = builder + character;
        }
        builder = builder - 1;
        builder = builder <<48;
        construct = construct | builder;
    }
    return construct;
}
/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 1 if validation succeeds and 0 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variables "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 1 if validation succeeds and 0 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */

int validargs(int argc, char **argv)
{
    int sizeArg = argc;
    if(sizeArg == 1){
        global_options  = 0x0;
        return 0;
    }
    else{
        char *ptr = *(argv + 1);
        int check = flag(ptr,1);
        if(check == 0){
            global_options  = 0x0;
            return 0;
        }
        else if(check == 1){ /* -h */
            global_options = 0x8000000000000000;
            return 1;
        }
        else if(check == 2 || check == 3){ /* -u || -d */
            if(check ==2){
                global_options = 0x4000000000000000;
            }
            else{
                 global_options = 0x2000000000000000;
            }
            if(sizeArg ==2){ /* just tag */
                return 1;
            }
            else if(sizeArg ==3){ /* tag is just -p */
                char *ptr = *(argv + 2);
                int second = flag(ptr,2);
                if(second == 7){
                    global_options = global_options |
                    0x0800000000000000;
                    return 1;
                }
               /* else return 0; */
            }
            else if(sizeArg ==4){ /*tag is just -f and flag */
                char *ptr = *(argv + 2);
                int second = flag(ptr,2);
                if(second == 5){
                   char *ptr = *(argv + 3);
                    if(validKeyset(ptr,'f')){
                        global_options = global_options |
                        keyBuilder(ptr,'f');
                        return 1;
                    }
                }
               /* else return 0; */
            }
            else if(sizeArg == 5){ /*tag is both */
                char *ptr = *(argv + 2);
                int second = flag(ptr,2);
                if(second == 7){/*-p first */
                global_options = global_options |
                0x0800000000000000;
                char  *ptr = *(argv + 3); /* -f ptr and key*/
                    char *key = *(argv + 4);
                    if(flag(ptr,3) == 5 &&  /*test valid */
                        validKeyset(key,'f')){
                        global_options = global_options |
                        keyBuilder(key,'f');
                        return 1;
                    }
                    /* else return 0; */
                }
                else if(second == 5){ /* -f first */
                    char *ptr = *(argv + 4); /* -p */
                    char *key = *(argv + 3); /*flag*/
                    if(flag(ptr,4) == 7 &&
                        validKeyset(key,'f')){
                        global_options = global_options |
                        0x0800000000000000;
                        global_options = global_options |
                        keyBuilder(key,'f');
                        return 1;
                    }
                    /* else return 0; */
                }
                /* else return 0; */
            }
            /* else return 0; */
        }
        else if(check == 4){
            global_options = 0x1000000000000000;
            if(sizeArg < 4){
                global_options = 0x0;
                return 0;
            }
            else if(sizeArg == 4){ /* -k and flag */
              char *ptr = *(argv + 2);
                int second = flag(ptr,2);
                if(second == 6){
                   char *ptr = *(argv + 3);
                    if(validKeyset(ptr,'k')){
                        global_options = global_options |
                        keyBuilder(ptr,'k');
                        return 1;
                    }
                }
               /* else return 0; */
            }
            else if(sizeArg == 5){ /*tag is both */
                char *ptr = *(argv + 2);
                int second = flag(ptr,2);
                if(second == 7){/*-p first */
                global_options = global_options |
                0x0800000000000000;
                char  *ptr = *(argv + 3); /* -k ptr and key*/
                    char *key = *(argv + 4);
                    if(flag(ptr,3) == 6 &&  /*test valid */
                        validKeyset(key,'k')){
                        global_options = global_options |
                        keyBuilder(key,'k');
                        return 1;
                    }
                    /* else return 0; */
                }
                else if(second == 6){ /* -k first */
                    char *ptr = *(argv + 4); /* -p */
                    char *key = *(argv + 3); /*flag*/
                    if(flag(ptr,4) == 7 &&
                        validKeyset(key,'k')){
                        global_options = global_options |
                        0x0800000000000000;
                        global_options = global_options |
                        keyBuilder(key,'k');
                        return 1;
                    }
                    /* else return 0; */
                }
                /* else return 0; */
            }
        }
        /* else return 0; */
    }
    global_options = 0x0;
    return 0;
}



int read_header(AUDIO_HEADER *hp){
    for(int j = 0;j < 6;j++){
        int c;
        int build = 0x00000000;
        for(int i = 0;i < 4; i++){
            c = getchar();
            c = c << ((3 - i) * 8);
            build = build | c;
        }
    if(j == 0){
        if(build == AUDIO_MAGIC)
        (*hp).magic_number = build;
        else return 0;
    }
    if(j == 1){
        if(build % 8 == 0)
        (*hp).data_offset = build;
        else return 0;
    }
    if(j == 2){
        (*hp).data_size = build;
    }
    if(j == 3){
        if(build == 2 || build == 3 || build == 4 || build == 5)
        (*hp).encoding = build;
        else return 0;
    }
    if(j == 4){
        (*hp).sample_rate = build;
    }
    if(j == 5){
        if(build == 1 || build == 2)
        (*hp).channels = build;
        else return 0;
    }
}
    if((*hp).data_offset-sizeof(AUDIO_HEADER) > ANNOTATION_MAX){
        return 0;
    }
     else return 1;
}

int write_header(AUDIO_HEADER *hp){
    for(int i = 1;i <= 4;i++){
         if(putchar(0xff & ((*hp).magic_number >> (8 * (4-i)))) == EOF){
            return 0;
         }
    }
    for(int i = 1;i <= 4;i++){
         if(putchar(0xff & ((*hp).data_offset >> (8 * (4-i)))) == EOF){
            return 0;
         }
    }
    for(int i = 1;i <= 4;i++){
         if(putchar(0xff & ((*hp).data_size >> (8 * (4-i)))) == EOF){
            return 0;
         }
    }
    for(int i = 1;i <= 4;i++){
         if(putchar(0xff & ((*hp).encoding >> (8 * (4-i)))) == EOF){
            return 0;
         }
    }
    for(int i = 1;i <= 4;i++){
         if(putchar(0xff & ((*hp).sample_rate >> (8 * (4-i)))) == EOF){
            return 0;
         }
    }
    for(int i = 1;i <= 4;i++){
         if(putchar(0xff & ((*hp).channels >> (8 * (4-i)))) == EOF){
            return 0;
         }
    }
    return 1;
}

int read_annotation(char *ap, unsigned int size){
    for(int i = 0;i < size;i++){
        char c = getchar();
        *(ap + i) = c;
    }
    if(size != 0){
        if((*(ap+(size-1)) != '\0')) return 0;
    }
    return 1;
}
int write_annotation(char *ap, unsigned int size){
    for(int i = 0; i < size;i++){
        if(putchar(*(ap+i))==EOF) return 0;
    }
    return 1;
}
int read_frame(int *fp, int channels, int bytes_per_sample){
    for(int j = 0;j<channels;j++){
        int negative = 0;
        signed int build = 0;
        for(int i = 0;i<bytes_per_sample;i++){
            signed int extracted = getchar();
            if((extracted & 0x80) == 0x80 && i ==0) negative = 1;
            if(extracted == EOF) return 0;
            build = (signed int)build | (extracted << ((bytes_per_sample-i-1) * 8));
        }
        if(bytes_per_sample == 1 && negative == 1) build = build | 0xffffff00;
        else if(bytes_per_sample == 2 && negative == 1) build = build | 0xffff0000;
        else if(bytes_per_sample == 3 && negative == 1) build = build | 0xff000000;
        *(fp + j) = build;

    }
    return 1;
}
int write_frame(int *fp, int channels, int bytes_per_sample){
    for(int j = 0;j<channels;j++){
        signed int build = 0;
        signed int extracted = (signed int)*(fp + j);
        for(int i = 0;i < bytes_per_sample;i++){
            build = ((signed int)extracted >> ((bytes_per_sample - i-1) * 8) & 0xFF);
            if(putchar(build)==EOF) return 0;
        }
    }

    return 1;
}

int frame_helper(int *fp,int *sfp,int*out,int factor,int channels,int bytes){
    for(int i = 1;i <= factor  + 1;i++){
        for(int j = 0;j < channels;j++){
            signed int first = (signed int)*(fp + j);
            signed int next = (signed int)*(sfp + j);
            signed int final = first + (next - first) * i  / ((factor+1));
            *(out + j) = (signed int)final;
        }
        if(!write_frame(out,channels,bytes)) return 0;
    }
    return 1;
}



/**
 * @brief  Recodes a Sun audio (.au) format audio stream, reading the stream
 * from standard input and writing the recoded stream to standard output.
 * @details  This function reads a sequence of bytes from the standard
 * input and interprets it as digital audio according to the Sun audio
 * (.au) format.  A selected transformation (determined by the global variable
 * "global_options") is applied to the audio stream and the transformed stream
 * is written to the standard output, again according to Sun audio format.
 *
 * @param  argv  Command-line arguments, for constructing modified annotation.
 * @return 1 if the recoding completed successfully, 0 otherwise.
 */
int recode(char **argv) {
    AUDIO_HEADER header;
    AUDIO_HEADER *pointer = &header;
    read_header(pointer);
    char *ap = input_annotation;
    unsigned int size = header.data_offset - sizeof(AUDIO_HEADER);
    read_annotation(ap,size);
    unsigned long first_bit = global_options & 0xf000000000000000;
    first_bit = first_bit >> 60;
    unsigned long p_bit = global_options & 0x0800000000000000;
    p_bit = p_bit >> 59;
    unsigned long factor = global_options & 0x03ff000000000000;
    factor = factor >> 48;
    unsigned long cript = global_options & 0x00000000ffffffff;
    char *out_point = output_annotation;
    int counter = 0;
    int total = 0;
    while(*(argv + counter) != NULL){
        int len = 0;
        char *string_point = *(argv + counter);
        counter++;
        char printer = *string_point;
        while(printer != '\0'){
            len++;
            total++;
            printer = *(string_point + len);
        }
        total++;
    }
    total++;
    unsigned int transformed_size = header.data_offset + total;
    while(transformed_size % 8 != 0){
        transformed_size++;
    }
    if(transformed_size - sizeof(AUDIO_HEADER) > ANNOTATION_MAX){
        return 0;
    }
    else{
        int offset = 0;
        for(int i = 0;i < counter;i++){
            int len = 0;
            char *string_point = *(argv + i);
            char printer = *string_point;
            while(printer != '\0'){
                len++;
                *(out_point + offset) = printer;
                offset++;
                printer = *(string_point + len);
            }
            if(i != counter - 1){
                *(out_point + offset) = ' ';
                offset++;
            }
            else{
                *(out_point + offset) = '\n';
                offset++;
            }
        }
        int old = 0;
        for(int i = offset; i < transformed_size - sizeof(AUDIO_HEADER);i++){
            *(out_point + i) = *(ap + old);
            old++;
        }
        if(p_bit == 0){
            header.data_offset = transformed_size;
        }
        unsigned int new_size = 0;
        int bytes = header.encoding - 1;
        if(first_bit==4){
            for(int i = 0;i < header.data_size/(header.channels * bytes);i++){
                if(i == 0 || i % (factor + 1) == 0){
                    new_size = new_size + (header.channels * bytes);
                }
            }
            int old_size = header.data_size;
            header.data_size = new_size;
            if(!write_header(pointer)) return 0;
            if(p_bit == 0){
                if(!write_annotation(out_point,transformed_size - sizeof(AUDIO_HEADER))) return 0;
            }
            else{
                if(!write_annotation(ap,size)) return 0;
            }
            char *framept = input_frame;
            for(int i = 0;i < old_size/(header.channels * bytes);i++){
                if(!read_frame((int *)framept,header.channels,bytes)) return 0;
                if(i == 0 || i % (factor + 1) == 0){
                    if(!write_frame((int *)framept,header.channels,bytes)) return 0;
                }
            }
        }
        else if(first_bit == 2){
            int data_points = header.data_size/(header.channels * bytes);
            int old_size = header.data_size;
            header.data_size = ((data_points * (factor+1)-factor) *(header.channels * bytes));
            if(!write_header(pointer)) return 0;
            if(p_bit == 0){
                if(!write_annotation(out_point,transformed_size - sizeof(AUDIO_HEADER))) return 0;
            }
            else{
                if(!write_annotation(ap,size)) return 0;
            }
            char *prevframe = previous_frame;
            char *framept = input_frame;
            char *frameout = output_frame;
            int  *prev = (int*)prevframe;
            int *frame = (int*)framept;
            int *out = (int*)frameout;
            if(!read_frame(prev,header.channels,bytes)) return 0;
            if(!write_frame(prev,header.channels,bytes)) return 0;
            for(int i = 1;i<(old_size/(header.channels * bytes));i++){
                if(!read_frame(frame,header.channels,bytes)) return 0;
                if(!frame_helper(prev,frame,out,factor,header.channels,bytes)) return 0;
                for(int j = 0;j < header.channels;j++){
                    *(prev + j) = *(frame + j);
                }
            }

        }
        else if(first_bit == 1){
            if(!write_header(pointer)) return 0;
            if(p_bit == 0){
                if(!write_annotation(out_point,transformed_size - sizeof(AUDIO_HEADER))) return 0;
            }
            else{
                if(!write_annotation(ap,size)) return 0;
            }
            mysrand(cript);
            char *framept = input_frame;
            char *frameout = output_frame;
            int *frame = (int*)framept;
            int *out = (int*)frameout;
            for(int i = 0;i < header.data_size/(header.channels * bytes);i++){
                if(!read_frame(frame,header.channels,bytes)) return 0;
                for(int j = 0;j < header.channels;j++){
                   signed int input =*(frame + j);
                    signed int encrypted = input ^  myrand32();
                    *(out + j) = encrypted;
                }
                if(!write_frame(out,header.channels,bytes)) return 0;
            }
        }

        return 1;
    }
}
