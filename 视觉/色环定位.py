'''
注意事项：

'''
#需要调试的参数---------------------------------------------------------------------------------------------------------
'''
sensor.QQVGA: 160x120
sensor.QVGA: 160x120
sensor.VGA: 640x480
'''

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
blob_x=0
blob_y=0
blob_color=0
cnt=0
mode=0
circle_radius=50
circle_threshold=3000
zoomratio=18 # =像素/毫米
radius_the=2
lens_corr_strength=1.3
find_lines_roi=[0,0,137,200]
sensor.reset()
sensor.set_framesize(sensor.QVGA)
sensor.set_pixformat(sensor.RGB565)
#sensor.set_auto_gain(False)  # must be turned off for color tracking
#sensor.set_auto_whitebal(False)  # must be turned off for color tracking
#sensor.set_auto_blc(False)


########串口发送数据函数处理#########
def UartSendDate(data):
    print(data)
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
    #print(data)
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
    #print(mode)

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
    global last_center_x,last_center_y,send_center_x,send_center_y,center_same_cnt,cnt,circle_threshold,circle_radius
    #img.erode(1)
    #img.dilate(1)
    #img.gaussian(1)
    circles=img.find_circles(threshold=circle_threshold, r_min=circle_radius, r_max=circle_radius+radius_the)
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
#        if cnt>5:# and (abs(center_x-send_center_x)>0.25*img.width() or abs(center_y-send_center_y)>0.25*img.height()):
#            center_same_cnt-=1
#        else:
#            cnt+=1
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
            img.draw_string(send_center_x,send_center_y+40,"x="+str((send_center_x-137)/zoomratio)+",y="+str((122-send_center_y)/zoomratio),color=(255,0,0))
        #print(center_same_cnt)
    else:
        send_center_x=100
        send_center_y=20
        send_radius=0
    last_center_x=center_x
    last_center_y=center_y


def distance_from_center(x1,y1):
    return abs(x1-137)+abs(y1-122)
def find_color(img):
    global blob_x,blob_y,blob_color
    green_blobs = img.find_blobs([green_threshold])
    blue_blobs = img.find_blobs([blue_threshold])
    red_blobs = img.find_blobs([red_threshold])
    green_max=0
    blue_max=0
    red_max=0
    green_center_x=1000
    green_center_y=1000
    blue_center_x=1000
    blue_center_y=1000
    red_center_x=1000
    red_center_y=000
    blob_x=0
    blob_y=0
    blob_color=0
    for blob in green_blobs:
        if blob.area() > 3000 and blob.area() > green_max:
            green_max = blob.area()
            green_center_x = blob.cx()-137
            green_center_y = 122-blob.cy()
            img.draw_rectangle(blob.rect(),color=(0,255,0))
    for blob in blue_blobs:
        if blob.area() > 3000 and blob.area() > blue_max:
            blue_max = blob.area()
            blue_center_x = blob.cx()-137
            blue_center_y = 122-blob.cy()
            img.draw_rectangle(blob.rect(),color=(0,0,255))
            # img.draw_string(blob.cx(),blob.cy(),str(blue_center_x)+","+str(blue_center_y),color=(0,0,255))
    for blob in red_blobs:
        if blob.area() > 3000 and blob.area() > red_max:
            red_max = blob.area()
            red_center_x = blob.cx()-137
            red_center_y = 122-blob.cy()
            img.draw_rectangle(blob.rect(),color=(255,0,0))
            # img.draw_string(blob.cx(),blob.cy(),str(red_center_x)+","+str(red_center_y),color=(255,0,0))
    green_distance=abs(green_center_x)+abs(green_center_y)
    blue_distance=abs(blue_center_x)+abs(blue_center_y)
    red_distance=abs(red_center_x)+abs(red_center_y)
    if red_max>3000 and red_distance<green_distance and red_distance<blue_distance:
        blob_x=red_center_x
        blob_y=red_center_y
        blob_color=1
        img.draw_string(blob_x,blob_y,str(blob_x)+","+str(blob_y)+",area="+str(red_max),color=(255,0,0))
    elif green_max>3000 and green_distance<blue_distance and green_distance<red_distance:
        blob_x=green_center_x
        blob_y=green_center_y
        blob_color=2
        img.draw_string(blob_x,blob_y,str(blob_x)+","+str(blob_y)+",area="+str(green_max),color=(0,255,0))

    elif blue_max>3000 and blue_distance<green_distance and blue_distance<red_distance:
        blob_x=blue_center_x
        blob_y=blue_center_y
        blob_color=3
        img.draw_string(blob_x,blob_y,str(blob_x)+","+str(blob_y)+",area="+str(blue_max),color=(0,0,255))
    else:
        blob_x=0
        blob_y=0
        blob_color=0
def mode_change():
    global lens_corr_strength
    global zoomratio
    global circle_radius
    global radius_the
    global mode
    global circle_threshold
    global blob_color
    if blob_color==2: #green
        circle_threshold=1500
    else:
        circle_threshold=2500
    if mode==0:#高
        zoomratio=18
        lens_corr_strength=1.3
        circle_radius=50
        radius_the=2
    elif mode==1:#低
        zoomratio=34
        lens_corr_strength=1.3
        circle_radius=90
        radius_the=5
    print(mode)
mode=0
while(True):

    # if mode==0:
    #     while(mode==0): #定位
    img=sensor.snapshot().lens_corr(strength = lens_corr_strength, zoom = 1.0)
    center_find_by_circle(img)
    find_color(img)
    UartSendDate([send_center_x-120,120-send_center_y,blob_x,blob_y,blob_color,zoomratio])
    UartReceiveDate()
    mode_change()
#    if mode==1:  #巡线
#        while(mode==1):
#            line_max_magnitude=0
#            img=sensor.snapshot()
#            #img.find_edges(image.EDGE_CANNY, threshold=(50, 80))
#            edge=None
#            lines=img.find_lines(roi=find_lines_roi,threshold=1000, theta_margin=15, rho_margin=10)
#            for line in lines:
#                if line.magnitude()>line_max_magnitude:
#                    line_max_magnitude=line.magnitude()
#                    # center_x=line.x()
#                    # center_y=line.y()
#                    edge=line.line()
#            if edge:
#                img.draw_line(edge,color=(255,0,0))
#            # UartSendDate([lines)
#            UartReceiveDate()
    # if mode==2: #颜色
    #     while(mode==2):

    #         UartSendDate([green_center_x,green_center_y,blue_center_x,blue_center_y,red_center_x,red_center_y])
    #         UartReceiveDate()
    #         #pyb.delay(100)
