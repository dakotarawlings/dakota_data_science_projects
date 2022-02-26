# -*- coding: utf-8 -*-
"""
Created on Mon Feb  7 21:11:50 2022

@author: dakota

This script defines a flask application that implements a tool image classification
model on an image file sent via a POST request and returns a class prediction

"""


#import flask tools
from flask import Flask, jsonify, render_template, request


#Import image an file processing tools
from PIL import Image, ImageOps
import io
import numpy as np
import pickle

#Call flask constructor
app=Flask(__name__)

#defin a function that loads the saved tensorflow model
def load_model():
    #Load the model structure from model json file
    
    loaded_model=pickle.load(open('model/model_file.p','rb'))

    return loaded_model

def format_image(image_data):
    img=Image.open(io.BytesIO(image_data))
    bw_img=img.convert(mode='L')
    inv_img = ImageOps.invert(bw_img)
    
    bbox = inv_img.getbbox()
    crop=inv_img.crop(bbox)
    new_size=max(abs(bbox[2]-bbox[0]),abs(bbox[3]-bbox[1]))
    delta_w=new_size-crop.size[0]
    delta_h = new_size - crop.size[1]
    padding = (delta_w//2, delta_h//2, delta_w-(delta_w//2), delta_h-(delta_h//2))
    new_im = ImageOps.expand(crop, padding)
    newsize = (28, 28)
    im28 = new_im.resize(newsize)
    im28 = ImageOps.expand(im28, (5,5,5,4))
    newsize = (28, 28)
    im28 = im28.resize(newsize)
    
    array=np.array(im28)
    array1D=array.reshape(1,-1)
    array1D=array1D.astype('float32')
    array1D/=255
    return np.array(array1D[0])
    #return list(np.array(array1D[0]))

#Define flask endpoint for the main html page
@app.route('/')
def index():
    
    return render_template('index.html')


#define an API endpoint that takes in an image file from a post reqest and returns
# a class prediction and an accuracy
@app.route('/predict', methods=['GET', 'POST'])
def predict():
    
    #monitor the success of the API through a success attribute
    response={'success': False}
    
    print("predict called")
    
    #Check for a post request    
    if request.method=='POST':
        print("POST called")
        
        #Check for a media attribute in the json input
        if request.files.get('media'):
            
            print('media found')
            
            #retrieve the file sent by the post request
            img_requested=request.files['media'].read()
            formatted_img=format_image(img_requested)
            
            model=load_model()
            
            #print("image array")
            #print(formatted_img)
            prediction=np.array(model.predict(formatted_img))
            
            #print("raw output")
            #print(prediction)
            
            prediction=str(prediction.argmax())
            
            #print(prediction)
            
            response['predictions']=[]
            
            
            pred={'label':prediction}
            response['predictions'].append(pred)
            
            response['success']=True
            
            
    return jsonify(response)
   
if __name__=='__main__':
    app.run(debug=False)
    
      
    
    
    
    
    
    
    