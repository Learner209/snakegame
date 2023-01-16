

import os
import fitz
import cv2
import numpy as np
import re
from PIL import Image
import os
from threading import Thread

file_dir = 'pdf'


def get_file_name(file_dir):
    '''
    获取指定目录下所有文件名称
    :param file_dir:指定目录
    :return:返回文件名列表
    '''
    for root, dirs, files in os.walk(file_dir):
        # return root#当前目录路径
        # return dirs#当前路径下所有子目录
        return files  # 当前路径下所有非目录子文件


def conver_img(file_dir, filename):
    '''
    把一个pdf文件中转换成图片,返回值是保存图片的地址
    '''
    # pdf_name = os.path.splitext(pdf)[0]
    print("Conver %s to Pictures"%filename)

    doc = fitz.open(os.path.join(file_dir, filename))
    label = hash(filename)
    pic_dir = '%s/%s'%(file_dir, label)
    if not os.path.exists(pic_dir):
        os.mkdir(pic_dir)
 
    for pg in range(doc.page_count):
        page = doc[pg]
        rotate = int(0)
        # 每个尺寸的缩放系数为2，这将为我们生成分辨率提高四倍的图像。
        zoom_x = 2.0
        zoom_y = 2.0
        trans = fitz.Matrix(zoom_x, zoom_y).prerotate(rotate)
        pm = page.get_pixmap(matrix=trans, alpha=False)
        # pm.writePNG('%s.png' % pg)
        pm._writeIMG('%s/%s.jpg'%(pic_dir, pg), format=1)
    return pic_dir



# 读取一张图片并保存
def read_img_output2save(path):
    
    formatter = '(\d+).jpg'
    res = re.search(formatter, path)
    if res is None:
        return
    else:
        res = res.group(1)
    img = cv2.imread(path, 1)  # 读取一张图片，彩色
    
    cha = img.shape
    height, width, deep = cha
    dst = np.zeros((height, width, 3), np.uint8)
    for i in range(height):  # 色彩反转
        for j in range(width):
            b, g, r = img[i, j]
            dst[i, j] = (255 - b, 255 - g, 255 - r)
    print("read_img is %s"%path)
    
    
    cv2.imwrite(path, dst)
    

def isPicPath(picPath):
    if not isinstance(picPath, str) or len(picPath) < 2:
        return
    rule = r'\.(jpg|jpeg|png|bmp|gif)$'
    res = None
    for hit in re.finditer(rule, picPath):
        res = hit.group(1)
    return res


def read_images_from_dir(dirpath = ''):
    if not isinstance(dirpath, str) or len(dirpath) <= 1:
        return
    print("开始目录: %s 反转图片颜色"%dirpath)
    ThreadList = list()
    for pic in os.listdir(dirpath):
        if not isPicPath(pic):
            return
        else:
            ThreadList.append(Thread(name="ReversePicture: %s"%(os.path.join(dirpath, pic)),\
                 target=read_img_output2save, args=[os.path.join(dirpath, pic)]))
            ThreadList[len(ThreadList)-1].start()
            #read_img_output2save(path=os.path.join(dirpath, pic))
    for thread in ThreadList:
        thread.join()
    print("目录: %s 反转图片颜色完成"%dirpath)





def combine2pdf(folderPath, pdfFilePath):
    # files = os.listdir(folderPath)

    pngFiles = []
    sources = []
    for file in range(len(list(filter(lambda element: element.endswith('.jpg'), os.listdir(folderPath))))):
        print(file)
        file = f'{file}.jpg'
        if 'jpg' in file:
            pngFiles.append(os.path.join(folderPath,file))
    # pngFiles.sort()  # 这里不要选择排序，这种排序是按头一个数字进行排序，会打乱原有顺序
    # print(pngFiles[0])
    output = Image.open(pngFiles[0])
    pngFiles.pop(0)
    for file in pngFiles:
        pngFile = Image.open(file)
        if pngFile.mode == "RGB":
            pngFile = pngFile.convert("RGB")
        sources.append(pngFile)
    output.save(pdfFilePath, "pdf", save_all=True, append_images=sources)


if __name__ == "__main__":
    target_dir = 'pdf'
    filename = "9.pdf"
    filenames = get_file_name(file_dir)
    print(filenames)

    pic_store_path = conver_img(file_dir = target_dir, filename = filename)
    read_images_from_dir(dirpath = pic_store_path)
    pdfFile = "jpg2pdf"
    combine2pdf(pic_store_path, 'destination/pdf')
