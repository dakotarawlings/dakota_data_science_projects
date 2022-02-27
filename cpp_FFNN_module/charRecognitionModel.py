# -*- coding: utf-8 -*-
"""
Created on Wed Feb 23 11:36:00 2022

@author: dakot
"""
import numpy as np

import pandas as pd

import matplotlib.pyplot as plt

try:
    import FFNN_pymodule
except:
    pass

import time

import pickle

from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix



data_path=r"training_data/MNIST_train.csv"

full_df=pd.read_csv(data_path)

df=full_df.sample(n=8000, random_state=2)

#make this into a grid with images and identifiers
imageTest=df.iloc[1,1:]
plt.imshow(np.array(imageTest).reshape(28,28))

x=df.iloc[:,1:]
y=df.iloc[:,0]

xtrain, xtest, ytrain, ytest=train_test_split(x,y,test_size=0.2, shuffle=False, random_state=1)

#xtrain_formatted=reshapeTrainData(xtrain, 1, 784)
#ytrain_formatted=reshapeTrainData(ytrain_dummies,0,9)

xtrain=xtrain.astype('float32')
ytrain=ytrain.astype('float32')

xtrain/=255

xtrain_formatted=xtrain.values

ytrain_dummies=pd.get_dummies(ytrain,prefix='dummy')

ytrain_formatted=ytrain_dummies.values



seconds1 = time.time()

model=FFNN_pymodule.FFNN_Builder()
model.setTopology([784, 32, 10],'sigmoid')

model.fitModel(xtrain_formatted, ytrain_formatted,1)


seconds2 = time.time()
print("total time", (seconds2-seconds1)/60)


img=np.random.randint(0,20000)

imageTest=full_df.iloc[img,1:]
catTest=full_df.iloc[img,0]
testImage=np.array(imageTest)
testImage=testImage.astype('float32')
testImage/=255
#print(testImage)

print('test image:', catTest)
print(np.array(model.predict(testImage)).argmax())
testImage.reshape(28,28)

testImage.reshape(1,-1)

print('after transformation')
print(np.array(model.predict(testImage)).argmax())






'''
'''










pickle.dump(model,open('model_file.p',"wb"), pickle.HIGHEST_PROTOCOL)

seconds1 = time.time()

model2=pickle.load(open('model_file.p','rb'))
print('now from pickle:')

img=np.random.randint(0,20000)

imageTest=full_df.iloc[img,1:]
catTest=full_df.iloc[img,0]
testImage=np.array(imageTest)
testImage=testImage.astype('float32')
testImage/=255


print('test image:', catTest)
print(np.array(model2.predict(testImage)).argmax())

seconds2 = time.time()
print("Pickle time:", (seconds2-seconds1)/60)

'''


'''
