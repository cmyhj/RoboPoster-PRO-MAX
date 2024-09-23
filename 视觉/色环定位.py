'''
注意事项：

'''
#需要调试的参数---------------------------------------------------------------------------------------------------------
'''
sensor.QQVGA: 160x120
sensor.QVGA: 320x240
sensor.VGA: 640x480
'''
find_step=10                                            #寻找中心的步长，越小越精确，但是速度越慢
#----------------------------------------------------------------------------------------------------------------------
camera_picture_angle_halfwidth=50                       #相机视场角,半宽度参数
camera_picture_angle_far=81                             #相机视场角,距离参数
prefix_elements = [0x43,0x4B]                           #帧头
suffix_elements = [0x59,0x46]                           #帧尾
lens_corr_threshold=2.0
import sensor
import math
import sensor,pyb
from pyb import Pin, Timer

uart = pyb.UART(3, 115200, timeout_char = 1000)
thresholds = (box_threshold,cross_threshold)
data=[0]*10
sensor.reset()
sensor.set_framesize(sensor.VGA)
sensor.set_pixformat(sensor.RGB565)
#sensor.set_auto_gain(False)  # must be turned off for color tracking
#sensor.set_auto_whitebal(False)  # must be turned off for color tracking
#sensor.set_auto_blc(False)


########串口发送数据函数处理#########
def UartSendDate(data):
    # 在列表前面插入元素
    data[:0] = prefix_elements  # 切片赋值
    # 将元素添加到列表后面
    data.extend(suffix_elements)
    sendData=bytearray(data)
    uart.write(sendData)
    for Da in sendData:
        print(Da)
########串口发送数据函数处理完毕#############
########串口接收数据函数处理#########
def UartReceiveDate():  #这个函数不能运行太快，否则会导致串口读取太快导致出错
    global data
    global mode

    data[0] = uart.readchar()
    data[1] = uart.readchar()
    data[2] = uart.readchar()
    data[3] = uart.readchar()
    data[4] = uart.readchar()
    data[5] = uart.readchar()
    data[6] = uart.readchar()
    data[7] = uart.readchar()
    data[8] = uart.readchar()
    data[9] = uart.readchar()
    for x in range(6):
        if data[(x)%10] == 0x43 and data[(x+1)%10] == 0x4B and data[(x+3)%10] == 0x59 and data[(x+4)%10] == 0x46:
            mode =  data[(x+2)%10]-48
########串口接收数据函数处理完毕#############

def int_to_uint8_t_array_via_bit_ops(n):
    uint8_array = []
    for i in range(2):
           byte = (n >> (i * 8)) & 0xFF  # 提取第i个字节
           uint8_array.append(byte)
       
       # 返回结果数组
    return uint8_array

    return uint8_t_array
def radians(degrees):
    return math.pi * degrees / 180.0

def center_find_by_lmy(img):
    img.binary([(0,0,0),(255,255,255)], invert=True)
    img.erode(1)
    img.dilate(1)
    #垂直方向查找
    for x in range(img.width(),step=find_step):
        y=0
        if img.get_pixel(x,y)==255:
            temp_y=y
            while(img.get_pixel(x,y)==255 and y<img.height()-1):
                temp_y+=0.5
                y+=1
        center_y+=temp_y/2
        y+=1
    #水平方向查找
    for y in range(img.height(),step=find_step):
        x=0
        if img.get_pixel(x,y)==255:
            temp_x=x
            while(img.get_pixel(x,y)==255 and x<img.width()-1):
                temp_x+=0.5
                x+=1
        center_x+=temp_x/2
        x+=1
    img.draw_circle(center_x,center_y,5,color=(255,0,0))
    return center_x,center_y

def center_find_by_circle(img):
    img.erode(1)
    img.dilate(1)
    circles=img.find_circles(r_min=10)
    for circle in circles:
        center_x+=circle.x()
        center_y+=circle.y()
    center_x/=len(circles)
    center_y/=len(circles)
    return center_x,center_y

mode=0
while(True):
    if mode==0:
        while(mode==0):
            pyb.delay(100)
    if mode==3:
        while(mode==3):
            pyb.delay(100)
