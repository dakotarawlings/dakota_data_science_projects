# Santa Barbara Real Estate Price Estimator

## Overview
* Built a web scraper from scratch to scrape data from ~1500 sinlge family homes that have been sold over the past 2 years in Santa Barbara from trulia.com and created SQLite database
* Engineered features from house description to quantify the value of certain house attributes such as "ocean views", "pool", "fireplace", etc. 
* Optimized hyper parameters for linear, lasso, random forest, LightGBM, Catboost, and XGboost
* Built a flask API for estimating a house price based on features and hosted API on Heroku

## Resources
**Python version:** 3.8
**Packages:** sqlite3, pandas, numpy, sklearn, Catboost, LightGBM, seaborn, requests, beautifulsoup, flask, pickle
**Flask api reference:** https://towardsdatascience.com/productionize-a-machine-learning-model-with-flask-and-heroku-8201260503d2

## Web Scraping
* Built a web scraper from scratch using python and beautiful soup to scrape data for ~1500 sinlge family homes that have been sold in the past 2 years off of trulia.com
* Extracted features from each listing such as:
  - Closing price
  - Year built
  - Lot area
  - House sqft
  - Number of bedrooms/bathrooms
  - House description
  - House address
  - Sold date
* Stored raw data in SQLite database

## Data Cleaning and Feature Engineering
* Extracted features from the house description, such as the presence of key words/phrases such as "pool", "ocean views", "mountain views", "fireplace", etc.
* Converted "year built" to house age
* Extracted the zipcode from the address as a categorical variable
* Used one hot encoding to create dummy variables for categorical variables such as the zipcode
* Used a KNN regressor to impute missing values 
* Rescaled several variables to ensure that most features had similar scales
* Normalized the distribution of several variables with skewed distributions

