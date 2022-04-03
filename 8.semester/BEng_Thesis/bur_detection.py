#This module takes care of bur-shaped blob detection

import os
from img_processing import *
from blob_cropping import *


def detect_burs(img):
    '''sets the parameters for the blob detection and returns kepoints of the found blobs'''
    close_kernel = np.ones((10,10),np.uint8)
    img = cv2.morphologyEx(img, cv2.MORPH_CLOSE, close_kernel)
    
    # Set the parameters
    params = cv2.SimpleBlobDetector_Params()
    #params.minThreshold = 40
    #params.maxThreshold = 100
    #params.thresholdStep = 5
    params.filterByArea = True
    params.minArea = 3000
    params.maxArea = 1000000
    params.filterByCircularity = True
    params.minCircularity = 0.01
    params.maxCircularity = 0.35
    params.filterByConvexity= True
    params.minConvexity = 0.35
    params.maxConvexity = 0.9999
    params.filterByInertia = True
    params.minInertiaRatio = 0.0005
    params.maxInertiaRatio = 0.3
    params.minDistBetweenBlobs = 0
    params.filterByColor = False

    # Create a detector with the parameters
    detector = cv2.SimpleBlobDetector_create(params)
    keypoints = detector.detect(img)
    
    im_with_keypoints = cv2.drawKeypoints(img, keypoints, np.array([]), (150,200,0),cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    #cv2.imshow("Keypoints", cv2.resize(inverse(im_with_keypoints), (150, int(150*proportion))))
    #cv2.waitKey(1000)
    #cv2.destroyAllWindows()
    
    pts = np.array([keypoints[idx].pt for idx in range(len(keypoints))]).reshape(-1, 1, 2)
    
    return keypoints, pts, im_with_keypoints


def detectBox(point, corners):
    '''determines whether the detected blob center lies inside of any contour'''
    p_x, p_y = np.around(point).astype(int)
    
    inside = []
    for c,i in zip(corners, range(len(corners))):
        side1 = np.array([c[0], c[1]])
        side2 = np.array([c[1], c[2]])
        side3 = np.array([c[2], c[3]])
        side4 = np.array([c[3], c[0]])
        sides = np.array([side1, side2, side3, side4])
        
        D_sign = 0
        outsideFlag = False
        
        for side in sides:
            x_0, y_0, x_1, y_1 = np.ravel(side)
            D = (p_x-x_0)*(y_1-y_0) - (p_y-y_0)*(x_1-x_0)
            #print(np.sign(D))
            if abs(np.sign(D)-D_sign)>1:
                outsideFlag = True
                break
            D_sign = np.sign(D)
        
        if outsideFlag==False:
            inside.append(i)
    
    detected = [corners[i] for i in inside]
    corners = [corners[i] for i in range(len(corners)) if i not in inside]
            
    return detected, corners


def refine(input_path, refined_dir, markup_dir):
    '''marks up, refines and saves the input image'''
    input_filename = os.path.splitext(os.path.basename(input_path))[0]
    output_dir = os.path.join(refined_dir, '/'.join(input_path.split('/')[1:-1]))
    markup_output_dir = os.path.join(markup_dir, '/'.join(input_path.split('/')[1:-1]))
        
    cropped = cv2.imread(input_path)
    marked_up = cropped.copy()
    gray_image = cv2.cvtColor(cropped, cv2.COLOR_BGR2GRAY)
    averaged = averaging(gray_image, 6)
    blurred = cv2.medianBlur(averaged, 21)
    
    close_kernel = np.ones((5,5),np.uint8)
    closed = cv2.morphologyEx(blurred, cv2.MORPH_CLOSE, close_kernel)
    dilate_kernel = np.ones((7,7),np.uint8)
    dilated = inverse(cv2.dilate(inverse(closed),dilate_kernel,iterations = 1))
    blobs = cv2.adaptiveThreshold(dilated,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,299,17)        
    
    _, pts, _ = detect_burs(blobs)
    pts = pts[:,0,:]
    marked, corners = drawBoxes(marked_up, blobs, minPixels=1000, maxFraction=0.99, minFraction=0.075)
    
    if len(pts)>0 and len(corners)>0:
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
            os.makedirs(markup_output_dir)
    
    for kp in range(len(pts)):
        cv2.circle(marked,(int(pts[kp][0]), int(pts[kp][1])), 15, (0,0,200), -1)
        
    print("Writing to " + markup_output_dir + "/" + input_filename + ".jpg")
    cv2.imwrite(os.path.join(markup_output_dir, input_filename + ".jpg"), marked)
        
    overlayed, x_offset, y_offset = overlayCanvas(cropped)   
    
    for kp in range(len(pts)):
        detected, corners = detectBox(pts[kp], corners)
        namePrefix = input_filename+"_"+str(kp)
        for d in detected:
            cropBoxes(overlayed, x_offset, y_offset, [d], output_dir, namePrefix, True)
    
    return True


def refineCropped(result_dir, refined_dir, markup_dir):
    '''wrapper function for refining the cropped images and measuring the refining time'''
    for root, subdirs, files in os.walk(result_dir):
        start = time.time()
        print("Refining '" + root +"' ...")
        for f in files:
            refine(os.path.join(root, f), refined_dir, markup_dir)
        end = time.time()
        print("Refining time for {}: {:.2f} sec".format(root, end-start))
            
    return True