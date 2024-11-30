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
green_threshold=(15, 80, -50, -14, -10, 30)      #绿色阈值
blue_threshold=(15, 80, 3, 50, -80, -40)       #蓝色阈值
red_threshold=(15, 80, 30, 60, -20, 50)        #红色阈值
prefix_elements = [0x43,0x4B]                           #帧头
suffix_elements = [0x59,0x46]                           #帧尾
#----------------------------------------------------------------------------------------------------------------------

import sensor
import image
import math
import sensor,pyb
from pyb import Pin, Timer

uart = pyb.UART(3, 115200, timeout_char = 1000)
data=[0]*10
center_same_cnt=0
last_5_center_x=[]
last_5_center_y=[]
last_5_radius=[]
send_center_x=0
send_center_y=0
send_radius=0
cnt=0
find_lines_roi=[0,0,320,200]
sensor.reset()
sensor.set_framesize(sensor.QVGA)
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
    #print(sendData)
    uart.write(sendData)
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

def average_of_recent_five(new_value, window):
    # 确保窗口大小为3
    if len(window) == 3:
        window.pop(0)  # 移除最旧的数值
    window.append(new_value)  # 添加新数值
    # 计算平均值
    return sum(window) / len(window) if window else None

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

def center_find_by_circle(img):
    global last_center_x,last_center_y,send_center_x,send_center_y,center_same_cnt,cnt
    #img.erode(1)
    #img.dilate(1)
    #img.gaussian(1)
    circles=img.find_circles(threshold=3000, r_min=40,r_max=42)
    circle_max_magnitude=0
    center_x=0
    center_y=0

    if circles:
        for circle in circles:
            if circle.magnitude()>circle_max_magnitude:
                circle_max_magnitude=circle.magnitude()
                center_x=circle.x()
                center_y=circle.y()
                center_circle=circle.circle()
            #img.draw_circle(circle.x(),circle.y(),circle.r())
        if cnt>5 and (abs(center_x-send_center_x)>0.25*img.width() or abs(center_y-send_center_y)>0.25*img.height()):
            center_same_cnt=0
        else:
            cnt+=1
            center_same_cnt+=1
        if center_same_cnt>=5:
            center_same_cnt=6
            send_center_x=int(average_of_recent_five(center_x,last_5_center_x))
            send_center_y=int(average_of_recent_five(center_y,last_5_center_y))
            send_radius=int(average_of_recent_five(int(center_circle[2]),last_5_radius))
            img.draw_cross(send_center_x,send_center_y,2,color=(255,0,0))
            img.draw_circle(send_center_x,send_center_y,send_radius,color=(255,0,0))
            img.draw_string(send_center_x,send_center_y,"r="+str(send_radius),color=(255,0,0))
            img.draw_string(send_center_x,send_center_y+20,"magnitude="+str(circle_max_magnitude),color=(255,0,0))
        print(center_same_cnt)
        last_center_x=center_x
        last_center_y=center_y


mode=1
while(True):
    if mode==0:
        while(mode==0): #定位
            img=sensor.snapshot().lens_corr(strength = 1.8, zoom = 1.0)
            center_find_by_circle(img)
            UartSendDate([send_center_x,send_center_y])
            UartReceiveDate()
    if mode==1:  #巡线
        while(mode==1):
            line_max_magnitude=0
            img=sensor.snapshot()
            #img.find_edges(image.EDGE_CANNY, threshold=(50, 80))
            edge=None
            lines=img.find_lines(roi=find_lines_roi,threshold=1000, theta_margin=15, rho_margin=10)
            for line in lines:
                if line.magnitude()>line_max_magnitude:
                    line_max_magnitude=line.magnitude()
                    # center_x=line.x()
                    # center_y=line.y()
                    edge=line.line()
            if edge:
                img.draw_line(edge,color=(255,0,0))
            # UartSendDate([lines)
            UartReceiveDate()
    if mode==3: #颜色
        while(mode==3):
            img=sensor.snapshot()
            green_blobs = img.find_blobs([green_threshold])
            blue_blobs = img.find_blobs([blue_threshold])
            red_blobs = img.find_blobs([red_threshold])
            green_max=0
            blue_max=0
            red_max=0
            green_center_x=0
            green_center_y=0
            blue_center_x=0
            blue_center_y=0
            red_center_x=0
            red_center_y=0
            for blob in green_blobs:
                if blob.area() > green_max:
                    green_max = blob.area()
                    green_center_x = blob.cx()
                    green_center_y = blob.cy()
                    img.draw_rectangle(blob.rect(),color=(0,255,0))
            for blob in blue_blobs:
                if blob.area() > blue_max:
                    blue_max = blob.area()
                    blue_center_x = blob.cx()
                    blue_center_y = blob.cy()
                    img.draw_rectangle(blob.rect(),color=(0,0,255))
            for blob in red_blobs:
                if blob.area() > red_max:
                    red_max = blob.area()
                    red_center_x = blob.cx()
                    red_center_y = blob.cy()
                    img.draw_rectangle(blob.rect(),color=(255,0,0))
            UartSendDate([green_center_x,green_center_y,blue_center_x,blue_center_y,red_center_x,red_center_y])
            UartReceiveDate()
            pyb.delay(100)
