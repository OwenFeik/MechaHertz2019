from rplidar import RPLidar
from math import sin,cos,radians,sqrt
import cv2

lidar=RPLidar('COM3')
data=lidar.iter_scans()

# import json # Use dummy data for no lidar
# with open('dummy2.json','r') as f:
#     data=json.load(f)

def make_point(bearing,distance):
    """Bearing is the bearing of the lidar reading in degrees, distance is the distance in mm.\nReturns a (x,y) point."""
    bearing=radians(bearing)

    x=int((sin(bearing)*distance)/4)+500
    y=int(-1*((cos(bearing)*distance)/4))+500
    
    return (x,y)

def in_range(point_a,point_b,dist):
    """Points a and b are two (x,y) coordinates, dist is the maximum allowable distance betwee them.\nReturns True or False based on pythagorean distance."""

    a=max(point_a[0],point_b[0])-min(point_a[0],point_b[0])
    b=max(point_a[1],point_b[1])-min(point_a[1],point_b[1])

    if sqrt(a**2+b**2)<dist:
        return True
    return False


colour=(0,255,0) # Draw colour
max_dist=20 # Max distance to consider 2 points of the same object

# Scan looks like this:
# [(quality, bearing, distance), (12, 345.25, 450)]

prev=(0,0)
for scan in data:
    base=cv2.imread('base.png')
    for reading in scan:

        point=make_point(reading[1],reading[2])

        cv2.circle(base,point,2,colour,2)
        
        if in_range(point,prev,max_dist):
            cv2.line(base,point,prev,colour,2)            

        prev=point


    cv2.imshow('scan',base)
    key=cv2.waitKey(30) & 0xFF
    if key==ord('q'):
        cv2.destroyAllWindows()
        break
