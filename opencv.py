#
# This code is based on Kush's algorithm from last year.
# In essence, it uses a greyscale image generated by subtracting Blue from Red,
# combined with a HSV image, to find the ball with minimal calibration.
#

import cv2
from time import perf_counter

# To use PiCamera:
from picamera import PiCamera
from picamera.array import PiRGBArray

camera=PiCamera()
camera.resolution=(640,480)
camera.framerate=90
raw_capture=PiRGBArray(camera,size=(640,480))

# # To use video file:
# vs=cv2.VideoCapture('path/to/test.mp4')
# # To use web cam:
# vs=cv2.VideoCapture(0)


fps_readings=[]
frame_count=0
prev_time=0

for image in camera.capture_continuous(raw_capture,format="bgr",use_video_port=True): # When using PiCamera.
# while True: # When using VideoCapture.
    ball_detected=False

    # rgb=vs.read()[1] # Opencv VideoCapture
    # rgb=cv2.UMat(vs.read()[1]) # OpenCV VideoCapture with Hardware Acceleration
    rgb=image.array # PiCamera
    # rgb=cv2.Umat(image.array) # PiCamera with Hardware Acceleration
    hsv=cv2.cvtColor(rgb, cv2.COLOR_BGR2HSV) # Create HSV frame

    rgb_B,rgb_G,rgb_R=cv2.split(rgb) # Split RGB -> B, G, R
    hsv_H,hsv_S,hsv_V=cv2.split(hsv) # Split HSV -> H, S, V

    greyscale=cv2.subtract(rgb_R,rgb_B) # Subtract Blue channel from Red to create greyscale image

    s_mask=cv2.inRange(hsv_S,100,255) # Create a mask of pixels with saturation > 100
    h_mask=cv2.bitwise_or(cv2.inRange(hsv_H,0,4),cv2.inRange(hsv_H,175,180)) # Create a mask of pixels of 180>hue>175 or 4>hue>0
    hsv_mask=cv2.bitwise_and(s_mask,h_mask) # Unify these two masks

    max_val=cv2.minMaxLoc(greyscale,hsv_mask)[1] # Find highest intensity among pixels with S>100, hue in above range

    if max_val>=70: # If this pixels intensity is > 70
        ball_detected=True # We probably found a ball

        ball_mask=cv2.bitwise_and(cv2.inRange(greyscale,(max_val-45),255),hsv_mask) # Unify hsv_mask with pixels close to greyscales max
        ball_mask=cv2.erode(ball_mask,cv2.getStructuringElement(cv2.MORPH_RECT,(5,5))) # Rectangular erosion to remove noise

        moments=cv2.moments(ball_mask) # Moments are values that represent the sum of a function applied to all pixels
        ball_area=moments['m00'] # m00 is the sum of pixel intensities; in a binary image, this is the area
        if moments:
            if moments['m00']<=0: ball_center=int(moments['m10']),int(moments['m01']) # If m00 is 0, we don't want to divide by 0
            else: ball_center=int(moments['m10']/moments['m00']),int(moments['m01']/moments['m00']) # (m10/m00,m01/m00)=(x,y) of image's 'center of mass'

            # ball_center_hsv=hsv.get()[ball_center[1]][ball_center[0]] # When using hardware acceleration, need to .get() the Mat image
            ball_center_hsv=hsv[ball_center[1]][ball_center[0]] # When not using hardware acceleration, raw image is available
            if 175>ball_center_hsv[0]>5: # Make sure the hue of the center pixel isn't outside reasonable range
                ball_detected=False

            if ball_center==(0,0): ball_detected=False # If ball center=(0,0), no pixels are live, no ball is visible

    if frame_count%5==0 and frame_count>0: # Calculate fps every 5 frames
        time=perf_counter()
        fps=(5/(time-prev_time))
        prev_time=time
        if fps_readings:
            print(f'FPS: {fps} AVG: {sum(fps_readings)/len(fps_readings)}')
        fps_readings.append(fps)
    frame_count+=1

    if ball_detected:
        print(ball_center)
        # cv2.circle(rgb,ball_center,2,(0,255,0),2) # For debug, put a dot where we think ball is.
    else:
        print('not found')
        
    # raw_capture.truncate(0) # Discard used frame, necessary when using PiCamera

    # # For debug, show frame.
    # cv2.imshow('frame',rgb)
    # key=cv2.waitKey(1) & 0xFF
    # if key==ord('q'):
    #     cv2.destroyAllWindows()
    #     go=False
    #     raise SystemExit
