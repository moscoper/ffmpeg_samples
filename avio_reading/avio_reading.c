//
//  main.c
//  avio_reading
//
//  Created by cuifei on 2017/3/20.
//  Copyright © 2017年 cuifei. All rights reserved.
//

#include <stdio.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavutil/file.h"


struct buffer_data{
    uint8_t *ptr;
    size_t size;
};
static int read_packet(void *opaque,uint8_t *buf, int buf_size){
    
    struct buffer_data *bd = (struct buffer_data *)opaque;
    buf_size = FFMIN(buf_size, bd->size);
    
    printf("ptr:%p size: %zu\n",bd->size,bd->size);
    memcpy(buf, bd->ptr, bd->size);
    bd->ptr += buf_size;
    bd->size -= buf_size;
    
    return buf_size;
}



int main(int argc, const char * argv[]) {
    // insert code here...
//    printf("%s",avutil_configuration());
    AVFormatContext *fmt_ctx = NULL;
    AVIOContext *avio_ctx = NULL;
    uint8_t *buffer = NULL, *avio_ctc_buffer = NULL;
    size_t buffer_size, avio_ctx_buffer_size = 4096;
    //视频路径
    char *input_filename = "/Users/cuifei/Downloads/win/小学期课程资料 - 基于FFmpeg+SDL的视频播放器的制作/代码/(3) FFmpeg视频解码器/ds.mov";
    struct buffer_data bd ={0};
    
    int ret = 0;
    
//    if (argc !=2) {
//        fprintf(stderr, "usage: %s input file\n API example program to show how to read from custom buffer accessed through AVIOContext.\n",argv[0]);
//        return 1;
//    }
//    
//    input_filename = argv[1];
    av_register_all();
    ret = av_file_map(input_filename, &buffer, &buffer_size, 0, NULL);
    
    if (ret < 0) {
        goto end;
    }
    
    bd.ptr = buffer;
    bd.size = buffer_size;
    
    if (!(fmt_ctx = avformat_alloc_context())) {
        ret= AVERROR(ENOMEM);
        goto end;
    }
   
    avio_ctc_buffer = av_malloc(avio_ctx_buffer_size);
    
    if (!avio_ctc_buffer) {
        ret= AVERROR(ENOMEM);
        goto end;
    }
    
    avio_ctx = avio_alloc_context(avio_ctc_buffer, avio_ctx_buffer_size, 0, &bd, &read_packet, NULL, NULL);
    if (!avio_ctx) {
        AVERROR(ENOMEM);
        goto end;
    }
    
    fmt_ctx->pb = avio_ctx;
    ret = avformat_open_input(&fmt_ctx, NULL, NULL, NULL);
    if (ret < 0) {
        fprintf(stderr, "could not open input\n");
        goto end;
    }
    
    ret = avformat_find_stream_info(fmt_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "could not find stream infomation\n");
        
        goto end;
    }
    
    av_dump_format(fmt_ctx, 0, input_filename, 0);
    
    
end:
    avformat_close_input(fmt_ctx);
    
    if (avio_ctx) {
        av_freep(&avio_ctx->buffer);
        av_freep(&avio_ctx);
        
    }
    
    av_file_unmap(buffer, buffer_size);
    
    if (ret < 0) {
        fprintf(stderr, "erro: occured: %s\n", av_err2str(ret));
        return 1;
    }
    return 0;
}


































