# -*- coding: utf-8 -*-
"""
Created on Thu Dec  2 22:17:29 2021

@author: dakot
"""

import requests
import data_input
#"http://127.0.0.1:5000/predict"
"https://sb-house-estimator.herokuapp.com/predict"
URL="https://test-sb-flask-app.herokuapp.com/predict"
headers={"Content-Type": "application/json"}
data={"input":data_input.data_in}
r=requests.get(URL, headers=headers, json=data)

#print(r)
print(r.json())