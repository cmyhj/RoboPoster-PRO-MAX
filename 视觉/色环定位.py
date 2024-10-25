'''
注意事项：

'''
#需要调试的参数---------------------------------------------------------------------------------------------------------
'''
sensor.QQVGA: 160x120
sensor.QVGA: 320x240
sensor.VGA: 640x480
'''
find_step=1                                           #寻找中心的步长，越小越精确，但是速度越慢
#----------------------------------------------------------------------------------------------------------------------

import sensor
import image
import math
import sensor,pyb
from pyb import Pin, Timer

uart = pyb.UART(3, 115200, timeout_char = 1000)
data=[0]*10
sensor.reset()
sensor.set_framesize(sensor.QVGA)
sensor.set_pixformat(sensor.GRAYSCALE)
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
    img.find_edges(image.EDGE_CANNY, threshold=(50, 80))
#    img.erode(1)
#    img.dilate(1)
    center_y=0
    center_x=0
    sumx=0
    sumy=0
    count=0
    #垂直方向查找
    for x in range(0,img.width()):
        for y in range(0,img.height()):
            if img.get_pixel(x,y)==255:
                sumx+=x
                sumy+=y
                count+=1
    center_x=sumx/count
    center_y=sumy/count
    #水平方向查找
    img.draw_circle(int(center_x),int(center_y),5,color=125,thickness=10)
    return 0,0#center_x,center_y

def center_find_by_circle(img):
    #img.erode(1)
    #img.dilate(1)

    circles=img.find_circles(r_min=100)
    center_x=0
    center_y=0
    if circles:
        for circle in circles:
            center_x+=circle.x()
            center_y+=circle.y()
            #img.draw_circle(circle.x(),circle.y(),circle.r())
            print(circle.r())
        center_x/=len(circles)
        center_y/=len(circles)
        img.draw_cross(int(center_x),int(center_y),2,color=(255,0,0))
    return center_x,center_y

mode=3
while(True):
    if mode==0:
        while(mode==0):
            img=sensor.snapshot()
            center_find_by_circle(img)
            print("sadfsfda")
            pyb.delay(100)
    if mode==3:
        while(mode==3):
            img=sensor.snapshot()
            center_find_by_lmy(img)
            pyb.delay(100)
