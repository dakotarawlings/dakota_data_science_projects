/* 
@author: dakota

this script file defines behaviors for the main (index.html) html file called 
*/

//Define the URL for the flask API endpoint for the tool image classification model
const API_URL="/predict";


//create a function to use the inputed image file to call the flask API and display the predictions on the HTML file
function onSubmit() {
    
    //var canvasElement = document.getElementById("canvas");
    //var ctx = a.getContext("2d");
    //var ImageData = ctx.getImageData(0,0, canvasElement.width, canvasElement.height);
    //console.log(ImageData);  
    
    /*

    
    /*
    canvasData=canvas.toDataURL('image/jpeg', 1.0);
 
    const formData=new FormData()  
    formData.append('media', canvasData)     
    */
    
    var canvasElement = document.getElementById("canvas");
    canvasBlob=canvasElement.toBlob(function(blob){ 
        console.log(blob);
        URLCanvas = URL.createObjectURL(blob,{type: 'text/plain'});
        console.log(URLCanvas);
        const formData=new FormData()  
        formData.append('media', blob)    
        PostRequest(formData)      
        
        
        }, 'image/jpeg')
             
    
}

function PostRequest(formData){

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
            console.log(data)
            //get the class prediction and the accuracy from the API response
            predictedChar=data['predictions'][0]['label'];
            updatePrediction(predictedChar)
            //probability=data['predictions'][0]['probability'];
            
            
             }))

}

//Simple function that takes in the class prediction from the API and updates the HTML element that displays the class
function updatePrediction(prediction) {
    
    var predictionElement=d3.select('#prediction');
    
    txtcolor='Orange';
    
    var predictionTemplate=`Character: <span style= "font-weight: bold; background: ${txtcolor}; color: white;"> ${prediction}</span>`;
    
    predictionElement.html(predictionTemplate)

}


//Simple function to empty the HTML elements that display the model predictions
//MEant to be called when the user inputs a new image before the user presses submit
function resetPredictions() {
  
    var predictionElement=d3.select('#prediction');
    
    predictionElement.html('')
    
    

}


// wait for the content of the window element
// to load, then performs the operations.
// This is considered best practice.
window.addEventListener('load', ()=>{
        
    //resize(); // Resizes the canvas once the window loads
    document.addEventListener('mousedown', startPainting);
    document.addEventListener('mouseup', stopPainting);
    document.addEventListener('mousemove', sketch);

    
    const clearButton = document.getElementById('erase')
    clearButton.addEventListener('click', clearCanvas);
    
});



    
const canvas = document.querySelector('#canvas');
   
// Context for the canvas for 2 dimensional operations
const ctx = canvas.getContext('2d');
ctx.fillStyle = 'white';

ctx.fillRect(0, 0, canvas.width, canvas.height);



function clearCanvas(event){
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.fillRect(0, 0, canvas.width, canvas.height);
}    

    
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
    
  ctx.lineWidth = 20;
   
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





