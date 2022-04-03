# This module constitutes of basic functions for initial image processing

import numpy as np
import cv2
from scipy import stats
import time

BLACK = 0
WHITE = 255


def image_meta(img):
    '''Extract image metadata (mean, mode, median) from its thumbnail'''
    proportion = 1.0*img.shape[0]/img.shape[1]
    small_size = 120
    small_burs = cv2.resize(img, (small_size, int(small_size*proportion)))
    small_burs = cv2.blur(small_burs,(10,10))
    
    mean = np.mean(small_burs).astype(int)
    mode,_ = stats.mode(small_burs.ravel())
    mode = mode[0]
    median = np.median(small_burs).astype(int)
    
    return mean, mode, median


def close_img(img):
    '''performing morphological closure on the input image'''
    imgSize = img.shape[0] * img.shape[1]
    kernelSize = int(imgSize**(0.245)/5)
    close_kernel = np.ones((kernelSize,kernelSize),np.uint8)
    return (cv2.morphologyEx(img, cv2.MORPH_CLOSE, close_kernel))


def coarse_filter(img):
    '''filters the image based on its metadata'''
    img = close_img(img)
    mean, mode, median = image_meta(img)
    coarse_formula = (3*mean+mode+3*median)/7
    return np.where(img<coarse_formula, img, 0)


#def increase_contrast(img, contrast_factor):
    #'''increases image contrast by an exponential function'''
    #print("CONTRAST", contrast_factor, np.mean(255*(img/255)**contrast_factor))
    #return (255*(inverse(img)/255)**contrast_factor).astype('uint8')


def averaging(img, averaging_factor):
    '''applying averaging kernel'''
    kernel = np.ones((averaging_factor,averaging_factor),np.float32)/averaging_factor**2
    return cv2.filter2D(img,-1,kernel)


def extract_background_mean(img, contrast_factor=1.7, averaging_factor=15, blockSize=111, C=7, blurSize=15):
    '''separates foregraound from background based on adaptive mean thresholding'''
    meta_filter = coarse_filter(img)
    #img = increase_contrast(img, contrast_factor)
    img = averaging(img, averaging_factor)
    img = cv2.adaptiveThreshold(img,WHITE,cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,blockSize,C)
    img = cv2.medianBlur(img, blurSize)
    img = np.where(meta_filter>BLACK, img, WHITE)
    return img


#def extract_background_gaussian(img, contrast_factor=2, averaging_factor=15, blockSize=351, C=9, blurSize=13):
    #'''separates foregraound from background based on adaptive gaussian thresholding'''
    #meta_filter = coarse_filter(img)
    #img = increase_contrast(img, contrast_factor)
    #img = averaging(img, averaging_factor)
    #img = cv2.adaptiveThreshold(img,WHITE,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,blockSize,C)
    #img = cv2.medianBlur(img, blurSize)
    #img = np.where(meta_filter>BLACK, img, WHITE)
    #return img


def inverse(img):
    '''inverses black to white and vice versa'''
    return (WHITE-img)


def dilate(img, divisionFactor=1):
    '''applying delation kernel'''
    #imgSize = img.shape[0] * img.shape[1]
    #kernelSize = int(imgSize**(0.22)/divisionFactor)
    kernelSize = int(24/divisionFactor)
    kernel = np.ones((kernelSize,kernelSize),np.uint8)
    return inverse(cv2.dilate(inverse(img),kernel,iterations = 1))


#def my_closing(img, dilateSize=10, erodeSize=11, smallErodeSize=3, iterations=4, smallIterations=1):
    #img = inverse(img)
    #dilate_kernel = np.ones((dilateSize,dilateSize),np.uint8)
    #erode_kernel = np.ones((erodeSize,erodeSize),np.uint8)
    #small_erode_kernel = np.ones((smallErodeSize,smallErodeSize),np.uint8)
    #for i in range(iterations):
        #img = cv2.erode(img,erode_kernel)
        #img = cv2.dilate(img,dilate_kernel)
    #img = cv2.erode(img,small_erode_kernel,iterations=smallIterations)
    #img = inverse(img)
    #return img


def gray_blobs(img):
    '''converting original image to grayscale'''
    gray_image = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) # converting the original image to grayscale
    blobs = extract_background_mean(gray_image) # convert to black blobs on wihte background using MEAN adaptive threshold
    #blobs = extract_background_gaussian(gray_image)  # convert to black blobs on wihte background using GAUSSIAN adaptive threshold
    return blobs

    
#def hsv_blobs(img):
    #'''converting original image to HSV'''
    #hsv_image = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    #hsv_image = np.reshape(hsv_image,(-1,3))
    #hue, saturation, value = hsv_image[:,0], hsv_image[:,1], hsv_image[:,2]
    #s_channel = np.reshape(saturation,(original.shape[:2]))
    #edged = inverse(cv2.Canny(s_channel, 25, 60))
    #averaged = averaging(edged,15)
    #closed = my_closing(averaged)
    #blobs = extract_background_mean(closed)
    ##blobs = extract_background_gaussian(closed)
    #return blobs