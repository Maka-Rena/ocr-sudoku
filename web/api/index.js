// load express
const express = require('express');
const app = express();
const { exec } = require('child_process');
const fs = require("fs");

app.use(express.json());
//GET /?color1=red&color2=blue

function process (filename, type, n) {
  console.log("./build/sudoc ./pictures/" + filename + ' ' + n + ' ' + type);
  exec('cd ../../ && ./build/sudoc ./pictures/' + filename + ' ' + n + ' ' + type, (error, stdout, stderr) => {
    if (error) {
      console.log(`error: ${error.message}`);
      return;
    }
    if (stderr) {
      console.log(`stderr: ${stderr}`);
      return;
    }
    console.log(`stdout: ${stdout}`);
  });
}

app.get("/", (req, res) => {
  const { filename, handwritted, hexa } = req.query;
  res.header("Access-Control-Allow-Origin", "*");
  console.log("FILE : " + filename);
  console.log("Is handwritted : " + handwritted);
  console.log("IS Hexa : " + hexa);
  let n = (hexa === "true") ? 16 : 9;
  let type = (handwritted === "true") ? 0 : (hexa === "true" ) ? 2 : 1;
  process(filename, type, n);
  res.send("OK");
  res.end();
});

app.get("/step", (req, res) => {
  res.header("Access-Control-Allow-Origin", "*");
  const { currentStep } = req.query;
  //Check if file path : ../website/src/components/assets/process/currentStep+1.jpeg exists
  //If yes, send currentStep+1
  //If no, send currentStep
  let step = currentStep;
  if (currentStep < 3 && fs.existsSync("../website/src/components/Upload/process/" + (parseInt(currentStep) + 1) + ".jpeg")) {
    step = parseInt(currentStep) + 1;
  }
  else{
    if (fs.existsSync("../website/src/components/Upload/process/solvedSudoku.json"))
    {
      step = 6;
    }
  }
  console.log("STEP sent: " + step);
  res.send({ step: step });
  res.end();
});

app.get("/result", (req, res) => {
  res.header("Access-Control-Allow-Origin", "*");
  let rawdata = fs.readFileSync('../website/src/components/Upload/process/solvedSudoku.json');
  let result = JSON.parse(rawdata);
  console.log(result["sudoku"]);
  res.send(result["sudoku"]);
  res.end();
});

app.listen(3001, () => { 
    console.log('Started on port 3001');
});

//Path API: http://localhost:3000/upload

//Key: file, Value: [name].jpeg
//Key: description, Value: [description]