/*
 * 白噪音生成器
 * 
 * 用法：./generate_white_noise <输出文件名> <通道数> <采样率> <时长>
 * 
 * 参数说明：
 * 1. 输出文件名：必须包含.wav后缀，例如：output.wav
 * 2. 通道数：整数
 *    - 1 = 单声道
 *    - 2 = 立体声
 *    - 更多通道数用于环绕声等
 * 3. 采样率：常用的采样率值（单位：Hz）
 * 4. 时长：生成音频的时长（单位：秒）
 *    - 必须是正整数
 * 
 * 示例：
 * ./generate_white_noise output.wav 2 44100 10  # 生成10秒的立体声白噪音，采样率44.1kHz
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sndfile.h>

int main(int argc, char *argv[]){

    // 0. 检查命令行参数及收割参数
    if (argc < 5){
        printf("用法：%s <输出文件名包括.wav> <通道数> <采样率> <时长>\n",argv[0]);
        return 1;
    }

    const char *outputFileName = argv[1];
    int channels = atoi(argv[2]);
    int sampleRate = atoi(argv[3]);
    int duration = atoi(argv[4]);
    
    // 初始化随机数生成器,这里用到time.h和stdlib.h
    srand(time(NULL));

    // 创建输出wav文件
    SF_INFO sf_info;
    sf_info.samplerate = sampleRate;
    sf_info.channels = channels;
    sf_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE *outfile = sf_open(outputFileName, SFM_WRITE, &sf_info);
    if (!outfile){
        printf("无法创建输出文件：%s\n",sf_strerror(NULL));
        return 1;
    }

    printf("开始生产白噪音文件：%s, 通道数：%d 通道, 采样率：%d Hz, 时常：%d 秒...\n",outputFileName, channels, sampleRate, duration);

    // 计算样本数量及分配buffer内存
    int numSamples = sampleRate * duration * channels;
    float *buffer = (float*)malloc(numSamples * sizeof(float));
    if (!buffer){
        printf("内存非配失败！\n");
        sf_close(outfile);
        return 1;
    }
    

    // 生成白噪音: 生成[-1,1]之间的随机数，一个一个放进buffer里
    for (int i=0; i<numSamples; i++){
        // 生成-1到1之间的随机数
        float random_value = ((float)rand()/(float)RAND_MAX) * 2.0f - 1.0f;
        buffer[i]  = random_value;
    }

    // 从buffer里读取数据并写入wav文件
    sf_count_t count = sf_write_float(outfile, buffer,numSamples);
    if (count != numSamples){
        printf("写入样本时出错！%s\n", sf_strerror(outfile));
    }
    else{
        printf("白噪音文件生成成功：%s\n", outputFileName);
    }

    // 释放内存
    free(buffer);
    sf_close(outfile);

    return 0;
}