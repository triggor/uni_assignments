# This module takes care of initial image cropping

import os
from img_processing import *

ErrorCNT = 0

def is_rectangle(box):
    return len(np.unique(box[:,0]))==2 and len(np.unique(box[:,1]))==2


def box_corners(box):
    '''returns 4 corners for each contour box'''
    if is_rectangle(box):
        top_left = [np.min(box[:,0]), np.min(box[:,1])]
        top_right = [np.min(box[:,0]), np.max(box[:,1])]
        bottom_left = [np.max(box[:,0]), np.min(box[:,1])]
        bottom_right = [np.max(box[:,0]), np.max(box[:,1])]
        box = np.array([top_left, bottom_left, bottom_right, top_right])
    else:
        ys = box[:,1]
        xs = box[:,0]
        try:
            top_ix = np.where(ys == np.min(ys))[0][0]
            bottom_ix = np.where(ys == np.max(ys))[0][0]
            left_ix = np.where(xs == np.min(xs))[0][0]
            right_ix = np.where(xs == np.max(xs))[0][0]
            
            top = box[top_ix]
            bottom = box[bottom_ix]
            left = box[left_ix]
            right = box[right_ix]
            assert len(np.unique(np.array([top_ix,bottom_ix,left_ix,right_ix])))==4
            
        except AssertionError:
            print('AssertionError!!!', np.array([top_ix,bottom_ix,left_ix,right_ix]), np.array([top,left,bottom,right]))

        box = np.array([top,left,bottom,right])
    return box


def findBoxes(img, minPixels, maxFraction, minFraction):
    '''returns contours for each parametrized blob in an image'''
    h, w = img.shape[:2]
    img_size = h*w
    #img = dilate(img)
    _, contours, hierarchy = cv2.findContours(inverse(img),cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
        
    boxes = []
    pixel_condition = np.array(list(map(cv2.contourArea, contours)))>minPixels
    smaller_condition = np.array(list(map(cv2.contourArea, contours)))<maxFraction*img_size
    bigger_condition = np.array(list(map(cv2.contourArea, contours)))>minFraction*img_size
    #print(pixel_condition  & smaller_condition & bigger_condition)
    
    if len(contours)>1:
        contours = np.extract(pixel_condition  & smaller_condition & bigger_condition, contours)
        for cnt in contours:
            try:
                rect = cv2.minAreaRect(cnt)
                box = cv2.boxPoints(rect)
                box = np.int0(box)
                boxes.append(box)
            except TypeError:
                global ErrorCNT
                ErrorCNT = ErrorCNT+1
                print("TypeError!!!", ErrorCNT)
    elif len(contours)==1 and (pixel_condition  & smaller_condition & bigger_condition):
        cnt = contours[0]
        rect = cv2.minAreaRect(cnt)
        box = cv2.boxPoints(rect)
        box = np.int0(box)
        boxes.append(box)
        
    return boxes


def drawBoxes(img, blobs, minPixels=1000, maxFraction=0.25, minFraction=0.0015):
    '''returns a set of contour corners for a parametrized blobs inside an image''' 
    boxes = findBoxes(blobs, minPixels, maxFraction, minFraction)
    corners=[]
    for box in boxes:
        pts = np.array(box, np.int32)
        pts = pts.reshape((-1,1,2))
        cv2.polylines(img,[pts],True,(0,0,220),4)
        corners.append(box_corners(box))
    return img, corners


def overlayCanvas(img):
    '''returns new bigger canvas with original-sized image in the center'''
    height, width = img.shape[:2]
    diagonal = int(np.ceil(np.sqrt(width**2 + height**2)))
    longer_side = max(height, width)
    shorter_side = min(height, width)
    diagonal = longer_side*2
    
    blank_canvas = np.ones((diagonal,diagonal,3), np.uint8)*200
    x_offset = int((diagonal-width)/2)
    y_offset = int((diagonal-height)/2)
    x1, x2 = x_offset, x_offset + width
    y1, y2 = y_offset, y_offset + height
    blank_canvas[y1:y2,x1:x2] = img
    return blank_canvas, x_offset, y_offset


def distance(side):
    '''returns distance between two ends of a side'''
    point1, point2 = side
    return np.sqrt((point1[0] - point2[0])**2 + (point1[1] - point2[1])**2)


def findMargins(coords_x, coords_y, margin=0.1, minRatio=6, maxRatio=15, checkDiscard=False):
    '''checks whether the sides proportion is reasonable and returnes margin sizes'''
    width = coords_x[1] - coords_x[0]
    height = coords_y[1] - coords_y[0]
    minProportion = 1.0/maxRatio
    maxProportion = 1.0/minRatio
    proportion = 1.0*width/height
    discardFlag = False
    
    y_extension = margin*(proportion-minProportion)/(maxProportion-minProportion)
    x_extension = margin - y_extension
    
    if proportion < minProportion or proportion > maxProportion:
        if checkDiscard==True:
            print("Image DISCARDED!!!")
            discardFlag = True
            x_margin = 0
            y_margin = 0
        else:
            x_margin = int(0.05*height/2)
            y_margin = int(0.05*height/2)
    else:
        x_margin = int(x_extension*height/2)
        y_margin = int(y_extension*height/2)
    
    return x_margin, y_margin, discardFlag


def cropBoxes(img, x_offset, y_offset, corners, directory, namePrefix, refiningFlag):
    '''handles finding the center of a countour along with its margins and the angle of rotation, saves the cropped contours'''
    if refiningFlag==False:
        addNumbering=True
        checkDiscard=False
        minRatio=7
        maxRatio=15
    else:
        addNumbering=False
        checkDiscard=True
        minRatio=5
        maxRatio=60

    for c,i in zip(corners, range(len(corners))):
        side1 = np.array([c[0], c[1]])
        side2 = np.array([c[1], c[2]])
        side3 = np.array([c[2], c[3]])
        side4 = np.array([c[3], c[0]])
        sides = np.array([side1, side2, side3, side4])
        
        sorted_ixs = np.argsort(np.array([distance(s) for s in sides]))
        longest_side = np.ceil(distance(sides[sorted_ixs[-1]]))
        longest_opposite = np.ceil(distance(sides[sorted_ixs[-3]]))
        
        cent1 = np.mean(sides[sorted_ixs[-3]], axis=0).astype(int)
        cent2 = np.mean(sides[sorted_ixs[-4]], axis=0).astype(int)
        center = (np.mean([cent1, cent2],axis=0)).astype(int)
        
        crop_coords_x = np.array([np.floor(center[0]-longest_opposite/2+x_offset), np.ceil(center[0]+longest_opposite/2+x_offset)]).astype(int)
        crop_coords_y = np.array([np.floor(center[1]-longest_side/2+y_offset), np.ceil(center[1]+longest_side/2+y_offset)]).astype(int)
        
        if distance(side2) > distance(side3): # acute rotation angle
            angle = np.pi-np.arccos((c[2][0]-c[1][0])/distance(side2))
        else: # obtuse rotation angle or right angle
            angle = np.arccos((c[3][0]-c[2][0])/distance(side3))
        
        angle = 90 - angle/np.pi*180 # radians to degrees
        #print(angle)
        #cv2.circle(img,(c[0][0]+x_offset, c[0][1]+y_offset), 5, (0,0,255), -1) #top or top_left - red
        #cv2.circle(img,(c[1][0]+x_offset, c[1][1]+y_offset), 5, (255,0,0), -1) #left or bottom_left - blue
        #cv2.circle(img,(c[2][0]+x_offset, c[2][1]+y_offset), 5, (0,255,0), -1) #bottom or bottom_right - green
        #cv2.circle(img,(c[3][0]+x_offset, c[3][1]+y_offset), 5, (255,0,255), -1) #right or top_right - pink
        #cv2.circle(img,(center[0]+x_offset, center[1]+y_offset), 10, (0,0,0), -1) #center - black
    
        M = cv2.getRotationMatrix2D((center[0]+x_offset,center[1]+y_offset),angle,1)
        cols,rows = img.shape[:2]
        rotated = cv2.warpAffine(img,M,(cols,rows))
        
        #cv2.circle(img,(crop_coords_x[1], crop_coords_y[1]), 10, (0,0,0), -1) #center - black
        #cv2.circle(img,(crop_coords_x[0], crop_coords_y[0]), 10, (0,0,0), -1) #center - black
        #cv2.circle(img,(crop_coords_x[1], crop_coords_y[0]), 10, (0,0,0), -1) #center - black
        #cv2.circle(img,(crop_coords_x[0], crop_coords_y[1]), 10, (0,0,0), -1) #center - black
            
        x_margin, y_margin, discardFlag = findMargins(crop_coords_x, crop_coords_y, 0.1, minRatio, maxRatio, checkDiscard)
        if discardFlag==True:
            continue

        #print(img.shape[:2])
        #print(crop_coords_y[0],y_margin,crop_coords_y[0]-y_margin, crop_coords_y[1]+y_margin, crop_coords_x[0]-x_margin,crop_coords_x[1]+x_margin)
        
        cropped = rotated[crop_coords_y[0]-y_margin:crop_coords_y[1]+y_margin, crop_coords_x[0]-x_margin:crop_coords_x[1]+x_margin]
        if addNumbering == True:
            print("Writing to " + directory+"/"+namePrefix+str(i)+".jpg")
            cv2.imwrite(os.path.join(directory, namePrefix+str(i)+".jpg"), cropped)
            #cv2.imwrite(os.path.join(directory, namePrefix+str(i)+".jpg"), cropped)
        else:
            print("Writing to " + directory+"/"+namePrefix+".jpg")
            cv2.imwrite(os.path.join(directory, namePrefix+".jpg"), cropped)
            #cv2.imwrite(os.path.join(directory, namePrefix+".jpg"), cropped)
        
    return True


def cropBlobs(source_postfix, result_dir):
    '''wrapping function taking care of proper directories handling and time measurement'''
    source_dir = '/'.join(source_postfix)
    source_imgs = [os.path.join(source_dir, name) for name in os.listdir(source_dir) if os.path.isfile(os.path.join(source_dir, name)) and os.path.splitext(os.path.basename(name))[1]=='.jpg']
    
    current_directory = os.getcwd()
    results_directory = os.path.join(current_directory, result_dir, '/'.join(source_postfix[1:]))
    #markups_directory = os.path.join(current_directory, markup_dir, '/'.join(source_postfix[1:]))
    #print(results_directory, source_postfix, '/'.join(source_postfix[1:]))
    
    if len(source_imgs)>0 and not os.path.exists(results_directory):
        os.makedirs(results_directory)
        #os.makedirs(markups_directory)
    
    for src_img in source_imgs:
        
        start = time.time()
        
        print("Cropping '" + src_img +"' ...")
        src_img_filename = os.path.splitext(os.path.basename(src_img))[0]
        target_directory = r'res_' + src_img_filename
        final_directory = os.path.join(results_directory, target_directory)
        #final_markup_dir = os.path.join(markups_directory, target_directory)
        if not os.path.exists(final_directory):
            os.makedirs(final_directory)
            #os.makedirs(final_markup_dir)
        
        original = cv2.imread(src_img) # reading the original image
        blobs = gray_blobs(original)
        #blobs = hsv_blobs(original)
        margin_blobs = dilate(blobs) # dilate the blobs in order to set margins before cropping
        smooth_blobs = cv2.medianBlur(margin_blobs, 27) # blur the blobs in order to smoothen the edges
        closed_blobs = dilate(smooth_blobs,4) # dilate a bit again in order to close possible gaps within blobs
        #boxes = find_boxes(closed_blobs)
        #_,_,dtctd = detect_burs(blobs)
        
        overlayed, x_offset, y_offset = overlayCanvas(original)
        marked_overlay = overlayed.copy()
        marked_up, corners = drawBoxes(original, closed_blobs)#, minPixels=1000, maxFraction=0.2, minFraction=0.0015)  
        cropBoxes(overlayed, x_offset, y_offset, corners, final_directory, namePrefix="bur_", refiningFlag=False)
        #cropBoxes(marked_overlay, x_offset, y_offset, corners, final_markup_dir, namePrefix="bur_", refiningFlag=False)
        
        end = time.time()
        print("Cropping time for {}x{}: {:.2f} sec".format(original.shape[0], original.shape[1], end-start))
    
    return True