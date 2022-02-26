# -*- coding: utf-8 -*-
"""
Created on Fri Feb 25 18:21:14 2022

@author: dakot
"""

from flask import Flask, jsonify, render_template, request
import scipy.misc
import imageio

#Import image an file processing tools

from PIL import Image, ImageOps
import io
import numpy as np
import pickle
import pandas as pd

import matplotlib.pyplot as plt
import matplotlib.image as mpimg

def show(img, figsize=(8, 4), title=None):
    plt.figure(figsize=figsize)
    plt.imshow(img)
    if title:
        plt.title(title)
    plt.show()

# import hachoir
# from hachoir.parser import createParser
# from hachoir.metadata import extractMetadata

# filename = "C:\\path\\to\\file\\image.png"
# parser = createParser(filename)
# metadata = extractMetadata(parser)

# for line in metadata.exportPlaintext():
#     print(line)


dataPath=r"C:\Users\dakot\Desktop\DataScience\projects\dakota_portfolio\cpp_NeuralNet_module\WebApplication\TestCharachter.jfif"
img=Image.open(dataPath)
bw_img=img.convert(mode='L')
#show(bw_img)
print(bw_img.size)
print(bw_img.format)
print(bw_img.mode)
print(bw_img.width)
print(bw_img.height )
print(bw_img.palette)

inv_img = ImageOps.invert(bw_img)
bbox = inv_img.getbbox()
crop=inv_img.crop(bbox)
#show(crop)
new_size=max(abs(bbox[2]-bbox[0]),abs(bbox[3]-bbox[1]))
delta_w=new_size-crop.size[0]
delta_h = new_size - crop.size[1]
padding = (delta_w//2, delta_h//2, delta_w-(delta_w//2), delta_h-(delta_h//2))
new_im = ImageOps.expand(crop, padding)
#show(new_im)
newsize = (28, 28)
im28 = new_im.resize(newsize)
show(im28, title='cropped')


im28 = ImageOps.expand(im28, (5,7,5,4))
newsize = (28, 28)
im28 = im28.resize(newsize)
show(im28, title='border')


array=np.array(im28)
#print(array)

array1D=array.reshape(1,-1)
array1D=array1D.astype('float32')
array1D/=255

#print(array1D)










