# This module is the main and launches the whole program

from img_processing import *
from bur_detection import *

# Please define here paths to the source picture directory, and (optionally) to the resulting output directories:
source_dir = r'originals'
cropped_dir = r'cropped'
markup_dir = r'markups'
refined_dir = r'refined'

#print(cv2.__version__)

def run(source_dir, cropped_dir, markup_dir):
    '''walks through source directory and puts its pictures into the processing pipeline'''
    for root, subdirs, files in os.walk(source_dir): 
        cropBlobs(root.split(os.sep), cropped_dir)
        refineCropped(cropped_dir, refined_dir, markup_dir)
    return True


start = time.time()
run(source_dir, cropped_dir, markup_dir)
end = time.time()
print("Cumulated time: {:.2f} sec".format(end - start))