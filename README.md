# Dakota Rawlings Personal Data Science Projects

## Overview

This repository contains my most recent personal data science projects. Here I am showcasing a few projects that show a variety of different skills including model productionalization, incorporation of models into full stack web apps, web scraping, data cleaning, feature engineering, visualization, shallow model implementation, hyper parameter tuning, model interpretation, neural network implementation, image classification, and more. 


## [1. Tool image classifier web application](/tool_image_classification)

<p align="center">
  <img src="/tool_image_classification/readme_images/homepage2.png" width="500" >
  <br>
  <em>Web App Link: https://tool-image-classifier.herokuapp.com/</em>
  </br>
</p>

* Built a web scraper that implements the Bing image search API to scrape ~4000 images of 10 different tool classes
* Implemented cleaning and preprocessing techniques such as image augmentation, image rescaling, and image reformatting
* Compared the performance of two different CNN models, one with a custom convolutional base and head and a second that implements trasfer learning with the MobileNetV2 pretrained base, which achieved an accuracy of 95%
* Built a flask API endpoint that recieves an image from a post request, implements the model, and returns an image class and a probability 
* Wrote a web application in HTML, CSS, and JavaScript to call API and display the class prediction
* Deployed web app on Heroku: https://tool-image-classifier.herokuapp.com/


## [2. Housing price estimator web application](/santa_barbara_realestate_analysis)

<p align="center">
  <img src="santa_barbara_realestate_analysis/readme_pictures/homepage2.png" width="500" >
  <br>
  <em>Web App Link: https://sb-house-price-estimator.herokuapp.com/</em>
  </br>
</p>

* Built a web scraper from scratch to scrape data from ~1500 realestate sales over the past 4 years in Santa Barbara from trulia.com. Created an associated SQLite database
* Implemented various feature engineering and cleaning strategies such as scaling, normalization, transformation, one hot encoding, KNN imputation, etc
* Engineered features from house description text to quantify the value of certain house attributes such as "ocean views", "pool", "fireplace", etc. 
* Optimized hyper parameters for linear, lasso, random forest, LightGBM, Catboost, and XGboost models and chose highest performing model
* Built a flask API endpoint that implements the model to estimate a house price based on features
* Wrote a web application in HTML, CSS, and JavaScript to call API and display price prediction based on user input
* Deployed web app on Heroku: https://sb-house-price-estimator.herokuapp.com/




## Learn More about me

You can learn more about me at the following links

1. [LinkedIn profile](https://www.linkedin.com/in/dakotarawlings/)
2. [Google Scholar profile](https://scholar.google.com/citations?view_op=list_works&hl=en&hl=en&user=X_x46vUAAAAJ)
3. [Reseach group website](http://www.segalman.mrl.ucsb.edu/)
