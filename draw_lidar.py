from rplidar import RPLidar
from math import sin,cos,radians,sqrt
import cv2

lidar=RPLidar('COM3')
data=lidar.iter_scans()

# import json # Use dummy data for no lidar
# with open('dummy2.json','r') as f:
#     data=json.load(f)

# Scan looks like this:
# [(quality, bearing, distance), (12, 345.25, 450)]

prev=(0,0)
for scan in data:
    base=cv2.imread('base.png')
    for point in scan:
        bearing=radians(point[1])
        distance=point[2]

        x=int((sin(bearing)*distance)/4)+500
        y=int(-1*((cos(bearing)*distance)/4))+500
        

        if radians(20)>bearing or bearing>radians(340):
            colour=(0,0,255)
        else:
            colour=(0,255,0)

        cv2.circle(base,(x,y),2,colour,2)

        a=max(prev[0],x)-min(prev[0],x)
        b=max(prev[1],y)-min(prev[1],y)

        if sqrt((a**2)+(b**2))<20:
            cv2.line(base,(x,y),prev,colour,2)            

        prev=(x,y)


    cv2.imshow('scan',base)
    key=cv2.waitKey(30) & 0xFF
    if key==ord('q'):
        cv2.destroyAllWindows()
        break
