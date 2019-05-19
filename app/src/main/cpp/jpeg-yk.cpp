#include <jni.h>
#include <string>
#include "../include/jpeglib.h"
#include <malloc.h>
#include <android/bitmap.h>


void write_JPEG_file(uint8_t *data, int w, int h, jint q, const char *path) {
//    3.1、创建jpeg压缩对象
    jpeg_compress_struct jcs;
    //错误回调
    jpeg_error_mgr error;
    jcs.err = jpeg_std_error(&error);
    //创建压缩对象
    jpeg_create_compress(&jcs);
//    3.2、指定存储文件  write binary
    FILE *f = fopen(path, "wb");
    jpeg_stdio_dest(&jcs, f);
//    3.3、设置压缩参数
    jcs.image_width = w;
    jcs.image_height = h;
    //bgr
    jcs.input_components = 3;
    jcs.in_color_space = JCS_RGB;
    jpeg_set_defaults(&jcs);
    //开启哈夫曼功能
    jcs.optimize_coding = true;
    jpeg_set_quality(&jcs, q, 1);
//    3.4、开始压缩
    jpeg_start_compress(&jcs, 1);
//    3.5、循环写入每一行数据
    int row_stride = w * 3;//一行的字节数
    JSAMPROW row[1];
    while (jcs.next_scanline < jcs.image_height) {
        //取一行数据
        uint8_t *pixels = data + jcs.next_scanline * row_stride;
        row[0] = pixels;
        jpeg_write_scanlines(&jcs, row, 1);
    }
//    3.6、压缩完成
    jpeg_finish_compress(&jcs);
//    3.7、释放jpeg对象
    fclose(f);
    jpeg_destroy_compress(&jcs);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_yk_libjpeg_1sample_libjpeg_JpegUtils_native_1Compress__Landroid_graphics_Bitmap_2ILjava_lang_String_2(
        JNIEnv *env, jclass type, jobject bitmap, jint q, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);
    //从bitmap获取argb数据
    AndroidBitmapInfo info;//info=new 对象();
    //获取里面的信息
    AndroidBitmap_getInfo(env, bitmap, &info);//  void method(list)
    //得到图片中的像素信息
    uint8_t *pixels;//uint8_t char    java   byte     *pixels可以当byte[]
    AndroidBitmap_lockPixels(env, bitmap, (void **) &pixels);
    //jpeg argb中去掉他的a ===>rgb
    int w = info.width;
    int h = info.height;
    int color;
    //开一块内存用来存入rgb信息
    uint8_t *data = (uint8_t *) malloc(w * h * 3);//data中可以存放图片的所有内容
    uint8_t *temp = data;
    uint8_t r, g, b;//byte
    //循环取图片的每一个像素
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            color = *(int *) pixels;//0-3字节  color4 个字节  一个点
            //取出rgb
            r = (color >> 16) & 0xFF;//    #00rrggbb  16  0000rr   8  00rrgg
            g = (color >> 8) & 0xFF;
            b = color & 0xFF;
            //存放，以前的主流格式jpeg    bgr
            *data = b;
            *(data + 1) = g;
            *(data + 2) = r;
            data += 3;
            //指针跳过4个字节
            pixels += 4;
        }
    }
    //把得到的新的图片的信息存入一个新文件 中
    write_JPEG_file(temp, w, h, q, path);

    //释放内存
    free(temp);
    AndroidBitmap_unlockPixels(env, bitmap);
    env->ReleaseStringUTFChars(path_, path);
}