
# Custom deep learning library in C++

<p float ="left" align="center">
  <img style="padding-right: 100px;" src="/cpp_FFNN_module/readme_pictures/video.gif" height="300" >
</p>

### Web App Link: https://char-recognition-cpp.herokuapp.com/

## Overview
* Wrote a feed forward neural network model-building library from scratch in C++ using only standard libraries
* Used pibind11 to build a wrapper for the C++ library to create a python DLL 
* Tested the library by using it to build a model to classify images of handwritten digits (trained using the MNIST dataset)
* Used the model to build a digit identifier webapp from scratch in python, flask, javascript, html, and css
* Deployed web app on Heroku: https://char-recognition-cpp.herokuapp.com/
* All development for this project was completed on a linux kernal via WSL2 

## Resources
**Python version:** 3.8

**Packages/software:** pibind11, linux/WSL2/Bash, pandas, numpy, flask, pickle, pillow

**Languages:** C++, python, JavaScript, HTML, CSS

## C++ Library/DLL documentation
* ## General syntax:
  - ### ***model=FFNN_pymodeule.FFNN_Builder()*** --> Create an instance of a FF neural net
    - **Member variables:**
      - modelTopology -> List[int]
      - epochs -> int
      - activationType -> string
  - ### ***model.setTopology( List[int] )*** --> Set the topology of the net:
    - **Parameters:**
      - List[int]: 1D list of ints, where each int indicates the width (number of neurons) in a layer. The first int in the list is the input layer (where the number of neurons should be equal to the number of features) and the last int is the output layer (where the number of neurons should be equal to the number of desired outputs)
  - ### ***model.setActivationFunction( string )*** --> Set the type of activation function for all neurons 
    - **Parameters:**
      - string: 'reLu', 'sigmoid', or 'tanh'
  - ### ***model.setEpochs( int )*** --> Set the number of epochs for training
    - **Parameters:**
      - int: number of epochs (default is 1)
  - ### ***model.fitModel( X: List[[double]], Y: List[[double]] )*** --> Fit the model to training data
    - **Parameters:**
      - X: List[[double]]: 2D python list or numpy array of features
      - Y: List[[double]]: correspondint 2D python list or numpy array of targets
  - ### ***model.predict( X: List[double] )-> List[double]*** --> Apply the model to testing data
    - **Parameters:**
      - X: List[[double]]: 1D python list or numpy array representing one sample of features
    - **Outputs:**
      - List[[double]]: 1D python list or numpy array representing the output 
* ## Importing the library
  - The easiest way to import the library is to include the .so object file in the same directory as your python file. 
    - NOTE: If you are not using linux, you will need to run cmake to re-build a DLL in the correct format for your system
* ## Example

```
import FFNN_pymodule
model = FFNN_pymodule.FFNN_Builder()
model.setTopology([2, 4, 1])
model.setActivationFunction('sigmoid')
model.setEpochs(20)
x_train=[[1,1],[1,0],[0,1],[0,0]]
y_train=[[0],[1],[1],[0]]
model.fitModel(x_train, y_train)
model.predict([1, 0])
>>>[1]
```

## Testing/demostrating the Library -> MNIST data model
