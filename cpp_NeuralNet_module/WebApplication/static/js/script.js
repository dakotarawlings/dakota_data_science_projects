/* 
@author: dakota

this script file defines behaviors for the main (index.html) html file called 
*/

//Define the URL for the flask API endpoint for the tool image classification model
const API_URL="/predict";

//Define a function to verify the user inputed file type and diplay the image on the HTML upon file upload
function onUpload() {
    
    //retieve the image from the file input HTML element
    const selectedFile = document.getElementById('toolFile').files[0];
    
    //Use our functions defined below to check the filetype and throw an errow/reload the page if the filetype is not correct
    checkRealMimeType(selectedFile)
    resetPredictions()
    
    //Call our empty HTML element that will display the uploaded image
    var uploadedImageElement=d3.select('#output');
    
    // create a temporary url for the image, input the URL into an image element, and input the element into our empty HTML container
    URLImage = URL.createObjectURL(selectedFile,{type: 'text/plain'});
    var ImageTemplate=`<img src=${String(URLImage)} alt="Selected Image" class="uploadedIm"/>`;
    uploadedImageElement.html(String(ImageTemplate))
}

//create a function to use the inputed image file to call the flask API and display the predictions on the HTML file
function onSubmit() {
    
    // get the image file from the file input HTML element
    const selectedFile = document.getElementById('toolFile').files[0];
    
    //create a temporary URL witht the file 
    URLImage = URL.createObjectURL(selectedFile,{type: 'text/plain'});

    //Define a form containing the image file to be sent to the flask API
    const formData = new FormData()
    formData.append('media', selectedFile)
    
    //Send a post request to the flask API with the image file
    fetch(API_URL, {
      method:"POST",
      body: formData,
      headers: {
        "Accept-Encoding": "*",
        "Connection": "keep-alive"
      }
      //define functions that handle API response
        }).then(response => response.json()
        .then(function(data) {
            //get the class prediction and the accuracy from the API response
            predictedClass=data['predictions'][0]['label'];
            probability=data['predictions'][0]['probability'];
            
            //call our classToTool function defined below to clean up the class name
            className=classToToolName(predictedClass);
            
            //call our GetRecomendation function to get a text element and formatting for our "suggestio" element that gives the user a suggestion if the prediction is not good
            [txtcolor, suggestionTemplate]=GetReccomendation(probability);
            
            //Call our function that updates the prediction element in the HTML
            updatePrediction(className, txtcolor);
            
            //Call our function that updates the prediction accuracy in the HTML
            updateProbability(probability,txtcolor, suggestionTemplate);
            
             }))
    
}

//Simple function that takes in the class prediction from the API and updates the HTML element that displays the class
function updatePrediction(prediction, txtcolor) {
    
    var predictionElement=d3.select('#prediction');
    
    var predictionTemplate=`Name of tool: <span style= "font-weight: bold; background: ${txtcolor}; color: white;"> ${prediction}</span>`;
    
    predictionElement.html(predictionTemplate)

}

//Simple function that takes in the accuracy of the prediction, a font color, and a suggestion and updates the HTML elements that displays the accuracy and a suggestion (when relevent)
function updateProbability(probability, txtcolor, suggestionTemplate) {
    
    var probabilityElement=d3.select('#probability');
    
    var suggestionElement=d3.select('#suggestion');
    
    var probabilityTemplate=`Accuracy: <span style= " font-weight: bold; color: ${txtcolor};"> ${Math.round(probability)}%</span>`;
    
    probabilityElement.html(probabilityTemplate)
    
    suggestionElement.html(suggestionTemplate)

}




//Simple function to empty the HTML elements that display the model predictions
//MEant to be called when the user inputs a new image before the user presses submit
function resetPredictions() {
    
    var probabilityElement=d3.select('#probability');
    
    var suggestionElement=d3.select('#suggestion');
    
    var predictionElement=d3.select('#prediction');
    
    probabilityElement.html('')
    
    suggestionElement.html('')
    
    predictionElement.html('')
    
    

}


// wait for the content of the window element
// to load, then performs the operations.
// This is considered best practice.
window.addEventListener('load', ()=>{
        
    resize(); // Resizes the canvas once the window loads
    document.addEventListener('mousedown', startPainting);
    document.addEventListener('mouseup', stopPainting);
    document.addEventListener('mousemove', sketch);
    window.addEventListener('resize', resize);
});
    
const canvas = document.querySelector('#canvas');
   
// Context for the canvas for 2 dimensional operations
const ctx = canvas.getContext('2d');
    
// Resizes the canvas to the available size of the window.

/*
function resize(){
  ctx.canvas.width = window.innerWidth;
  ctx.canvas.height = window.innerHeight;
}
*/
    
// Stores the initial position of the cursor
let coord = {x:0 , y:0}; 
   
// This is the flag that we are going to use to 
// trigger drawing
let paint = false;
    
// Updates the coordianates of the cursor when 
// an event e is triggered to the coordinates where 
// the said event is triggered.
function getPosition(event){
  coord.x = event.clientX - canvas.offsetLeft;
  coord.y = event.clientY - canvas.offsetTop;
}
  
// The following functions toggle the flag to start
// and stop drawing
function startPainting(event){

  paint = true;
  getPosition(event);
}
function stopPainting(){
  paint = false;
}
    
function sketch(event){
  if (!paint) return;
  ctx.beginPath();
    
  ctx.lineWidth = 10;
   
  // Sets the end of the lines drawn
  // to a round shape.
  ctx.lineCap = 'round';
    
  ctx.strokeStyle = 'black';
      
  // The cursor to start drawing
  // moves to this coordinate
  ctx.moveTo(coord.x, coord.y);
   
  // The position of the cursor
  // gets updated as we move the
  // mouse around.
  getPosition(event);
   
  // A line is traced from start
  // coordinate to this coordinate
  ctx.lineTo(coord.x , coord.y);
    
  // Draws the line.
  ctx.stroke();
}



//Event listener for when the user clicks submit
d3.select("#button").on("click",onSubmit);





